# Lecture 7 — Sockets

## 1. Network Communication as IPC

If two processes are on **different machines**, pipes, shared memory, and message queues are no use — they must communicate over the **network**. The network also works between processes on the **same** machine (via `localhost` / `127.0.0.1`). We treat the network as a "cloud" and focus on *using* it, not how it works internally (that's a networks course).

## 2. Sockets

The **socket API** is a long-standing standard for network communication. A **socket** is an endpoint of a communication channel between two processes. Two styles:

| | **Datagram** (`SOCK_DGRAM`) | **Stream** (`SOCK_STREAM`) |
|-|------------------------------|-----------------------------|
| Analogy | Sending letters by mail | A phone call |
| Connection? | No — just deliver messages | Yes — connect, talk, hang up |
| Order | May arrive **out of order** | Bytes arrive **in order** |
| Reliability | May be **lost** | **Reliable** |
| Direction | Unidirectional (per message) | **Bidirectional** byte stream |
| Default protocol | **UDP** | **TCP** (TCP/IP) |

Like everything in UNIX, **a socket is handled like a file**: you get a **file descriptor** (`int`), and reads/writes on it are routed over the network.

### Creating a socket

```c
#include <sys/socket.h>
int socket( int domain, int type, int protocol );
```

- **`domain`** — address family / format. `AF_INET` = IPv4 (used in this course); `AF_INET6` = IPv6.
- **`type`** — `SOCK_STREAM` or `SOCK_DGRAM`.
- **`protocol`** — `0` = default for the type (TCP for streams, UDP for datagrams).
- Returns a **file descriptor**, or -1 on error.

Close it with plain **`close(fd)`** when finished. Why is opening type-specific (`open`, `pipe`, `socket`) but closing generic? When opening you must say *what* to open; when closing, the OS already knows.

## 3. Byte Order (Endianness)

Machines disagree about how multi-byte integers are laid out in memory. For the 32-bit value `0x0A0B0C0D`:

| Address | n | n+1 | n+2 | n+3 |
|---------|---|-----|-----|-----|
| **Big-endian** (most significant byte first) | `0A` | `0B` | `0C` | `0D` |
| **Little-endian** (least significant byte first) | `0D` | `0C` | `0B` | `0A` |

x86 is **little-endian**; others (e.g. PowerPC) are big-endian. You can't assume what the other side is, so network protocols define **network byte order = big-endian**, and everyone converts.

Conversion functions (`<arpa/inet.h>`) — use them **even if you think your machine is big-endian**, for portability:

```c
uint32_t htonl( uint32_t hostint32 );  /* host -> network, 32-bit (e.g. IP addresses) */
uint16_t htons( uint16_t hostint16 );  /* host -> network, 16-bit (e.g. ports) */
uint32_t ntohl( uint32_t netint32 );   /* network -> host, 32-bit */
uint16_t ntohs( uint16_t netint16 );   /* network -> host, 16-bit */
```

Mnemonic: **h**ost **to** **n**etwork **l**ong / **s**hort. The exact-width types `uint32_t`/`uint16_t` (from `<stdint.h>`) are used because byte swapping only makes sense if you know exactly how many bytes the integer has. On a big-endian host these are no-ops.

## 4. Addresses and Ports

### `struct sockaddr_in` (IPv4 address)

```c
struct sockaddr_in {
    sa_family_t    sin_family;  /* address family: AF_INET */
    in_port_t      sin_port;    /* port number, NETWORK byte order */
    struct in_addr sin_addr;    /* IPv4 address, NETWORK byte order (.s_addr) */
};

struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons( 2520 );
addr.sin_addr.s_addr = htonl( INADDR_ANY );
```

- **IP address**: IPv4 looks like `XXX.XXX.XXX.XXX`, each 0–255 (e.g. router `192.168.0.1`). Names like `uwaterloo.ca` are translated (DNS) to numbers like `129.97.208.23` — computers use numbers, humans prefer names.
- **`INADDR_ANY`** — "use whatever address(es) this machine has" (a machine may have several network interfaces). Handy for servers.
- **Port**: if the IP is a street address of an apartment building, the **port** is the apartment number. Different services (processes) use different ports; **no two processes can use the same port at the same time**.
  - Ports **below 1024** are reserved for system services and need superuser rights. Use ports **above 1024**.
  - **Well-known ports**: everyone agrees in advance, e.g. `ssh` = 22, HTTP = 80, HTTPS = 443. The `sshd` daemon listens on 22 and the `ssh` client connects there by default.

Functions like `bind`/`connect` take a generic `struct sockaddr *`, so you **cast** your `sockaddr_in *` to `struct sockaddr *` when passing it.

### Looking up addresses: `getaddrinfo`

You rarely know IP addresses; you know names (`ecelinux.uwaterloo.ca`). The command-line tool is `nslookup`; in C use **`getaddrinfo`** (`<netdb.h>`). The older `gethostbyname()` is **deprecated** — you may see it in the wild, but use the new way.

```c
int getaddrinfo( const char *node,              /* "www.example.com" or "129.97.208.23" */
                 const char *service,           /* "80" (prefer numbers) or "http" */
                 const struct addrinfo *hints,  /* restrict results, or NULL */
                 struct addrinfo **res );       /* OUT: linked list of results */
void freeaddrinfo( struct addrinfo *res );
```

Returns **0 on success** (non-zero error codes can be turned into text with `gai_strerror`).

```c
struct addrinfo hints;
struct addrinfo *serverinfo;               /* will point to the results */

memset( &hints, 0, sizeof hints );         /* make sure the struct is empty */
hints.ai_family = AF_INET;                 /* IPv4 */
hints.ai_socktype = SOCK_STREAM;           /* TCP */
hints.ai_flags = AI_PASSIVE;               /* fill in my IP for me */

int result = getaddrinfo( "www.example.com", "2520", &hints, &serverinfo );
if ( result != 0 ) {
    return -1;
}
struct sockaddr_in *sain = (struct sockaddr_in *) serverinfo->ai_addr;
/* Do things with this */
freeaddrinfo( serverinfo );
```

- `res` points to a **linked list** of `struct addrinfo`; each node's `ai_addr` is the address (a generic `struct sockaddr *` you may cast), `ai_addrlen` its length, `ai_next` the next result. Usually the first result is enough.
- Nodes also carry `ai_family`, `ai_socktype`, `ai_protocol` — exactly the arguments `socket()` needs.
- `node = NULL` (with `AI_PASSIVE`) gives an address for the **local machine** — useful for servers.
- `hints = NULL` accepts defaults.
- Always `freeaddrinfo` the list.

## 5. Client Side: `connect`

Everything so far (`socket`, building an address) is shared by clients and servers. Now they diverge. The client simply connects:

```c
int connect( int sockfd, struct sockaddr *addr, socklen_t len );
```

- `sockfd` — from `socket()`.
- `addr` — server's address (built manually or from `getaddrinfo`).
- `len` — `sizeof` your struct, or `res->ai_addrlen`.
- Returns **0 on success**, -1 on error with `errno` set — e.g. `ECONNREFUSED` (nobody listening), `ETIMEDOUT`. Check the man page (`man 2 connect`) for the list and compare against the **constants**, not raw numbers.

```c
struct addrinfo hints, *res;
int sockfd;

memset( &hints, 0, sizeof( hints ) );
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;

getaddrinfo( "www.uwaterloo.ca", "80", &hints, &res );
sockfd = socket( res->ai_family, res->ai_socktype, res->ai_protocol );
int status = connect( sockfd, res->ai_addr, res->ai_addrlen );
```

The client doesn't `bind`: it usually doesn't care which **outgoing** port it uses — the OS picks one.

## 6. Server Side: `bind`, `listen`, `accept`

Phone analogy: get a phone number (**bind**), turn the phone on (**listen**), pick up when it rings (**accept**).

### `bind` — claim a port

```c
int bind( int sockfd, const struct sockaddr *addr, socklen_t len );

int socketfd = socket( AF_INET, SOCK_STREAM, 0 );
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons( 2520 );
addr.sin_addr.s_addr = htonl( INADDR_ANY );
bind( socketfd, (struct sockaddr *) &addr, sizeof( addr ) );
```

Now port 2520 belongs to this process. (Fails with `EADDRINUSE` if someone else has it, `EACCES` for ports < 1024 without privileges.)

### `listen` — ready for connections

```c
int listen( int sockfd, int backlog );
```

Marks a bound socket as accepting connections. `backlog` = how many pending (not yet accepted) connections may queue up (system-limited, ~20 typical); beyond that, new requests are rejected.

### `accept` — take a call

```c
int accept( int sockfd, struct sockaddr *addr, socklen_t *len );
```

- `sockfd` — the listening socket.
- `addr`, `len` — **out** parameters filled with the **client's** address (pass `NULL, NULL` if you don't care). Note `len` is a *pointer*: set it to the buffer size before the call; it's updated afterward.
- **Blocks** if no connection is waiting.
- **Returns a NEW file descriptor** for talking to *this* client. The original socket keeps listening for more clients.

```c
struct sockaddr_in client_addr;
socklen_t client_addr_size = sizeof( struct sockaddr_in );

int socketfd = socket( AF_INET, SOCK_STREAM, 0 );
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons( 2520 );
server_addr.sin_addr.s_addr = htonl( INADDR_ANY );

bind( socketfd, (struct sockaddr *) &server_addr, sizeof( server_addr ) );
listen( socketfd, 5 );
int newsockfd = accept( socketfd, (struct sockaddr *) &client_addr, &client_addr_size );

/* Do something useful with newsockfd */

close( newsockfd );
/* Later when all is done */
close( socketfd );
```

Real servers call `accept` **in a loop**, handling each client before (or while — see threads, L10) accepting the next.

After this, the **client** talks over its original socket fd and the **server** over the fd returned by `accept`. (Sending/receiving is Lecture 8; since sockets are files, `read`/`write` also work.)

### Wrap the boilerplate

Real programs hide this setup in helpers, e.g.:

```c
int connect_to( const char *host, const char *port );   /* returns a connected fd or -1 */
```

## 7. Summary of the Flow

```
        SERVER                                 CLIENT
  socket()                               socket()
  bind()        (claim port)
  listen()      (ready)
  accept()  ◀──── blocks until ─────────  connect()
     │  returns new fd                        │
     ▼                                        ▼
  talk on new fd  ◀══════ data ══════▶   talk on socket fd
  close(new fd)                          close(fd)
  close(listen fd)
```

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `socket` | `<sys/socket.h>` | `int socket( int domain, int type, int protocol )` | Create a socket; returns a file descriptor. |
| `close` | `<unistd.h>` | `int close( int fd )` | Close the socket (same as for files). |
| `htonl` | `<arpa/inet.h>` | `uint32_t htonl( uint32_t )` | Host → network byte order, 32-bit. |
| `htons` | `<arpa/inet.h>` | `uint16_t htons( uint16_t )` | Host → network byte order, 16-bit. |
| `ntohl` | `<arpa/inet.h>` | `uint32_t ntohl( uint32_t )` | Network → host byte order, 32-bit. |
| `ntohs` | `<arpa/inet.h>` | `uint16_t ntohs( uint16_t )` | Network → host byte order, 16-bit. |
| `getaddrinfo` | `<netdb.h>` | `int getaddrinfo( const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res )` | Resolve a host/service into a linked list of addresses. 0 = success. |
| `freeaddrinfo` | `<netdb.h>` | `void freeaddrinfo( struct addrinfo *res )` | Free the `getaddrinfo` result list. |
| `connect` | `<sys/socket.h>` | `int connect( int sockfd, const struct sockaddr *addr, socklen_t len )` | Client: connect to a server. |
| `bind` | `<sys/socket.h>` | `int bind( int sockfd, const struct sockaddr *addr, socklen_t len )` | Server: attach the socket to an address/port. |
| `listen` | `<sys/socket.h>` | `int listen( int sockfd, int backlog )` | Server: mark socket as accepting connections. |
| `accept` | `<sys/socket.h>` | `int accept( int sockfd, struct sockaddr *addr, socklen_t *len )` | Server: take the next connection; returns a new fd. |

Types/constants: `struct sockaddr_in`, `struct sockaddr`, `struct addrinfo`, `socklen_t`, `AF_INET`, `SOCK_STREAM`, `SOCK_DGRAM`, `INADDR_ANY`, `AI_PASSIVE`.

Deprecated (recognize, don't use): `gethostbyname`.

Useful related (used in exercises): `inet_ntop` / `inet_pton` (convert between binary IPv4 and `"a.b.c.d"` text), `gai_strerror` (message for `getaddrinfo` errors), `setsockopt(... SO_REUSEADDR ...)` (reuse a port right after the server restarts), `nc` (netcat: a command-line TCP/UDP client/server for testing).

## Exercises

1. [ex1_greeting_server.c](ex1_greeting_server.c) — socket/bind/listen/accept and greet each client.
2. [ex2_client.c](ex2_client.c) — `getaddrinfo`/`connect` and read the greeting.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.

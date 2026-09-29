# Lecture 8 — Network Communication

Lecture 7 set up connections; this lecture actually **moves data** — over TCP streams, over UDP datagrams, and with the **libcurl** library for web requests.

## 1. Sending and Receiving on a Stream

Once connected, **either side can send or receive**. Deciding whose turn it is, what the content is, and how to interpret it is entirely the job of the client and server (the *protocol* they agree on).

### `send`

```c
int send( int sockfd, const void *msg, int length, int flags );
```

- `sockfd` — the client's socket (from `socket()`), or the server's per-client socket (from `accept()`).
- `msg`, `length` — the bytes to send.
- `flags` — `0` for simple use.
- **Returns the number of bytes actually sent**, or `-1` with `errno` set.

```c
char *msg = "Hello world!";
int len = strnlen( msg, 13 );
int sent = send( sockfd, msg, len, 0 );
```

**Always check the return value.** Unlike `malloc` failing (rare), networks fail often.

**Partial sends**: `send` may send **fewer bytes than you asked** — there's a limit per call (on the order of 1 KB+, but system-dependent; don't memorize a number). For anything beyond a toy, loop until everything is sent:

```c
int sendall( int socket, char *buf, int *len ) {
    int total = 0;          /* how many bytes we've sent */
    int bytesleft = *len;   /* how many we have left to send */
    int n;

    while ( total < *len ) {
        n = send( socket, buf + total, bytesleft, 0 );
        if ( n == -1 ) {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;            /* report how many were actually sent */
    return n == -1 ? -1 : 0; /* -1 on failure, 0 on success */
}
```

Note the pointer arithmetic `buf + total` — each retry starts where the last one ended — and that `len` is an in/out parameter.

### `recv`

```c
int recv( int sockfd, void *buffer, int length, int flags );
```

- `buffer`, `length` — where to put the data and the **maximum** size.
- **Returns**: number of bytes received (> 0); **`0` = the other side closed the connection** (hung up — stop waiting); **`-1` = error** (check `errno`).
- Like `read`, it does **not** null-terminate, and may return **less** than you asked for, even if more is on the way.

### Knowing when the other side is done ("over!")

A stream is just bytes with no built-in message boundaries. You have to agree on how to tell where a message ends — like saying "over" on a walkie-talkie. Options:

1. **Known size in advance** (fixed-size messages).
2. **Size sent as part of the protocol** (e.g. a length header first, then that many bytes).
3. **Delimiter** (e.g. newline, or HTTP's blank line).
4. **Wait for the other side to close** (`recv` returns 0) — then it's definitely done.

### Sending structured data: serialization

You *can* `recv` straight into a `struct` by passing its address and `sizeof`, but only if the byte layout is **exactly** the same on both machines (endianness, type sizes, padding between fields…). The sensible approach:

- **Serialization** — convert your data into a well-defined byte representation for sending (or storing).
- **Deserialization** — reconstruct it on the other side.

Then systems with different software and architectures can still communicate. In practice, use a library (e.g. **protobuf-c**) rather than writing your own.

When done, `close()` the socket.

## 2. Datagrams (UDP)

No connection — just fire off messages (texting instead of calling). Each call carries the destination or reports the source:

```c
int sendto( int sockfd, const void *msg, int length, unsigned int flags,
            const struct sockaddr *to, socklen_t tolength );

int recvfrom( int sockfd, void *buffer, int length, unsigned int flags,
              struct sockaddr *from, socklen_t *fromlength );
```

- `sendto` returns the number of bytes sent (may be less than expected for large data).
- `recvfrom` fills in `from` with the sender's address — that's how you know who to reply to.
- Remember UDP's properties: messages may be **lost**, **reordered**; each datagram is delivered whole or not at all.
- If you call **`connect()` on a UDP socket**, it just records a default peer: you can then use plain `send`/`recv`. It's still UDP — no real connection is set up.

## 3. libcurl

Normally you don't hand-write sockets to talk to URLs. **cURL** is a client-side network transfer library (plus a command-line tool, `curl`) that speaks many protocols (HTTP, HTTPS, FTP, …). It's **client-side only** — not for servers.

Motivation: web services expose **endpoints** over HTTP, often following **REST** principles. You *could* open a socket and `send` `"GET / HTTP/1.0\r\n..."` yourself, but libcurl handles HTTP, TLS, redirects, etc. for you.

Docs: <https://curl.se/libcurl/c/>. Install headers with `sudo apt install libcurl4-openssl-dev` (a.k.a. "libcurl-dev"); compile with **`-lcurl`**.

### Basic GET

```c
#include <stdio.h>
#include <curl/curl.h>

int main( int argc, char **argv ) {
    CURL *curl;
    CURLcode res;

    curl_global_init( CURL_GLOBAL_DEFAULT );

    curl = curl_easy_init();
    if ( curl ) {
        curl_easy_setopt( curl, CURLOPT_URL, "https://example.com/" );
        res = curl_easy_perform( curl );
        if ( res != CURLE_OK ) {
            fprintf( stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror( res ) );
        }
        curl_easy_cleanup( curl );
    }

    curl_global_cleanup();
    return 0;
}
```

Walkthrough:

| Piece | Role |
|-------|------|
| `CURL *` | The **easy handle** — represents one transfer. Must be initialized and cleaned up. |
| `CURLcode` | Enum result code of operations. **Always check it** — networks fail. |
| `curl_global_init(CURL_GLOBAL_DEFAULT)` | Initialize the library **once**, before any other curl call. (`curl_easy_init` would do it implicitly, but calling it more than once is a problem — do it explicitly.) |
| `curl_global_cleanup()` | Matching global teardown at the very end. |
| `curl_easy_init()` | Create a handle; returns `NULL` on failure (hence the `if`). "Easy" as opposed to the **multi** interface (for many simultaneous transfers — later in the course). |
| `curl_easy_setopt(h, OPTION, value)` | Configure the transfer. `CURLOPT_URL` is the only mandatory one; there are ~200+. |
| `curl_easy_perform(h)` | **Actually does the transfer** over the network; returns a `CURLcode`. |
| `curl_easy_strerror(code)` | Human-readable message for a `CURLcode` (nicer than looking up `errno`). |
| `curl_easy_reset(h)` | Clear all options to reuse a handle for something different. |
| `curl_easy_cleanup(h)` | Free the handle. |

A handle can be **reused** for several transfers (update options between them). By default, a GET's response body is simply written to **stdout**.

### Callbacks

To do something useful with data in either direction, register a **callback** — "when you have data / need data, call this function of mine."

- **Write callback** — called when data is **received from** the server (e.g. a GET response). You decide what to do with it.
- **Read callback** — called when curl needs data to **upload to** the server (e.g. a POST body).

#### Write callback

```c
size_t write_callback( char *ptr, size_t size, size_t nmemb, void *userdata );
```

- `ptr` — the received data (**not** null-terminated). Size of the chunk is `size * nmemb` (`size` is always 1).
- `userdata` — whatever pointer you registered with `CURLOPT_WRITEDATA` (e.g. a `FILE *`, or a struct holding a growing buffer).
- **Return the number of bytes you handled.** If it's not equal to `size * nmemb`, curl treats it as an error and aborts.
- May be called **many times** per transfer (one chunk at a time).

#### Read callback

```c
size_t read_callback( char *buffer, size_t size, size_t nitems, void *inputdata );
```

- Copy up to `size * nitems` bytes into `buffer`.
- Return how many bytes you put there; **return 0 to signal end of data**.
- Called repeatedly until you return 0 — you need to track your position between calls (via `inputdata`).

#### Registering

```c
curl_easy_setopt( handle, CURLOPT_READFUNCTION,  read_callback );
curl_easy_setopt( handle, CURLOPT_READDATA,      void_pointer );   /* -> inputdata */
curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, write_callback );
curl_easy_setopt( handle, CURLOPT_WRITEDATA,     void_pointer );   /* -> userdata  */
```

### POST with a read callback

```c
struct data {
    char *readptr;      /* where to read next */
    size_t sizeleft;    /* how much is left */
};

size_t read_callback( void *dest, size_t size, size_t nmemb, void *userp ) {
    struct data *d = (struct data *) userp;
    size_t buffer_size = size * nmemb;

    if ( d->sizeleft > 0 ) {
        size_t copy_this_much = d->sizeleft;
        if ( copy_this_much > buffer_size ) {
            copy_this_much = buffer_size;    /* only what fits */
        }
        memcpy( dest, d->readptr, copy_this_much );
        d->readptr += copy_this_much;
        d->sizeleft -= copy_this_much;
        return copy_this_much;
    }
    return 0;                                /* no more data */
}

/* in main, after curl_easy_init(): */
curl_easy_setopt( curl, CURLOPT_URL, "https://example.com/index.cgi" );
curl_easy_setopt( curl, CURLOPT_POST, 1L );              /* POST instead of default GET */
curl_easy_setopt( curl, CURLOPT_READFUNCTION, read_callback );
curl_easy_setopt( curl, CURLOPT_READDATA, d );
res = curl_easy_perform( curl );
```

The callback copies as much as fits, advances its position, and returns 0 when everything's been handed over. `main` also checks `curl_global_init`'s result and frees `d` at the end. (For a small in-memory body, `CURLOPT_POSTFIELDS` is a simpler alternative — but the callback approach scales to large or streamed uploads.)

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `send` | `<sys/socket.h>` | `ssize_t send( int fd, const void *msg, size_t len, int flags )` | Send bytes on a connected socket; returns bytes sent (may be partial) or -1. |
| `recv` | `<sys/socket.h>` | `ssize_t recv( int fd, void *buf, size_t len, int flags )` | Receive up to `len` bytes; 0 = peer closed; -1 = error. |
| `sendto` | `<sys/socket.h>` | `ssize_t sendto( int fd, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen )` | Send a datagram to a specific address. |
| `recvfrom` | `<sys/socket.h>` | `ssize_t recvfrom( int fd, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen )` | Receive a datagram and learn who sent it. |
| `sendall` | (lecture helper) | `int sendall( int s, char *buf, int *len )` | Loop over `send` until all bytes are sent. |
| `strnlen` | `<string.h>` | `size_t strnlen( const char *s, size_t maxlen )` | Length of a string, bounded by `maxlen`. |
| `memcpy` | `<string.h>` | `void *memcpy( void *dst, const void *src, size_t n )` | Copy `n` bytes. |
| `curl_global_init` | `<curl/curl.h>` | `CURLcode curl_global_init( long flags )` | One-time library init (`CURL_GLOBAL_DEFAULT`). |
| `curl_global_cleanup` | `<curl/curl.h>` | `void curl_global_cleanup( void )` | One-time library teardown. |
| `curl_easy_init` | `<curl/curl.h>` | `CURL *curl_easy_init( void )` | Create an easy handle (NULL on failure). |
| `curl_easy_setopt` | `<curl/curl.h>` | `CURLcode curl_easy_setopt( CURL *h, CURLoption opt, ... )` | Set an option: `CURLOPT_URL`, `CURLOPT_POST`, `CURLOPT_WRITEFUNCTION`, `CURLOPT_WRITEDATA`, `CURLOPT_READFUNCTION`, `CURLOPT_READDATA`, … |
| `curl_easy_perform` | `<curl/curl.h>` | `CURLcode curl_easy_perform( CURL *h )` | Perform the transfer (blocking). |
| `curl_easy_strerror` | `<curl/curl.h>` | `const char *curl_easy_strerror( CURLcode )` | Text for an error code. |
| `curl_easy_reset` | `<curl/curl.h>` | `void curl_easy_reset( CURL *h )` | Reset all options on a handle. |
| `curl_easy_cleanup` | `<curl/curl.h>` | `void curl_easy_cleanup( CURL *h )` | Free a handle. |

Types/constants: `CURL`, `CURLcode`, `CURLE_OK`, `CURL_GLOBAL_DEFAULT`.

Useful related: `curl_easy_getinfo( h, CURLINFO_RESPONSE_CODE, &long_code )` (HTTP status), `CURLOPT_FOLLOWLOCATION`, `CURLOPT_POSTFIELDS`, `CURLOPT_HTTPHEADER`, `realloc`.

## Exercises

1. [ex1_echo_server.c](ex1_echo_server.c) — `recv`/`send` echo server.
2. [ex2_curl_get.c](ex2_curl_get.c) — download a page with libcurl and count the bytes.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.

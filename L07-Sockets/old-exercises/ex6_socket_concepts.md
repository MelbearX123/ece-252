# Lecture 7 — Exercise 6: Socket Concepts

## 1. Stream or datagram?

Pick `SOCK_STREAM` (TCP) or `SOCK_DGRAM` (UDP) and justify:

1. Downloading a 2 GB file.
2. A multiplayer game broadcasting player positions 60 times a second.
3. `ssh`.
4. A DNS lookup (one small question, one small answer).
5. Live video call.
6. Submitting your assignment to a marking server.

## 2. Analogy check

The lecture compares datagrams to mail and streams to phone calls. For each property, say which analogy it matches and why: (a) messages may arrive out of order; (b) the other side must be available before anything is exchanged; (c) you can hang up; (d) a message might be lost without you knowing.

## 3. Byte order by hand

The 32-bit value `0xC0A80001` is `192.168.0.1`.

a. Write its 4 bytes in memory order on a little-endian machine and on a big-endian machine.
b. What bytes go over the wire (network order)?
c. A little-endian programmer forgets `htonl` and stores `0xC0A80001` directly into `sin_addr.s_addr`. What dotted address will the network actually see?
d. Port 2520 = `0x09D8`. Same question: what port does the other side see if you forget `htons` on a little-endian machine?

## 4. Server vs. client

Fill in which side calls each function (client, server, or both), and in what order:

`socket`, `bind`, `listen`, `accept`, `connect`, `getaddrinfo`, `close`

Then answer:

1. Why doesn't the client need `bind`? When *might* a client want to call it?
2. `accept` returns a new file descriptor. Why not just use the listening one to talk to the client?
3. A server has accepted 3 clients that are still connected. How many socket file descriptors does it hold?
4. What does `backlog` in `listen(fd, 5)` limit? What happens to the 6th pending connection?
5. What happens if you call `accept` and no one is connecting?

## 5. Addresses

1. What does binding to `INADDR_ANY` mean for a machine with Wi-Fi, Ethernet, and loopback interfaces?
2. Why are ports below 1024 restricted? Name three well-known ports and their services.
3. Two processes on the same machine try to bind port 2520. What happens to the second one?
4. Two *clients* on the same machine both connect to a server's port 80. Is that a conflict? Why not?
5. Why should you prefer `getaddrinfo` over `gethostbyname`?
6. `getaddrinfo` returns a *linked list*. Why might a name resolve to several addresses, and what should a robust client do with them?

## 6. Everything is a file

List every way you've now seen to obtain a file descriptor (`open`, …). Which functions work on all of them? Which are type-specific?

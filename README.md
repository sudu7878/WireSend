<h1>WireSend</h1>

> ⚠️ **Work in progress** — may contain bugs. Chat support is functional; more features coming soon.

A FTP project that currently only supports **chatting**. Additional FTP functionality is still in development.

This software uses NimLib, Copyright (c) 2026 ArtikLamartik.
Licensed under NLL-2.0.0: https://github.com/ArtikLamartik/NimLib/blob/main/LICENSE

---
## Architecture
WireSend has an asynchronous messaging protocol that supports serializing/deserializing techniques to send data efficiently over the wire. Have a look below!

<img src = "mess/Arch.png" alt="WireSend architecture">

Here's a precise protocol design for messaging (note: we convert UTF-8 into binary before sending. That way it is also suitable for sending files).

<img src = "mess/ProtocolVer0.1.1.jpg" alt="WireSend packet design"> 

---

## Building

Just run the makefile.
```bash
make all
```

---

## Usage

### Start a server

```bash
./wires server
```

### Connect as a client

```bash
./wires <ip-address> <port>
```

Replace `<ip-address>` and `<port>` with the values shown when the server starts.

---

## Debugging

Add the `-dbg` flag to enable debugging mode:

```bash
./wires server -dbg
./wires <ip-address> <port> -dbg
```

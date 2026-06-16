# Computer Networking Labs

Lab work from a university Computer Networking course, implemented in C. Most labs use a
simulated network environment rather than real sockets, except for the HTTP server.

## Lab 1 — Simple HTTP Server

- Implemented using the C socket API
- Listens for connections, parses basic HTTP GET requests, maps URL paths to files on disk,
  and returns valid HTTP responses
- Serves HTML and image (PNG) files with correct content-type headers

## Lab 2 — Reliable Transport Protocol Simulator

- Simulates two hosts communicating over an unreliable network (loss, corruption, delay,
  duplication)
- Implements sequence numbers, checksums, duplicate detection, and timer-based retransmission
- Based on stop-and-wait / alternating-bit protocol principles

## Lab 3 — Distance Vector Routing Simulator

- Simulates a 4-node network where each node maintains a distance table and exchanges routing
  info with its neighbors
- Implements the Bellman-Ford distance vector principle, recalculating and propagating
  cheapest paths until the network converges

## Tech stack

C, TCP sockets (Lab 1), simulated network environments (Labs 2–3)

## What I learned

- How TCP sockets and HTTP requests/responses work in practice
- How reliable data transfer is built on top of an unreliable network layer
- How distance vector routing (Bellman-Ford) computes and converges on shortest paths
- Structuring shared logic in C across multiple simulated network nodes

## How to run

_(add your actual build/run command(s) here — one per lab if they're separate programs)_

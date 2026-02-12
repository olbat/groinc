# Groinc

Groinc is a network packet sniffer and analyzer for Linux. It captures packets from a live network interface or reads them from files, parses protocol headers across multiple layers (datalink, network, transport), applies filters, and produces configurable reports.

## Features

- Live packet capture using Linux PF_PACKET raw sockets
- Read/write packet data from/to files
- Multi-layer protocol parsing: Ethernet, IPv4, ARP, TCP, UDP, ICMP
- Flexible filtering by MAC address, IP address (with CIDR support), port, protocol, substring, or regex
- Configurable display: raw data, hex dump, per-layer headers, simple or verbose output
- Report generation with packet counts and capture timing
- DNS name caching for hostname resolution
- Graceful signal handling (SIGINT, SIGQUIT)

## Implementation

Groinc is written in ANSI C with no external dependencies beyond the C standard library and Linux system headers.

### Architecture

Packet capture uses `select()` for I/O multiplexing on a PF_PACKET socket. Each captured packet is passed through a three-stage parsing pipeline:

1. **Datalink layer** - Ethernet (IEEE 802.3) header parsing
2. **Network layer** - IPv4 (RFC 791), ARP
3. **Transport layer** - TCP (RFC 793), UDP, ICMP

Protocol implementations are modular: each protocol provides scan (parse) and print functions registered via function pointer tables, making it straightforward to add new protocols.

### Key data structures

- `struct data` - Tracks the raw packet buffer, current parse offset, and total length
- `struct protocol_header` - Holds a parsed header's protocol ID, length, and data pointer
- A union-based **linked list** is used throughout for filters, display options, reports, and errors
- A **hash table** backs the error lookup system

### Filtering

All active filters are AND-composed. The filter chain is evaluated sequentially with early exit. Compiler branch-prediction hints (`likely`/`unlikely` macros) optimize the hot path.


## Building

Requires GCC and standard Linux headers.

```sh
make
```

To install the binary and man page system-wide:

```sh
make install
```

Other targets:

| Target     | Description                                    |
|------------|------------------------------------------------|
| `all`      | Build the executable (default)                 |
| `dev`      | Development build (keeps intermediate objects)  |
| `install`  | Install binary to `/usr/bin` and man page       |
| `clean`    | Remove object files                            |
| `cleanall` | Remove object files and the executable         |


The default build uses `-Wall -ansi -pedantic -O3 -fomit-frame-pointer -g`.

## Usage

```
groinc [options]
```

Running `groinc` with no arguments captures and displays all packets on the default interface. Root privileges (or `CAP_NET_RAW`) are required for live capture.

### Display options

| Flag | Long form                  | Description                              |
|------|----------------------------|------------------------------------------|
| `-a` | `--displaydata`            | Show packet payload data                 |
| `-A` | `--simpledisplay`          | Simple display format                    |
| `-v` | `--verbose`                | Verbose header information               |
| `-n` | `--displaypackets`         | Show packet numbers                      |
| `-H` | `--hexa`                   | Hexadecimal dump of packet               |
| `-j` | `--displayprotodatalink`   | Show datalink layer protocol info        |
| `-b` | `--displayprototransport`  | Show transport layer protocol info       |
| `-B` | `--displayprotonetwork`    | Show network layer protocol info         |
| `-N` | `--displayallpackets`      | Display all packets including empty ones |

### Filter options

| Flag | Long form            | Argument   | Description                                |
|------|----------------------|------------|--------------------------------------------|
| `-d` | `--destport`         | PORT       | Filter by destination port                 |
| `-s` | `--sourceport`       | PORT       | Filter by source port                      |
| `-g` | `--globalport`       | PORT       | Filter by either source or destination port|
| `-D` | `--destip`           | IP[/CIDR]  | Filter by destination IP                   |
| `-S` | `--sourceip`         | IP[/CIDR]  | Filter by source IP                        |
| `-G` | `--globalip`         | IP[/CIDR]  | Filter by either source or destination IP  |
| `-m` | `--sourcemac`        | MAC        | Filter by source MAC address               |
| `-M` | `--destmac`          | MAC        | Filter by destination MAC address          |
| `-p` | `--protocol`         | PROTO      | Filter by protocol (TCP, UDP, ICMP, ARP, IP) |
| `-f` | `--filter`           | STRING     | Substring match in packet payload          |
| `-F` | `--filter-regex`     | REGEX      | POSIX regex match on packet payload        |
| `-l` | `--limitnb`          | NUMBER     | Stop after capturing N packets             |
| `-t` | `--timelimit`        | SECONDS    | Stop after N seconds                       |
| `-z` | `--dontdisplayemptysl`| -          | Hide packets with empty payloads          |
| `-q` | `--quiet`            | -          | Suppress packet display                    |

IP addresses accept several formats: a single IP (`192.168.1.1`), CIDR notation (`192.168.1.0/24`), colon-separated netmask (`192.168.1.0:255.255.255.0`), or a hostname.

### Input/output options

| Flag | Long form        | Argument | Description                          |
|------|------------------|----------|--------------------------------------|
| `-r` | `--read`         | FILE     | Read packets from a file             |
| `-w` | `--write`        | FILE     | Write captured packets to a file     |
| `-o` | `--outputdata`   | FILE     | Write packet data to a file          |
| `-O` | `--output`       | FILE     | Write packet headers to a file       |
| `-j` | `--noresolv`     | -        | Disable hostname resolution          |

### Report options

| Flag | Long form               | Description                    |
|------|-------------------------|--------------------------------|
| `-T` | `--reportotaltime`      | Report total capture time      |
| `-c` | `--countpacketstot`     | Report total packet count      |
| `-C` | `--countpacketsfiltred` | Report filtered packet count   |

### Miscellaneous

| Flag | Long form     | Description             |
|------|---------------|-------------------------|
| `-h` | `--help`      | Show help message       |
|      | `--version`   | Show version            |
|      | `--license`   | Show GPLv3 license text |

## Examples

Capture all traffic and display payload data:

```sh
groinc -a
```

Capture TCP traffic on port 80 with verbose headers:

```sh
groinc -p TCP -d 80 -v
```

Capture 100 packets from a subnet and write to a file:

```sh
groinc -S 192.168.1.0/24 -l 100 -w capture.dat
```

Read a capture file and filter for HTTP content:

```sh
groinc -r capture.dat -f "HTTP" -a
```

Quiet capture with a report of total and filtered packet counts over 60 seconds:

```sh
groinc -q -c -C -t 60
```

Hex dump of ICMP packets:

```sh
groinc -p ICMP -H
```

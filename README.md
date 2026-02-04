# Simple IMAP Client

A command-line IMAP client written in C. It connects to an IMAP server over port 143, authenticates, and supports retrieving emails, parsing headers, extracting MIME sections, and listing messages.

## Features

- **Login** — Connect and authenticate with username/password
- **Folder selection** — Use a specific mailbox or default to INBOX
- **retrieve** — Fetch raw email content by message number
- **parse** — Parse and display email headers
- **mime** — Extract the first `text/plain` MIME section from an email
- **list** — List messages in the selected folder

## Build

Requirements: C17-capable compiler (e.g. `cc`/`gcc`/`clang`).

```bash
make
```

This produces the `fetchmail` executable. To clean build artifacts:

```bash
make clean
```

## Usage

```text
./fetchmail -u <username> -p <password> [-f <folder>] [-n <messageNum>] [-t] <command> <server_name>
```

### Options

| Option | Description |
|--------|-------------|
| `-u`   | Username (required) |
| `-p`   | Password (required) |
| `-f`   | Folder/mailbox (optional; defaults to INBOX if omitted) |
| `-n`   | Message sequence number (optional). Use `*` or omit for the most recent message. Must be a positive integer otherwise. |
| `-t`   | Test mode — exits immediately with code 0 |

### Commands

| Command    | Description |
|-----------|-------------|
| `retrieve` | Fetch and print the full raw email |
| `parse`   | Parse and display the email header |
| `mime`    | Print the first `text/plain` MIME part |
| `list`    | List emails in the selected folder |

### Examples

```bash
# List messages in INBOX
./fetchmail -u user@example.com -p secret -f INBOX list imap.example.com

# Retrieve the latest message from INBOX
./fetchmail -u user@example.com -p secret retrieve imap.example.com

# Retrieve message number 5 from folder "Archive"
./fetchmail -u user@example.com -p secret -f Archive -n 5 retrieve imap.example.com

# Parse headers of the most recent message
./fetchmail -u user@example.com -p secret parse imap.example.com

# Get the first text/plain MIME section of the latest message
./fetchmail -u user@example.com -p secret mime imap.example.com
```

## Exit codes

| Code | Meaning |
|------|---------|
| 0    | Success |
| 1    | Invalid arguments (e.g. missing username/password/command/server, invalid `-n`) |
| 2    | Connection init failed or invalid server greeting |
| 3    | Server error (login failure, folder not found, message not found) |
| 4    | MIME/parse error |
| 5    | Other error |

## Project structure

| File | Role |
|------|------|
| `main.c` | CLI argument parsing, connection flow, command dispatch |
| `communication.c` / `communication.h` | Socket connection, IMAP send/receive, login, folder select, fetch |
| `email_handle.c` / `email_handle.h` | Header parsing, MIME extraction, list messages |
| `dynamic_string.c` / `dynamic_string.h` | Dynamic string type and helpers |
| `utils.c` / `utils.h` | Shared utilities |
| `ERROR_CODE.h` | Exit/error code constants |
| `IMAP_RESPONSE.h` | IMAP response handling |

## License

See repository for license information.

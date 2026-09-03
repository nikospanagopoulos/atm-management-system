# ATM Management System (C)

A terminal-based ATM management system written in C. Supports user
registration/login, account management, and transactions, with all data
persisted to text files.

> **Status: in progress.** All core features (register/login, account
> CRUD, transactions, interest calculation, transfers) are complete, along
> with two bonus features (TUI, password encryption). SQLite migration is
> partially implemented; real-time transfer notifications (pipes/fork) not
> yet started.

---

## Features

- **Register / Login** — unique usernames, credential validation
- **Create / List accounts** — per-user account management
- **Check account details** — with interest calculation by account type
- **Update account** — phone / country
- **Make transactions** — deposit / withdraw, with balance validation
- **Remove account**
- **Transfer ownership** — between users

### Account types & interest
| Type | Rate | When |
|---|---|---|
| current | 0% | No interest |
| savings | 7% | Monthly, on deposit day |
| fixed01 | 4% | After 1 year |
| fixed02 | 5% | After 2 years |
| fixed03 | 8% | After 3 years |

`fixed` accounts are locked — no withdrawals or deposits allowed.

---

## Bonus features

| Feature | Status |
|---|---|
| Custom Makefile | ✅ Done |
| TUI (colors, borders) | ✅ Done |
| Password encryption (SHA-256 + salt) | ✅ Done |
| SQLite database | 🚧 In progress |
| Real-time transfer notifications (pipes + fork) | ⬜ Not started |

---

## Requirements
- GCC / any C compiler
- `make`

## Build & Run
```bash
make        # compiles
./atm       # runs the program
make clean  # removes binary
```

---

## Data Storage
All data is persisted to text files under `data/`:
- `users.txt` — `{id} {name} {password}`
- `records.txt` — `{id} {user_id} {username} {account_id} {date} {country} {phone} {balance} {type}`

(Migrating to SQLite — see Bonus features above.)

---

## Project Structure
```
.
├── data/
│ ├── records.txt
│ └── users.txt
├── Makefile
└── src/
├── auth.c # register, login
├── header.h # structs, function declarations
├── main.c # entry point, main menu loop
└── system.c # account operations
```

---

## What I learned
Structuring a non-trivial C project across multiple files, manual file-based
persistence, implementing SHA-256 password hashing, and beginning the
transition from flat-file storage to SQLite.

## License
Built as part of the [Zone01](https://zone01.gr) curriculum.

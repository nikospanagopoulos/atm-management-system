# PRD — ATM Management System

## 1. Overview

A terminal-based ATM Management System written in C. Users can register/login, manage bank accounts, and perform transactions. All data is persisted in text files.

- Language: C
- Storage: text files (`users.txt`, `records.txt`)
- Solo project


## 2. Goals

- Implement all required features from the project spec
- Write clean, modular, and auditable code
- Persist all changes immediately to the correct files
- Pass all audit questions successfully
- Implement all bonus features (TUI, encryption, pipes, SQLite, Makefile)


## 3. Scope

| In scope | Out of scope |
|---|---|
| Register / Login | Multi-user sessions on same terminal (except bonus) |
| Create / List accounts | Auto-generated account IDs |
| Check account details + interest | Unicode / non-ASCII input |
| Update account (phone / country) | Web or GUI interface |
| Make transactions (withdraw / deposit) | |
| Remove account | |
| Transfer ownership | |
| Bonus: TUI, encryption, pipes, SQLite, Makefile | |


## 4. File System

```
.
├── data
│   ├── records.txt
│   └── users.txt
├── Makefile
└── src
    ├── auth.c       ← register, login
    ├── header.h     ← structs, function declarations
    ├── main.c       ← entry point, main menu loop
    └── system.c     ← all account operations
```


## 5. Data Formats

### users.txt
```
{id} {name} {password}
```
Example:
```
0 Alice 1234password
1 Michel password1234
```

### records.txt
```
{id} {user_id} {username} {account_id} {date} {country} {phone} {balance} {type}
```
Example:
```
0 0 Alice 0 10/02/2020 germany 986134231 11090830.00 current
1 1 Michel 2 10/10/2021 portugal 914134431 1920.42 savings
```


## 6. Features

### 6.1 Register — `auth.c`
- User inputs name + password
- If name already exists → display error message
- If new → save to `users.txt` with a new auto-incremented id
- Names must be unique

### 6.2 Login — `auth.c`
- User inputs name + password
- Check credentials against `users.txt`
- If correct → enter main menu
- If wrong → display error message

### 6.3 Create Account — `system.c`
- User inputs: account_id, date, country, phone, balance, type
- Account types: `current`, `savings`, `fixed01`, `fixed02`, `fixed03`
- Save to `records.txt`

### 6.4 Check Account Details — `system.c`
- User inputs account_id
- If not found → display error
- Display account info + interest calculation:

| Type | Rate | When |
|---|---|---|
| savings | 7% | On deposit day every month |
| fixed01 | 4% | After 1 year from creation date |
| fixed02 | 5% | After 2 years from creation date |
| fixed03 | 8% | After 3 years from creation date |
| current | 0% | "You will not get interests because the account is of type current" |

Interest formulas:
- `savings`  → `balance * 0.07 / 12`
- `fixed01`  → `balance * 0.04`
- `fixed02`  → `balance * 0.05 * 2`
- `fixed03`  → `balance * 0.08 * 3`

### 6.5 Update Account — `system.c`
- User inputs account_id → if not found → error
- Prompt: update `phone` or `country`?
- Save updated record to `records.txt`

### 6.6 Make Transaction — `system.c`
- User inputs account_id
- `fixed01`, `fixed02`, `fixed03` → display error, transactions not allowed
- `current` / `savings` → prompt: withdraw or deposit
- Withdraw: cannot exceed available balance → display error if attempted
- Save updated balance to `records.txt`

### 6.7 Remove Account — `system.c`
- User inputs account_id → if not found → error
- Delete record from `records.txt`
- Save updated file

### 6.8 Transfer Ownership — `system.c`
- User inputs account_id + target username
- Update `user_id` and `username` fields in `records.txt`
- Save updated file

### 6.9 List Accounts — `system.c`
- Display all accounts belonging to the currently logged-in user


## 7. Data Structures — `header.h`

```c
typedef struct {
    int    id;
    char   name[50];
    char   password[50];
} User;

typedef struct {
    int    id;
    int    user_id;
    char   username[50];
    int    account_id;
    char   date[12];
    char   country[50];
    char   phone[20];
    double balance;
    char   type[10];
} Account;
```


## 8. Function Signatures

```c
/* auth.c */
void   registerUser(void);
int    loginUser(User *out_user);

/* system.c */
void   createAccount(User user);
void   listAccounts(User user);
void   checkAccountDetails(User user);
void   updateAccount(User user);
void   makeTransaction(User user);
void   removeAccount(User user);
void   transferOwnership(User user);

/* utils (in system.c or separate) */
int    findAccountById(int account_id, User user, Account *out);
void   saveAllRecords(Account *accounts, int count);
int    loadAllRecords(Account *out_accounts);
int    loadAllUsers(User *out_users);
double calculateInterest(Account acc);
```


## 9. Interest Acceptance Criteria

All values based on balance `$1001.20`, date `10/10/2012`:

| Type | Expected output |
|---|---|
| savings | "You will get $5.84 as interest on day 10 of every month" |
| fixed01 | "You will get $40.05 as interest on 10/10/2013" |
| fixed02 | "You will get $100.12 as interest on 10/10/2014" |
| fixed03 | "You will get $240.29 as interest on 10/10/2015" |


## 10. Bonus Features

| Feature | Description | Tool / Method |
|---|---|---|
| Custom Makefile | Automate compilation with `make` | Makefile rules |
| Better TUI | Colors, borders, structured menus | ANSI escape codes or `ncurses` |
| Password encryption | Do not store passwords as plain text | SHA-256 via OpenSSL or simple hash |
| SQLite database | Replace text files with a real DB | `libsqlite3` in C |
| Real-time notification | Notify receiver instantly on transfer | `pipe()` + `fork()` |


## 11. Audit-Critical Checklist

- [ ] Marcus/q1w2e3r4t5y6 saves correctly to `users.txt`
- [ ] Re-registering Alice shows error
- [ ] All usernames in `users.txt` are unique
- [ ] Alice can login and reach main menu
- [ ] Updating non-existent account shows error
- [ ] Update prompts phone or country choice
- [ ] Phone/country update persists in `records.txt`
- [ ] Interest values match exactly (see section 9)
- [ ] fixed accounts block transactions with error
- [ ] Withdraw blocked when amount > balance
- [ ] Deposit/withdraw updates `records.txt`
- [ ] Remove accounts 834213, 320421, 3214 → deleted from file
- [ ] Removing non-existent account shows error
- [ ] Transfer account 3212 from Alice to Michel → updates `records.txt`

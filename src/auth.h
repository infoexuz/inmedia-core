// auth.h - accounts and protection against password guessing.
// Part of inMedia Core. Written with help from Claude (AI assistant);
// every function was reviewed and can be explained by the team.
#ifndef INMEDIA_AUTH_H
#define INMEDIA_AUTH_H

#include <ctime>
#include <string>

const int MAX_ACCOUNTS = 20;

struct Account {
    std::string email;
    std::string passwordHash;  // never the password itself
    std::string name;
};

// Turns a password into a number written in hex (the djb2 algorithm). Good for
// a classroom demo only: the real inMedia server uses Argon2id from libsodium.
std::string simpleHash(const std::string& password);

// Reads accounts from a file with lines "email|hash|name". Returns how many
// were read, or -1 when the file cannot be opened.
int loadAccounts(const std::string& path, Account accounts[], int maxAccounts);

// Returns a pointer to the account with this e-mail and password, or nullptr.
// The pointer points into the `accounts` array, so nothing is copied.
Account* findAccount(Account accounts[], int count, const std::string& email,
                     const std::string& password);

// Counts failed log-ins. After `maxFailures` failures in a row, log-in is
// locked for `lockSeconds` seconds. A successful log-in calls reset().
class LoginGuard {
public:
    LoginGuard(int maxFailures, int lockSeconds);

    bool isLocked(std::time_t now) const;
    int secondsLeft(std::time_t now) const;
    void recordFailure(std::time_t now);
    void reset();
    int failures() const;

private:
    int maxFailures_;
    int lockSeconds_;
    int failures_;
    std::time_t lockedUntil_;
};

#endif

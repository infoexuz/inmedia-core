// auth.cpp - see auth.h.
// Written with help from Claude (AI assistant); reviewed by the team.
#include "auth.h"

#include <fstream>
#include <sstream>

std::string simpleHash(const std::string& password) {
    unsigned long long hash = 5381;
    for (char c : password) {
        hash = hash * 33 + static_cast<unsigned char>(c);
    }
    std::ostringstream hex;
    hex << std::hex << hash;
    return hex.str();
}

int loadAccounts(const std::string& path, Account accounts[], int maxAccounts) {
    std::ifstream file(path);
    if (!file) {
        return -1;
    }
    int count = 0;
    std::string line;
    while (count < maxAccounts && std::getline(file, line)) {
        std::size_t first = line.find('|');
        std::size_t second = line.find('|', first + 1);
        if (line.empty() || line[0] == '#' || first == std::string::npos ||
            second == std::string::npos) {
            continue;  // comment or broken line
        }
        accounts[count].email = line.substr(0, first);
        accounts[count].passwordHash = line.substr(first + 1, second - first - 1);
        accounts[count].name = line.substr(second + 1);
        ++count;
    }
    return count;
}

Account* findAccount(Account accounts[], int count, const std::string& email,
                     const std::string& password) {
    std::string hash = simpleHash(password);
    for (int i = 0; i < count; ++i) {
        if (accounts[i].email == email && accounts[i].passwordHash == hash) {
            return &accounts[i];
        }
    }
    return nullptr;
}

LoginGuard::LoginGuard(int maxFailures, int lockSeconds)
    : maxFailures_(maxFailures), lockSeconds_(lockSeconds), failures_(0), lockedUntil_(0) {}

bool LoginGuard::isLocked(std::time_t now) const {
    return now < lockedUntil_;
}

int LoginGuard::secondsLeft(std::time_t now) const {
    return isLocked(now) ? static_cast<int>(lockedUntil_ - now) : 0;
}

void LoginGuard::recordFailure(std::time_t now) {
    ++failures_;
    if (failures_ >= maxFailures_) {
        lockedUntil_ = now + lockSeconds_;
        failures_ = 0;  // start counting again after the lock
    }
}

void LoginGuard::reset() {
    failures_ = 0;
    lockedUntil_ = 0;
}

int LoginGuard::failures() const {
    return failures_;
}

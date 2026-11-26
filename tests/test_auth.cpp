// test_auth.cpp - checks for auth.cpp.
// Written with help from Claude (AI assistant); reviewed by the team.
#include <ctime>

#include "auth.h"
#include "check.h"

void testAuth() {
    check(simpleHash("demo12345") == simpleHash("demo12345"), "hash is the same every time");
    check(simpleHash("demo12345") != simpleHash("demo12346"), "different passwords, different hash");

    Account accounts[2];
    accounts[0] = {"demo@inmedia.uz", simpleHash("demo12345"), "Demo"};
    accounts[1] = {"studio@inmedia.uz", simpleHash("studio2026"), "Studio"};
    check(findAccount(accounts, 2, "studio@inmedia.uz", "studio2026") == &accounts[1],
          "right password returns a pointer to that account");
    check(findAccount(accounts, 2, "demo@inmedia.uz", "wrong") == nullptr,
          "wrong password returns nullptr");

    LoginGuard guard(5, 60);
    std::time_t now = 1000;
    for (int i = 0; i < 4; ++i) {
        guard.recordFailure(now);
    }
    check(!guard.isLocked(now), "4 failures do not lock");
    guard.recordFailure(now);
    check(guard.isLocked(now) && guard.secondsLeft(now) == 60, "5th failure locks for 60 s");
    check(!guard.isLocked(now + 60), "lock ends after 60 s");
}

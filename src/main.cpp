// main.cpp - inMedia Core: a console version of the core logic of inMedia,
// a media platform for studios and creators from Central Asia.
// Team: Muhammadamin Nazarov, Feruz Alidjanov (CAU, Computer Programming, CS-1).
// Written with help from Claude (AI assistant); reviewed by the team.
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "auth.h"
#include "catalog.h"
#include "upload.h"
#include "validation.h"

using namespace std;

const string CATALOG_FILE = "data/catalog.txt";
const string USERS_FILE = "data/users.txt";

// Asks until the user types a whole number between low and high.
int readInt(const string& prompt, int low, int high) {
    while (true) {
        cout << prompt;
        int value;
        if (cin >> value && value >= low && value <= high) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof()) {
            cout << "\nInput closed. Goodbye.\n";
            exit(0);
        }
        cin.clear();  // clear the error after letters such as "abc"
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Please enter a number from " << low << " to " << high << ".\n";
    }
}

// Asks until the user types a line that is not empty (spaces do not count).
string readLine(const string& prompt, size_t maxLength) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            cout << "\nInput closed. Goodbye.\n";
            exit(0);
        }
        line = trim(line);
        if (line.empty()) {
            cout << "  This cannot be empty.\n";
        } else if (line.size() > maxLength) {
            cout << "  Too long: at most " << maxLength << " characters.\n";
        } else if (line.find('|') != string::npos) {
            cout << "  The character '|' is not allowed.\n";
        } else {
            return line;
        }
    }
}

// Counts letters, not bytes: in UTF-8 a Cyrillic letter takes 2 bytes, and only
// the first byte of a letter does not start with the bits 10.
size_t letterCount(const string& text) {
    size_t letters = 0;
    for (char c : text) {
        if ((static_cast<unsigned char>(c) & 0xC0) != 0x80) {
            ++letters;
        }
    }
    return letters;
}

// Adds spaces after the text so it fills `width` letters (setw counts bytes).
string padRight(const string& text, size_t width) {
    size_t letters = letterCount(text);
    return letters >= width ? text + " " : text + string(width - letters, ' ');
}

void printVideoRow(const Video& v) {
    cout << "  " << setw(3) << v.id << "  " << padRight(v.type, 8) << padRight(v.title, 32)
         << padRight(v.channel, 20) << setw(8) << formatDuration(v.durationSec) << '\n';
}

void printHeader() {
    cout << "  " << setw(3) << "ID" << "  " << padRight("TYPE", 8) << padRight("TITLE", 32)
         << padRight("CHANNEL", 20) << setw(8) << "LENGTH" << '\n';
}

void logIn(Account accounts[], int accountCount, Account*& current, LoginGuard& guard) {
    if (current != nullptr) {
        cout << "  Logged out " << current->name << ".\n";
        current = nullptr;
        return;
    }
    time_t now = time(nullptr);
    if (guard.isLocked(now)) {
        cout << "  Too many wrong passwords. Try again in " << guard.secondsLeft(now)
             << " seconds.\n";
        return;
    }
    string email = readLine("  E-mail: ", 100);
    if (!isValidEmail(email)) {
        cout << "  \"" << email << "\" is not a valid e-mail address.\n";
        return;
    }
    string password = readLine("  Password: ", 100);
    Account* account = findAccount(accounts, accountCount, toLowerAscii(email), password);
    if (account == nullptr) {
        guard.recordFailure(time(nullptr));
        if (guard.isLocked(time(nullptr))) {
            cout << "  Wrong e-mail or password. Log-in is now locked for "
                 << guard.secondsLeft(time(nullptr)) << " seconds.\n";
        } else {
            cout << "  Wrong e-mail or password. Failed attempts: " << guard.failures() << ".\n";
        }
        return;
    }
    guard.reset();
    current = account;
    cout << "  Welcome, " << current->name << "!\n";
}

void listVideos(Catalog& catalog) {
    if (catalog.count == 0) {
        cout << "  The catalog is empty.\n";
        return;
    }
    sortByTitle(catalog);
    printHeader();
    for (int i = 0; i < catalog.count; ++i) {
        printVideoRow(catalog.items[i]);
    }
    cout << "  " << catalog.count << " videos.\n";
}

void searchVideos(const Catalog& catalog) {
    string query = readLine("  Search for: ", 80);
    int results[MAX_VIDEOS];
    int found = searchCatalog(catalog, query, results, MAX_VIDEOS);
    if (found == 0) {
        cout << "  Nothing found for \"" << query << "\".\n";
        return;
    }
    printHeader();
    for (int i = 0; i < found; ++i) {
        printVideoRow(catalog.items[results[i]]);
    }
    cout << "  " << found << " found.\n";
}

void viewVideo(Catalog& catalog) {
    int id = readInt("  Video id: ", 1, 1000000);
    Video* video = findById(catalog, id);  // pointer into the catalog array
    if (video == nullptr) {
        cout << "  There is no video with id " << id << ".\n";
        return;
    }
    cout << "  Title:   " << video->title << '\n'
         << "  Type:    " << video->type << '\n'
         << "  Channel: " << video->channel << '\n'
         << "  Length:  " << formatDuration(video->durationSec) << '\n'
         << "  Address: inmedia.uz/watch/" << video->slug << '\n';
}

bool addNewVideo(Catalog& catalog, const Account* current) {
    if (current == nullptr) {
        cout << "  Please log in first (menu item 1).\n";
        return false;
    }
    Video video;
    video.title = readLine("  Title: ", 30);
    cout << "  Type: 1) movie  2) episode  3) podcast  4) lesson\n";
    video.type = VIDEO_TYPES[readInt("  Choose 1-4: ", 1, TYPE_COUNT) - 1];
    video.channel = current->name;
    int minutes = readInt("  Length, minutes (0-600): ", 0, 600);
    int seconds = readInt("  Length, seconds (0-59): ", 0, 59);
    video.durationSec = minutes * 60 + seconds;
    if (video.durationSec == 0) {
        cout << "  A video cannot be 0 seconds long. Nothing was added.\n";
        return false;
    }
    if (!addVideo(catalog, video)) {
        cout << "  The catalog is full (" << MAX_VIDEOS << " videos).\n";
        return false;
    }
    const Video& added = catalog.items[catalog.count - 1];
    cout << "  Added \"" << added.title << "\" with id " << added.id
         << ", address inmedia.uz/watch/" << added.slug << '\n';
    return true;
}

void planUpload() {
    int sizeMb = readInt("  File size in MB (1-4096): ", 1, 4096);
    int height = readInt("  Video height in pixels (2-4320): ", 2, 4320);
    long long size = sizeMb * BYTES_PER_MB;
    long long parts = partCount(size, CHUNK_BYTES);
    long long last = partSize(size, CHUNK_BYTES, parts);
    if (parts == 1) {
        cout << "  Upload: 1 part (the whole " << sizeMb << " MB file)";
    } else {
        cout << "  Upload: " << parts << " parts of 8 MB";
        if (last != CHUNK_BYTES) {
            cout << ", the last one " << last / BYTES_PER_MB << " MB";
        }
    }
    cout << ".\n  If the connection drops, only the missing parts are sent again.\n";

    Rendition ladder[MAX_RENDITIONS];
    int count = buildLadder(height, ladder, MAX_RENDITIONS);
    cout << "  Qualities made for viewers:";
    for (int i = 0; i < count; ++i) {
        cout << ' ' << ladder[i].height << "p (" << ladder[i].bitrateKbps << " kbit/s)";
        if (i + 1 < count) {
            cout << ',';
        }
    }
    cout << '\n';
}

int main() {
    Catalog catalog;
    int skipped = 0;
    int loaded = loadCatalog(catalog, CATALOG_FILE, skipped);
    if (loaded < 0) {
        cout << "Note: " << CATALOG_FILE << " not found, starting with an empty catalog.\n";
    } else {
        cout << "Loaded " << loaded << " videos from " << CATALOG_FILE;
        if (skipped > 0) {
            cout << " (" << skipped << " broken line(s) skipped)";
        }
        cout << ".\n";
    }

    Account accounts[MAX_ACCOUNTS];
    int accountCount = loadAccounts(USERS_FILE, accounts, MAX_ACCOUNTS);
    if (accountCount < 0) {
        cout << "Note: " << USERS_FILE << " not found, nobody can log in.\n";
        accountCount = 0;
    }

    Account* current = nullptr;    // the logged-in account, or nullptr
    LoginGuard guard(5, 60);       // 5 wrong passwords -> locked for 60 seconds
    bool unsaved = false;

    while (true) {
        cout << "\n=== inMedia Core ===" << (current ? "  [" + current->name + "]" : "") << '\n'
             << "1) " << (current ? "Log out" : "Log in") << '\n'
             << "2) List all videos\n"
             << "3) Search videos\n"
             << "4) View a video by id\n"
             << "5) Add a video\n"
             << "6) Plan an upload\n"
             << "7) Save the catalog\n"
             << "0) Exit\n";
        int choice = readInt("Choose: ", 0, 7);
        switch (choice) {
            case 1:
                logIn(accounts, accountCount, current, guard);
                break;
            case 2:
                listVideos(catalog);
                break;
            case 3:
                searchVideos(catalog);
                break;
            case 4:
                viewVideo(catalog);
                break;
            case 5:
                if (addNewVideo(catalog, current)) {
                    unsaved = true;
                }
                break;
            case 6:
                planUpload();
                break;
            case 7:
                if (saveCatalog(catalog, CATALOG_FILE)) {
                    cout << "  Saved " << catalog.count << " videos to " << CATALOG_FILE << ".\n";
                    unsaved = false;
                } else {
                    cout << "  Could not write " << CATALOG_FILE << ".\n";
                }
                break;
            case 0:
                if (unsaved && saveCatalog(catalog, CATALOG_FILE)) {
                    cout << "Saved your changes to " << CATALOG_FILE << ".\n";
                }
                cout << "Goodbye!\n";
                return 0;
        }
    }
}

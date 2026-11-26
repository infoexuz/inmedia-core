# inMedia Core

A console program in C++ that runs the core logic of **inMedia**, a media platform where studios and creators from Central Asia publish films, series, podcasts and lessons.

Final group project, Computer Programming (4-CPR-10-386), Central Asian University, Fall 2026-2027, group CS-1.

| Member | Student ID | Role |
| --- | --- | --- |
| Muhammadamin Nazarov | [ID] | Part A: menu, text checks, search, tests, project setup |
| Feruz Alidjanov | [ID] | Part B: upload planner, catalog files, log-in protection |

See [docs/TASKS.md](docs/TASKS.md) for the exact split.

## What it does

- **Log in** with an e-mail and password. After 5 wrong passwords, log-in is locked for 60 seconds.
- **List** all videos, sorted by title.
- **Search** by title or channel, ignoring capital letters; Russian and Uzbek Cyrillic work too (`шелк` finds `Шёлковый путь`).
- **View** one video by its id, with its web address (`inmedia.uz/watch/silk-road`).
- **Add** a video (after logging in). The program gives it the next id and a web address.
- **Plan an upload**: how many 8 MB parts a file is sent in, and which qualities (1080p, 720p, 480p, 360p) are made for viewers.
- **Save** the catalog to `data/catalog.txt`; changes are also saved when you exit.

Every input is checked: letters where a number is expected, numbers out of range, empty text, invalid e-mail addresses and broken lines in the data file are all handled without crashing.

## Build and run

You need a C++17 compiler (g++ or clang++).

```
make
./inmedia-core
```

Without `make`:

```
g++ -std=c++17 -Wall -Wextra -Isrc src/*.cpp -o inmedia-core
./inmedia-core
```

Run it from this folder so it finds `data/catalog.txt` and `data/users.txt`.

Demo accounts: `demo@inmedia.uz` / `demo12345` and `studio@inmedia.uz` / `studio2026`.

## Tests

```
make test
```

48 automatic checks cover every module: input checks, web addresses, search, part counting, the quality ladder, log-in lock, loading, saving, sorting and searching the catalog.

## Sample session

```
=== inMedia Core ===
1) Log in
...
Choose: abc
  Please enter a number from 0 to 7.
Choose: 6
  File size in MB (1-4096): 20
  Video height in pixels (2-4320): 720
  Upload: 3 parts of 8 MB, the last one 4 MB.
  If the connection drops, only the missing parts are sent again.
  Qualities made for viewers: 720p (2800 kbit/s), 480p (1400 kbit/s), 360p (800 kbit/s)
```

Screenshots: see `docs/screenshots/`.

## Project structure

```
src/
  main.cpp         menu, reading numbers and text safely
  validation.*     trim, e-mail and password checks, web addresses, search matching
  upload.*         part counting and the quality ladder
  auth.*           accounts, password hash, LoginGuard class
  catalog.*        Video and Catalog structures, file loading and saving, adding, finding
  catalog_search.cpp  sorting and searching the catalog
data/
  catalog.txt      the videos: id|type|title|channel|seconds
  users.txt        demo accounts: email|password hash|name
tests/
  run_tests.cpp    runs every test group
  test_*.cpp       one test file per module
docs/
  TASKS.md         who does what
```

## C++ concepts used

Loops and conditions, functions, arrays (fixed-size arrays of structures), strings, structures, a class with private data (`LoginGuard`), pointers (`findById` and `findAccount` return a pointer into the array or `nullptr`), references, file input and output (`ifstream`, `ofstream`), and safe input with `cin.clear()` and `cin.ignore()`.

## Acknowledgements

Parts of this code were written with help from Claude, an AI assistant by Anthropic. Every file says so in its first comment. The team reviewed, tested and can explain every function. The quality ladder and the part counting follow the same rules as the inMedia server.

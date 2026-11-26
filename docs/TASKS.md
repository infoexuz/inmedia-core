# inMedia Core: who does what

Team: Muhammadamin Nazarov (Part A) and Feruz Alidjanov (Part B), group CS-1.
Every file belongs to one person. That person commits it from their own GitHub
account and must be able to explain every line of it. Each of us must also be
ready to answer questions about the other part.

## Part A: Muhammadamin Nazarov — menu, text checks, search, tests

| File | What it is |
| --- | --- |
| README.md, Makefile, .gitignore, docs/TASKS.md | Project setup, build, this plan |
| src/validation.h, src/validation.cpp | trim, isValidEmail, passwordProblem, slugify, containsIgnoreCase |
| src/catalog_search.cpp | sortByTitle (bubble sort), searchCatalog |
| src/main.cpp | The menu, readInt, readLine, every menu action |
| data/catalog.txt, data/users.txt | The 16 demo videos and 2 demo accounts |
| tests/ (all files) | The test helper, the runner and tests for every module |

## Part B: Feruz Alidjanov — upload planner, catalog files, log-in protection

| File | What it is |
| --- | --- |
| src/upload.h, src/upload.cpp | struct Rendition; partCount, partSize, buildLadder (arrays, loops) |
| src/catalog.h, src/catalog.cpp | struct Video and Catalog; splitLine, loadCatalog, saveCatalog, addVideo, findById, formatDuration (file I/O, pointers) |
| src/auth.h, src/auth.cpp | struct Account, class LoginGuard; simpleHash, loadAccounts, findAccount (class, pointers) |

## Shared

- The presentation: Muhammadamin — slides 1, 2, 3, 4, the search on slide 5, and 8; Feruz — parts, ladder and log-in lock on slide 5, 6 and 7.
- The report: each writes the sections about their own files; together the introduction, testing and conclusion.

## Order of work

1. Muhammadamin creates the repository and pushes Part A (6 commits). It does not build yet.
2. Muhammadamin adds Feruz as a collaborator.
3. Feruz clones the repository and pushes Part B (3 commits).
4. Both run `make` and `make test`: all 48 tests must pass.

// test_catalog.cpp - checks for catalog.cpp (files, adding, finding).
// Written with help from Claude (AI assistant); reviewed by the team.
#include <cstdio>

#include "catalog.h"
#include "check.h"

void testCatalog() {
    std::string parts[5];
    check(splitLine("1|movie|Silk Road|Studio|60", '|', parts, 5) == 5 && parts[2] == "Silk Road",
          "splitLine finds 5 fields");

    const std::string path = "test_catalog.txt";
    {
        std::FILE* f = std::fopen(path.c_str(), "w");
        std::fputs("# comment\n2|movie|Zebra|A|60\n1|lesson|apple|B|30\nbroken line\n3|song|X|C|5\n", f);
        std::fclose(f);
    }
    Catalog catalog;
    int skipped = 0;
    check(loadCatalog(catalog, path, skipped) == 2 && skipped == 2,
          "load keeps good lines and skips 2 broken ones");
    Video v;
    v.type = "podcast";
    v.title = "Tech Uz";
    v.channel = "Voices";
    v.durationSec = 90;
    check(addVideo(catalog, v) && catalog.items[2].id == 3 && catalog.items[2].slug == "tech-uz",
          "add gives the next id and a slug");
    check(findById(catalog, 3) == &catalog.items[2], "findById returns a pointer to the video");
    check(findById(catalog, 99) == nullptr, "findById returns nullptr when missing");

    check(saveCatalog(catalog, path), "save writes the file");
    Catalog again;
    check(loadCatalog(again, path, skipped) == 3 && skipped == 0, "saved file loads back the same");
    std::remove(path.c_str());

    int missing = 0;
    check(loadCatalog(again, "no_such_file.txt", missing) == -1, "missing file returns -1");
    check(formatDuration(95) == "1:35" && formatDuration(3725) == "1:02:05", "durations print well");
}

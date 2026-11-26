// test_search.cpp - checks for catalog_search.cpp (sorting and searching).
// Written with help from Claude (AI assistant); reviewed by the team.
#include "check.h"
#include "catalog.h"

void testSearch() {
    Catalog catalog;
    Video v;
    v.type = "movie";
    v.channel = "Silk Road Studio";
    v.durationSec = 60;
    v.title = "Zebra";
    addVideo(catalog, v);
    v.title = "apple";
    addVideo(catalog, v);
    v.title = "Шёлковый путь";
    v.channel = "Steppe Films";
    addVideo(catalog, v);

    sortByTitle(catalog);
    check(catalog.items[0].title == "apple" && catalog.items[1].title == "Zebra",
          "sort ignores capital letters");

    int results[MAX_VIDEOS];
    check(searchCatalog(catalog, "ZEB", results, MAX_VIDEOS) == 1, "search finds by title");
    check(searchCatalog(catalog, "шелк", results, MAX_VIDEOS) == 1, "search finds a Cyrillic title");
    check(searchCatalog(catalog, "steppe", results, MAX_VIDEOS) == 1, "search finds by channel");
    check(searchCatalog(catalog, "nothing here", results, MAX_VIDEOS) == 0, "search can find nothing");
}

// catalog_search.cpp - sorting and searching the catalog (declared in catalog.h).
// Part of inMedia Core. Written with help from Claude (AI assistant);
// every function was reviewed and can be explained by the team.
#include "catalog.h"
#include "validation.h"

void sortByTitle(Catalog& catalog) {
    for (int pass = 0; pass < catalog.count - 1; ++pass) {
        bool swapped = false;
        for (int i = 0; i < catalog.count - 1 - pass; ++i) {
            if (toLowerAscii(catalog.items[i].title) > toLowerAscii(catalog.items[i + 1].title)) {
                Video temp = catalog.items[i];
                catalog.items[i] = catalog.items[i + 1];
                catalog.items[i + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) {
            break;  // already sorted, stop early
        }
    }
}

int searchCatalog(const Catalog& catalog, const std::string& query, int results[], int maxResults) {
    int found = 0;
    for (int i = 0; i < catalog.count && found < maxResults; ++i) {
        const Video& v = catalog.items[i];
        if (containsIgnoreCase(v.title, query) || containsIgnoreCase(v.channel, query)) {
            results[found] = i;
            ++found;
        }
    }
    return found;
}

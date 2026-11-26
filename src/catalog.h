// catalog.h - the list of videos: adding, sorting, searching, saving to a file.
// Part of inMedia Core. Written with help from Claude (AI assistant);
// every function was reviewed and can be explained by the team.
#ifndef INMEDIA_CATALOG_H
#define INMEDIA_CATALOG_H

#include <string>

const int MAX_VIDEOS = 100;
const int TYPE_COUNT = 4;
const std::string VIDEO_TYPES[TYPE_COUNT] = {"movie", "episode", "podcast", "lesson"};

struct Video {
    int id;
    std::string type;     // one of VIDEO_TYPES
    std::string title;
    std::string channel;
    int durationSec;
    std::string slug;     // web address made from the title
};

// A fixed-size array of videos plus how many places are in use.
struct Catalog {
    Video items[MAX_VIDEOS];
    int count = 0;
};

// Splits "a|b|c" at `separator` into `parts`. Returns how many parts were found
// (at most maxParts).
int splitLine(const std::string& line, char separator, std::string parts[], int maxParts);

// True when `type` is one of VIDEO_TYPES.
bool isKnownType(const std::string& type);

// Reads "id|type|title|channel|seconds" lines into the catalog. Returns how many
// videos were loaded, or -1 when the file cannot be opened. Broken lines are
// skipped and counted in `skipped`.
int loadCatalog(Catalog& catalog, const std::string& path, int& skipped);

// Writes every video to the file in the same format. False when it cannot write.
bool saveCatalog(const Catalog& catalog, const std::string& path);

// Adds a copy of `video`, giving it the next free id and a slug. False when full.
bool addVideo(Catalog& catalog, Video video);

// Sorts by title, A to Z, ignoring capital letters (bubble sort).
void sortByTitle(Catalog& catalog);

// Writes the positions of matching videos into `results`. A video matches when
// the query is in its title or channel name. Returns how many matched.
int searchCatalog(const Catalog& catalog, const std::string& query, int results[], int maxResults);

// Returns a pointer to the video with this id, or nullptr when there is none.
Video* findById(Catalog& catalog, int id);

// 95 -> "1:35", 3725 -> "1:02:05".
std::string formatDuration(int seconds);

#endif

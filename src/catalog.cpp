// catalog.cpp - see catalog.h.
// Written with help from Claude (AI assistant); reviewed by the team.
#include "catalog.h"

#include <fstream>
#include <sstream>

#include "validation.h"

int splitLine(const std::string& line, char separator, std::string parts[], int maxParts) {
    int count = 0;
    std::string current;
    for (char c : line) {
        if (c == separator) {
            if (count < maxParts) {
                parts[count] = current;
            }
            ++count;
            current.clear();
        } else {
            current += c;
        }
    }
    if (count < maxParts) {
        parts[count] = current;
    }
    ++count;
    return count < maxParts ? count : maxParts;
}

bool isKnownType(const std::string& type) {
    for (int i = 0; i < TYPE_COUNT; ++i) {
        if (VIDEO_TYPES[i] == type) {
            return true;
        }
    }
    return false;
}

namespace {

// Reads a whole positive number; false for "", "12a", "-3" and so on.
bool parsePositiveInt(const std::string& text, int& value) {
    if (text.empty() || text.size() > 9) {
        return false;
    }
    value = 0;
    for (char c : text) {
        if (c < '0' || c > '9') {
            return false;
        }
        value = value * 10 + (c - '0');
    }
    return value > 0;
}

}  // namespace

int loadCatalog(Catalog& catalog, const std::string& path, int& skipped) {
    skipped = 0;
    std::ifstream file(path);
    if (!file) {
        return -1;
    }
    catalog.count = 0;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        const int FIELDS = 5;
        std::string parts[FIELDS];
        int id = 0;
        int seconds = 0;
        if (splitLine(line, '|', parts, FIELDS) != FIELDS || !parsePositiveInt(parts[0], id) ||
            !isKnownType(parts[1]) || trim(parts[2]).empty() ||
            !parsePositiveInt(parts[4], seconds) || catalog.count >= MAX_VIDEOS) {
            ++skipped;
            continue;
        }
        Video& video = catalog.items[catalog.count];
        video.id = id;
        video.type = parts[1];
        video.title = trim(parts[2]);
        video.channel = trim(parts[3]);
        video.durationSec = seconds;
        video.slug = slugify(video.title);
        ++catalog.count;
    }
    return catalog.count;
}

bool saveCatalog(const Catalog& catalog, const std::string& path) {
    std::ofstream file(path);
    if (!file) {
        return false;
    }
    file << "# id|type|title|channel|seconds\n";
    for (int i = 0; i < catalog.count; ++i) {
        const Video& v = catalog.items[i];
        file << v.id << '|' << v.type << '|' << v.title << '|' << v.channel << '|'
             << v.durationSec << '\n';
    }
    return static_cast<bool>(file);
}

bool addVideo(Catalog& catalog, Video video) {
    if (catalog.count >= MAX_VIDEOS) {
        return false;
    }
    int nextId = 1;
    for (int i = 0; i < catalog.count; ++i) {
        if (catalog.items[i].id >= nextId) {
            nextId = catalog.items[i].id + 1;
        }
    }
    video.id = nextId;
    video.slug = slugify(video.title);
    catalog.items[catalog.count] = video;
    ++catalog.count;
    return true;
}

Video* findById(Catalog& catalog, int id) {
    for (int i = 0; i < catalog.count; ++i) {
        if (catalog.items[i].id == id) {
            return &catalog.items[i];
        }
    }
    return nullptr;
}

std::string formatDuration(int seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    std::ostringstream out;
    if (hours > 0) {
        out << hours << ':' << (minutes < 10 ? "0" : "");
    }
    out << minutes << ':' << (secs < 10 ? "0" : "") << secs;
    return out.str();
}

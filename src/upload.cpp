// upload.cpp - see upload.h.
// Written with help from Claude (AI assistant); reviewed by the team.
#include "upload.h"

namespace {

// The quality ladder, tallest first (the same steps the inMedia server uses).
const Rendition LADDER[MAX_RENDITIONS] = {
    {1080, 5000},
    {720, 2800},
    {480, 1400},
    {360, 800},
};

}  // namespace

long long partCount(long long sizeBytes, long long chunkBytes) {
    if (sizeBytes <= 0 || chunkBytes <= 0) {
        return 0;
    }
    return (sizeBytes + chunkBytes - 1) / chunkBytes;
}

long long partSize(long long sizeBytes, long long chunkBytes, long long part) {
    long long parts = partCount(sizeBytes, chunkBytes);
    if (part < 1 || part > parts) {
        return 0;
    }
    if (part < parts) {
        return chunkBytes;
    }
    return sizeBytes - (parts - 1) * chunkBytes;  // the last part
}

int buildLadder(int sourceHeight, Rendition out[], int maxOut) {
    if (sourceHeight < 2 || maxOut < 1) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < MAX_RENDITIONS && count < maxOut; ++i) {
        if (LADDER[i].height <= sourceHeight) {
            out[count] = LADDER[i];
            ++count;
        }
    }
    if (count == 0) {
        // Smaller than every step: keep the source size, rounded down to even
        // because video encoders need an even height.
        out[0].height = sourceHeight - sourceHeight % 2;
        out[0].bitrateKbps = 500;
        count = 1;
    }
    return count;
}

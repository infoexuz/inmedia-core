// upload.h - planning how a video file is uploaded and which qualities it gets.
// Part of inMedia Core. Written with help from Claude (AI assistant);
// every function was reviewed and can be explained by the team.
#ifndef INMEDIA_UPLOAD_H
#define INMEDIA_UPLOAD_H

const long long BYTES_PER_MB = 1024LL * 1024LL;
const long long CHUNK_BYTES = 8 * BYTES_PER_MB;  // inMedia sends files in 8 MB parts

// One quality of the finished video, for example 720p at 2800 kbit/s.
struct Rendition {
    int height;       // picture height in pixels
    int bitrateKbps;  // target bitrate
};

const int MAX_RENDITIONS = 4;

// How many parts a file of `sizeBytes` is split into: division rounded up,
// (size + chunk - 1) / chunk. Returns 0 for a size or chunk that is not positive.
long long partCount(long long sizeBytes, long long chunkBytes);

// Size of part number `part` (1, 2, ...). Every part is `chunkBytes` except the
// last, which holds what is left. Returns 0 for a part number out of range.
long long partSize(long long sizeBytes, long long chunkBytes, long long part);

// Fills `out` with the qualities made for a video `sourceHeight` pixels tall:
// every step of 1080/720/480/360 that is not taller than the source. A source
// smaller than 360 gets one quality of its own height (made even). Returns how
// many were written, at most `maxOut`; 0 when the height is below 2.
int buildLadder(int sourceHeight, Rendition out[], int maxOut);

#endif

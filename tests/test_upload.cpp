// test_upload.cpp - checks for upload.cpp.
// Written with help from Claude (AI assistant); reviewed by the team.
#include "check.h"
#include "upload.h"

void testUpload() {
    long long mb = BYTES_PER_MB;
    check(partCount(20 * mb, 8 * mb) == 3, "20 MB in 8 MB parts is 3 parts");
    check(partCount(16 * mb, 8 * mb) == 2, "16 MB in 8 MB parts is exactly 2 parts");
    check(partCount(0, 8 * mb) == 0, "empty file has 0 parts");
    check(partCount(-5, 8 * mb) == 0, "negative size has 0 parts");
    check(partSize(20 * mb, 8 * mb, 3) == 4 * mb, "last part of 20 MB is 4 MB");
    check(partSize(20 * mb, 8 * mb, 4) == 0, "part number past the end is 0");

    Rendition out[MAX_RENDITIONS];
    int n = buildLadder(720, out, MAX_RENDITIONS);
    check(n == 3 && out[0].height == 720 && out[2].height == 360, "720p source gives 720/480/360");
    n = buildLadder(2160, out, MAX_RENDITIONS);
    check(n == 4 && out[0].height == 1080, "4K source gives all four qualities");
    n = buildLadder(201, out, MAX_RENDITIONS);
    check(n == 1 && out[0].height == 200, "small source keeps its own even height");
    check(buildLadder(1, out, MAX_RENDITIONS) == 0, "height below 2 gives nothing");
}

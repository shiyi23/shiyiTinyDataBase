#include "DiskFile.h"

DiskFile::DiskFile()
{
    const int BLOCK_SIZE_UP_LIMIT = 1024 * 1024 *2;
    const int BLOOM_FILTER_HASH_COUNT = 3;
    const int BLOOM_FILTER_BITS_PER_KEY = 10;

    const int TRAILER_SIZE = 8 + 4 + 8 + 8 + 8;
    const long DISK_FILE_MAGIC = 0xFAC881234221FFA9L;
}




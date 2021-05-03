#ifndef DISKFILE_H_
#define DISKFILE_H_

#include "KeyValue.h"
#include "Bytes.h"
#include <deque>
#include <string>
//header files fro file I/O
#include <iostream>
#include <fstream>
#include <set>

class DiskFile
{
public:
    DiskFile();
    virtual ~DiskFile();
    static const int BLOCK_SIZE_UP_LIMIT;
    static const int BLOOM_FILTER_HASH_COUNT;
    static const int BLOOM_FILTER_BITS_PER_KEY;

    // fileSize(8B)+ blockCount(4B) + blockIndexOffset(8B) + blockIndexOffset(8B) + DISK_FILE_MAGIC(8B)
    static const int TRAILER_SIZE;
    static const long DISK_FILE_MAGIC;

    class BlockMeta;//先在外部类的public域声明内部类，再在外部类的外部实现该内部类

private:
    std::string fname;
    //open file
    std::ifstream file;
    std::set<DiskFile::BlockMeta> blockMetaSet;

    long fileSize;
    int blockCount;
    long blockIndexOffset;
    long blockIndexSize;
        
};

//实现内部类
class DiskFile::BlockMeta
{
public:
    BlockMeta()
    {
        const int OFFSET_SIZE = 8;
        const int SIZE_SIZE = 8;
        const int BF_LEN_SIZE = 4;
    }
    BlockMeta(KeyValue& lastKV, long offset, long size, std::deque<unsigned char>& bloomFilter)
    {
        this->lastKV = lastKV;
        this->blockOffset = offset;
        this->blockSize = size;
        this->bloomFilter = bloomFilter;
    }
    KeyValue getLastKV();
    long getBlockOffset();
    long getBlockSize();
    std::deque<unsigned char> getBloomFilter();
    int getSerializeSize();
    std::deque<unsigned char> toBytes();
    static BlockMeta parseFrom(std::deque<unsigned char>& buf, int offset);
    int compareTo(BlockMeta o);
    virtual ~BlockMeta();

private:
    /* data */
    static const int OFFSET_SIZE;
    static const int SIZE_SIZE;
    static const int BF_LEN_SIZE;
    
    KeyValue lastKV;
    long blockOffset;
    long blockSize;
    std::deque<unsigned char> bloomFilter;
    BlockMeta createSeekDummy(KeyValue& lastKV)
    {
        BlockMeta blockMeta(lastKV, 0L, 0L, Bytes::EMPTY_BYTES);
    }

};


#endif
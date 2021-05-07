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
    class BlockIndexWriter;
    class BlockWriter;
    class BlockReader;
    class DiskFileWriter;

    void open(std::string filename);
    std::string getFileName();

private:
    std::string fname;
    //open file
    std::ifstream in;
    std::set<DiskFile::BlockMeta> blockMetaSet;

    long fileSize;
    int blockCount;
    long blockIndexOffset;
    long blockIndexSize;

    BlockReader load(BlockMeta meta);
    //声明在private域的类无法在外部实现，所以需要在声明的同时直接实现
    class InternalIterator
    {
    private:
        int currentKVIndex = 0;
        BlockReader currentReader;
        Iterator<BlockMeta> blockMetaIter;
        bool nextBlockReader()
        {
            if (blockMetaIter.hasNext())
            {
                ....
            } else {
                ....
            }
        }
    public:
        InternalIterator()
        {
            currentReader = nullptr;
            blockMetaIter = blockMetaSet.iterator();
        }

        bool hasNext()
        {
            if (currentReader 等于 nullptr) {
                return nextBlockReader();
            } else {
                if (currentKVIndex < currentReader.getKeyValues().size()) 
                {
                    ....
                }
            }
        }

    };

    SeekIter<KeyValue> iterator();
    void close();
};

//实现内部类BlockMeta
class DiskFile::BlockMeta
{
public:
    BlockMeta()
    {
        int OFFSET_SIZE = 8;
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
    KeyValue getLastKV() 
    {
        return this->lastKV;
    }
    long getBlockOffset()
    {
        return this->blockOffset;
    }
    long getBlockSize()
    {
        return this->blockSize;
    }
    std::deque<unsigned char> getBloomFilter()
    {
        return this->bloomFilter;
    }
    int getSerializeSize()
    {
        return lastKV.getSerializeSize() + OFFSET_SIZE + SIZE_SIZE + BF_LEN_SIZE + bloomFilter.size();
    }
    std::deque<unsigned char> toBytes()
    {
        std::deque<unsigned char> bytes(getSerializeSize());
        int pos = 0;
        auto posForCopy = bytes.begin();

        //Encode last kv
        std::deque<unsigned char> kvBytes = lastKV.toBytes();
        for (int i = 0; i < pos; i++)
        {
            ++posForCopy;
        }
        std::copy_n(kvBytes.begin(), kvBytes.size(), posForCopy);
        pos += kvBytes.size();

        //Encode blockOffset
        std::deque<unsigned char> offsetBytes = Bytes::toBytes(blockOffset);
        for (int i = 0; i < pos; i++)
        {
            ++posForCopy;
        }
        std::copy_n(offsetBytes.begin(), offsetBytes.size(), posForCopy);
        pos += offsetBytes.size();

        //Encode blockSize
        std::deque<unsigned char> sizeBytes = Bytes::toBytes(blockSize);
        for (int i = 0; i < pos; i++)
        {
            ++posForCopy;
        }
        std::copy_n(sizeBytes.begin(), sizeBytes.size(), posForCopy);
        pos += sizeBytes.size();

        //Encode length of bloom filter
        std::deque<unsigned char> bfLenBytes = Bytes::toBytes(bloomFilter.size());
        for (int i = 0; i < pos; i++)
        {
            ++posForCopy;
        }
        std::copy_n(bfLenBytes.begin(), bfLenBytes.size(), posForCopy);
        pos += bfLenBytes.size();

        //Encode bytes of bloom filter
        for (int i = 0; i < pos; i++)
        {
            ++posForCopy;
        }
        std::copy_n(bloomFilter.begin(), bloomFilter.size(), posForCopy);
        pos += bloomFilter.size();
        
        if (pos != bytes.size())
        {   
            std::cout << "pos = " << pos << "\n";
            std::cout << "bytes.size() = " << bytes.size() << "\n";
            throw "pos should be equal to the size of bytes";
        }
        return bytes;
    }

    static BlockMeta parseFrom(std::deque<unsigned char>& buf, int offset)
    {
        int pos = offset;

        //Decode last key value
        KeyValue lastKV = KeyValue::parseFrom(buf, offset);
        pos += lastKV.getSerializeSize();

        //Decode block blockOffset
        std::deque<unsigned char> temp0 = Bytes::slice(buf, pos, OFFSET_SIZE);
        long blockOffset = Bytes::toLong(temp0);
        pos += OFFSET_SIZE;

        //Decode block blockSize
        std::deque<unsigned char> temp1 = Bytes::slice(buf, pos, SIZE_SIZE);
        pos += SIZE_SIZE;

        //Decode blockSize of block's bloom filter
        std::deque<unsigned char> temp2 = Bytes::slice(buf, pos, BF_LEN_SIZE);
        int bloomFilterSize = Bytes::toInt(temp2);
        pos += BF_LEN_SIZE;

        //Decode bytes of block's bloom filter
        
    }
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
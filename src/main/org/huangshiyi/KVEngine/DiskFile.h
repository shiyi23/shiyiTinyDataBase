#ifndef DISKFILE_H_
#define DISKFILE_H_

#include "KeyValue.h"
#include "Bytes.h"
#include "BloomFilter.h"
#include <deque>
#include <string>
//header files fro file I/O
#include <iostream>
#include <fstream>
#include <set>
#include <assert.h>
#include <list>
#include <algorithm>

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
    //???????????????????????????????????/
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
        std::deque<unsigned char> bloomFilter = Bytes::slice(buf, pos, bloomFilterSize);
        pos += bloomFilterSize;

        assert(pos <= buf.size());//header file: <assert.h>
        BlockMeta blockMeta(lastKV, blockOffset, BlockMeta::blockSize, bloomFilter);
        return blockMeta;
    }
    int compareTo(BlockMeta o) 
    {
        int res = this->lastKV.compareTo(o.lastKV);
        return res;
    }
    virtual ~BlockMeta();

private:
    /* data */
    static const int OFFSET_SIZE;
    static const int SIZE_SIZE;
    static const int BF_LEN_SIZE;
    
    KeyValue lastKV;
    long blockOffset;
    static long blockSize;
    std::deque<unsigned char> bloomFilter;
    BlockMeta createSeekDummy(KeyValue& lastKV)
    {
        BlockMeta blockMeta(lastKV, 0L, 0L, Bytes::EMPTY_BYTES);
    }

};

class DiskFile::BlockIndexWriter
{
public:
    void append(KeyValue& lastKV, long offset, long size, std::deque<unsigned char> bloomFilter)
    {
        DiskFile::BlockMeta meta(lastKV, offset, size, bloomFilter);
        blockMetas.push_back(meta);
        totalBytes += meta.getSerializeSize();
    }

    std::deque<unsigned char> serialize()
    {
        std::deque<unsigned char> buffer(totalBytes);
        int pos = 0;
        for (auto meta : blockMetas)
        {
            std::deque<unsigned char> metaBytes = meta.toBytes();
            auto posForCopy = buffer.begin();
            for (int i = 0; i < 0; i++)
            {
                ++posForCopy;
            }
            
            std::copy_n(metaBytes.begin(), metaBytes.size(), posForCopy);
            pos += metaBytes.size();
        }
        assert(pos == totalBytes);
        return buffer;
    }

private:
    std::list<DiskFile::BlockMeta> blockMetas;
    int totalBytes = 0;
};

class DiskFile::BlockWriter
{
public:
    static const int KV_SIZE_LEN = 4;
    static const int CHECKSUM_LEN = 4;

        BlockWriter() {
        totalSize = 0; 
        kvBuf = std::deque<KeyValue>(0);
        bloomFilter = BloomFilter(DiskFile::BLOOM_FILTER_HASH_COUNT, DiskFile::BLOOM_FILTER_BITS_PER_KEY);
        //????????????????????????????????????????????????????
        crc32 = CRC32();
    }

    void append(KeyValue& kv) {
        kvBuf.push_back(kv);
        lastKV = kv;

        //Update checksum
        std::deque<unsigned char> buf = kv.toBytes();
        //下面这个update()方法还没有实现
        crc32.update(buf, 0, buf.size());

        totalSize += kv.getSerializeSize();
        KeyValueCount += 1;
    }

    std::deque<unsigned char> getBloomFilter() {
        std::deque<std::deque<unsigned char>> bytes(kvBuf.size() );
        for (int i = 0; i < kvBuf.size(); i++) {
            /* code */
            bytes[i] = kvBuf[i].getKey();
        }
        return bloomFilter.generate(bytes);
    }

    int getChecksum() {
        //下面的getValue()方法同样没有实现
        return (int) crc32.getValue();
    }

    KeyValue getLastKV() {
        return this->lastKV;
    }

    int size() {
        return KV_SIZE_LEN + totalSize + CHECKSUM_LEN;
    }

    int getKeyValueCount() {
        return KeyValueCount;
    }

    std::deque<unsigned char> serialize() {
        std::deque<unsigned char> buffer(size());
        int pos = 0;
        auto posForCopy = buffer.begin();

        //Append kv getSerializeSize.
        std::deque<unsigned char> kvSize = Bytes::toBytes(kvBuf.size());
        for (int i = 0; i < pos; i++)
        {
            /* code */
            ++posForCopy;
        }
        std::copy_n(kvSize.begin(), kvSize.size(), posForCopy);
        pos += kvSize.size();

        //Append all the key value.
        for (int i = 0; i < kvBuf.size(); i++) {
            for (int i = 0; i < pos; i++) {
                ++posForCopy;
            }
            std::deque<unsigned char> kv = kvBuf[i].toBytes();
            std::copy_n(kv.begin(), kv.size(), posForCopy);
            pos += kv.size();
        }
        
        //Append checksum.
        for (int i = 0; i < pos; i++)
        {
            ++posForCopy;
        }
        std::deque<unsigned char> checksum = Bytes::toBytes(this->getChecksum());
        std::copy_n(checksum.begin(), checksum.size(), posForCopy);
        pos += checksum.size();

        assert( pos == size());
        return buffer;
    }

private:
    int totalSize;
    std::deque<KeyValue> kvBuf;
    BloomFilter bloomFilter;
    //Checksum类还有待实现
    Checksum crc32;
    KeyValue lastKV;
    int KeyValueCount;

};

class DiskFile::BlockReader {
public:
    BlockReader() {};

    BlockReader(std::deque<KeyValue>& kvBuf) {
        this->kvBuf = kvBuf;
    }

    static BlockReader parseFrom(std::deque<unsigned char>& buffer, int offset, int size) {
        int pos = 0;
        std::deque<KeyValue> kvBuf(0);
        //这里的Chesum类还没有实现
        Checksum crc32;

        //Parse kv getSerializeSize
        std::deque<unsigned char> temp0 = Bytes::slice(buffer, offset + pos, BlockWriter::KV_SIZE_LEN);
        int kvSize = Bytes::toInt(temp0);
        pos += BlockWriter::KV_SIZE_LEN;

        //parse all key value
        for (int i = 0; i < kvSize; i++)
        {
            KeyValue kv = KeyValue::parseFrom(buffer, offset + pos);
            kvBuf.push_back(kv);
            //这里的crc32的例如update()相关方法也需要用C++重新实现一遍
            crc32.update(buffer, offset + pos, kv.getSerializeSize());
            pos += kv.getSerializeSize();
        }
        
        //parse checksum
        std::deque<unsigned char> temp1 = Bytes::slice(buffer, offset + pos, BlockWriter::CHECKSUM_LEN);
        int checksum = Bytes::toInt(temp1);
        pos += BlockWriter::CHECKSUM_LEN;
        //这里的crc32.getValue()也有待实现.????????????????????????????????????????????????????
        assert(checksum == (int) (crc32.getValue() & 0xFFFFFFFF) );

        //???????????????????????????????????????????????????????????????????????
        assert(pos == size : "pos: " + pos + ", getSerializeSize: " + size);

        BlockReader newKVBuf;
        return newKVBuf;
    }

    std::deque<KeyValue> getKeyValues() {
        return this->kvBuf;
    }

private:
    std::deque<KeyValue> kvBuf;
};

class DiskFile::DiskFileWriter {
public:
    

private:
    std::string fname;

    long currentOffset;
    BlockIndexWriter indexWriter;
    BlockWriter currentWriter;

};


#endif
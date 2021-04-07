#ifndef BLOOMFILTER_H_
#define BLOOMFILTER_H_

#include <deque>

class BloomFilter
{
private:
    /* data */
    int k;
    int bitsPerKey;
    int bitLen;
    unsigned char* result;
public:
    BloomFilter(/* args */);
    BloomFilter(int& k, int& bitsPerKey);
    ~BloomFilter();
    unsigned char* generate(std::deque<std::deque<unsigned char* >>& keys);
    bool contains(unsigned char* key);
};

BloomFilter::BloomFilter(/* args */)
{
}

BloomFilter::~BloomFilter()
{
}



#endif
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
    std::deque<unsigned char> result;
public:
    BloomFilter(/* args */);
    BloomFilter(int& k, int& bitsPerKey);
    ~BloomFilter();
    std::deque<unsigned char> generate(std::deque<std::deque<unsigned char >>& keys);//函数参数是二维数组
    bool contains(std::deque<unsigned char>& key);
};

BloomFilter::BloomFilter(/* args */)
{
}

BloomFilter::~BloomFilter()
{
}



#endif
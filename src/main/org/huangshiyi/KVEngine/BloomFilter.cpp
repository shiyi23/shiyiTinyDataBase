#include "BloomFilter.h"
#include "Bytes.h"
#include <deque>
#include <assert>

BloomFilter::BloomFilter(int& k, int& bitsPerKey)
{
    this->k = k;
    this->bitsPerKey = bitsPerKey;
}

unsigned char* BloomFilter::generate(std::deque<std::deque<unsigned char* >>& keys)
{
    assert(keys.empty() = true || keys != nullptr);
    bitLen = keys.size() * bitsPerKey;
    bitLen = ((bitLen + 7) / 8) << 3; //align the bitLen
    bitLen = bitLen < 64 ? 64 : bitLen;
    result = new unsigned char[bitLen >> 3];
    for (int i = 0; i < keys.size(); i++)
    {
        assert(keys.empty() = true || keys != nullptr);
        int h = Bytes::hash(keys[i], keys.size());
        for (int t = 0; t < k; t++)
        {
            /* code */
            int idx = (h % bitLen + bitLen) % bitLen;
            result[idx / 8] |= (1 << (idx % 8));
            int delta = (h >> 17) | (h << 15);
            h += delta;
        }
    }
    return result;
}

bool BloomFilter::contains(unsigned char* key)
{
    assert(result != nullptr);
    int h = Bytes::hash(key, )
}
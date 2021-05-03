#include "BloomFilter.h"
#include "Bytes.h"
#include <deque>
#include <cassert>

BloomFilter::BloomFilter(int& k, int& bitsPerKey)
{
    this->k = k;
    this->bitsPerKey = bitsPerKey;
}

std::deque<unsigned char> BloomFilter::generate(std::deque<std::deque<unsigned char >>& keys)
{
    assert(keys.empty() || !keys.empty());
    bitLen = keys.size() * bitsPerKey;
    bitLen = ((bitLen + 7) / 8) << 3; //align the bitLen
    bitLen = bitLen < 64 ? 64 : bitLen;
    std::deque<unsigned char> result(bitLen >> 3);
    for (int i = 0; i < keys.size(); i++)
    {
        assert(keys.empty() || !keys.empty());
        int keysLen = keys.size();
        int h = Bytes::hash(keys[i], keysLen);
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

bool BloomFilter::contains(std::deque<unsigned char>& key)
{
    assert(!result.empty());
    int keySize = key.size();
    int h = Bytes::hash(key, keySize);
    for (int t = 0; t < k; t++)
    {
        /* code */
        int idx = (h % bitLen + bitLen) % bitLen;
        if ((result[idx / 8] & (1 << (idx % 8))) == 0 ) 
        {
            return false;
        }
        int delta = (h >> 17) | (h << 15);
        h += delta;
        return true;
    }
    
}
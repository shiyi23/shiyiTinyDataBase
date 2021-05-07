#include "Bytes.h"
#include <cstring>
#include <deque>
#include <algorithm>
#include <iostream>

Bytes::Bytes(){
    std::deque<unsigned char> EMPTY_BYTES;
    std::string HEX_TMP = "0123456789ABCDEF";
}

std::deque<unsigned char> Bytes::toBytes(unsigned char& b) {
    std::deque<unsigned char> temp;
    temp[0] = b;
    return temp;
}
/**
 * @brief 
 * Convert std::string to char array in C++.
 * @param s 
 * @return  std::deque<unsigned char>
 */
std::deque<unsigned char> Bytes::toBytes(std::string& s) {
    if (s.empty()) {
        std::deque<unsigned char> temp;
        return temp;
    }
    std::deque<unsigned char> res;
    for (int i = 0; i < s.size(); i++)
    {
        /* code */
        res.push_back(s[i]);
    }
    
    return res;
}
/**
 * @brief 
 * ???????????????????????????????????????
 * @param x 
 * @return std::deque<unsigned char> 
 */
std::deque<unsigned char> Bytes::toBytes(int x) {
    std::deque<unsigned char> b;
    b[3] = (unsigned char) (x & 0xFF );
    b[2] = (unsigned char) ( (x >> 8) );
    b[1] = (unsigned char) ( (x >> 16) );
    b[0] = (unsigned char) ( (x >> 24) );
    return b;
}
/**
 * @brief 
 * ????????????????????????????????????
 * @param x 
 * @return std::deque<unsigned char>
 */
std::deque<unsigned char> Bytes::toBytes(long& x) {
    std::deque<unsigned char> b;
    for (int i = 7; i >= 0; i--){
        int j = (7 - i) << 3;
        b[i] = (unsigned char) ((x >> j) & 0xFF );
    }
    
    return b;
}

std::string Bytes::toHex(std::deque<unsigned char>& buf, int& buf_array_len) {
    return toHex(buf, 0, buf.size() );
}
std::string Bytes::toHex(std::deque<unsigned char>& buf, int offset, int len) {
    std::string str;
    for (int i = offset; i < offset + len; i++)
    {
        int x = buf[i];
        if (x > 32 && x < 127) {
            str += ( (char) x);
        } else {
            str.append("\\x").append(std::to_string(HEX_TMP.at( (x >> 4) & 0x0F )) )
            .append( std::to_string( HEX_TMP.at(x & 0x0F) ) );
        }
        return str;
    }
}

std::deque<unsigned char> Bytes::toBytes(std::deque<unsigned char>& a, std::deque<unsigned char>& b, int& a_len, int& b_len )  
{
    if (a.empty() ) return b;
    if (b.empty() ) return a;
    std::deque<unsigned char> result(a.size() + b.size());
    std::move(a.begin(), a.end(), result.begin());

    std::move(b.begin(), b.end(), result.end());
    return result;
}
/**
 * @brief 
 * ??????????????????????????????????????????????
 * @param a 
 * @return int 
 */
int toInt(std::deque<unsigned char>& a) {
    return (a[0] << 24) & 0xFF000000 | (a[1] << 16) & 0x00FF0000 | (a[2] << 8) & 0x0000FF00
    | (a[3] << 0) & 0x000000FF;
}
long toLong(std::deque<unsigned char>& a) {
    long x = 0;
    for (int i = 0; i < 8; i++)
    {
        /* code */
        int j = (7 - i) << 3;
        x |= ((0xFFL << j) & ((long) a[i] << j));
    }
    return x;
}
std::deque<unsigned char> slice(std::deque<unsigned char>& buf, int offset, int len) {
    if (buf.empty() ) {
        throw "buffer is empty";
    }

    if (offset < 0 || len < 0) {
        std::cout << "Invalid offset or len! " << "offset = " << offset << "len = " << len << "\n";
        throw "Invalid offset or len!";
    }

    if (offset + len > buf.size() ) {
        std::cout << "Buffer overflow, offset: " << offset << ", len: " << len <<"\n";
        throw "Buffer overflow";
    }
    std::deque<unsigned char> result(len);
    auto sourceEnd = buf.begin();
    for (int i = 0; i < offset; i++)
    {
        ++sourceEnd;
    }
    auto destPos = buf.begin();
    for (int i = 0; i < len; i++)
    {
        ++destPos;
    }
    std::move_backward(buf.begin(), sourceEnd,  destPos);
    return result;
}
//TODO: 换一个性能更高的哈希函数
int hash(std::deque<unsigned char>& key, int& key_len) 
{
    if (key.empty() ) return 0;
    int h = 1;
    for (size_t i = 0; i < key_len; i++)
    {
        /* code */
        h = (h << 5) + h + key[i];
    }
    return h;
}
int compare(std::deque<unsigned char>& a, std::deque<unsigned char>& b, int& a_len, int& b_len) 
{
    if (a == b) return 0;
    if (a.empty() ) return -1;
    if (b.empty() ) return 1;
    for (int i = 0, j = 0; i < a_len && j < b_len; ++i, ++j) 
    {
        /* code */
        int x = a[i] & 0xFF;
        int y = b[i] & 0xFF;
        if (x != y) 
        {
            return x - y;
        }
    }
    return a_len - b_len;
}

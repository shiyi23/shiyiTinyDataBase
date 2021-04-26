#ifndef BYTES_H_
#define BYTES_H_

#include <string>
#include <deque>

class Bytes
{
private:
    /* data */
public:
    Bytes(/* args */);
    ~Bytes();

    const static std::deque<unsigned char> EMPTY_BYTES;

    const static std::string HEX_TMP;

    static std::deque<unsigned char> toBytes(unsigned char& b);

    static std::deque<unsigned char> toBytes(std::string& s);

    static std::deque<unsigned char> toBytes(int& x);

    static std::deque<unsigned char> toBytes(long& x);

    static std::string toHex(std::deque<unsigned char>& buf, int& buf_array_len);

    static std::string toHex(std::deque<unsigned char>& buf, int offset, int len);

    static std::deque<unsigned char> toBytes(std::deque<unsigned char>& a, std::deque<unsigned char>& b, int& a_len, int& b_len);

    static int toInt(std::deque<unsigned char>& a);

    static long toLong(std::deque<unsigned char>& a);

    static std::deque<unsigned char> slice(std::deque<unsigned char>& buf, int& offset, int& len, int& buf_len);

    static int hash(std::deque<unsigned char>& key, int& key_len);

    static int compare(std::deque<unsigned char>& a, std::deque<unsigned char>& b);
};

Bytes::Bytes(/* args */)
{
}

Bytes::~Bytes()
{
}


#endif 
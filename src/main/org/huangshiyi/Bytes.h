#ifndef BYTES_H_
#define BYTES_H_

#include <string>

class Bytes
{
private:
    /* data */
public:
    Bytes(/* args */);
    ~Bytes();

    const static unsigned char* EMPTY_BYTES;

    const static std::string HEX_TMP;

    static unsigned char* toBytes(unsigned char& b);

    static unsigned char* toBytes(std::string& s);

    static unsigned char* toBytes(int& x);

    static unsigned char* toBytes(long& x);

    static std::string toHex(unsigned char* buf, int& buf_array_len);

    static std::string toHex(unsigned char* buf, int& offset, int& len);

    static unsigned char* toBytes(unsigned char* a, unsigned char* b, int& a_len, int& b_len);

    static int toInt(unsigned char* a);

    static long toLong(unsigned char* a);

    static unsigned char* slice(unsigned char* buf, int& offset, int& len, int& buf_len);

    static int hash(unsigned char* key, int& key_len);

    static int compare(unsigned char* a, unsigned char* b);
};

Bytes::Bytes(/* args */)
{
}

Bytes::~Bytes()
{
}


#endif 
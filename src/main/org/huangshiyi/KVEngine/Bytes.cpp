#include "Bytes.h"
#include <cstring>

class Bytes {

public:
    unsigned char* EMPTY_BYTES = new unsigned char[1];

    std::string HEX_TMP = "0123456789ABCDEF";

    static unsigned char* toBytes(unsigned char& b) {
        unsigned char* temp = new unsigned char[1];
        temp[0] = b;
        return temp;
    }

    /**
     * @brief 
     * Convert std::string to char array in C++.
     * @param s 
     * @return unsigned char* 
     */
    static unsigned char* toBytes(std::string& s) {
        if (s.empty()) {
            unsigned char* temp = new unsigned char[1];
            return temp;
        }
        
        char res_temp[s.length() + 1];
        strcpy(res_temp, s.c_str()); //import from cstring

        //In C++11, char is by default UTF8
        unsigned char* res = (unsigned char*)res_temp;
        return res;
    }

    /**
     * @brief 
     * ???????????????????????????????????????
     * @param x 
     * @return unsigned char* 
     */
    static unsigned char* toBytes(int& x) {
        unsigned char* b = new unsigned char[4];
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
     * @return unsigned char* 
     */
    static unsigned char* toBytes(long& x) {
        unsigned char* b = new unsigned char[8];
        for (int i = 7; i >= 0; i--){
            int j = (7 - i) << 3;
            b[i] = (unsigned char) ((x >> j) & 0xFF );
        }
        
        return b;
    }

    std::string toHex(unsigned char* buf, int& buf_array_len) {
        return toHex(buf, 0, buf_array_len * sizeof(buf)/sizeof(unsigned char*)  );
    }

    std::string toHex(unsigned char* buf, int offset, int len) {
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

    unsigned char* toBytes(unsigned char* a, unsigned char* b, int& a_len, int& b_len )  
    {
        if (a == nullptr) return b;
        if (b == nullptr) return a;

        unsigned char* result = new unsigned char[a_len + b_len];
        memcpy(result, a, a_len);
        memcpy(result, b, a_len + b_len);
        return result;
    }

    /**
     * @brief 
     * ??????????????????????????????????????????????
     * @param a 
     * @return int 
     */
    int toInt(unsigned char* a) {
        return (a[0] << 24) & 0xFF000000 | (a[1] << 16) & 0x00FF0000 | (a[2] << 8) & 0x0000FF00
        | (a[3] << 0) & 0x000000FF;
    }

    long toLong(unsigned char* a) {
        long x = 0;

        for (int i = 0; i < 8; i++)
        {
            /* code */
            int j = (7 - i) << 3;
            x |= ((0xFFL << j) & ((long) a[i] << j));

        }
        return x;
    }

    unsigned char* slice(unsigned char* buf, int& offset, int& len, int& buf_len) {
        if (buf == nullptr) {
            //C++ 如何实现类似Java的 throw new IOException("buffer is null") ？？
        }

        if (offset < 0 || len < 0) {
            // throw new IOException("Invalid offset: " + offset + " or len: " + len);
        }

        if (offset + len > buf_len) {
            //C++的异常
        }

        unsigned char* result = new unsigned char[len];

        //????how to do this in C++
        // System.arraycopy(buf, offset, result, 0, len);
        return result;
    }

    //TODO: 换一个性能更高的哈希函数
    int hash(unsigned char* key, int& key_len) 
    {
        if (key == nullptr) return 0;
        int h = 1;
        for (size_t i = 0; i < key_len; i++)
        {
            /* code */
            h = (h << 5) + h + key[i];
        }

        return h;
    }

    int compare(unsigned char* a, unsigned char* b, int& a_len, int& b_len) 
    {
        if (a == b) return 0;
        if (a == nullptr) return -1;
        if (b == nullptr) return 1;

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

};
#ifndef CHECKSUM_H_
#define CHECKSUM_H_

#include <stdint.h>
#include <deque>

class Checksum
{
public:
	Checksum();
	virtual ~Checksum();
	void update(std::deque<unsigned char>& buffer, int off, int len);
	unsigned long getValue();
    unsigned long crc32(int& crc, std::deque<unsigned char>& b, int len);
    unsigned long crc32_z();
    static int updateBytes(int crc, std::deque<unsigned char>& b, int off, int len);
private:
	static unsigned long crc_32_tab[256]; 
	unsigned long crc32Value;
    static int crc;
};

#endif
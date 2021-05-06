#ifndef KEYVALUE_h_
#define KEYVALUE_h_
#include <iostream>
#include <assert.h>
#include <string>
#include <deque>
// #include <string.h> //for memcpy("dest", "source", "size") function
// #include <cstring>
#include <algorithm>


class KeyValue
{
public:
    KeyValue();
    KeyValue(const KeyValue& k);
    virtual ~KeyValue();

    class Op
    {
    public:
        static unsigned char Put;
        static unsigned char Drop;
        Op(){}
        Op(unsigned char& paramCode)
        {
            this->code = paramCode;
        }
        virtual ~Op();
        static unsigned char code2Op(unsigned char& paramCode)
        {
            switch ((int)paramCode)
            {
            case 0:
                return Op::Put;
                break;
            case 1:
                return Op::Drop;
                break;
            default:
                std::cout << "default break" << "\n";
                break;
            }
        }
        unsigned char getCode()
        {
            return this->code;
        }
        // Op Put(ZERO);
        // Op Drop(ONE);
        // static const unsigned char Put = 0;
        // static const unsigned char Drop = 1;
    private:
        unsigned char code;
    };
   

    static KeyValue create(std::deque<unsigned char>& key, std::deque<unsigned char>& value, unsigned char& op, long sequenceId);

    static KeyValue createPut(std::deque<unsigned char>& key, std::deque<unsigned char>& value, long sequenceId);

    static KeyValue createDrop(std::deque<unsigned char>& key, long sequenceId);

    std::deque<unsigned char> getKey();
    std::deque<unsigned char> getValue();
    int getSerializeSize();
    std::deque<unsigned char> toBytes();
    int compareTo(KeyValue& kv);
    std::string toString();

    static KeyValue parseFrom(std::deque<unsigned char>& bytes, int offset);
    static KeyValue parseFrom(std::deque<unsigned char>& bytes);

    long getSequenceId();

    std::deque<unsigned char> toBytes();
    
    template<typename T>
    bool equals(T& kv);

    const static int RAW_KEY_LEN_SIZE;
    const static int VAL_LEN_SIZE;
    const static int OP_SIZE;
    const static int SEQ_ID_SIZE;

    Op getOp();

    inline Op getOp();
    inline long getSequenceId();

private:
    std::deque<unsigned char> key;
    std::deque<unsigned char> value;
    Op op;
    // unsigned char op;
    long sequenceId;
    int getRawKeyLen()
    {
        return KeyValue::key.size() + OP_SIZE + SEQ_ID_SIZE;
    }
    KeyValue(std::deque<unsigned char>& paramKey, std::deque<unsigned char>& paramValue, unsigned char& paramOp,
    long paramSequenceId);
    //重载 == 运算符
    bool operator==(KeyValue& param)
    {
        KeyValue itself;
        
        if (itself.key == param.key && itself.value == param.value)
        {
            return true;
        }
        return false;
    }

    class  KeyValueComparator
    {
    public:
         KeyValueComparator(/* args */);
        ~ KeyValueComparator();

        int compare(KeyValue& a, KeyValue& b)
        {
           
            if (a == b) return 0;
            if (a.key.empty() && a.value.empty() ) return -1;
            if (b.key.empty() && b.value.empty() ) return 1;
            return a.compareTo(b);
        }
    
    private:
    };

    public:
    const static KeyValueComparator KV_CMP;
};


#endif
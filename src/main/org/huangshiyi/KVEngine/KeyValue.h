#ifndef KEYVALUE_h_
#define KEYVALUE_h_
#include <iostream>
#include <assert.h>
#include <string>
// #include <string.h> //for memcpy("dest", "source", "size") function
// #include <cstring>
#include <algorithm>
#include <iterator>

class KeyValue
{
public:
    KeyValue(/* args */);
    KeyValue(const KeyValue& k) {};
    ~KeyValue();

    unsigned char* getKey() {
        return key;
    }

    unsigned char* getValue() {
        return value;
    }

    int getSerializeSize() {
        /**
         * @brief 
         * sizeof(value)/sizeof(unsigned char*)乘4的原因:
         * 给key、value分配的动态空间的大小是4
         * unsigned char* key = new unsigned char[4];
           unsigned char* value = new unsigned char[4];

         */
        return RAW_KEY_LEN_SIZE + VAL_LEN_SIZE + getRawKeyLen() + 4 * (sizeof(value)/sizeof(unsigned char*) );
    }

    unsigned char* toBytes() {
        int rawKeyLen = getRawKeyLen();
        int pos = 0;
        unsigned char* bytes = new unsigned char[getSerializeSize()];

        //Encode raw key length.
        unsigned char* rawKeyLenBytes = new unsigned char[rawKeyLen];
        // memcpy("dest", "source", "size") function
        memcpy(bytes, rawKeyLenBytes, RAW_KEY_LEN_SIZE);
        pos += RAW_KEY_LEN_SIZE;


        // Encode value length.
        unsigned char* valLen = new unsigned char[4 * (sizeof(value)/sizeof(unsigned char*) )];
        memcpy(bytes, valLen, VAL_LEN_SIZE);
        pos += VAL_LEN_SIZE;

        //Encode key
        memcpy(bytes, key, 4 * (sizeof(key)/sizeof(unsigned char*) ));
        pos += 4 * (sizeof(key)/sizeof(unsigned char*) );

        //Encode Op
        Op op;
        bytes[pos] = op.getCode();
        pos += 1;

        //Encode sequenceId
        unsigned char* seqIdBytes = new unsigned char[sequenceId];
        memcpy(bytes, seqIdBytes, sequenceId * (sizeof(seqIdBytes)/sizeof(unsigned char*) ) );
        pos += sequenceId * (sizeof(seqIdBytes)/sizeof(unsigned char*) );

        //Encode value
        memcpy(bytes, value, 4 * (sizeof(value)/sizeof(unsigned char*) ) );
        return bytes;
    }

    int compareTo(KeyValue& kv) ;

    bool equals(unsigned char& kv);

    std::string toString();

    static KeyValue parseFrom(unsigned char* bytes, int offset);

    static KeyValue parseFrom(unsigned char* bytes);



    class KeyValueComparator {
        public:
        KeyValueComparator() {};
        ~KeyValueComparator(){};
        int compare(KeyValue& a, KeyValue& b) {
            /**
             * 还要在这里做运算符重载这样的工作
             * 
             */
            return 0;
        }
    };

    const static int RAW_KEY_LEN_SIZE = 4;
    const static int VAL_LEN_SIZE = 4;
    const static int OP_SIZE = 1;
    const static int SEQ_ID_SIZE = 8;
    const static KeyValueComparator KV_CMP;
    
    class Op
    {
    private:
        /* data */
        const static unsigned char Put = 0;
        const static unsigned char Drop = 1;
        unsigned char code;
    public:
        Op(/* args */){};
        Op(unsigned char& paramCode) {
            this->code = paramCode;
        }
        ~Op();
        //友元类使用要慎重，不要滥用；我这里之所以使用友元类，
        // 是因为我想通过外部类KeyValue访问其内部类Op的private属性.例如本文件的第74、第78行代码
        friend class KeyValue;
        
        static unsigned char code2Op(unsigned char& paramCode) {
            switch ((int)paramCode)
            {
            case 0:
                return Op::Put;
                break;
            case 1:
                return Op::Drop;
                break;
            default:
                std::cout<< "default break" << "\n";
                break;
            }
        }

        unsigned char getCode() {
            return this->code;
        }
    };
    Op getOp() {
        return this->op;
    }

    long getSequenceId() {
        return this->sequenceId;
    }



    static KeyValue create(unsigned char* key, unsigned char* value, Op& op, long sequenceId) {
        return new KeyValue(key, value, op, sequenceId);
    }

    static KeyValue createPut(unsigned char* key, unsigned char* value, long sequenceId) {
        return KeyValue::create(key, value, KeyValue::Op::Put, sequenceId);
    }

    static KeyValue createDrop(unsigned char* key, long sequenceId) {
        return KeyValue::create(key, (unsigned char)0, KeyValue::Op::Drop, sequenceId);
    }

private:
    unsigned char* key = new unsigned char[4];
    unsigned char* value = new unsigned char[4];
    Op op;
    long sequenceId;
    int getRawKeyLen() {
        return 4 * ( sizeof(key)/sizeof(unsigned char*) ) + OP_SIZE + SEQ_ID_SIZE;
    }

    KeyValue(unsigned char* paramKey, unsigned char* paramValue, Op& paramOp, long paramSequenceId) {
        assert(paramKey != nullptr);
        assert(paramValue != nullptr);
        assert(paramOp != nullptr);//TODO: 看来要实现一个运算符重载了......
        assert(paramSequenceId >= 0);
        this->key = paramKey;
        this->value = paramValue;
        this->op = paramOp;
        this->sequenceId = paramSequenceId;        
    }
    
};

#endif KEYVALUE_h_


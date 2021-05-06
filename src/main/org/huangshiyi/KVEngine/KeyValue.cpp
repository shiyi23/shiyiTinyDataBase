#include "KeyValue.h"
#include "Bytes.h"
#include <deque>
#include <algorithm>

KeyValue::KeyValue()
{
    const int RAW_KEY_LEN_SIZE = 4;
    const int VAL_LEN_SIZE = 4;
    const int OP_SIZE = 4;
    const int SEQ_ID_SIZE =8;
    KeyValue::Op::Put = 0;
    KeyValue::Op::Drop = 1;
    const KeyValue::KeyValueComparator KV_CMP;
}

KeyValue KeyValue::create(std::deque<unsigned char>& key, std::deque<unsigned char>& value, unsigned char& op, long sequenceId)
{
    KeyValue key_value(key, value, op, sequenceId);
    return key_value;
}

KeyValue KeyValue::createPut(std::deque<unsigned char>& key, std::deque<unsigned char>& value, long sequenceId)
{
    KeyValue key_value = KeyValue::create(key, value, KeyValue::Op::Put, sequenceId);
    return key_value;    
}

KeyValue KeyValue::createDrop(std::deque<unsigned char>& key, long sequenceId)
{
    KeyValue key_value = KeyValue::create(key, Bytes::EMPTY_BYTES, KeyValue::Op::Drop, sequenceId);
    return key_value;
}

std::deque<unsigned char> KeyValue::getKey()
{
    return KeyValue::key;
}

std::deque<unsigned char> KeyValue::getValue()
{
    return KeyValue::value;
}

KeyValue::Op KeyValue::getOp()
{
    return this->op;
}

long KeyValue::getSequenceId()
{
    return this->sequenceId;
}

std::deque<unsigned char> KeyValue::toBytes()
{
    int rawKeyLen = KeyValue::getRawKeyLen();
    int pos = 0;
    std::deque<unsigned char> bytes(KeyValue::getSerializeSize());
    
    //Encode raw key length
    std::deque<unsigned char> rawKeyLenBytes = Bytes::toBytes(rawKeyLen);
    


}

int KeyValue::compareTo(KeyValue& kv)
{
    if (kv.key.empty() )
        throw "kv to compare should not to be null";

    int res = Bytes::compare(this->key, kv.key);
    if (res != 0) 
    {
        return res;
    }
    if (this->sequenceId != kv.sequenceId)
    {
        return this->sequenceId > kv.sequenceId ? -1 : 1;
    }
    //Op类对象也需要重载!=操作符
    if (this->op != kv.op)
    {
        return this->op.getCode() > kv.op.getCode() ? -1 : 1;
    }
    return 0;
}



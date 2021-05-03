#include "KeyValue.h"
#include <deque>

KeyValue::KeyValue()
{
    const int RAW_KEY_LEN_SIZE = 4;
    const int VAL_LEN_SIZE = 4;
    const int OP_SIZE = 4;
    const int SEQ_ID_SIZE =8;
    const KeyValue::KeyValueComparator KV_CMP;
}

KeyValue KeyValue::create(std::deque<unsigned char>& key, std::deque<unsigned char>& value, KeyValue::Op& op, long sequenceId)
{
    KeyValue keyvalue(key, value, op, sequenceId);
    return keyvalue;
}

KeyValue KeyValue::createPut(std::deque<unsigned char>& key, std::deque<unsigned char>& value, long sequenceId)
{
    KeyValue keyvalue = KeyValue::create(key, value, KeyValue::Op::Put, sequenceId);
    return keyvalue;    
}
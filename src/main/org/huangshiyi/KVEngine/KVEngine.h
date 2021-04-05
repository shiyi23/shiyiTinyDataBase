#ifndef KVENGINE_h_
#define KVENGINE_h_
#include "KeyValue.h"
#include <thread>

/**
 * *
 * 参考文章：
 * 1、https://zhuanlan.zhihu.com/p/34658848
 * 2、https://medium.com/@pthtantai97/implement-key-value-store-by-btree-5a100a03da3a
 * 参考repo：
 */


//用类模板实现Iter接口
/**
 * C++中，接口相比抽象类的特殊性在于：
 * 1、接口没有任何属性，抽象类可以有属性
 * 2、除了构造函数，接口所有的方法都是public的纯虚函数，而抽象类可以有非纯虚函数
 * 总结：接口是特殊的抽象类
*/
template <typename T>
class Iter
{
public:
    Iter(/* args */);
    Iter(T& key_value);
    virtual void ~Iter() = 0;
    virtual bool hasNext() = 0;
    virtual KeyValue next() = 0;
};

template <typename T>
Iter<T>::Iter(/* args */)
{
    //TODO
}
template <typename T>
bool Iter<T>::hasNext() {
    //TODO
}

template<typename T>
KeyValue Iter<T>::next() {
    //TODO
}


//使用类模板实现Flusher接口
template<typename T>
class Flusher
{

public:
    Flusher(/* args */);
    virtual ~Flusher() = 0;
    virtual void flush(Iter<KeyValue>& it) = 0;
};


class KVEngine
{
public:
    KVEngine(/* args */);
    virtual ~KVEngine() = 0;

    virtual void put(unsigned char* key, unsigned char* value) = 0;

    virtual void get(unsigned char* key) = 0;

    virtual void drop(unsigned char* key) = 0;
    
     /**
    * Fetch all the key values whose key located in the range [startKey, stopKey)
    *
    * @param startKey start key to scan (inclusive), if start is unsigned char[0], it means negative
    *                 infinity.
    * @param stopKey  to stop the scan. (exclusive), if stopKey is unsigned char[0], it means positive
    *                 infinity.
    * @return Iterator for fetching the key value one by one.
    */
    virtual Iter<KeyValue> scan(unsigned char* startKey, unsigned char* stopKey) = 0;

    /**
     * Full scan the Key Value store.
     *
     * @return Iterator to fetch the key value one by one.
     * @throws IOException
     */
    virtual Iter<KeyValue> scan() = 0;
    
    //内部类
    class Compactor : public std::thread {
        public:
        virtual void compact();
    }

};


#endif /*KVENGINE_h_*/

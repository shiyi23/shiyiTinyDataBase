#include "Config.h"

Config::Config(){
    maxMemstoreSize = 16 * 1024 * 1024;
    flushMaxRetries = 10;
    dataDir = "shiyiTinyDataBase";
    maxDiskFiles = 10;
    maxThreadPoolSize = 5;
    Config DEFAULT;
}

Config* Config::setMaxMemstoreSize(long maxMemstoreSize)
{
    this->maxMemstoreSize = maxMemstoreSize;
    return this;
}

long Config::getMaxMemstoreSize() {
    return this->maxMemstoreSize;
}

Config* Config::setFlushMaxRetries(int flushMaxRetries)
{
    this->flushMaxRetries = flushMaxRetries;
    return this;
}

int Config::getFlushMaxRetries() {
    return this->flushMaxRetries;
}

Config* Config::setDataDir(std::string dataDir)
{
    this->dataDir = dataDir;
    return this;
}

std::string Config::getDataDir() 
{
    return this->dataDir;
}

Config* Config::setMaxDiskFiles(int maxDiskFiles)
{
    this->maxDiskFiles = maxDiskFiles;
    return this;
}

int Config::getMaxDiskFiles() {
    return this->maxDiskFiles;
}

Config* Config::setMaxThreadPoolSize(int maxThreadPoolSize)
{
    this->maxThreadPoolSize = maxThreadPoolSize;
    return this;
}

int Config::getMaxThreadPoolSize()
{
    return this->maxThreadPoolSize;
}

Config Config::getDefault()
{
    return Config::DEFAULT;
}
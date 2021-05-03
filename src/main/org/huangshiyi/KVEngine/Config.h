#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>

class Config
{
private:
    /* data */
    long maxMemstoreSize;
    int flushMaxRetries;
    std::string dataDir;
    int maxDiskFiles;
    int maxThreadPoolSize;
    static const Config DEFAULT;
public:
    Config(/* args */);
    virtual ~Config();
    Config* setMaxMemstoreSize(long maxMemstoreSize);
    long getMaxMemstoreSize();
    Config* setFlushMaxRetries(int flushMaxRetries);
    int getFlushMaxRetries();
    Config* setDataDir(std::string dataDir);
    std::string getDataDir();
    Config* setMaxDiskFiles(int maxDiskFiles);
    int getMaxDiskFiles();
    Config* setMaxThreadPoolSize(int maxThreadPoolSize);
    int getMaxThreadPoolSize();
    static Config getDefault();
};

Config::Config(/* args */)
{
}

Config::~Config()
{
}




#endif
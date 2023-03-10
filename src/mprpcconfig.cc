#include "mprpcconfig.h"

#include <iostream>

// 负责解析加载配置文件
void MprpcConfig::LoadConfigFile(const char* config_file)
{
    FILE* pf = fopen(config_file, "r");
    if (nullptr == pf)
    {
        std::cout << config_file << " is not exists!" << std::endl;
        exit(EXIT_FAILURE);
    }

    //
    while (!feof(pf)) 
    {
        char buf[512] = { 0 };
        fgets(buf, 512, pf);

        // 去掉字符串前面多余的空格
        std::string src_buf(buf);
        Trim(src_buf);

        // 判断#的注释
        if (src_buf[0] == '#' || src_buf.empty())
        {
            continue;
        }

        // 解析配置项目
        int idx = src_buf.find('=');
        if (idx == -1)
        {
            // 配置不合法
            continue;
        }

        std::string key;
        std::string value;
        key = src_buf.substr(0, idx);
        Trim(key);
        int endidx = src_buf.find('\n', idx); // 返回给定位置idx后的子串的位置(索引值)
        value = src_buf.substr(idx+1, endidx - idx - 1);
        Trim(value);
        m_configMap.insert({key, value});

    }
}

// 查询配置项信息
std::string MprpcConfig::Load(const std::string& key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        return "";
    }
    return m_configMap[key];
}

void MprpcConfig::Trim(std::string& str)
{
    // 去掉字符串前面多余的空格
    int idx = str.find_first_not_of(' ');
    if (idx != -1) 
    {
        // 说明字符串前面有空格
        str = str.substr(idx, str.size() - idx);
    }

    idx = str.find_last_not_of(' ');
    if (idx != -1) 
    {
        // 说明字符串后面有空格
        str = str.substr(0, idx + 1);
    }
}
#pragma once
#include <string>
#include <unordered_map>

class TextTable {
public:
    TextTable();
    ~TextTable();

    void Load(const std::string& filePath);
    void SetLanguage(const std::string& language);
    std::string GetString(const std::string& id) const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_strings;
    std::string m_currentLanguage;
};

/*#pragma once
#include "../wolf/wolf.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <string>
#include <unordered_map>
#include <vector>

class TextTable {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> table;
    std::vector<std::string> languages;
    std::string currentLanguage;

public:
    bool Load(const std::string& filePath);
    void SetLanguage(const std::string& language);
    std::string GetString(const std::string& id) const;
};*/
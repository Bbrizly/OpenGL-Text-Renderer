#pragma once
#include "../wolf/wolf.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

class TextTable
{
private:
    std::map<std::string, std::map<std::string, std::string>> textData; // language -> ID -> string
    std::string currentLanguage;

public:
    void Load(const std::string& filePath);
    void SetLanguage(const std::string& language);
    std::string GetString(const std::string& id);
};
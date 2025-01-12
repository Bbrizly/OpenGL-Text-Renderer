#pragma once
#include "../wolf/wolf.h"

#include <fstream>
#include <sstream>
#include <stdexcept>


class Font
{
private:
    wolf::Texture* fontTexture;
    std::map<char, float[4]> charExtents;

public:
    Font(const std::string& texturePath, const std::string& fontDataPath);
    wolf::Texture* GetTextureForChar(char c);
    void GetExtentsForChar(char c, float* uStart, float* uEnd, float* vStart, float* vEnd);

};
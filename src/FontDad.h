#pragma once
#include "../wolf/wolf.h"
#include <string>
#include <unordered_map>
#include <utility>
#include "Font.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

class FontDad {
private:
    vector<Font> m_fonts;
    Font m_activeFont;

    wolf::Texture* m_arrayTexture;

    int m_lineHeight = 0; // Distance between lines
    int m_scaleW;     // Texture width
    int m_scaleH;     // Texture height

public:
    FontDad(const string& fontPath);
    ~FontDad();

    Font &getActiveFont();
    const int GetLineHeight();
    wolf::Texture* GetTexture() const;

};
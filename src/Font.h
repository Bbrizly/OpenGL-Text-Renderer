#pragma once
#include "../wolf/wolf.h"
#include <string>
#include <unordered_map>
#include <utility>
#include "MiniFont.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

class Font {
private:
    vector<MiniFont*> m_fonts;
    // MiniFont m_activeFont;

    wolf::Texture* m_arrayTexture;

    int m_lineHeight = 0; // Distance between lines
    int m_scaleW;     // Texture width
    int m_scaleH;     // Texture height


    
    void LoadAll(const string& baseName);
    void ArrayTextureOfAllFiles(const vector<string>& listOfFontSizes);

public:
    Font(const string& fontPath);
    ~Font();


    const vector<MiniFont*>& GetFonts() const;

    // MiniFont &getActiveFont();
    
    const CharInfo& GetCharacter(char c, int index) const;
    const int GetLineHeight(int index);
    const unordered_map<pair<char, char>, int, PairHash>& GetKerning(int index) const;

    // const int GetLineHeight();
    wolf::Texture* GetTexture() const;

};
#pragma once
#include "../wolf/wolf.h"
#include <string>
#include <unordered_map>
#include "CharInfo.h"

using namespace std;


class Font {
private:
    void LoadFont(const string& fontPath);

    wolf::Texture* m_texture;
    unordered_map<char, CharInfo> m_characters;
    // unordered_map<pair<char, char>, int> m_kerning;
    
    int m_lineHeight; // Distance between lines
    int m_scaleW;     // Texture width
    int m_scaleH;     // Texture height

public:
    Font(const string& texturePath, const string& fontPath);
    ~Font();

    const CharInfo& GetCharacter(char c) const;
    wolf::Texture* GetTexture() const;

    // const unordered_map<std::pair<char, char>, int>& GetKerning();
    const unordered_map<pair<char, char>, int>& GetKerning() const;

};
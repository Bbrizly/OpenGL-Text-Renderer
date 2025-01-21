#pragma once
#include "../wolf/wolf.h"
#include <string>
#include <unordered_map>
#include <utility>
#include "CharInfo.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

// Custom hash function for std::pair<char, char>
struct PairHash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& pair) const {
        return hash<T1>()(pair.first) ^ (hash<T2>()(pair.second) << 1);
    }
};


class Font {
private:
    void LoadFont(const string& fontPath);//, const std::vector<std::string>& texturePaths);
    void ArrayTextureOfAllFiles(string filename, int totalPages);
    // void LoadFont(const string& fontPath, const std::vector<std::string>& texturePaths);

    wolf::Texture* m_arrayTexture;
    unordered_map<char, CharInfo> m_characters;
    unordered_map<pair<char, char>, int, PairHash> m_kerning;
    // unordered_map<pair<char, char>, int> m_kerning;
    
    int m_lineHeight = 0; // Distance between lines
    int m_scaleW;     // Texture width
    int m_scaleH;     // Texture height

public:
    Font(const string& fontPath);
    ~Font();

    const CharInfo& GetCharacter(char c) const;
    const int GetLineHeight();
    wolf::Texture* GetTexture() const;

    const unordered_map<pair<char, char>, int, PairHash>& GetKerning() const;

};
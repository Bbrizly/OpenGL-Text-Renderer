#include "Font.h"

//return data needed to render individual letters

Font::Font(const std::string& texturePath, const std::string& fontDataPath) {
    fontTexture = new wolf::Texture(texturePath);
    
    std::ifstream file(fontDataPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open font data file: " + fontDataPath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        char c;
        float uStart, uEnd, vStart, vEnd;
        stream >> c >> uStart >> uEnd >> vStart >> vEnd;
        charExtents[c][0] = uStart;
        charExtents[c][1] = uEnd;
        charExtents[c][2] = vStart;
        charExtents[c][3] = vEnd;
    }
}

wolf::Texture* Font::GetTextureForChar(char c) {
    return fontTexture;
}

void Font::GetExtentsForChar(char c, float* uStart, float* uEnd, float* vStart, float* vEnd) {
    if (charExtents.find(c) != charExtents.end()) {
        *uStart = charExtents[c][0];
        *uEnd = charExtents[c][1];
        *vStart = charExtents[c][2];
        *vEnd = charExtents[c][3];
    } else {
        throw std::out_of_range("Character not found in font data: " + std::string(1, c));
    }
}
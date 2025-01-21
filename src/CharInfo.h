#pragma once
#include <GL/glew.h>

struct CharInfo {
    float uStart, uEnd, vStart, vEnd; // Texture coordinates
    float xOffset, yOffset;           // Position offsets
    float xAdvance;                   // Advance to the next character
    int page;
};
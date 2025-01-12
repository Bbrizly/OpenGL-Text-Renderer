#pragma once
#include "../wolf/wolf.h"
#include <TextTable.h>
#include <Font.h>

enum HorizontalAlignment {
    AL_Left,
    AL_Center,
    AL_Right
};

class TextBox {
private:
    Font* font;
    std::string text;
    float width, height;
    float posX, posY;
    HorizontalAlignment alignment;
    float color[4]; // RGBA
    std::vector<float> vertexData; // Stores quad vertices for rendering

    void UpdateVertexData();

public:
    TextBox(Font* font, const std::string& text, float width, float height);
    void SetPos(float x, float y);
    void SetText(const std::string& newText);
    void SetHorizontalAlignment(HorizontalAlignment align);
    void SetColour(float r, float g, float b, float a);
    void Render();
};
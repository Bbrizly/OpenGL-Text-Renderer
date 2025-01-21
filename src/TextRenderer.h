#pragma once
#include "../wolf/wolf.h"
#include "TextBox.h"
#include "Font.h"
#include <glm/glm.hpp>
#include "../samplefw/Sample.h"
#include "Vertex.h"
#include <string>
#include <vector>
using namespace std;
// #include "Grid2D.h"
class Grid2D;

class TextRenderer {
private:
    wolf::Program* TextShader;

    vector<TextBox*> m_textBoxes;
    
public:
    TextRenderer();
    ~TextRenderer();

    void init();
    void update(float dt);
    void render(const glm::mat4& proj, const glm::mat4& view);

    Font* createFont(const std::string& fontDataPath);
    TextBox* createTextBox(Font* font, const std::string& text, float x, float y, float width, float height);

    void setTextBox(TextBox* textBox);
};
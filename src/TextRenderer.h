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
    wolf::Program* m_pProgram;
    wolf::VertexBuffer* m_vertexBuffer;
    wolf::VertexDeclaration* m_vertexDecl;
    vector<Vertex> m_vertices;
    int m_numVertices;

    TextBox* m_currentTextBox;
    
public:
    TextRenderer();
    ~TextRenderer();

    void init();
    void update(float dt);
    void render(const glm::mat4& proj, const glm::mat4& view);

    void pushVertexData(wolf::VertexBuffer*& vBuffer, wolf::VertexDeclaration*& vDecl, const vector<Vertex>& vertices);

    Font* createFont(const std::string& texturePath, const std::string& fontDataPath);
    TextBox* createTextBox(Font* font, const std::string& text, float x, float y);

    void setTextBox(TextBox* textBox);
};
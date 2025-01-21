#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Font.h"
#include "Vertex.h"
#include "CharInfo.h"
#include <fstream>
#include <sstream>

class TextBox {
private:

    wolf::Program* m_pProgram;
    wolf::VertexBuffer* m_vertexBuffer;
    wolf::VertexDeclaration* m_vertexDecl;
    vector<Vertex> m_vertices;
    int m_numVertices;

    Font* m_font;
    string m_text;
    float m_width = 400;
    float m_height = 400;
    glm::vec4 m_color;
    glm::vec2 m_position;
    int m_alignment = 0;
    bool m_visualization = true;
    bool broke = false;

    glm::vec2 CalculateAlignmentCursor(const std::string& line, float lineWidth, float cursorY);   // Calculates the starting cursor position based on alignment
    void ApplyKerning(size_t index, char c, glm::vec2& cursor); // Adjusts cursor for kerning
    void GenerateCharacterVertices(const CharInfo& ch, glm::vec2 cursor, float textureWidth, float textureHeight); // Generates vertices for a single character
    void GenerateBoundingBoxVertices();     // Generates vertices for the bounding box
    float CalculateWordWidth(const std::string& word);
    void GenerateVertices();

    void pushVertexData(wolf::VertexBuffer*& vBuffer, wolf::VertexDeclaration*& vDecl, const vector<Vertex>& vertices);

public:
    TextBox(Font* font, const std::string& text, float width, float height, wolf::Program* shader);
    ~TextBox();

    void SetText(const std::string& text);
    void SetPosition(float x, float y);
    void SetAlignment(int alignment);
    void SetColor(float r, float g, float b, float a);
    void SetVisualization(bool visualize);

    const std::vector<Vertex>& GetVertices() const;
    Font* GetFont() const; // Add this method
    void Render(const glm::mat4& proj, const glm::mat4& view);

};


/*#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Font.h"
#include "Vertex.h"
#include "../wolf/wolf.h"

using namespace std;

// #include <string>
// #include <vector>
// #include <glm/glm.hpp>
// #include "Font.h"
// #include "Vertex.h"

enum Alignment {
    AL_Left,
    AL_Center,
    AL_Right
};

class TextBox {
private:
    // Helper functions
    void generateVertices();
    void setupRendering();

    // TextBox properties
    string m_text;
    Font m_font;
    glm::vec2 m_position;
    float m_scale;
    float m_width;
    Alignment m_alignment;
    glm::vec4 m_color;

    // Rendering components
    vector<Vertex> m_vertices;
    wolf::VertexBuffer* m_vertexBuffer;
    wolf::VertexDeclaration* m_vertexDecl;
    wolf::Program* m_program;
    int m_numVertices;

public:
    // Constructor and Destructor
    TextBox(const string& text, const Font& font, float x, float y, float scale);//, float width);
    ~TextBox();

    // Generate and render functions
    void render(const glm::mat4& proj, const glm::mat4& view);

    void SetHorizontalAlignment(Alignment alignment);
    void SetColour(float r, float g, float b, float a);
};*/
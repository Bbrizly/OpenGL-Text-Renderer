#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Font.h"
#include "Vertex.h"
#include "CharInfo.h"

class TextBox {
private:
    Font* m_font;
    std::string m_text;
    float m_width = 400;
    float m_height = 400;
    glm::vec4 m_color;
    glm::vec2 m_position;
    int m_alignment = 0;
    std::vector<Vertex> m_vertices;
    bool m_visualization = true;

    glm::vec2 CalculateAlignmentCursor();   // Calculates the starting cursor position based on alignment
    float CalculateTextWidth() const;       // Computes total width of the text
    void ApplyKerning(size_t index, char c, glm::vec2& cursor); // Adjusts cursor for kerning
    void GenerateCharacterVertices(const CharInfo& ch, glm::vec2 cursor, float textureWidth, float textureHeight); // Generates vertices for a single character
    void GenerateBoundingBoxVertices();     // Generates vertices for the bounding box
    void GenerateVertices();

public:
    TextBox(Font* font, const std::string& text, float width, float height);

    void SetText(const std::string& text);
    void SetPosition(float x, float y);
    void SetAlignment(int alignment);
    void SetColor(float r, float g, float b, float a);
    void SetVisualization(bool visualize);

    const std::vector<Vertex>& GetVertices() const;
    Font* GetFont() const; // Add this method

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
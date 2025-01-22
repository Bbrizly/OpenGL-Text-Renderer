#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Font.h"
#include "TextTable.h"
#include "Vertex.h"
#include "CharInfo.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdarg>
#include <cstdio>

#include <glm/gtc/matrix_transform.hpp>

class TextBox {
private:

    wolf::Program* m_pProgram;
    wolf::VertexBuffer* m_vertexBuffer;
    wolf::VertexDeclaration* m_vertexDecl;
    vector<Vertex> m_vertices;
    int m_numVertices;


    Font* m_font;
    TextTable* m_pTextTable = nullptr;
    string m_text;
    float m_width = 400;
    float m_height = 400;
    glm::vec4 m_color;
    glm::vec2 m_position;
    bool m_visualization = true;
    bool            m_wrap;        
    bool            m_visualize;    
    bool            m_dynamicFit;   
    bool m_shrinkToFit = false;
    
    int m_hAlign; //0 : left, 1: center, 2: right
    int m_vAlign; //0 : top, 1: center, 2: bottom

    bool broke = false;

private:

    glm::vec2 CalculateAlignmentCursor(const std::string& line, float lineWidth, float cursorY);   // Calculates the starting cursor position based on alignment
    void ApplyKerning(size_t index, char c, glm::vec2& cursor); // Adjusts cursor for kerning
    void GenerateCharacterVertices(const CharInfo& ch, glm::vec2 cursor, float textureWidth, float textureHeight, float scale); // Generates vertices for a single character
    void GenerateBoundingBoxVertices();     // Generates vertices for the bounding box
    float CalculateWordWidth(const std::string& word);
    void SetShrinkToFit(bool enable);
    void GenerateVertices();

    void pushVertexData(wolf::VertexBuffer*& vBuffer, wolf::VertexDeclaration*& vDecl, const vector<Vertex>& vertices);

public:
    TextBox(Font* font, const std::string& text, float width, float height, wolf::Program* shader, TextTable* pTable);
    ~TextBox();
    void printf(const char* fmt, ...);

    // The basic setters
    void SetTextTable(TextTable* pTable);
    void SetText(const std::string& text);
    void SetPosition(float x, float y);
    void SetColor(float r, float g, float b, float a);
    void SetVisualization(bool visualization);

    // Horizontal alignment: 0=left, 1=center, 2=right
    void SetAlignment(int alignment);
    // Vertical alignment: 0=top, 1=middle, 2=bottom
    void SetVerticalAlignment(int vAlign);

    // Accessors
    const std::vector<Vertex>& GetVertices() const;
    Font* GetFont() const;

    // Rendering
    void Render(const glm::mat4& proj, const glm::mat4& view);


};
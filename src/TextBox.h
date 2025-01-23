#pragma once

#include <string>
#include <vector>
#include <cstdarg>
#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "Font.h"
#include "TextTable.h"
#include "CharInfo.h"
#include "Vertex.h"

class TextBox {
public:
    TextBox(Font* font,
            const string& text,
            float width,
            float height,
            wolf::Program* shader,
            TextTable* pTable = nullptr);
    ~TextBox();

    void SetTextTable(TextTable* pTable);

    // Basic ways to set text
    void SetText(const string& text);
    void SetText(const char* fmt, ...);

    // Position, color, alignment, etc.
    void SetPosition(float x, float y);
    void SetAlignment(int alignment);         // 0=left, 1=center, 2=right
    void SetVerticalAlignment(int vAlign);    // 0=top, 1=middle, 2=bottom
    void SetColor(float r, float g, float b, float a);
    void SetVisualization(bool visualization);

    // If text doesn't fit, try smaller scale
    void SetShrinkToFit(bool enable);

    // The final draws
    void Render(const glm::mat4& proj, const glm::mat4& view);

    // Access
    Font* GetFont() const;
    const vector<Vertex>& GetVertices() const;

private:
    // The main logic
    void GenerateVertices();


    // Step 1: do placeholders if we have a table
    string SubstitutePlaceholders(const string& raw) const;

    // Step 2: word-wrap & hyphenate, returning lines
    // If we can't fit everything, we set a bool that we truncated.
    vector<string> WrapAndHyphenate(const string& text,
                                              float scale,
                                              bool& truncated);
    
    vector<string> Truncate(vector<string> lines, float scale);

    // Step 3: measure lines to see if they fit in box. If they don't, we reduce scale.
    bool FitsInBox(const vector<string>& lines,
                   float scale);

    // Basic text metrics
    float CalculateWordWidth(const string& word);

    // Build final quads from lines
    void BuildVerticesFromLines(const vector<string>& lines,
                                float scale,
                                bool truncated);

    void BuildVerticesActual(const vector<string>& lines,
                                  float scale);

    // Helpers
    glm::vec2 CalculateAlignmentCursor(const string& line,
                                       float lineWidth,
                                       float cursorY,
                                       float scale);
    void ApplyKerning(char prevChar, char c,
                      float scale,
                      float& xCursor);

    void GenerateCharacterVertices(const CharInfo& ch,
                                   float x,
                                   float y,
                                   float textureWidth,
                                   float textureHeight,
                                   float scale);

    void GenerateBoundingBoxVertices();
    void pushVertexData(wolf::VertexBuffer*& vBuffer,
                        wolf::VertexDeclaration*& vDecl,
                        const vector<Vertex>& vertices);

private:
    Font*  m_font;
    string m_text;
    float m_width, m_height;
    glm::vec4 m_color;
    glm::vec2 m_position;

    bool   m_visualization = true;
    bool   m_shrinkToFit   = true;

    int m_hAlign=0;  // 0=left
    int m_vAlign=0;  // 0=top

    // If user typed a super-long word that can't fit -> we do a hyphen break
    // If even that can't help, eventually we do ellipses

    wolf::Program*  m_pProgram;
    wolf::VertexBuffer* m_vertexBuffer=nullptr;
    wolf::VertexDeclaration* m_vertexDecl=nullptr;
    vector<Vertex> m_vertices;
    size_t m_numVertices=0;

    TextTable* m_pTextTable=nullptr;
};

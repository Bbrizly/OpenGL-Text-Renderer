#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "Font.h"
#include "Vertex.h"
#include "../wolf/wolf.h"
#include "TextTable.h"
#include <sstream>
#include <cstdarg>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class TextTable;

class TextBox
{
public:
    TextBox(Font* font,
            const std::string& text,
            float width,
            float height,
            wolf::Program* shader,
            TextTable* pTable);
    ~TextBox();

    // If you want to re-use the same text box but with new text
    // it re-generates the geometry
    void SetText(const std::string& text);

    // C-style varargs
    void SetText(const char* fmt, ...);

    // Ties this text box to a string key from the text table
    // so that if the table changes language/properties, we auto-regen text.
    // e.g. textBox->ObserveTextID("str_greeting");
    void ObserveTextID(const std::string& textID);

    void SetTextTable(TextTable* pTable);
    void SetPosition(float x, float y);
    void SetColor(float r, float g, float b, float a);
    void SetAlignment(int alignment);          // 0=left,1=center,2=right
    void SetVerticalAlignment(int vAlignment); // 0=top,1=middle,2=bottom
    void SetVisualization(bool visualization); // debug bounding box
    void SetShrinkToFit(bool enable);

    // If we have multiple font sizes in a single Font, pick the index
    // 0 => largest, 1 => smaller, etc.
    void SetFontIndex(int index) { m_fontIndex = index; GenerateVertices(); }

    // main call
    void Render(const glm::mat4& proj, const glm::mat4& view);

    // convenience
    Font* GetFont() const;
    const std::vector<Vertex>& GetVertices() const;

    // Re-generate geometry if text table changed
    // (This is called automatically if you used ObserveTextID)
    void OnTextTableChanged();

private:
    // -------------- The big pipeline --------------
    void GenerateVertices();
    std::string SubstitutePlaceholders(const std::string& raw) const;

    // step 1: tokenization
    std::vector<std::string> TokenizeText(const std::string& text) const;

    // step 2: wrap/hyphen for a given scale
    std::vector<std::string> WrapAndHyphenate(const std::string& text,
                                              float scale) const;

    // step 3: check if it fits in the box at that scale
    bool FitsInBox(const std::vector<std::string>& lines, float scale) const;

    // step 4: pick best scale from 1 down to e.g. 0.05
    // if none fits, we do truncation
    void FindBestMiniFontAndWrap(const std::string& text, std::vector<std::string>& finalLines);

    // step 5: if still not fit, we do truncation
    void TruncateLines(std::vector<std::string>& lines, float scale);

    // step 6: build final geometry
    void BuildVerticesFromLines(const std::vector<std::string>& lines,
                                float scale,
                                bool truncated);
    void BuildVerticesActual(const std::vector<std::string>& lines,
                             float scale);
    void GenerateCharacterVertices(const CharInfo& ch, float x, float y,
                                   float texW, float texH, float scale);

    // alignment
    glm::vec2 CalculateAlignmentCursor(const std::string& line,
                                       float lineWidth,
                                       float cursorY,
                                       float scale) const;
    void ApplyKerning(char prevChar, char c, float scale, float& xCursor);

    // measure
    float CalculateWordWidth(const std::string& word) const;

    // bounding box debug
    void GenerateBoundingBoxVertices();

    // GPU push
    void pushVertexData(wolf::VertexBuffer*& vBuffer,
                        wolf::VertexDeclaration*& vDecl,
                        const std::vector<Vertex>& inVerts);

private:
    Font* m_font;
    TextTable* m_pTextTable;  // for placeholders
    std::string m_text;       // the raw text (could be a direct string or a textID)
    std::string m_observeID;  // if non-empty, we auto-get the text from the table

    float m_positionX, m_positionY;
    float m_width, m_height;

    // alignment
    int m_hAlign = 0; // 0=left,1=center,2=right
    int m_vAlign = 0; // 0=top,1=middle,2=bottom

    // debugging
    bool m_visualization = true;
    bool m_shrinkToFit = false;

    // which miniFont index in m_font do we use?
    int m_fontIndex = 0;

    glm::vec4 m_color; // (r,g,b,a)

    // GPU
    wolf::Program* m_pProgram;
    wolf::VertexBuffer* m_vertexBuffer = nullptr;
    wolf::VertexDeclaration* m_vertexDecl = nullptr;
    int m_numVertices = 0;

    // CPU data
    std::vector<Vertex> m_vertices;
};
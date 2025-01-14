#include "TextBox.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

TextBox::TextBox(Font* font, const std::string& text, float width, float height)
    : m_font(font), m_text(text), m_width(width), m_height(height), m_alignment(0) {
    m_color = glm::vec4(255);
    // m_color = glm::vec4(1.0f);
}

void TextBox::SetText(const std::string& text) {
    m_text = text;
    GenerateVertices();
}

void TextBox::SetPosition(float x, float y) {
    m_position = glm::vec2(x, y);
    GenerateVertices();
}

void TextBox::SetAlignment(int alignment) {
    m_alignment = alignment;
    GenerateVertices();
}

void TextBox::SetColor(float r, float g, float b, float a) {
    m_color = glm::vec4(r, g, b, a);
    GenerateVertices();
}

void TextBox::SetVisualization(bool visualization){
    m_visualization = visualization;
}

void TextBox::GenerateVertices() {
    m_vertices.clear();

    if(m_visualization)
    {
        //draw lines around the text box
        m_position;
        m_width;
        m_height;
        float uStart = 31;
        float uEnd = 45;
        float vStart = 106;
        float vEnd = 107;
        
        Vertex v1 = {m_position.x, m_position.y - m_height, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vEnd};   // Bottom left
        Vertex v2 = {m_position.x + m_width, m_position.y - m_height, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vEnd}; // Bottom right
        Vertex v3 = {m_position.x, m_position.y, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vStart};     // Top left
        Vertex v4 = {m_position.x + m_width, m_position.y, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vStart};   // Top right

        m_vertices.push_back(v1);
        m_vertices.push_back(v2);
        m_vertices.push_back(v3);

        m_vertices.push_back(v2);
        m_vertices.push_back(v4);
        m_vertices.push_back(v3);

    }

    // Starting position for rendering (top-left of the text box)
    vec2 cursor = vec2(m_position.x, m_position.y);

    // Loop through each character in the text
    for (size_t i = 0; i < m_text.size(); ++i) {
        char c = m_text[i];
        const CharInfo& ch = m_font->GetCharacter(c);

        // WHAT THE FUCK IS KERNING? ;;; Some pairs are closer to eachother than others
        // if (i > 0) {
        //     char prevChar = m_text[i - 1];
        //     auto kerningIt = m_font->GetKerning().find({prevChar, c});
        //     if (kerningIt != m_font->GetKerning().end()) {
        //         cursor.x += kerningIt->second;
        //     }
        // }

        // Calculate the position and size of the character quad
        float xpos = cursor.x + ch.xOffset;
        float ypos = cursor.y - ch.yOffset; // Move down for negative yOffset
        float w    = ch.uEnd;
        float h    = ch.vEnd;

        // Texture coordinates (normalized to [0, 1])
        float textureWidth = m_font->GetTexture()->GetWidth();
        float textureHeight = m_font->GetTexture()->GetHeight();

        float uStart =  ch.uStart      / textureWidth;
        float uEnd   = (ch.uStart + w) / textureWidth;
        float vStart =  ch.vStart      / textureHeight;
        float vEnd   = (ch.vStart + h) / textureHeight;
        
        Vertex v1 = {xpos, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vEnd};   // Bottom left
        Vertex v2 = {xpos + w, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vEnd}; // Bottom right
        Vertex v3 = {xpos, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vStart};     // Top left
        Vertex v4 = {xpos + w, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vStart};   // Top right

        // Add the two triangles to form the quad
        m_vertices.push_back(v1);
        m_vertices.push_back(v2);
        m_vertices.push_back(v3);

        m_vertices.push_back(v2);
        m_vertices.push_back(v4);
        m_vertices.push_back(v3);

        // Advance the cursor for the next character
        cursor.x += ch.xAdvance;
    }

    // Debug output for the generated vertices
    std::cout << "Generating vertices for text: " << m_text << std::endl;
    std::cout << "Number of vertices generated: " << m_vertices.size() << std::endl;

    for (const auto& vertex : m_vertices) {
        std::cout << "Vertex Position: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")";
        std::cout << " Color: (" << (int)vertex.r << ", " << (int)vertex.g << ", " << (int)vertex.b << ")";
        std::cout << " TexCoords: (" << vertex.u << ", " << vertex.v << ")" << std::endl;
    }
}

const std::vector<Vertex>& TextBox::GetVertices() const {
    return m_vertices;
}

Font* TextBox::GetFont() const {
    return m_font;
}
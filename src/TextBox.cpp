#include "TextBox.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

TextBox::TextBox(Font* font, const string& text, float width, float height)
    : m_font(font), m_text(text), m_width(width), m_height(height), m_alignment(0) {
    m_color = vec4(255);
    // m_color = glm::vec4(1.0f);
}

void TextBox::SetText(const string& text) {
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

float TextBox::CalculateWordWidth(const std::string& word) const {
    float wordWidth = 0.0f;
    for (size_t i = 0; i < word.size(); ++i) {
        char c = word[i];
        const CharInfo& ch = m_font->GetCharacter(c);

        // Add kerning adjustment for consecutive characters
        if (i > 0) {
            char prevChar = word[i - 1];
            auto kerningIt = m_font->GetKerning().find({prevChar, c});
            if (kerningIt != m_font->GetKerning().end()) {
                wordWidth += kerningIt->second;
            }
        }

        wordWidth += ch.xAdvance;
    }
    return wordWidth;
}

// /*
void TextBox::GenerateVertices() {
    m_vertices.clear();

    float textureWidth = m_font->GetTexture()->GetWidth();
    float textureHeight = m_font->GetTexture()->GetHeight();

    // Compute the starting cursor position based on alignment
    vec2 cursor = CalculateAlignmentCursor();
    cout << "Calculated Alignment Cursor: (" << cursor.x << ", " << cursor.y << ")" <<endl;
    float lineWidth = 0.0f; // Keeps track of the width of the current line

    // Split text into words
    std::istringstream wordStream(m_text);
    std::string word;

    
    while (wordStream >> word) {
        // Calculate the width of the word
        float wordWidth = CalculateWordWidth(word);

        // Check if the word fits in the current line
        if (lineWidth + wordWidth > m_width) {
            // Move to the next line
            cursor.x = m_position.x;
            cursor.y -= m_font->GetLineHeight();
            lineWidth = 0.0f;
        }

        // Render the word character by character
        for (size_t i = 0; i < word.size(); ++i) {
            char c = word[i];
            const CharInfo& ch = m_font->GetCharacter(c);

            // Adjust cursor for kerning
            ApplyKerning(i, c, cursor);

            // Generate vertices for the current character
            GenerateCharacterVertices(ch, cursor, textureWidth, textureHeight);

            // Advance cursor and line width
            cursor.x += ch.xAdvance;
            lineWidth += ch.xAdvance;
        }

        // Add a space after the word (but don't render it yet)
        if (lineWidth + m_font->GetCharacter(' ').xAdvance <= m_width) {
            cursor.x += m_font->GetCharacter(' ').xAdvance;
            lineWidth += m_font->GetCharacter(' ').xAdvance;
        }
    }

    // Generate bounding box visualization if needed
    if (m_visualization) {
        GenerateBoundingBoxVertices();
    }

    /*
    for (size_t i = 0; i < m_text.size(); ++i) {
        char c = m_text[i];
        const CharInfo& ch = m_font->GetCharacter(c);
        
        lineWidth += ch.xAdvance;

        if (lineWidth + ch.xAdvance > m_width) {
            // Move to the next line
            cursor.x = m_position.x;
            cursor.y -= m_font->GetLineHeight();
            cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n\n\n"<<endl;
            lineWidth = 0.0f;

            // If the current character is a space, skip it at the beginning of the new line
            if (c == ' ') {
                continue;
            }
        }

        // Adjust cursor for kerning
        ApplyKerning(i, c, cursor);

        // Generate vertices for the current character
        GenerateCharacterVertices(ch, cursor, textureWidth, textureHeight);

        cursor.x += ch.xAdvance;
    }

    // Generate bounding box visualization if needed
    if (!m_visualization) {
        GenerateBoundingBoxVertices();
    }

    
    // // Debug 
    // cout << "Generating vertices for text: " << m_text << endl;
    // cout << "Number of vertices generated: " << m_vertices.size() << endl;
    // for (const auto& vertex : m_vertices) {
    //     cout << "Vertex Position: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")";
    //     cout << " Color: (" << (int)vertex.r << ", " << (int)vertex.g << ", " << (int)vertex.b << ")";
    //     cout << " TexCoords: (" << vertex.u << ", " << vertex.v << ")" << endl;
    // }
*/
}

vec2 TextBox::CalculateAlignmentCursor() {
    float textWidth = CalculateTextWidth(); // Total width of the text in pixels

    m_alignment = 0;

    switch (m_alignment) {
        case 0: // Left-aligned
            return vec2(m_position.x, m_position.y);
        case 1: // Center-aligned
            return vec2(m_position.x + (m_width - textWidth) / 2, m_position.y);
        case 2: // Right-aligned
            return vec2(m_position.x + (m_width - textWidth), m_position.y);
        default: // Default to left alignment
            return vec2(m_position.x, m_position.y);
    }
}

float TextBox::CalculateTextWidth() const {
    float textWidth = 0.0f;
    for (size_t i = 0; i < m_text.size(); ++i) {
        char c = m_text[i];
        const CharInfo& ch = m_font->GetCharacter(c);

        // if(c == ' ')
        // {
        //     cout<<"TEXT WIDTH: "<<textWidth<<endl;
        //     return textWidth;
        // }

        // Add kerning adjustment for consecutive characters
        if (i > 0) {
            char prevChar = m_text[i - 1];
            auto kerningIt = m_font->GetKerning().find({prevChar, c});
            if (kerningIt != m_font->GetKerning().end()) {
                textWidth += kerningIt->second;
            }
        }

        textWidth += ch.xAdvance;
    }
    cout<<"TEXT WIDTH: "<<textWidth<<endl;
    return textWidth;
}

void TextBox::ApplyKerning(size_t index, char c, vec2& cursor) {
    if (index > 0) {
        char prevChar = m_text[index - 1];
        auto kerningIt = m_font->GetKerning().find({prevChar, c});
        if (kerningIt != m_font->GetKerning().end()) {
            cursor.x += kerningIt->second;
            cout << "Applied Kerning: " << kerningIt->second 
                      << " for pair (" << prevChar << ", " << c << ")" << endl;
        }
    }
}

void TextBox::GenerateCharacterVertices(const CharInfo& ch, vec2 cursor, float textureWidth, float textureHeight) {
    float xpos = cursor.x + ch.xOffset;
    float ypos = cursor.y - ch.yOffset;
    float w = ch.uEnd;
    float h = ch.vEnd;

    float uStart = ch.uStart / textureWidth;
    float uEnd = (ch.uStart + w) / textureWidth;
    float vStart = ch.vStart / textureHeight;
    float vEnd = (ch.vStart + h) / textureHeight;

    Vertex v1 = {xpos, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vEnd};   // Bottom left
    Vertex v2 = {xpos + w, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vEnd}; // Bottom right
    Vertex v3 = {xpos, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vStart};     // Top left
    Vertex v4 = {xpos + w, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vStart};   // Top right

    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);

    m_vertices.push_back(v2);
    m_vertices.push_back(v4);
    m_vertices.push_back(v3);
}

void TextBox::GenerateBoundingBoxVertices() {
    float uStart = 0;
    float uEnd = 1;
    float vStart = 0;
    float vEnd = 1;
    vec4 color = vec4(0, 0, 0, 255);

    Vertex a1 = {
        m_position.x, m_position.y - m_height, 0.0f,
        color.x, color.y, color.z, color.w,
        uStart, vEnd};

    Vertex a2 = {
        m_position.x + m_width, m_position.y - m_height, 0.0f,
        color.x, color.y, color.z, color.w,
        uEnd, vEnd};

    Vertex a3 = {
        m_position.x, m_position.y, 0.0f,
        color.x, color.y, color.z, color.w,
        uStart, vStart};

    Vertex a4 = {
        m_position.x + m_width, m_position.y, 0.0f,
        color.x, color.y, color.z, color.w,
        uEnd, vStart};

    m_vertices.push_back(a1);
    m_vertices.push_back(a2);
    m_vertices.push_back(a3);

    m_vertices.push_back(a2);
    m_vertices.push_back(a4);
    m_vertices.push_back(a3);
}
// */
/*
void TextBox::GenerateVertices() {
    m_vertices.clear();
    vec2 cursor = vec2(m_position.x, m_position.y);
    
    float lineWidth = 0;

    float textureWidth = m_font->GetTexture()->GetWidth();
    float textureHeight = m_font->GetTexture()->GetHeight();

    for (size_t i = 0; i < m_text.size(); ++i) {
        char c = m_text[i];
        const CharInfo& ch = m_font->GetCharacter(c);

        //  if (c == ' ') {
        //     lineWidth += ch.xAdvance;
        //     if (lineWidth > m_width) {
        //         cursor.y -= m_font->GetLineHeight(); // Move to next line
        //         cursor.x = m_position.x;
        //         lineWidth = 0;
        //     }
        //     continue;
        // }
        // WHAT THE FUCK IS KERNING? ;;; Some pairs are closer to eachother than others
        // if (i > 0) {
        //     char prevChar = m_text[i - 1];
        //     auto kerningIt = m_font->GetKerning().find({prevChar, c});
        //     if (kerningIt != m_font->GetKerning().end()) {
        //         cursor.x += kerningIt->second;
        //     }
        // }

        float xpos = cursor.x + ch.xOffset;
        float ypos = cursor.y - ch.yOffset;
        float w    = ch.uEnd;
        float h    = ch.vEnd;

        float uStart =  ch.uStart      / textureWidth;
        float uEnd   = (ch.uStart + w) / textureWidth;
        float vStart =  ch.vStart      / textureHeight;
        float vEnd   = (ch.vStart + h) / textureHeight;
        
        Vertex v1 = {xpos, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vEnd};   // Bottom left
        Vertex v2 = {xpos + w, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vEnd}; // Bottom right
        Vertex v3 = {xpos, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vStart};     // Top left
        Vertex v4 = {xpos + w, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vStart};   // Top right

        m_vertices.push_back(v1);
        m_vertices.push_back(v2);
        m_vertices.push_back(v3);

        m_vertices.push_back(v2);
        m_vertices.push_back(v4);
        m_vertices.push_back(v3);

        cursor.x += ch.xAdvance;
    }

    // // Debug 
    // cout << "Generating vertices for text: " << m_text << endl;
    // cout << "Number of vertices generated: " << m_vertices.size() << endl;
    // for (const auto& vertex : m_vertices) {
    //     cout << "Vertex Position: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")";
    //     cout << " Color: (" << (int)vertex.r << ", " << (int)vertex.g << ", " << (int)vertex.b << ")";
    //     cout << " TexCoords: (" << vertex.u << ", " << vertex.v << ")" << endl;
    // }

    
    if(!m_visualization)
    {
        //draw lines around the text box
        m_position;
        m_width;
        m_height;
        float uStart = 0;
        float uEnd = 1;
        float vStart = 0;
        float vEnd = 1;
        // vec4 color = m_color;
        vec4 color = vec4(0,0,0,255);
        cout<<"height: "<<m_height<<endl;
        cout<<"width: "<<m_width<<endl;
        Vertex a1 = {m_position.x, m_position.y - m_height, 0.0f, color.x, color.y, color.z, color.w, uStart, vEnd};   // Bottom left
        Vertex a2 = {m_position.x + m_width, m_position.y - m_height, 0.0f, color.x, color.y, color.z, color.w, uEnd, vEnd}; // Bottom right
        Vertex a3 = {m_position.x, m_position.y, 0.0f, color.x, color.y, color.z, color.w, uStart, vStart};     // Top left
        Vertex a4 = {m_position.x + m_width, m_position.y, 0.0f, color.x, color.y, color.z, color.w, uEnd, vStart};   // Top right

        m_vertices.push_back(a1);
        m_vertices.push_back(a2);
        m_vertices.push_back(a3);

        m_vertices.push_back(a2);
        m_vertices.push_back(a4);
        m_vertices.push_back(a3);

        cout << "\nBounding Box Visualization Vertices:" << std::endl;
        cout << "\nVertex Bottom left - Position: (" << a1.x << ", " << a1.y << ")";
        cout << "\nVertex Bottom right - Position: (" << a2.x << ", " << a2.y << ")";
        cout << "\nVertex Top left - Position: (" << a3.x << ", " << a3.y << ")";
        cout << "\nVertex Top Right - Position: (" << a4.x << ", " << a4.y << ")";
        
    }
}
*/
const vector<Vertex>& TextBox::GetVertices() const {
    return m_vertices;
}

Font* TextBox::GetFont() const {
    return m_font;
}
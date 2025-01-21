#include "TextBox.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

TextBox::TextBox(Font* font, const string& text, float width, float height, wolf::Program* shader)
    : m_font(font), m_text(text), m_width(width), m_height(height), m_alignment(0) {
    
    // cout << "TextBox initialized with Font at address: " << m_font << endl;
    m_color = vec4(255);
    m_pProgram = shader;
    // m_color = glm::vec4(1.0f);
    cout<<"INITIALIZED"<<endl;
}
TextBox::~TextBox() {
    if (m_vertexBuffer) {
        wolf::BufferManager::DestroyBuffer(m_vertexBuffer);
    }
    if (m_vertexDecl) {
        delete m_vertexDecl;
    }
    // if (m_font){
    //     delete m_font;
    // }
}

void TextBox::SetText(const string& text) {
    m_text = text;
    GenerateVertices();
}

void TextBox::SetPosition(float x, float y) {
    m_position = glm::vec2(x, y);
        cout<<"POSIITION"<<endl;
    GenerateVertices();
        cout<<"VERTICIES"<<endl;
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

float TextBox::CalculateWordWidth(const std::string& word){
    float wordWidth = 0.0f;
    for (size_t i = 0; i < word.size(); ++i) {
        char c = word[i];
        
        if(c == '\n')
        {
            broke = true;
            return wordWidth;
        }

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

    if (!m_visualization) {
        GenerateBoundingBoxVertices();
    }

    float textureWidth = m_font->GetTexture()->GetWidth();
    float textureHeight = m_font->GetTexture()->GetHeight();
    // Split text into lines based on '\n'
    std::vector<std::string> lines;
    std::stringstream ss(m_text);
    std::string line;
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    float cursorY = m_position.y;
    for (const auto& currentLine : lines) {
        float lineWidth = CalculateWordWidth(currentLine);
        glm::vec2 cursor = CalculateAlignmentCursor(currentLine, lineWidth, cursorY);

        // Iterate over characters in the line
        CharInfo prevCharInfo = m_font->GetCharacter(' '); // Initialize with space for kerning
        for (size_t i = 0; i < currentLine.size(); ++i) {
            char c = currentLine[i];
            if (c == '\n') continue; // Already handled by splitting lines

            const CharInfo& ch = m_font->GetCharacter(c);

            // Apply kerning
            ApplyKerning(i,c,cursor);
            // auto kerningIt = m_font->GetKerning().find({ static_cast<char>(prevCharInfo.id), c });
            // if (kerningIt != m_font->GetKerning().end()) {
            //     cursor.x += kerningIt->second;
            // }

            GenerateCharacterVertices(ch, cursor, textureWidth, textureHeight);

            cursor.x += ch.xAdvance;
            prevCharInfo = ch;
        }

        // Move cursor to next line
        cursorY -= m_font->GetLineHeight();
    }
    pushVertexData(m_vertexBuffer, m_vertexDecl, m_vertices);

    /*
    // Compute the starting cursor position based on alignment
    vec2 cursor = CalculateAlignmentCursor();
    // cout << "Calculated Alignment Cursor: (" << cursor.x << ", " << cursor.y << ")" <<endl;
    float lineWidth = 0.0f; // Keeps track of the width of the current line

    // Split text into words
    istringstream wordStream(m_text);
    string word;
    
    bool dumb = false;


    while (wordStream >> word) {
        // Calculate the width of the word
        float wordWidth = CalculateWordWidth(word);

        // Check if the word fits in the current line
        if (lineWidth + wordWidth > m_width || dumb) {
            // Move to the next line
            broke = false, dumb = false;

            // cursor = CalculateAlignmentCursor();
            cursor.x = m_position.x;
            cursor.y -= m_font->GetLineHeight();
            lineWidth = 0.0f;
        }

        // Render the word character by character
        for (size_t i = 0; i < word.size(); ++i) {
            char c = word[i];

            const CharInfo& ch = m_font->GetCharacter(c);
            ApplyKerning(i, c, cursor);
            GenerateCharacterVertices(ch, cursor, textureWidth, textureHeight);

            cursor.x += ch.xAdvance;
            lineWidth += ch.xAdvance;
        }

        // Add a space after the word (but don't render it yet)
        if (lineWidth + m_font->GetCharacter(' ').xAdvance <= m_width) {
            cursor.x += m_font->GetCharacter(' ').xAdvance;
            lineWidth += m_font->GetCharacter(' ').xAdvance;
        }
        if(broke) dumb = true;
    }
    

    pushVertexData(m_vertexBuffer, m_vertexDecl, m_vertices);
    cout<<"TExxture: "<<m_font->GetTexture()<<endl;
    */
}

vec2 TextBox::CalculateAlignmentCursor(const std::string& line, float lineWidth, float cursorY) {
    switch (m_alignment) {
        case 0: // Left-aligned
            return glm::vec2(m_position.x, cursorY);
        case 1: // Center-aligned
            return glm::vec2(m_position.x + (m_width - lineWidth) / 2.0f, cursorY);
        case 2: // Right-aligned
            return glm::vec2(m_position.x + (m_width - lineWidth), cursorY);
        default:
            return glm::vec2(m_position.x, cursorY);
    }
}

void TextBox::ApplyKerning(size_t index, char c, vec2& cursor) {
    if (index > 0) {
        char prevChar = m_text[index - 1];
        auto kerningIt = m_font->GetKerning().find({prevChar, c});
        if (kerningIt != m_font->GetKerning().end()) {
            cursor.x += kerningIt->second;
            // cout << "Applied Kerning: " << kerningIt->second 
            //           << " for pair (" << prevChar << ", " << c << ")" << endl;
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
    GLfloat page = (GLfloat)(ch.page);
    
    GLubyte r = (GLubyte)(m_color.x);
    GLubyte g = (GLubyte)(m_color.y);
    GLubyte b = (GLubyte)(m_color.z);
    GLubyte a = (GLubyte)(m_color.w);

    Vertex v1 = {xpos, ypos - h, 0.0f, r, g, b, a, uStart, vEnd, page};   // Bottom left
    Vertex v2 = {xpos + w, ypos - h, 0.0f, r, g, b, a, uEnd, vEnd, page}; // Bottom right
    Vertex v3 = {xpos, ypos, 0.0f, r, g, b, a, uStart, vStart, page};     // Top left
    Vertex v4 = {xpos + w, ypos, 0.0f, r, g, b, a, uEnd, vStart, page};   // Top right

    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);

    m_vertices.push_back(v2);
    m_vertices.push_back(v4);
    m_vertices.push_back(v3);
}

void TextBox::GenerateBoundingBoxVertices() {
    return;
    //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    float uStart = 0;
    float uEnd = 1;
    float vStart = 0;
    float vEnd = 1;
    // int layer = -1;
    
    GLfloat layer = (GLfloat)(-1);
    vec4 color = vec4(0, 0, 0, 255);
    
    GLubyte r = (GLubyte)(color.x);
    GLubyte g = (GLubyte)(color.y);
    GLubyte b = (GLubyte)(color.z);
    GLubyte a = (GLubyte)(color.w);

    Vertex a1 = {
        m_position.x, m_position.y - m_height, 0.0f,
        r, g, b, a,
        uStart, vEnd,
        layer};

    Vertex a2 = {
        m_position.x + m_width, m_position.y - m_height, 0.0f,
        r, g, b, a,
        uEnd, vEnd,
        layer};

    Vertex a3 = {
        m_position.x, m_position.y, 0.0f,
        r, g, b, a,
        uStart, vStart,
        layer};

    Vertex a4 = {
        m_position.x + m_width, m_position.y, 0.0f,
        r, g, b, a,
        uEnd, vStart,
        layer};

    m_vertices.push_back(a1);
    m_vertices.push_back(a2);
    m_vertices.push_back(a3);

    m_vertices.push_back(a2);
    m_vertices.push_back(a4);
    m_vertices.push_back(a3);
}

const vector<Vertex>& TextBox::GetVertices() const {
    return m_vertices;
}

Font* TextBox::GetFont() const {
    return m_font;
}

void TextBox::pushVertexData(wolf::VertexBuffer*& vBuffer, wolf::VertexDeclaration*& vDecl, const vector<Vertex>& vertices)
{
    vBuffer = wolf::BufferManager::CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    vDecl = new wolf::VertexDeclaration();
    vDecl->Begin();
    vDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    vDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
    vDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    vDecl->AppendAttribute(wolf::AT_TexCoord2, 1, wolf::CT_Float);
    vDecl->SetVertexBuffer(vBuffer);
    vDecl->End();
    m_numVertices = vertices.size();
}

void TextBox::Render(const glm::mat4& proj, const glm::mat4& view)
{
    if (!m_pProgram || !m_vertexBuffer || !m_vertexDecl || m_numVertices == 0) return;
    
    m_pProgram->Bind();
    m_pProgram->SetUniform("projection", proj);
    m_pProgram->SetUniform("view", view);

    m_font->GetTexture()->Bind(0);
    m_pProgram->SetUniform("u_texture", 0);

    // cout<<"Binding: "<<m_font->GetTexture()<<endl;

    m_vertexDecl->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
}

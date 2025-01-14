#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using namespace std;

TextRenderer::TextRenderer()
    : m_pProgram(nullptr), m_vertexBuffer(nullptr), m_vertexDecl(nullptr), m_currentTextBox(nullptr) {}

TextRenderer::~TextRenderer() {
    if (m_vertexBuffer) {
        wolf::BufferManager::DestroyBuffer(m_vertexBuffer);
    }
    if (m_vertexDecl) {
        delete m_vertexDecl;
    }
    if (m_pProgram) {
        wolf::ProgramManager::DestroyProgram(m_pProgram);
    }
}

void TextRenderer::pushVertexData(wolf::VertexBuffer*& vBuffer, wolf::VertexDeclaration*& vDecl, const vector<Vertex>& vertices)
{
    vBuffer = wolf::BufferManager::CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    vDecl = new wolf::VertexDeclaration();
    vDecl->Begin();
    vDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    vDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
    vDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    vDecl->SetVertexBuffer(vBuffer);
    vDecl->End();

    m_numVertices = vertices.size();
}

void TextRenderer::init() {
    m_pProgram = wolf::ProgramManager::CreateProgram("data/2d.vsh", "data/2d.fsh");
    m_vertices.clear();
    
    // vector<Vertex> quadVertices = {
    //     {100.0f, 100.0f, 0.0f, 255, 0, 0, 255, 0.0f, 0.0f},
    //     {200.0f, 100.0f, 0.0f, 255, 0, 0, 255, 1.0f, 0.0f},
    //     {100.0f, 200.0f, 0.0f, 255, 0, 0, 255, 0.0f, 1.0f},

    //     {200.0f, 100.0f, 0.0f, 255, 0, 0, 255, 1.0f, 0.0f},
    //     {200.0f, 200.0f, 0.0f, 255, 0, 0, 255, 1.0f, 1.0f},
    //     {100.0f, 200.0f, 0.0f, 255, 0, 0, 255, 0.0f, 1.0f},
    // };

    // m_vertices = quadVertices;

    // pushVertexData(m_vertexBuffer, m_vertexDecl, m_vertices);
}

void TextRenderer::update(float dt) {}

void TextRenderer::setTextBox(TextBox* textBox) {
    m_currentTextBox = textBox;
    const auto& vertices = textBox->GetVertices();

    m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());

    pushVertexData(m_vertexBuffer, m_vertexDecl, m_vertices);

}

void TextRenderer::render(const glm::mat4& proj, const glm::mat4& view) {
    if(!m_pProgram){return;}

    if (!m_currentTextBox) return;

    m_pProgram->Bind();
    m_pProgram->SetUniform("projection", proj);
    m_pProgram->SetUniform("view", view);
    m_pProgram->SetUniform("u_texture", 0);
    m_currentTextBox->GetFont()->GetTexture()->Bind(0);

    m_vertexDecl->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    // glDrawArraysInstanced()
}

Font* TextRenderer::createFont(const std::string& texturePath, const std::string& fontDataPath) {
    return new Font(texturePath, fontDataPath);
}

TextBox* TextRenderer::createTextBox(Font* font, const std::string& text, float x, float y) {
    auto textBox = new TextBox(font, text, 0.0f, 0.0f);
    textBox->SetPosition(x, y);
    return textBox;
}


/*#include "TextRenderer.h"
#include "TextBox.h"
#include "Font.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../samplefw/Grid2D.h"

TextRenderer::~TextRenderer()
{
    if (m_pGrid) {
        delete m_pGrid;
        m_pGrid = nullptr;
    }
    if (m_pDecl) {
        delete m_pDecl;
        m_pDecl = nullptr;
    }
    if (m_pVB) {
        wolf::BufferManager::DestroyBuffer(m_pVB);
        m_pVB = nullptr;
    }
    if (m_pProgram) {
        wolf::ProgramManager::DestroyProgram(m_pProgram);
        m_pProgram = nullptr;
    }
}

void TextRenderer::PushVertexData(wolf::VertexBuffer*& vBuffer, wolf::VertexDeclaration*& vDecl, vector<Vertex>& vertices)
{
    m_pVB = wolf::BufferManager::CreateVertexBuffer(nullptr, 0);
    m_pDecl = new wolf::VertexDeclaration();
    
    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
    m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
    m_pDecl->SetVertexBuffer(m_pVB);
    m_pDecl->End();
}

void TextRenderer::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    if (!m_pProgram) {
        m_pProgram = wolf::ProgramManager::CreateProgram("data/2d.vsh", "data/2d.fsh");

        
        PushVertexData(m_pVB, m_pDecl, m_numVertices);

        m_pGrid = new Grid2D(30);
    }
}

void TextRenderer::update(float dt)
{
	//Updates?
}

void TextRenderer::render(int width, int height)
{
    
    glClearColor(0.3f, 0.3f, 0.3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float fWidth = static_cast<float>(width);
    float fHeight = static_cast<float>(height);
    glm::mat4 mProj = glm::ortho(0.0f, fWidth, 0.0f, fHeight, 0.0f, 1000.0f);

    glDepthMask(GL_FALSE);
    m_pGrid->render(glm::mat4(1.0f), mProj);
    glDepthMask(GL_TRUE);
}

TextBox* TextRenderer::createTextBox(const std::string& text, Font& font, float x, float y, float scale)
{
    return new TextBox(text, font, x, y, scale);
}

Font* TextRenderer::createFont(const std::string& texturePath, const std::string& fontDataPath)
{
    return new Font(texturePath, fontDataPath);
}
*/
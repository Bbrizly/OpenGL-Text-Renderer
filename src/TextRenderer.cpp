#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using namespace std;

TextRenderer::TextRenderer()
    : TextShader(nullptr) {}

TextRenderer::~TextRenderer() {
    if (TextShader) {
        wolf::ProgramManager::DestroyProgram(TextShader);
    }
}

void TextRenderer::init(TextTable* pTable) {
    TextShader = wolf::ProgramManager::CreateProgram("data/2d.vsh", "data/2d.fsh");
    
    m_pTextTable = pTable;
}

void TextRenderer::update(float dt) {}

void TextRenderer::setTextBox(TextBox* textBox) {
    // cout<<"Set Texture: "<<textBox->GetFont()->GetTexture()<<endl;

    m_textBoxes.push_back(textBox);
}

void TextRenderer::render(const glm::mat4& proj, const glm::mat4& view) {
    if(!TextShader || m_textBoxes.empty()) return;

    for( auto textBox : m_textBoxes ){
        textBox->Render(proj,view);
    }
}

Font* TextRenderer::createFont(const std::string& fontDataPath) {
    return new Font(fontDataPath);
}

TextBox* TextRenderer::createTextBox(Font* font, const std::string& text, float x, float y, float width, float height) {
    
    // cout<<"Create Texture: "<<font->GetTexture()<<endl;
    auto textBox = new TextBox(font, text, width, height, TextShader, m_pTextTable);

    // cout<<"BFEORE SET POSITOIN"<<endl;
    
    textBox->SetPosition(x, y);
    return textBox;
}

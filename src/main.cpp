#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "../samplefw/SampleRunner.h"
#include "TextRenderer.h"
#include "Font.h"
#include "TextBox.h"
#include "TextTable.h"

class Main : public wolf::App {
private:
    TextRenderer* m_textRenderer;

    
    TextBox* textBoz;
    TextBox* textBox;
    TextBox* textBoc;
    TextBox* textBov;

public:
    Main() : App("Text Renderer") {
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        m_textRenderer = new TextRenderer();
        
        TextTable table("data/test.csv");
        m_textRenderer->init(&table);
        
        // Create Font and TextBox
        Font* font = m_textRenderer->createFont("data/amphabet.fnt");
        Font* font1 = m_textRenderer->createFont("data/English_Alphabet.fnt");
        Font* font2 = m_textRenderer->createFont("data/amphabet.fnt");
        // Font* font = m_textRenderer->createFont("data/test.fnt");

        textBoz = m_textRenderer->createTextBox(font2, "HEALTH: 87%", 010, 700, 200, 100);
        textBox = m_textRenderer->createTextBox(font1, "STA\nMINA: 100%", 500, 600, 400, 400);
        textBoc = m_textRenderer->createTextBox(font2, "AM\nMO: 60/60", 900, 700, 300, 50);
        textBov = m_textRenderer->createTextBox(font2, "Graphics\n 436: As\nsignment 1", 100, 500, 400, 100);
        

        table.SetLanguage("English");
        table.SetStringProperty("playerName","Bassam Kamal");
        table.SetStringProperty("scoreValue","999");

        std::string greet = table.GetString("str_greeting");
        std::cout << "Greeting in English: " << greet << "\n";

        table.SetLanguage("Spanish");
        greet = table.GetString("str_greeting");
        
        // Graphics 436: Assignment 1

        // Pass TextBox to TextRenderer
        textBoz->SetColor(255,0,0,255);
        textBox->SetColor(0,0,255,255);
        textBoc->SetColor(0,255,0,255);
        textBov->SetColor(0,0,0,255);

        // textBoz->SetAlignment(1);
        textBox->SetAlignment(1);
        textBox->SetVerticalAlignment(2);
        // textBoc->SetAlignment(1);
        // textBov->SetAlignment(1);
        
        textBox->printf("Your score is %d", 9001);
        textBoc->SetText(table.GetString("str_greeting"));
        
        textBoc->SetTextTable(&table);
        textBoc->SetText("Hello {playerName}");


        m_textRenderer->setTextBox(textBoz);
        m_textRenderer->setTextBox(textBox);
        m_textRenderer->setTextBox(textBoc);
        m_textRenderer->setTextBox(textBov);

        // cout<<"FINAL TEXTURE: "<<textBoz->GetFont()->GetTexture()<<endl;
        // cout<<"FINAL TEXTURE: "<<textBox->GetFont()->GetTexture()<<endl;
        // cout<<"FINAL TEXTURE: "<<textBoc->GetFont()->GetTexture()<<endl;
        // cout<<"FINAL TEXTURE: "<<textBov->GetFont()->GetTexture()<<endl;
    }

    ~Main() {
        delete m_textRenderer;
    }

    void update(float dt) override {
        m_textRenderer->update(dt);
    }

    void render() override {
        // glViewport(0, 0, m_width, m_height);
        // return;

        glClearColor(0,0.5,0.5,255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        // Define projection and view matrices
        glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(m_width), 0.0f, static_cast<float>(m_height), -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);

        textBoz->Render(proj,view);
        textBox->Render(proj,view);
        textBoc->Render(proj,view);
        textBov->Render(proj,view);
        

        // Render using TextRenderer
        // m_textRenderer->render(proj, view);

        // m_textRenderer->render(m_width, m_height);
    }
};

int main(int, char**) {
    Main main;
    main.run();
}

/*#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "../samplefw/SampleRunner.h"
#include "TextRenderer.h"
#include "Font.h"
#include "TextBox.h"

class Main: public wolf::App
{
private:
    TextRenderer* m_textRenderer;
    TextBox* m_textBox;
    Font* m_font;

public:
    Main() : App("Text Renderer")
    {
        m_textRenderer = new TextRenderer();
        m_textRenderer->init();

        // Create Font
        m_font = m_textRenderer->createFont("data/English_Alphabet.tga", "data/English_Alphabet.fnt");

        // Create TextBox
        m_textBox = m_textRenderer->createTextBox("Hello, World!", *m_font, 100.0f, 200.0f, 1.0f);
        // TextBox* textBox = new TextBox(m_font, "Hello, World!", 100.0f, 200.0f, 1.0f);

    }

    ~Main()
    {
        delete m_textRenderer;
        delete m_font;
    }

    void update(float dt) override
    {
        m_textRenderer->update(dt);
    }

    void render() override
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        // glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(m_width), 0.0f, static_cast<float>(m_height), 0.0f, 1.0f);
        // glm::mat4 view = glm::mat4(1.0f); // Identity matrix
        // m_shaderProgram->SetUniform("projection", proj);
        // m_shaderProgram->SetUniform("view", view);

        glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(m_width), 0.0f, static_cast<float>(m_height), 0.0f, 1000.0f);
        // glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(m_width), 0.0f, static_cast<float>(m_height), -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);

        // Render TextBox
        m_textBox->render(proj, view);

        m_textRenderer->render(m_width, m_height);
    }
};

int main(int, char**) {
    Main main;
    main.run();
}*/
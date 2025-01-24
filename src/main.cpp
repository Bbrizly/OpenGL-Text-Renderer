#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../wolf/wolf.h"        
#include "TextRenderer.h"       
#include "Font.h"
#include "TextBox.h"
#include "TextTable.h"

using namespace std;

class Main : public wolf::App {
private:
    TextRenderer* m_textRenderer;
    TextBox*      m_inputBox; 
    TextBox*      m_infoBox;    
    std::string   m_typedString;

    Font* fontA = nullptr;
    Font* fontB = nullptr;
    Font* fontC = nullptr;

    bool          m_typingMode;

public:
    Main() 
        : App("Text Renderer"),
          m_textRenderer(nullptr),
          m_inputBox(nullptr),
          m_infoBox(nullptr),
          m_typedString(""),
          m_typingMode(false)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        m_textRenderer = new TextRenderer();
        
        TextTable* table = new TextTable("data/test.csv");
        m_textRenderer->init(table);
        table->SetLanguage("English");
        table->SetStringProperty("playerName","Raj Mataj");
        table->SetStringProperty("scoreValue","999");

        Font* fontTimes   = m_textRenderer->createFont("TimesNewRoman");
        Font* fontPapyrus = m_textRenderer->createFont("Papyrus");
        Font* fontArial   = m_textRenderer->createFont("Arial");

        fontA = fontTimes;
        fontB = fontPapyrus;
        fontC = fontArial;

        TextBox* textBoz = m_textRenderer->createTextBox(fontArial, 
            "Penguins find all their food in the sea and are carnivores. They eat mostly fish and squid. They also eat crustaceans, such as crabs, shrimp, and krill. A large penguin can collect up to 30 fish in one dive. Penguins (and any animal) that eat only fish are called piscivorous."
            , 550, 700, 200, 100);
        TextBox* textBox = m_textRenderer->createTextBox(fontArial, 
            "", 700, 400, 400, 300);
        TextBox* textBoc = m_textRenderer->createTextBox(fontPapyrus, 
            "", 900, 700, 300, 50);
        TextBox* textBov = m_textRenderer->createTextBox(fontArial, 
            "Graphics 436: Assignment111, {playerName} Graphics 436: Assignment111, Graphics 436: Assignment111, Graphics 436: Assignment111, Graphics 436: Assignment111, Graphics 436: Assignment111",
            700, 550, 400, 100);

        textBox->SetText("Your scoreeeeeeeeeeeeeeeeeeeeeeeee is %d", 9001);
        textBoc->SetText(table->GetString("str_greeting"));
        
        textBoz->SetColor(0,0,0,255);
        textBox->SetColor(0,0,255,255);
        textBoc->SetColor(0,0,0,255);
        textBov->SetColor(0,0,0,255);

        textBox->SetAlignment(2);
        textBox->SetVerticalAlignment(0);

        m_inputBox = m_textRenderer->createTextBox(fontTimes, "", 
                                                   010, 370,
                                                   600, 300);
        m_inputBox->SetColor(0,0,0,255);

        m_infoBox = m_textRenderer->createTextBox(fontArial, 
            "Press ENTER to toggle typing mode.\n"
            "\nWhen typing mode is OFF:\n"
            "Alternate between 1,2,3 to switch font.\n"
            "And to switch alignments:\nHorizontally | 4: left, 5: middle, 6: right\nVertically | 7: Top, 8: Middle, 9: Bottom\n"
            "\nWhen typing mode is ON:\n"
            "- Type letters, digits, punctuation\n"
            "- Press BACKSPACE to delete\n"
            "- Press ENTER again to stop typing",
            10, 700, 500, 300);
        m_infoBox->SetColor(80,80,80,255);
        m_infoBox->SetAlignment(0);

        m_textRenderer->setTextBox(textBoz);
        m_textRenderer->setTextBox(textBox);
        m_textRenderer->setTextBox(textBoc);
        m_textRenderer->setTextBox(textBov);
        m_textRenderer->setTextBox(m_inputBox);
        m_textRenderer->setTextBox(m_infoBox);
    }

    ~Main() override 
    {
        delete m_textRenderer; 
        m_textRenderer = nullptr;
    }

    // Called every frame
    void update(float dt) override 
    {
        m_textRenderer->update(dt);

        // Toggle typing mode if user presses enter
        if(isKeyJustDown(GLFW_KEY_ENTER)) {
            m_typingMode = !m_typingMode;
        }

        // If typing mode is ON, Check for input
        if(m_typingMode) {
            handleTyping();
        }
        else
        {
            // check for input of 1 2 or 3 to switch between fonts
            if(isKeyJustDown(GLFW_KEY_1)) {
                m_inputBox->SetFont(fontA);
            }
            else if(isKeyJustDown(GLFW_KEY_2)) {
                m_inputBox->SetFont(fontB);
            }
            else if(isKeyJustDown(GLFW_KEY_3)) {
                m_inputBox->SetFont(fontC);
            }
            // check for input of 4 5 or 6 to switch between horizaontal alignments
            else if(isKeyJustDown(GLFW_KEY_4)) {
                m_inputBox->SetAlignment(0);
            }
            else if(isKeyJustDown(GLFW_KEY_5)) {
                m_inputBox->SetAlignment(1);
            }
            else if(isKeyJustDown(GLFW_KEY_6)) {
                m_inputBox->SetAlignment(2);
            }

            //vertical alignemtnt
            else if(isKeyJustDown(GLFW_KEY_7)) {
                m_inputBox->SetVerticalAlignment(0);
            }
            else if(isKeyJustDown(GLFW_KEY_8)) {
                m_inputBox->SetVerticalAlignment(1);
            }
            else if(isKeyJustDown(GLFW_KEY_9)) {
                m_inputBox->SetVerticalAlignment(2);
            }
            
                    
        }
    }

    void render() override 
    {
        glClearColor(0.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height, -1.f, 1.f);
        glm::mat4 view(1.0f);

        m_textRenderer->render(proj, view);
    }

private:
    void handleTyping()
    {
        for(int key = GLFW_KEY_A; key <= GLFW_KEY_Z; ++key) {
            if(isKeyJustDown(key)) {
                bool shift = isKeyDown(GLFW_KEY_LEFT_SHIFT) || isKeyDown(GLFW_KEY_RIGHT_SHIFT);
                char c = (char)('a' + (key - GLFW_KEY_A)); // default lowercase
                if(shift) {
                    c = toupper(c);
                }
                m_typedString.push_back(c);
                m_inputBox->SetText(m_typedString);
            }
        }

        // TO get numbers !!
        for(int key = GLFW_KEY_0; key <= GLFW_KEY_9; ++key) {
            if(isKeyJustDown(key)) {
                char c = (char)('0' + (key - GLFW_KEY_0));
                m_typedString.push_back(c);
                m_inputBox->SetText(m_typedString);
            }
        }

        // Some punctuation buttons
        if(isKeyJustDown(GLFW_KEY_SPACE)) {
            m_typedString.push_back(' ');
            m_inputBox->SetText(m_typedString);
        }
        if(isKeyJustDown(GLFW_KEY_PERIOD)) {
            m_typedString.push_back('.');
            m_inputBox->SetText(m_typedString);
        }
        if(isKeyJustDown(GLFW_KEY_COMMA)) {
            m_typedString.push_back(',');
            m_inputBox->SetText(m_typedString);
        }
        if(isKeyJustDown(GLFW_KEY_APOSTROPHE)) {
            m_typedString.push_back('\'');
            m_inputBox->SetText(m_typedString);
        }
        if(isKeyJustDown(GLFW_KEY_MINUS)) {
            m_typedString.push_back('-');
            m_inputBox->SetText(m_typedString);
        }

        // delete last letter placed
        if(isKeyJustDown(GLFW_KEY_BACKSPACE)) {
            if(!m_typedString.empty()) {
                m_typedString.pop_back();
                m_inputBox->SetText(m_typedString);
            }
        }
    }
};

int main(int, char**) {
    Main main;
    main.run();
    return 0;
}

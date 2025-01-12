#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "../samplefw/SampleRunner.h"
#include "TextRenderer.h"

class Main: public wolf::App
{
private:
    TextRenderer* m_textRenderer;
public:
    Main() : App("Text Renderer")
    {
        m_textRenderer = new TextRenderer(this);
        m_textRenderer->init();
    }

    ~Main()
    {
        delete m_textRenderer;
    }

    void update(float dt) override
    {
        m_textRenderer->update(dt);
    }

    void render() override
    {
        m_textRenderer->render(m_width, m_height);
    }
};

int main(int, char**) {
    Main main;
    main.run();
}
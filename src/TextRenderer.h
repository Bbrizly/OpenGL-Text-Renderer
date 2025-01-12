#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

class Grid2D;

class TextRenderer: public Sample
{
public:
    TextRenderer(wolf::App* pApp) : Sample(pApp,"2D Transforms") {}
    ~TextRenderer();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    wolf::VertexBuffer* m_pVB = 0;
    wolf::VertexDeclaration* m_pDecl = 0;
    wolf::Program* m_pProgram = 0;

    Grid2D* m_pGrid = nullptr;
};
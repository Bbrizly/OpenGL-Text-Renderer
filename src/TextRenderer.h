#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

class Grid2D;

class Font: public Sample
{
public:
    Font(wolf::App* pApp) : Sample(pApp,"2D Transforms") {}
    ~Font();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    wolf::VertexBuffer* m_pVB = 0;
    wolf::VertexDeclaration* m_pDecl = 0;
    wolf::Program* m_pProgram = 0;

    Grid2D* m_pGrid = nullptr;
};
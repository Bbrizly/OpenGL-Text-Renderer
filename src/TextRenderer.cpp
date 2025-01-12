#include "TextRenderer.h"
#include "../samplefw/Grid2D.h"

struct Vertex
{
	GLfloat x,y;
	GLubyte r,g,b,a;
};

const unsigned char squareColor[] = { 248, 183, 50, 255 };
const unsigned char gridLinesColor[] = { 110, 110, 110, 255 };

static const Vertex squareVertices[] = {
	{ -0.5f, -0.5f, squareColor[0],   squareColor[1], squareColor[2], squareColor[3] },
	{ -0.5f, 0.5f, squareColor[0],   squareColor[1], squareColor[2], squareColor[3] },
	{ 0.5f, 0.5f, squareColor[0],   squareColor[1], squareColor[2], squareColor[3] },

	{ 0.5f, 0.5f, squareColor[0],   squareColor[1], squareColor[2], squareColor[3] },
	{ 0.5f, -0.5f, squareColor[0],   squareColor[1], squareColor[2], squareColor[3] },
	{ -0.5f, -0.5f, squareColor[0],   squareColor[1], squareColor[2], squareColor[3] },
	
};

static glm::mat4 mWorld;
static void TEST_MATRIX(float p_00, float p_01, float p_02,
				        float p_10, float p_11, float p_12, 
				        float p_20, float p_21, float p_22)
{
	mWorld[0][0] = p_00;
	mWorld[0][1] = p_01;
	mWorld[0][2] = p_02;
	mWorld[0][3] = 0.0f;

	mWorld[1][0] = p_10;
	mWorld[1][1] = p_11;
	mWorld[1][2] = p_12;
	mWorld[1][3] = 0.0f;

	mWorld[2][0] = 0.0f;
	mWorld[2][1] = 0.0f;
	mWorld[2][2] = 1.0f;
	mWorld[2][3] = 0.0f;

	mWorld[3][0] = p_20;
	mWorld[3][1] = p_21;
	mWorld[3][2] = 0.0f;
	mWorld[3][3] = p_22;
}

Font::~Font()
{
	printf("Destroying 2D Sample\n");
	delete m_pGrid;
	delete m_pDecl;
	wolf::BufferManager::DestroyBuffer(m_pVB);
	wolf::ProgramManager::DestroyProgram(m_pProgram);
}

void Font::init()
{
    // Only init if not already done
    if(!m_pProgram)
    {
        m_pProgram = wolf::ProgramManager::CreateProgram("data/2d.vsh", "data/2d.fsh");
        m_pVB = wolf::BufferManager::CreateVertexBuffer(squareVertices, sizeof(Vertex) * 3 * 2);

        m_pDecl = new wolf::VertexDeclaration();
        m_pDecl->Begin();
        m_pDecl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
        m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
        m_pDecl->SetVertexBuffer(m_pVB);
        m_pDecl->End();

        m_pGrid = new Grid2D(30);
    }

    printf("Successfully initialized 2D Sample\n");
}

void Font::update(float dt) 
{
}

void Font::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float fWidth = (float)width;
    float fHeight = (float)height;
    glm::mat4 mProj = glm::ortho(0.0f,fWidth,0.0f,fHeight,0.0f,1000.0f);
    glm::mat4 mView = glm::translate(glm::mat4(1.0f), glm::vec3(fWidth/2.0f,fHeight/2.0f,0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(40.0f,40.0f,40.0f));

    mProj = mProj * mView;

	// CHANGE HERE

	TEST_MATRIX(cos(0.785), -sin(0.785), 0.0f,
				sin(0.785), cos(0.785), 0.0f,
			    3.0f, 1.0f, 1.0f);

	// END CHANGE HERE


    glDepthMask(GL_FALSE);
    m_pGrid->render(glm::mat4(1.0f), mProj);

    m_pProgram->SetUniform("projection", mProj);
	m_pProgram->Bind();
    
    glDepthMask(GL_TRUE);
	m_pDecl->Bind();
    m_pProgram->SetUniform("world", mWorld);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

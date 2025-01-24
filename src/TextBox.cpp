#include "TextBox.h"
#include <sstream>
#include <iostream>
#include <cstdarg>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm; using namespace std;

// ====================== CONSTRUCTOR/DESTRUCTOR =====================

TextBox::TextBox(Font* font,
                 const std::string& text,
                 float width,
                 float height,
                 wolf::Program* shader,
                 TextTable* pTable)
    : m_font(font),
      m_text(text),
      m_width(width),
      m_height(height),
      m_pProgram(shader),
      m_pTextTable(pTable)
{
    m_color = vec4(255,255,255,255);
    GenerateVertices();
}

TextBox::~TextBox() {
    if(m_vertexBuffer) {
        wolf::BufferManager::DestroyBuffer(m_vertexBuffer);
        m_vertexBuffer=nullptr;
    }
    if(m_vertexDecl) {
        delete m_vertexDecl;
        m_vertexDecl=nullptr;
    }
}

#pragma region Setters

void TextBox::SetTextTable(TextTable* pTable) {
    m_pTextTable=pTable;
    GenerateVertices();
}

void TextBox::SetText(const std::string& text) {
    // apply placeholders if we have texttable
    if(m_pTextTable) {
        m_text = m_pTextTable->Substitute(text);
    } else {
        m_text = text;
    }
    GenerateVertices();
}

void TextBox::SetText(const char* fmt, ...) {
    if(!fmt) return;
    char buf[1024];
    va_list args;
    va_start(args,fmt);
    vsnprintf(buf,sizeof(buf),fmt,args);
    va_end(args);
    SetText(std::string(buf));
}

void TextBox::SetPosition(float x, float y) {
    m_position=vec2(x,y);
    GenerateVertices();
}

void TextBox::SetColor(float r,float g,float b,float a){
    m_color=vec4(r,g,b,a);
    GenerateVertices();
}

void TextBox::SetVisualization(bool visualization){
    m_visualization=visualization;
    GenerateVertices();
}

void TextBox::SetAlignment(int alignment){
    // 0=left,1=center,2=right
    m_hAlign=alignment;
    GenerateVertices();
}
void TextBox::SetVerticalAlignment(int vAlign){
    // 0=top,1=middle,2=bottom
    m_vAlign=vAlign;
    GenerateVertices();
}

void TextBox::SetShrinkToFit(bool enable){
    m_shrinkToFit=enable;
    GenerateVertices();
}

#pragma endregion
// ====================== MAIN GENERATE =====================
#pragma region Main Gen

vector<string> TextBox::Truncate(vector<string> lines, float scale)
{
    int size = lines.size();

    int availableLines = (int) (m_height / m_font->GetLineHeight(m_fontIndex) * scale);

    cout<<"-- EQUATION: "<<m_height<<" / "<<m_font->GetLineHeight(m_fontIndex) * scale<<endl;
    cout<<"     = "<< m_height / m_font->GetLineHeight(m_fontIndex) * scale<<endl;

    cout<<"-=-Av Lines"<<availableLines<<endl;
    for(auto x : lines){
        cout<<x<<endl;
    }


    if(size > availableLines)
    {
        lines.erase(lines.end() - (size - availableLines), lines.end());
    }
    
    return lines;
}


void TextBox::GenerateVertices() {
    // clear old CPU data
    m_vertices.clear();

    // bounding box if requested
    if(m_visualization){
        GenerateBoundingBoxVertices();
    }

    // If we do substitution at time of SetText, this might be redundant,
    // but you could do it here as well:
    // m_text = SubstitutePlaceholders(m_text);

    // We'll try scale from 1.0 down to maybe 0.3 in steps, or we just do 50 tries
    // If the text is STILL too big, we do an ellipses
    float bestScale=1.0f;
    bool fitFound=false;
    std::vector<std::string> finalLines;
    bool truncated=false;

    if(!m_text.empty()) {
        if(!m_shrinkToFit) {
            // just do scale=1
            finalLines = WrapAndHyphenate(m_text,1.0f,truncated);
            if(!FitsInBox(finalLines,1.0f)){
                // if it doesn't fit, we set truncated=true -> ellipses
                truncated=true;
                finalLines = Truncate(finalLines, 1.0f);
                // finalLines = WrapAndHyphenate(m_text,1.0f,truncated);
            }
            bestScale=1.0f;
            fitFound=true;
        } else {
            // do multiple tries
            float testScale=1.0f;
            float minScale=0.8f;

            // while(testScale>minScale)
            // {

            for(int i=0; i<50; i++){ 
                // Wrap lines at testScale
                bool tempTruncated=false;
                auto lines = WrapAndHyphenate(m_text,testScale,tempTruncated);

                if(FitsInBox(lines,testScale)){
                    // great, we found lines that fit
                    finalLines=lines;
                    truncated=false; // or tempTruncated if you want to see partial
                    bestScale=testScale;
                    fitFound=true;
                    break;
                }
                // else reduce scale
                testScale*=0.9f;
                if(testScale<minScale){
                    cout<<"+++++ARE WE GOING HERE ASLAN?"<<endl;
                    // can't go smaller
                    // We do one last wrap at min scale & set truncated
                    finalLines=WrapAndHyphenate(m_text,minScale,tempTruncated);
                    bestScale=minScale;
                    truncated=true; // Forced to do this, remove lines that exceed.

                    if(!FitsInBox(finalLines,minScale)){
                        // finalLines = WrapAndHyphenate(m_text,minScale,truncated);                
                        finalLines = Truncate(finalLines, minScale);    
                    }

                    fitFound=true;
                    break;
                }
                

            }

            // for(int i=0; i<50; i++){ 
            //     // Wrap lines at testScale
            //     bool tempTruncated=false;
            //     auto lines = WrapAndHyphenate(m_text,testScale,tempTruncated);
            //     if(FitsInBox(lines,testScale)){
            //         // great, we found lines that fit
            //         finalLines=lines;
            //         truncated=false; // or tempTruncated if you want to see partial
            //         bestScale=testScale;
            //         fitFound=true;
            //         break;
            //     }
            //     // else reduce scale
            //     testScale*=0.9f;
            //     if(testScale<minScale){
            //         // can't go smaller
            //         // We do one last wrap at min scale & set truncated
            //         finalLines=WrapAndHyphenate(m_text,testScale,tempTruncated);
            //         bestScale=testScale;
            //         truncated=true; // forcibly
            //         fitFound=true;
            //         break;
            //     }
            // }
        }
    }

    // for (const auto& line : finalLines) {
    //     cout << line << std::endl;
    // }

    // now build final quads
    if(!finalLines.empty()){
        BuildVerticesFromLines(finalLines,bestScale,truncated);
    }

    // push to GPU
    pushVertexData(m_vertexBuffer,m_vertexDecl,m_vertices);
}

// ====================== STEP 1: Substitution if needed =====================

std::string TextBox::SubstitutePlaceholders(const std::string& raw) const {
    if(!m_pTextTable) return raw;
    return m_pTextTable->Substitute(raw);
}

// ====================== STEP 2: Word-wrap & hyphenate =====================

/*
    This function splits text on spaces, tries to fit each word
    in the line. If word alone is bigger than the box width => break it with a hyphen.

    If we detect we used "lineCount" that might exceed the box height, we can also
    set truncated = true. But we'll rely on the later step "FitsInBox" for that.
*/
std::vector<std::string> TextBox::WrapAndHyphenate(const std::string& text,
                                                   float scale,
                                                   bool& truncated)
{
    // truncated=false; // default
    std::vector<std::string> lines;

    // Split on whitespace:
    std::vector<std::string> tokens;
    {
        std::istringstream iss(text);
        std::string w;
        while(iss >> w){
            tokens.push_back(w);
        }
    }
    if(tokens.empty()){
        lines.push_back("");
        return lines;
    }

    float maxWidth = m_width; // scaled in measure => we do measure scaled inside
    int lineCount=0;

    std::string currentLine;

    for(size_t i=0; i<tokens.size(); i++){
        
        
        
        if(lineCount);


        std::string word = tokens[i];

        // // Thisis a joke
        // // measure the word's width
        // float w=CalculateWordWidth(word)*scale;
        // if(!currentLine.empty()){
        //     // +1 space if we are not at start
        //     w+= (CalculateWordWidth(" ")*scale);
        // }

        // check if it fits in this line
        float currentLineWidth=CalculateWordWidth(currentLine)*scale;
        float singleWordWidth=CalculateWordWidth(word)*scale;

        // + measure for space if needed
        if(!currentLine.empty()){
            currentLineWidth += (CalculateWordWidth(" ")*scale);
        }

        if(currentLineWidth + singleWordWidth <= maxWidth){
            // we can just append
            if(!currentLine.empty()) currentLine+=" ";
            currentLine+=word;

        } else {
            
            //HYPHENATE

            // doesn't fit, so we start a new line
            // if the word itself is bigger than maxWidth?
            // float singleWordWidth=CalculateWordWidth(word)*scale;
            if(singleWordWidth > maxWidth){
                // we break the word with hyphens
                // e.g. "superlongword" => "superl-" then new line "ongword" etc.
                int offset=0;
                string preSubWord;
                string subWord;
                float available = maxWidth - currentLineWidth; 
                // but simpler approach: we just flush currentLine first
                if(!currentLine.empty()){
                    lines.push_back(currentLine);
                    currentLine.clear();
                }
                
                // subWord.push_back(word[offset]);

                while(offset < (int)word.size()){
                    
                    // build subWord char by char until it doesn't fit

                    subWord.push_back(word[offset]);
                    float subW = CalculateWordWidth(subWord)*scale;
                    if(subW > maxWidth){
                        subWord.pop_back(); subWord.pop_back();
                        offset--;

                        subWord+="-";
                        lines.push_back(subWord);
                        subWord.clear();
                    } else {
                        offset++;
                    }
                }

                // leftover
                if(!subWord.empty()){
                    lines.push_back(subWord);
                    subWord.clear();
                }
            } else {
                // just start new line
                if(!currentLine.empty()){
                    lines.push_back(currentLine);
                }
                currentLine = word;
            }
        }
    }
    // push the last line
    if(!currentLine.empty()){
        lines.push_back(currentLine);
    }
    // if(truncated)
    // {
    //     int size = lines.size();

    //     int availableLines = m_height / m_font->GetLineHeight() * scale;
    //     if(size > availableLines)
    //     {
    //         lines.erase(lines.end() - (size - availableLines), lines.end());
    //     }
    // }


    return lines;
}

// ====================== STEP 3: measure lines to see if they fit =====================

bool TextBox::FitsInBox(const std::vector<std::string>& lines,
                        float scale)
{
    // measure total height
    float totalH = lines.size()* m_font->GetLineHeight(m_fontIndex) * scale;

    //Where to stop?
    

    if(totalH> m_height) return false;

    // measure max line width
    float maxW=0.f;
    for(auto& line : lines){
        float w= CalculateWordWidth(line)* scale;
        if(w>maxW) maxW=w;
        if(maxW> m_width) return false;
    }
    return true;
}

#pragma endregion

#pragma region Finale

// ====================== Build final quads =====================
void TextBox::BuildVerticesFromLines(const std::vector<std::string>& lines,
                                     float scale,
                                     bool truncated)
{
    // If truncated is true, we might need to replace the last line's end with "..."
    // if there's more text than we can show
    // for simplicity, let's do it if truncated:
    if(truncated && !lines.empty()){
        // replace last line's end with "..."
        // or if last line is super short, we just put "..."
        // e.g. we remove 3 chars from the last line, add ...
        std::string lastLine= lines.back();
        if(lastLine.size()>3){
            // remove 3 chars
            lastLine.resize(lastLine.size()-3);
        }
        lastLine+="...";
        // pop & replace
        auto copy=lines;
        copy.back()=lastLine;
        BuildVerticesActual(copy,scale);
    } else {
        BuildVerticesActual(lines,scale);
    }
}

void TextBox::BuildVerticesActual(const std::vector<std::string>& lines,float scale)
{
    float totalTextHeight= lines.size()* m_font->GetLineHeight(m_fontIndex)* scale;

    float cursorY=m_position.y; // top
    // handle vertical alignment
    // 0=top,1=middle,2=bottom
    if(m_vAlign==1){
        // middle => subtract half leftover
        cursorY= m_position.y - ((m_height- totalTextHeight)* 0.5f);
    } else if(m_vAlign==2){
        cursorY= m_position.y - (m_height - totalTextHeight);
    }

    float texW= m_font->GetTexture()->GetWidth();
    float texH= m_font->GetTexture()->GetHeight();

    for(auto& line : lines){
        float lineW= CalculateWordWidth(line)*scale;
        // alignment
        vec2 cursor= CalculateAlignmentCursor(line,lineW,cursorY,scale);

        for(size_t i=0; i< line.size(); i++){
            char c= line[i];
            if(i>0){
                char prevC=line[i-1];
                ApplyKerning(prevC,c,scale,cursor.x);
            }
            const CharInfo& ch= m_font->GetCharacter(c, m_fontIndex);

            GenerateCharacterVertices(ch,cursor.x,cursor.y,texW,texH,scale);
            cursor.x += (ch.xAdvance*scale);
        }

        cursorY -= (m_font->GetLineHeight(m_fontIndex)* scale);
    }
}

void TextBox::GenerateCharacterVertices(const CharInfo& ch,float x,float y,float texW,float texH,float scale)
{
    float xpos= x + ch.xOffset* scale;
    float ypos= y - ch.yOffset* scale;
    float w= ch.uEnd* scale;
    float h= ch.vEnd* scale;

    float u0= ch.uStart/ texW;
    float v0= ch.vStart/ texH;
    float u1= (ch.uStart + ch.uEnd)/ texW;
    float v1= (ch.vStart + ch.vEnd)/ texH;

    GLubyte R= (GLubyte)m_color.x;
    GLubyte G= (GLubyte)m_color.y;
    GLubyte B= (GLubyte)m_color.z;
    GLubyte A= (GLubyte)m_color.w;

    float page= (float)ch.page;

    // 2 triangles
    Vertex V1= {xpos,    ypos- h, 0.0f, R,G,B,A,  u0,v1, page};
    Vertex V2= {xpos+ w, ypos- h, 0.0f, R,G,B,A,  u1,v1, page};
    Vertex V3= {xpos,    ypos,    0.0f, R,G,B,A,  u0,v0, page};
    Vertex V4= {xpos+ w, ypos,    0.0f, R,G,B,A,  u1,v0, page};

    m_vertices.push_back(V1);
    m_vertices.push_back(V2);
    m_vertices.push_back(V3);

    m_vertices.push_back(V2);
    m_vertices.push_back(V4);
    m_vertices.push_back(V3);
}

#pragma endregion

#pragma region Helper Functions

// ====================== HELPER for alignment cursor =====================
vec2 TextBox::CalculateAlignmentCursor(const std::string& line,
                                       float lineWidth,
                                       float cursorY,
                                       float scale)
{
    // we just scale for lineWidth externally
    switch(m_hAlign){
        case 1: // center
            return vec2(m_position.x+ (m_width- lineWidth)* 0.5f, cursorY);
        case 2: // right
            return vec2(m_position.x+ (m_width- lineWidth), cursorY);
        default:// left
            return vec2(m_position.x, cursorY);
    }
}

// ====================== KERNING =====================
void TextBox::ApplyKerning(char prevChar, char c, float scale, float& xCursor){
    auto it= m_font->GetKerning(m_fontIndex).find({prevChar,c});
    if(it!= m_font->GetKerning(m_fontIndex).end()){
        xCursor+= (it->second* scale);
    }
}

// ====================== Basic measure =====================
float TextBox::CalculateWordWidth(const std::string& word){
    float width=0.f;
    for(size_t i=0; i< word.size(); i++){
        char c= word[i];
        const CharInfo& ch= m_font->GetCharacter(c, m_fontIndex);
        // kerning if i>0
        if(i>0){
            char prev= word[i-1];
            auto kit= m_font->GetKerning(m_fontIndex).find({prev,c});
            if(kit!= m_font->GetKerning(m_fontIndex).end()){
                width+= kit->second;
            }
        }
        width+= ch.xAdvance;
    }
    return width;
}
#pragma endregion
// ====================== BOUNDING BOX =====================
void TextBox::GenerateBoundingBoxVertices(){
    // optional
    float layer=-1.f;
    GLubyte R=255,G=255,B=255,A=255;

    float left= m_position.x;
    float top= m_position.y;
    float right= m_position.x+ m_width;
    float bottom= m_position.y- m_height;

    Vertex vA= {left, bottom,0.f, R,G,B,A, 0.f,1.f, layer};
    Vertex vB= {right,bottom,0.f, R,G,B,A,1.f,1.f, layer};
    Vertex vC= {left, top,   0.f, R,G,B,A, 0.f,0.f, layer};
    Vertex vD= {right,top,   0.f, R,G,B,A, 1.f,0.f, layer};

    m_vertices.push_back(vA);
    m_vertices.push_back(vB);
    m_vertices.push_back(vC);

    m_vertices.push_back(vB);
    m_vertices.push_back(vD);
    m_vertices.push_back(vC);
}

// ====================== GPU UPLOAD & RENDER =====================
void TextBox::pushVertexData(wolf::VertexBuffer*& vBuffer,
                             wolf::VertexDeclaration*& vDecl,
                             const std::vector<Vertex>& inVerts)
{
    if(vBuffer) {
        wolf::BufferManager::DestroyBuffer(vBuffer);
        vBuffer=nullptr;
    }
    if(vDecl){
        delete vDecl;
        vDecl=nullptr;
    }
    if(inVerts.empty()){
        m_numVertices=0;
        return;
    }
    vBuffer= wolf::BufferManager::CreateVertexBuffer(inVerts.data(), inVerts.size()*sizeof(Vertex));
    vDecl= new wolf::VertexDeclaration();
    vDecl->Begin();
    vDecl->AppendAttribute(wolf::AT_Position,3, wolf::CT_Float);
    vDecl->AppendAttribute(wolf::AT_Color,4, wolf::CT_UByte);
    vDecl->AppendAttribute(wolf::AT_TexCoord1,2, wolf::CT_Float);
    vDecl->AppendAttribute(wolf::AT_TexCoord2,1, wolf::CT_Float);
    vDecl->SetVertexBuffer(vBuffer);
    vDecl->End();
    m_numVertices=inVerts.size();
}


void TextBox::Render(const glm::mat4& proj, const glm::mat4& view){
    if(!m_pProgram || !m_vertexBuffer || !m_vertexDecl || m_numVertices==0) return;

    m_pProgram->Bind();
    m_pProgram->SetUniform("projection",proj);
    m_pProgram->SetUniform("view",view);

    m_font->GetTexture()->Bind(0);
    m_pProgram->SetUniform("u_texture",0);

    m_vertexDecl->Bind();
    glDrawArrays(GL_TRIANGLES,0,m_numVertices);
}

Font* TextBox::GetFont() const {
    return m_font;
}
const std::vector<Vertex>& TextBox::GetVertices() const{
    return m_vertices;
}

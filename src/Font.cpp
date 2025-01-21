#include "Font.h"

using namespace std;

Font::Font(const string& fontPath) {
    LoadFont(fontPath);
}

Font::~Font() {
    if (m_texture)
        wolf::TextureManager::DestroyTexture(m_texture);
        m_texture = nullptr;
}

void Font::ArrayTextureOfAllFiles(string filename, int totalPages)
{
    vector<string> files;
    cout<<"AAAAAAAAAAAAA"<<filename<<endl;

    size_t lastDot = filename.find_last_of('.'); //remove .fnt
    if (lastDot == string::npos) {
        cerr << "Invalid filename: " << filename << endl;
        return;
    }
    
    string baseName = filename.substr(0, lastDot); // Get "data/English_Alphabet"

    for (int i = 0; i <= totalPages-1; ++i) {
        std::string textureFile = baseName + "_" + to_string(i) + ".tga";
        cout<<"Looking for: "<<textureFile<<endl;
        files.push_back(textureFile);
    }

    m_texture = wolf::TextureManager::CreateAutoArrayTexture(files);
}

void Font::LoadFont(const string& fontPath){//, const vector<string>& texturePaths) {
    ifstream file(fontPath);

    if (!file.is_open()) {
        throw runtime_error("Failed to open font file: " + fontPath);
    }

    string line;
    while (std::getline(file, line)) {
        if (line.rfind("info", 0) == 0) {
            // I think I need to use the info in info??
        } else if (line.rfind("common", 0) == 0) {
            std::istringstream iss(line);
            std::string key;
            while (iss >> key) {
                if (key.find("lineHeight=") == 0) {
                    m_lineHeight = std::stoi(key.substr(11));
                } else if (key.find("scaleW=") == 0) {
                    m_scaleW = std::stoi(key.substr(7));
                } else if (key.find("scaleH=") == 0) {
                    m_scaleH = std::stoi(key.substr(7));
                } else if (key.find("pages=") == 0) { // Extract the number of pages
                    int totalPages = std::stoi(key.substr(6));
                    std::cout << "Total pages: " << totalPages << std::endl;
                    ArrayTextureOfAllFiles(fontPath, totalPages);
                }
            }
        } else if (line.rfind("page", 0) == 0) {
            size_t filePos = line.find("file=");
            // cout<<"AA: "<<line.rfind("file", 0)<<endl;
            if (filePos != std::string::npos) {
                std::string textureFile = line.substr(filePos + 6); // sskip "file=\""
                textureFile.pop_back(); // Remove the behind shit 
                
                // std::cout << "Loading texture: " << textureFile << std::endl;
                // m_texture = wolf::TextureManager::CreateTexture("data/"+textureFile);
            }
        } else if (line.rfind("char", 0) == 0) {
            // Parse char tag
            std::istringstream iss(line);
            CharInfo ch;
            int id = 0;
            std::string key;
            while (iss >> key) {
                if (key.find("id=") == 0) {
                    id = std::stoi(key.substr(3));
                } else if (key.find("x=") == 0) {
                    ch.uStart = std::stoi(key.substr(2));
                } else if (key.find("y=") == 0) {
                    ch.vStart = std::stoi(key.substr(2));
                } else if (key.find("width=") == 0) {
                    ch.uEnd = std::stoi(key.substr(6));
                } else if (key.find("height=") == 0) {
                    ch.vEnd = std::stoi(key.substr(7));
                } else if (key.find("xoffset=") == 0) {
                    ch.xOffset = std::stoi(key.substr(8));
                } else if (key.find("yoffset=") == 0) {
                    ch.yOffset = std::stoi(key.substr(8));
                } else if (key.find("xadvance=") == 0) {
                    ch.xAdvance = std::stoi(key.substr(9));
                } else if (key.find("page=") == 0) {
                    ch.page = std::stoi(key.substr(5));
                }
            }

            if(ch.uStart < 0){cout<<"AHA"<<endl; continue;} //shit is fucking dumb

            //Debugg
            // cout << "\nChar ID: " << id << "\n"
            //     << " uStart: " << ch.uStart << "\n"
            //     << " vStart: " << ch.vStart << "\n"
            //     << " uEnd: " << ch.uEnd << "\n"
            //     << " vEnd: " << ch.vEnd << "\n"
            //     << " xOffset: " << ch.xOffset << "\n"
            //     << " yOffset: " << ch.yOffset << "\n"
            //     << " xAdvance: " << ch.xAdvance << "\n";

            // Add character to the map
            m_characters[id] = ch;
            
        //more kerning shit that doesnt work
        } else if (line.rfind("kerning", 0) == 0) {
            //  If they kern then remove the amount of pixels from .fnt 
            int first, second, amount;
            sscanf(line.c_str(), "kerning first=%d second=%d amount=%d", &first, &second, &amount);
            m_kerning[std::make_pair(first, second)] = amount;
        }
    }


    file.close();
    if (m_characters.find('?') == m_characters.end()) {
        throw std::runtime_error("Fallback character '?' is missing in the font map.");
    }

}

const int Font::GetLineHeight(){
    return m_lineHeight;
}

const CharInfo& Font::GetCharacter(char c) const {
    auto it = m_characters.find(c);
    if (it == m_characters.end()) {
        throw std::runtime_error("Character not found: " + string(1, c));
    }
    return it->second;
}

wolf::Texture* Font::GetTexture() const {
    // cout<<"Getting texture: "<<m_texture->GetHeight()<<endl;
    return m_texture;
}

const unordered_map<std::pair<char, char>, int, PairHash>& Font::GetKerning() const {
    return m_kerning;
}

// const unordered_map<std::pair<char, char>, int>& Font::GetKerning() const {
//     return m_kerning;
// }


/* ADD THISSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS

//----------------------------------------------------------------------------- // File: Font.cpp // Updates to support array textures for multi-page font rendering
#include "Font.h"
#include "W_TextureManager.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>

Font::Font(const std::string& fontPath, const std::vector<std::string>& texturePaths) {
    std::cout << "Loading font from " << fontPath << std::endl;
    LoadFont(fontPath, texturePaths);
}

Font::~Font() {
    if (m_textureArray) {
        wolf::TextureManager::DestroyTexture(m_textureArray);
        m_textureArray = nullptr;
    }
}

void Font::LoadFont(const std::string& fontPath, const std::vector<std::string>& texturePaths) {
    // Load array texture
    m_textureArray = wolf::TextureManager::CreateAutoArrayTexture(texturePaths);
    if (!m_textureArray) {
        throw std::runtime_error("Failed to create array texture.");
    }

    std::ifstream file(fontPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open font file: " + fontPath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("info", 0) == 0) {
            // Parse info section if needed
        } else if (line.rfind("common", 0) == 0) {
            // Parse common section
            std::istringstream iss(line);
            std::string key;
            while (iss >> key) {
                if (key.find("lineHeight=") == 0) {
                    m_lineHeight = std::stoi(key.substr(11));
                } else if (key.find("scaleW=") == 0) {
                    m_scaleW = std::stoi(key.substr(7));
                } else if (key.find("scaleH=") == 0) {
                    m_scaleH = std::stoi(key.substr(7));
                }
            }
        } else if (line.rfind("char", 0) == 0) {
            // Parse char data
            std::istringstream iss(line);
            CharInfo ch;
            int id = 0;
            std::string key;
            while (iss >> key) {
                if (key.find("id=") == 0) {
                    id = std::stoi(key.substr(3));
                } else if (key.find("x=") == 0) {
                    ch.uStart = std::stoi(key.substr(2));
                } else if (key.find("y=") == 0) {
                    ch.vStart = std::stoi(key.substr(2));
                } else if (key.find("width=") == 0) {
                    ch.uEnd = std::stoi(key.substr(6));
                } else if (key.find("height=") == 0) {
                    ch.vEnd = std::stoi(key.substr(7));
                } else if (key.find("xoffset=") == 0) {
                    ch.xOffset = std::stoi(key.substr(8));
                } else if (key.find("yoffset=") == 0) {
                    ch.yOffset = std::stoi(key.substr(8));
                } else if (key.find("xadvance=") == 0) {
                    ch.xAdvance = std::stoi(key.substr(9));
                } else if (key.find("page=") == 0) {
                    ch.page = std::stoi(key.substr(5));
                }
            }

            m_characters[id] = ch;
        } else if (line.rfind("kerning", 0) == 0) {
            // Parse kerning data
            int first, second, amount;
            sscanf(line.c_str(), "kerning first=%d second=%d amount=%d", &first, &second, &amount);
            m_kerning[{first, second}] = amount;
        }
    }

    file.close();

    if (m_characters.find('?') == m_characters.end()) {
        throw std::runtime_error("Fallback character '?' is missing in the font map.");
    }
}

const CharInfo& Font::GetCharacter(char c) const {
    auto it = m_characters.find(c);
    if (it == m_characters.end()) {
        throw std::runtime_error("Character not found: " + std::string(1, c));
    }
    return it->second;
}

wolf::Texture* Font::GetTextureArray() const {
    return m_textureArray;
}

// Modify rendering to use array textures and layer indices
void TextBox::GenerateCharacterVertices(const CharInfo& ch, vec2 cursor, float textureWidth, float textureHeight) {
    float xpos = cursor.x + ch.xOffset;
    float ypos = cursor.y - ch.yOffset;
    float w = ch.uEnd;
    float h = ch.vEnd;

    float uStart = ch.uStart / textureWidth;
    float uEnd = (ch.uStart + w) / textureWidth;
    float vStart = ch.vStart / textureHeight;
    float vEnd = (ch.vStart + h) / textureHeight;

    // Include layer index (page) in UV or shader
    Vertex v1 = {xpos, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vEnd, ch.page};
    Vertex v2 = {xpos + w, ypos - h, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vEnd, ch.page};
    Vertex v3 = {xpos, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uStart, vStart, ch.page};
    Vertex v4 = {xpos + w, ypos, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w, uEnd, vStart, ch.page};

    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);

    m_vertices.push_back(v2);
    m_vertices.push_back(v4);
    m_vertices.push_back(v3);
}

*/
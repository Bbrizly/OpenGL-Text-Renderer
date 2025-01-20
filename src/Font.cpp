#include "Font.h"

using namespace std;

Font::Font(const string& texturePath, const string& fontPath) {
    cout << "Loading texture: " << texturePath << endl;
    m_texture = wolf::TextureManager::CreateTexture(texturePath);
    LoadFont(fontPath);
}

Font::~Font() {
    if (m_texture)
        wolf::TextureManager::DestroyTexture(m_texture);
        m_texture = nullptr;
}

void Font::LoadFont(const string& fontPath) {
    std::ifstream file(fontPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open font file: " + fontPath);
    }

    std::string line;
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
                }
            }
        } else if (line.rfind("page", 0) == 0) {
            size_t filePos = line.find("file=");
            // cout<<"AA: "<<line.rfind("file", 0)<<endl;
            if (filePos != std::string::npos) {
                std::string textureFile = line.substr(filePos + 6); // sskip "file=\""
                textureFile.pop_back(); // Remove the behind shit 
                
                std::cout << "Loading texture: " << textureFile << std::endl;
                m_texture = wolf::TextureManager::CreateTexture("data/"+textureFile);
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

//     for (const auto& pair : m_kerning) {
//     std::cout << "Kerning: (" << pair.first.first << ", " << pair.first.second 
//               << ") -> " << pair.second << std::endl;
//     }

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
#include "MiniFont.h"

using namespace std;

MiniFont::MiniFont(const string& fontPath, int pageOffset) {
    m_pageOffset = pageOffset;
    LoadFont(fontPath);
}

MiniFont::MiniFont() 
    : m_lineHeight(0), m_scaleW(0), m_scaleH(0), m_pages(1), m_arrayTexture(nullptr) {
    // Initialize members with default values
}

MiniFont::~MiniFont() {
    cout<<"DESTRUCTOR"<<endl;
    if (m_arrayTexture)
        wolf::TextureManager::DestroyTexture(m_arrayTexture);
        m_arrayTexture = nullptr;
}

void MiniFont::ArrayTextureOfAllFiles(string filename)//, int totalPages)
{
    vector<string> files;
    cout<<"open font file: " + filename<<endl;

    size_t lastDot = filename.find_last_of('.'); //remove .fnt
    if (lastDot == string::npos) {
        cerr << "Invalid filename: " << filename << endl;
        return;
    }
    
    string baseName = filename.substr(0, lastDot); // Get "data/English_Alphabet"

    for (int i = 0; i <= m_pages-1; ++i) {
        string textureFile = baseName + "_" + to_string(i) + ".tga";
        cout<<"Looking for: "<<textureFile<<endl;
        files.push_back(textureFile);
    }

    m_arrayTexture = wolf::TextureManager::CreateAutoArrayTexture(files);
    cout<<"Im here"<<endl;
}

void MiniFont::LoadFont(const string& fontPath){//, const vector<string>& texturePaths) {
    ifstream file(fontPath);

    if (!file.is_open()) {
        throw runtime_error("Failed to open font file: " + fontPath);
    }

    string line;
    m_pages = 1;
    while (getline(file, line)) {
        if (line.rfind("info", 0) == 0) {
            // I think I need to use the info in info??
        } else if (line.rfind("common", 0) == 0) {
            istringstream iss(line);
            string key;
            while (iss >> key) {
                if (key.find("lineHeight=") == 0) {
                    m_lineHeight = stoi(key.substr(11));
                } else if (key.find("scaleW=") == 0) {
                    m_scaleW = stoi(key.substr(7));
                } else if (key.find("scaleH=") == 0) {
                    m_scaleH = stoi(key.substr(7));
                } else if (key.find("pages=") == 0) { // Extract the number of pages
                    m_pages = stoi(key.substr(6));
                    cout << "Total pages: " << m_pages << endl;
                }
            }
        } else if (line.rfind("page", 0) == 0) {
            size_t filePos = line.find("file=");
            // cout<<"AA: "<<line.rfind("file", 0)<<endl;
            if (filePos != string::npos) {
                string textureFile = line.substr(filePos + 6); // sskip "file=\""
                textureFile.pop_back(); // Remove the behind shit 
                
                // cout << "Loading texture: " << textureFile << endl;
                // m_texture = wolf::TextureManager::CreateTexture("data/"+textureFile);
            }
        } else if (line.rfind("char", 0) == 0) {
            // Parse char tag
            istringstream iss(line);
            CharInfo ch;
            int id = 0;
            string key;
            while (iss >> key) {
                if (key.find("id=") == 0) {
                    id = stoi(key.substr(3));
                } else if (key.find("x=") == 0) {
                    ch.uStart = stoi(key.substr(2));
                } else if (key.find("y=") == 0) {
                    ch.vStart = stoi(key.substr(2));
                } else if (key.find("width=") == 0) {
                    ch.uEnd = stoi(key.substr(6));
                } else if (key.find("height=") == 0) {
                    ch.vEnd = stoi(key.substr(7));
                } else if (key.find("xoffset=") == 0) {
                    ch.xOffset = stoi(key.substr(8));
                } else if (key.find("yoffset=") == 0) {
                    ch.yOffset = stoi(key.substr(8));
                } else if (key.find("xadvance=") == 0) {
                    ch.xAdvance = stoi(key.substr(9));
                } else if (key.find("page=") == 0) {
                    ch.page = stoi(key.substr(5)) + m_pageOffset; //FOR ARRAY TEXTURE YAYAYAYA
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
            m_kerning[make_pair(first, second)] = amount;
        }
    }


    file.close();
    
    // ArrayTextureOfAllFiles(fontPath);//, m_pages);  NOW DOING IN FONT CLASS :D

    if (m_characters.find('?') == m_characters.end()) {
        throw runtime_error("Fallback character '?' is missing in the font map.");
    }

}

const int MiniFont::GetLineHeight(){
    return m_lineHeight;
}
const int MiniFont::GetPages(){
    return m_pages;
}

const CharInfo& MiniFont::GetCharacter(char c) const {
    auto it = m_characters.find(c);
    if (it == m_characters.end()) {
        throw runtime_error("Character not found: " + string(1, c));
    }
    return it->second;
}

wolf::Texture* MiniFont::GetTexture() const {
    return m_arrayTexture;
}

const unordered_map<pair<char, char>, int, PairHash>& MiniFont::GetKerning() const {
    return m_kerning;
}

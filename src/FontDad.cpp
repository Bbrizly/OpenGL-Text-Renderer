#include "FontDad.h"

using namespace std;

FontDad::FontDad(const string& fontPath) {}

FontDad::~FontDad() {
    cout<<"DESTRUCTOR"<<endl;
    if (m_arrayTexture)
        wolf::TextureManager::DestroyTexture(m_arrayTexture);
        m_arrayTexture = nullptr;
}

// void FontDad::ArrayTextureOfAllFiles(string filename, int totalPages)
// {
//     vector<string> files;
//     cout<<"open font file: " + filename<<endl;

//     size_t lastDot = filename.find_last_of('.'); //remove .fnt
//     if (lastDot == string::npos) {
//         cerr << "Invalid filename: " << filename << endl;
//         return;
//     }
    
//     string baseName = filename.substr(0, lastDot); // Get "data/English_Alphabet"

//     for (int i = 0; i <= totalPages-1; ++i) {
//         std::string textureFile = baseName + "_" + to_string(i) + ".tga";
//         cout<<"Looking for: "<<textureFile<<endl;
//         files.push_back(textureFile);
//     }

//     m_arrayTexture = wolf::TextureManager::CreateAutoArrayTexture(files);
//     cout<<"Im here"<<endl;
// }

const int FontDad::GetLineHeight(){
    return m_lineHeight;
}
wolf::Texture* FontDad::GetTexture() const {
    return m_arrayTexture;
}


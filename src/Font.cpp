#include "Font.h"

using namespace std;

Font::Font(const string& fontPath) 
{
    LoadAll(fontPath);
}

Font::~Font() {
    cout<<"DESTRUCTOR"<<endl;
    if (m_arrayTexture)
        wolf::TextureManager::DestroyTexture(m_arrayTexture);
        m_arrayTexture = nullptr;
}

// void Font::ArrayTextureOfAllFiles(string filename, int totalPages)
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
//         string textureFile = baseName + "_" + to_string(i) + ".tga";
//         cout<<"Looking for: "<<textureFile<<endl;
//         files.push_back(textureFile);
//     }
//     m_arrayTexture = wolf::TextureManager::CreateAutoArrayTexture(files);
//     cout<<"Im here"<<endl;
// }

void Font::LoadAll(const string& fileName)
{
    cout << "[Font::LoadAll] Base .fnt: " << fileName << endl;

    string baseName = "data/Fonts/" + fileName;

    vector<string> variationBaseNames;
    int pageOffset = 0;
    int count = 0;

    while(true) {
        // "data/test0.fnt"
        stringstream ss;
        ss << baseName << count << ".fnt";
        string dataFile = ss.str();

        ifstream file(dataFile);
        if(!file.is_open()) {
            // no more variations
            break;
        }
        file.close();

        MiniFont* mini = new MiniFont(dataFile, pageOffset);
        if(!mini) break;

        pageOffset += mini->GetPages();

        m_fonts.push_back(mini);

        // store e.g. "data/test0" for array textures
        stringstream ss2;
        ss2 << baseName << count;
        variationBaseNames.push_back(ss2.str());

        count++;
    }

    // now we create one big array texture for all pages
    ArrayTextureOfAllFiles(variationBaseNames);

    cout << "[Font::LoadAll] # of mini-fonts loaded: " << m_fonts.size() << endl;
}

void Font::ArrayTextureOfAllFiles(const vector<string>& listOfFontSizes)
{
    cout << "[Font::ArrayTextureOfAllFiles] building array texture..." << endl;
    vector<string> files;

    // for each mini-font variation i
    for (int i=0; i < (int)listOfFontSizes.size(); i++){
        int pages = m_fonts[i]->GetPages();
        for(int j=0; j < pages; j++){
            // e.g. "data/test0_0.tga"
            string textureFile = listOfFontSizes[i] + "_" + to_string(j) + ".tga";
            cout << "  Looking for: " << textureFile << endl;
            files.push_back(textureFile);
        }
    }

    // now create the big array texture
    m_arrayTexture = wolf::TextureManager::CreateAutoArrayTexture(files);
    cout << "[Font::ArrayTextureOfAllFiles] Done creating array texture.\n";
}

const CharInfo& Font::GetCharacter(char c, int index) const{
    
    return m_fonts[index]->GetCharacter(c);
}
const int Font::GetLineHeight(int index){
    return m_fonts[index]->GetLineHeight();
}
const unordered_map<pair<char, char>, int, PairHash>& Font::GetKerning(int index) const{
    return m_fonts[index]->GetKerning();
}

const vector<MiniFont*>& Font::GetFonts() const {
    return m_fonts;
}

wolf::Texture* Font::GetTexture() const {
    return m_arrayTexture;
}


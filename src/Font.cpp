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
//         std::string textureFile = baseName + "_" + to_string(i) + ".tga";
//         cout<<"Looking for: "<<textureFile<<endl;
//         files.push_back(textureFile);
//     }
//     m_arrayTexture = wolf::TextureManager::CreateAutoArrayTexture(files);
//     cout<<"Im here"<<endl;
// }

void Font::ArrayTextureOfAllFiles(vector<string> listOfFontSizes)//, int totalPages)
{
    vector<string> files;
    for(auto x : listOfFontSizes)
    {
        cout<<"WHAT: "<<x<<endl;
    }

    for(int i = 0; i < listOfFontSizes.size(); i++)
    {
        int pages = m_fonts[i]->GetPages();

        for (int i = 0; i <= pages-1; ++i) {
            string textureFile = listOfFontSizes[i] + "_" + to_string(i) + ".tga";
            cout<<"Looking for: "<<textureFile<<endl;
            files.push_back(textureFile);
        }
    }
    m_arrayTexture = wolf::TextureManager::CreateAutoArrayTexture(files);
}

void Font::LoadAll(const std::string& fileName) {
    cout<<"++BASENAME:     "<<fileName<<endl;

    vector<string> baseNames;

    size_t lastDot = fileName.find_last_of('.'); //remove .fnt
    if (lastDot == string::npos) {
        cerr << "Invalid filename: " << fileName << endl;
        return;
    }
    
    string baseName = fileName.substr(0, lastDot); // Get "data/English_Alphabet"

    cout<<"++BASENAME:     "<<baseName<<endl;

    // baseName e.g. "data/test"
    // we load test0.fnt, test1.fnt, test2.fnt,... up to count-1
    // assume 0 is largest -> 1 is smaller -> 2 is smallest
    // or reverse if you want

    int count = 0;
    int pageOffset = 0;
    // for(int i=0; i<count; i++){
    while(true){
        // e.g. "data/test0.fnt"
        std::stringstream ss;
        ss << baseName << count << ".fnt";
        // string dataFile = ss.str();
        string dataFile = baseName + to_string(count) + ".fnt";
        
        ifstream file(dataFile);

        if (!file.is_open()) {
            cout<<"File is empty: "<<dataFile<<endl;
            break;
        }
        file.close();

        MiniFont* f = new MiniFont(dataFile, pageOffset);
        baseNames.push_back(baseName);

        if(f == nullptr)    break;

        pageOffset += f->GetPages(); //Might have to add 1 cuz it starts at 0 RECHECKKKKKKKKKKKKKKKKKKKKK
        
        m_fonts.push_back(f);
        count++;
    }

    ArrayTextureOfAllFiles(baseNames);

    cout<<"=-=-LOAD ALL DONE"<<endl;

    // Now m_fonts[0] is presumably largest, m_fonts[1] is smaller
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

const std::vector<MiniFont*>& Font::GetFonts() const {
    return m_fonts;
}

// const int Font::GetLineHeight(){
//     return m_lineHeight;
// }
wolf::Texture* Font::GetTexture() const {
    return m_arrayTexture;
}


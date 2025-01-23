#include "TextTable.h"

using namespace std;

TextTable::TextTable(string path){ LoadCSV(path);}
TextTable::~TextTable(){}

void TextTable::LoadCSV(const string& filePath) {
    ifstream file(filePath);
    if(!file.is_open()) {
        throw runtime_error("Failed to open CSV file: " + filePath);
    }

    // First line = headers. e.g. "ID,English,Spanish"
    string headerLine;
    if(!getline(file, headerLine)) {
        throw runtime_error("CSV file is empty or invalid: " + filePath);
    }

    // Split the header
    vector<string> headers;
    {
        stringstream ss(headerLine);
        string col;
        while(getline(ss, col, ',')) {
            headers.push_back(col);
        }
    }
    if(headers.size() < 2) {
        throw runtime_error("CSV must have at least 2 columns: ID and at least 1 language!");
    }

    string row;
    while(getline(file,row)) {
        if(row.empty()) continue;

        vector<string> columns;
        {
            stringstream ss(row);
            string c;
            while(getline(ss, c, ',')) {
                columns.push_back(c);
            }
        }
        // Must have same # columns as header line, or we skip
        if(columns.size() < headers.size()) {
            cerr << "Skipping CSV row with fewer columns than header: " << row << endl;
            continue;
        }

        // The first column is the "ID" (e.g. "str_greeting")
        string id = columns[0];

        for(size_t i=1; i<headers.size(); i++) {
            string language = headers[i];
            string textVal  = columns[i];
            m_strings[language][id] = textVal;
        }
    }

    file.close();
}

void TextTable::SetLanguage(const string& language) {
    // Check if we have an entry for that language
    if(m_strings.find(language)==m_strings.end()) {
        throw runtime_error("Language not found: " + language);
    }
    cout<<"BITCH"<<endl;
    m_currentLanguage=language;
}

string TextTable::GetString(const string& id) const {
    // see if currentLanguage is set
    auto langIt = m_strings.find(m_currentLanguage);
    if(langIt==m_strings.end()) {
        throw runtime_error("TextTable: no current language set or invalid language: " + m_currentLanguage);
    }
    // find the string
    auto strIt = langIt->second.find(id);
    if(strIt==langIt->second.end()) {
        throw runtime_error("TextTable: no string for ID: " + id);
    }

    // do dynamic substitution
    string out = strIt->second; 
    out = Substitute(out);
    // for(const auto& kv : m_substitutions) {
    //     // e.g. kv.first="playerName" => kv.second="kingkong"
    //     string placeholder = "{" + kv.first + "}";
    //     size_t pos=0;
    //     while((pos=out.find(placeholder,pos)) != string::npos) {
    //         out.replace(pos, placeholder.size(), kv.second);
    //         pos += kv.second.size();
    //     }
    // }
    return out;
}

string TextTable::Substitute(const string& input) const {
    string out = input;
    cout<<"\n\nWTFFFF\n\n";
    for(const auto& kv : m_substitutions) {
        string placeholder = "{" + kv.first + "}";
        cout<<placeholder<<endl;
        size_t pos=0;
        while((pos=out.find(placeholder,pos))!=string::npos) {
            out.replace(pos, placeholder.size(), kv.second);
            pos += kv.second.size();
        }
    }
    
    cout<<"\n\n\n";
    return out;
}

void TextTable::SetStringProperty(const string& key, const string& value) {
    m_substitutions[key] = value;
}

#pragma once
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class TextTable {
public:
    TextTable(string path);
    ~TextTable();


    void SetLanguage(const string& language);

    string GetString(const string& id) const;

    void SetStringProperty(const string& key, const string& value);
    
    string Substitute(const string& input) const;

private:

    void LoadCSV(const string& filePath);

    unordered_map<string, unordered_map<string, string>> m_strings;

    unordered_map<string, string> m_substitutions;

    string m_currentLanguage;
};

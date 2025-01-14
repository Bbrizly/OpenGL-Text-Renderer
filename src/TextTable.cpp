#include "TextTable.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

TextTable::TextTable() {}

TextTable::~TextTable() {}

void TextTable::Load(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open text file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string language, id, value;

        std::getline(iss, language, '\t');
        std::getline(iss, id, '\t');
        std::getline(iss, value, '\t');

        m_strings[language][id] = value;
    }

    file.close();
}

void TextTable::SetLanguage(const std::string& language) {
    if (m_strings.find(language) == m_strings.end()) {
        throw std::runtime_error("Language not found: " + language);
    }
    m_currentLanguage = language;
}

std::string TextTable::GetString(const std::string& id) const {
    auto langIt = m_strings.find(m_currentLanguage);
    if (langIt == m_strings.end()) {
        throw std::runtime_error("Current language not set or not found.");
    }

    auto idIt = langIt->second.find(id);
    if (idIt == langIt->second.end()) {
        throw std::runtime_error("String ID not found: " + id);
    }

    return idIt->second;
}

/*#include "TextTable.h"

bool TextTable::Load(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open text table file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    // Read header
    if (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        std::getline(ss, token, '\t'); // Skip first column (id)
        while (std::getline(ss, token, '\t')) {
            languages.push_back(token);
        }
    }

    // Read data rows
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string id;
        std::getline(ss, id, '\t');
        for (size_t i = 0; i < languages.size(); ++i) {
            std::string text;
            if (std::getline(ss, text, '\t')) {
                table[languages[i]][id] = text;
            }
        }
    }

    file.close();

    // Set default language
    if (!languages.empty())
        currentLanguage = languages[0];

    return true;
}

void TextTable::SetLanguage(const std::string& language)
{
    if (table.find(language) != table.end()) {
        currentLanguage = language;
    }
    else {
        std::cerr << "Language not found: " << language << std::endl;
    }
}

std::string TextTable::GetString(const std::string& id) const
{
    auto langIt = table.find(currentLanguage);
    if (langIt != table.end()) {
        auto strIt = langIt->second.find(id);
        if (strIt != langIt->second.end()) {
            return strIt->second;
        }
    }
    return "";
}*/
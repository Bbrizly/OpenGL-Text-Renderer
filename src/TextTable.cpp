#include "TextTable.h"

//return data needed to render individual letters

void TextTable::Load(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open text table file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string language, id, text;
        stream >> language >> id;
        std::getline(stream, text);
        textData[language][id] = text;
    }
}

void TextTable::SetLanguage(const std::string& language) {
    if (textData.find(language) == textData.end()) {
        throw std::invalid_argument("Language not found: " + language);
    }
    currentLanguage = language;
}

std::string TextTable::GetString(const std::string& id) {
    if (textData[currentLanguage].find(id) != textData[currentLanguage].end()) {
        return textData[currentLanguage][id];
    }
    throw std::out_of_range("Text ID not found: " + id);
}
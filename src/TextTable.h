#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
using namespace std;

class TextBox;

class TextTable
{
public:
    TextTable(const std::string& csvPath);
    ~TextTable();

    // Load CSV file with columns: ID,English,Spanish,...
    void LoadCSV(const std::string& filePath);

    // Switch current language. For example "English" or "Spanish".
    void SetLanguage(const std::string& language);

    // Return a localized string by ID, substituted for placeholders.
    std::string GetString(const std::string& id) const;

    // Add or update a property that can be used in placeholders
    // e.g. {playerName} => "Foobar"
    void SetStringProperty(const std::string& key, const std::string& value);

    // For dynamic placeholders, we do a final pass on the string:
    // e.g. "Hello, {playerName}" => "Hello, Foobar"
    std::string Substitute(const std::string& input) const;

    // Observer pattern so text boxes can re-generate if something changes
    // For example: language changed or property changed
    void RegisterOnChange(std::function<void()> observer) {
        m_observers.push_back(observer);
    }

private:
    // We notify all observers that something changed (language or property)
    void Notify() {
        for(auto& obs : m_observers) {
            obs();
        }
    }

private:
    // m_strings[language][id] = text
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_strings;
    std::string m_currentLanguage;

    // placeholder {someKey} => "someValue"
    std::unordered_map<std::string, std::string> m_substitutions;

    // watchers
    std::vector<std::function<void()>> m_observers;
};

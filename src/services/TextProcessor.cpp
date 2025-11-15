#include "./abstract/TextProcessor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

void TextProcessor::loadStopwords(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file)
    {
        throw std::runtime_error("Unable to open stopwords file: " + filepath);
    }

    std::string word;
    while (file >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c)
                       { return static_cast<char>(std::tolower(c)); });
        stopwords_.insert(word);
    }
}

std::string TextProcessor::normalize(const std::string &text)
{
    std::string normalized;
    normalized.reserve(text.size());

    for (unsigned char c : text)
    {
        if (std::isalnum(c))
        {
            normalized.push_back(static_cast<char>(std::tolower(c)));
        }
        else if (std::isspace(c))
        {
            normalized.push_back(' ');
        }
        else
        {
            normalized.push_back(' ');
        }
    }

    return normalized;
}

std::vector<std::string> TextProcessor::tokenize(const std::string &text) const
{
    std::string normalized = normalize(text);
    std::istringstream iss(normalized);
    std::string token;
    std::vector<std::string> tokens;

    while (iss >> token)
    {
        if (!token.empty() && stopwords_.find(token) == stopwords_.end())
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

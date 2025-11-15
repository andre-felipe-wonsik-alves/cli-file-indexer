#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>
#include <unordered_set>
#include <vector>

class TextProcessor
{
public:
    TextProcessor() = default;

    void loadStopwords(const std::string &filepath);

    std::vector<std::string> tokenize(const std::string &text) const;

private:
    std::unordered_set<std::string> stopwords_;

    static std::string normalize(const std::string &text);
};

#endif
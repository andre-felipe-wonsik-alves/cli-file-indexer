#include "./abstract/Indexer.h"
#include "./abstract/Index.h"
#include "./abstract/TextProcessor.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

Indexer::Indexer(Index &index, const TextProcessor &textProcessor)
    : index_(index), textProcessor_(textProcessor) {}

void Indexer::buildIndexFromDirectory(const std::string &directoryPath)
{
    namespace fs = std::filesystem;

    fs::path dir(directoryPath);
    if (!fs::exists(dir) || !fs::is_directory(dir))
    {
        throw std::runtime_error("Invalid directory: " + directoryPath);
    }

    index_.setBaseDirectory(fs::canonical(dir).string());

    for (const auto &entry : fs::recursive_directory_iterator(dir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            indexFile(entry.path().string());
        }
    }
}

void Indexer::indexFile(const std::string &filePath)
{
    namespace fs = std::filesystem;

    std::ifstream file(filePath);
    if (!file)
    {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    auto tokens = textProcessor_.tokenize(content);

    std::unordered_set<std::string> uniqueTokens(tokens.begin(), tokens.end());

    std::string filename = fs::path(filePath).filename().string();
    for (const auto &token : uniqueTokens)
    {
        index_.addToken(token, filename);
    }
}

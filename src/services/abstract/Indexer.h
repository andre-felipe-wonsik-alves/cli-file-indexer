#ifndef INDEXER_H
#define INDEXER_H

#include <string>

class Index;
class TextProcessor;

class Indexer
{
public:
    Indexer(Index &index, const TextProcessor &textProcessor);

    void buildIndexFromDirectory(const std::string &directoryPath);

private:
    Index &index_;
    const TextProcessor &textProcessor_;

    void indexFile(const std::string &filePath);
};

#endif
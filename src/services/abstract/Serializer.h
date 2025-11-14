#pragma once

#include <string>
#include "InvertedIndex.h"

class Serializer
{
public:
    void save(const InvertedIndex &index, const std::string &filePath);

    void load(InvertedIndex &index, const std::string &filePath);

    bool indexFileExists(const std::string &filePath) const;
};

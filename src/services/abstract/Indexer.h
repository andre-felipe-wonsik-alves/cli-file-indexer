#pragma once

#include <string>
#include "InvertedIndex.h"

class Indexer
{
public:
    InvertedIndex buildIndex(const std::string &directoryPath);
};

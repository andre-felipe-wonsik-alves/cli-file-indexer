#pragma once

#include <string>
#include <vector>
#include "InvertedIndex.h"

class QueryProcessor
{
public:
    void setIndex(const InvertedIndex &index);

    // Realiza a busca pelo termo e retorna lista de nomes de arquivos.
    std::vector<std::string> search(const std::string &term) const;

private:
    InvertedIndex index_;
};

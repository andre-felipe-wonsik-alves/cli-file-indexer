#pragma once

#include <string>
#include <vector>
#include "Index.h"

class QueryProcessor
{
public:
    void setIndex(const Index &index);

    // Realiza a busca pelo termo e retorna lista de nomes de arquivos.
    std::vector<std::string> search(const std::string &term) const;

private:
    Index index_;
};

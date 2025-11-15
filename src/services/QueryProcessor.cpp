#include "./abstract/QueryProcessor.h"
#include <iostream>

void QueryProcessor::setIndex(const Index &index)
{
    index_ = index;
}

std::vector<std::string> QueryProcessor::search(const std::string &term) const
{
    std::cout << "[QueryProcessor] Busca ainda NÃO implementada.\n";
    std::cout << "[QueryProcessor] TODO: usar o InvertedIndex para encontrar "
                 "arquivos contendo o termo '"
              << term << "'.\n";

    std::vector<std::string> results;

    return results;
}

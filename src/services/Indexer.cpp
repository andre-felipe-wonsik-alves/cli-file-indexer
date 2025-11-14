#include "./abstract/Indexer.h"
#include <iostream>

InvertedIndex Indexer::buildIndex(const std::string &directoryPath)
{
    std::cout << "[Indexer] Construção de índice ainda NÃO implementada.\n";
    std::cout << "[Indexer] TODO: varrer o diretório '" << directoryPath
              << "' e preencher o InvertedIndex (termo -> arquivos).\n";

    InvertedIndex index;

    return index;
}

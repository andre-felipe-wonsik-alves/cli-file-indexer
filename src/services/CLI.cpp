#include "./abstract/CLI.h"
#include "./abstract/Indexer.h"
#include "./abstract/Serializer.h"
#include "./abstract/QueryProcessor.h"

#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int CLI::run(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage();
        return 1;
    }

    std::string command = argv[1];

    if (command == "construir")
    {
        if (argc < 3)
        {
            std::cerr << "Erro: caminho do diretório não informado.\n";
            printUsage();
            return 1;
        }
        handleBuildIndex(argv[2]);
    }
    else if (command == "buscar")
    {
        if (argc < 3)
        {
            std::cerr << "Erro: termo de busca não informado.\n";
            printUsage();
            return 1;
        }
        handleSearch(argv[2]);
    }
    else
    {
        std::cerr << "Comando desconhecido: " << command << "\n";
        printUsage();
        return 1;
    }

    return 0;
}

void CLI::printUsage() const
{
    std::cout << "Uso:\n";
    std::cout << "  indice construir <caminho_do_diretorio>\n";
    std::cout << "  indice buscar <termo_de_busca>\n";
}

void CLI::handleBuildIndex(const char *directoryPathCStr)
{
    std::string directoryPath = directoryPathCStr;

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
    {
        std::cerr << "Erro: diretório inválido: " << directoryPath << "\n";
        return;
    }

    std::cout << "Iniciando indexação do diretório: " << directoryPath << "\n";

    Indexer indexer;
    InvertedIndex index = indexer.buildIndex(directoryPath);

    Serializer serializer;
    const std::string indexFileName = "index.dat";

    try
    {
        serializer.save(index, indexFileName);
        std::cout << "Índice salvo em '" << indexFileName << "'.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro ao salvar o índice: " << e.what() << "\n";
    }
}

void CLI::handleSearch(const char *termCStr)
{
    std::string term = termCStr;
    const std::string indexFileName = "index.dat";

    Serializer serializer;

    if (!serializer.indexFileExists(indexFileName))
    {
        std::cerr << "Arquivo de índice '" << indexFileName << "' não encontrado.\n";
        std::cerr << "Execute primeiro:\n";
        std::cerr << "  indice construir <caminho_do_diretorio>\n";
        return;
    }

    InvertedIndex index;
    try
    {
        serializer.load(index, indexFileName);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro ao carregar o índice: " << e.what() << "\n";
        return;
    }

    QueryProcessor queryProcessor;
    queryProcessor.setIndex(index);

    auto results = queryProcessor.search(term);

    if (results.empty())
    {
        std::cout << "Nenhum arquivo encontrado para o termo '" << term << "'.\n";
        std::cout << "(Ou a lógica de busca ainda não foi implementada.)\n";
    }
    else
    {
        std::cout << "Arquivos que contém o termo '" << term << "':\n";
        for (const auto &file : results)
        {
            std::cout << " - " << file << "\n";
        }
    }
}

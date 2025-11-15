#include "./abstract/CLI.h"
#include "./abstract/Index.h"
#include "./abstract/TextProcessor.h"
#include "./abstract/Indexer.h"
#include "./abstract/Serializer.h"

#include <filesystem>
#include <iostream>

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
            std::cout << "Erro: caminho do diretório não informado.\n\n";
            printUsage();
            return 1;
        }

        const char *directoryPath = argv[2];
        handleBuildIndex(directoryPath);
        return 0;
    }
    else if (command == "buscar")
    {
        if (argc < 3)
        {
            std::cout << "Erro: termo de busca não informado.\n\n";
            printUsage();
            return 1;
        }

        const char *term = argv[2];
        handleSearch(term);
        return 0;
    }
    else
    {
        std::cout << "Erro: comando desconhecido: " << command << "\n\n";
        printUsage();
        return 1;
    }
}

void CLI::printUsage() const
{
    std::cout << "Uso:\n"
              << "  programa index <diretorio_txt>\n"
              << "  programa search <termo>\n";
}

void CLI::handleBuildIndex(const char *directoryPathCStr)
{
    std::string directoryPath = directoryPathCStr;

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
    {
        std::cout << "Erro: diretório inválido: " << directoryPath << "\n";
        return;
    }

    std::cout << "Iniciando indexação do diretório: " << directoryPath << "\n";

    TextProcessor textProcessor;
    try
    {
        textProcessor.loadStopwords("stopwords.txt");
    }
    catch (const std::exception &e)
    {
        std::cout << "Erro ao carregar stopwords: " << e.what() << "\n";
        return;
    }

    Index index;
    Indexer indexer(index, textProcessor);

    try
    {
        indexer.buildIndexFromDirectory(directoryPath);
    }
    catch (const std::exception &e)
    {
        std::cout << "Erro durante a indexação: " << e.what() << "\n";
        return;
    }

    Serializer serializer;
    const std::string indexFileName = "index.dat";

    try
    {
        serializer.save(index, indexFileName);
        std::cout << "Índice salvo em '" << indexFileName << "'.\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Erro ao salvar o índice: " << e.what() << "\n";
    }
}

void CLI::handleSearch(const char *termCStr)
{
    std::string query = termCStr;

    Index index;
    Serializer serializer;

    try
    {
        serializer.load(index, "index.dat");
    }
    catch (const std::exception &e)
    {
        std::cout << "Erro ao carregar o índice (index.dat): " << e.what() << "\n";
        return;
    }

    TextProcessor processor;
    try
    {
        processor.loadStopwords("stopwords.txt");
    }
    catch (const std::exception &e)
    {
        std::cout << "Erro ao carregar stopwords: " << e.what() << "\n";
        return;
    }

    auto tokens = processor.tokenize(query);
    if (tokens.empty())
    {
        std::cout << "Consulta vazia ou composta apenas por stopwords.\n";
        return;
    }

    const std::string &term = tokens.front();
    auto results = index.search(term);

    if (results.empty())
    {
        std::cout << "Nenhum documento encontrado para o termo normalizado: " << term << "\n";
    }
    else
    {
        std::cout << "Documentos que contêm o termo '" << term << "':\n";
        for (const auto &path : results)
        {
            std::cout << " - " << path << "\n";
        }
    }
}
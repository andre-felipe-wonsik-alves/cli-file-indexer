#include "./abstract/CLI.h"
#include "./abstract/Index.h"
#include "./abstract/Indexer.h"
#include "./abstract/QueryProcessor.h"
#include "./abstract/Serializer.h"
#include "./abstract/TextProcessor.h"

#include <filesystem>
#include <iostream>
#include <vector>

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

        handleSearch(argc, argv);
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
              << "  programa construir <diretorio_txt>\n"
              << "  programa buscar <termo1> [termo2 ...]\n";
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

void CLI::handleSearch(int argc, char *argv[])
{
    const std::string indexFileName = "index.dat";
    if (!fs::exists(indexFileName))
    {
        std::cout << "Índice não encontrado. Execute 'indice construir <diretorio>' antes de buscar.\n";
        return;
    }

    Index index;
    Serializer serializer;

    try
    {
        serializer.load(index, indexFileName);
    }
    catch (const std::exception &e)
    {
        std::cout << "Erro ao carregar o índice (" << indexFileName << "): " << e.what() << "\n";
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

    std::vector<std::string> tokens;
    for (int i = 2; i < argc; ++i)
    {
        auto parsed = processor.tokenize(argv[i]);
        tokens.insert(tokens.end(), parsed.begin(), parsed.end());
    }

    if (tokens.empty())
    {
        std::cout << "Consulta vazia ou composta apenas por stopwords.\n";
        return;
    }

    QueryProcessor queryProcessor;
    queryProcessor.setIndex(index);

    auto results = queryProcessor.search(tokens);

    if (results.empty())
    {
        std::cout << "Nenhum documento encontrado para os termos fornecidos.\n";
    }
    else
    {
        std::cout << "Documentos que contêm todos os termos informados:\n";
        for (const auto &path : results)
        {
            std::cout << " - " << path << "\n";
        }
    }
}

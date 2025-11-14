#include "./abstract/Serializer.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

void Serializer::save(const InvertedIndex & /*index*/, const std::string &filePath)
{
    std::ofstream out(filePath, std::ios::trunc);
    if (!out)
    {
        throw std::runtime_error("Não foi possível abrir o arquivo para gravação: " + filePath);
    }

    out << "# Arquivo de índice gerado.\n";
    out << "# TODO: salvar o conteúdo REAL do índice aqui.\n";

    std::cout << "[Serializer] Índice salvo em '" << filePath
              << "' (conteúdo ainda não implementado).\n";
}

void Serializer::load(InvertedIndex &index, const std::string &filePath)
{
    std::ifstream in(filePath);
    if (!in)
    {
        throw std::runtime_error("Não foi possível abrir o arquivo de índice: " + filePath);
    }

    index.clear();

    std::cout << "[Serializer] Carregando índice de '" << filePath
              << "' (lógica ainda NÃO implementada).\n";
    std::cout << "[Serializer] TODO: ler o arquivo e reconstruir o InvertedIndex.\n";
}

bool Serializer::indexFileExists(const std::string &filePath) const
{
    return fs::exists(filePath) && fs::is_regular_file(filePath);
}

#include "./abstract/Serializer.h"
#include "./abstract/Index.h"

#include <fstream>
#include <stdexcept>

void Serializer::writeString(std::ostream &os, const std::string &str) const
{
    std::uint32_t length = static_cast<std::uint32_t>(str.size());
    writePrimitive(os, length);
    os.write(str.data(), length);
}

std::string Serializer::readString(std::istream &is) const
{
    std::uint32_t length = 0;
    readPrimitive(is, length);
    std::string str(length, '\0');
    if (length > 0)
    {
        is.read(&str[0], length);
    }
    return str;
}

void Serializer::save(const Index &index, const std::string &filepath) const
{
    std::ofstream os(filepath, std::ios::binary);
    if (!os)
    {
        throw std::runtime_error("Unable to open index file for writing: " + filepath);
    }

    const std::uint32_t magic = 0x58444E49;
    const std::uint32_t version = 1;

    writePrimitive(os, magic);
    writePrimitive(os, version);

    writeString(os, index.getBaseDirectory());

    const auto &documents = index.getAllDocumentNames();
    std::uint32_t docCount = static_cast<std::uint32_t>(documents.size());
    writePrimitive(os, docCount);

    for (const auto &name : documents)
    {
        writeString(os, name);
    }

    const auto &inverted = index.getInvertedIndex();
    std::uint32_t termCount = static_cast<std::uint32_t>(inverted.size());
    writePrimitive(os, termCount);

    for (const auto &pair : inverted)
    {
        const std::string &term = pair.first;
        const auto &postingList = pair.second;

        writeString(os, term);

        std::uint32_t postingsSize = static_cast<std::uint32_t>(postingList.size());
        writePrimitive(os, postingsSize);

        for (auto docId : postingList)
        {
            std::uint32_t id = docId;
            writePrimitive(os, id);
        }
    }
}

void Serializer::load(Index &index, const std::string &filepath) const
{
    std::ifstream is(filepath, std::ios::binary);
    if (!is)
    {
        throw std::runtime_error("Unable to open index file for reading: " + filepath);
    }

    std::uint32_t magic = 0;
    std::uint32_t version = 0;

    readPrimitive(is, magic);
    readPrimitive(is, version);

    if (magic != 0x58444E49)
    {
        throw std::runtime_error("Invalid index file (magic mismatch)");
    }

    if (version != 1)
    {
        throw std::runtime_error("Unsupported index version");
    }

    index.clear();

    std::string baseDir = readString(is);
    index.setBaseDirectory(baseDir);

    std::uint32_t docCount = 0;
    readPrimitive(is, docCount);

    for (std::uint32_t i = 0; i < docCount; ++i)
    {
        std::string name = readString(is);
        index.addDocument(name);
    }

    std::uint32_t termCount = 0;
    readPrimitive(is, termCount);

    for (std::uint32_t i = 0; i < termCount; ++i)
    {
        std::string term = readString(is);
        std::uint32_t postingsSize = 0;
        readPrimitive(is, postingsSize);

        for (std::uint32_t j = 0; j < postingsSize; ++j)
        {
            std::uint32_t docId = 0;
            readPrimitive(is, docId);
            index.addPosting(term, docId);
        }
    }
}

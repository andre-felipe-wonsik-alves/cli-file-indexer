#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <ostream>
#include <istream>
#include <cstdint>

class Index;

class Serializer
{
public:
    Serializer() = default;

    void save(const Index &index, const std::string &filepath) const;
    void load(Index &index, const std::string &filepath) const;

private:
    template <typename T>
    void writePrimitive(std::ostream &os, const T &value) const
    {
        os.write(reinterpret_cast<const char *>(&value), sizeof(T));
    }

    template <typename T>
    void readPrimitive(std::istream &is, T &value) const
    {
        is.read(reinterpret_cast<char *>(&value), sizeof(T));
    }

    void writeString(std::ostream &os, const std::string &str) const;
    std::string readString(std::istream &is) const;
};

#endif
#pragma once

class CLI
{
public:
    int run(int argc, char *argv[]);

private:
    void printUsage() const;
    void handleBuildIndex(const char *directoryPath);
    void handleSearch(int argc, char *argv[]);
};

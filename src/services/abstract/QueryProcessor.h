#pragma once

#include "Index.h"
#include <string>
#include <vector>

class QueryProcessor
{
public:
    void setIndex(const Index &index);

    std::vector<std::string> search(const std::string &term) const;
    std::vector<std::string> search(const std::vector<std::string> &terms) const;

private:
    const Index *index_{nullptr};
};

#include "./abstract/QueryProcessor.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

void QueryProcessor::setIndex(const Index &index)
{
    index_ = &index;
}

std::vector<std::string> QueryProcessor::search(const std::string &term) const
{
    if (term.empty())
    {
        return {};
    }
    return search(std::vector<std::string>{term});
}

std::vector<std::string> QueryProcessor::search(const std::vector<std::string> &terms) const
{
    if (!index_)
    {
        std::cout << "QueryProcessor index not set" << "\n";
        return;
    }

    std::vector<std::string> filteredTerms;
    filteredTerms.reserve(terms.size());
    for (const auto &term : terms)
    {
        if (!term.empty())
        {
            filteredTerms.push_back(term);
        }
    }

    if (filteredTerms.empty())
    {
        return {};
    }

    const auto &invertedIndex = index_->getInvertedIndex();
    std::vector<const Index::PostingList *> postingLists;
    postingLists.reserve(filteredTerms.size());

    for (const auto &term : filteredTerms)
    {
        auto it = invertedIndex.find(term);
        if (it == invertedIndex.end() || it->second.empty())
        {
            return {};
        }
        postingLists.push_back(&(it->second));
    }

    std::sort(postingLists.begin(), postingLists.end(),
              [](const Index::PostingList *a, const Index::PostingList *b)
              { return a->size() < b->size(); });

    std::vector<Index::DocumentId> intersection;
    intersection.reserve(postingLists.front()->size());
    for (auto docId : *postingLists.front())
    {
        intersection.push_back(docId);
    }

    std::vector<Index::DocumentId> buffer;
    for (std::size_t i = 1; i < postingLists.size(); ++i)
    {
        const auto *currentList = postingLists[i];
        buffer.clear();
        for (auto docId : intersection)
        {
            if (currentList->find(docId) != currentList->end())
            {
                buffer.push_back(docId);
            }
        }

        if (buffer.empty())
        {
            return {};
        }

        intersection.swap(buffer);
    }

    namespace fs = std::filesystem;
    fs::path basePath(index_->getBaseDirectory());
    std::vector<std::string> results;
    results.reserve(intersection.size());
    for (auto docId : intersection)
    {
        fs::path fullPath = basePath / index_->getDocumentName(docId);
        results.push_back(fullPath.string());
    }

    std::sort(results.begin(), results.end());
    return results;
}

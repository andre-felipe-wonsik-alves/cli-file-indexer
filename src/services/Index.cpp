#include "./abstract/Index.h"
#include <stdexcept>
#include <filesystem>

void Index::clear()
{
    invertedIndex_.clear();
    filenameToId_.clear();
    idToFilename_.clear();
    baseDirectory_.clear();
}

Index::DocumentId Index::addDocument(const std::string &filename)
{
    auto it = filenameToId_.find(filename);
    if (it != filenameToId_.end())
    {
        return it->second;
    }

    DocumentId id = static_cast<DocumentId>(idToFilename_.size());
    filenameToId_[filename] = id;
    idToFilename_.push_back(filename);
    return id;
}

const std::string &Index::getDocumentName(DocumentId id) const
{
    if (id >= idToFilename_.size())
    {
        throw std::out_of_range("Invalid document ID");
    }
    return idToFilename_[id];
}

const std::vector<std::string> &Index::getAllDocumentNames() const
{
    return idToFilename_;
}

void Index::addToken(const std::string &token, const std::string &filename)
{
    DocumentId docId = addDocument(filename);
    invertedIndex_[token].insert(docId);
}

void Index::addPosting(const std::string &term, DocumentId documentId)
{
    invertedIndex_[term].insert(documentId);
}

void Index::setBaseDirectory(const std::string &baseDirectory)
{
    baseDirectory_ = baseDirectory;
}

std::vector<std::string> Index::search(const std::string &term) const
{
    std::vector<std::string> result;
    auto it = invertedIndex_.find(term);
    if (it == invertedIndex_.end())
    {
        return result;
    }

    namespace fs = std::filesystem;
    for (DocumentId id : it->second)
    {
        fs::path fullPath = fs::path(baseDirectory_) / getDocumentName(id);
        result.push_back(fullPath.string());
    }
    return result;
}
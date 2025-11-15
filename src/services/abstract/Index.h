#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdint>

class Index
{
public:
    using DocumentId = std::uint32_t;
    using PostingList = std::unordered_set<DocumentId>;
    using InvertedIndex = std::unordered_map<std::string, PostingList>;

    Index() = default;

    void clear();

    DocumentId addDocument(const std::string &filename);
    const std::string &getDocumentName(DocumentId id) const;
    const std::vector<std::string> &getAllDocumentNames() const;

    void addToken(const std::string &token, const std::string &filename);
    void addPosting(const std::string &term, DocumentId documentId);

    const InvertedIndex &getInvertedIndex() const { return invertedIndex_; }
    InvertedIndex &getInvertedIndex() { return invertedIndex_; }

    const std::string &getBaseDirectory() const { return baseDirectory_; }
    void setBaseDirectory(const std::string &baseDirectory);

    std::vector<std::string> search(const std::string &term) const;

private:
    InvertedIndex invertedIndex_;
    std::unordered_map<std::string, DocumentId> filenameToId_;
    std::vector<std::string> idToFilename_;
    std::string baseDirectory_;
};

#endif
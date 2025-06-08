//
// Created by CanhDo on 2024/12/20.
//

#ifndef SEARCH_HPP
#define SEARCH_HPP

class Search {
public:
    enum class Type {
        ARROW_ONE,
        ARROW_STAR,
        ARROW_PLUS,
        ARROW_EXCLAMATION
    };

    inline static const char *typeNames[] = {
        "=>1",
        "=>*",
        "=>+",
        "=>!"
    };

    const char *getSearchType(Type searchType) const {
        return typeNames[static_cast<int>(searchType)];
    }

    bool isArrowExclamation(Type searchType) const {
        return searchType == Type::ARROW_EXCLAMATION;
    }

    bool isArrowStar(Type searchType) const {
        return searchType == Type::ARROW_STAR;
    }

    bool isArrowPlus(Type searchType) const {
        return searchType == Type::ARROW_PLUS;
    }

    bool isArrowOne(Type searchType) const {
        return searchType == Type::ARROW_ONE;
    }
};

#endif //SEARCH_HPP

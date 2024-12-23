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
};

#endif //SEARCH_HPP

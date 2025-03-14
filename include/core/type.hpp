//
// Created by CanhDo on 2024/11/15.
//

#ifndef TYPE_HPP
#define TYPE_HPP

enum class Type {
    Qubit,
    Complex
};

inline static const char *typeNames[] = {
    "Qubit",
    "Complex"
};

inline const char *getTypeName(Type type) {
    return typeNames[static_cast<int>(type)];
}

#endif//TYPE_HPP

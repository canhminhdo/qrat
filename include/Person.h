//
// Created by CanhDo on 2024/09/06.
//

#ifndef QRAT_PERSON_H
#define QRAT_PERSON_H

#include <string>

namespace qrat {
    /**
     * @brief Person class
     * @details This is the detail
     */
    class Person {
    public:
        /**
         * @brief default constructor
         */
        Person() = default;
        /**
         * @brief get name of the person
         * @return string the name of the person
         */
        std::string getName(void);
        /**
         * @brief set name for a person
         * @param name the string being set to the name
         */
        void setName(std::string name);

    private:
        std::string name;
    };
}// namespace qrat

#endif//QRAT_PERSON_H

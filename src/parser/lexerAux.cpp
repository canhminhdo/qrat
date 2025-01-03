//
// Created by CanhDo on 2024/09/10.
//

#include "parser/lexerAux.hpp"
#include <vector>

std::vector<char *> pendingFiles;

void lexerIdMode() {
    BEGIN(ID_MODE);
}

void lexerCmdMode() {
    BEGIN(CMD_MODE);
}

void lexerInitialMode() {
    BEGIN(INITIAL);
}

void lexerCommentMode() {
    BEGIN(COMMENT_MODE);
}
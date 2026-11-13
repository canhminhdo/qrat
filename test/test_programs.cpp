//
// Created by CanhDo on 2024/09/06.
//

#include <gtest/gtest.h>
#include <cstdlib>

TEST(ProgTest, teleportProg) {
    // Define a buffer to store the current working directory
    char buffer[1024];

    // Get the current working directory
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        std::cout << "Current working directory: " << buffer << std::endl;
    } else {
        std::cerr << "Error getting current working directory" << std::endl;
    }
    int res = std::system("bash testScript.sh teleport");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, loopProg) {
    int res = std::system("bash testScript.sh loop");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, rusProg) {
    int res = std::system("bash testScript.sh rus");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, groverProg) {
    int res = std::system("bash testScript.sh grover");
    EXPECT_EQ(res, 0);
}

// exact backend

TEST(ProgTest, teleportExactProg) {
    int res = std::system("bash testScript.sh exact/teleport");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, loopExactProg) {
    int res = std::system("bash testScript.sh exact/loop");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, rusExactProg) {
    int res = std::system("bash testScript.sh exact/rus");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, groverExactProg) {
    int res = std::system("bash testScript.sh exact/grover");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, seqmeasExactProg) {
    int res = std::system("bash testScript.sh exact/seqmeas");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, unsupportedExactProg) {
    int res = std::system("bash testScript.sh exact/unsupported");
    EXPECT_EQ(res, 0);
}

// mix

TEST(ProgTest, mixedBackendProg) {
    int res = std::system("bash testScript.sh mix/mixed-backend");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, nestedIfProg) {
    int res = std::system("bash testScript.sh mix/nested-if");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, ifInLoopProg) {
    int res = std::system("bash testScript.sh mix/if-in-loop");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, redefineProg) {
    int res = std::system("bash testScript.sh mix/redefine-prog");
    EXPECT_EQ(res, 0);
}

TEST(ProgTest, emptyProg) {
    int res = std::system("bash testScript.sh mix/empty-prog");
    EXPECT_EQ(res, 0);
}

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

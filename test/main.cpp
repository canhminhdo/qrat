//
// Created by CanhDo on 2024/09/06.
//

#include <gtest/gtest.h>

extern int boostrap(void);

TEST(QRAT, Bootstrap) {
    boostrap();
    EXPECT_EQ(1, 0);
}
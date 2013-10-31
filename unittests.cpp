// tests.cpp
#include "cw-test.c"
#include <gtest/gtest.h>
 
TEST(FileReaderTest, ValidFiles) { 
    ASSERT_EQ(1, read("test1"));
    ASSERT_EQ(1, read("test2"));
}

TEST(FileReaderTest, InvalidFiles) { 
    ASSERT_EQ(0, read("not-a-file"));
    ASSERT_EQ(0, read("1234"));
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
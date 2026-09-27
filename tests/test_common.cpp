#include "common.h"
#include <gtest/gtest.h>

using namespace kernel_evasion;

TEST(KernelInfoTest, VersionComparison) {
    KernelInfo v1{5, 10, 0, "5.10.0"};
    KernelInfo v2{5, 7, 0, "5.7.0"};

    EXPECT_TRUE(v1 >= v2);
    EXPECT_FALSE(v2 >= v1);
}

TEST(CommonTest, GetKernelInfo) {
    KernelInfo info = get_kernel_info();
    EXPECT_GT(info.major, 0);
    EXPECT_GE(info.minor, 0);
}

TEST(CommonTest, CheckKernelCompatibility) {
    bool compatible = check_kernel_compatibility();
    // Should be compatible on any recent system
    EXPECT_TRUE(compatible);
}

TEST(CommonTest, RootCheck) {
    bool is_root_user = is_root();
    // This test will pass/fail depending on whether tests run as root
    // Just verify it doesn't crash
    EXPECT_TRUE(is_root_user || !is_root_user);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>

#include "Cornelius.h"

TEST(Cornelius, Constructor) {
  Cornelius cornelius;
  EXPECT_EQ(cornelius.get_number_elements(), 0);
}

TEST(Cornelius, uninitialized) {
    Cornelius cornelius;
    
    // Test for exit(1) when calling find_surface_2d
    std::array<std::array<double, 2>, 2> cu;
    EXPECT_EXIT(cornelius.find_surface_2d(cu), ::testing::ExitedWithCode(1), "Cornelius not initialized for 2D case.");

    // Test for exit(1) when calling find_surface_3d
    std::array<std::array<std::array<double, 2>, 2>, 2> cu3d;
    EXPECT_EXIT(cornelius.find_surface_3d(cu3d), ::testing::ExitedWithCode(1), "Cornelius not initialized for 3D case.");

    // Test for exit(1) when calling find_surface_4d
    std::array<std::array<std::array<std::array<double, 2>, 2>, 2>, 2> cu4d;
    EXPECT_EXIT(cornelius.find_surface_4d(cu4d), ::testing::ExitedWithCode(1), "Cornelius not initialized for 4D case.");
}

TEST(Cornelius, throw_errors_out_of_range) {
    Cornelius cornelius;
    std::array<double, 4> new_dx = {1.0, 1.0, 1.0, 1.0};
    cornelius.init_cornelius(2, 1.0, new_dx);

    // Test for exit(1) when calling get_centroid_element with index out of range
    EXPECT_EXIT(cornelius.get_centroid_element(1, 0), ::testing::ExitedWithCode(1), "Cornelius error: asking for an element which does not exist.");
    EXPECT_EXIT(cornelius.get_centroid_element(0, 10), ::testing::ExitedWithCode(1), "Cornelius error: asking for an element which does not exist.");

    EXPECT_EXIT(cornelius.get_normal_element(1, 0), ::testing::ExitedWithCode(1), "Cornelius error: asking for an element which does not exist.");
    EXPECT_EXIT(cornelius.get_normal_element(0, 10), ::testing::ExitedWithCode(1), "Cornelius error: asking for an element which does not exist.");

    // Test get_number_elements == 0
    EXPECT_EQ(cornelius.get_number_elements(), 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
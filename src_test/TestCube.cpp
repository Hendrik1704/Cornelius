#include <gtest/gtest.h>

#include "Cube.h"

TEST(CubeTest, init_cube) {
  Cube cube;
  std::array<std::array<std::array<double, 2>, 2>, 2> cu = {
      {{{{0, 0}, {0, 1}}}, {{{1, 0}, {1, 1}}}}};
  int new_const_i = 2;
  double new_const_value = 0;
  std::array<double, 4> new_dx = {0.1, 0.1, 0.1, 0.1};
  cube.init_cube(cu, new_const_i, new_const_value, new_dx);

  EXPECT_EQ(cube.get_number_lines(), 0);
  EXPECT_EQ(cube.get_number_polygons(), 0);
  EXPECT_FALSE(cube.is_ambiguous());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
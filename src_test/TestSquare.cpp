#include <gtest/gtest.h>

#include "Square.h"

TEST(SquareTest, init_square) {
  Square square;
  std::array<std::array<double, 2>, 2> sq = {{{0, 0}, {1, 1}}};
  std::array<int, 2> c_i = {2, 3};
  std::array<double, 2> c_v = {0, 0};
  std::array<double, 4> dx = {0.1, 0.1, 0.1, 0.1};
  square.init_square(sq, c_i, c_v, dx);

  EXPECT_EQ(square.get_number_lines(), 0);
  EXPECT_FALSE(square.is_ambiguous());
}

TEST(SquareTest, construct_lines) {
  Square square;
  std::array<std::array<double, 2>, 2> sq = {{{0, 0}, {1, 1}}};
  std::array<int, 2> c_i = {2, 3};
  std::array<double, 2> c_v = {0, 0};
  std::array<double, 4> dx = {0.1, 0.1, 0.1, 0.1};
  square.init_square(sq, c_i, c_v, dx);
  square.construct_lines(0.5);

  EXPECT_EQ(square.get_number_lines(), 1);
  EXPECT_FALSE(square.is_ambiguous());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
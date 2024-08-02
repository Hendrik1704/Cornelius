#include <array>
#include <gtest/gtest.h>

#include "Line.h"

TEST(LineTest, init_line) {
  Line line;

  std::array<std::array<double, 4>, 2> corners = {{{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<double, 2> const_i = {0, 1};

  line.init_line(corners, out, const_i);

  ASSERT_EQ(line.get_start_point()[0], 0);
  ASSERT_EQ(line.get_start_point()[1], 0);
  ASSERT_EQ(line.get_start_point()[2], 0);
  ASSERT_EQ(line.get_start_point()[3], 0);

  ASSERT_EQ(line.get_end_point()[0], 1);
  ASSERT_EQ(line.get_end_point()[1], 1);
  ASSERT_EQ(line.get_end_point()[2], 1);
  ASSERT_EQ(line.get_end_point()[3], 1);

  ASSERT_EQ(line.get_outside_point()[0], 2);
  ASSERT_EQ(line.get_outside_point()[1], 1);
  ASSERT_EQ(line.get_outside_point()[2], 1);
  ASSERT_EQ(line.get_outside_point()[3], 1);

  // flip start and end points
  line.flip_start_end();
  ASSERT_EQ(line.get_start_point()[0], 1);
  ASSERT_EQ(line.get_start_point()[1], 1);
  ASSERT_EQ(line.get_start_point()[2], 1);
  ASSERT_EQ(line.get_start_point()[3], 1);

  ASSERT_EQ(line.get_end_point()[0], 0);
  ASSERT_EQ(line.get_end_point()[1], 0);
  ASSERT_EQ(line.get_end_point()[2], 0);
  ASSERT_EQ(line.get_end_point()[3], 0);
}

TEST(LineTest, calculate_centroid) {
  Line line;

  std::array<std::array<double, 4>, 2> corners = {{{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<double, 2> const_i = {0, 1};

  line.init_line(corners, out, const_i);

  line.calculate_centroid();

  ASSERT_EQ(line.get_centroid()[0], 0.5);
  ASSERT_EQ(line.get_centroid()[1], 0.5);
  ASSERT_EQ(line.get_centroid()[2], 0.5);
  ASSERT_EQ(line.get_centroid()[3], 0.5);
}

TEST(LineTest, calculate_normal) {
  Line line;

  std::array<std::array<double, 4>, 2> corners = {{{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<double, 2> const_i = {0, 1};

  line.init_line(corners, out, const_i);

  line.calculate_normal();

  ASSERT_EQ(line.get_normal()[0], 0);  // fixed by const_i[0]
  ASSERT_EQ(line.get_normal()[1], 0);  // fixed by const_i[1]
  ASSERT_EQ(line.get_normal()[2], -1);
  ASSERT_EQ(line.get_normal()[3], 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
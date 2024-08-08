#include <gtest/gtest.h>

#include "Polygon.h"

TEST(PolygonTest, init_polygon) {
  Polygon polygon;

  polygon.init_polygon(0);

  ASSERT_EQ(polygon.get_number_lines(), 0);
}

TEST(PolygonTest, add_line) {
  Polygon polygon;

  polygon.init_polygon(0);

  Line line1;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 1, 1, 1}, {2, 2, 2, 2}}};
  Line line2;
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 2, 2, 2}, {3, 3, 3, 3}}};
  Line line3;
  line3.init_line(corners3, out, const_i);

  // line 4 is not connected to the Polygon
  std::array<std::array<double, 4>, 2> corners4 = {
      {{5, 5, 5, 5}, {6, 6, 6, 6}}};
  Line line4;
  line4.init_line(corners4, out, const_i);

  bool b1 = polygon.add_line(line1, false);
  bool b2 = polygon.add_line(line2, false);
  bool b3 = polygon.add_line(line3, false);
  bool b4 = polygon.add_line(line4, false);

  ASSERT_TRUE(b1);
  ASSERT_TRUE(b2);
  ASSERT_TRUE(b3);
  ASSERT_FALSE(b4);
}

TEST(PolygonTest, calculate_centroid) {
  Polygon polygon;

  polygon.init_polygon(0);

  Line line1;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 1, 1, 1}, {2, 2, 2, 2}}};
  Line line2;
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 2, 2, 2}, {3, 3, 3, 3}}};
  Line line3;
  line3.init_line(corners3, out, const_i);

  // Case with 3 lines
  polygon.add_line(line1, false);
  polygon.add_line(line2, false);
  polygon.add_line(line3, false);

  polygon.calculate_centroid();

  ASSERT_EQ(polygon.get_centroid()[0], 1.5);
  ASSERT_EQ(polygon.get_centroid()[1], 1.5);
  ASSERT_EQ(polygon.get_centroid()[2], 1.5);
  ASSERT_EQ(polygon.get_centroid()[3], 1.5);

  // Case with 4 lines
  std::array<std::array<double, 4>, 2> corners4 = {
      {{3, 3, 3, 3}, {-3, -1, -2, -3}}};
  Line line4;
  line4.init_line(corners4, out, const_i);

  polygon.add_line(line4, false);

  polygon.calculate_centroid();

  ASSERT_NEAR(polygon.get_centroid()[0], 0.625, 1e-3);
  ASSERT_NEAR(polygon.get_centroid()[1], 1.208, 1e-3);
  ASSERT_NEAR(polygon.get_centroid()[2], 0.916, 1e-3);
  ASSERT_NEAR(polygon.get_centroid()[3], 0.625, 1e-3);
}

TEST(PolygonTest, calculate_normal) {
  Polygon polygon;

  polygon.init_polygon(0);

  Line line1;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 1, 1, 1}, {2, 2, 2, 2}}};
  Line line2;
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 2, 2, 2}, {3, 2, 3, 3}}};
  Line line3;
  line3.init_line(corners3, out, const_i);

  polygon.add_line(line1, false);
  polygon.add_line(line2, false);
  polygon.add_line(line3, false);

  polygon.calculate_normal();

  ASSERT_EQ(polygon.get_normal()[0], 0);
  ASSERT_EQ(polygon.get_normal()[1], 0);
  ASSERT_EQ(polygon.get_normal()[2], -0.5);
  ASSERT_EQ(polygon.get_normal()[3], 0.5);
}

TEST(PolygonTest, get_lines) {
  Polygon polygon;

  polygon.init_polygon(0);

  Line line1;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 1, 1, 1}, {2, 2, 2, 2}}};
  Line line2;
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 2, 2, 2}, {3, 2, 3, 3}}};
  Line line3;
  line3.init_line(corners3, out, const_i);

  polygon.add_line(line1, false);
  polygon.add_line(line2, false);
  polygon.add_line(line3, false);

  auto lines = polygon.get_lines();

  ASSERT_EQ(lines[0].get_start_point()[0], 0);
  ASSERT_EQ(lines[0].get_start_point()[1], 0);
  ASSERT_EQ(lines[0].get_start_point()[2], 0);
  ASSERT_EQ(lines[0].get_start_point()[3], 0);

  ASSERT_EQ(lines[0].get_end_point()[0], 1);
  ASSERT_EQ(lines[0].get_end_point()[1], 1);
  ASSERT_EQ(lines[0].get_end_point()[2], 1);
  ASSERT_EQ(lines[0].get_end_point()[3], 1);

  ASSERT_EQ(lines[0].get_outside_point()[0], 2);
  ASSERT_EQ(lines[0].get_outside_point()[1], 1);
  ASSERT_EQ(lines[0].get_outside_point()[2], 1);
  ASSERT_EQ(lines[0].get_outside_point()[3], 1);

  ASSERT_EQ(lines[1].get_start_point()[0], 1);
  ASSERT_EQ(lines[1].get_start_point()[1], 1);
  ASSERT_EQ(lines[1].get_start_point()[2], 1);
  ASSERT_EQ(lines[1].get_start_point()[3], 1);

  ASSERT_EQ(lines[1].get_end_point()[0], 2);
  ASSERT_EQ(lines[1].get_end_point()[1], 2);
  ASSERT_EQ(lines[1].get_end_point()[2], 2);
  ASSERT_EQ(lines[1].get_end_point()[3], 2);

  ASSERT_EQ(lines[1].get_outside_point()[0], 2);
  ASSERT_EQ(lines[1].get_outside_point()[1], 1);
  ASSERT_EQ(lines[1].get_outside_point()[2], 1);
  ASSERT_EQ(lines[1].get_outside_point()[3], 1);

  ASSERT_EQ(lines[2].get_start_point()[0], 2);
  ASSERT_EQ(lines[2].get_start_point()[1], 2);
  ASSERT_EQ(lines[2].get_start_point()[2], 2);
  ASSERT_EQ(lines[2].get_start_point()[3], 2);

  ASSERT_EQ(lines[2].get_end_point()[0], 3);
  ASSERT_EQ(lines[2].get_end_point()[1], 2);
  ASSERT_EQ(lines[2].get_end_point()[2], 3);
  ASSERT_EQ(lines[2].get_end_point()[3], 3);

  ASSERT_EQ(lines[2].get_outside_point()[0], 2);
  ASSERT_EQ(lines[2].get_outside_point()[1], 1);
  ASSERT_EQ(lines[2].get_outside_point()[2], 1);
  ASSERT_EQ(lines[2].get_outside_point()[3], 1);
}

TEST(PolygonTest, print) {
  Polygon polygon;

  polygon.init_polygon(0);

  Line line1;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 1, 1, 1}, {2, 2, 2, 2}}};
  Line line2;
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 2, 2, 2}, {3, 3, 3, 3}}};
  Line line3;
  line3.init_line(corners3, out, const_i);

  polygon.add_line(line1, false);
  polygon.add_line(line2, false);
  polygon.add_line(line3, false);

  std::ofstream file;
  file.open("test_polygon_print.txt");
  std::array<double, 4> position = {0, 0, 0, 0};
  polygon.print(file, position);
  file.close();

  // check that the file exists
  std::ifstream file_check("test_polygon_print.txt");

  // delete the file after the test
  remove("test_polygon_print.txt");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
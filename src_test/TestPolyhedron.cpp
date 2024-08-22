#include <gtest/gtest.h>

#include "Line.h"
#include "Polyhedron.h"

TEST(PolyhedronTest, init_polyhedron) {
  Polyhedron polyhedron;

  polyhedron.init_polyhedron();

  ASSERT_EQ(polyhedron.get_number_polygons(), 0);
  ASSERT_EQ(polyhedron.get_number_tetrahedrons(), 0);
}

TEST(PolyhedronTest, lines_are_equal) {
  Line line1;
  Line line2;
  Line line3;
  Polyhedron polyhedron;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  line2.init_line(corners2, out, const_i);

  ASSERT_TRUE(polyhedron.lines_are_connected(line1, line2));

  std::array<std::array<double, 4>, 2> corners3 = {
      {{0, 0, 0, 2}, {1, 1, 1, 3}}};
  line3.init_line(corners3, out, const_i);

  ASSERT_FALSE(polyhedron.lines_are_connected(line1, line3));
}

TEST(PolyhedronTest, tetrahedron_volume) {
  Polyhedron polyhedron;
  std::array<double, 4> v1 = {1, 0, 0, 0};
  std::array<double, 4> v2 = {1, 2, 0, 0};
  std::array<double, 4> v3 = {1, 1, 1, 1};
  std::array<double, 4> n = {0, 0, 0, 0};

  polyhedron.tetrahedron_volume(v1, v2, v3, n);

  ASSERT_NEAR(n[0], 0.0, 1e-10);
  ASSERT_NEAR(n[1], 0.0, 1e-10);
  ASSERT_NEAR(n[2], 1. / 3., 1e-10);
  ASSERT_NEAR(n[3], -1. / 3., 1e-10);
}

TEST(PolyhedronTest, add_polygon) {
  Polyhedron polyhedron;
  Polygon polygon1;

  polygon1.init_polygon(0);
  polyhedron.init_polyhedron();

  Line line1;
  Line line2;
  Line line3;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{0, 0, 0, 0}, {1, 1, 1, 1}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 1, 1, 1}, {2, 2, 2, 2}}};
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 2, 2, 2}, {3, 3, 3, 3}}};
  line3.init_line(corners3, out, const_i);

  polygon1.add_line(line1, false);
  polygon1.add_line(line2, false);
  polygon1.add_line(line3, false);

  ASSERT_EQ(polyhedron.get_number_polygons(), 0);
  bool b1 = polyhedron.add_polygon(polygon1, false);
  ASSERT_EQ(polyhedron.get_number_polygons(), 1);
  ASSERT_TRUE(b1);
}

TEST(PolyhedronTest, calculate_centroid) {
  Polyhedron polyhedron;
  Polygon polygon1;

  polygon1.init_polygon(0);
  polyhedron.init_polyhedron();

  Line line1;
  Line line2;
  Line line3;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{5, 1, 2, 3}, {1, 3, 5, 6}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 3, 5, 6}, {2, 3, 4, 5}}};
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 3, 4, 5}, {5, 1, 2, 3}}};
  line3.init_line(corners3, out, const_i);

  std::array<std::array<double, 4>, 2> corners4 = {
      {{5, 1, 2, 3}, {7, 2, 4, 9}}};

  polygon1.add_line(line1, false);
  polygon1.add_line(line2, false);
  polygon1.add_line(line3, false);
  polygon1.add_line(line3, false);

  polyhedron.add_polygon(polygon1, false);
  polyhedron.calculate_centroid();

  std::array<double, 4> centroid = polyhedron.get_centroid();
  ASSERT_NEAR(centroid[0], 2.79167, 1e-5);
  ASSERT_NEAR(centroid[1], 2.28333, 1e-5);
  ASSERT_NEAR(centroid[2], 3.56667, 1e-5);
  ASSERT_NEAR(centroid[3], 4.56667, 1e-5);
}

TEST(PolyhedronTest, calculate_normal) {
  Polyhedron polyhedron;
  Polygon polygon1;

  polygon1.init_polygon(0);
  polyhedron.init_polyhedron();

  Line line1;
  Line line2;
  Line line3;

  std::array<std::array<double, 4>, 2> corners1 = {
      {{5, 1, 2, 3}, {1, 3, 5, 6}}};
  std::array<double, 4> out = {2, 1, 1, 1};
  std::array<int, 2> const_i = {0, 1};
  line1.init_line(corners1, out, const_i);

  std::array<std::array<double, 4>, 2> corners2 = {
      {{1, 3, 5, 6}, {2, 3, 4, 5}}};
  line2.init_line(corners2, out, const_i);

  std::array<std::array<double, 4>, 2> corners3 = {
      {{2, 3, 4, 5}, {5, 1, 2, 3}}};
  line3.init_line(corners3, out, const_i);

  std::array<std::array<double, 4>, 2> corners4 = {
      {{5, 1, 2, 3}, {7, 2, 4, 9}}};

  polygon1.add_line(line1, false);
  polygon1.add_line(line2, false);
  polygon1.add_line(line3, false);
  polygon1.add_line(line3, false);

  polyhedron.add_polygon(polygon1, false);
  polyhedron.calculate_normal();

  std::array<double, 4> normal = polyhedron.get_normal();
  ASSERT_NEAR(normal[0], 0.0, 1e-5);
  ASSERT_NEAR(normal[1], 0.0, 1e-5);
  ASSERT_NEAR(normal[2], 0.0, 1e-5);
  ASSERT_NEAR(normal[3], 0.0, 1e-5);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

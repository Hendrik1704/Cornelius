#include <gtest/gtest.h>

#include "GeneralGeometryElement.h"

TEST(GeneralGeometryElementTest, flip_normal_if_needed) {
  GeneralGeometryElement element;
  std::array<double, 4> normal1 = {1, 2, 3, 4};
  std::array<double, 4> reference_normal1 = {1, 2, 3, 4};
  element.flip_normal_if_needed(normal1, reference_normal1);
  ASSERT_EQ(normal1[0], 1);
  ASSERT_EQ(normal1[1], 2);
  ASSERT_EQ(normal1[2], 3);
  ASSERT_EQ(normal1[3], 4);

  std::array<double, 4> normal2 = {1, 0, 0, 0};
  std::array<double, 4> reference_normal2 = {-1, 0, 0, 0};
  element.flip_normal_if_needed(normal2, reference_normal2);
  ASSERT_EQ(normal2[0], -1);
  ASSERT_EQ(normal2[1], 0);
  ASSERT_EQ(normal2[2], 0);
  ASSERT_EQ(normal2[3], 0);
}

// test that the get_normal function returns an array with 4 elements
TEST(GeneralGeometryElementTest, get_normal) {
  GeneralGeometryElement element;
  std::array<double, 4> normal = element.get_normal();
  ASSERT_EQ(normal.size(), 4);
}

// test that the get_centroid function returns an array with 4 elements
TEST(GeneralGeometryElementTest, get_centroid) {
  GeneralGeometryElement element;
  std::array<double, 4> centroid = element.get_centroid();
  ASSERT_EQ(centroid.size(), 4);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

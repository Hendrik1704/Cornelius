#include <gtest/gtest.h>

#include "Hypercube.h"

TEST(HypercubeTest, init_hypercube) {
  Hypercube hypercube;
  std::array<std::array<std::array<std::array<double, 2>, 2>, 2>, 2> hc;
  hc[0][0][0][0] = 0;
  hc[0][0][0][1] = 1;
  hc[0][0][1][0] = 2;
  hc[0][0][1][1] = 3;
  hc[0][1][0][0] = 4;
  hc[0][1][0][1] = 5;
  hc[0][1][1][0] = 6;
  hc[0][1][1][1] = 7;
  hc[1][0][0][0] = 8;
  hc[1][0][0][1] = 9;
  hc[1][0][1][0] = 10;
  std::array<double, 4> new_dx = {0.1, 0.1, 0.1, 0.1};
  hypercube.init_hypercube(hc, new_dx);

  EXPECT_EQ(hypercube.get_number_polyhedra(), 0);
  EXPECT_FALSE(hypercube.is_ambiguous());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
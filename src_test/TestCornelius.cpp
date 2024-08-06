#include <chrono>
#include <gtest/gtest.h>

#include "Cornelius.h"
#include "cornelius_old.h"

TEST(Cornelius, Constructor) {
  Cornelius cornelius;
  EXPECT_EQ(cornelius.get_number_elements(), 0);
}

TEST(CorneliusTest, uninitialized) {
  Cornelius cornelius;

  // Test for exit(1) when calling find_surface_2d
  std::array<std::array<double, 2>, 2> cu;
  EXPECT_EXIT(cornelius.find_surface_2d(cu), ::testing::ExitedWithCode(1),
              "Cornelius not initialized for 2D case.");

  // Test for exit(1) when calling find_surface_3d
  std::array<std::array<std::array<double, 2>, 2>, 2> cu3d;
  EXPECT_EXIT(cornelius.find_surface_3d(cu3d), ::testing::ExitedWithCode(1),
              "Cornelius not initialized for 3D case.");

  // Test for exit(1) when calling find_surface_4d
  std::array<std::array<std::array<std::array<double, 2>, 2>, 2>, 2> cu4d;
  EXPECT_EXIT(cornelius.find_surface_4d(cu4d), ::testing::ExitedWithCode(1),
              "Cornelius not initialized for 4D case.");
}

TEST(CorneliusTest, throw_errors_out_of_range) {
  Cornelius cornelius;
  std::array<double, 4> new_dx = {1.0, 1.0, 1.0, 1.0};
  cornelius.init_cornelius(2, 1.0, new_dx);

  // Test for exit(1) when calling get_centroid_element with index out of range
  EXPECT_EXIT(cornelius.get_centroid_element(1, 0),
              ::testing::ExitedWithCode(1),
              "Cornelius error: asking for an element which does not exist.");
  EXPECT_EXIT(cornelius.get_centroid_element(0, 10),
              ::testing::ExitedWithCode(1),
              "Cornelius error: asking for an element which does not exist.");

  EXPECT_EXIT(cornelius.get_normal_element(1, 0), ::testing::ExitedWithCode(1),
              "Cornelius error: asking for an element which does not exist.");
  EXPECT_EXIT(cornelius.get_normal_element(0, 10), ::testing::ExitedWithCode(1),
              "Cornelius error: asking for an element which does not exist.");

  // Test get_number_elements == 0
  EXPECT_EQ(cornelius.get_number_elements(), 0);
}

TEST(CorneliusTest, apply_to_3D_surface) {
  double grid_dt = 0.1;
  double grid_dx = 0.2;
  double grid_dy = 0.2;
  double T_cut = 0.16;
  std::array<double, 4> dx = {grid_dt, grid_dx, grid_dy, 1.0};

  const int dim = 3;
  double lattice_spacing[dim];
  lattice_spacing[0] = grid_dt;
  lattice_spacing[1] = grid_dx;
  lattice_spacing[2] = grid_dy;

  // load the cornelius_3D_input_#.dat file
  // the file contains the following 3D cube in each line:
  // tau_local x_local y_local cube[0][0][0] cube[0][0][1] cube[0][1][0]
  // cube[0][1][1] cube[1][0][0] cube[1][0][1] cube[1][1][0] cube[1][1][1]
  // perform the test for each cube in the file

  // for the test with the old cornelius version
  double ***cube = new double **[2];
  for (int i = 0; i < 2; i++) {
    cube[i] = new double *[2];
    for (int j = 0; j < 2; j++) {
      cube[i][j] = new double[2];
      for (int k = 0; k < 2; k++)
        cube[i][j][k] = 0.0;
    }
  }

  for (int cube_file = 0; cube_file < 10; cube_file++) {
    std::string filename =
        "cornelius_3D_input_" + std::to_string(cube_file) + ".dat";
    std::ifstream file("./cornelius_test_data_3D/" + filename);
    // each file contains one cube
    std::string line;
    while (std::getline(file, line)) {
      double tau_local, x_local, y_local;
      std::array<std::array<std::array<double, 2>, 2>, 2> cu;

      std::istringstream iss(line);
      iss >> tau_local >> x_local >> y_local;
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          for (int k = 0; k < 2; k++) {
            iss >> cu[i][j][k];
            cube[i][j][k] = cu[i][j][k];
          }
        }
      }

      // run timing for the new cornelius version
      // start timing
      std::chrono::high_resolution_clock::time_point start =
          std::chrono::high_resolution_clock::now();

      std::unique_ptr<Cornelius> cornelius_ptr(new Cornelius());
      cornelius_ptr->init_cornelius(3, T_cut, dx);
      cornelius_ptr->find_surface_3d(cu);

      // end timing
      std::chrono::high_resolution_clock::time_point end =
          std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> time_span = end - start;
      std::cout << "Time for the new cornelius version: " << time_span.count()
                << " seconds." << std::endl;

      // timing for the old cornelius version
      // start timing
      start = std::chrono::high_resolution_clock::now();

      std::unique_ptr<CorneliusOld> cornelius_old_ptr(new CorneliusOld());
      cornelius_old_ptr->init(3, T_cut, lattice_spacing);
      cornelius_old_ptr->find_surface_3d(cube);

      // end timing
      end = std::chrono::high_resolution_clock::now();
      time_span = end - start;
      std::cout << "Time for the old cornelius version: " << time_span.count()
                << " seconds." << std::endl;

      std::cout << std::endl;

      std::string filename_expected_output =
          "cornelius_3D_expected_output_" + std::to_string(cube_file) + ".dat";
      std::ifstream file_expected_output("./cornelius_test_data_3D/" +
                                         filename_expected_output);
      std::string line_expected_output;
      // read the expected output into vectors
      // tau_center, x_center, y_center, da_tau, da_x, da_y
      std::vector<double> tau_center_expected;
      std::vector<double> x_center_expected;
      std::vector<double> y_center_expected;
      std::vector<double> da_tau_expected;
      std::vector<double> da_x_expected;
      std::vector<double> da_y_expected;
      while (std::getline(file_expected_output, line_expected_output)) {
        std::istringstream iss(line_expected_output);
        double tau_center, x_center, y_center, da_tau, da_x, da_y;
        iss >> tau_center >> x_center >> y_center >> da_tau >> da_x >> da_y;
        tau_center_expected.push_back(tau_center);
        x_center_expected.push_back(x_center);
        y_center_expected.push_back(y_center);
        da_tau_expected.push_back(da_tau);
        da_x_expected.push_back(da_x);
        da_y_expected.push_back(da_y);
      }

      // compare the length of the expected output with get_number_elements of
      // cornelius_ptr
      EXPECT_EQ(cornelius_ptr->get_number_elements(),
                tau_center_expected.size());
      EXPECT_EQ(cornelius_old_ptr->get_Nelements(), tau_center_expected.size());

      // test the values of the expected output with the values of cornelius_ptr
      for (int i = 0; i < cornelius_ptr->get_number_elements(); i++) {
        auto tau_center = (cornelius_ptr->get_centroid_element(i, 0) +
                           tau_local - grid_dt / 2.);
        auto x_center = (cornelius_ptr->get_centroid_element(i, 1) + x_local -
                         grid_dx / 2.);
        auto y_center = (cornelius_ptr->get_centroid_element(i, 2) + y_local -
                         grid_dy / 2.);

        auto da_tau = cornelius_ptr->get_normal_element(i, 0);
        auto da_x = cornelius_ptr->get_normal_element(i, 1);
        auto da_y = cornelius_ptr->get_normal_element(i, 2);

        // results from the old cornelius version
        auto tau_center_old = (cornelius_old_ptr->get_centroid_elem(i, 0) +
                               tau_local - grid_dt / 2.);
        auto x_center_old = (cornelius_old_ptr->get_centroid_elem(i, 1) +
                             x_local - grid_dx / 2.);
        auto y_center_old = (cornelius_old_ptr->get_centroid_elem(i, 2) +
                             y_local - grid_dy / 2.);

        auto da_tau_old = cornelius_old_ptr->get_normal_elem(i, 0);
        auto da_x_old = cornelius_old_ptr->get_normal_elem(i, 1);
        auto da_y_old = cornelius_old_ptr->get_normal_elem(i, 2);

        EXPECT_NEAR(tau_center, tau_center_old, 1e-3);
        EXPECT_NEAR(x_center, x_center_old, 1e-3);
        EXPECT_NEAR(y_center, y_center_old, 1e-3);

        EXPECT_NEAR(da_tau, da_tau_old, 1e-3);
        EXPECT_NEAR(da_x, da_x_old, 1e-3);
        EXPECT_NEAR(da_y, da_y_old, 1e-3);

        EXPECT_NEAR(tau_center, tau_center_expected[i], 1e-3);
        EXPECT_NEAR(x_center, x_center_expected[i], 1e-3);
        EXPECT_NEAR(y_center, y_center_expected[i], 1e-3);
        EXPECT_NEAR(da_tau, da_tau_expected[i], 1e-3);
        EXPECT_NEAR(da_x, da_x_expected[i], 1e-3);
        EXPECT_NEAR(da_y, da_y_expected[i], 1e-3);
        /*std::cout << "tau_center: " << tau_center << " x_center: " << x_center
                  << " y_center: " << y_center << " da_tau: " << da_tau
                  << " da_x: " << da_x << " da_y: " << da_y << std::endl;
        std::cout << "tau_center_expected: " << tau_center_expected[i]
                  << " x_center_expected: " << x_center_expected[i]
                  << " y_center_expected: " << y_center_expected[i]
                  << " da_tau_expected: " << da_tau_expected[i]
                  << " da_x_expected: " << da_x_expected[i]
                  << " da_y_expected: " << da_y_expected[i] << std::endl;*/
      }
    }
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
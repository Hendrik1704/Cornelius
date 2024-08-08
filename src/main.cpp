#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "Cornelius.h"
#include "cornelius_old.h"

int main(int argc, char const *argv[]) {
  int number_of_cubes_to_test =
      10;  // 10 is the maximum, since there are only 10 test files
  int number_of_tests = 100000;  // number of times the test is repeated for one
                                 // cube to get a more accurate time measurement
  bool print_intermediate_times = false;
  // Initialize all the variables for the comparison
  double grid_dt = 0.1;
  double grid_dx = 0.2;
  double grid_dy = 0.2;
  double grid_dz = 0.2;
  double T_cut = 0.16;
  std::array<double, 4> dx_3D = {grid_dt, grid_dx, grid_dy, 1.0};
  std::array<double, 4> dx_4D = {grid_dt, grid_dx, grid_dy, grid_dz};

  const int dim_3D = 3;
  double lattice_spacing_3D[dim_3D];
  lattice_spacing_3D[0] = grid_dt;
  lattice_spacing_3D[1] = grid_dx;
  lattice_spacing_3D[2] = grid_dy;

  const int dim_4D = 4;
  double lattice_spacing_4D[dim_4D];
  lattice_spacing_4D[0] = grid_dt;
  lattice_spacing_4D[1] = grid_dx;
  lattice_spacing_4D[2] = grid_dy;
  lattice_spacing_4D[3] = grid_dz;

  // for the test with the old cornelius version
  double ***cube_3D = new double **[2];
  for (int i = 0; i < 2; i++) {
    cube_3D[i] = new double *[2];
    for (int j = 0; j < 2; j++) {
      cube_3D[i][j] = new double[2];
      for (int k = 0; k < 2; k++)
        cube_3D[i][j][k] = 0.0;
    }
  }

  double ****cube_4D = new double ***[2];
  for (int i = 0; i < 2; i++) {
    cube_4D[i] = new double **[2];
    for (int j = 0; j < 2; j++) {
      cube_4D[i][j] = new double *[2];
      for (int k = 0; k < 2; k++) {
        cube_4D[i][j][k] = new double[2];
        for (int l = 0; l < 2; l++)
          cube_4D[i][j][k][l] = 0.0;
      }
    }
  }

  // Test for 3D
  std::unique_ptr<Cornelius> cornelius_ptr(new Cornelius());
  // print the size of the Cornelius object
  std::cout << "Size of the Cornelius object: " << sizeof(*cornelius_ptr.get())
            << " bytes\n";

  std::unique_ptr<CorneliusOld> cornelius_old_ptr(new CorneliusOld());
  // print the size of the CorneliusOld object
  std::cout << "Size of the CorneliusOld object: "
            << sizeof(*cornelius_old_ptr.get()) << " bytes\n";

  // Perform a time measurement for the new and old cornelius version and
  // average over 10 cubes
  std::chrono::duration<double> elapsed_seconds_new_avg_3D =
      std::chrono::duration<double>::zero();
  std::chrono::duration<double> elapsed_seconds_old_avg_3D =
      std::chrono::duration<double>::zero();

  for (int cube_file = 0; cube_file < number_of_cubes_to_test; cube_file++) {
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
            cube_3D[i][j][k] = cu[i][j][k];
          }
        }
      }

      // timing of the new cornelius version
      auto start = std::chrono::high_resolution_clock::now();

      // Perform this step many times to get a more accurate time measurement
      for (int i = 0; i < number_of_tests; i++) {
        cornelius_ptr->init_cornelius(3, T_cut, dx_3D);
        cornelius_ptr->find_surface_3d(cu);
      }

      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      if (print_intermediate_times) {
        std::cout << "Elapsed time for find_surface_3d new: "
                  << elapsed_seconds.count() << "s\n";
      }
      elapsed_seconds_new_avg_3D += elapsed_seconds;

      // timing of the old cornelius version
      start = std::chrono::high_resolution_clock::now();

      for (int i = 0; i < number_of_tests; i++) {
        cornelius_old_ptr->init(3, T_cut, lattice_spacing_3D);
        cornelius_old_ptr->find_surface_3d(cube_3D);
      }

      end = std::chrono::high_resolution_clock::now();
      elapsed_seconds = end - start;
      if (print_intermediate_times) {
        std::cout << "Elapsed time for find_surface_3d old: "
                  << elapsed_seconds.count() << "s\n";
      }
      elapsed_seconds_old_avg_3D += elapsed_seconds;
    }
  }
  // average the elapsed time
  elapsed_seconds_new_avg_3D /= 10;
  elapsed_seconds_old_avg_3D /= 10;

  std::cout << "Average elapsed time for find_surface_3d new: "
            << elapsed_seconds_new_avg_3D.count() << "s\n";
  std::cout << "Average elapsed time for find_surface_3d old: "
            << elapsed_seconds_old_avg_3D.count() << "s\n";
  std::cout << "Ratio of the average elapsed time for find_surface_3d new/old: "
            << elapsed_seconds_new_avg_3D.count() /
                   elapsed_seconds_old_avg_3D.count()
            << "\n";

  // Test for 4D
  // Perform a time measurement for the new and old cornelius version and
  // average over 10 cubes
  std::chrono::duration<double> elapsed_seconds_new_avg_4D =
      std::chrono::duration<double>::zero();
  std::chrono::duration<double> elapsed_seconds_old_avg_4D =
      std::chrono::duration<double>::zero();

  for (int cube_file = 0; cube_file < number_of_cubes_to_test; cube_file++) {
    std::string filename =
        "cornelius_4D_input_" + std::to_string(cube_file) + ".dat";
    std::ifstream file("./cornelius_test_data_4D/" + filename);
    // each file contains one cube
    std::string line;
    while (std::getline(file, line)) {
      double tau_local, x_local, y_local, z_local;
      std::array<std::array<std::array<std::array<double, 2>, 2>, 2>, 2> cu;

      std::istringstream iss(line);
      iss >> tau_local >> x_local >> y_local >> z_local;
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          for (int k = 0; k < 2; k++) {
            for (int l = 0; l < 2; l++) {
              iss >> cu[i][j][k][l];
              cube_4D[i][j][k][l] = cu[i][j][k][l];
            }
          }
        }
      }

      // timing of the new cornelius version
      auto start = std::chrono::high_resolution_clock::now();

      for (int i = 0; i < number_of_tests; i++) {
        cornelius_ptr->init_cornelius(4, T_cut, dx_4D);
        cornelius_ptr->find_surface_4d(cu);
      }

      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      if (print_intermediate_times) {
        std::cout << "Elapsed time for find_surface_4d new: "
                  << elapsed_seconds.count() << "s\n";
      }
      elapsed_seconds_new_avg_4D += elapsed_seconds;

      // timing of the old cornelius version
      start = std::chrono::high_resolution_clock::now();

      for (int i = 0; i < number_of_tests; i++) {
        cornelius_old_ptr->init(4, T_cut, lattice_spacing_4D);
        cornelius_old_ptr->find_surface_4d(cube_4D);
      }

      end = std::chrono::high_resolution_clock::now();
      elapsed_seconds = end - start;
      if (print_intermediate_times) {
        std::cout << "Elapsed time for find_surface_4d old: "
                  << elapsed_seconds.count() << "s\n";
      }
      elapsed_seconds_old_avg_4D += elapsed_seconds;
    }
  }
  // average the elapsed time
  elapsed_seconds_new_avg_4D /= 10;
  elapsed_seconds_old_avg_4D /= 10;

  std::cout << "Average elapsed time for find_surface_4d new: "
            << elapsed_seconds_new_avg_4D.count() << "s\n";
  std::cout << "Average elapsed time for find_surface_4d old: "
            << elapsed_seconds_old_avg_4D.count() << "s\n";
  std::cout << "Ratio of the average elapsed time for find_surface_4d new/old: "
            << elapsed_seconds_new_avg_4D.count() /
                   elapsed_seconds_old_avg_4D.count()
            << "\n";

  return 0;
}

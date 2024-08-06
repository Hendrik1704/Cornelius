#include "Cornelius.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

Cornelius::Cornelius() {
  number_elements = 0;
  cube_dimension = 0;
  initialized = false;
  print_initialized = false;
}

Cornelius::~Cornelius() {
  // Close the file if it is open
  if (output_file.is_open()) {
    output_file.close();
  }
}

void Cornelius::init_cornelius(int dimension, double new_value,
                               std::array<double, DIM>& new_dx) {
  cube_dimension = dimension;
  value = new_value;
  for (int i = 0; i < DIM; i++) {
    if (i < DIM - cube_dimension) {
      dx[i] = 1;
    } else {
      dx[i] = new_dx[i - (DIM - cube_dimension)];
    }
  }
  initialized = true;
}

void Cornelius::init_print_cornelius(std::string filename) {
  output_file.open(filename);
  print_initialized = true;
}

void Cornelius::find_surface_2d(
    std::array<std::array<double, STEPS>, STEPS> cu) {
  if (!initialized || cube_dimension != 2) {
    std::cerr << "Cornelius not initialized for 2D case." << std::endl;
    exit(1);
  }
  std::array<int, 2> c_i;
  std::array<double, 2> c_v;
  c_i[0] = 0;
  c_i[1] = 1;
  c_v[0] = 0;
  c_v[1] = 0;
  cube_2d.init_square(cu, c_i, c_v, dx);
  cube_2d.construct_lines(value);
  number_elements = cube_2d.get_number_lines();
  std::array<Line, Square::MAX_LINES> lines = cube_2d.get_lines();
  for (int i = 0; i < number_elements; i++) {
    for (int j = 0; j < DIM; j++) {
      normals[i][j] = lines[i].get_normal()[j];
      centroids[i][j] = lines[i].get_centroid()[j];
    }
  }
}

void Cornelius::find_surface_3d(
    std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS> cu) {
  std::array<double, DIM> position = {0};
  surface_3d(cu, position, false);
}

void Cornelius::find_surface_3d_print(
    std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS> cu,
    std::array<double, DIM> position) {
  surface_3d(cu, position, true);
}

void Cornelius::surface_3d(
    std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS> cu,
    std::array<double, DIM> position, bool do_print) {
  if (!initialized || cube_dimension != 3) {
    std::cerr << "Cornelius not initialized for 3D case." << std::endl;
    exit(1);
  }
  // Check if the cube actually contains surface elements.
  // If all or none of the elements are below the criterion, no surface
  // elements exist.
  int value_greater = std::accumulate(
      cu.begin(), cu.end(), 0, [this](int sum, const auto& array2d) {
        return sum + std::accumulate(
                         array2d.begin(), array2d.end(), 0,
                         [this](int sum2, const auto& array1d) {
                           return sum2 +
                                  std::count_if(array1d.begin(), array1d.end(),
                                                [this](double element) {
                                                  return element >= this->value;
                                                });
                         });
      });
  if (value_greater == 0 || value_greater == 8) {
    // No elements in this cube
    number_elements = 0;
    return;
  }
  // This cube has surface elements, start constructing the cube
  int c_i = 0;
  double c_v = 0.0;
  cube_3d.init_cube(cu, c_i, c_v, dx);
  // Find the elements
  cube_3d.construct_polygons(value);
  // Obtain the information about the elements
  number_elements = cube_3d.get_number_polygons();
  std::array<Polygon, Cube::MAX_POLY> polygons = cube_3d.get_polygons();
  for (int i = 0; i < number_elements; i++) {
    // Always work with 4 dimensions
    for (int j = 0; j < DIM; j++) {
      centroids[i][j] = polygons[i].get_centroid()[j];
      normals[i][j] = polygons[i].get_normal()[j];
    }
    // If the triangles should be printed, print them
    if (print_initialized && do_print) {
      polygons[i].print(output_file, position);
    }
  }
}

void Cornelius::find_surface_4d(
    std::array<std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>,
               STEPS>
        cu) {
  if (!initialized || cube_dimension != 4) {
    std::cerr << "Cornelius not initialized for 4D case." << std::endl;
    exit(1);
  }
  // Check if the cube actually contains surface elements.
  // If all or none of the elements are below the criterion, no surface
  // elements exist.
  int number_elements = std::accumulate(
      cu.begin(), cu.end(), 0, [this](int sum, const auto& array3d) {
        return sum +
               std::accumulate(
                   array3d.begin(), array3d.end(), 0,
                   [this](int sum2, const auto& array2d) {
                     return sum2 +
                            std::accumulate(
                                array2d.begin(), array2d.end(), 0,
                                [this](int sum3, const auto& array1d) {
                                  return sum3 +
                                         std::count_if(
                                             array1d.begin(), array1d.end(),
                                             [this](double element) {
                                               return element >= this->value;
                                             });
                                });
                   });
      });
  if (number_elements == 0 || number_elements == 16) {
    // No elements in this cube
    number_elements = 0;
    return;
  }
  // This cube has surface elements, start constructing the cube
  cube_4d.init_hypercube(cu, dx);
  // Find the elements
  cube_4d.construct_polyhedra(value);
  // Obtain the information about the elements
  number_elements = cube_4d.get_number_polyhedra();
  std::array<Polyhedron, Hypercube::MAX_POLY> polyhedra =
      cube_4d.get_polyhedra();
  for (int i = 0; i < number_elements; i++) {
    for (int j = 0; j < DIM; j++) {
      centroids[i][j] = polyhedra[i].get_centroid()[j];
      normals[i][j] = polyhedra[i].get_normal()[j];
    }
  }
}

int Cornelius::get_number_elements() { return number_elements; }

std::vector<std::vector<double>> Cornelius::get_normals_4d() {
  std::vector<std::vector<double>> normals_vector(number_elements);
  for (int i = 0; i < number_elements; i++) {
    std::vector<double> normal(DIM);
    for (int j = 0; j < DIM; j++) {
      normal[j] = normals[i][j];
    }
    normals_vector[i] = normal;
  }
  return normals_vector;
}

std::vector<std::vector<double>> Cornelius::get_centroids_4d() {
  std::vector<std::vector<double>> centroids_vector(number_elements);
  for (int i = 0; i < number_elements; i++) {
    std::vector<double> centroid(DIM);
    for (int j = 0; j < DIM; j++) {
      centroid[j] = centroids[i][j];
    }
    centroids_vector[i] = centroid;
  }
  return centroids_vector;
}

std::vector<std::vector<double>> Cornelius::get_normals() {
  std::vector<std::vector<double>> normals_vector(number_elements);
  for (int i = 0; i < number_elements; i++) {
    std::vector<double> normal(cube_dimension);
    for (int j = 0; j < cube_dimension; j++) {
      normal[j] = normals[i][j + (DIM - cube_dimension)];
    }
    normals_vector[i] = normal;
  }
  return normals_vector;
}

std::vector<std::vector<double>> Cornelius::get_centroids() {
  std::vector<std::vector<double>> centroids_vector(number_elements);
  for (int i = 0; i < number_elements; i++) {
    std::vector<double> centroid(cube_dimension);
    for (int j = 0; j < cube_dimension; j++) {
      centroid[j] = centroids[i][j + (DIM - cube_dimension)];
    }
    centroids_vector[i] = centroid;
  }
  return centroids_vector;
}

double Cornelius::get_centroid_element(int index_surface_element,
                                       int element_centroid) {
  if ((index_surface_element >= number_elements) ||
      (element_centroid >= cube_dimension)) {
    std::cerr << "Cornelius error: asking for an element which does not exist."
              << std::endl;
    exit(1);
  }
  return centroids[index_surface_element]
                  [element_centroid + (DIM - cube_dimension)];
}

double Cornelius::get_normal_element(int index_surface_element,
                                     int element_normal) {
  if ((index_surface_element >= number_elements) ||
      (element_normal >= cube_dimension)) {
    std::cerr << "Cornelius error: asking for an element which does not exist."
              << std::endl;
    exit(1);
  }
  return normals[index_surface_element]
                [element_normal + (DIM - cube_dimension)];
}
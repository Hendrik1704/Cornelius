#include "Polyhedron.h"

#include <iostream>

Polyhedron::Polyhedron() {
  polygons.clear();
  polygons.reserve(MAX_POLYGONS);
  polygons.emplace_back();  // Default to construct 1 Polygon
}

Polyhedron::~Polyhedron() = default;

bool Polyhedron::add_polygon(Polygon& new_polygon, bool perform_no_check) {
  // For the first polygon, we don't need to check
  if (number_polygons == 0 || perform_no_check) {
    // Ensure there's space in the vector
    if (number_polygons >= polygons.size()) {
      polygons.emplace_back();  // Add a new Polygon if needed
    }
    polygons[number_polygons++] = new_polygon;
    number_tetrahedrons += new_polygon.get_number_lines();
    return true;
  } else {
    // Check if the current polygon is connected to the last polygon, since
    // the polygons are ordered
    for (int i = 0; i < number_polygons; i++) {
      const int number_lines1 = new_polygon.get_number_lines();
      // Check if the lines are equal
      for (int j = 0; j < number_lines1; j++) {
        for (int k = 0; k < polygons[i].get_number_lines(); k++) {
          if (lines_are_connected(new_polygon.get_lines()[j],
                                  polygons[i].get_lines()[k])) {
            // Ensure there's space in the vector
            if (number_polygons >= polygons.size()) {
              polygons.emplace_back();  // Add a new Polygon if needed
            }
            polygons[number_polygons++] = new_polygon;
            number_tetrahedrons += number_lines1;
            return true;
          }
        }
      }
    }
    // If the polygon is not connected to the last polygon, return false
    return false;
  }
}

void Polyhedron::calculate_centroid() {
  // Array of 0s to store the mean values
  std::array<double, DIM> mean_values = {0};

  // Determine the mean values of the corner points, all points appear twice
  for (int i = 0; i < number_polygons; i++) {
    auto& lines = polygons[i].get_lines();
    const int n_lines = polygons[i].get_number_lines();
    for (int j = 0; j < n_lines; j++) {
      const auto& start_point = lines[j].get_start_point();
      const auto& end_point = lines[j].get_end_point();
      for (int k = 0; k < DIM; k++) {
        mean_values[k] += start_point[k] + end_point[k];
      }
    }
  }
  for (int k = 0; k < DIM; k++) {
    mean_values[k] /= (2.0 * number_tetrahedrons);
  }

  std::array<double, DIM> sum_up = {0};
  double sum_down = 0.0;
  // Loop over all polygons
  for (int i = 0; i < number_polygons; ++i) {
    auto& lines = polygons[i].get_lines();
    const int n_lines = polygons[i].get_number_lines();
    const auto& cent = polygons[i].get_centroid();
    // Loop over all lines in the polygon
    for (int j = 0; j < n_lines; j++) {
      const auto& start_point = lines[j].get_start_point();
      const auto& end_point = lines[j].get_end_point();
      // Center of mass of the tetrahedron
      for (int k = 0; k < DIM; k++) {
        cm_i[k] =
            (start_point[k] + end_point[k] + cent[k] + mean_values[k]) * 0.25;
        // Compute the defining vectors of the tetrahedron
        a[k] = start_point[k] - mean_values[k];
        b[k] = end_point[k] - mean_values[k];
        c[k] = cent[k] - mean_values[k];
      }
      // Calculate the volume of the tetrahedron
      tetrahedron_volume(a, b, c, n);
      double V_i = 0.0;
      for (int k = 0; k < DIM; ++k) {
        V_i += n[k] * n[k];
      }
      V_i = std::sqrt(V_i);
      // Add the contribution to the sum
      for (int i = 0; i < DIM; i++) {
        sum_up[i] += V_i * cm_i[i];
      }
      sum_down += V_i;
    }
  }
  // Centroid of the polygon is the volume weighted average of the individual
  // tetrahedrons
  for (int i = 0; i < DIM; i++) {
    centroid[i] = sum_up[i] / sum_down;
  }
  centroid_calculated = true;
}

void Polyhedron::calculate_normal() {
  if (!centroid_calculated) {
    calculate_centroid();
  }
  int index_tetrahedron = 0;
  // Loop over all polygons
  for (int i = 0; i < number_polygons; i++) {
    auto& lines = polygons[i].get_lines();
    const auto& cent = polygons[i].get_centroid();
    // Loop over all lines in the polygon
    for (int j = 0; j < polygons[i].get_number_lines(); j++) {
      const auto& start_point = lines[j].get_start_point();
      const auto& end_point = lines[j].get_end_point();

      // Construct vector pointing outside the polygon
      const auto& o = lines[j].get_outside_point();

      // Compute the defining vectors of the tetrahedron
      for (int k = 0; k < DIM; ++k) {
        a[k] = start_point[k] - centroid[k];
        b[k] = end_point[k] - centroid[k];
        c[k] = cent[k] - centroid[k];

        Vout[k] = o[k] - centroid[k];
      }
      // Normal vector is calculated with the same function as the volume
      tetrahedron_volume(a, b, c, normals[index_tetrahedron]);

      // Check if the normal is pointing in the correct direction
      flip_normal_if_needed(normals[index_tetrahedron], Vout);
      index_tetrahedron++;
    }
  }
  // The normal is the sum of the normals of the tetrahedrons
  for (int i = 0; i < DIM; i++) {
    normal[i] = 0.0;
    for (int j = 0; j < number_tetrahedrons; j++) {
      normal[i] += normals[j][i];
    }
  }
  normal_calculated = true;
}
#include "Polyhedron.h"

#include <iostream>

Polyhedron::Polyhedron() {
  polygons.reserve(MAX_POLYGONS);
  polygons.emplace_back();  // Default to construct 1 Polygon
}

Polyhedron::~Polyhedron() = default;

void Polyhedron::init_polyhedron() {
  // Reset the number of polygons and tetrahedrons in the polyhedron
  number_polygons = number_tetrahedrons = 0;
  // Set the flags for normal and centroid calculations to false
  normal_calculated = centroid_calculated = false;
}

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

bool Polyhedron::lines_are_connected(Line& line1, Line& line2) {
  // Get the start and end points of the lines
  const auto& start_point1 = line1.get_start_point();
  const auto& end_point1 = line1.get_end_point();
  const auto& start_point2 = line2.get_start_point();

  // Check if any point pairs are close enough
  double difference1 = 0.0;
  double difference2 = 0.0;
  for (int i = 0; i < DIM; i++) {
    difference1 += std::abs(start_point1[i] - start_point2[i]);
    difference2 += std::abs(end_point1[i] - start_point2[i]);
    if (difference1 > EPSILON && difference2 > EPSILON) {
      return false;
    }
  }
  return true;
}

void Polyhedron::tetrahedron_volume(std::array<double, DIM>& v1,
                                    std::array<double, DIM>& v2,
                                    std::array<double, DIM>& v3,
                                    std::array<double, DIM>& n) {
  // Calculate the volume of the tetrahedron
  const double bc01 = v2[0] * v3[1] - v2[1] * v3[0];
  const double bc02 = v2[0] * v3[2] - v2[2] * v3[0];
  const double bc03 = v2[0] * v3[3] - v2[3] * v3[0];
  const double bc12 = v2[1] * v3[2] - v2[2] * v3[1];
  const double bc13 = v2[1] * v3[3] - v2[3] * v3[1];
  const double bc23 = v2[2] * v3[3] - v2[3] * v3[2];
  n[0] = (v1[1] * bc23 - v1[2] * bc13 + v1[3] * bc12) * INV_SIX;
  n[1] = -(v1[0] * bc23 - v1[2] * bc03 + v1[3] * bc02) * INV_SIX;
  n[2] = (v1[0] * bc13 - v1[1] * bc03 + v1[3] * bc01) * INV_SIX;
  n[3] = -(v1[0] * bc12 - v1[1] * bc02 + v1[2] * bc01) * INV_SIX;
}

void Polyhedron::calculate_centroid() {
  // Array of 0s to store the mean values
  std::array<double, DIM> mean_values = {0};

  // Determine the mean values of the corner points, all points appear twice
  for (int i = 0; i < number_polygons; i++) {
    auto& lines = polygons[i].get_lines();
    for (int j = 0; j < polygons[i].get_number_lines(); j++) {
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

  // Temporary arrays
  std::array<double, DIM> sum_up = {0};
  double sum_down = 0.0;
  // Loop over all polygons
  for (int i = 0; i < number_polygons; ++i) {
    auto& lines = polygons[i].get_lines();
    const auto& cent = polygons[i].get_centroid();
    // Loop over all lines in the polygon
    for (int j = 0; j < polygons[i].get_number_lines(); j++) {
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
      const double V_i =
          std::sqrt(std::inner_product(n.begin(), n.end(), n.begin(), 0.0));
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
      // Compute defining vectors of the tetrahedron
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
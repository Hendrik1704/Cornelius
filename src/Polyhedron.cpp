#include "Polyhedron.h"

#include <iostream>

Polyhedron::Polyhedron() {}

Polyhedron::~Polyhedron() {}

void Polyhedron::init_polyhedron() {
  // Reset the number of polygons and tetrahedrons in the polyhedron
  number_polygons = 0;
  number_tetrahedrons = 0;
  // Set the flags for normal and centroid calculations to false
  normal_calculated = false;
  centroid_calculated = false;
}

bool Polyhedron::add_polygon(Polygon& new_polygon, bool perform_no_check) {
  // For the first polygon, we don't need to check
  if (number_polygons == 0 || perform_no_check) {
    polygons[number_polygons++] = new_polygon;
    number_tetrahedrons += new_polygon.get_number_lines();
    return true;
  } else {
    // Check if the current polygon is connected to the last polygon, since
    // the polygons are ordered
    for (int i = 0; i < number_polygons; i++) {
      int number_lines1 = new_polygon.get_number_lines();
      int number_lines2 = polygons[i].get_number_lines();
      auto& lines1 = new_polygon.get_lines();
      auto& lines2 = polygons[i].get_lines();
      // Check if the lines are equal
      for (int j = 0; j < number_lines1; j++) {
        for (int k = 0; k < number_lines2; k++) {
          if (lines_are_connected(lines1[j], lines2[k])) {
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
  constexpr double epsilon = 1e-10;
  // Get the start and end points of the lines
  auto& start_point1 = line1.get_start_point();
  auto& end_point1 = line1.get_end_point();
  auto& start_point2 = line2.get_start_point();

  // Check if any point pairs are close enough
  bool start_equal = true;
  bool end_equal = true;
  for (int i = 0; i < DIM; ++i) {
    if (std::abs(start_point1[i] - start_point2[i]) > epsilon) {
      start_equal = false;
    }
    if (std::abs(end_point1[i] - start_point2[i]) > epsilon) {
      end_equal = false;
    }
    // If both differences are greater than epsilon, exit early
    if (!start_equal && !end_equal) {
      return false;
    }
  }
  // If either start or end points are close enough, return true
  return start_equal || end_equal;
}

void Polyhedron::tetrahedron_volume(std::array<double, DIM>& v1,
                                    std::array<double, DIM>& v2,
                                    std::array<double, DIM>& v3,
                                    std::array<double, DIM>& n) {
  // Calculate the volume of the tetrahedron
  double bc01 = v2[0] * v3[1] - v2[1] * v3[0];
  double bc02 = v2[0] * v3[2] - v2[2] * v3[0];
  double bc03 = v2[0] * v3[3] - v2[3] * v3[0];
  double bc12 = v2[1] * v3[2] - v2[2] * v3[1];
  double bc13 = v2[1] * v3[3] - v2[3] * v3[1];
  double bc23 = v2[2] * v3[3] - v2[3] * v3[2];
  n[0] = 1.0 / 6.0 * (v1[1] * bc23 - v1[2] * bc13 + v1[3] * bc12);
  n[1] = -1.0 / 6.0 * (v1[0] * bc23 - v1[2] * bc03 + v1[3] * bc02);
  n[2] = 1.0 / 6.0 * (v1[0] * bc13 - v1[1] * bc03 + v1[3] * bc01);
  n[3] = -1.0 / 6.0 * (v1[0] * bc12 - v1[1] * bc02 + v1[2] * bc01);
}

void Polyhedron::calculate_centroid() {
  // Array of 0s to store the mean values
  std::array<double, DIM> mean_values = {0};

  // Determine the mean values of the corner points, all points appear twice
  for (int k = 0; k < DIM; k++) {
    mean_values[k] =
        std::transform_reduce(
            polygons.begin(), polygons.begin() + number_polygons, 0.0,
            std::plus<>(),
            [k](Polygon& polygon) {
              return std::transform_reduce(
                  polygon.get_lines().begin(),
                  polygon.get_lines().begin() + polygon.get_number_lines(), 0.0,
                  std::plus<>(), [k](Line& line) {
                    return line.get_start_point()[k] + line.get_end_point()[k];
                  });
            }) /
        (2.0 * number_tetrahedrons);
  }
  // Temporary arrays
  std::array<double, DIM> a = {0};
  std::array<double, DIM> b = {0};
  std::array<double, DIM> c = {0};
  std::array<double, DIM> n = {0};
  std::array<double, DIM> cm_i= {0};
  std::array<double, DIM> sum_up = {0};
  double sum_down = 0.0;
  // Loop over all polygons
  for (int i = 0; i < number_polygons; ++i) {
    auto& polygon = polygons[i];
    int number_lines = polygon.get_number_lines();
    auto& lines = polygon.get_lines();
    auto& centroid = polygon.get_centroid();
    // Loop over all lines in the polygon
    for (int j = 0; j < number_lines; j++) {
      auto& start_point = lines[j].get_start_point();
      auto& end_point = lines[j].get_end_point();
      // Center of mass of the tetrahedron
      for (int k = 0; k < DIM; k++) {
        cm_i[k] =
            (start_point[k] + end_point[k] + centroid[k] + mean_values[k]) /
            4.0;
            // Compute the defining vectors of the tetrahedron
        a[k] = start_point[k] - mean_values[k];
        b[k] = end_point[k] - mean_values[k];
        c[k] = centroid[k] - mean_values[k];
      }
      // Compute the defining vectors of the tetrahedron
      // std::transform(start_point.begin(), start_point.end(),
      //                mean_values.begin(), a.begin(), std::minus<>());
      // std::transform(end_point.begin(), end_point.end(), mean_values.begin(),
      //                b.begin(), std::minus<>());
      // std::transform(centroid.begin(), centroid.end(), mean_values.begin(),
      //                c.begin(), std::minus<>());

      // Calculate the volume of the tetrahedron
      tetrahedron_volume(a, b, c, n);
      double V_i =
          std::sqrt(std::inner_product(n.begin(), n.end(), n.begin(), 0.0));
      // Add the contribution to the sum
      std::transform(sum_up.begin(), sum_up.end(), cm_i.begin(), sum_up.begin(),
                     [V_i](double sum, double cm) { return sum + V_i * cm; });
      sum_down += V_i;
    }
  }
  // Centroid of the polygon is the volume weighted average of the individual
  // tetrahedrons
  std::transform(sum_up.begin(), sum_up.end(), centroid.begin(),
                 [sum_down](double sum) { return sum / sum_down; });
  centroid_calculated = true;
}

void Polyhedron::calculate_normal() {
  if (!centroid_calculated) {
    calculate_centroid();
  }
  // Temporary arrays
  std::array<double, DIM> Vout = {0};
  std::array<double, DIM> a = {0};
  std::array<double, DIM> b = {0};
  std::array<double, DIM> c = {0};
  std::array<std::array<double, DIM>, MAX_POLYGONS*24> normals;
  for (auto& n : normals) {
    std::fill(n.begin(), n.end(), 0.0);
  }
  int index_tetrahedron = 0;
  // Loop over all polygons
  for (int i = 0; i < number_polygons; i++) {
    auto& polygon = polygons[i];
    int number_lines = polygon.get_number_lines();
    auto& lines = polygon.get_lines();
    auto& cent = polygon.get_centroid();
    // Loop over all lines in the polygon
    for (int j = 0; j < number_lines; j++) {
      auto& start_point = lines[j].get_start_point();
      auto& end_point = lines[j].get_end_point();

      // Construct vector pointing outside the polygon
      auto& o = lines[j].get_outside_point();
      
      // Compute the defining vectors of the tetrahedron
      // Compute defining vectors of the tetrahedron
      for (int k = 0; k < DIM; ++k) {
        a[k] = start_point[k] - centroid[k];
        b[k] = end_point[k] - centroid[k];
        c[k] = cent[k] - centroid[k];

        Vout[k] = o[k] - centroid[k];
      }
      // std::transform(start_point.begin(), start_point.end(), centroid.begin(),
      //                a.begin(), std::minus<>());
      // std::transform(end_point.begin(), end_point.end(), centroid.begin(),
      //                b.begin(), std::minus<>());
      // std::transform(cent.begin(), cent.end(), centroid.begin(), c.begin(),
      //                std::minus<>());
      // Normal vector is calculated with the same function as the volume
      tetrahedron_volume(a, b, c, normals[index_tetrahedron]);
      // // Construct the vector pointing outside the polygon
      // auto& o = lines[j].get_outside_point();
      // std::transform(o.begin(), o.end(), centroid.begin(), Vout.begin(),
      //                std::minus<>());
      // Check if the normal is pointing in the correct direction
      flip_normal_if_needed(normals[index_tetrahedron], Vout);
      index_tetrahedron++;
    }
  }
  // The normal is the sum of the normals of the tetrahedrons
  std::fill(normal.begin(), normal.end(), 0.0);
  for (int i = 0; i < number_tetrahedrons; i++) {
    std::transform(normal.begin(), normal.end(), normals[i].begin(),
                   normal.begin(), std::plus<>());
  }
  normal_calculated = true;
}
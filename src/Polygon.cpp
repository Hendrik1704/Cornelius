#include "Polygon.h"

Polygon::Polygon() {}

Polygon::~Polygon() = default;

void Polygon::init_polygon(int new_const_i) {
  // Copy the new value into the class variable
  const_i = new_const_i;
  // Fix the indices which are not constant
  switch (new_const_i) {
    case 0:
      x1 = 1;
      x2 = 2;
      x3 = 3;
      break;
    case 1:
      x1 = 0;
      x2 = 2;
      x3 = 3;
      break;
    case 2:
      x1 = 0;
      x2 = 1;
      x3 = 3;
      break;
    case 3:
      x1 = 0;
      x2 = 1;
      x3 = 2;
      break;
    default:
      break;
  }

  // Set the flags for normal and centroid calculations to false
  normal_calculated = centroid_calculated = false;

  // Reset the number of lines in the polygon
  number_lines = 0;
}

bool Polygon::add_line(Line& new_line, bool perform_no_check) {
  // For the first line, we don't need to check
  if (number_lines == 0 || perform_no_check) {
    lines[number_lines++] = new_line;
    return true;
  } else {
    // Check if the current line is connected to the last line, since
    // the lines are ordered
    const auto& start_point = new_line.get_start_point();
    const auto& end_point = new_line.get_end_point();
    const auto& last_end_point = lines[number_lines - 1].get_end_point();

    double difference1 = calc_difference(start_point, last_end_point);
    double difference2 = calc_difference(end_point, last_end_point);

    // If start or end point is the same as the end point of previous line,
    // line is connected to polygon and it is added
    if (difference1 < EPSILON || difference2 < EPSILON) {
      // We always want that the start point is connected to end so we
      // flip the start and end in the new line if needed
      if (difference2 < EPSILON) {
        new_line.flip_start_end();
      }
      lines[number_lines++] = new_line;
      return true;
    } else {
      // In this case, the line is not connected to the polygon
      return false;
    }
  }
}

void Polygon::calculate_centroid() {
  // Array of 0s to store the mean values
  std::array<double, DIM> mean_values = {0};

  // Determine the mean values of the corner points, all points appear twice
  for (int i = 0; i < number_lines; i++) {
    const auto& start_point = lines[i].get_start_point();
    const auto& end_point = lines[i].get_end_point();
    for (int k = 0; k < DIM; ++k) {
      mean_values[k] += start_point[k] + end_point[k];
    }
  }
  for (int i = 0; i < DIM; i++) {
    mean_values[i] /= (2.0 * number_lines);
  }
  // In the case there are only 3 lines, the centroid is the mean of the corner
  // points and the Polygon is on a plane
  if (number_lines == 3) {
    centroid = mean_values;
    centroid_calculated = true;
    return;
  }

  // In the case where there are more than 3 lines, we form triangles from the
  // lines and the mean point

  // Array to store the areas of the triangles
  std::array<double, DIM> sum_up = {0};
  double sum_down = 0.0;  // Sum of the areas of the triangles
  for (int l = 0; l < number_lines; l++) {
    const auto& line_start = lines[l].get_start_point();
    const auto& line_end = lines[l].get_end_point();
    // Form the vectors of the triangle
    for (int j = 0; j < DIM; j++) {
      a[j] = line_start[j] - mean_values[j];
      b[j] = line_end[j] - mean_values[j];
      triangle_centroid[j] =
          (line_start[j] + line_end[j] + mean_values[j]) / 3.0;
    }
    // Calculate the area of the triangle
    double A_l = 0.5 * std::sqrt(std::pow(a[x2] * b[x3] - a[x3] * b[x2], 2.0) +
                                 std::pow(a[x1] * b[x3] - a[x3] * b[x1], 2.0) +
                                 std::pow(a[x2] * b[x1] - a[x1] * b[x2], 2.0));
    // Store the area and update the total area
    for (int i = 0; i < DIM; ++i) {
      sum_up[i] += A_l * triangle_centroid[i];
    }
    sum_down += A_l;
  }
  // Determine centroid as a weighted average of the centroids of the triangles
  for (int i = 0; i < DIM; i++) {
    centroid[i] = sum_up[i] / sum_down;
  }
  centroid_calculated = true;
}

void Polygon::calculate_normal() {
  // Check if the centroid is calculated
  if (!centroid_calculated) {
    calculate_centroid();
  }
  // Find the normal vector for all the triangles formed from one edge of the
  // centroid
  for (int i = 0; i < number_lines; i++) {
    for (int j = 0; j < DIM; j++) {
      normals[i][j] = 0.0;
    }
  }
  std::array<double, DIM> v_out = {0};  // point always outside
  // Loop over all triangles
  for (int i = 0; i < number_lines; i++) {
    const auto& l1 = lines[i].get_start_point();
    const auto& l2 = lines[i].get_end_point();

    for (int j = 0; j < DIM; j++) {
      a[j] = l1[j] - centroid[j];
      b[j] = l2[j] - centroid[j];
    }
    // Calculate the normal vector of the triangle with cross product
    normals[i][x1] = 0.5 * (a[x2] * b[x3] - a[x3] * b[x2]);
    normals[i][x2] = -0.5 * (a[x1] * b[x3] - a[x3] * b[x1]);
    normals[i][x3] = 0.5 * (a[x1] * b[x2] - a[x2] * b[x1]);
    normals[i][const_i] = 0.0;

    // Construct the vector pointing outside the polygon
    const auto& o = lines[i].get_outside_point();
    for (int j = 0; j < DIM; ++j) {
      v_out[j] = o[j] - centroid[j];
    }
    // Check if the normal is pointing in the correct direction
    flip_normal_if_needed(normals[i], v_out);
  }
  // The normal is the sum of the normals of the triangles
  for (int i = 0; i < DIM; i++) {
    normal[i] = 0.0;
  }
  for (int i = 0; i < number_lines; i++) {
    for (int j = 0; j < DIM; ++j) {
      normal[j] += normals[i][j];
    }
  }
  normal_calculated = true;
}

void Polygon::print(std::ofstream& file, std::array<double, DIM> position) {
  // Print the polygon to the file
  for (int i = 0; i < number_lines; i++) {
    const auto& p1 = lines[i].get_start_point();
    const auto& p2 = lines[i].get_end_point();
    file << position[x1] + p1[x1] << " " << position[x2] + p1[x2] << " "
         << position[x3] + p1[x3] << " " << position[x1] + p2[x1] << " "
         << position[x2] + p2[x2] << " " << position[x3] + p2[x3] << " "
         << position[x1] + centroid[x1] << " " << position[x2] + centroid[x2]
         << " " << position[x3] + centroid[x3] << std::endl;
  }
}

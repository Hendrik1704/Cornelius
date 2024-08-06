
#include "Polygon.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

Polygon::Polygon() {
  // Initialize a GeneralGeometryElement object
  GeneralGeometryElement();
}

Polygon::~Polygon() {}

void Polygon::init_polygon(int new_const_i) {
  // Copy the new value into the class variable
  const_i = new_const_i;
  // Fix the indices which are not constant
  x1 = -1;
  x2 = -1;
  x3 = -1;
  for (int i = 0; i < DIM; i++) {
    if (i != new_const_i) {
      if (x1 < 0) {
        x1 = i;
      } else if (x2 < 0) {
        x2 = i;
      } else {
        x3 = i;
      }
    }
  }

  // Set the flags for normal and centroid calculations to false
  normal_calculated = false;
  centroid_calculated = false;
  // Reset the number of lines in the polygon
  number_lines = 0;
}

bool Polygon::add_line(Line& new_line, bool perform_no_check) {
  double epsilon = 1e-10;
  // For the first line, we don't need to check
  if (number_lines == 0 || perform_no_check) {
    lines[number_lines] = new_line;
    number_lines++;
    return true;
  } else {
    // Check if the current line is connected to the last line, since
    // the lines are ordered
    std::array<double, DIM> start_point = new_line.get_start_point();
    std::array<double, DIM> end_point = new_line.get_end_point();
    std::array<double, DIM> last_end_point =
        lines[number_lines - 1].get_end_point();

    // Lambda function to calculate the difference between two points
    auto calc_difference = [](std::array<double, DIM>& p1,
                              std::array<double, DIM>& p2) {
      return std::transform_reduce(
          p1.begin(), p1.end(), p2.begin(), 0.0, std::plus<double>(),
          [](double a, double b) { return std::abs(a - b); });
    };

    double difference1 = calc_difference(start_point, last_end_point);
    double difference2 = calc_difference(end_point, last_end_point);

    // If start or end point is the same as the end point of previous line,
    // line is connected to polygon and it is added
    if (difference1 < epsilon || difference2 < epsilon) {
      // We always want that the start point is connected to end so we
      // flip the start and end in the new line if needed
      if (difference2 < epsilon) {
        new_line.flip_start_end();
      }
      lines[number_lines] = new_line;
      number_lines++;
      return true;
    } else {
      // In this case, the line is not connected to the polygon
      return false;
    }
  }
}

int Polygon::get_number_lines() { return number_lines; }

void Polygon::calculate_centroid() {
  // Array of 0s to store the mean values
  std::array<double, DIM> mean_values = {0};

  // Determine the mean values of the corner points, all points appear twice
  for (int k = 0; k < DIM; ++k) {
    mean_values[k] =
        std::transform_reduce(
            lines.begin(), lines.begin() + number_lines, 0.0, std::plus<>(),
            [k](Line line) {
              return line.get_start_point()[k] + line.get_end_point()[k];
            }) /
        (2.0 * number_lines);
  }
  // In the case there are only 3 lines, the centroid is the mean of the corner
  // points and the Polygon is on a plane
  if (number_lines == 3) {
    for (int j = 0; j < DIM; j++) {
      centroid[j] = mean_values[j];
    }
    centroid_calculated = true;
    return;
  }

  // In the case where there are more than 3 lines, we form triangles from the
  // lines and the mean point

  // Array to store the areas of the triangles
  std::array<double, DIM> sum_up = {0};
  double sum_down = 0.0;  // Sum of the areas of the triangles
  // Arrays a and b to store the vectors of the triangles
  std::array<double, DIM> a = {0};
  std::array<double, DIM> b = {0};
  // Centroid of the triangle (always on the plane)
  std::array<double, DIM> triangle_centroid = {0};
  for (int l = 0; l < number_lines; l++) {
    // Form the vectors of the triangle
    for (int j = 0; j < DIM; j++) {
      a[j] = lines[l].get_start_point()[j] - mean_values[j];
      b[j] = lines[l].get_end_point()[j] - mean_values[j];
      triangle_centroid[j] = (lines[l].get_start_point()[j] +
                              lines[l].get_end_point()[j] + mean_values[j]) /
                             3.0;
    }
    // Calculate the area of the triangle
    double A_l = 0.5 * std::sqrt(std::pow(a[x2] * b[x3] - a[x3] * b[x2], 2.0) +
                                 std::pow(a[x1] * b[x3] - a[x3] * b[x1], 2.0) +
                                 std::pow(a[x2] * b[x1] - a[x1] * b[x2], 2.0));
    // Store the area and update the total area
    std::transform(
        sum_up.begin(), sum_up.end(), triangle_centroid.begin(), sum_up.begin(),
        [A_l](double sum, double coord) { return sum + A_l * coord; });
    sum_down += A_l;
  }
  // Determine centroid as a weighted average of the centroids of the triangles
  std::transform(sum_up.begin(), sum_up.end(), centroid.begin(),
                 [sum_down](double sum) { return sum / sum_down; });
  centroid_calculated = true;
}

void Polygon::calculate_normal() {
  // Check if the centroid is calculated
  if (!centroid_calculated) {
    calculate_centroid();
  }
  // Find the normal vector for all the triangles formed from one edge of the
  // centroid
  std::vector<std::array<double, DIM>> normals(number_lines,
                                               std::array<double, DIM>{0});
  std::array<double, DIM> v_out = {0};  // point always outside

  // Arrays a and b to define the vectors of the triangles
  std::array<double, DIM> a = {0};
  std::array<double, DIM> b = {0};
  // Loop over all triangles
  for (int i = 0; i < number_lines; i++) {
    auto& line = lines[i];
    a = line.get_start_point();
    b = line.get_end_point();
    for (int j = 0; j < DIM; j++) {
      a[j] -= centroid[j];
      b[j] -= centroid[j];
    }
    // Calculate the normal vector of the triangle with cross product
    normals[i][x1] = 0.5 * (a[x2] * b[x3] - a[x3] * b[x2]);
    normals[i][x2] = -0.5 * (a[x1] * b[x3] - a[x3] * b[x1]);
    normals[i][x3] = 0.5 * (a[x1] * b[x2] - a[x2] * b[x1]);
    normals[i][const_i] = 0.0;

    // Construct the vector pointing outside the polygon
    std::array<double, DIM> o = lines[i].get_outside_point();
    std::transform(o.begin(), o.end(), centroid.begin(), v_out.begin(),
                   std::minus<>());
    // Check if the normal is pointing in the correct direction
    flip_normal_if_needed(normals[i], v_out);
  }
  // The normal is the sum of the normals of the triangles
  for (int j = 0; j < DIM; j++) {
    normal[j] = 0.0;
  }
  for (int i = 0; i < number_lines; i++) {
    for (int j = 0; j < DIM; j++) {
      normal[j] += normals[i][j];
    }
  }
  normal_calculated = true;
}

std::array<Line, Polygon::MAX_LINES>& Polygon::get_lines() { return lines; }

void Polygon::print(std::ofstream& file, std::array<double, DIM> position) {
  // Print the polygon to the file
  for (int i = 0; i < number_lines; i++) {
    std::array<double, DIM> p1 = lines[i].get_start_point();
    std::array<double, DIM> p2 = lines[i].get_end_point();
    file << position[x1] + p1[x1] << " " << position[x2] + p1[x2] << " "
         << position[x3] + p1[x3] << " " << position[x1] + p2[x1] << " "
         << position[x2] + p2[x2] << " " << position[x3] + p2[x3] << " "
         << position[x1] + centroid[x1] << " " << position[x2] + centroid[x2]
         << " " << position[x3] + centroid[x3] << std::endl;
  }
}


#include "Polygon.h"

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

    double difference1 = 0.0;
    double difference2 = 0.0;
    for (int i = 0; i < DIM; i++) {
      difference1 += std::abs(start_point[i] - last_end_point[i]);
      difference2 += std::abs(end_point[i] - last_end_point[i]);
    }
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

void Polygon::calculate_centroid() {
  // Array of 0s to store the mean values
  std::array<double, DIM> mean_values = {0};

  // Determine the mean values of the corner points, all points appear twice
  for (int i = 0; i < number_lines; i++) {
    for (int j = 0; j < DIM; j++) {
      mean_values[j] +=
          lines[i].get_start_point()[j] + lines[i].get_end_point()[j];
    }
  }
  for (int j = 0; j < DIM; j++) {
    mean_values[j] /= (2.0 * number_lines);
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

  // CONTINUE HERE
}

int Polygon::get_number_lines() { return number_lines; }

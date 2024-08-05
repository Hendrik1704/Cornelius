#include "Line.h"

#include <array>

Line::Line() {
  // Initialize a GeneralGeometryElement object
  GeneralGeometryElement();
}

Line::~Line() {}

void Line::init_line(std::array<std::array<double, DIM>, LINE_DIM> new_corners,
                     std::array<double, DIM> new_out,
                     std::array<int, DIM - LINE_DIM> new_const_i) {
  // Copy the new values into the class variables
  corners = new_corners;
  out = new_out;
  const_i = new_const_i;

  // Set the indices of the start and end points
  // This allows us to easily flip the start and end points
  start_point = 0;
  end_point = 1;

  // Fix the non-zero indices in such a way that x1 is always smaller
  if (new_const_i[0] == 0) {
    if (new_const_i[1] == 1) {
      x1 = 2;
      x2 = 3;
    } else if (new_const_i[1] == 2) {
      x1 = 1;
      x2 = 3;
    } else {
      x1 = 1;
      x2 = 2;
    }
  } else if (new_const_i[0] == 1) {
    if (new_const_i[1] == 2) {
      x1 = 0;
      x2 = 3;
    } else {
      x1 = 0;
      x2 = 2;
    }
  } else {
    x1 = 0;
    x2 = 1;
  }

  // Set the flags for normal and centroid calculations to false
  normal_calculated = false;
  centroid_calculated = false;
}

void Line::calculate_normal() {
  // Centroid must be calculated before we can calculate normal
  if (!centroid_calculated) {
    calculate_centroid();
  }
  // The normal is given by (-dy, dx)
  normal[x1] = -(corners[1][x2] - corners[0][x2]);
  normal[x2] = corners[1][x1] - corners[0][x1];
  normal[const_i[0]] = 0;
  normal[const_i[1]] = 0;

  // Check if the normal is pointing in the correct direction
  std::array<double, DIM> reference_normal;
  for (int j = 0; j < DIM; j++) {
    reference_normal[j] = out[j] - centroid[j];
  }
  flip_normal_if_needed(normal, reference_normal);

  normal_calculated = true;
}

void Line::calculate_centroid() {
  for (int i = 0; i < DIM; i++) {
    centroid[i] = (corners[0][i] + corners[1][i]) / 2.0;
  }
  centroid_calculated = true;
}

void Line::flip_start_end() { std::swap(start_point, end_point); }

std::array<double, GeneralGeometryElement::DIM>& Line::get_start_point() {
  return corners[start_point];
}

std::array<double, GeneralGeometryElement::DIM>& Line::get_end_point() {
  return corners[end_point];
}

std::array<double, GeneralGeometryElement::DIM>& Line::get_outside_point() {
  return out;
}
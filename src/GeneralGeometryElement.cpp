#include "GeneralGeometryElement.h"

#include <algorithm>
#include <array>
#include <numeric>

GeneralGeometryElement::GeneralGeometryElement()
    : normal_calculated(false), centroid_calculated(false) {}

GeneralGeometryElement::~GeneralGeometryElement() {}

void GeneralGeometryElement::flip_normal_if_needed(
    std::array<double, DIM> &normal,
    std::array<double, DIM> &reference_normal) {
  // Compute the dot product of the two normals
  double dot_product = std::inner_product(normal.begin(), normal.end(),
                                          reference_normal.begin(), 0.0);

  // If the dot product is negative, flip the normal
  if (dot_product < 0) {
    for (double &component : normal) {
      component = -component;
    }
  }
}

void GeneralGeometryElement::calculate_normal() {
  // Provide implementation in derived classes
}

void GeneralGeometryElement::calculate_centroid() {
  // Provide implementation in derived classes
}

std::array<double, GeneralGeometryElement::DIM>
    &GeneralGeometryElement::get_normal() {
  if (!normal_calculated) {
    calculate_normal();
  }
  return normal;
}

std::array<double, GeneralGeometryElement::DIM>
    &GeneralGeometryElement::get_centroid() {
  if (!centroid_calculated) {
    calculate_centroid();
  }
  return centroid;
}
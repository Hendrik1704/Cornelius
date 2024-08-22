#include "GeneralGeometryElement.h"

GeneralGeometryElement::GeneralGeometryElement()
    : normal_calculated(false), centroid_calculated(false) {}

GeneralGeometryElement::~GeneralGeometryElement() = default;

void GeneralGeometryElement::calculate_normal() {
  // Provide implementation in derived classes
}

void GeneralGeometryElement::calculate_centroid() {
  // Provide implementation in derived classes
}

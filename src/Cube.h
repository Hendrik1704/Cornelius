#ifndef CUBE_H
#define CUBE_H

#include "GeneralGeometryElement.h"
#include "Line.h"
#include "Polygon.h"
#include "Square.h"

class Cube : public GeneralGeometryElement {
 private:
  static const int DIM = 4;
  static const int CUBE_DIM = 4;
  static const int MAX_POLY = 8;
  static const int NSQUARES = 6;
  static const int STEPS = 2;

  std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS> cube;
  std::array<Line, NSQUARES * 2> lines;
  std::array<Polygon, MAX_POLY> polygons;
  std::array<Square, NSQUARES> squares;

  int number_lines;
  int number_polygons;
  bool ambiguous;
  int const_i;
  double const_value;
  int x1, x2, x3;
  std::array<double, DIM> dx;

 public:
  Cube();
  ~Cube();

  void init_cube(
      std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>& cu,
      int new_const_i, double new_const_value, std::array<double, DIM>& new_dx);
  void construct_polygons(double value);
  void split_to_squares();
  void check_ambiguity(int number_lines);
  bool is_ambiguous();
  int get_number_polygons();
  int get_number_lines();
  std::array<Polygon, MAX_POLY>& get_polygons();
};

#endif  // CUBE_H
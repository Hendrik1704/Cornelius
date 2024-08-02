#ifndef POLYGON_H
#define POLYGON_H

#include <fstream>

#include "GeneralGeometryElement.h"
#include "Line.h"

class Polygon : public GeneralGeometryElement {
 private:
  static const int MAX_LINES = 24;  ///< Maximum number of lines in a polygon
  static const int POLYGON_DIM =
      3;  ///< Dimension for polygon-specific properties

  std::array<Line, MAX_LINES> lines;  ///< Array of lines in the polygon
  int number_lines;                   ///< Number of lines in the polygon
  int x1, x2, x3;  ///< Indices representing the polygon's dimensions
  int const_i;     ///< Constant index for the polygon

 public:
  Polygon();
  ~Polygon();

  void init_polygon(int new_const_i);

  bool add_line(Line& new_line, bool perform_no_check);

  int get_number_lines();

  void calculate_normal();
  void calculate_centroid();

  std::array<Line, MAX_LINES>& get_lines();

  void print(std::ofstream& file, std::array<double, DIM> position);
};

#endif  // POLYGON_H
#ifndef POLYGON_H
#define POLYGON_H

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <numeric>
#include <vector>

#include "GeneralGeometryElement.h"
#include "Line.h"

/**
 * @class Polygon
 * @brief A class representing a polygon in a geometric space.
 *
 * This class extends the GeneralGeometryElement class and provides methods
 * to manage and calculate properties related to polygons.
 */
class Polygon : public GeneralGeometryElement {
 protected:
  static constexpr int MAX_LINES =
      24;                             ///< Maximum number of lines in a polygon
  std::array<Line, MAX_LINES> lines;  ///< Array of lines in the polygon
  int number_lines;                   ///< Number of lines in the polygon
  int x1, x2, x3;  ///< Indices representing the polygon's dimensions
  int const_i;     ///< Constant index for the polygon

 public:
  /**
   * @brief Default constructor for the Polygon class.
   *
   * Initializes a new instance of the Polygon class.
   */
  Polygon();

  /**
   * @brief Destructor for the Polygon class.
   *
   * Cleans up resources used by the Polygon class.
   */
  ~Polygon();

  /**
   * @brief Initializes the polygon with a constant index.
   *
   * @param new_const_i The constant index for the polygon's dimensions.
   */
  void init_polygon(int new_const_i);

  /**
   * @brief Adds a line to the polygon.
   *
   * @param new_line The line to be added.
   * @param perform_no_check If true, the line is added without connectivity
   * checks.
   * @return True if the line was successfully added, otherwise false.
   */
  bool add_line(Line& new_line, bool perform_no_check);

  /**
   * @brief Gets the number of lines in the polygon.
   *
   * @return The number of lines in the polygon.
   */
  int get_number_lines();

  /**
   * @brief Calculates the normal vector of the polygon.
   *
   * This method calculates the normal vector based on the lines and centroid of
   * the polygon.
   */
  void calculate_normal() override;

  /**
   * @brief Calculates the centroid of the polygon.
   *
   * This method calculates the centroid based on the vertices of the polygon.
   */
  void calculate_centroid() override;

  /**
   * @brief Gets the lines that form the polygon.
   *
   * @return A reference to the array of lines in the polygon.
   */
  std::array<Line, MAX_LINES>& get_lines();

  /**
   * @brief Prints the triangles formed from the polygon into a given file.
   * Prints the absolute points, so this file can be used to plot the surface.
   *
   * @param file The output file stream.
   * @param position The position offset for printing the polygon.
   */
  void print(std::ofstream& file, std::array<double, DIM> position);
};

#endif  // POLYGON_H
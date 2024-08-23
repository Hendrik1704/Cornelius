#ifndef CUBE_H
#define CUBE_H

#include <array>
#include <iostream>
#include <vector>

#include "GeneralGeometryElement.h"
#include "Line.h"
#include "Polygon.h"
#include "Square.h"

/**
 * @class Cube
 * @brief Represents a cube in a geometric space, derived from
 * GeneralGeometryElement.
 *
 * The Cube class encapsulates the properties and operations related to a cube
 * in a geometric space. It provides methods to initialize the cube, construct
 * polygons within the cube, split the cube into squares, and check for
 * ambiguity.
 *
 * 13.10.2011 Hannu Holopainen
 * 23.08.2024 Hendrik Roch, Haydar Mehryar
 *
 */
class Cube : public GeneralGeometryElement {
 private:
  static constexpr int DIM = 4;           ///< Dimension of the space.
  static constexpr int CUBE_DIM = 4;      ///< Dimension of the cube.
  static constexpr int NSQUARES = 6;      ///< Number of squares in the cube.
  static constexpr int STEPS = 2;         ///< Number of steps.
  static constexpr int MAX_POLYGONS = 8;  ///< Maximum number of polygons.

  std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>
      cube;                       ///< 3D array representing the cube.
  std::vector<Polygon> polygons;  ///< Vector to store the polygons in the cube.
  std::array<Square, NSQUARES> squares;  ///< Array of squares in the cube.
  std::array<Line, NSQUARES * 2> lines;  ///< Array of lines in the squares.

  int number_lines;            ///< Number of lines in the cube.
  int number_polygons;         ///< Number of polygons in the cube.
  bool ambiguous;              ///< Indicates if the cube is ambiguous.
  int const_i;                 ///< Index for the constant dimension.
  double const_value;          ///< Value for the constant dimension.
  int x1, x2, x3;              ///< Indices for dimensions.
  std::array<double, DIM> dx;  ///< Delta values for discretization.

  // Temporary array for storing a square
  std::array<std::array<double, STEPS>, STEPS>
      square;  ///< 2D array representing a square

 public:
  /**
   * @brief Default constructor for the Cube class.
   */
  Cube();

  /**
   * @brief Destructor for the Cube class.
   */
  ~Cube();

  /**
   * @brief Initializes the cube with given parameters.
   * @param cu 3D array representing the cube.
   * @param new_const_i Index for the constant dimension.
   * @param new_const_value Value for the constant dimension.
   * @param new_dx Delta values for discretization.
   */
  void init_cube(
      std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>& cu,
      int new_const_i, double new_const_value, std::array<double, DIM>& new_dx);

  /**
   * @brief Constructs polygons within the cube based on a given value.
   * @param value The value used to construct polygons.
   */
  void construct_polygons(double value);

  /**
   * @brief Splits the cube into squares.
   */
  void split_to_squares();

  /**
   * @brief Checks if the cube is ambiguous based on the number of lines.
   * @param number_lines The number of lines in the cube.
   */
  inline void check_ambiguity(int number_lines) {
    // Check if any squares may have ambiguous elements
    for (int i = 0; i < NSQUARES; i++) {
      if (squares[i].is_ambiguous()) {
        ambiguous = true;
        return;
      }
    }
    // If the surface is not ambiguous already, it is still possible to
    // have a ambiguous case if we have exactly 6 lines, i.e. the surface
    // elements are at the opposite corners
    if (number_lines == 6) {
      ambiguous = true;
    }
  }

  /**
   * @brief Checks if the cube is ambiguous.
   * @return True if the cube is ambiguous, false otherwise.
   */
  inline bool is_ambiguous() { return ambiguous; }

  /**
   * @brief Gets the number of polygons in the cube.
   * @return The number of polygons.
   */
  inline int get_number_polygons() { return number_polygons; }

  /**
   * @brief Gets the number of lines in the cube.
   * @return The number of lines.
   */
  inline int get_number_lines() { return number_lines; }

  /**
   * @brief Gets the polygons in the cube.
   * @return A reference to the array of polygons.
   */
  inline std::vector<Polygon>& get_polygons() { return polygons; }
};

#endif  // CUBE_H
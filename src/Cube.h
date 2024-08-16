#ifndef CUBE_H
#define CUBE_H

#include <array>
#include <iostream>

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
 * 08.08.2024 Hendrik Roch
 */
class Cube : public GeneralGeometryElement {
 private:
  static constexpr int DIM = 4;           ///< Dimension of the space.
  static constexpr int CUBE_DIM = 4;      ///< Dimension of the cube.
  static constexpr int NSQUARES = 6;      ///< Number of squares in the cube.
  static constexpr int STEPS = 2;         ///< Number of steps.
  static constexpr int MAX_POLYGONS = 8;  ///< Maximum number of polygons.

  std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>
      cube;  ///< 3D array representing the cube.
  std::array<Polygon, MAX_POLYGONS>
      polygons;                          ///< Array of polygons in the cube.
  std::array<Square, NSQUARES> squares;  ///< Array of squares in the cube.
  std::array<Line, NSQUARES * 2> lines;  ///< Array of lines in the squares.

  int number_lines;            ///< Number of lines in the cube.
  int number_polygons;         ///< Number of polygons in the cube.
  bool ambiguous;              ///< Indicates if the cube is ambiguous.
  int const_i;                 ///< Index for the constant dimension.
  double const_value;          ///< Value for the constant dimension.
  int x1, x2, x3;              ///< Indices for dimensions.
  std::array<double, DIM> dx;  ///< Delta values for discretization.

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
  void check_ambiguity(int number_lines);

  /**
   * @brief Checks if the cube is ambiguous.
   * @return True if the cube is ambiguous, false otherwise.
   */
  bool is_ambiguous();

  /**
   * @brief Gets the number of polygons in the cube.
   * @return The number of polygons.
   */
  int get_number_polygons();

  /**
   * @brief Gets the number of lines in the cube.
   * @return The number of lines.
   */
  int get_number_lines();

  /**
   * @brief Gets the polygons in the cube.
   * @return A reference to the array of polygons.
   */
  std::array<Polygon, MAX_POLYGONS>& get_polygons();
};

#endif  // CUBE_H
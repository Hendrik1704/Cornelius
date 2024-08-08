#ifndef SQUARE_H
#define SQUARE_H

#include <vector>

#include "GeneralGeometryElement.h"
#include "Line.h"

/**
 * @class Square
 * @brief A class representing a square element.
 *
 * This class handles squares. Finds the edges of the surface and also
 * the points which are always outside the surface so that we can determine
 * correct direction for normal vector.
 */
class Square : public GeneralGeometryElement {
 private:
  static constexpr int DIM = 4;          ///< Dimension of the space.
  static constexpr int SQUARE_DIM = 2;   ///< Dimension of the square.
  const double ALMOST_ONE = 1.0 - 1e-9;  ///< Almost one value.
  std::array<std::array<double, SQUARE_DIM>, SQUARE_DIM>
      points;                                        ///< Points of the square.
  std::vector<std::array<double, SQUARE_DIM>> cuts;  ///< Points of the cuts.
  std::vector<std::array<double, SQUARE_DIM>>
      out;  ///< Points outside the square.
  std::array<std::array<double, DIM>, SQUARE_DIM>
      points_temp;                            ///< Temporary points.
  std::array<double, DIM> out_temp;           ///< Temporary outside points.
  std::array<int, DIM - SQUARE_DIM> const_i;  ///< Indices for constraints.
  std::array<double, DIM - SQUARE_DIM>
      const_value;             ///< Values for constraints.
  int x1, x2;                  ///< Indices for dimensions of the square.
  std::array<double, DIM> dx;  ///< Delta values for lines.
  int number_cuts;             ///< Number of cuts.
  int number_lines;            ///< Number of lines.
  std::vector<Line> lines;     ///< Lines in the square.
  bool ambiguous;              ///< Indicates if the square is ambiguous.

 public:
  /**
   * @brief Default constructor for the Square class.
   */
  Square();

  /**
   * @brief Destructor for the Square class.
   */
  ~Square();

  /**
   * @brief Initializes the square with given parameters.
   * @param sq Points defining the square.
   * @param c_i Constant indices for the square.
   * @param c_v Values for the constant indices.
   * @param dex Delta values for lines.
   */
  void init_square(std::array<std::array<double, SQUARE_DIM>, SQUARE_DIM>& sq,
                   std::array<int, DIM - SQUARE_DIM>& c_i,
                   std::array<double, DIM - SQUARE_DIM>& c_v,
                   std::array<double, DIM>& dex);

  /**
   * @brief Constructs lines within the square based on a given value.
   * @param value The value used to construct lines.
   */
  void construct_lines(double value);

  /**
   * @brief Determines the ends of edges based on a given value.
   *
   * This finds the points from the edges where the surface crosses
   * the given square.
   *
   * The edges are gone through in the following order
   *      4
   *    -----
   *  1 |   | 3
   *    -----
   *      2
   * Since in the case where the ends are on each edge the ends are
   * assumed to be connected like \\
   *
   * @param value The value used to find ends of edges.
   */
  void ends_of_edge(double value);

  /**
   * @brief Finds the points outside the square based on a given value.
   * @param value The value used to find outside points.
   */
  void find_outside(double value);

  /**
   * @brief Checks if the square is ambiguous.
   * @return True if the square is ambiguous, false otherwise.
   */
  bool is_ambiguous();

  /**
   * @brief Gets the number of lines in the square.
   * @return The number of lines.
   */
  int get_number_lines();

  /**
   * @brief Gets the lines in the square.
   * @return A reference to the array of lines.
   */
  std::vector<Line>& get_lines();
};

#endif  // SQUARE_H
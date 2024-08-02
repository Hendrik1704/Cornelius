#ifndef LINE_H
#define LINE_H

#include "GeneralGeometryElement.h"

/**
 * @class Line
 * @brief Represents a line in a geometric space, derived from
 * GeneralGeometryElement.
 *
 * The Line class encapsulates the properties and operations related to a line
 * segment in a geometric space. It provides methods to initialize the line,
 * flip its start and end points, and calculate various geometric properties
 * such as the normal and centroid.
 */
class Line : public GeneralGeometryElement {
 protected:
  static const int LINE_DIM = 2;  ///< Dimension for line-specific properties
  static const int LINE_CORNERS = 2;  ///< Number of corners for a line

  int x1, x2;       ///< Indices representing the line's dimensions
  int start_point;  ///< Index of the start point
  int end_point;    ///< Index of the end point
  std::array<std::array<double, DIM>, LINE_DIM>
      corners;                  ///< Array of line corners
  std::array<double, DIM> out;  ///< Output point of the line
  std::array<double, DIM - LINE_DIM>
      const_i;  ///< Constant indices for the line

 public:
  /**
   * @brief Default constructor for the Line class.
   *
   * Initializes a Line object. Calls the constructor of GeneralGeometryElement.
   */
  Line();

  /**
   * @brief Destructor for the Line class.
   *
   * Cleans up any resources used by the Line object.
   */
  ~Line();

  /**
   * @brief Initializes the line with specific corner points, an output point,
   * and constant indices.
   *
   * @param new_corners Array of corner points defining the line
   * @param new_out Point outside the surface
   * @param new_const_i Array of constant indices
   */
  void init_line(std::array<std::array<double, DIM>, LINE_DIM> new_corners,
                 std::array<double, DIM> new_out,
                 std::array<double, DIM - LINE_DIM> new_const_i);

  /**
   * @brief Flips the start and end points of the line.
   *
   * This method swaps the line's start and end points to reverse the direction
   * of the line.
   */
  void flip_start_end();

  /**
   * @brief Calculates the normal vector of the line.
   *
   * Computes the normal vector for the line. This function must be implemented
   * based on the specific geometric context of the line.
   */
  void calculate_normal();

  /**
   * @brief Calculates the centroid of the line.
   *
   * Computes the centroid point of the line. This function must be implemented
   * based on the specific geometric context of the line.
   */
  void calculate_centroid();

  /**
   * @brief Retrieves the start point of the line in 4D.
   *
   * @return Reference to the array representing the start point
   */
  std::array<double, GeneralGeometryElement::DIM>& get_start_point();

  /**
   * @brief Retrieves the end point of the line in 4D.
   *
   * @return Reference to the array representing the end point
   */
  std::array<double, GeneralGeometryElement::DIM>& get_end_point();

  /**
   * @brief Retrieves the point which is always outside in 4D.
   *
   * @return Reference to the array representing the outside point
   */
  std::array<double, GeneralGeometryElement::DIM>& get_out_point();
};

#endif  // LINE_H
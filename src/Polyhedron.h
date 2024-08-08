#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include "GeneralGeometryElement.h"
#include "Line.h"
#include "Polygon.h"

/**
 * @class Polyhedron
 * @brief Represents a polyhedron composed of multiple polygons.
 *
 * The Polyhedron class inherits from GeneralGeometryElement and contains
 * methods for initializing the polyhedron, adding polygons, checking line
 * equality, calculating the volume of tetrahedrons, and calculating the
 * centroid and normal.
 */
class Polyhedron : public GeneralGeometryElement {
 private:
  std::vector<Polygon> polygons;  ///< Vector of polygons in the polyhedron
  int number_polygons;            ///< Number of polygons in the polyhedron
  int number_tetrahedrons;        ///< Number of tetrahedrons in the polyhedron
  int x1, x2, x3, x4;  ///< Indices representing the polyhedron's dimensions

 public:
  /**
   * @brief Constructs a Polyhedron object.
   */
  Polyhedron();

  /**
   * @brief Destroys the Polyhedron object.
   */
  ~Polyhedron();

  /**
   * @brief Initializes the polyhedron.
   *
   * Sets the initial values for the indices, polygon count, tetrahedron count,
   * and flags.
   */
  void init_polyhedron();

  /**
   * @brief Adds a polygon to the polyhedron.
   *
   * @param new_polygon The polygon to add.
   * @param perform_no_check If true, adds the polygon without checking for
   * connection to existing polygons.
   * @return True if the polygon was added successfully, false otherwise.
   */
  bool add_polygon(Polygon& new_polygon, bool perform_no_check);

  /**
   * @brief Checks if two lines are connected.
   *
   * @param line1 The first line.
   * @param line2 The second line.
   * @return True if the lines are connected, false otherwise.
   */
  bool lines_are_connected(Line& line1, Line& line2);

  /**
   * @brief Calculates the normal vector of a tetrahedron, which is also
   * the volume of the tetrahedron.
   *
   * @param v1 First vector defining the tetrahedron.
   * @param v2 Second vector defining the tetrahedron.
   * @param v3 Third vector defining the tetrahedron.
   * @param n Normal vector of the tetrahedron.
   */
  void tetrahedron_volume(std::array<double, DIM>& v1,
                          std::array<double, DIM>& v2,
                          std::array<double, DIM>& v3,
                          std::array<double, DIM>& n);

  /**
   * @brief Calculates the centroid of the polyhedron.
   *
   * Computes the centroid as the volume-weighted average of the individual
   * tetrahedrons.
   */
  void calculate_centroid() override;

  /**
   * @brief Calculates the normal of the polyhedron.
   *
   * Computes the normal as the sum of the normals of the individual
   * tetrahedrons.
   */
  void calculate_normal() override;

  /**
   * @brief Retrieves the number of polygons in the polyhedron.
   *
   * @return The number of polygons in the polyhedron.
   */
  int get_number_polygons() { return number_polygons; }

  /**
   * @brief Retrieves the number of tetrahedrons in the polyhedron.
   *
   * @return The number of tetrahedrons in the polyhedron.
   */
  int get_number_tetrahedrons() { return number_tetrahedrons; }
};

#endif  // POLYHEDRON_H
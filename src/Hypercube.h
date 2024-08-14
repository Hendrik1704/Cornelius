#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include <algorithm>
#include <numeric>
#include <vector>

#include "Cube.h"
#include "GeneralGeometryElement.h"
#include "Polyhedron.h"

/**
 * @class Hypercube
 * @brief A class representing a 4-dimensional hypercube for geometric
 * operations.
 *
 * The Hypercube class provides methods to initialize a hypercube, split into
 * cubes, check for ambiguities, construct polyhedra, and access geometric
 * elements.
 *
 * 13.10.2011 Hannu Holopainen
 * 08.08.2024 Hendrik Roch
 */
class Hypercube : public GeneralGeometryElement {
 private:
  static constexpr int DIM = 4;     ///< Dimension of the space.
  static constexpr int STEPS = 2;   ///< Number of steps for discretization.
  static constexpr int NCUBES = 8;  ///< Number of cubes in the hypercube.

  std::array<std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>,
             STEPS>
      hypercube;  ///< 4D array representing the hypercube.
  std::vector<Polyhedron>
      polyhedra;            ///< Vector to store the polyhedra in the hypercube.
  std::vector<Cube> cubes;  ///< Vector to store the cubes in the hypercube.
  std::vector<Polygon>
      polygons;  ///< Vector to store the polygons in the hypercube.

  int number_polyhedra;        ///< Number of polyhedra in the hypercube.
  bool ambiguous;              ///< Indicates if the hypercube is ambiguous.
  std::array<double, DIM> dx;  ///< Delta values for discretization.

 public:
  /**
   * @brief Default constructor for the Hypercube class.
   */
  Hypercube();

  /**
   * @brief Destructor for the Hypercube class.
   */
  ~Hypercube();

  /**
   * @brief Initializes the hypercube with given parameters.
   * @param hc 4D array representing the hypercube.
   * @param new_dx Delta values for discretization.
   */
  void init_hypercube(
      std::array<
          std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>,
          STEPS>& hc,
      std::array<double, DIM>& new_dx);

  /**
   * @brief Splits the hypercube into cubes.
   * @param value The value used to decide if the point is inside the surface.
   * @return The number of points below the value.
   */
  double split_to_cubes(double value);

  /**
   * @brief Checks if the hypercube is ambiguous based on a given value.
   * @param number_points_below_value The number of points below the value.
   */
  void check_ambiguity(int number_points_below_value);

  /**
   * @brief Constructs polyhedra within the hypercube based on a given value.
   * @param value The value used to construct polyhedra.
   */
  void construct_polyhedra(double value);

  /**
   * @brief Gets the number of polyhedra in the hypercube.
   * @return The number of polyhedra.
   */
  int get_number_polyhedra();

  /**
   * @brief Gets the polyhedra in the hypercube.
   * @return A reference to the vector of polyhedra.
   */
  std::vector<Polyhedron>& get_polyhedra();

  /**
   * @brief Checks if the hypercube is ambiguous.
   * @return True if the hypercube is ambiguous, false otherwise.
   */
  bool is_ambiguous();
};

#endif  // HYPERCUBE_H

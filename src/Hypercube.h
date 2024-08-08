#ifndef HYPERCUBE_H
#define HYPERCUBE_H

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
 */
class Hypercube : public GeneralGeometryElement {
 private:
  static constexpr int DIM = 4;    ///< Dimension of the space.
  static constexpr int STEPS = 2;  ///< Number of steps for discretization.

  std::array<std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>,
             STEPS>
      hypercube;  ///< 4D array representing the hypercube.
  std::vector<Polyhedron>
      polyhedra;  ///< Vector to store the polyhedra in the hypercube.

  int number_polyhedra;        ///< Number of polyhedra in the hypercube.
  bool ambiguous;              ///< Indicates if the hypercube is ambiguous.
  int x1, x2, x3, x4;          ///< Indices for dimensions.
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
   * @param cubes Vector to store the cubes.
   */
  void split_to_cubes(std::vector<Cube>& cubes);

  /**
   * @brief Checks if the hypercube is ambiguous based on a given value.
   * @param value The value used to check for ambiguity.
   * @param cubes The cubes in the hypercube.
   */
  void check_ambiguity(double value, std::vector<Cube>& cubes);

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

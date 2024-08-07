#include "Hypercube.h"

#include <algorithm>
#include <numeric>
#include <vector>

Hypercube::Hypercube() : number_polyhedra(0), ambiguous(false) {}

Hypercube::~Hypercube() {}

void Hypercube::init_hypercube(
    std::array<std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>,
               STEPS>& hc,
    std::array<double, DIM>& new_dx) {
  hypercube = hc;
  dx = new_dx;
  x1 = 0;
  x2 = 1;
  x3 = 2;
  x4 = 3;
  number_polyhedra = 0;
  ambiguous = false;
}

void Hypercube::split_to_cubes() {
  std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS> cube;
  int number_cubes = 0;
  for (int i = 0; i < DIM; i++) {
    // i is the index which is kept constant, thus we ignore the index which
    // is constant in this cube
    int c_i = i;
    double c_v;
    for (int j = 0; j < STEPS; j++) {
      c_v = j * dx[i];
      for (int ci1 = 0; ci1 < STEPS; ci1++) {
        for (int ci2 = 0; ci2 < STEPS; ci2++) {
          for (int ci3 = 0; ci3 < STEPS; ci3++) {
            if (i == x1) {
              cube[ci1][ci2][ci3] = hypercube[j][ci1][ci2][ci3];
            } else if (i == x2) {
              cube[ci1][ci2][ci3] = hypercube[ci1][j][ci2][ci3];
            } else if (i == x3) {
              cube[ci1][ci2][ci3] = hypercube[ci1][ci2][j][ci3];
            } else {
              cube[ci1][ci2][ci3] = hypercube[ci1][ci2][ci3][j];
            }
          }
        }
      }
      cubes[number_cubes++].init_cube(cube, c_i, c_v, dx);
    }
  }
}

void Hypercube::construct_polyhedra(double value) {
  split_to_cubes();
  // Construct polygons for each cube
  for (int i = 0; i < NCUBES; i++) {
    cubes[i].construct_polygons(value);
  }
  int number_polygons = 0;
  // Store the reference to the polygons
  for (int i = 0; i < NCUBES; i++) {
    for (int j = 0; j < cubes[i].get_number_polygons(); j++) {
      polygons[number_polygons++] = cubes[i].get_polygons()[j];
    }
  }
  check_ambiguity(value);
  if (ambiguous) {
    // The surface might be ambiguous and we need to connect the polygons and
    // see how many polyhedrons we have
    std::vector<bool> not_used(number_polygons, true);
    // Keep track of the used number of lines
    int used = 0;
    do {
      polyhedra[number_polyhedra].init_polyhedron();
      // Go through all the polygons and try to add them to the polyhedron
      for (int i = 0; i < number_polygons; i++) {
        if (not_used[i]) {
          // add_polygon returns true if the polygon was added
          if (polyhedra[number_polyhedra].add_polygon(polygons[i], false)) {
            not_used[i] = false;
            used++;
            // If the polygon is successfully added we start the loop from the
            // beginning
            i = 0;
          }
        }
      }
      number_polyhedra++;
    } while (used < number_polygons);
  } else {
    // Here surface cannot be ambiguous and all polygons can be added to
    // the polyhedron without ordering them
    polyhedra[number_polyhedra].init_polyhedron();
    for (int i = 0; i < number_polygons; i++) {
      polyhedra[number_polyhedra].add_polygon(polygons[i], true);
    }
    number_polyhedra++;
  }
}

void Hypercube::check_ambiguity(double value) {
  for (int i = 0; i < NCUBES; i++) {
    if (cubes[i].is_ambiguous()) {
      ambiguous = true;
      return;
    }
  }

  int number_lines = std::accumulate(
      cubes.begin(), cubes.end(), 0,
      [](int sum, Cube& cube) { return sum + cube.get_number_lines(); });

  int number_points = std::accumulate(
      hypercube.begin(), hypercube.end(), 0, [&](int sum, const auto& array3d) {
        return sum +
               std::accumulate(
                   array3d.begin(), array3d.end(), 0,
                   [&](int sum2, const auto& array2d) {
                     return sum2 +
                            std::accumulate(
                                array2d.begin(), array2d.end(), 0,
                                [&](int sum3, const auto& array1d) {
                                  return sum3 +
                                         std::count_if(array1d.begin(),
                                                       array1d.end(),
                                                       [&](double element) {
                                                         return element < value;
                                                       });
                                });
                   });
      });
  if (number_points > 8) {
    number_points = 16 - number_points;
  }
  if (number_lines == 24 && number_points == 2) {
    ambiguous = true;
  }
}

int Hypercube::get_number_polyhedra() { return number_polyhedra; }

bool Hypercube::is_ambiguous() { return ambiguous; }

std::array<Polyhedron, Hypercube::MAX_POLY>& Hypercube::get_polyhedra() {
  return polyhedra;
}
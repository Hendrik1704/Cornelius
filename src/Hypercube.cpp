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

void Hypercube::split_to_cubes(std::vector<Cube>& cubes) {
  std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS> cube;
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
      cubes.emplace_back();
      cubes.back().init_cube(cube, c_i, c_v, dx);
    }
  }
}

void Hypercube::construct_polyhedra(double value) {
  std::vector<Cube> cubes;
  split_to_cubes(cubes);
  // Construct polygons for each cube
  for (auto& cube : cubes) {
    cube.construct_polygons(value);
  }

  // Store the reference to the polygons
  std::vector<Polygon> polygons;
  for (auto& cube : cubes) {
    for (auto& polygon : cube.get_polygons()) {
      polygons.push_back(polygon);
    }
  }
  check_ambiguity(value, cubes);
  if (ambiguous) {
    // The surface might be ambiguous and we need to connect the polygons and
    // see how many polyhedrons we have
    std::vector<bool> not_used(polygons.size(), true);
    // Keep track of the used number of lines
    int used = 0;
    do {
      Polyhedron new_polyhedron;
      new_polyhedron.init_polyhedron();
      // Go through all the polygons and try to add them to the polyhedron
      for (size_t i = 0; i < polygons.size(); i++) {
        // add_polygon returns true if the polygon was added
        if (not_used[i] && new_polyhedron.add_polygon(polygons[i], false)) {
          not_used[i] = false;
          used++;
          // If the polygon is successfully added we start the loop from the
          // beginning
          i = 0;
        }
      }
      polyhedra.push_back(new_polyhedron);
      number_polyhedra++;
    } while (used < polygons.size());
  } else {
    // Here surface cannot be ambiguous and all polygons can be added to
    // the polyhedron without ordering them
    Polyhedron new_polyhedron;
    new_polyhedron.init_polyhedron();
    for (auto& polygon : polygons) {
      new_polyhedron.add_polygon(polygon, true);
    }
    polyhedra.push_back(new_polyhedron);
    number_polyhedra++;
  }
}

void Hypercube::check_ambiguity(double value, std::vector<Cube>& cubes) {
  for (auto& cube : cubes) {
    if (cube.is_ambiguous()) {
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

std::vector<Polyhedron>& Hypercube::get_polyhedra() { return polyhedra; }
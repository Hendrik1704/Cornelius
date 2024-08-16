#include "Hypercube.h"

Hypercube::Hypercube() : number_polyhedra(0), ambiguous(false) {}

Hypercube::~Hypercube() {}

void Hypercube::init_hypercube(
    std::array<std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>,
               STEPS>& hc,
    std::array<double, DIM>& new_dx) {
  hypercube = hc;
  dx = new_dx;
  number_polyhedra = 0;
  ambiguous = false;
}

double Hypercube::split_to_cubes(double value) {
  std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS> cube;
  int number_points_below_value = 0;
  int cube_index = 0;
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
            cube[ci1][ci2][ci3] = (i == 0)   ? hypercube[j][ci1][ci2][ci3]
                                  : (i == 1) ? hypercube[ci1][j][ci2][ci3]
                                  : (i == 2) ? hypercube[ci1][ci2][j][ci3]
                                             : hypercube[ci1][ci2][ci3][j];

            if ((i == 0) && (hypercube[j][ci1][ci2][ci3] < value)) {
              number_points_below_value++;
            }
          }
        }
      }
      cubes[cube_index++].init_cube(cube, c_i, c_v, dx);
    }
  }
  return number_points_below_value;
}

void Hypercube::construct_polyhedra(double value) {
  int number_points_below_value = split_to_cubes(value);

  // Store the reference to the polygons
  int number_polygons = 0;
  for (int i = 0; i < NCUBES; i++) {
    cubes[i].construct_polygons(value);
    auto& polygons_cube = cubes[i].get_polygons();
    for (int j = 0; j < cubes[i].get_number_polygons(); j++) {
      polygons[number_polygons++] = polygons_cube[j];
    }
  }
  check_ambiguity(value);
  if (ambiguous) {
    // The surface might be ambiguous and we need to connect the polygons and
    // see how many polyhedra we have
    std::vector<bool> not_used(number_polygons, true);
    // Keep track of the used number of lines
    int used = 0;
    do {
      polyhedra[number_polyhedra].init_polyhedron();
      // Go through all the polygons and try to add them to the polyhedron
      for (int i = 0; i < number_polygons; i++) {
        // add_polygon returns true if the polygon was added
        if (not_used[i] &&
            polyhedra[number_polyhedra].add_polygon(polygons[i], false)) {
          not_used[i] = false;
          used++;
          // If the polygon is successfully added we start the loop from the
          // beginning
          i = 0;
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

void Hypercube::check_ambiguity(int number_points_below_value) {
  ambiguous = std::any_of(cubes.begin(), cubes.end(),
                          [](Cube& cube) { return cube.is_ambiguous(); });

  if (!ambiguous) {
    int number_lines = 0;
    for (auto& cube : cubes) {
      number_lines += cube.get_number_lines();
    }

    if (number_points_below_value > 8) {
      number_points_below_value = 16 - number_points_below_value;
    }
    if (number_lines == 24 && number_points_below_value == 2) {
      ambiguous = true;
    }
  }
}

int Hypercube::get_number_polyhedra() { return number_polyhedra; }

bool Hypercube::is_ambiguous() { return ambiguous; }

std::array<Polyhedron, Hypercube::MAX_POLYHEDRONS>& Hypercube::get_polyhedra() {
  return polyhedra;
}
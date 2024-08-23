#include "Cube.h"

Cube::Cube() : number_lines(0), number_polygons(0), ambiguous(false) {
  polygons.reserve(MAX_POLYGONS);
  polygons.emplace_back();  // Default to construct 1 Polygon
}

Cube::~Cube() = default;

void Cube::init_cube(
    std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>& cu,
    int new_const_i, double new_const_value, std::array<double, DIM>& new_dx) {
  cube = cu;
  const_i = new_const_i;
  const_value = new_const_value;
  dx = new_dx;
  switch (new_const_i) {
    case 0:
      x1 = 1;
      x2 = 2;
      x3 = 3;
      break;
    case 1:
      x1 = 0;
      x2 = 2;
      x3 = 3;
      break;
    case 2:
      x1 = 0;
      x2 = 1;
      x3 = 3;
      break;
    case 3:
      x1 = 0;
      x2 = 1;
      x3 = 2;
      break;
    default:
      break;
  }
  number_lines = number_polygons = 0;
  ambiguous = false;
}

void Cube::split_to_squares() {
  std::array<int, STEPS> c_i = {const_i, 0};
  std::array<double, STEPS> c_v = {const_value, 0.0};
  int number_squares = 0;
  for (int i = 0; i < DIM; i++) {
    // i is the index which is kept constant, thus we ignore the index which
    // is constant in this cube
    if (i != const_i) {
      c_i[1] = i;
      for (int j = 0; j < STEPS; j++) {
        c_v[1] = j * dx[i];
        for (int ci1 = 0; ci1 < STEPS; ci1++) {
          for (int ci2 = 0; ci2 < STEPS; ci2++) {
            square[ci1][ci2] = (i == x1)   ? cube[j][ci1][ci2]
                               : (i == x2) ? cube[ci1][j][ci2]
                                           : cube[ci1][ci2][j];
          }
        }
        squares[number_squares++].init_square(square, c_i, c_v, dx);
      }
    }
  }
}

void Cube::construct_polygons(double value) {
  // Start by splitting the cube to squares and finding the lines
  split_to_squares();

  // Then we make a table which contains references to the lines
  number_lines = 0;
  for (int i = 0; i < NSQUARES; i++) {
    squares[i].construct_lines(value);
    for (int j = 0; j < squares[i].get_number_lines(); j++) {
      lines[number_lines++] = squares[i].get_lines()[j];
    }
  }

  // If no lines were found we may exit. This can happen only in 4D case
  if (number_lines == 0) {
    return;
  }
  // Then we check if the surface is ambiguous and continue
  check_ambiguity(number_lines);
  if (ambiguous) {
    // Surface is ambiguous, connect the lines to polygons and see how
    // many polygons we have
    std::array<bool, NSQUARES* 2> not_used = {true};
    // Keep track of the lines which are used
    int used = 0;
    do {
      if (number_lines - used < 3) {
        std::cerr << "Error: cannot construct polygon from "
                  << number_lines - used << " lines" << std::endl;
        exit(1);
      }
      // Ensure there's space in the vector
      if (number_polygons >= polygons.size()) {
        polygons.emplace_back();  // Add a new Polygon if needed
      }
      // Initialize a new polygon
      polygons[number_polygons].init_polygon(const_i);
      // Go through all lines and try to add them to the polygon
      for (int i = 0; i < number_lines; i++) {
        // add_line returns true if line is successfully added
        if (not_used[i] &&
            polygons[number_polygons].add_line(lines[i], false)) {
          not_used[i] = false;
          used++;
          // If line is successfully added we start the loop from the
          // beginning
          i = 0;
        }
      }
      // When we have reached this point one complete polygon is formed
      number_polygons++;
    } while (used < number_lines);
  } else {
    // Surface is not ambiguous, so we have only one polygon and all lines
    // can be added to it without ordering them
    if (number_polygons >= polygons.size()) {
      polygons.emplace_back();  // Add a new Polygon if needed
    }
    polygons[number_polygons].init_polygon(const_i);
    for (int i = 0; i < number_lines; i++) {
      polygons[number_polygons].add_line(lines[i], true);
    }
    number_polygons++;
  }
}
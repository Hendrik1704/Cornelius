#include "Cube.h"

#include <iostream>
#include <vector>

Cube::Cube() {
  number_lines = 0;
  number_polygons = 0;
  ambiguous = false;
}

Cube::~Cube() {}

void Cube::init_cube(
    std::array<std::array<std::array<double, STEPS>, STEPS>, STEPS>& cu,
    int new_const_i, double new_const_value, std::array<double, DIM>& new_dx) {
  cube = cu;
  const_i = new_const_i;
  const_value = new_const_value;
  dx = new_dx;
  x1 = -1;
  x2 = -1;
  x3 = -1;
  for (int i = 0; i < DIM; i++) {
    if (i != new_const_i) {
      if (x1 < 0) {
        x1 = i;
      } else if (x2 < 0) {
        x2 = i;
      } else {
        x3 = i;
      }
    }
  }
  number_lines = 0;
  number_polygons = 0;
  ambiguous = false;
}

void Cube::split_to_squares() {
  std::array<std::array<double, STEPS>, STEPS> square;
  std::array<int, STEPS> c_i;
  std::array<double, STEPS> c_v;
  int number_squares = 0;
  for (int i = 0; i < DIM; i++) {
    // i is the index which is kept constant, thus we ignore the index which
    // is constant in this cube
    if (i == const_i) {
      continue;
    } else {
      c_i[0] = const_i;
      c_i[1] = i;
      for (int j = 0; j < STEPS; j++) {
        c_v[0] = const_value;
        c_v[1] = j * dx[i];
        for (int ci1 = 0; ci1 < STEPS; ci1++) {
          for (int ci2 = 0; ci2 < STEPS; ci2++) {
            if (i == x1) {
              square[ci1][ci2] = cube[j][ci1][ci2];
            } else if (i == x2) {
              square[ci1][ci2] = cube[ci1][j][ci2];
            } else {
              square[ci1][ci2] = cube[ci1][ci2][j];
            }
          }
        }
        squares[number_squares].init_square(square, c_i, c_v, dx);
        number_squares++;
      }
    }
  }
}

void Cube::construct_polygons(double value) {
  // Start by splitting the cube to squares and finding the lines
  split_to_squares();
  for (int i = 0; i < NSQUARES; i++) {
    squares[i].construct_lines(value);
  }
  // Then we make a table which contains references to the lines
  number_lines = 0;
  for (int i = 0; i < NSQUARES; i++) {
    int number_lines_temp = squares[i].get_number_lines();
    std::array<Line, Square::MAX_LINES>& lines_temp = squares[i].get_lines();
    for (int j = 0; j < number_lines_temp; j++) {
      lines[number_lines] = lines_temp[j];
      number_lines++;
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
    std::vector<bool> not_used(number_lines, true);
    // Keep track of the lines which are used
    int used = 0;
    do {
      if (number_lines - used < 3) {
        std::cerr << "Error: cannot construct polygon from "
                  << number_lines - used << " lines" << std::endl;
        exit(1);
      }
      // Initialize a new polygon
      polygons[number_polygons].init_polygon(const_i);
      // Go through all lines and try to add them to the polygon
      for (int i = 0; i < number_lines; i++) {
        if (not_used.at(i)) {
          // add_line returns true if line is successfully added
          if (polygons[number_polygons].add_line(lines[i], false)) {
            not_used.at(i) = false;
            used++;
            // If line is successfully added we start the loop from the
            // beginning
            i = 0;
          }
        }
      }
      // When we have reached this point one complete polygon is formed
      number_polygons++;
    } while (used < number_lines);
  } else {
    // Surface is not ambiguous, so we have only one polygon and all lines
    // can be added to it without ordering them
    polygons[number_polygons].init_polygon(const_i);
    for (int i = 0; i < number_lines; i++) {
      polygons[number_polygons].add_line(lines[i], true);
    }
    number_polygons++;
  }
}

void Cube::check_ambiguity(int number_lines) {
  // Check if any squares may have ambiguous elements
  for (int i = 0; i < NSQUARES; i++) {
    if (squares[i].is_ambiguous()) {
      ambiguous = true;
      return;
    }
  }
  // If the surface is not ambiguous already, it is still possible to
  // have a ambiguous case if we have exactly 6 lines, i.e. the surface
  // elements are at the opposite corners
  if (ambiguous == false && number_lines == 6) {
    ambiguous = true;
  }
}

bool Cube::is_ambiguous() { return ambiguous; }

int Cube::get_number_lines() { return number_lines; }

int Cube::get_number_polygons() { return number_polygons; }

std::array<Polygon, Cube::MAX_POLY>& Cube::get_polygons() { return polygons; }
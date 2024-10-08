#include "Square.h"

Square::Square() : ambiguous(false) {}

Square::~Square() = default;

void Square::init_square(
    std::array<std::array<double, SQUARE_DIM>, SQUARE_DIM>& sq,
    std::array<int, DIM - SQUARE_DIM>& c_i,
    std::array<double, DIM - SQUARE_DIM>& c_v, std::array<double, DIM>& dex) {
  points = sq;
  const_i = c_i;
  const_value = c_v;
  dx = dex;
  x1 = x2 = -1;
  for (int i = 0; i < DIM; i++) {
    if (i != const_i[0] && i != const_i[1]) {
      (x1 < 0 ? x1 : x2) = i;
    }
  }
  number_cuts = number_lines = 0;
  ambiguous = false;
}

void Square::construct_lines(double value) {
  // Check the corner points to see if there are lines
  int above = 0;
  for (int i = 0; i < DIM - SQUARE_DIM; i++) {
    for (int j = 0; j < DIM - SQUARE_DIM; j++) {
      if (points[i][j] >= value)
        above++;
    }
  }
  // If all corners are above or below this value, there are no lines in this
  // square
  if (above == 0 || above == 4) {
    number_lines = 0;
    return;
  }
  // Find the cut points and the points which are always outside of the
  // surface. Also find_outside() arranges cuts so that first two cuts form
  // a line as defined in the algorithm (in case there are 4 cuts)
  ends_of_edge(value);
  if (number_cuts > 0) {
    find_outside(value);
  }
  // Then we go through the cut points and form the line elements
  bool toggle = false;
  for (int i = 0; i < number_cuts; i++) {
    const int toggle_index = toggle ? 1 : 0;

    points_temp[toggle_index][x1] = cuts[i][0];
    points_temp[toggle_index][x2] = cuts[i][1];

    const int c0 = const_i[0];
    const int c1 = const_i[1];
    points_temp[toggle_index][c0] = const_value[0];
    points_temp[toggle_index][c1] = const_value[1];
    // If we inserted both endpoints we insert the outside point
    // and we are ready to create the line element
    if (toggle) {
      out_temp[x1] = out[i / 2][0];
      out_temp[x2] = out[i / 2][1];
      out_temp[c0] = const_value[0];
      out_temp[c1] = const_value[1];
      lines[number_lines++].init_line(points_temp, out_temp, const_i);
    }
    toggle = !toggle;  // Toggle between 0 and 1
  }
}

void Square::ends_of_edge(double value) {
  const double top_left = points[0][0] - value;
  const double top_right = points[0][1] - value;
  const double bottom_left = points[1][0] - value;
  const double bottom_right = points[1][1] - value;
  // Edge 1
  if (top_left * bottom_left < 0) {
    add_cut(std::array<double, SQUARE_DIM>{
        top_left / (points[0][0] - points[1][0]) * dx[x1], 0});
  } else if (points[0][0] == value && points[1][0] < value) {
    add_cut(std::array<double, SQUARE_DIM>{ALMOST_ZERO * dx[x1], 0});
  } else if (points[1][0] == value && points[0][0] < value) {
    add_cut(std::array<double, SQUARE_DIM>{ALMOST_ONE * dx[x1], 0});
  }

  // Edge 2
  if (top_left * top_right < 0) {
    add_cut(std::array<double, SQUARE_DIM>{
        0, top_left / (points[0][0] - points[0][1]) * dx[x2]});
  } else if (points[0][0] == value && points[0][1] < value) {
    add_cut(std::array<double, SQUARE_DIM>{0, ALMOST_ZERO * dx[x2]});
  } else if (points[0][1] == value && points[0][0] < value) {
    add_cut(std::array<double, SQUARE_DIM>{0, ALMOST_ONE * dx[x2]});
  }

  // Edge 3
  if (bottom_left * bottom_right < 0) {
    add_cut(std::array<double, SQUARE_DIM>{
        dx[x1], bottom_left / (points[1][0] - points[1][1]) * dx[x2]});
  } else if (points[1][0] == value && points[1][1] < value) {
    add_cut(std::array<double, SQUARE_DIM>{dx[x1], ALMOST_ZERO * dx[x2]});
  } else if (points[1][1] == value && points[1][0] < value) {
    add_cut(std::array<double, SQUARE_DIM>{dx[x1], ALMOST_ONE * dx[x2]});
  }

  // Edge 4
  if (top_right * bottom_right < 0) {
    add_cut(std::array<double, SQUARE_DIM>{
        top_right / (points[0][1] - points[1][1]) * dx[x1], dx[x2]});
  } else if (points[0][1] == value && points[1][1] < value) {
    add_cut(std::array<double, SQUARE_DIM>{ALMOST_ZERO * dx[x1], dx[x2]});
  } else if (points[1][1] == value && points[0][1] < value) {
    add_cut(std::array<double, SQUARE_DIM>{ALMOST_ONE * dx[x1], dx[x2]});
  }

  if (number_cuts != 0 && number_cuts != 2 && number_cuts != 4) {
    std::cerr << "Error in ends_of_edge: number_cuts " << number_cuts
              << std::endl;
    exit(1);
  }
}

void Square::find_outside(double value) {
  if (number_cuts == 4) {
    // If there are 4 cuts, the surface is ambiguous
    ambiguous = true;

    // Compute the value in the middle of the square
    double value_middle = 0.0;
    for (const auto& row : points) {
      for (double point : row) {
        value_middle += point;
      }
    }
    value_middle *= 0.25;
    // The default is that cuts are connected as \\ here.
    // If both value_middle and (0,0) are above or below the criterion
    // the cuts should be like // and we have to switch order in cuts
    // Determine if cuts need to be swapped
    if ((points[0][0] < value && value_middle < value) ||
        (points[0][0] > value && value_middle > value)) {
      std::swap(cuts[1], cuts[2]);
    }

    // The center is below, so the middle point is always outside the surface
    if (value_middle < value) {
      out[0][0] = 0.5 * dx[x1];
      out[0][1] = 0.5 * dx[x2];
      out[1][0] = 0.5 * dx[x1];
      out[1][1] = 0.5 * dx[x2];
    } else {  // The center is above
      // Cuts are \\ here so bottom left and top right corners are outside
      if (points[0][0] < value) {
        out[0][0] = 0;
        out[1][0] = dx[x1];
        out[0][1] = 0;
        out[1][1] = dx[x2];
        // Cuts are // here so bottom right and top left corners are outside
      } else {
        out[0][0] = dx[x1];
        out[0][1] = 0;
        out[1][0] = 0;
        out[1][1] = dx[x2];
      }
    }
  } else {
    // This is the normal case (not ambiguous)
    int number_out = 0;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        out[i][j] = 0.0;
        if (points[i][j] < value) {
          out[0][0] += i * dx[x1];
          out[0][1] += j * dx[x2];
          number_out++;
        }
      }
    }
    if (number_out > 0) {
      for (int i = 0; i < SQUARE_DIM; i++) {
        for (int j = 0; j < SQUARE_DIM; j++) {
          out[i][j] /= number_out;
        }
      }
    }
  }
}

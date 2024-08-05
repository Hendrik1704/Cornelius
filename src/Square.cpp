#include "Square.h"

#include <iostream>

Square::Square() { ambiguous = false; }

Square::~Square() {}

void Square::init_square(
    std::array<std::array<double, SQUARE_DIM>, SQUARE_DIM> sq,
    std::array<int, DIM - SQUARE_DIM> c_i,
    std::array<double, DIM - SQUARE_DIM> c_v, std::array<double, DIM> dx) {
  points = sq;
  const_i = c_i;
  const_value = c_v;
  dx = dx;
  x1 = -1;
  x2 = -1;
  number_cuts = 0;
  number_lines = 0;
  ambiguous = false;
}

void Square::construct_lines(double value) {
  // Check the corner points to see if there are lines
  int above = 0;
  for (int i = 0; i < DIM - SQUARE_DIM; i++) {
    for (int j = 0; j < DIM - SQUARE_DIM; j++) {
      if (points[i][j] >= value) {
        above++;
      }
    }
  }
  // If all corners are above or below this value, there are no lines in this
  // square
  if (above == 0 || above == 4) {
    number_lines = 0;
    return;
  }
  // Find the cut points and the points which are always outside of the
  // surface. Also find_outside() arranges cuts so that first two cuts form 7
  // a line as defined in the algorithm (in case there are 4 cuts)
  ends_of_edge(value);
  if (number_cuts > 0) {
    find_outside(value);
  }
  // Then we go through the cut points and form the line elements
  for (int i = 0; i < number_cuts; i++) {
    points_temp[i % 2][x1] = cuts[i][0];
    points_temp[i % 2][x2] = cuts[i][1];
    points_temp[i % 2][const_i[0]] = const_value[0];
    points_temp[i % 2][const_i[1]] = const_value[1];
    // If we inserted both endpoints we insert the outside point
    //  and we are ready to create the line element
    if (i % 2 == 1) {
      out_temp[x1] = out[i / 2][0];
      out_temp[x2] = out[i / 2][1];
      out_temp[const_i[0]] = const_value[0];
      out_temp[const_i[1]] = const_value[1];
      lines[i / 2].init_line(points_temp, out_temp, const_i);
      number_lines++;
    }
  }
}

void Square::ends_of_edge(double value) {
  // Edge 1
  if ((points[0][0] - value) * (points[1][0] - value) < 0) {
    cuts[number_cuts][0] =
        (points[0][0] - value) / (points[0][0] - points[1][0]) * dx[x1];
    cuts[number_cuts][1] = 0;
    number_cuts++;
  } else if (points[0][0] == value && points[1][0] < value) {
    cuts[number_cuts][0] = 1e-9 * dx[x1];
    cuts[number_cuts][1] = 0;
    number_cuts++;
  } else if (points[1][0] == value && points[0][0] < value) {
    cuts[number_cuts][0] = (1.0 - 1e-9) * dx[x1];
    cuts[number_cuts][1] = 0;
    number_cuts++;
  }
  // Edge 2
  if ((points[0][0] - value) * (points[0][1] - value) < 0) {
    cuts[number_cuts][0] = 0;
    cuts[number_cuts][1] =
        (points[0][0] - value) / (points[0][0] - points[0][1]) * dx[x2];
    number_cuts++;
  } else if (points[0][0] == value && points[0][1] < value) {
    cuts[number_cuts][0] = 0;
    cuts[number_cuts][1] = 1e-9 * dx[x2];
    number_cuts++;
  } else if (points[0][1] == value && points[0][0] < value) {
    cuts[number_cuts][0] = 0;
    cuts[number_cuts][1] = (1.0 - 1e-9) * dx[x2];
    number_cuts++;
  }
  // Edge 3
  if ((points[1][0] - value) * (points[1][1] - value) < 0) {
    cuts[number_cuts][0] = dx[x1];
    cuts[number_cuts][1] =
        (points[1][0] - value) / (points[1][0] - points[1][1]) * dx[x2];
    number_cuts++;
  } else if (points[1][0] == value && points[1][1] < value) {
    cuts[number_cuts][0] = dx[x1];
    cuts[number_cuts][1] = 1e-9 * dx[x2];
    number_cuts++;
  } else if (points[1][1] == value && points[1][0] < value) {
    cuts[number_cuts][0] = dx[x1];
    cuts[number_cuts][1] = (1.0 - 1e-9) * dx[x2];
    number_cuts++;
  }
  // Edge 4
  if ((points[0][1] - value) * (points[1][1] - value) < 0) {
    cuts[number_cuts][0] =
        (points[0][1] - value) / (points[0][1] - points[1][1]) * dx[x1];
    cuts[number_cuts][1] = dx[x2];
    number_cuts++;
  } else if (points[0][1] == value && points[1][1] < value) {
    cuts[number_cuts][0] = 1e-9 * dx[x1];
    cuts[number_cuts][1] = dx[x2];
    number_cuts++;
  } else if (points[1][1] == value && points[0][1] < value) {
    cuts[number_cuts][0] = (1.0 - 1e-9) * dx[x1];
    cuts[number_cuts][1] = dx[x2];
    number_cuts++;
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
    double value_middle =
        (points[0][0] + points[1][0] + points[0][1] + points[1][1]) / 4.0;

    // The default is that cuts are connected as \\ here.
    // If both value_middle and (0,0) are above or below the criterion
    // the cuts should be like // and we have to switch order in cuts
    if ((points[0][0] < value && value_middle < value) ||
        (points[0][0] > value && value_middle > value)) {
      for (int i = 0; i < 2; i++) {
        double temp = cuts[1][i];
        cuts[1][i] = cuts[2][i];
        cuts[2][i] = temp;
      }
    }
    // The center is below, so the middle point is always outside the surface
    if ((value_middle - value) < 0) {
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          if (j == 0)
            out[i][j] = 0.5 * dx[x1];
          else
            out[i][j] = 0.5 * dx[x2];
        }
      }
    } else {  // The center is above
      // Cuts are \\ here so bottom left and top right corners are outside
      if ((points[0][0] - value) < 0) {
        for (int i = 0; i < 2; i++) {
          out[0][i] = 0;
          if (i == 0)
            out[1][i] = dx[x1];
          else
            out[1][i] = dx[x2];
        }
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
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        out[i][j] = 0;
      }
    }
    int number_out = 0;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        if (points[i][j] < value) {
          out[0][0] += i * dx[x1];
          out[0][1] += j * dx[x2];
          number_out++;
        }
      }
    }
    if (number_out > 0) {
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          out[i][j] = out[i][j] / double(number_out);
        }
      }
    }
  }
}

bool Square::is_ambiguous() { return ambiguous; }

int Square::get_number_lines() { return number_lines; }

std::array<Line, Square::MAX_LINES>& Square::get_lines() { return lines; }

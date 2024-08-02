#ifndef LINE_H
#define LINE_H

#include "GeneralGeometryElement.h"

class Line : public GeneralGeometryElement {
protected:
    static const int LINE_DIM = 2;
    static const int LINE_CORNERS = 2;

    int x1, x2;
    int start_point;
    int end_point;
    std::array<std::array<double, DIM>, LINE_DIM> corners;
    std::array<double, DIM> out;
    std::array<double, DIM-LINE_DIM> const_i;

public:
    Line();
    ~Line();

    void init_line(std::array<std::array<double, DIM>, LINE_DIM> new_corners, std::array<double, DIM> new_out, std::array<double, DIM-LINE_DIM> new_const_i);

    void flip_start_end();
    void calculate_normal();
    void calculate_centroid();

    // Getter functions for the start, end, and out points
    std::array<double, GeneralGeometryElement::DIM>& get_start_point();
    std::array<double, GeneralGeometryElement::DIM>& get_end_point();
    std::array<double, GeneralGeometryElement::DIM>& get_out_point();

};

#endif // LINE_H
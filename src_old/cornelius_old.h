#ifndef CORNELIUS_OLD_H
#define CORNELIUS_OLD_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>

using namespace std;

/**
 *
 * General class for elements. All other element classes are inherited
 * from this.
 *
 */
class GeneralElementOld
{
  protected:
    static const int DIM = 4;
    double *centroid;
    double *normal;
    int normal_calculated;
    int centroid_calculated;
    virtual void calculate_centroid() {};
    virtual void calculate_normal() {};
    void check_normal_direction(double *normal, double *out);
  public:
    GeneralElementOld();
    ~GeneralElementOld();
    double *get_centroid();
    double *get_normal();
};

/**
 *
 * This class is used for line elements gotten from the squares. Can
 * calculate the centroid and normal of the line.
 *
 */
class LineOld : public GeneralElementOld
{
  private:
    static const int LINE_CORNERS = 2;
    static const int LINE_DIM = 2;
    int x1,x2;
    int start_point;
    int end_point;
    double **corners;
    double *out;
    int *const_i;
    void calculate_centroid();
    void calculate_normal();
  public:
    LineOld();
    ~LineOld();
    void init(double**,double*,int*);
    void flip_start_end();
    double *get_start();
    double *get_end();
    double *get_out();
};

/**
 *
 * A class for storing polygons given by CorneliusOld. Can calculate
 * the centroid and normal of the polygon.
 *
 */
class PolygonOld : public GeneralElementOld
{
  private:
    static const int MAX_LINES = 24;
    static const int POLYGON_DIM = 3;
    LineOld **lines;
    int Nlines;
    int x1,x2,x3;
    int const_i;
    void calculate_centroid();
    void calculate_normal();
  public:
    PolygonOld();
    ~PolygonOld();
    void init(int);
    bool add_line(LineOld*,int);
    int get_Nlines();
    LineOld** get_lines();
    void print(ofstream &file,double*);
};

/**
 *
 * A class for storing polyhedrons given by CorneliusOld. Can calculate
 * the normal and centroid of the polyhedron.
 *
 */
class PolyhedronOld : public GeneralElementOld
{
  private:
    static const int MAX_POLYGONS = 24;
    PolygonOld **polygons;
    int Npolygons;
    int Ntetrahedra;
    int x1,x2,x3,x4;
    bool lines_equal(LineOld*,LineOld*);
    void tetravolume(double*,double*,double*,double*);
    void calculate_centroid();
    void calculate_normal();
  public:
    PolyhedronOld();
    ~PolyhedronOld();
    void init();
    bool add_polygon(PolygonOld*,int);
};

/**
 *
 * This class handles the squares which are splitted from the cube.
 * Finds the edges of the surface and also the poinst which point
 * the outward direction.
 *
 * 13.10.2011 Hannu Holopainen
 *
 */
class SquareOld
{
  private:
    static const int DIM = 4;
    static const int SQUARE_DIM = 2;
    static const int MAX_POINTS = 4;
    static const int MAX_LINES = 2;
    double **points;
    double **cuts;
    double **out;
    double **points_temp;
    double *out_temp;
    int *const_i;
    double *const_value;
    int x1, x2;
    double *dx;
    int Ncuts;
    int Nlines;
    LineOld *lines;
    int ambiguous;
    void ends_of_edge(double);
    void find_outside(double);
  public:
    SquareOld();
    ~SquareOld();
    void init(double**,int*,double*,double*);
    void construct_lines(double);
    int is_ambiguous();
    int get_Nlines();
    LineOld* get_lines();
};

/**
 *
 * This class handles 3d-cubes. Splits them into squares and collects
 * polygons from the lines given from squares.
 *
 * 13.10.2011 Hannu Holopainen
 *
 */
class CubeOld
{
  private:
    static const int DIM = 4;
    static const int CUBE_DIM = 4;
    static const int MAX_POLY = 8;
    static const int NSQUARES = 6;
    static const int STEPS = 2;
    double ***cube;
    LineOld **lines;
    PolygonOld *polygons;
    SquareOld *squares;
    int Nlines;
    int Npolygons;
    int ambiguous;
    int const_i;
    double const_value;
    int x1,x2,x3;
    double *dx;
    void split_to_squares();
    void check_ambiguous(int);
  public:
    CubeOld();
    ~CubeOld();
    void init(double***&,int,double,double*&);
    void construct_polygons(double);
    int get_Nlines();
    int get_Npolygons();
    int is_ambiguous();
    PolygonOld* get_polygons();
};


/**
 *
 * This class handles 4d-cubes. Splits them into squares and collects
 * polygons from the lines given from squares.
 *
 * 13.10.2011 Hannu Holopainen
 *
 */
class HypercubeOld
{
  private:
    static const int DIM = 4;
    static const int MAX_POLY = 10;
    static const int NCUBES = 8;
    static const int STEPS = 2;
    double ****hcube;
    PolyhedronOld *polyhedrons;
    PolygonOld **polygons;
    CubeOld *cubes;
    int Npolyhedrons;
    int ambiguous;
    int x1,x2,x3,x4;
    double *dx;
    void split_to_cubes();
    void check_ambiguous(double);
  public:
    HypercubeOld();
    ~HypercubeOld();
    void init(double****,double*);
    void construct_polyhedrons(double);
    int get_Npolyhedrons();
    PolyhedronOld* get_polyhedrons();
};

/**
 *
 * A class for finding a constant value surface from a 2-4 dimensional
 * cube. The values at corners and length of the side are needed as
 * an input.
 *
 * Algorithm by Pasi Huovinen. This code is based on the original FORTRAN
 * code by Pasi Huovinen.
 *
 * 23.04.2012 Hannu Holopainen
 *
 */
class CorneliusOld
{
  private:
    static const int STEPS = 2;
    static const int DIM = 4;
    static const int MAX_ELEMENTS = 10;
    int Nelements;
    double **normals;
    double **centroids;
    int cube_dim;
    int initialized;
    int print_initialized;
    double value0;
    double *dx;
    ofstream output_print;
    void surface_3d(double***,double*,int);
    SquareOld cu2d;
    CubeOld cu3d;
    HypercubeOld cu4d;
  public:
    CorneliusOld();
    ~CorneliusOld();
    void init(int,double,double*);
    void init_print(string);
    void find_surface_2d(double**);
    void find_surface_3d(double***);
    void find_surface_3d_print(double***,double*);
    void find_surface_4d(double****);
    int get_Nelements();
    double **get_normals();
    double **get_centroids();
    double **get_normals_4d();
    double **get_centroids_4d();
    double get_centroid_elem(int,int);
    double get_normal_elem(int,int);
};

#endif /* CORNELIUS_H */

// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
/***********************************************************************
  FILE: GridPoint.h
  Copyright (C) 1998 by Edward Earl

  This is the C++ header file that supports the services
  of the grid point classes.
 ***********************************************************************/

#ifndef _GRID_POINT_
#define _GRID_POINT_

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "file.h"


// First, a few general tidbits ...
typedef long int Code_stamp;
typedef float Distance;
typedef short int Elevation;
static const Elevation elev_undef=-16384;
static const double sec2rad=3.14159265358979/648000L; // seconds to radians conversion factor
#define coord_infty Interval<GridPoint::Coord>::infty

// nearest integer (helper function)
inline static long nint(double x)
{
  return (long)(x + (x>0 ? 0.5 : -0.5));
}


/***********************************************************************
  Conceptually, A "grid point" is an index into a map of discrete
  grid points.  There are many kinds of grid points corresponding to
  different types of grids (e.g. cartesian, equatorial, UTM).
 ***********************************************************************/


class GridPoint_cart {
/***********************************************************************
  Description of a cartesian grid point, whose grid is the points in the x-y
  plane whose coodinates are integers.
 ***********************************************************************/
public:
  typedef signed long int Coord;
  Coord x,y;
  GridPoint_cart(Coord x_, Coord y_) : x(x_),y(y_) {}
  static const GridPoint_cart undefined;
  static const char *format,*field_format,*coord_format,
    *undef_string,*hdr_string;
  static const char *unit;
  static const Code_stamp code;
  static Coord quad_int_x,quad_int_y;
  static const Distance huje; // huge is a reserved word on some platforms

  GridPoint_cart() {undefine();} // default constructor produces "undefined" grid point
  void undefine() {x=y=-1073741824l;} // fill grid point with "undefined" value
  Coord parity() const {return (x-y)%2;}
  unsigned priority() const {return (x&0x1)|(((x^y)&0x1)<<1);}
  double area() const {return 1.0;}
  bool quad_corner() const
    {return x%quad_int_x==0 && y%quad_int_y==0;}
  bool quad_edge() const
    {return x%quad_int_x==0 || y%quad_int_y==0;}
  bool operator==(const GridPoint_cart& c) const {return x==c.x && y==c.y;}
  bool operator!=(const GridPoint_cart& c) const {return !(*this==c);}
  bool operator<(const GridPoint_cart& c) const
    {return x<c.x || (x==c.x && y<c.y);}
  bool operator>(const GridPoint_cart& c) const
    {return x>c.x || (x==c.x && y>c.y);}
  Distance rel_dist(const GridPoint_cart& a) const
    {Coord dx=x-a.x,dy=y-a.y; return (Distance)(dx*dx+dy*dy);}
  Distance operator-(const GridPoint_cart& a) const
    {return (Distance)sqrt(rel_dist(a));}
  double direction(const GridPoint_cart&) const;
  GridPoint_cart midpoint(const GridPoint_cart& a) const
    {return GridPoint_cart((x+a.x)/2,(y+a.y)/2);}
  void set_cart(Coord x_, Coord y_) {x=x_; y=y_;}
  operator bool() const {return *this!=undefined;}
  static void write_quad(FILE *f) throw (file_error);
  static void read_quad(FILE *f) throw (file_error);
  static void write_stamp(FILE *f) throw (file_error);
  static void read_stamp(FILE *f) throw (file_error);
  void input();
  char *sprint(char *) const; // print a grid point to a string
  void print(FILE *f=stdout) const; // print a grid point to a formatted file
  void trace() const; // debugging trace
  char *sprint1(char *buf) const {return buf+sprintf(buf,coord_format,x);}
  char *sprint2(char *buf) const {return buf+sprintf(buf,coord_format,y);}
  void print1(FILE *f=stdout) const {fprintf(f,coord_format,x);}
  void print2(FILE *f=stdout) const {fprintf(f,coord_format,y);}
  static const char *coord1_label,*coord2_label,
    *bound1_low,*bound1_high,*bound2_low,*bound2_high;
  static void print_header(FILE *f=stdout)
    {fprintf(f,field_format,hdr_string);}
};


// more tidbits
extern GridPoint_cart::Coord sscansec(const char *, char, char);
extern char *sprint_angle(char *, GridPoint_cart::Coord, char, char);
extern void print_angle(FILE *, GridPoint_cart::Coord, char, char);

enum Angle_fmt {AF_DEGREE,AF_DM,AF_DMS,AF_RADIAN,AF_GRAD};


class GridPoint_equat : public GridPoint_cart {
/***********************************************************************
  Description of an equatorial grid point, whose grid is the points on the
  earth's surface whose latitiude and longitude, when measured in seconds
  of arc, are multiples of lat_step and lon_step.  Except for the following
  differences, an equatorial grid point is identical to a cartesian grid point:
  1) It is printed in degrees, minutes, and seconds rather than as a simple
     (x,y) coordinate pair.
  2) It has a constructor that accepts coordinates (in seconds of arc).
  3) Its area is returned in square kilometers.  The unit may be changed
     by adjusting the "radius" static data member so that it expresses the
     radius of the earth in the desired units.
 ***********************************************************************/
  friend class GridSet_equat;
public:
  static const Code_stamp code;
  static Coord lat_step,lon_step; // latitude and longitude grid point intervals
  static Distance radius; // radius of earth
  static const char *unit;
  static const char *format;
  static const char *real_format;
  static Angle_fmt angle_format;
  GridPoint_equat() {}
  GridPoint_equat(const GridPoint_cart& c) : GridPoint_cart(c) {}
  GridPoint_equat(Coord lat_, Coord lon_) {set_sec(lat_,lon_);}

  Distance rel_dist(const GridPoint_equat&) const;
  Distance operator-(const GridPoint_equat&) const;
  Distance xstep() const {return lat_step*sec2rad*radius;}
  Distance ystep() const
    {return lon_step*sec2rad*radius*cos(x*lat_step*sec2rad);}
  double direction(const GridPoint_equat&) const;
  GridPoint_equat midpoint(const GridPoint_cart& a) const
    {GridPoint_equat z; z.x=(x+a.x)/2,z.y=(y+a.y)/2; return z;}
  Coord lat() const {return x*lat_step;}
  Coord lon() const {return y*lon_step;}
  void set_sec(Coord lat_, Coord lon_)
    {x=nint(lat_/(double)lat_step); y=nint(lon_/(double)lon_step);}
  bool set_UTM(int, double, double);
  void get_UTM(int&, double&, double&) const;
  Distance ever_radius(const GridPoint_equat& c) const
    {return (Distance)(labs(x-c.x)*lat_step*sec2rad*radius);}

  static void sprintx(char *buf, Coord x)
    {sprint_angle(buf,x*lat_step,' ',' ');}
  static void sprinty(char *buf, Coord y)
    {sprint_angle(buf,y*lon_step,' ',' ');}
  static void sprint_quadx(char *buf) {sprintx(buf,quad_int_x);}
  static void sprint_quady(char *buf) {sprinty(buf,quad_int_y);}
  static void sprint_xstep(char *buf) {sprintx(buf,1);}
  static void sprint_ystep(char *buf) {sprinty(buf,1);}
  static Coord sscanx(const char *);
  static Coord sscany(const char *);
  //static Coord inputsec(const char *);

  double area() const
    {return cos(x*lat_step*sec2rad)*lat_step*lon_step*
	sec2rad*sec2rad*radius*radius;}

  static void write_stamp(FILE *);
  static void read_stamp(FILE *);
  static void clear_step();
  void input();
  char *sprint(char *) const;
  void print(FILE *f=stdout) const;
  void trace() const;
  char *sprint1(char *buf) const {return sprint_angle(buf,lat(),'N','S');}
  char *sprint2(char *buf) const {return sprint_angle(buf,lon(),'E','W');}
  void print1(FILE *f=stdout) const {print_angle(f,lat(),'N','S');}
  void print2(FILE *f=stdout) const {print_angle(f,lon(),'E','W');}
  static const char *coord1_label,*coord2_label,
    *bound1_low,*bound1_high,*bound2_low,*bound2_high;
  char *sprint_lat(char *buf) const {return sprint1(buf);}
  char *sprint_lon(char *buf) const {return sprint2(buf);}
};


class GridPoint_polar : public GridPoint_cart {
/***********************************************************************
  Description of a terrestrial grid point in a polar stereographic projection.
  Each grid point is a point whose x,y coordinates relative to the nearest pole
  are multiples of the "step" member.  It differs from a cartesian grid point
  in ways analogous to an equatorial grid point.
 ***********************************************************************/
  friend class GridSet_polar;
  static Coord scan_coord(const char *);
  static double ps_radius(double, double);
public:
  static const Code_stamp code;
  static Distance step; // grid point interval
  static Distance radius; // equatorial radius of earth
  static double eps,E2,E; // eccentricity of earth's polar cross section
  static double scale; // adjust this to make the desired latitude (71S for ADD) the latitude of true scale
  static const char *unit;
  static const char *format;
  static Angle_fmt angle_format;
  static const char *real_format;
  GridPoint_polar() {}
  GridPoint_polar(const GridPoint_cart& c) : GridPoint_cart(c) {}
  GridPoint_polar(Coord lat_, Coord lon_) {set_sec(lat_,lon_);}

  Distance rel_dist(const GridPoint_polar& a) const
    {return GridPoint_cart::rel_dist(a)*step*step;}
  Distance operator-(const GridPoint_polar& a) const
    {return GridPoint_cart::operator-(a)*step;}
  Distance xstep() const {return step;}
  Distance ystep() const {return step;}
  double direction(const GridPoint_polar&) const;
  GridPoint_polar midpoint(const GridPoint_cart& a) const
    {GridPoint_polar z; z.x=(x+a.x)/2,z.y=(y+a.y)/2; return z;}
  Coord lat() const;
  Coord lon() const {return atan2((float)y,(float)x)/sec2rad;}
  void set_sec(Coord, Coord);
  bool set_UTM(int, double, double);
  void get_UTM(int&, double&, double&) const;
  Distance ever_radius(const GridPoint_polar& c) const
    {return (Distance)(labs(x-c.x)*step);}

  static void sprintx(char *buf, Coord x)
    {sprintf(buf,"%7.3f",x*step);}
  static void sprinty(char *buf, Coord y)
    {sprintf(buf,"%7.3f",y*step);}
  static void sprint_quadx(char *buf) {sprintx(buf,quad_int_x);}
  static void sprint_quady(char *buf) {sprinty(buf,quad_int_y);}
  static void sprint_xstep(char *buf) {sprintx(buf,1);}
  static void sprint_ystep(char *buf) {sprinty(buf,1);}
  static Coord sscanx(const char *buf) {return scan_coord(buf);}
  static Coord sscany(const char *buf) {return scan_coord(buf);}
  //static Coord inputsec(const char *);

  double area() const {return step*step;} // consider varying latitude scale
  static void write_stamp(FILE *);
  static void read_stamp(FILE *);
  static void clear_step();
  void input();
  char *sprint(char *) const;
  void print(FILE *f=stdout) const;
  void trace() const;
  char *sprint_lat(char *buf) const {return sprint_angle(buf,lat(),'N','S');}
  char *sprint_lon(char *buf) const {return sprint_angle(buf,lon(),'E','W');}
  static const char *coord1_label,*coord2_label,
    *bound1_low,*bound1_high,*bound2_low,*bound2_high;
  void print_lat(FILE *f=stdout) const {print_angle(f,lat(),'N','S');}
  void print_lon(FILE *f=stdout) const {print_angle(f,lon(),'E','W');}
};


typedef GridPoint_equat GridPoint;
//typedef GridPoint_polar GridPoint;


#endif // ndef _GRID_POINT_

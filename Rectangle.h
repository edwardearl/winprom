#ifndef _RECTANGLE_
#define _RECTANGLE_


#include "GridPoint.h"
#include "block.h"

struct Rectangl : Block<GridPoint_cart::Coord,2> {
/***********************************************************************
  This class is exactly what its name implies.  It has many uses, but most
  of them pertain to defining bounds on regions and iterators.  
 ***********************************************************************/
  typedef GridPoint_cart::Coord Coord;

  Rectangl() {}
  Rectangl(Coord x1, Coord x2, Coord y1, Coord y2)
    {limit[0].set(x1,x2); limit[1].set(y1,y2);}
  Rectangl(const GridPoint_cart& c) {set(c,c);}
  Rectangl(const GridPoint_cart& c1, const GridPoint_cart& c2) {set(c1,c2);}

  void set(const GridPoint_cart& c1, const GridPoint_cart& c2)
    {limit[0].set(c1.x,c2.x); limit[1].set(c1.y,c2.y);}

  // check if c is within this rectangle
  bool contains(const GridPoint_cart& c) const
    {return Block<Coord,2>::contains(c.x,c.y);}
  bool contains(const Rectangl& r) const
    {return Block<Coord,2>::contains(r);}

  Rectangl& operator|=(const Rectangl& a)
    {(Block<Coord,2> &)(*this)|=a; return *this;}
  Rectangl& operator|=(const GridPoint_cart& c)
    {expand(c.x,c.y); return *this;}
  Rectangl& operator&=(const Rectangl& a)
    {(Block<Coord,2> &)(*this)&=a; return *this;}
  Rectangl operator&(const Rectangl& a) const
    {Rectangl b(*this); b&=a; return b;}

  // check if c is on the edge of a rectangle
  bool on_edge(const GridPoint_cart& c) const
    {return Block<Coord,2>::on_edge(c.x,c.y);}

  // check if c is on the corner of a rectangle
  bool on_corner(const GridPoint_cart& c) const
    {return Block<Coord,2>::on_corner(c.x,c.y);}

  GridPoint_cart corner1() const // "lower left" corner
    {return GridPoint_cart(limit[0].low,limit[1].low);}
  GridPoint_cart corner2() const // "upper right" corner
    {return GridPoint_cart(limit[0].high,limit[1].high);}
  Coord xsize() const {return size(0)+1;} // X dimension
  Coord ysize() const {return size(1)+1;} // Y dimension
  Coord area() const {return xsize()*ysize();}
  Coord perimeter() const {return 2*(size(0)+size(1));}

  void print(FILE *f=stdout) const;

};


class Rect_iter_all {
/***********************************************************************
  This class is used to iterate over _all_ points on a Cartesian grid.
  Its semantics are the same as those of neighbor iterators except that
  the constructor accepts a grid argument instead of a grid point argument,
  the reset() operation does not accept a grid point parameter, and the
  center() operation is not available.
 ***********************************************************************/
  Rect_iter_all& operator++(int);
  Rect_iter_all& operator--(int);
public:
  typedef GridPoint_cart GridPoint;
protected:
  Rectangl bound;
  GridPoint current;
public:
  Rect_iter_all(const Rectangl& r) : bound(r) {reset();}
  Rect_iter_all& operator=(const Rect_iter_all&);
  Rect_iter_all& operator++();
  Rect_iter_all& operator--();
  const GridPoint& operator*() const {return current;}
  operator bool() const {return bound.contains(current);}
  bool bol() const {return current.y==bound.limit[1].low;}
  bool eol() const {return current.y==bound.limit[1].high;}
  void reset()  {current.x=bound.limit[0].low; current.y=bound.limit[1].low;}
  void endset() {current.x=bound.limit[0].high; current.y=bound.limit[1].high;}
};

class Rect_iter_edge : public Rect_iter_all {
/***********************************************************************
  This class is used to iterate over all cartesian grid points on the edge
  of a bounding rectangle.  Its semantics are the same as those of grid
  iterators.  It has one additional feature: it can be made to "close"; i.e.
  it makes a second stop on the initial grid point at the end of its loop.
 ***********************************************************************/
  unsigned side; // which side of the rectangle (0-3) is the iterator currently on?
  bool closed;
  Rect_iter_edge& operator++(int);
public:
  Rect_iter_edge(const Rectangl& r, bool c=false) :
    Rect_iter_all(r) {reset(c);}
  Rect_iter_edge& operator++();
  operator bool() const {return side<4;}
  bool on_corner() const {return bound.on_corner(current);}
  void reset() {Rect_iter_all::reset(); side=0;}
  void reset(bool c) {reset(); closed=c;}
};

#endif // ndef _RECTANGLE_

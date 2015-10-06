// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#ifndef _GRID_
#define _GRID_

#include "grid_map.h"

typedef Map_cart<Elevation> Elev_map;


/***********************************************************************
  GRID SETS
  A "grid set" is a grid without the neighbor relation.
  Every grid set still supports an all-point iterator, edge iterator,
  and a containment test.
 ***********************************************************************/

class GridSet_cart {
  // Cartesian grid set
protected:
  const Rectangl bound;
public:
  GridSet_cart(const Rectangl& r) : bound(r) {}
  typedef GridPoint_cart GridPoint;
  operator Rectangl() const {return bound;}
  typedef Rect_iter_all Iter;
  typedef Rect_iter_edge EdgeIter;
  bool contains(const GridPoint& g) const {return bound.contains(g);}
};

template <class GridPoint_geo>
class GridSet_geo : public GridSet_cart {
  // Geographic grid set
  int lat_step,lon_step;
  void set_step() const {}
public:
  GridSet_geo(const Rectangl& r) : GridSet_cart(r) {}
  Distance nearest_edge(const GridPoint_geo&) const;
  class Iter : public Rect_iter_all {
  public:
    Iter(const GridSet_geo& a) : Rect_iter_all(a) {}
    Iter(const Rectangl& a) : Rect_iter_all(a) {}
    const GridPoint_geo& operator*() const {return (GridPoint_geo&)current;}
  };
  class EdgeIter : public Rect_iter_edge {
  public:
    EdgeIter(const GridSet_geo& a) : Rect_iter_edge(a) {}
    EdgeIter(const Rectangl& a, bool c=false) : Rect_iter_edge(a,c) {}
    const GridPoint_geo& operator*() const {return (GridPoint_geo&)current;}
  };
};

template <class GridSet, class Elev_map>
class GridSet_elev : public GridSet {
  // Grid set including only grid points at which elevation is defined
  const Elev_map& elev;
public:
  //typedef Grid::GridPoint GridPointx;
  typedef GridPoint_equat GridPointx;
  GridSet_elev(const Elev_map& e) : GridSet(e),elev(e) {}
  GridSet_elev(const Elev_map& e, const Rectangl& r) :
    GridSet(r),elev(e) {assert(e.contains(r));}
  bool contains(const GridPoint& g) const
    {return GridSet::contains(g) && elev[g]!=elev_undef;}
  class Iter : public GridSet::Iter {
    const GridSet_elev& grid_set;
    void next_stop()
      {while (*this && !grid_set.contains(**this)) ++((GridSet::Iter&)(*this));}
    void prev_stop();
  public:
    Iter(const GridSet_elev& a) : GridSet::Iter(a),grid_set(a) {next_stop();}
    Iter& operator++() {++((GridSet::Iter&)(*this)); next_stop(); return *this;}
    Iter& operator--() {GridSet::Iter::operator--(); prev_stop();}
    void reset()  {((GridSet::Iter&)(*this)).reset(); next_stop();}
    void endset() {GridSet::Iter::endset(); prev_stop();}
  };
  class EdgeIter : public GridSet::EdgeIter {
    const GridSet_elev& grid_set;
    void next_stop();
  public:
    EdgeIter(const GridSet_elev& a) :
      GridSet::EdgeIter(a),grid_set(a) {next_stop();}
    EdgeIter& operator++() {GridSet::EdgeIter::operator++(); next_stop();}
    void reset() {GridSet::EdgeIter::reset(); next_stop();}
  };
};

template <class GridSet, class Elev_map>
void GridSet_elev<GridSet,Elev_map>::Iter::prev_stop()
{
  while (*this && !elev.contains(**this)) --*this;
}


/***********************************************************************
  A neighbor iterator is an object that iterates over all the neighbors
  of a grid point.  All neighbor iterators support the following operations:

	neighbor_iter(const GridPoint& center), the constructor, which prepares
	  the iterator for iteration over all neighbors of the given center grid
	  point.
	void operator*() const, which returns a grid point that indicates the
	  current position of the iterator.
	neighbor_iter operator++(), which advances the iterator to the next
	  neighbor.
	operator bool() const, which returns true if and only if the iterator
	  has not yet exhausted all neighbors.  If operator bool() returns
	  false, operator*() and operator++() are invalid.
	const GridPoint& center() const, which returns the grid point whose
	  neighbors are being iterated over.
	void reset(const GridPoint& center), which resets the iterator for
	  iteration over all neigbors of a new center grid point.

  Every neighbor iterator has a typedef GridPoint, which indicates the type
  of grid point to be used with the iterator.

  All neighbor iterators are useable (i.e. positioned at the first neighbor)
  as soon as they are created.
 ***********************************************************************/

class Nbr_iter_cart_diag {
/***********************************************************************
  This class represents a diagonal cartesian neighbor iterator.  A
  diagonal iterator recognizes the 8 nearest neighbors of a cartesian
  grid point. 
 ***********************************************************************/
protected:
  enum {START,RIGHT=START,UP_R,UP,UP_L,LEFT,DOWN_L,DOWN,DOWN_R,END} direction;
  void set_neighbor() {
    neighbor.x=ctr.x+dx[direction];
    neighbor.y=ctr.y+dy[direction];
  }
public:
  typedef GridPoint_cart GridPoint;
private:
  GridPoint ctr,neighbor;
  static const int dx[END],dy[END];
  Nbr_iter_cart_diag& operator++(int);
public:
  Nbr_iter_cart_diag() : direction(END) {}
  Nbr_iter_cart_diag(const GridPoint& c) {reset(c);}
  const GridPoint& operator*() const {return neighbor;}
  operator bool() const {return direction<END;}
  const GridPoint& center() const {return ctr;}
  Nbr_iter_cart_diag& operator++()
    {(int &)direction+=1; set_neighbor(); return *this;}
  void reset(const GridPoint& c) {ctr=c; direction=START; set_neighbor();}
  Distance dist() const {return ctr-neighbor;}
  bool over_triangl() const {return (bool)(direction&0x01);}
  static bool is_neighbor(const GridPoint& c1, const GridPoint& c2)
    {return labs(c1.x-c2.x)<=1 && labs(c1.y-c2.y)<=1 && c1!=c2;}
  static bool ever_neighbor(const GridPoint& c1, const GridPoint& c2)
    {return labs(c1.x-c2.x)<=1;}
};

class Nbr_iter_cart_ortho : public Nbr_iter_cart_diag {
/***********************************************************************
  This class represents an orthogonal cartesian neighbor iterator.
  An orthogonal iterator recognizes only the 4 nearest neighbors of a
  cartesian grid point.
 ***********************************************************************/
public:
  Nbr_iter_cart_ortho(const GridPoint& c) : Nbr_iter_cart_diag(c) {}
  Nbr_iter_cart_ortho& operator++()
    {(int &)direction+=2; set_neighbor(); return *this;}
  static bool is_neighbor(const GridPoint& c1, const GridPoint& c2)
    {return labs(c1.x-c2.x)+labs(c1.y-c2.y)==1;}
};

template <class GridPoint_geo>
class Nbr_iter_geo_diag : public Nbr_iter_cart_diag {
public:
  Nbr_iter_geo_diag() {}
  Nbr_iter_geo_diag(const GridPoint_geo& c) : Nbr_iter_cart_diag(c) {}
  const GridPoint_geo& operator*() const
    {return (const GridPoint_geo&)(Nbr_iter_cart_diag::operator*());}
  const GridPoint_geo& center() const
    {return (const GridPoint_geo&)(Nbr_iter_cart_diag::center());}
  Distance dist() const {return center()-(**this);}
};

template <class Neighbor_iter, class GridSet>
class Conditional_nbr_iter : public Neighbor_iter {
/***********************************************************************
  This class behaves exactly like its Neighbor_iter parameter, except that
  the iterator stops only on neighbors that are members of its GridSet
  parameter.
 ***********************************************************************/
  const GridSet &grid_set;
  void next_stop();
public:
  typedef GridPoint_equat GridPoint;
  Conditional_nbr_iter(const GridSet& gs) : grid_set(gs) {}
  Conditional_nbr_iter(const GridSet& gs, const GridPoint& c) :
    Neighbor_iter(c),grid_set(gs) {next_stop();}
  void reset(const GridPoint& c) {Neighbor_iter::reset(c); next_stop();}
  Conditional_nbr_iter& operator++()
    {Neighbor_iter::operator++(); next_stop(); return *this;}
  Conditional_nbr_iter& operator=(const Conditional_nbr_iter&)
    {assert(0); return *this;}
};

template <class Neighbor_iter, class GridSet>
void Conditional_nbr_iter<Neighbor_iter,GridSet>::next_stop()
{
  while (*this && !grid_set.contains(**this)) ++((Neighbor_iter&)(*this));
}


/***********************************************************************
  GRIDS
 ***********************************************************************/

template <class GridSet, class Itype, class Nbr_iter>
class Grid_tpl : public GridSet {
public:
  Grid_tpl(const Itype& i) : GridSet(i) {}
  typedef Conditional_nbr_iter<Nbr_iter,GridSet> Neighbor_iter;
};

//typedef GridSet_equat RawGridSet;
//typedef Nbr_iter_equat_diag RawNbr_iter;
typedef GridSet_geo<GridPoint> RawGridSet;
typedef Nbr_iter_geo_diag<GridPoint> RawNbr_iter;

typedef GridSet_elev<RawGridSet,Elev_map> GridSet;
typedef RawGridSet::Iter RawGrid_iter;
typedef RawGridSet::EdgeIter RawEdgeIter;
typedef GridSet::Iter Grid_iter;
typedef GridSet::EdgeIter EdgeIter;
typedef Grid_tpl<GridSet,Elev_map,RawNbr_iter> Grid;
typedef Grid::Neighbor_iter Neighbor_iter;

#endif // ndef _GRID_

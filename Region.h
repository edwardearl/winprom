/***********************************************************************
  FILE: region.h
  Copyright (C) 1998 by Edward Earl

  This is the C++ header file that supports region iterators.
 ***********************************************************************/

#ifndef _REGION_
#define _REGION_

#include <queue>


/***********************************************************************
  A region iterator is any iterator that explores a "region", which is any
  contiguous set of grid points that satisfy some condition, e.g. being
  equal to some target elevation. Region iterators have the same semantics
  as neighbor iterators (decribed in GridPoint.h) except that the center()
  operation is not available and the following operations are added:

	region_iter(const GridPoint& start, const region& r), the constructor,
	  which creates a region iterator described by region descriptor r
	  starting at start.
	void clear(), which releases the resources used by the iterator but
	  does _not_ initialize it for more iteration. This operation is
	  normally used if an iterator is not needed for a long time while
	  remaining in scope so that memory and disk space resources can be
	  freed sooner.

  In addition to a region condition, region iterators may also be equipped
  with restrictions on the path that can be taken from the starting point
  (e.g. nonascending walk).

  A region iterator must be supplied with a starting point and a "region
  descriptor", which is a class object that supports the following member
  functions:

	bool path(const neighbor_iter& ni) const, which indicates to the
	  iterator whether or not a step from ni.center() to *ni would
	  satisfy path restrictions.
	bool region(const GridPoint& c) const, which indicates to the iterator
	  whether or not c is in the region.
	bool visited(const GridPoint& c) const, which indicates to the iterator
	  whether or not c has already been visited by the iterator.
	bool step(const neighbor_iter& ni) const, which indicates to the
	  iterator whether or not a step from ni.center() to *ni can formally
	  be taken under current conditions. The result should normally be
	  the same as path(ni) && region(*ni) && !visited(*ni), which can
	  sometimes be computed more efficiently than with separate calls to
	  path(), region(), and visited() due to possible redundancy in the
	  latter.
	void visit(const GridPoint& c), which is called by the iterator
	  to indicate that it is visiting c.
	void clear(), which is called by the iterator to indicate that it
	  no longer requires the resources of the descriptor.

  Region iterators normally work by iterating over all neighbors of the
  starting point. For each of those points, all of their neighbors are
  iterated over, and so on recursively, thereby building a stack of neighbor
  iterators in which each iterates over the neighbors of the current value
  of the previous. However, as soon as the iterator visits any point, it
  must be marked as such so that whenever that point is encountered again,
  the iterator knows to skip it. Throughout this process, the iterator stops
  on whatever points are actually being iterated over (e.g. interior,
  boundary).
 ***********************************************************************/


template <class Region, class Grid>
/***********************************************************************
  This region iterator class visits each point in the interior of a region.

  This class only calls the step(), visit(), and clear() operations of its
  region descriptor. The path(), region(), and visited() operations are
  never called.
 ***********************************************************************/
class Region_iter {
  Region_iter(const Region_iter&);
  Region_iter& operator=(const Region_iter&);
  void operator++(int);
  const Grid& grid;
public:
  //typedef Grid::GridPoint GridPoint;
  //typedef Grid::Neighbor_iter Neighbor_iter;
  typedef GridPoint_equat GridPoint;
  typedef RawNbr_iter Neighbor_iter;
protected:
  Region regn; // region descriptor
  queue<GridPoint> front;
  /* Initialize the iterator to its first grid point. This differs from
     reset() in that it assumes that no resources have been allocated or are
     left over from a previous run. */
  void init(const GridPoint&);
public:
  Region_iter(const GridPoint& s, const Region& r, const Grid& g) :
    regn(r),grid(g) {init(s);}
  ~Region_iter() {clear();}
  void operator++();
  const GridPoint& operator*() const {return front.front();}
  operator bool() const {return !front.empty();}
  void reset(const GridPoint& s) {clear(); init(s);}
  void clear() {while (!front.empty()) front.pop(); regn.clear();}
};

template <class Region, class Grid>
void Region_iter<Region,Grid>::init(const GridPoint& s)
{
  assert(!regn.visited(s) && regn.region(s));
  front.push(s);
  regn.visit(s);
}

template <class Region, class Grid>
void Region_iter<Region,Grid>::operator++()
{
  for (::Neighbor_iter ni(grid,front.front()); ni; ++ni) {
    if (regn.step(ni)) {
      front.push(*ni);
      regn.visit(*ni);
    }
  }
  front.pop();
}

template <class Region, class Grid>
class Boundary_iter : public Region_iter<Region,Grid> {
/***********************************************************************
  This region iterator class is a boundary iterator, which iterates over
  the boundary of a region, which is all grid points neighboring a region but
  not in it but still accesble through an allowed path. Each boundary point
  may be visited more than once.

  This class does not call the step() operation of its region descriptor.
 ***********************************************************************/
  Boundary_iter(const Boundary_iter&);
  Boundary_iter& operator=(const Boundary_iter&);
  ::Neighbor_iter ni;
  void init(const GridPoint&);
  void grow_to_boundary(); // extend the front to the region boundary
public:
  Boundary_iter(const GridPoint& s, const Region& r, const Grid& g) :
    Region_iter<Region,Grid>(s,r,g),ni(g) {grow_to_boundary();}
  void operator++();
  const GridPoint& operator*() const {return *ni;}
  operator bool() const {return ni;}
  void reset(const GridPoint& s) {clear(); init(s);}
};

template <class Region, class Grid>
void Boundary_iter<Region,Grid>::init(const GridPoint& s)
{
  Region_iter<Region,Grid>::init(s);
  grow_to_boundary();
}

template <class Region, class Grid>
void Boundary_iter<Region,Grid>::operator++()
{
  ++ni;
  grow_to_boundary();
}

template <class Region, class Grid>
void Boundary_iter<Region,Grid>::grow_to_boundary()
{
  while (true) {
    if (!ni) {
      if (front.empty()) break;
      ni.reset(front.front());
      front.pop();
    }
    if (regn.path(ni)) {
      if (!regn.region(*ni)) break;
      if (!regn.visited(*ni)) {
	front.push(*ni);
	regn.visit(*ni);
      }
    }
    ++ni;
  }
}


#if 0 // include this conditional to test compilation of this .h file

class Universe {
public:
  bool path(const Neighbor_iter&) const {return true;}
  bool region(const GridPoint&) const {return true;}
  bool step(const Neighbor_iter&) const {return true;}
  bool visited(const GridPoint&) {return true;}
  void visit(const GridPoint&) {}
  void clear(const GridPoint&) {}
};

//typedef Region_mark<Simple> Simple_mark;

//static Packed_bool area_buf[100],visit_buf[100];
//static Packed_bool *ab=area_buf,*vb=visit_buf;
//static Visit_map area(ab,10,10),visit(vb,10,10);

Universe u;

Region_iter<Universe>
  ri(GridPoint::undefined,u);
Boundary_iter<Universe>
  bi(GridPoint::undefined,u);
#endif // test compilation

#endif // ndef _REGION_

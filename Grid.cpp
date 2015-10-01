#include "stdafx.h"
#include "Grid.h"


Distance GridSet_geo<GridPoint_equat>::nearest_edge(const GridPoint_equat& a) const
{
  double fac=GridPoint_equat::radius*sec2rad,
    cos_lat=cos(bound.limit[0].midpt()*GridPoint_equat::lat_step*sec2rad);
  Distance n=fac*(bound.limit[0].high-a.x)*GridPoint_equat::lat_step,
    s=fac*(a.x-bound.limit[0].low)*GridPoint_equat::lat_step,
    e=fac*(bound.limit[1].high-a.y)*GridPoint_equat::lon_step*cos_lat,
    w=fac*(a.y-bound.limit[1].low)*GridPoint_equat::lon_step*cos_lat,
    ns=n<s?n:s,ew=e<w?e:w;
  return ns<ew?ns:ew;
}

Distance GridSet_geo<GridPoint_polar>::nearest_edge(const GridPoint_polar& a) const
{
  Distance xp=bound.limit[0].high-a.x,xm=a.x-bound.limit[0].low,
    yp=bound.limit[1].high-a.y,ym=a.y-bound.limit[1].low,
    x=xp<xm?xp:xm,y=yp<ym?yp:ym;
  return (x<y?x:y)*GridPoint_polar::step;
}

/***************************************************************
  GRID & EDGE ITERATOR MEMBER FUNCTIONS
  See GridPoint.h for a complete explanation of these classes.
 ***************************************************************/

Rect_iter_all& Rect_iter_all::operator=(const Rect_iter_all& gi)
{
  assert(bound==gi.bound);
  current=gi.current;
  return *this;
}

Rect_iter_all& Rect_iter_all::operator++()
{
  if (++current.y>bound.limit[1].high) {
    current.y=bound.limit[1].low;
    ++current.x;
  }
  return *this;
}

Rect_iter_all& Rect_iter_all::operator--()
{
  if (--current.y<bound.limit[1].low) {
    current.y=bound.limit[1].high;
    --current.x;
  }
  return *this;
}

Rect_iter_edge& Rect_iter_edge::operator++()
{
  switch (side) {
  case 0:
    ++current.x;
    if (current.x>=bound.limit[0].high) ++side;
    break;
  case 1:
    ++current.y;
    if (current.y>=bound.limit[1].high) ++side;
    break;
  case 2:
    --current.x;
    if (current.x<=bound.limit[0].low) ++side;
    break;
  case 3:
    --current.y;
    if (current.y<=bound.limit[1].low-closed) ++side;
    break;
  }
  return *this;
}


/***********************************************
  NEIGHBOR ITERATOR STATIC DATA MEMBERS
 ***********************************************/

/* Nbr_iter_cart_diag::dx[] and Nbr_iter_cart_diag::dy[] are
   arrays of offsets in X and Y coordinates that represent movement
   to the eight nearest neighbors of a cartesian grid point. */
const int Nbr_iter_cart_diag::dx[]={1,1,0,-1,-1,-1,0,1};
const int Nbr_iter_cart_diag::dy[]={0,1,1,1,0,-1,-1,-1};

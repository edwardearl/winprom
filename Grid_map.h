// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
/***********************************************************************
  FILE: map.h
  Copyright (C) 1998 by Edward Earl

  This is the C++ header file that supports the services of map classes.
 ***********************************************************************/

#ifndef _GRID_MAP_
#define _GRID_MAP_

#include <new>
#include "Rectangle.h"

using std::bad_alloc;


/***********************************************************************
  A map assigns a value of some data type T to every grid point.
  All maps provide an operator[](const GridPoint& c) member function, which
  returns the value associated with c.  This function is available in const
  and mutable versions.  The const version should be used whenever possible.

  All maps are specific to a kind of grid point (up to polymorphism in the
  grid point types), which is typedef'ed locally.
 ***********************************************************************/


template <class T>
class Map_cart {
/***********************************************************************
  This class maintains a cartesian map that is stored in memory.
 ***********************************************************************/
  Map_cart& operator=(const Map_cart&);
  Map_cart(const Map_cart&);
  bool dealloc;
  T *array;
public:
  typedef GridPoint::Coord Coord;
protected:
  Coord xmin,ymin,xsize,ysize;
  void set_bnd(const Rectangl& r);
  void set_bnd(Coord xs, Coord ys) {xmin=ymin=0; xsize=xs; ysize=ys;}
  bool contains(Coord x, Coord y) const
    {return x>=0 && x<xsize && y>=0 && y<ysize;}
  void read_dim(FILE *) throw(file_error);
  void write_dim(FILE *) const throw(file_error);
  void read_contents(FILE *f) throw(file_error);
  void write_contents(FILE *f) const throw(file_error);
public:
  explicit Map_cart() : dealloc(false) {set_bound(0,0);}
  explicit Map_cart(Coord xs, Coord ys) : dealloc(false) {set_bound(xs,ys);}
  explicit Map_cart(Coord xs, Coord ys, T *a) : dealloc(false) {set_bound(xs,ys,a);}
  explicit Map_cart(const Rectangl& r) : dealloc(false) {set_bound(r);}
  explicit Map_cart(const Rectangl& r, T *a) : dealloc(false) {set_bound(r,a);}
  ~Map_cart() {if (dealloc) delete[] array;}
  void realloc() throw(bad_alloc);
  void realloc(T *);
  void set_bound(const Rectangl& r) throw(bad_alloc)
    {set_bnd(r); realloc();}
  void set_bound(const Rectangl& r, T *a) throw(bad_alloc)
    {set_bnd(r); realloc(a);}
  void set_bound(Coord xs, Coord ys) throw(bad_alloc)
    {set_bnd(xs,ys); realloc();}
  void set_bound(Coord xs, Coord ys, T *a) throw(bad_alloc)
    {set_bnd(xs,ys); realloc(a);}
  void fill(const T&); // fill the map with a constant value
  operator Rectangl() const; // return the map's bounding Rectangl
  Coord area() const {return xsize*ysize;}
  bool contains(const GridPoint& c) const
    {return contains(c.x-xmin,c.y-ymin);}
  bool transfer(const Map_cart&);
  bool transfer(const Map_cart&, const Rectangl&);
  T& operator[](const GridPoint&);
  const T& operator[](const GridPoint&) const;
  void print(const char *, FILE *f=stdout) const;
  void read(FILE *) throw(file_error,bad_alloc);
  void write(FILE *) const throw(file_error);
  void read(const char *) throw(file_error_name,bad_alloc);
  void write(const char *) const throw(file_error_name);
};

template <class T>
void Map_cart<T>::set_bnd(const Rectangl& r)
{
  xsize=r.xsize();
  ysize=r.ysize();
  xmin=r.limit[0].low;
  ymin=r.limit[1].low;
}

template <class T>
void Map_cart<T>::realloc() throw(bad_alloc)
{
  if (dealloc) delete[] array;
  array = area()==0 ? 0 : new T[area()];
  dealloc=true;
}

template <class T>
void Map_cart<T>::realloc(T *a)
{
  if (dealloc) delete[] array;
  array=a;
  dealloc=false;
}

template <class T>
void Map_cart<T>::fill(const T& value)
{
  for (Coord i=0; i<xsize*ysize; ++i)
    array[i]=value;
}

template <class T>
Map_cart<T>::operator Rectangl() const
{
  return (xsize==0 || ysize==0) ? Rectangl() :
    Rectangl(xmin,xmin+xsize-1,ymin,ymin+ysize-1);
}

template <class T>
bool Map_cart<T>::transfer(const Map_cart<T>& a)
{
  Rectangl r=(*this);
  r&=(Rectangl)a;
  if (r.is_empty()) return false;
  for (Rect_iter_all gi(r); gi; ++gi)
    (*this)[*gi]=a[*gi];
  return true;
}

template <class T>
bool Map_cart<T>::transfer(const Map_cart<T>& a, const Rectangl& r)
{
  if (!((Rectangl)(*this)).contains(r) || !((Rectangl)a).contains(r))
    return false;
  for (Rect_iter_all gi(r); gi; ++gi)
    (*this)[*gi]=a[*gi];
  return true;
}

template <class T>
T& Map_cart<T>::operator[](const GridPoint& c)
{
  const Coord x=c.x-xmin,y=c.y-ymin;
  return array[x*ysize+y];
}

template <class T>
const T& Map_cart<T>::operator[](const GridPoint& c) const
{
  assert(c);
  const Coord x=c.x-xmin,y=c.y-ymin;
  return array[x*ysize+y];
}

template <class T>
void Map_cart<T>::read_dim(FILE *f) throw(file_error)
{
  Rectangl edge;
  T defalt;
  Coord xdim,ydim;
  fread(edge,f);
  fread(defalt,f);
  fread(xdim,f);
  fread(ydim,f);
  if (xdim!=0 || ydim!=0) throw file_error("sectored file");
  set_bound(edge);
}

template <class T>
void Map_cart<T>::write_dim(FILE *f) const throw(file_error)
{
  Rectangl edge(*this);
  T defalt;
  const Coord dim0=0;
  fwrite(edge,f);
  if (fwrite(&defalt,sizeof defalt,1,f)!=1)
    throw file_error();
  fwrite(dim0,f);
  fwrite(dim0,f);
}

template <class T>
void Map_cart<T>::read(FILE *f) throw(bad_alloc,file_error)
{
  read_dim(f);
  GridPoint::read_stamp(f);
  read_contents(f);
}

template <class T>
void Map_cart<T>::write(FILE *f) const throw(file_error)
{
  write_dim(f);
  GridPoint::write_stamp(f);
  write_contents(f);
}

template <class T>
void Map_cart<T>::read(const char *fn) throw(bad_alloc,file_error_name)
{
  FILE *file=fopen(fn,"rb");
  try {
    if (!file) throw file_error();
    read(file);
  }
  catch (file_error e) {
    fclose(file);
    throw file_error_name(e,fn);
  }
  catch (...) {
    fclose(file);
    throw;
  }
  fclose(file);
}

template <class T>
void Map_cart<T>::write(const char *fn) const throw(file_error_name)
{
  FILE *file=fopen(fn,"wb");
  if (!file) throw file_error_name(fn);
  try {write(file);}
  catch (file_error e) {
    fclose(file);
    throw file_error_name(e,fn);
  }
  catch (...) {
    fclose(file);
    throw;
  }
  fclose(file);
}

template <class T>
void Map_cart<T>::read_contents(FILE *f) throw(file_error)
{
  if ((Coord)fread(array,sizeof(T),area(),f)!=area())
    throw file_error();
}

template <class T>
void Map_cart<T>::write_contents(FILE *f) const throw(file_error)
{
  if ((Coord)fwrite(array,sizeof(T),area(),f)!=area())
    throw file_error();
}

// Print the entire map, row by row, to file f.  Each element uses format format.
template <class T>
void Map_cart<T>::print(const char *format, FILE *f) const
{
  for (Rect_iter_all gi(Rectangl(*this)); gi; ++gi) {
    fprintf(f,format,(*this)[*gi]);
    if (gi.eol()==ymin+ysize-1) putc('\n',f);
  }
}


#if 0 // test compilation
Map_cart<int> mi(10,10);
//Map_cart<GridPoint_cart> mc(10,10);
#endif // test compilation

#endif // ndef _GRID_MAP_

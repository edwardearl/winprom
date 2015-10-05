// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#ifndef _BLOCK_
#define _BLOCK_

//#define test

#include "interval.h"


template <class T, unsigned dim>
struct Block {
  Interval<T> limit[dim];
  bool operator==(const Block) const;
  bool is_empty() const;
  void empty();
  void fill();
  T size(unsigned i) const {return limit[i].size();}
  T space() const;
  bool contains(const Block&) const;
  bool contains(const T *) const;
  bool contains(const T& e1, const T& e2) const
    {return limit[0].contains(e1) && limit[1].contains(e2);}
  bool on_edge(const T *) const;
  bool on_edge(const T& e1, const T& e2) const
    {return limit[0].on_edge(e1) || limit[1].on_edge(e2);}
  bool on_corner(const T *) const;
  bool on_corner(const T& e1, const T& e2) const
    {return limit[0].on_edge(e1) && limit[1].on_edge(e2);}
  bool operator&&(const Block&) const;
  Block& operator|=(const Block&);
  Block& operator|=(const T *);
  Block& expand(const T& e1, const T& e2)
    {limit[0]|=e1; limit[1]|=e2; return *this;}
  Block& operator&=(const Block&);
};

template <class T, unsigned dim>
bool Block<T,dim>::operator==(const Block<T,dim> b) const
{
  for (unsigned i=0; i<dim; ++i)
    if (!(limit[i]==b.limit[i])) return false;
  return true;
}

template <class T, unsigned dim>
bool Block<T,dim>::is_empty() const
{
  for (unsigned i=0; i<dim; ++i)
    if (limit[i].is_empty()) return true;
  return false;
}

template <class T, unsigned dim>
void Block<T,dim>::empty()
{
  for (unsigned i=0; i<dim; ++i)
    limit[i].empty();
}

template <class T, unsigned dim>
void Block<T,dim>::fill()
{
  for (unsigned i=0; i<dim; ++i)
    limit[i].fill();
}


template <class T, unsigned dim>
T Block<T,dim>::space() const
{
  T x=1,y;
  for (unsigned i=0; i<dim; ++i) {
    y=size(i);
    if (y<0) return -1;
    if (y==Interval<T>::infty) return Interval<T>::infty;
    x*=y;
  }
  return x;
}

template <class T, unsigned dim>
bool Block<T,dim>::contains(const Block<T,dim>& b) const
{
  for (unsigned i=0; i<dim; ++i)
    if (!limit[i].contains(b.limit[i])) return false;
  return true;
}

template <class T, unsigned dim>
bool Block<T,dim>::contains(const T *e) const
{
  for (unsigned i=0; i<dim; ++i)
    if (!limit[i].contains(e[i])) return false;
  return true;
}

template <class T, unsigned dim>
bool Block<T,dim>::on_edge(const T *e) const
{
  for (unsigned i=0; i<dim; ++i)
    if (limit[i].on_edge(e[i])) return true;
  return false;
}

template <class T, unsigned dim>
bool Block<T,dim>::on_corner(const T *e) const
{
  for (unsigned i=0; i<dim; ++i)
    if (!limit[i].on_edge(e[i])) return false;
  return true;
}

template <class T, unsigned dim>
Block<T,dim>& Block<T,dim>::operator|=(const Block<T,dim>& b)
{
  for (unsigned i=0; i<dim; ++i)
    limit[i]|=b.limit[i];
  return *this;
}

template <class T, unsigned dim>
Block<T,dim>& Block<T,dim>::operator|=(const T *e)
{
  for (unsigned i=0; i<dim; ++i)
    limit[i]|=e[i];
  return *this;
}

template <class T, unsigned dim>
Block<T,dim>& Block<T,dim>::operator&=(const Block<T,dim>& b)
{
  for (unsigned i=0; i<dim; ++i)
    limit[i]&=b.limit[i];
  return *this;
}

template <class T, unsigned dim>
bool Block<T,dim>::operator&&(const Block<T,dim>& b) const
{
  for (unsigned i=0; i<dim; ++i)
    if (!(limit[i]&&b.limit[i])) return false;
  return true;
}


#ifdef test
Block<int,2> b;
#endif


#endif // ndef _BLOCK_

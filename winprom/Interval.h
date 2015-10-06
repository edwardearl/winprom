// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#ifndef _INTERVAL_
#define _INTERVAL_

#include <assert.h>
#include <stdio.h>


template <class T>
struct Interval {
  static const char *format,*elem_format,*field_format,*hdr_string,
    *range_format,*precise_format,*max_format,*min_format,
    *minus_infty_string,*plus_infty_string,*limitless_string,*empty_string;
  static const T infty;
  typedef T Interval<T>::*endpt;
  T low,high;
  Interval() : low(infty),high(-infty) {}
  Interval(T e) : low(e),high(e) {}
  Interval(T l, T h) {set(l,h);}
  bool operator==(const T& a) const
    {return low==a && high==a;}
  bool operator==(const Interval& a) const
    {return low==a.low && high==a.high;}
  bool operator<(const Interval& a) const {return high<a.low;}
  bool operator>(const Interval& a) const {return a<(*this);}
  bool operator<=(const Interval& a) const {return low<a.low && high<a.high;}
  bool operator>=(const Interval& a) const {return low>a.low && high>a.high;}
  void set(const T& l, const T& h) {low=l; high=h; legalize();}
  void set(const T& e) {set(e,e);}
  void set() {empty();}
  void empty() {low=infty; high=-infty;}
  void fill() {low=-infty; high=infty;}
  bool is_empty() const {return high<low;}
  T size() const
    {return is_empty() ? -1 : (high<infty && low>-infty) ? high-low : infty;}
  T midpt() const {return low+(high-low)/2;}
  bool contains(const Interval& a) const {return low<=a.low && a.high<=high;}
  bool contains(const T& e) const {return low<=e && e<=high;}
  bool on_edge(const T& e) const {return e==low || e==high;}
  bool operator&&(const Interval& a) const {return !((*this)<a || a<(*this));}
  Interval& operator-=(const Interval& a);
  bool check_limit(float, float) const;
  void transform(float, float);
  static void subtract(T&, const T&);
  Interval operator-(const Interval& a) const
    {Interval<T> b(*this); b-=a; return b;}
  Interval& operator|=(const Interval&);
  Interval& operator|=(const T&);
  Interval& operator&=(const Interval&);
  static void order(T& a, T& b); // order a and b so that a<=b
  char *sprint(char *) const;
  void print(FILE *f=stdout) const;
  static void print_header(FILE *f=stdout)
    {fprintf(f,field_format,hdr_string);}
private:
  void legalize() {assert((low==infty && high==-infty) || low<=high);}//order(low,high);}
};

template <class T>
void Interval<T>::subtract(T& a, const T& b)
{
  if (a==infty || b==-infty) a=infty;
  else if (a==-infty || b==infty) a=-infty;
  else a-=b;
}

template <class T>
Interval<T>& Interval<T>::operator-=(const Interval<T>& a)
{
  subtract(high,a.low);
  subtract(low,a.high);
  return *this;
}

template <class T>
bool Interval<T>::check_limit(float u, float v) const
{
  return (low==-infty || u<low) && (high==infty || high<v);
}

static void transform(Elevation& e, float m, float b)
{
  if (abs(e)<Interval<Elevation>::infty) e=nint(e*m+b);
  else if (m<0) e*=-1;
}

template <class T>
void Interval<T>::transform(float m, float b)
{
  ::transform(low,m,b);
  ::transform(high,m,b);
  order(low,high);
}

template <class T>
Interval<T>& Interval<T>::operator|=(const Interval<T>& r)
{
  if (r.high>high) high=r.high;
  if (r.low<low) low=r.low;
  return *this;
}

template <class T>
Interval<T>& Interval<T>::operator|=(const T& a)
{
  if (a>high) high=a;
  if (a<low) low=a;
  return *this;
}

template <class T>
Interval<T>& Interval<T>::operator&=(const Interval<T>& a)
{
  if (a.high<high) high=a.high;
  if (a.low>low) low=a.low;
  if (high<low) {
    high=-infty;
    low=infty;
  }
  return *this;
}

template <class T>
void Interval<T>::order(T& a, T& b)
{
  if (a>b) {
    T t=a;
    a=b;
    b=t;
  }
}

template <class T>
void Interval<T>::print(FILE *f) const
{
  static char buffer[64];
  sprint(buffer);
  fprintf(f,"%s",buffer);
}

template <class T>
char *Interval<T>::sprint(char *buf) const
{
  if (low==-infty && high==infty)
    return buf+sprintf(buf,field_format,limitless_string);
  if (is_empty()) return buf+sprintf(buf,field_format,empty_string);
  if (low==-infty) {
    if (high>-infty) return buf+sprintf(buf,max_format,high);
    return buf+sprintf(buf,field_format,minus_infty_string);
  }
  if (high==infty) {
    if (low<infty) return buf+sprintf(buf,min_format,low);
    return buf+sprintf(buf,field_format,plus_infty_string);
  }
  if (low==high && precise_format)
    return buf+sprintf(buf,precise_format,low);
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'L': case 'l':
	buf+=sprintf(buf,elem_format,low);
	break;
      case 'M': case 'm':
	buf+=sprintf(buf,elem_format,midpt());
	break;
      case 'H': case 'h':
	buf+=sprintf(buf,elem_format,high);
	break;
      case 'R': case 'r':
	buf+=sprintf(buf,range_format?range_format:elem_format,(high-low)/2);
	break;
      case '%':
	*(buf++)='%';
	break;
      default:
	fprintf(stderr,"Interval format error! (%c)\n", *fc);
	assert(0);
      }
    }
    else *(buf++)=*fc;
  }
  *buf='\0';
  return buf;
}


#endif // ndef _INTERVAL_

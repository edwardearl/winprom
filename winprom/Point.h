// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#ifndef _POINT_
#define _POINT_

#include "GridPoint.h"
#include "rectangle.h"
#ifdef io_support
#include "Database.h"
#endif

typedef Interval<Elevation> Elev_intvl;
typedef Elev_intvl::endpt Elev_endpt;

#define HI_END &Elev_intvl::high
#define LO_END &Elev_intvl::low
#define OTHER_END(endpt) (endpt==LO_END ? HI_END : LO_END)
#define elev_infty Elev_intvl::infty

extern bool zero_prob;

bool Elev_intvl::operator<(const Elev_intvl& a) const
{
  return high<a.low || (!zero_prob && high==a.low && low<a.high);
}

struct Topo_point {
  GridPoint location;
  Elev_intvl elev;
  static const char *format,*field_format,*hdr_string,*undef_string;
  Topo_point() {}
  Topo_point(const Elev_intvl& er) : elev(er) {}
  Topo_point(const GridPoint& c, Elevation e) : location(c),elev(e) {}
  Topo_point(const GridPoint& c, Elevation l, Elevation h) :
    location(c),elev(l,h) {}
  Topo_point(const GridPoint& c, const Elev_intvl& er) :
    location(c),elev(er) {}
  operator bool() const {return location;}
  void undefine() {location.undefine(); elev.empty();}
  bool operator==(const Topo_point& tp) const {return location==tp.location;}
#ifdef io_support
  char *sprint(char *) const;
  void print(FILE *f=stdout) const;
  void trace() const;
  static void print_header(FILE *f=stdout);
#endif
};


struct Feature: public Topo_point {
  static const char *format,*field_format,*name_format,
    *hdr_string,*name_hdr,
    *edit_string,*noedit_string,*edit_hdr,
    *edge_string,*noedge_string,*edge_hdr;
#ifdef io_support
  static const Database *data;
#endif
  char *name;
  bool edited,edge_effect;
  Feature() : name(0),edited(true),edge_effect(false) {}
  Feature(const GridPoint& l, const Elev_intvl& e) :
    Topo_point(l,e),name(0),edited(!l),edge_effect(false) {}
  Feature(const Topo_point& tp, const char *n=0) :
    Topo_point(tp),name(_strdup(n)),edited(!tp.location),
    edge_effect(false) {}
  Feature(const Feature& ftr) : Topo_point(ftr),
    name(_strdup(ftr.name)),edited(ftr.edited),
    edge_effect(ftr.edge_effect) {}
  ~Feature() {delete[] name;}
  Feature& operator=(const Topo_point&);
  Feature& operator=(const Feature&);
  Feature& operator=(const char *);
  bool check_limit(float, float, bool) const;
  void transform(float, float, bool);
  void set_range(Elevation);
  void edit(const Feature&);
  void min_range(Elevation&) const;
  void clear()
    {(*this)=((const char *)0); undefine(); edited=true; edge_effect=false;}
  void write(FILE *) const throw(file_error);
  void read(FILE *) throw(file_error);
#ifdef io_support
  char *sprint(char *) const;
  void print(FILE *f=stdout) const;
  static void print_header(FILE *f=stdout);
#endif
};

enum FT_index {FT_PEAK,FT_SADDLE,FT_RUNOFF,FT_BASINSADL,FT_NUM_TYPES,
		FT_BASIN=FT_NUM_TYPES,FT_SBRO,FT_TREE,FT_MIXED,FT_MIXED_DRAIN,
		FT_NUM_IMG};


#ifdef io_support

struct FeatureFilter {
  Elev_intvl elev;
  bool strict,edge,nonedge,edit,unedit;
  CString name;
  bool case_sens,isolatin1_sens;
  enum Name_ft {IS=0,CONTAINS,ISNOT,CONTAINSNOT,BEGINS,ENDS} name_filter_type;
  Rectangl limits;
  Record_filter rec_filter;
  FeatureFilter(bool=false);
  bool test(const Feature&, const Database&) const;
};

struct FeatureTypeFilter : FeatureFilter {
  bool peaks,saddles,runoffs,bsnsdls;
  FeatureTypeFilter() : FeatureFilter(true),
    peaks(true),saddles(true),runoffs(true),bsnsdls(true) {}
  bool test(FT_index) const;
  bool test(const Feature& f, FT_index ft, const Database& db) const
    {return test(ft) && FeatureFilter::test(f,db);}

};

#endif // def io_support


#endif // ndef _POINT_

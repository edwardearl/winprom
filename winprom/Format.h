// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#ifndef _FORMAT_
#define _FORMAT_

#include "GridPoint.h"

enum Format_style {FMS_NOFLAG,FMS_FLAG,FMS_HTML,FMS_CSV};
#define AF_UTM ((Angle_fmt)(AF_GRAD+1))

enum Col_align {CA_LEFTx,CA_CENTERx,CA_RIGHTx};
extern const char *get_delimiter(Format_style, Col_align);

struct Location_fmt {
  Angle_fmt angle_f;
  bool news_f;
  CString custom;
  Location_fmt() : angle_f(AF_DMS),news_f(true) {}
  void set(bool=false) const;
};

struct Feature_fmt {
  enum Elev_fmt {MIDPOINT,RANGE,EXTREMES} elev_f;
  bool precise_f;
  Location_fmt loc_f;
  bool show_elev,show_loc;
  enum Name_fmt {NONE,ALT_ELEV,ALT_LOC,SEPARATE} name_f;
  CString custom;
  Feature_fmt() : elev_f(EXTREMES),precise_f(true),
    show_elev(true),name_f(SEPARATE),show_loc(true) {}
  void set(const Database&, Format_style=FMS_FLAG) const;
  void load_registry(const char *, const Feature_fmt&);
  void save_registry(const char *) const;
};

#endif // ndef _FORMAT_

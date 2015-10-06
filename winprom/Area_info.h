// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#ifndef _AREA_INFO_
#define _AREA_INFO_

#include "Rectangle.h"


struct Area_info {
  long ngp;
  float area;
  Rectangl rect;
  Area_info() : ngp(0),area(0.0) {}
  void empty() {ngp=0; area=0.0; rect.empty();}
};


#endif // ndef _AREA_INFO_

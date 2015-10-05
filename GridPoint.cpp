// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
/***********************************************************************
  FILE: GridPoint.cpp
  Copyright (C) 1998 by Edward Earl

  This is the C++ source file that implements the services of the GridPoint,
  neighbor iterator, grid iterator, and edge iterator classes.
 ***********************************************************************/

#include "stdafx.h"
#include <stdlib.h>
#include <ctype.h>
#include "GridPoint.h"
#include "interval.h"

const char *Interval<GridPoint_cart::Coord>::format="%L to %H";
const char *Interval<GridPoint_cart::Coord>::elem_format="%5d";
const char *Interval<GridPoint_cart::Coord>::range_format=0;
const char *Interval<GridPoint_cart::Coord>::precise_format=0;
const char *Interval<GridPoint_cart::Coord>::min_format="%5d or more ";
const char *Interval<GridPoint_cart::Coord>::max_format=" at most %5d";
const char *Interval<GridPoint_cart::Coord>::limitless_string=0;
const GridPoint_cart::Coord coord_infty=1073741824l;
const char *Interval<GridPoint_cart::Coord>::field_format=0;
const char *Interval<GridPoint_cart::Coord>::plus_infty_string=0;
const char *Interval<GridPoint_cart::Coord>::minus_infty_string=0;
const char *Interval<GridPoint_cart::Coord>::empty_string=0;


/***************************************
  CARTESIAN GRID POINT STATIC DATA MEMBERS
 ***************************************/

// constant representing an "undefined" cartesian grid point
const GridPoint_cart GridPoint_cart::undefined=GridPoint_cart();

// format for printing a cartesian grid point
const char *GridPoint_cart::format="(%x %y)";

// format for printing a field to replace a cartesian grid point
const char *GridPoint_cart::field_format="%s";

// format for printing X and Y coordinates of a cartesian grid point
const char *GridPoint_cart::coord_format="%ld";

// string for printing an undefined grid point
const char *GridPoint_cart::undef_string="undefined";

// string for printing a grid point header
const char *GridPoint_cart::hdr_string=0;

const Code_stamp GridPoint_cart::code=1;
GridPoint_cart::Coord GridPoint_cart::quad_int_x,GridPoint_cart::quad_int_y;
const Distance GridPoint_cart::huje=1e8;
const char *GridPoint_cart::unit="unit",
  *GridPoint_cart::coord1_label="X",*GridPoint_cart::coord2_label="Y",
  *GridPoint_cart::bound1_low="left",*GridPoint_cart::bound1_high="right",
  *GridPoint_cart::bound2_low="bottom",*GridPoint_cart::bound2_high="top";

/***************************************************************
  CARTESIAN GRID POINT MEMBER FUNCTIONS
  See GridPoint.h for a complete explanation of their usage.
 ***************************************************************/

double GridPoint_cart::direction(const GridPoint_cart& a) const
{
  return atan2((double)(a.y-y),(double)(a.x-x));
}

void GridPoint_cart::input()
{
  printf("Enter x,y: ");
  scanf("%ld %ld",&x,&y);
}

static char buffer[64];

void GridPoint_cart::print(FILE *f) const
{
  sprint(buffer);
  fprintf(f,"%s",buffer);
}

void GridPoint_cart::trace() const
{
  sprint(buffer);
  //TRACE("%s",buffer);
}

/* The format for printing cartesian grid points works much like that for printf,
   except that %x or %X means to print the X coordinate and %y or %Y means
   to print the Y coordinate.  "%%" means print a '%'. */
char *GridPoint_cart::sprint(char *buf) const
{
  if (!*this) return buf+sprintf(buf,field_format,undef_string);
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'X': case 'x':
	buf=sprint1(buf);
	break;
      case 'Y': case 'y':
	buf=sprint2(buf);
	break;
      case '%':
	*(buf++)='%';
	break;
      default:
	fprintf(stderr,"Cartesian grid point format error! (%c)\n",*fc);
	abort();
      }
    }
    else *(buf++)=*fc;
  }
  *buf='\0';
  return buf;
}

void GridPoint_cart::read_quad(FILE *f) throw (file_error)
{
  fread(quad_int_x,f);
  fread(quad_int_y,f);
}

void GridPoint_cart::read_stamp(FILE *f) throw (file_error)
{
  Code_stamp code_;
  fread(code_,f);
  if (code_!=code)
    throw file_error("Grid point stamp not cartesian");
  read_quad(f);
}

void GridPoint_cart::write_quad(FILE *f) throw (file_error)
{
  fwrite(quad_int_x,f);
  fwrite(quad_int_y,f);
}

void GridPoint_cart::write_stamp(FILE *f) throw (file_error)
{
  Code_stamp code_=code;
  fwrite(code_,f);
  write_quad(f);
}


// Convert text (expressed in any unit) to a # of seconds
// p and n are positive and negative NEWS characters
// A return of infty+1 indicates a format error
static GridPoint_cart::Coord
  sscanangle(const char *text, char lbl, double conv_fac, char p, char n)
{
  char *t;
  double fa;

  // get # of radians
  fa=strtod(text,&t);
  // did we get anything <=180 and >=-180? if not, format error
  const double limit=648000/conv_fac;
  if (t==text || fa>limit || fa<-limit) return coord_infty+1;
  text=t;
  while (isspace(*text)) ++text; // skip white space
  if (lbl!='\0' && *text==lbl) {
    // skip label character (e.g. degree sign)
    ++text;
    while (isspace(*text)) ++text; // skip white space
  }
  if (*text=='\0') return (GridPoint_cart::Coord)(fa*conv_fac);
  if (toupper(*text)==p || toupper(*text)==n) {
    // next character was NEWS.  fr must be positive or format error.
    if (fa<0) return coord_infty+1;
    if (toupper(*text)==n) fa*=-1; // adjust sign of fr according to NEWS character
    ++text;
    // make sure there is no non-white text remaining
    while (isspace(*text)) ++text;
    if (*text) return coord_infty+1;
    return (GridPoint_cart::Coord)(fa*conv_fac);
  }
  // there's something remaining.  format error.
  return coord_infty+1;
}

// skip white space
inline static void skip_white(const char *&t)
{
  while (isspace(*t)) ++t;
}

// skip white space with possible delimiter embedded
static void skip_white_delimiter(const char *d, const char *&t)
{
  skip_white(t);
  if (*t && strchr(d,*t)) {
    // skip delimiter
    ++t;
    skip_white(t);
  }
}

// check for NEWS character; if found, return its sign; if not, return 0
static int get_news(const char *&t, char p, char n)
{
  if (toupper(*t)==p) {
    ++t;
    skip_white(t);
    return 1;
  }
  if (toupper(*t)==n) {
    ++t;
    skip_white(t);
    return -1;
  }
  return 0;
}

// Convert text (in degree, DM, or DMS format) to a # of seconds
// p and n are positive and negative NEWS characters
// A return of infty+1 indicates a format error
GridPoint_cart::Coord sscansec(const char *text, char p, char n)
{
  char *t;
  double fd,fm,fs;
  int id,im,sign;

  // get the sign
  skip_white(text);
  if (*text=='-') {
    sign=-1;
    ++text;
  }
  else sign=1;

  // get # of degrees
  fd=strtod(text,&t);
  // did we get a nonnegative number <=180? if not, format error
  if (t==text || fd>180 || fd<0) return coord_infty+1;
  text=t;
  skip_white_delimiter("°:",text); // skip white space with possible degree sign
  if (*text=='\0') return (GridPoint_cart::Coord)(sign*fd*3600);
  if (sign>0) {
    // we had no minus sign at the beginning, so check for NEWS
    sign=get_news(text,p,n);
    if (sign!=0) {
      // got NEWS, make sure it's the end of the input text, and return.
      if (*text) return coord_infty+1;
      return (GridPoint_cart::Coord)(sign*fd*3600);
    }
    sign=1;
  }
  // there's more to come. make sure # of degrees is an integer.
  id=(int)fd;
  if (id!=fd) return coord_infty+1;

  // get # of minutes
  fm=strtod(text,&t);
  // did we get a nonnegative number <60?  if not, format error
  if (t==text || fm<0 || fm>=60) return coord_infty+1;
  text=t;
  skip_white_delimiter("\':",text); // skip white space with possible minute sign
  if (*text=='\0') return (GridPoint_cart::Coord)(sign*(id*3600+fm*60));
  if (sign>0) {
    // we had no minus sign at the beginning, so check for NEWS
    sign=get_news(text,p,n);
    if (sign!=0) {
      // got NEWS, make sure it's the end of the input text, and return.
      if (*text) return coord_infty+1;
      return (GridPoint_cart::Coord)(sign*(id*3600+fm*60));
    }
    sign=1;
  }
  // there's more to come. make sure # of minutes is an integer.
  im=(int)fm;
  if (im!=fm) return coord_infty+1;

  // get # of seconds
  fs=strtod(text,&t);
  // did we get a nonnegative number <60?  if not, format error
  if (t==text || fs<0 || fs>=60) return coord_infty+1;
  text=t;
  skip_white_delimiter("\":",text); // skip white space with possible second sign
  if (sign>0) {
    // we had no minus sign at the beginning, so check for NEWS
    sign=get_news(text,p,n);
    if (sign==0) sign=1;
  }
  if (*text=='\0') return (GridPoint_cart::Coord)(sign*(id*3600+im*60+fs));

  // there's something remaining.  format error.
  return coord_infty+1;
}

void print_angle(FILE *f, GridPoint_cart::Coord sec, char p, char n)
{
  sprint_angle(buffer,sec,p,n);
  fprintf(f,"%s",buffer);
}

char *sprintsec(char *buf, GridPoint_cart::Coord sec, char p, char n)
{
  if (sec<0) {sec*=-1; p=n;}
  return buf+sprintf(buf,"%3ld°%02ld'%02ld\"%c",sec/3600,sec/60%60,sec%60,p);
}

char *sprintmin(char *buf, GridPoint_cart::Coord sec, char p, char n)
{
  if (sec<0) {sec*=-1; p=n;}
  return buf+sprintf(buf,"%3ld°%05.2f'%c",sec/3600,(double)(sec%3600)/60,p);
}

char *sprint_anyunit(char *buf, double x, char p, char n)
{
  if (strstr(GridPoint::real_format,"%c") && x<0) {x*=-1; p=n;}
  return buf+sprintf(buf,GridPoint::real_format,x,p);
}

char *sprint_angle(char *buf, GridPoint_cart::Coord sec, char p, char n)
{
  switch (GridPoint::angle_format) {
  case AF_DEGREE:
    buf=sprint_anyunit(buf,sec/3600.0,p,n);
    break;
  case AF_DM:
    buf=sprintmin(buf,sec,p,n);
    break;
  case AF_DMS:
    buf=sprintsec(buf,sec,p,n);
    break;
  case AF_RADIAN:
    buf=sprint_anyunit(buf,sec*sec2rad,p,n);
    break;
  case AF_GRAD:
    buf=sprint_anyunit(buf,sec/3240.0,p,n);
    break;
  default:
    fprintf(stderr,"Unknown angle format\n");
    break;
  }
  return buf;
}


/***************************************
  EQUATORIAL GRID POINT STATIC DATA MEMBERS
  See GridPoint.h for an explanation
 ***************************************/

const Code_stamp GridPoint_equat::code=GridPoint_cart::code+1;
GridPoint_cart::Coord GridPoint_equat::lat_step=3,GridPoint_equat::lon_step=3;
Distance GridPoint_equat::radius=6371.0;
const char *GridPoint_equat::unit="km";
const char *GridPoint_equat::format="%A %O",
  *GridPoint_equat::real_format="%8.4f%c";
const char *GridPoint_equat::coord1_label="Latitude",
  *GridPoint_equat::coord2_label="Longitude",
  *GridPoint_equat::bound1_low="south",*GridPoint_equat::bound1_high="north",
  *GridPoint_equat::bound2_low="west",*GridPoint_equat::bound2_high="east";
Angle_fmt GridPoint_equat::angle_format=AF_DMS;


/***************************************
  EQUATORIAL GRID POINT MEMBER FUNCTIONS
  See GridPoint.h for explanations
 ***************************************/

Distance GridPoint_equat::rel_dist(const GridPoint_equat& a) const
{
  double lat1=x*lat_step*sec2rad,lat2=a.x*lat_step*sec2rad;
  double dlon=(y-a.y)*lon_step*sec2rad,c2=cos(lat2);
  double dz=sin(lat1)-sin(lat2),dy=c2*sin(dlon),dx=cos(lat1)-c2*cos(dlon);
  return (Distance)(dz*dz+dx*dx+dy*dy);
}

Distance GridPoint_equat::operator-(const GridPoint_equat& a) const
{
  return (Distance)(2*radius*asin(sqrt(rel_dist(a))/2));
}

double GridPoint_equat::direction(const GridPoint_equat& a) const
{
  double lat1=x*lat_step*sec2rad,lat2=a.x*lat_step*sec2rad,
    dlon=(a.y-y)*lon_step*sec2rad,c2=cos(lat2);
  return atan2(c2*sin(dlon),cos(lat1)*sin(lat2)-sin(lat1)*c2*cos(dlon));
}

void GridPoint_equat::write_stamp(FILE *f)
{
  Code_stamp code_=code;
  fwrite(code_,f);
  fwrite(lat_step,f);
  fwrite(lon_step,f);
  write_quad(f);
}

void GridPoint_equat::read_stamp(FILE *f)
{
  Code_stamp code_;
  Coord as,os;
  fread(code_,f);
  if (code_!=code)
    throw file_error("Grid point stamp not equatorial");
  fread(as,f);
  fread(os,f);
  if (as<=0 || as>120 || os<=0 && os>120)
    throw file_error("Invalid equatorial grid point step size");
  if (lat_step==0 && lon_step==0) {
    lat_step=as;
    lon_step=os;
  }
  else if (as!=lat_step || os!=lon_step)
    throw file_error("Latitude or longitude step does not match loaded element");
  read_quad(f);
}

void GridPoint_equat::clear_step()
{
  lat_step=lon_step=0;
}

static GridPoint_cart::Coord
  sscan_coord(const char *buf, char p, char n, GridPoint_cart::Coord step)
{
  GridPoint_cart::Coord c;
  switch (GridPoint_equat::angle_format) {
  case AF_DEGREE: case AF_DM: case AF_DMS:
    c=sscansec(buf,p,n);
    break;
  case AF_RADIAN:
    c=sscanangle(buf,'\0',1/sec2rad,p,n);
    break;
  case AF_GRAD:
    c=sscanangle(buf,'G',3240,p,n);
    break;
  default:
    assert(0);
  }
  if (c!=coord_infty+1) c/=step;
  return c;
}

GridPoint_cart::Coord GridPoint_equat::sscanx(const char *buf)
{
  return sscan_coord(buf,'N','S',lat_step);
}

GridPoint_cart::Coord GridPoint_equat::sscany(const char *buf)
{
  return sscan_coord(buf,'E','W',lon_step);
}

void GridPoint_equat::print(FILE *f) const
{
  sprint(buffer);
  fprintf(f,"%s",buffer);
}

void GridPoint_equat::trace() const
{
  sprint(buffer);
  TRACE("%s",buffer);
}

// the radius has the "central scale factor" 0.9996 built in
//static const double utm_radius=6375585.7,b=0.99664719; // WGS-84
static const double utm_radius=6375655.1,b=0.99660992; // NAD-27
static const double b2=1-b*b,b4=b2*b2;

static bool UTM2equat(double& latr, double& lonr, int zone, double ew, double ns)
{
  double x,y,z,gamma,lonrz;
  double cg,tg,tga,tga2,cga2,cga,sga,El,D,F,mu,dg,dmu,cmu;

  gamma=ns/=utm_radius;
  do {
    dg=gamma+(b-1)*(gamma+sin(2*gamma)/2)/2+
	(b-1)*(b-1)*(gamma-sin(4*gamma)/4)/16-ns;
    gamma-=dg;
  } while (fabs(dg)>1e-9);
  tg=tan(gamma);
  cg=1/sqrt(1+tg*tg);
  tga=tg/b;
  tga2=tga*tga;
  cga2=1/(1+tga2);
  cga=sqrt(cga2);
  sga=tga*cga;
  El=b2*tga/(b*b+tga2)*cg;
  D=cga*sqrt(1+tga2/(b*b));
  F=b/sqrt(b*b+b4*tga2*cga2*cga2);
  mu=ew=atan(sinh((ew-500000)/utm_radius))*D/F;
  do {
    dmu=mu+(D-1)*(mu+sin(2*mu)/2)/2-ew;
    mu-=dmu;
  } while (fabs(dmu)>1e-9);
  cmu=cos(mu);
  x=El*tga+F/D*cga*cmu;
  y=F*sin(mu);
  z=-b*b*El+F/D*sga*cmu;
  latr=atan(z/(b*sqrt(b*b-z*z)));
  lonrz=atan(y/x); // longitude within zone
  lonr=lonrz+0.10471976*zone-3.19395253; // longitude
  return fabs(lonrz)<0.052651; // return true if within 1' of zone
}

static void equat2UTM(double x, double y, int& zone, double& ew, double& ns)
{
  int lons;
  double latr,zoner,telat,celat,selat,x1,y1,z,b2,b4,C0,C2,C3,sg,dsg,gamma;
  double tlat,cg,tg,tga,tga2,cga2,cga,sga,El,D,F,X1,smu,mu;

  latr=x*sec2rad;
  lons=y+648000;
  zone=lons/21600+1;
  zoner=(lons-zone*21600+10800)*sec2rad;

  tlat=tan(latr);
  telat=b*tlat;
  celat=1/sqrt(1+telat*telat);
  selat=telat*celat;

  x1=celat*cos(zoner);
  y1=celat*sin(zoner);
  z=b*selat;
  b2=1-b*b;
  b4=b2*b2;
  C3=2*b*z*b2;
  C0=b*b*z*z;
  C2=x1*x1+C0-b4;
  sg=selat;
  if (sg!=0) {
    do {
      dsg=((((b4*sg+C3)*sg+C2)*sg-C3)*sg-C0)/(((4*b4*sg+3*C3)*sg+2*C2)*sg-C3);
      sg-=dsg;
    } while (fabs(dsg)>1e-9);
  }
  cg=sqrt(1-sg*sg);
  tg=sg/cg;
  tga=tg/b;
  tga2=tga*tga;
  cga2=1/(1+tga2);
  cga=sqrt(cga2);
  sga=tga*cga;

  El=b2*tga/(b*b+tga2)*cg;
  D=cga*sqrt(1+tga2/(b*b));
  F=b/sqrt(b*b+b4*tga2*cga2*cga2);
  X1=(x1-tlat*El)*cga+(z+b*b*El)*sga;
  smu=y1/(D*sqrt(y1*y1+X1*X1));

  mu=asin(smu);
  gamma=asin(sg);
  ns=(gamma+(b-1)*(gamma+sin(2*gamma)/2)/2+
      (b-1)*(b-1)*(gamma-sin(4*gamma)/4)/16)*utm_radius;
  ew=F/D*(mu+(D-1)*(mu+sin(2*mu)/2)/2);
  ew=log(1/cos(ew)+tan(ew))*utm_radius+500000;
}

void GridPoint_equat::get_UTM(int& zone, double& ew, double& ns) const
{
  equat2UTM(lat(),lon(),zone,ew,ns);
}

bool GridPoint_equat::set_UTM(int zone, double ew, double ns)
{
  double latr,lonr;
  bool status=UTM2equat(latr,lonr,zone,ew,ns);
  set_sec(latr/sec2rad,lonr/sec2rad);
  return status;
}

char *GridPoint_equat::sprint(char *buf) const
{
  if (!*this) return buf+sprintf(buf,field_format,undef_string);

  int zone;
  double ns,ew=0; // ew==0 indicates that we don't have UTM info
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'A': case 'a':
	buf=sprint_lat(buf);
	break;
      case 'O': case 'o':
	buf=sprint_lon(buf);
	break;
      case 'Z': case 'z':
	if (ew==0) get_UTM(zone,ew,ns);
	buf+=sprintf(buf,"%2d",zone);
	break;
      case 'N': case 'n':
	if (ew==0) get_UTM(zone,ew,ns);
	buf+=sprintf(buf,"%+08.0lf",ns);
	break;
      case 'E': case 'e':
	if (ew==0) get_UTM(zone,ew,ns);
	buf+=sprintf(buf,"%06.0lf",ew);
	break;
      case '%':
	*(buf++)='%';
	break;
      default:
	fprintf(stderr,"Equatorial grid point format error! (%c)\n",*fc);
	abort();
      }
    }
    else *(buf++)=*fc;
  }
  *buf='\0';
  return buf;
}


/***************************************
  POLAR GRID POINT STATIC DATA MEMBERS
  See GridPoint.h for an explanation
 ***************************************/

const Code_stamp GridPoint_polar::code=GridPoint_equat::code+1;
Distance GridPoint_polar::radius=6371.0f,GridPoint_polar::step=1.0f;
const char *GridPoint_polar::format="%A %O",
  *GridPoint_polar::real_format="%8.4f%c";
const char *GridPoint_polar::coord1_label="Y",
  *GridPoint_polar::coord2_label="X",
  *GridPoint_polar::bound1_low="bottom",*GridPoint_polar::bound1_high="top",
  *GridPoint_polar::bound2_low="left",*GridPoint_polar::bound2_high="right";
Angle_fmt GridPoint_polar::angle_format=AF_DMS;
double GridPoint_polar::eps=0,
  GridPoint_polar::E2=1,GridPoint_polar::E=1,
  GridPoint_polar::scale=2;


/***************************************
  POLAR GRID POINT MEMBER FUNCTIONS
  See GridPoint.h for explanations
 ***************************************/

GridPoint_cart::Coord GridPoint_polar::scan_coord(const char *buf)
{
  char *end;
  double c=strtod(buf,&end);
  while (isspace(*end)) ++end;
  return *end=='\0'?nint(c/step):coord_infty+1;
}

double GridPoint_polar::direction(const GridPoint_polar& a) const
{
  return atan2((double)((a.y-y)*x-(a.x-x)*y),(double)((a.x-x)*x+(a.y-y)*y));
}

double GridPoint_polar::ps_radius(double clat, double slat)
{
  return pow((1-eps*slat)/((1+eps*slat)*E2),eps/2)*E*
    scale*clat/(1-slat);
}

void GridPoint_polar::set_sec(Coord lat, Coord lon)
{
  double lonr=lon*sec2rad,latr=lat*sec2rad,
    r=ps_radius(cos(latr),sin(latr))*radius/step;
  x=nint(r*cos(lonr));
  y=nint(r*sin(lonr));
}

GridPoint_cart::Coord GridPoint_polar::lat() const
{
  double rr=sqrt((double)(x*x+y*y))*step/radius,rs=rr*2/scale,rs4i=1/(4+rs*rs),
    clat=4*rs*rs4i,slat=(rs*rs-4)*rs4i,dr,dlat,dslat,ui;
  unsigned n=0;
  do {
    ++n;
    assert(n<10);
    dr=rr-ps_radius(clat,slat);
    dlat=dr*(clat*(1-eps*slat)*(1+eps*slat))/(E2*rr);
    dslat=clat*dlat;
    clat-=slat*dlat;
    slat+=dslat;
    ui=1/sqrt(clat*clat+slat*slat);
    slat*=ui;
    clat*=ui;
  } while (fabs(dlat)>1e-9);
  return nint(-acos(clat)/sec2rad);
}

void GridPoint_polar::get_UTM(int& zone, double& ew, double& ns) const
{
  equat2UTM(lat(),lon(),zone,ew,ns);
}

bool GridPoint_polar::set_UTM(int zone, double ew, double ns)
{
  double latr,lonr;
  bool status=UTM2equat(latr,lonr,zone,ew,ns);
  set_sec(latr/sec2rad,lonr/sec2rad);
  return status;
}

char *GridPoint_polar::sprint(char *buf) const
{
  if (!*this) return buf+sprintf(buf,field_format,undef_string);

  int zone;
  double ns,ew=0;
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'A': case 'a':
	buf=sprint_lat(buf);
	break;
      case 'O': case 'o':
	buf=sprint_lon(buf);
	break;
      case 'Z': case 'z':
	if (ew==0) get_UTM(zone,ew,ns);
	buf+=sprintf(buf,"%2d",zone);
	break;
      case 'N': case 'n':
	if (ew==0) get_UTM(zone,ew,ns);
	buf+=sprintf(buf,"%+08.0lf",ew);
	break;
      case 'E': case 'e':
	if (ew==0) get_UTM(zone,ew,ns);
	buf+=sprintf(buf,"%06.0lf",ns);
	break;
      case '%':
	*(buf++)='%';
	break;
      default:
	fprintf(stderr,"Polar grid point format error! (%c)\n",*fc);
	abort();
      }
    }
    else *(buf++)=*fc;
  }
  *buf='\0';
  return buf;
}

void GridPoint_polar::write_stamp(FILE *f)
{
  Code_stamp code_=code;
  fwrite(code_,f);
  fwrite(step,f);
  fwrite(radius,f);
  fwrite(eps,f);
  fwrite(scale,f);
  write_quad(f);
}

void GridPoint_polar::read_stamp(FILE *f)
{
  Code_stamp code_;
  Distance s;
  fread(code_,f);
  if (code_!=code)
    throw file_error("Grid point stamp not polar");
  fread(s,f);
  if (s<=0 || s>10)
    throw file_error("Invalid polar grid point step size");
  if (step==0) step=s;
  else if (step!=s)
    throw file_error("Polar stereographic step does not match loaded element");
  fread(radius,f);
  if (radius<=0)
    throw file_error("Earth radius <= 0");
  fread(eps,f);
  if (eps<0 || eps>0.5)
    throw file_error("Invalid eccentricity for earth\'s figure");
  E2=1-eps*eps;
  E=sqrt(E2);
  fread(scale,f);
  if (scale<1 || scale>4)
    throw file_error("Invalid scale factor");
  read_quad(f);
}

void GridPoint_polar::clear_step()
{
  step=0;
}

void GridPoint_polar::print(FILE *f) const
{
  sprint(buffer);
  fprintf(f,"%s",buffer);
}

void GridPoint_polar::trace() const
{
  sprint(buffer);
}

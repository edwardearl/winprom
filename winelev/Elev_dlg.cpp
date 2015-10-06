// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Elev_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winelev.h"
#include "Elev_dlg.h"
#include "FileError_dlg.h"
#include "FileExist_dlg.h"
#include "ConvertCtl_dlg.h"
#include "../winprom/domain_anal.h"
#include <assert.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef GridPoint_cart::Coord Coord;

static char name_buf[1024],error_name[1024];

static int gcd(int a, int b)
{
  a=abs(a);
  b=abs(b);
  int r;
  while (b!=0) {
    r=a%b;
    a=b;
    b=r;
  }
  return a;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

// Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CElev_dlg::set_suffix(CString& name, const char *ext)
{
  const char *mem=name,*suffix=strrchr(mem,'.');
  if (suffix) name=name.Left(suffix-name); // strip suffix
  name+=ext;
}

FromType CElev_dlg::get_auto_type(const char *name) const
{
  const char *suffix=strrchr(name,'.');
  if (suffix) {
    if (!_stricmp(suffix,".hgt")) return FROM_SRTM;
    if (!_stricmp(suffix,".bil")) return FROM_NED;
    if (!_stricmp(suffix,".dem")) return FROM_UDEM;
    // unfortunately we can't tell between USGS .dem and GTOPO30 .dem (same suffix)
    if (!_stricmp(suffix,".glb")) return FROM_GLOBE;
    if (!_stricmp(suffix,".bin")) return FROM_ETOPO2;
    // nor ETOPO2 .bin and RAMP .bin
    if (!_stricmp(suffix,".delv")) return FROM_DELEV;
    if (!_stricmp(suffix,".elv")) return FROM_ELEV;
  }
  return FROM_UDEM;
}

FromType CElev_dlg::get_from_type(const char *name) const
{
  return (FromType)m_from_type==FROM_AUTO ?
    get_auto_type(name) : (FromType)m_from_type;
}

static const char *tail(const char *s)
{
  const char *t=s+strlen(s);
  while (t>(const char *)s && t[-1]!='\\') --t;
  return t;
}

// A return of true indicates that it is OK to overwrite the existing file, if any.
// A return of false indicates that the file already exists and it is NOT OK to replace it.
bool CElev_dlg::check_exist(const char *out_name) throw(file_error_name)
{
  if (_access(out_name,00)!=0) return true;
  switch (m_exist_action) {
  case 0: return true; // overwrite
  case 1: // prompt
    {
      CFileExist_dlg dlg;
      dlg.m_filename=out_name;
      switch (dlg.DoModal()) {
      case IDOK:
	if (!dlg.m_future) m_exist_action=0;
	return true;
      case IDCANCEL:
	strcpy(error_name,out_name);
	throw file_error_name("File exists and cannot be overwritten.",error_name);
      }
    }
    break;
  case 2: return false; // skip
  }
  assert(0); // shouldn't be here
  return false;
}

void CElev_dlg::process_hgt_bil(const char *in_name, FILE *in_file,
				unsigned short hgt) throw(file_error_name)
{
  Coord lat,lon;
  char ns,ew;
  const char *in_tail=tail(in_name);
  lat=lon=-1;
  sscanf(in_tail,"%c%d%c%d",&ns,&lat,&ew,&lon);
  if (lat<0 || lon<0) {
    strcpy(error_name,in_name);
    throw file_error_name("can\'t tell quad location from file name",error_name);
  }
  if (ns=='S' || ns=='s') lat*=-1;
  else if (ns!='N' && ns!='n') {
    strcpy(error_name,in_name);
    throw file_error_name("quad file name doesn\'t have N or S",error_name);
  }
  if (ew=='W' || ew=='w') lon*=-1;
  else if (ew!='E' && ns!='w') {
    strcpy(error_name,in_name);
    throw file_error_name("quad file name doesn\'t have E or W",error_name);
  }
  lat*=3600;
  lon*=3600;
  GridPoint_equat sw,ne;
  sw.set_sec(lat,lon);
  ne.set_sec(lat+3600,lon+3600);
  Rectangl bound(sw,ne);
  GridPoint_equat::quad_int_x=3600/GridPoint_equat::lat_step;
  GridPoint_equat::quad_int_y=3600/GridPoint_equat::lon_step;
  Elev_map elev(bound);
  fseek(in_file,0L,SEEK_SET);
  Elevation elev_buf[3601],elv;
  GridPoint_equat gp,bol=sw;
  bol.x+=GridPoint_equat::quad_int_x; // beginning of line
  unsigned i,j;
  for (i=0; i<=GridPoint_equat::quad_int_x; ++i) {
    if (hgt || i<GridPoint_equat::quad_int_x)
      fread(elev_buf,sizeof(Elevation),GridPoint_equat::quad_int_y+hgt,in_file);
    gp=bol;
    for (j=0; j<GridPoint_equat::quad_int_y+hgt; ++j) {
      if (hgt) elv=((elev_buf[j]&0xFF)<<8)|((elev_buf[j]>>8)&0xFF);
      else elv=elev_buf[j];
      if (elv==-32768) elv=elev_undef;
      else elv=nint(elv*conv_fac);
      elev[gp]=elv;
      ++gp.y;
    }
    if (!hgt) { // SRTM hgt has edges, NED bil does not
      elev[gp]=elv;
    }
    --bol.x;
  }
  fclose(in_file);
  control_dlg->m_progress.OffsetPos(1);

  process_elev(elev,in_name);
}

void CElev_dlg::srtm_port(const CString& in_name) throw(file_error_name)
{
  control_dlg->m_stage="SRTM height -> Elevation map";
  control_dlg->m_fromfile=in_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  FILE *height_file=fopen(in_name,"rb");
  if (!height_file) {
    strcpy(error_name,in_name);
    throw file_error_name(in_name);
  }

  // find size of file -- that'll yield spacing
  assert(sizeof(Elevation)==2);
  fseek(height_file,0L,SEEK_END);
  int length=ftell(height_file);
  unsigned i,j;
  for (i=1; i<=30; ++i) {
    if (3600%i!=0) continue;
    for (j=1; j<=i; ++j) {
      if (3600%j==0 && (3600/i+1)*(3600/j+1)*sizeof(Elevation)==length) {
	GridPoint_equat::lat_step=j;
	GridPoint_equat::lon_step=i;
	goto found_size;
      }
    }
  }
  fclose(height_file);
  strcpy(error_name,in_name);
  throw file_error_name("size doesn\'t match any possible spacing",error_name);
found_size:
  process_hgt_bil(in_name,height_file,1);
}

void CElev_dlg::ned_port(const CString& in_name) throw(file_error_name)
{
  control_dlg->m_stage="NED BIL -> Elevation map";
  control_dlg->m_fromfile=in_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  FILE *bil_file=fopen(in_name,"rb");
  if (!bil_file) {
    strcpy(error_name,in_name);
    throw file_error_name(in_name);
  }

  // find size of file -- that'll yield spacing
  assert(sizeof(Elevation)==2);
  fseek(bil_file,0L,SEEK_END);
  int length=ftell(bil_file),i,j;
  for (i=1; i<=30; ++i) {
    if (3600%i==0) {
      j=3600/i+1;
      if (length==j*j*sizeof(Elevation)) {
	GridPoint_equat::lat_step=GridPoint_equat::lon_step=i;
	goto found_size;
      }
    }
  }
  fclose(bil_file);
  strcpy(error_name,in_name);
  throw file_error_name("wrong input file size",error_name);
found_size:
  process_hgt_bil(in_name,bil_file,0);
}

static void ftoc(const char *in_name, const char *out_name)
  throw(file_error_name)
{
  FILE *fin=fopen(in_name,"r");
  if (fin==0) throw file_error_name(in_name);
  FILE *fout=fopen(out_name,"w");
  if (fout==0) {
    fclose(fin);
    throw file_error_name(out_name);
  }
  char c;
  while ((c=getc(fin))!=EOF)
    putc(c=='D'?'E':c,fout);
  fclose(fin);
  fclose(fout);
}

static int read_int(FILE *f, const char *fmt="%d") throw(file_error)
{
  int result;
  if (fscanf(f,fmt,&result)<1) throw file_error();
  return result;
}

static double read_double(FILE *f, const char *fmt="%le") throw(file_error)
{
  double result;
  if (fscanf(f,fmt,&result)<1) throw file_error();
  return result;
}

struct Deleter {
  const char *name;
  Deleter(const char *n) : name(n) {}
  ~Deleter() {remove(name);}
};

void CElev_dlg::dem_port(const CString& in_name) throw(file_error_name)
{
  control_dlg->m_stage="USGS DEM -> Elevation map";
  control_dlg->m_fromfile=in_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  const char *temp_name=tmpnam(NULL);
  ftoc(in_name,temp_name);
  Deleter del_temp(temp_name); // delete "temp" on scope exit, in case of exception
  const char *iunit=0;
  char qname[145];
  FILE *dem_file=fopen(temp_name,"r");
  if (!dem_file) throw file_error_name(temp_name);
  Elev_map out_map;
  try {
    if (fgets(qname,sizeof qname,dem_file)==0)
      throw file_error();
    (void)read_int(dem_file); // skip DEM level
    (void)read_int(dem_file); // skip pattern code
    int syscode=read_int(dem_file);
    if (syscode!=0)
      throw file_error("Input DEM is not in equatorial coordinates");
    (void)read_int(dem_file); // skip UTM zone
    for (unsigned i=0; i<15; ++i)
      (void)read_double(dem_file); // skip projection parameters
    if (read_int(dem_file)!=3)
      throw file_error("Horizontal coordinates of DEM must be in arc seconds");
    switch (read_int(dem_file)) {
    case 1:
      iunit="feet";
      break;
    case 2:
      iunit="meters";
      break;
    default:
      throw file_error("Vertical coordinates of DEM must be in feet or meters");
    }
    if (read_int(dem_file)!=4)
      throw file_error("Input DEM is an n-sided polygon.\n"
		       "I can only handle rectangles");
    Coord lat_corner[4],lon_corner[4];
    Elevation elev_step;
    double x;
    for (unsigned iside=0; iside<4; ++iside) {
      x=Coord(read_double(dem_file));
      if (x!=(lon_corner[iside]=(Coord)x))
	throw file_error("Quad corner longitude is not an integer number of seconds");
      x=Coord(read_double(dem_file));
      if (x!=(lat_corner[iside]=(Coord)x))
	throw file_error("Quad corner latitude is not an integer number of seconds");
    }
    if (lon_corner[0]!=lon_corner[1] || lon_corner[2]!=lon_corner[3] ||
	lat_corner[1]!=lat_corner[2] || lat_corner[3]!=lat_corner[0])
      throw file_error("DEM quadrilateral is not aligned with equatorial coordinate system");
    (void)read_double(dem_file);
    (void)read_double(dem_file);

    //if (read_double(dem_file)!=0)
      //throw file_error("DEM coordinate system is cocked; I can\'t handle that");
    //(void)read_int(dem_file,"%1d"); // skip type C record flag
    for (int i=0; i<30; ++i) fgetc(dem_file); // skip cock angle & type C record flag (inconsistent formatting)

    Coord &lat_step=GridPoint_equat::lat_step;
    Coord &lon_step=GridPoint_equat::lon_step;
    x=read_double(dem_file,"%12le");
    if (x!=(lon_step=(Coord)x))
      throw file_error("longitude step is not an integer number of seconds");
    x=read_double(dem_file,"%12le");
    if (x!=(lat_step=(Coord)x))
      throw file_error("latitude step is not an integer number of seconds");
    x=read_double(dem_file);
    if (x!=(elev_step=(Elevation)x))
      throw file_error("elevation step is not an integer");
    GridPoint_equat sw_corner,ne_corner;
    sw_corner.set_sec(lat_corner[0],lon_corner[0]);
    ne_corner.set_sec(lat_corner[2],lon_corner[2]);
    Rectangl bound(sw_corner,ne_corner);
    GridPoint_equat::quad_int_x=gcd(ne_corner.x,sw_corner.x);
    GridPoint_equat::quad_int_y=gcd(ne_corner.y,sw_corner.y);

    int nprow=read_int(dem_file),npcol=read_int(dem_file),nrow,ncol;
    int prowsize=bound.xsize()/nprow,pcolsize=bound.ysize()/npcol;

    // read past junk that sometimes appears
    for (int i=0; i<64; ++i) fgetc(dem_file);

    int iprow,ipcol,irow,icol,row_id,col_id;
    Coord pfl_lat,pfl_lon;
    double datum;
    out_map.set_bound(bound);
    out_map.fill(elev_undef);
    Elevation elev_temp;
    GridPoint_equat loc;
    for (iprow=0; iprow<nprow; ++iprow) {
      for (ipcol=0; ipcol<npcol; ++ipcol) {
	row_id=read_int(dem_file);
	col_id=read_int(dem_file);
	nrow=read_int(dem_file);
	ncol=read_int(dem_file);
	pfl_lon=nint(read_double(dem_file));
	pfl_lat=nint(read_double(dem_file));
	if (pfl_lon!=(bound.limit[1].low+(col_id-1)*pcolsize)*lon_step ||
	    pfl_lat!=(bound.limit[0].low+(row_id-1)*prowsize)*lat_step)
	  throw file_error("A profile is misplaced");
	datum=read_double(dem_file);
	(void)read_double(dem_file); // skip minimum profile elevation
	(void)read_double(dem_file); // skip maximum profile elevation
	for (irow=0; irow<nrow; ++irow) {
	  for (icol=0; icol<ncol; icol++) {
	    elev_temp=(Elevation)nint((read_int(dem_file)+datum)*conv_fac);
	    loc.set_sec(pfl_lat+irow*lat_step,pfl_lon+icol*lon_step);
	    out_map[loc]=elev_temp;
	  }
	}
      }
    }
    fclose(dem_file);
  }
  catch (file_error e) {
    fclose(dem_file);
    throw file_error_name(e.what(),temp_name);
  }
  control_dlg->m_progress.OffsetPos(1);

  process_elev(out_map,in_name);
}

bool CElev_dlg::breakup_bin_equat(FILE *bin_file, bool swap,
				  Coord lat, Coord lon,
				  Coord lat_quads, Coord lon_quads,
				  Elev_map elev[], GridPoint_equat bol[],
				  Elevation elev_buf[],
				  Elevation ocean_from, Elevation ocean_to)
{
  int iq,jq,i,j,iqd,jqd,jmax;
  Elevation elv;
  GridPoint_equat corner1,corner2,gp;
  const int row_size=GridPoint_equat::quad_int_y*lon_quads;

  if (fread(elev_buf,sizeof(Elevation),row_size,bin_file)!=row_size)
    return false;
  elev_buf[row_size]=elev_buf[row_size-1];
  for (iq=lat_quads-1; iq>=0; --iq) { // for each row of tiles
    // set bounds on elevation map for each tile in the row
    for (jq=0; jq<lon_quads; ++jq) {
      corner1.set_sec(lat+iq*GridPoint_equat::quad_int_x*GridPoint_equat::lat_step,
		      lon+jq*GridPoint_equat::quad_int_y*GridPoint_equat::lon_step),
      corner2.set_sec(lat+(iq+1)*GridPoint_equat::quad_int_x*GridPoint_equat::lat_step,
		      lon+(jq+1)*GridPoint_equat::quad_int_y*GridPoint_equat::lon_step);
      Rectangl bound(corner1,corner2);
      elev[jq].set_bound(bound);
      bol[jq].set_sec(lat+(iq+1)*GridPoint_equat::quad_int_x*GridPoint_equat::lat_step,
		      lon+jq*GridPoint_equat::quad_int_y*GridPoint_equat::lon_step);
    }
    for (i=0; i<=GridPoint_equat::quad_int_x; ++i) { // for each row of data in current row of tiles
      if (i!=0 && (iq!=0 || i!=GridPoint_equat::quad_int_x)) {
	// read the next row, unless we already have it from previous row of tiles
	// or this is the very last row
	if (fread(elev_buf,sizeof(Elevation),row_size,bin_file)!=row_size)
	  return false;
	elev_buf[row_size]=elev_buf[row_size-1];
      }
      for (jq=0; jq<lon_quads; ++jq) {
	jmax=(jq+1)*GridPoint_equat::quad_int_y;
	gp=bol[jq];
	for (j=jq*GridPoint_equat::quad_int_y; j<=jmax; ++j) {
	  if (swap) elv=((elev_buf[j]&0xFF)<<8)|((elev_buf[j]>>8)&0xFF);
	  else elv=elev_buf[j];
	  //if (elv==-9999 || elv==-500) elv=elev_undef;
	  if (elv==ocean_from) elv=ocean_to;
	  else elv=nint(elv*conv_fac);
	  elev[jq][gp]=elv;
	  ++gp.y;
	}
	--bol[jq].x;
      }
    }
    iqd=(lat+iq*GridPoint_equat::quad_int_x*GridPoint_equat::lat_step)/3600;
    for (jq=0; jq<lon_quads; ++jq) {
      jqd=(lon+jq*GridPoint_equat::quad_int_y*GridPoint_equat::lon_step)/3600;
      sprintf(name_buf,"%c%03d%c%02d",
	      jqd>=0?'E':'W',abs(jqd),iqd>=0?'N':'S',abs(iqd));
      process_elev(elev[jq],name_buf);
    }
  }
  return true;
}

bool CElev_dlg::breakup_bin_polar(FILE *file, bool swap,
				  const char *name_root, int fn_int,
				  Coord x_quads, Coord y_quads,
				  Elev_map elev[], GridPoint_polar bol[],
				  Elevation elev_buf[],
				  Elevation ocean_from, Elevation ocean_to)
{
  GridPoint_polar gp;
  Elevation elv;
  int iq,jq,i,j,jmax;
  const int row_size=GridPoint_polar::quad_int_y*y_quads;
  x_quads/=2;
  y_quads/=2;
  /* We divide the antarctic polar stereo DEM into a 6x6 grid of tiles,
     each measuring 900km square. */
  fread(elev_buf,sizeof(Elevation),row_size,file);
  elev_buf[row_size]=elev_buf[row_size-1];
  GridPoint_polar::radius=6378.1370f;
  GridPoint_polar::eps=0.08181918;
  // set scale factor for 71S (ADD value)
  GridPoint_polar::scale=1.94501811;
  for (iq=x_quads-1; iq>=-x_quads; --iq) { // for each row of tiles
    // set bounds on elevation map for each tile in the row
    for (jq=-y_quads; jq<y_quads; ++jq) {
      GridPoint_polar
	corner1(GridPoint_cart(iq*GridPoint_polar::quad_int_x,jq*GridPoint_polar::quad_int_y)),
	corner2(GridPoint_cart((iq+1)*GridPoint_polar::quad_int_x,(jq+1)*GridPoint_polar::quad_int_y));
      Rectangl bound(corner1,corner2);
      elev[jq+y_quads].set_bound(bound);
      bol[jq+y_quads].set_cart((iq+1)*GridPoint_polar::quad_int_x,jq*GridPoint_polar::quad_int_y); // beginning of line for each active tile
    }
    for (i=0; i<=GridPoint_polar::quad_int_x; ++i) { // for each row in current row of tiles
      if (i!=0 && (iq!=-x_quads || i!=GridPoint_polar::quad_int_x)) {
	if (fread(elev_buf,sizeof(Elevation),row_size,file)!=row_size)
	  return false;
	elev_buf[row_size]=elev_buf[row_size-1];
      }
      for (jq=0; jq<2*y_quads; ++jq) {
	jmax=(jq+1)*GridPoint_polar::quad_int_y;
	gp=bol[jq];
	for (j=jq*GridPoint_polar::quad_int_y; j<=jmax; ++j) {
	  if (swap) elv=((elev_buf[j]&0xFF)<<8)|((elev_buf[j]>>8)&0xFF);
	  else elv=elev_buf[j];
	  if (elv==-9999) elv=elev_undef;
	  else elv=nint(elv*conv_fac);
	  elev[jq][gp]=elv;
	  ++gp.y;
	}
	--bol[jq].x;
      }
    }
    for (jq=0; jq<2*y_quads; ++jq) {
      sprintf(name_buf,"%s_%c%02d_%c%02d",
	      name_root,iq>=0?'P':'N',abs(iq)*fn_int,
	      jq>=y_quads?'P':'N',abs(jq-y_quads)*fn_int);
      process_elev(elev[jq],name_buf);
    }
  }

  return true;
}

void CElev_dlg::gtopo30_port(const CString& in_name) throw (file_error_name)
{
  control_dlg->m_stage="GTOPO30 DEM -> Elevation map";
  control_dlg->m_fromfile=in_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  FILE *dem_file=fopen(in_name,"rb");
  if (!dem_file) {
    strcpy(error_name,in_name);
    throw file_error_name(error_name);
  }

  // find length of file -- that'll yield spacing
  assert(sizeof(Elevation)==2);
  fseek(dem_file,0L,SEEK_END);
  int length=ftell(dem_file);
  fseek(dem_file,0L,SEEK_SET);
  if (length==58320000L) { // antarctic polar stereo
    Elev_map elev[6];
    GridPoint_polar bol[6];
    Elevation elev_buf[5401];
    GridPoint_polar::quad_int_x=GridPoint_polar::quad_int_y=900;
    GridPoint_polar::step=1.0;
    if (!breakup_bin_polar(dem_file,true,"antarcps",9,6,6,
			   elev,bol,elev_buf,-9999,m_ocean_def?0:elev_undef)) {
      int eof=feof(dem_file);
      fclose(dem_file);
      strcpy(error_name,in_name);
      if (eof) throw file_error_name("Unexpected end of file",error_name);
      else throw file_error_name(error_name);
    }
  }
  else {
    unsigned lat_quads,lon_quads;
    if (length==57600000L) { // non-Antarctic
      lat_quads=10;
      lon_quads=8;
    }
    else if (length==51840000L) { // Antarctic
      lat_quads=6;
      lon_quads=12;
    }
    else {
      fclose(dem_file);
      strcpy(error_name,in_name);
      throw file_error_name("Input file not recognized as a GTOPO30 file size",error_name);
    }
    const int quad_int=600;
    assert(quad_int*quad_int*lat_quads*lon_quads*sizeof Elevation == length);
    Coord lat,lon;
    char ns,ew;
    const char *in_tail=tail(in_name);
    lat=lon=-1;
    sscanf(in_tail,"%c%d%c%d",&ew,&lon,&ns,&lat);
    if (lat<0 || lon<0) {
      fclose(dem_file);
      strcpy(error_name,in_name);
      throw file_error_name("can\'t tell location from file name",error_name);
    }
    if (ns=='S' || ns=='s') lat*=-1;
    else if (ns!='N' && ns!='n') {
      fclose(dem_file);
      strcpy(error_name,in_name);
      throw file_error_name("quad file name doesn\'t have N or S",error_name);
    }
    if (ew=='W' || ew=='w') lon*=-1;
    else if (ew!='E' && ew!='e') {
      fclose(dem_file);
      strcpy(error_name,in_name);
      throw file_error_name("quad file name doesn\'t have E or W",error_name);
    }
    lat-=lat_quads*5; // shift from NW to SW corner
    lat*=3600;
    lon*=3600;
    GridPoint_equat::quad_int_x=GridPoint_equat::quad_int_y=quad_int;
    Elev_map elev[12];
    GridPoint_equat bol[12];
    Elevation elev_buf[7201];

    GridPoint_equat::lat_step=GridPoint_equat::lon_step=30;

    if (!breakup_bin_equat(dem_file,true,lat,lon,lat_quads,lon_quads,
			   elev,bol,elev_buf,-9999,m_ocean_def?0:elev_undef)) {
      int eof=feof(dem_file);
      fclose(dem_file);
      strcpy(error_name,in_name);
      if (eof) throw file_error_name("Unexpected end of file",error_name);
      else throw file_error_name(error_name);
    }
  }

  fclose(dem_file);
  control_dlg->m_progress.OffsetPos(1);
}

void CElev_dlg::globe_port(const CString& in_name) throw (file_error_name)
{
  control_dlg->m_stage="GLOBE -> Elevation map";
  control_dlg->m_fromfile=in_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  FILE *globe_file=fopen(in_name,"rb");
  if (!globe_file) {
    strcpy(error_name,in_name);
    throw file_error_name(error_name);
  }

#ifndef NDEBUG
  assert(sizeof(Elevation)==2);
#endif

  GridPoint_equat::quad_int_x=GridPoint_equat::quad_int_y=600;
  Elev_map elev[18];
  GridPoint_equat bol[18];
  Elevation elev_buf[10801];
  GridPoint_equat::lat_step=GridPoint_equat::lon_step=30;
  const char *in_file=strrchr(in_name,'\\');
  if (in_file) ++in_file;
  else in_file=in_name;
  int big_quad_indx=in_file[0]-'a',lat_quads,s_lat,
    w_lon=(big_quad_indx%4)*324000-648000;
  switch (big_quad_indx/4) {
  case 0:
    s_lat=180000;
    lat_quads=8;
    break;
  case 1:
    s_lat=0;
    lat_quads=10;
    break;
  case 2:
    s_lat=-180000;
    lat_quads=10;
    break;
  case 3:
    s_lat=-324000;
    lat_quads=8;
    break;
  default:
    fclose(globe_file);
    strcpy(error_name,in_name);
    throw file_error_name("Bad name for GLOBE source file",error_name);
  }

  // check length of file
  fseek(globe_file,0L,SEEK_END);
  if (ftell(globe_file)!=lat_quads*12960000) {
    fclose(globe_file);
    strcpy(error_name,in_name);
    throw file_error_name("Bad length for GLOBE source file",error_name);
  }
  fseek(globe_file,0L,SEEK_SET);

  if (!breakup_bin_equat(globe_file,false,s_lat,w_lon,lat_quads,18,
			 elev,bol,elev_buf,-500,m_ocean_def?0:elev_undef)) {
    int eof=feof(globe_file);
    fclose(globe_file);
    strcpy(error_name,in_name);
    if (eof) throw file_error_name("Unexpected end of file",error_name);
    else throw file_error_name(error_name);
  }

  fclose(globe_file);
  control_dlg->m_progress.OffsetPos(1);
}

void CElev_dlg::etopo2_port(const CString& in_name) throw (file_error_name)
{
  control_dlg->m_stage="ETOPO2 -> Elevation map";
  control_dlg->m_fromfile=in_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  FILE *etopo_file=fopen(in_name,"rb");
  if (!etopo_file) {
    strcpy(error_name,in_name);
    throw file_error_name(error_name);
  }

#ifndef NDEBUG
  assert(sizeof(Elevation)==2);
#endif
  // check length of file
  fseek(etopo_file,0L,SEEK_END);
  if (ftell(etopo_file)!=116640000L) {
    fclose(etopo_file);
    strcpy(error_name,in_name);
    throw file_error_name("Input file is not ETOPO2 file size",error_name);
  }
  fseek(etopo_file,0L,SEEK_SET);

  GridPoint_equat::quad_int_x=GridPoint_equat::quad_int_y=900;
  Elev_map elev[12];
  GridPoint_equat bol[12];
  Elevation elev_buf[10801];

  GridPoint_equat::lat_step=GridPoint_equat::lon_step=120;

  if (!breakup_bin_equat(etopo_file,false,-324000,-648000,6,12,elev,
			 bol,elev_buf,elev_undef,elev_undef)) {
    int eof=feof(etopo_file);
    fclose(etopo_file);
    strcpy(error_name,in_name);
    if (eof) throw file_error_name("Unexpected end of file",error_name);
    else throw file_error_name(error_name);
  }

  fclose(etopo_file);
  control_dlg->m_progress.OffsetPos(1);
}

void CElev_dlg::ramp_port(const CString& in_name) throw (file_error_name)
{
  control_dlg->m_stage="RAMP -> Elevation map";
  control_dlg->m_fromfile=in_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  FILE *ramp_file=fopen(in_name,"rb");
  if (!ramp_file) {
    strcpy(error_name,in_name);
    throw file_error_name(error_name);
  }

#ifndef NDEBUG
  assert(sizeof(Elevation)==2);
#endif
  // check length of file
  fseek(ramp_file,0L,SEEK_END);
  int length=ftell(ramp_file),nquad;
  fseek(ramp_file,0L,SEEK_SET);
  if (length==56396352) {
    nquad=4;
  }
  else if (length==352477200) {
    nquad=10;
  }
  else if (length==1409908800) {
    nquad=20;
  }
  else {
    fclose(ramp_file);
    strcpy(error_name,in_name);
    throw file_error_name("Input file not recognized as a RAMP file size",error_name);
  }

  GridPoint_polar::quad_int_x=1229;
  GridPoint_polar::quad_int_y=1434;
  GridPoint_polar::step=4.0/nquad;
  Elev_map elev[20];
  GridPoint_polar bol[20];
  Elevation elev_buf[28681];

  if (!breakup_bin_polar(ramp_file,true,"ramp",1,nquad,nquad,
			 elev,bol,elev_buf,elev_undef,elev_undef)) {
    int eof=feof(ramp_file);
    fclose(ramp_file);
    strcpy(error_name,in_name);
    if (eof) throw file_error_name("Unexpected end of file",error_name);
    else throw file_error_name(error_name);
  }

  fclose(ramp_file);
  control_dlg->m_progress.OffsetPos(1);
}

void CElev_dlg::process_elev(const CString& elev_name)
{
  GridPoint::lat_step=GridPoint::lon_step=0;
  Elev_map elev;
  try {elev.read(elev_name);}
  catch (file_error e) {
    strcpy(error_name,elev_name);
    throw file_error_name(e.what(),error_name);
  }
  process_elev(elev,elev_name);
}

void CElev_dlg::process_delev(const CString& delev_name)
{
  control_dlg->m_stage="Differential elevation map -> Elevation map";
  control_dlg->m_fromfile=delev_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
  GridPoint::lat_step=GridPoint::lon_step=0;
  Map_cart<signed char> delev;
  unsigned count,*overflow=0;
  FILE *file=fopen(delev_name,"rb");
  try {
    if (!file) throw file_error();
    delev.read(file);
    fread(count,file);
    overflow=new unsigned[count];
    fread(overflow,sizeof(unsigned),count,file);
  }
  catch (file_error e) {
    fclose(file);
    delete[] overflow;
    strcpy(error_name,delev_name);
    throw file_error_name(e,error_name);
  }
  catch (...) {
    fclose(file);
    delete[] overflow;
    throw;
  }
  fclose(file);
  Elev_map elev((Rectangl)delev);
  Elevation curnt_elev=0;
  unsigned iovf=0,next_ovf=overflow[0]>>8;
  count=0;
  for (RawGrid_iter gi(delev); gi; ++gi,++count) {
    curnt_elev+=delev[*gi];
    if (count==next_ovf) {
      curnt_elev+=((Elevation)(signed char)overflow[iovf])<<8;
      ++iovf;
      next_ovf=overflow[iovf]>>8;
    }
    elev[*gi]=curnt_elev;
  }
  delete[] overflow;
  process_elev(elev,delev_name);
}

void CElev_dlg::process_elev(Elev_map& elev, CString file_name)
  throw (file_error_name)
{
  Index_map domap((Rectangl)elev);
  Divide_tree_map tree;
  if (m_blank) {
    Interval<Elevation> unlimit;
    unlimit.fill();
    undefine_flat(elev,(Rectangl)elev,unlimit);
  }
  if (m_to_elevmap) {
    // write elevation map to file
    set_suffix(file_name,".elv");
    if (!check_exist(file_name)) return;
    try {elev.write(file_name);}
    catch (file_error e) {
      strcpy(error_name,file_name);
      throw file_error_name(e.what(),error_name);
    }
  }
  if (m_to_diffelev) {
    // convert to and write differential elev map
    control_dlg->m_stage="Elevation map -> Differential elevation map";
    control_dlg->m_fromfile=file_name;
    AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
    set_suffix(file_name,".delv");
    if (!check_exist(file_name)) return;
    Map_cart<signed char> delev((Rectangl)elev);
    vector<int> overflows;
    Elevation prev_elev=0,elv,delv,delv2;
    unsigned count=0;
    for (RawGrid_iter gi((Rectangl)elev); gi; ++gi,++count) {
      elv=elev[*gi];
      delv=elv-prev_elev;
      delev[*gi]=delv;
      delv2=(Elevation)(signed char)delv;
      if (delv!=delv2) {
	// overflow in elevation difference. Add to the overflow array.
	overflows.push_back((count<<8) | (((delv-delv2)>>8) & 0xFF));
      }
      prev_elev=elv;
    }
    FILE *file=fopen(file_name,"wb");
    try {
      if (!file) throw file_error();
      delev.write(file);
      count=overflows.size(); // new use of count
      fwrite(count,file);
      if (fwrite(&(*overflows.begin()),sizeof(unsigned),overflows.size(),file)
	    !=overflows.size()) {
	throw file_error();
      }
    }
    catch (file_error e) {
      fclose(file);
      throw file_error_name(e,file_name);
    }
    catch (...) {
      fclose(file);
      throw;
    }
    fclose(file);
  }
  if (!m_to_divtree && !m_to_domap) return;
  // run analyzer
  control_dlg->m_stage="Elevation map -> Domain map / Divide tree";
  control_dlg->m_fromfile=file_name;
  AfxGetApp()->PostThreadMessage(ID_CVT_FILE,0,0);
#ifdef administrator
  tree.walkup_analyze(elev,domap);
#else
  assert(0);
#endif
  if (m_to_divtree) {
    set_suffix(file_name,".divt");
    if (!check_exist(file_name)) return;
    try {tree.write(file_name);}
    catch (file_error e) {
      strcpy(error_name,file_name);
      throw file_error_name(e.what(),error_name);
    }
  }
  if (m_to_domap) {
    fill_domap(elev,domap);
    set_suffix(file_name,".domap");
    if (!check_exist(file_name)) return;
    try {domap.write(file_name);}
    catch (file_error e) {
      strcpy(error_name,file_name);
      throw file_error_name(e.what(),error_name);
    }
  }
  control_dlg->m_progress.OffsetPos(1);
}

ConvertStatus CElev_dlg::convert_file(const CString& in_name,
	void (CElev_dlg::*cvt_func)(const CString&))
{
  static char buf[1024];
  while (true) {
    if (control_dlg->stop_request) return CF_ABORT;
    try {
      (this->*cvt_func)(in_name);
    }
    catch (file_error_name e) {
      if (control_dlg->stop_request) return CF_ABORT; // user pressed cancel
      if (m_error_prompt) {
	CFileError_dlg dlg;
	sprintf(buf,"Error: %s\nfile: %s",e.what(),e.file_name);
	dlg.m_message=buf;
	switch (dlg.DoModal()) {
	case IDC_RETRY: continue;
	case IDC_SKIP:
	  if (!dlg.m_future) m_error_prompt=0;
	  return CF_FAIL;
	case IDC_ABORT: return CF_ABORT;
	}
      }
    }
    break;
  }
  return CF_SUCCESS;
}

UINT CElev_dlg::convert_files()
{
  POSITION p=files_dlg->GetStartPosition();
  CString from_name;
  int from_type;
  ConvertStatus status;
  while (p!=NULL) {
    from_name=files_dlg->GetNextPathName(p);
    from_type=get_from_type(from_name);

    // perform the conversions
    switch (from_type) {
    case FROM_SRTM:
      status=convert_file(from_name,&CElev_dlg::srtm_port);
      break;
    case FROM_NED:
      status=convert_file(from_name,&CElev_dlg::ned_port);
      break;
    case FROM_UDEM:
      status=convert_file(from_name,&CElev_dlg::dem_port);
      break;
    case FROM_GTOPO30:
      status=convert_file(from_name,&CElev_dlg::gtopo30_port);
      break;
    case FROM_GLOBE:
      status=convert_file(from_name,&CElev_dlg::globe_port);
      break;
    case FROM_ETOPO2:
      status=convert_file(from_name,&CElev_dlg::etopo2_port);
      break;
    case FROM_RAMP:
      status=convert_file(from_name,&CElev_dlg::ramp_port);
      break;
    case FROM_ELEV:
      status=convert_file(from_name,&CElev_dlg::process_elev);
      break;
    case FROM_DELEV:
      status=convert_file(from_name,&CElev_dlg::process_delev);
      break;
    default:
      assert(0);
    }
    if (status==CF_ABORT) break;
    // delete source if desired and conversion successful,
    // unless native elv or delv was not converted to the other
    if (status==CF_SUCCESS && m_delsrc &&
	(from_type<FROM_ELEV || m_to_elevmap || m_to_diffelev))
      remove(from_name);
  }
  return IDOK;
}


/////////////////////////////////////////////////////////////////////////////
// CElev_dlg dialog

CElev_dlg::CElev_dlg()
  : CDialog(CElev_dlg::IDD,NULL)
{
  //{{AFX_DATA_INIT(CElev_dlg)
  m_exist_action = -1;
  m_error_prompt = FALSE;
  m_blank = FALSE;
  m_conv_fac = _T("");
  m_from_type = -1;
  m_delsrc = FALSE;
  m_to_domap = FALSE;
  m_to_elevmap = FALSE;
  m_to_divtree = FALSE;
  m_ocean_def = FALSE;
  m_to_diffelev = FALSE;
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_WINELEV);
}

void CElev_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CElev_dlg)
  DDX_Control(pDX, IDC_CONV_FAC, m_conv_fac_ctl);
  DDX_Radio(pDX, IDC_OVERWRITE, m_exist_action);
  DDX_Check(pDX, IDC_ERROR_PROMPT, m_error_prompt);
  DDX_Check(pDX, IDC_BLANK, m_blank);
  DDX_CBString(pDX, IDC_CONV_FAC, m_conv_fac);
  DDX_Radio(pDX, IDC_FROM_SRTM, m_from_type);
  DDX_Check(pDX, IDC_DELETE_SRC, m_delsrc);
  DDX_Check(pDX, IDC_DOMAP, m_to_domap);
  DDX_Check(pDX, IDC_ELEV_MAP, m_to_elevmap);
  DDX_Check(pDX, IDC_DIV_TREE, m_to_divtree);
  DDX_Check(pDX, IDC_OCEAN_DEF, m_ocean_def);
  DDX_Check(pDX, IDC_TO_DIFFELEV, m_to_diffelev);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CElev_dlg, CDialog)
  //{{AFX_MSG_MAP(CElev_dlg)
  ON_WM_SYSCOMMAND()
  ON_WM_DESTROY()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_CONVERT, OnConvert)
  ON_BN_CLICKED(IDC_ABOUT, OnAbout)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_HLP, OnHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CElev_dlg message handlers

BOOL CElev_dlg::OnInitDialog()
{
  m_conv_fac="3.28084";
  m_from_type=FROM_AUTO;
  m_to_elevmap=TRUE;
  m_to_domap=FALSE;
  m_to_divtree=FALSE;
  m_delsrc=FALSE;
  m_error_prompt=TRUE;
  m_blank=FALSE;
  m_exist_action=1;

  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL) {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty()) {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);	// Set big icon
  //SetIcon(m_hIcon, FALSE);	// Set small icon

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CElev_dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else {
    CDialog::OnSysCommand(nID, lParam);
  }
}

void CElev_dlg::OnDestroy()
{
  WinHelp(0L, HELP_QUIT);
  CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CElev_dlg::OnPaint()
{
  if (IsIconic()) {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CElev_dlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

void CElev_dlg::OnConvert()
{
  static char buf[4096];
  UpdateData(TRUE);
  if (!m_to_domap && !m_to_divtree) {
    // not analyzing
    if (!m_to_elevmap && !m_to_diffelev) {
      MessageBox("You must check at least one box in the \"Convert to\" group.",
		 "Winelev Conversion",MB_OK|MB_ICONEXCLAMATION);
      return;
    }
    if (m_from_type==FROM_ELEV && m_to_elevmap) {
      MessageBox("It doesn\'t take long to convert from\n"
		 "an elevation map to an elevation map.",
		 "Winelev Conversion",MB_OK|MB_ICONEXCLAMATION);
      return;
    }
    if (m_from_type==FROM_DELEV && m_to_diffelev) {
      MessageBox("It doesn\'t take long to convert from a differential\n"
		 "elevation map to a differential elevation map.",
		 "Winelev Conversion",MB_OK|MB_ICONEXCLAMATION);
      return;
    }
  }
#ifndef administrator
  else {
    MessageBox("The analyzer is not implemented in this shipment of Winelev.\n"
	       "Contact the Custodian for the divide tree(s) of interest.",
	       "Winelev",MB_OK|MB_ICONEXCLAMATION);
    return;
  }
#endif
  char *endp;
  conv_fac=strtod(m_conv_fac,&endp);
  if (conv_fac==0 || *endp!='\0') {
    MessageBox("Conversion factor must be a nonzero real number",
	       "Winelev Conversion",MB_OK|MB_ICONEXCLAMATION);
    m_conv_fac_ctl.SetEditSel(0,-1);
    m_conv_fac_ctl.SetFocus();
    return;
  }
  CFileDialog dlg(TRUE,NULL,NULL,
    OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,
    "SRTM height files (*.hgt)|*.hgt|"
    "NED BIL files (*.bil)|*.bil|"
    "USGS/GTOPO30 DEM files (*.dem)|*.dem|"
    "GLOBE files (*.glb)|*.glb|"
    "ETOPO2/RAMP files (*.bin)|*.bin|"
    "Elevation maps (*.elv)|*.elv|"
    "Differential elevation maps (*.delv)|*.delv|"
    "All raster map files (*.hgt,*.bil,*.dem,*.glb,*.bin,*.elv,*.delv)|*.hgt;*.bil;*.dem;*.glb;*.bin;*.elv;*.delv|"
    "All files (*.*)|*.*||");

  dlg.m_ofn.lpstrFile=buf;
  dlg.m_ofn.nMaxFile=sizeof buf;
  switch (m_from_type) {
  case 0: // SRTM height
    dlg.m_ofn.nFilterIndex=1;
    break;
  case 1: // NED BIL
    dlg.m_ofn.nFilterIndex=2;
    break;
  case 2: case 3: // USGS/GTOPO30 DEM
    dlg.m_ofn.nFilterIndex=3;
    break;
  case 4: // GLOBE
    dlg.m_ofn.nFilterIndex=4;
    break;
  case 5: case 6: // ETOPO2/RAMP
    dlg.m_ofn.nFilterIndex=5;
    break;
  case 7: case 8: case 9:
    dlg.m_ofn.nFilterIndex=m_from_type-1;
    break;
  }
  if (dlg.DoModal()!=IDOK) return;

  BeginWaitCursor();
  CConvertCtl_dlg ctl_dlg(*this);

  files_dlg=&dlg;
  ((CElevApp *)AfxGetApp())->control_dlg=control_dlg=&ctl_dlg;
  ctl_dlg.DoModal();

  EndWaitCursor();
}

void CElev_dlg::OnAbout()
{
  CDialog(IDD_ABOUTBOX).DoModal();
}

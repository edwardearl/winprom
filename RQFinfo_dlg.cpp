// RQFinfo_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "RQFinfo_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRQFinfo_dlg dialog

CRQFinfo_dlg::CRQFinfo_dlg(const CPromDoc *doc, const GridPoint& gp, CWnd* pParent /*=NULL*/)
  : document(doc),center(gp),featr(0),CDialog(CRQFinfo_dlg::IDD, pParent)
{
  data_init();
}

CRQFinfo_dlg::CRQFinfo_dlg(const CPromDoc *doc, const Feature *f, FT_index ft, CWnd *pParent)
  : document(doc),center(f->location),featr(f),ftype(ft),CDialog(CRQFinfo_dlg::IDD,pParent)
{
  data_init();
}

void CRQFinfo_dlg::data_init()
{
  //{{AFX_DATA_INIT(CRQFinfo_dlg)
  m_convlutn = _T("");
  m_eigen1 = _T("");
  m_eigen2 = _T("");
  m_elev_ctr = _T("");
  m_elev_stat = _T("");
  m_laplac = _T("");
  m_nbr00 = _T("");
  m_nbr01 = _T("");
  m_nbr02 = _T("");
  m_nbr10 = _T("");
  m_nbr11 = _T("");
  m_nbr12 = _T("");
  m_nbr20 = _T("");
  m_nbr21 = _T("");
  m_nbr22 = _T("");
  m_statx = _T("");
  m_staty = _T("");
  m_location = _T("");
  m_axis = _T("");
  m_resid_asym1 = _T("");
  m_resid_asym2 = _T("");
  m_resid_sym = _T("");
  m_hess1 = _T("");
  m_hess2 = _T("");
  //}}AFX_DATA_INIT
}

void CRQFinfo_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRQFinfo_dlg)
  DDX_Text(pDX, IDC_CONVLUTN, m_convlutn);
  DDX_Text(pDX, IDC_EIGEN1, m_eigen1);
  DDX_Text(pDX, IDC_EIGEN2, m_eigen2);
  DDX_Text(pDX, IDC_ELEV_CTR, m_elev_ctr);
  DDX_Text(pDX, IDC_ELEV_STAT, m_elev_stat);
  DDX_Text(pDX, IDC_LAPLAC, m_laplac);
  DDX_Text(pDX, IDC_NBR00, m_nbr00);
  DDX_Text(pDX, IDC_NBR01, m_nbr01);
  DDX_Text(pDX, IDC_NBR02, m_nbr02);
  DDX_Text(pDX, IDC_NBR10, m_nbr10);
  DDX_Text(pDX, IDC_NBR11, m_nbr11);
  DDX_Text(pDX, IDC_NBR12, m_nbr12);
  DDX_Text(pDX, IDC_NBR20, m_nbr20);
  DDX_Text(pDX, IDC_NBR21, m_nbr21);
  DDX_Text(pDX, IDC_NBR22, m_nbr22);
  DDX_Text(pDX, IDC_STATX, m_statx);
  DDX_Text(pDX, IDC_STATY, m_staty);
  DDX_Text(pDX, IDC_LOCATION, m_location);
  DDX_Text(pDX, IDC_AXIS, m_axis);
  DDX_Text(pDX, IDC_RESID_ASYM1, m_resid_asym1);
  DDX_Text(pDX, IDC_RESID_ASYM2, m_resid_asym2);
  DDX_Text(pDX, IDC_RESID_SYM, m_resid_sym);
  DDX_Text(pDX, IDC_HESS1, m_hess1);
  DDX_Text(pDX, IDC_HESS2, m_hess2);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRQFinfo_dlg, CDialog)
  //{{AFX_MSG_MAP(CRQFinfo_dlg)
  ON_BN_CLICKED(IDC_CENTER, OnCenter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRQFinfo_dlg message handlers

BOOL CRQFinfo_dlg::OnInitDialog() 
{
  double z[3][3];
  int i,j;
  bool good=true;
  Elevation elv;
  const Elev_map& elev=document->elev;
  CString *const nbr_elevs[3][3] = {
    {&m_nbr20,&m_nbr21,&m_nbr22},
    {&m_nbr10,&m_nbr11,&m_nbr12},
    {&m_nbr00,&m_nbr01,&m_nbr02},
  };

  char buf[64];
  center.sprint(buf);
  m_location=buf;

  // we shouldn't rely on the neighbor iterator working a particular way.
  // we should explicity set the location of each neighboring grid point
  // because we must know for sure which neighbor is being sampled.
  for (i=-1; i<=1; ++i) {
    for (j=-1; j<=1; ++j) {
      GridPoint nbr=center;
      nbr.x+=i;
      nbr.y+=j;
      elv=z[i+1][j+1]=elev.contains(nbr)?elev[nbr]:elev_undef;
      if (elv!=elev_undef) nbr_elevs[i+1][j+1]->Format("%d",elv);
      else good=false;
    }
  }

  if (good) {
    double u,v,sqrt6i=1/sqrt(6.0),
      Cxx=(z[0][0]+z[0][1]+z[0][2]+z[2][0]+z[2][1]+z[2][2]
	    -2*(z[1][0]+z[1][1]+z[1][2]))/6,
      Cyy=(z[0][0]+z[1][0]+z[2][0]+z[0][2]+z[1][2]+z[2][2]
	    -2*(z[0][1]+z[1][1]+z[2][1]))/6,
      Cxy=(z[0][0]-z[0][2]-z[2][0]+z[2][2])/8,
      Cx=(z[2][0]-z[0][0]+z[2][1]-z[0][1]+z[2][2]-z[0][2])/6,
      Cy=(z[0][2]-z[0][0]+z[1][2]-z[1][0]+z[2][2]-z[2][0])/6,
      C1=(5*z[1][1]-z[0][0]-z[0][2]-z[2][0]-z[2][2]
	    +2*(z[0][1]+z[2][1]+z[1][0]+z[1][2]))/9,
      laplac=Cxx+Cyy, // Laplacian
      shev2=Cxx*Cxx+2*Cxy*Cxy+Cyy*Cyy, // sum of squares of Hessian eigenvalues
      D=Cxx*Cyy-Cxy*Cxy, // product of Hessian eigenvalues
      dd=(Cxx-Cyy)/2,
      DD=sqrt(dd*dd+Cxy*Cxy), // discriminant of eigenvalue quadratic
      hess1=Cxy*4,
      hess2=(z[0][1]+z[2][1]-z[1][0]-z[1][2])/2,
      ra1=z[0][0]+z[1][2]+z[2][1]-z[2][2]-z[0][1]-z[1][0],
      ra2=z[0][2]-z[1][2]+z[2][1]-z[2][0]-z[0][1]+z[1][0],
      rsym=4*z[1][1]+z[0][0]+z[0][2]+z[2][0]+z[2][2]
	-2*(z[0][1]+z[1][0]+z[1][2]+z[2][1]);
    m_laplac.Format("%d",nint(laplac));
    m_convlutn.Format("%d",nint(sqrt(shev2)));
    m_hess1.Format("%d",nint(hess1));
    m_hess2.Format("%d",nint(hess2));
    m_resid_asym1.Format("%d",nint(ra1*sqrt6i));
    m_resid_asym2.Format("%d",nint(ra2*sqrt6i));
    m_resid_sym.Format("%d",nint(rsym/6));
    if (dd>0) {
      v=Cxy;
      u=dd+DD;
      if (v<0) {
	v*=-1;
	u*=-1;
      }
    }
    else {
      v=DD-dd;
      u=Cxy;
    }
    m_axis.Format("%d°",nint(57.2957795*atan2(v,u)));
    m_eigen1.Format("%d",nint(laplac/2+DD));
    m_eigen2.Format("%d",nint(laplac/2-DD));
    if (D!=0) {
      D=0.5/D;
      double x=(Cxy*Cy-Cx*Cyy)*D,y=(Cxy*Cx-Cy*Cxx)*D;
      // yes, x and y are switched ... x is lat and y is lon
      m_statx.Format("%.2f",y);
      m_staty.Format("%.2f",x);
      if (x>1) {
	y/=x;
	x=1;
      }
      if (x<-1) {
	y/=-x;
	x=-1;
      }
      if (y>1) {
	x/=y;
	y=1;
      }
      if (y<-1) {
	x/=-y;
	y=-1;
      }
      m_elev_stat.Format("%d",nint(C1+(Cx*x+Cy*y)/2));
    }
    m_elev_ctr.Format("%d",nint(C1));
  }

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CRQFinfo_dlg::OnCenter()
{
  if (featr) CPromApp::getApp()->viewFeature(document,*featr,ftype);
  else CPromApp::getApp()->viewLocation(center);
}

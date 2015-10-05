#pragma warning (disable : 4786)

#include "stdafx.h"
#include <algorithm>
#include <map>
#include "domain.h"

//#define trace_connect
#ifndef NDEBUG
#define timer
#endif

using namespace std;

const char *Elev_intvl::format="%L to %H";
const char *Elev_intvl::field_format="%s";
const char *Elev_intvl::elem_format="%5d";
const char *Elev_intvl::hdr_string=0;
const char *Elev_intvl::range_format="%3d";
const char *Elev_intvl::precise_format=0;
const char *Elev_intvl::min_format="%5d or more";
const char *Elev_intvl::max_format="at most %5d";
const char *Elev_intvl::limitless_string="Who knows?";
const char *Elev_intvl::plus_infty_string="high point";
const char *Elev_intvl::minus_infty_string="";
const char *Elev_intvl::empty_string="empty";

static const short unsigned VERSION_FS=257,
  VERSION_SADL_STAT=258,VERSION_SADL_STAT_RIVER=259;

#pragma warning (disable : 4786)


#if 0
void LIS::update(const Elev_intvl& a)
{
  if (a.high<elev) {
    elev=a.high;
    closed=a.high==a.low;
  }
  else if (a.high==elev) {
    if (a.low<a.high) closed=false;
  }
}

bool LIS::test(const Elev_intvl& a) const
{
  return zero_prob ? a.low<=elev :
    a.low<elev || closed && a.low==elev && a.low==a.high;
}
#endif


/* SADDLE MEMBERS */

bool Saddle::distance_io=false;
unsigned short Saddle::stat_io=0;
bool Saddle::use_status=false;
const char *const Saddle::status_name[] = {"River","Unk","Real","Prom"};


Saddle::Status Saddle::eff_stat(bool splice_mode) const
{
  return edge_effect?
	(splice_mode?STAT_RIVER:STAT_PROM):
	(use_status?(Status)status:STAT_UNKNOWN);
}

void Saddle::transform(float m, float b, bool trans_edit)
{
  Feature::transform(m,b,trans_edit);
  if (flat_sep<0 && (trans_edit || !edited)) flat_sep*=abs(m);
}

void Saddle::write(FILE *f) const
{
  Feature::write(f);
  fwrite(flat_sep,f);
  fwrite(status,f);
}

void Saddle::read(FILE *f)
{
  Feature::read(f);
  if (distance_io) fread(flat_sep,f);
  else flat_sep=0;
  if (stat_io) {
    fread(status,f);
    if (stat_io==1) ++status;
  }
  else status=STAT_UNKNOWN;
}


/* RUNOFF DATA MEMBERS */

const char
  *Runoff::format="RO #I: IQ=%q, %F",
  *Runoff::IQ_format="%2d ",
  *Runoff::id_ro_hdr=0,
  *Runoff::IQ_hdr="IQs";


/* RUNOFF MEMBER FUNCTIONS */

void Runoff::splice(Runoff& a, Basin_saddle& basin_sadl)
{
  if (peak && a.peak && (peak->peak.elev.high==4714 || a.peak->peak.elev.high==4714)) {
    int aa=1;
  }
  Domain *a_peak=0;
  interior_quads+=a.interior_quads;
  edit(a); // coalesce runoffs
  if (a.peak) {
    if (peak) {
      Saddle new_sadl=*this;
      if (flat_sep>=0 && a.flat_sep>=0)
	new_sadl.flat_sep=flat_sep+a.flat_sep;
      else if (flat_sep>a.flat_sep) new_sadl.flat_sep=flat_sep;
      else new_sadl.flat_sep=a.flat_sep;
      new_sadl.edge_effect=interior_quads<4;
      if (peak!=a.peak) {
	// parent peaks are not the same
	Domain::connect(new_sadl,peak,a.peak,basin_sadl,
			true,edge_effect||a.edge_effect);
	Domain::connect_nbr(basin_sadl,peak,a.peak);

	if (a.edge_effect) {
	  #ifdef trace_splice
	  TRACE("  Peak #%ld is edge effect- removing it\n",
		Divide_tree::writing->index(a.peak));
	  #endif
	  if (basin_sadl) {
	    if (basin_sadl.peak1==a.peak) basin_sadl.peak1=peak;
	    if (basin_sadl.peak2==a.peak) basin_sadl.peak2=peak;
	    #ifdef trace_splice
	    TRACE("    Basin saddle relinked to #%ld and #%ld\n",
		  Divide_tree::writing->index(basin_sadl.peak1),
		  Divide_tree::writing->index(basin_sadl.peak2));
	    #endif
	  }
	  a.peak->remove(peak,edge_effect);
	}
	else if (edge_effect) {
	  #ifdef trace_splice
	  TRACE("  Peak #%ld is edge effect- removing it\n",
		Divide_tree::writing->index(peak));
	  #endif
	  if (basin_sadl) {
	    if (basin_sadl.peak1==peak) basin_sadl.peak1=a.peak;
	    if (basin_sadl.peak2==peak) basin_sadl.peak2=a.peak;
	    #ifdef trace_splice
	    TRACE("    Basin saddle relinked to #%ld and #%ld\n",
		  Divide_tree::writing->index(basin_sadl.peak1),
		  Divide_tree::writing->index(basin_sadl.peak2));
	    #endif
	  }
	  peak->remove(a.peak,a.edge_effect);
	}
      }
      if (!a.edge_effect) edge_effect=false;
    }
    else {
      peak=a.peak;
      edge_effect=a.edge_effect;
      // we'll soon have to make a's owner "this"s owner;
      // save it now before a is removed
      a_peak=a.peak;
    }
  }
  a.remove();
  if (a_peak) a_peak->add_nbr(this);

  if (interior_quads>=4) {
    if (edge_effect && peak) {
      ROiter roi;
      for (roi=peak->runoffs.begin(); roi!=peak->runoffs.end(); ++roi)
	if ((*roi)!=this && (*roi)->edge_effect) break;
      if (roi==peak->runoffs.end()) peak->peak.edge_effect=false;
    }
    remove();
  }
}

void Runoff::clear_inv(bool update_nbr)
{
  if (!peak) return;
  if (update_nbr) peak->remove_nbr(this);
  //peak=peak->island_parent(elev);
  while (peak->primary_nbr && elev<peak->saddle.elev) peak=peak->primary_nbr;
  if (update_nbr) peak->add_nbr(this);
}

void Runoff::remove()
{
  clear();
  if (peak) {
    peak->remove_nbr(this);
    peak=0;
  }
  interior_quads=0;
}

void Runoff::reconnect(Domain *newpk)
{
  peak->remove_nbr(this);
  peak=newpk;
  newpk->add_nbr(this);
}

void Runoff::SWI_switch(Domain *sadl)
{
  Domain *ca=Domain::common_ancestor(peak,sadl);
  reconnect(ca==sadl ? sadl->primary_nbr : sadl);
}

void Runoff::write(FILE *f) const
{
  Saddle::write(f);
  fwrite(Divide_tree::writing->index(peak),f);
  fwrite(interior_quads,f);
}

void Runoff::read(FILE *f)
{
  Index index;
  Saddle::read(f);
  fread(index,f);
  peak=Divide_tree::reading->address(index);
  fread(interior_quads,f);
}

#ifdef io_support

void Runoff::print(FILE *f) const
{
  if (!format) {
    Feature::print(f);
    return;
  }
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	fprintf(f,Domain::id_format,Divide_tree::writing->index(*this));
	break;
      case 'Q': case 'q':
	fprintf(f,IQ_format,interior_quads);
	break;
      case 'F': case 'f':
	Feature::print(f);
	break;
      case 'P': case 'p':
	fprintf(f,Domain::id_format,Divide_tree::writing->index(peak));
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Basin saddle format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}

void Runoff::dump(FILE *f) const
{
  Feature::print(f);
  fprintf(f," IQ=%u, Peak #%ld\n",
	  interior_quads,Divide_tree::writing->index(peak));
}

void Runoff::print_header(FILE *f)
{
  if (!format) {
    Feature::print_header(f);
    return;
  }
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	fprintf(f,Domain::id_field_format,id_ro_hdr);
	break;
      case 'Q': case 'q':
	fprintf(f,"%s",IQ_hdr);
	break;
      case 'F': case 'f':
	Feature::print_header(f);
	break;
      case 'P': case 'p':
	fprintf(f,Domain::id_field_format,Domain::id_peak_hdr);
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Runoff format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }

}

#endif // def io_support


/* BASIN SADDLE DATA MEMBERS */

const char
  *Basin_saddle::format="BS %I: walk risk=%W, %F",
  *Basin_saddle::id_bs_hdr=0,
  *Basin_saddle::id_peak1_hdr=0,
  *Basin_saddle::id_peak2_hdr=0,
  *Basin_saddle::walk_risk_hdr=0,
  *Basin_saddle::WRO_hdr=0,
  *Basin_saddle::cycle_format="%u",
  *Basin_saddle::cycle_hdr=0,
  *Basin_saddle::WS_string="none";


/* BASIN SADDLE MEMBER FUNCTIONS */

static Domain::Sadl_cxn
check_low_cxn(const Domain *sadl1, const Domain *sadl2,
	      const Domain *ca, Elevation floor,
	      short int dc, Elev_endpt outer)
{
  for (; sadl1!=ca; sadl1=sadl1->primary_nbr)
    if (dc*sadl1->saddle.elev.*outer<floor) return Domain::LOW_CXN;
  for (; sadl2!=ca; sadl2=sadl2->primary_nbr)
    if (dc*sadl2->saddle.elev.*outer<floor) return Domain::LOW_CXN;
  return Domain::GOOD_CXN;
}

// Determine the peaks that would be connected in an SWI switch
// between this and sadl_dom.
Domain::Sadl_cxn Basin_saddle::SWI_switch_cxn(Domain *sadl_dom,
		Domain *&ps_cxn, Domain *&bs_cxn, unsigned short& cycle_side,
		short int dc, Elev_endpt inner, Elev_endpt outer)
{
  const Domain *ca,*bs_brk,*ps_brk;
  cycle_side=on_cycle(sadl_dom);
  if (cycle_side) {
    // saddle is on the basin saddle's cycle
    unsigned short hs=high_side(sadl_dom,sadl_dom->primary_nbr,dc,inner,outer);
    switch (hs) {
    case 0:
      return Domain::AMBIG_CXN;
    case 1:
      bs_cxn=peak2;
      bs_brk=peak1;
      break;
    case 2:
      bs_cxn=peak1;
      bs_brk=peak2;
      break;
    default: assert(0);
    }
    if (cycle_side==hs) {
      ps_cxn=sadl_dom->primary_nbr;
      ca=ps_brk=sadl_dom;
    }
    else {
      ps_cxn=sadl_dom;
      ps_brk=sadl_dom->primary_nbr;
      ca=common_ancestor;
    }
  }
  else {
    // saddle is off of basin saddle's cycle
    Domain *ca1,*ca2,*jct_dom=cycle_jct(sadl_dom,ca1,ca2);
    if (jct_dom==0) return Domain::NO_CXN;
    switch (high_side(jct_dom,dc,inner,outer)) {
    case 0:
      return Domain::AMBIG_CXN;
    case 1:
      bs_cxn=peak2;
      bs_brk=peak1;
      ca=ca1;
      break;
    case 2:
      bs_cxn=peak1;
      bs_brk=peak2;
      ca=ca2;
      break;
    default: assert(0);
    }
    if (ca!=sadl_dom) {
      ps_cxn=sadl_dom;
      ps_brk=sadl_dom->primary_nbr;
    }
    else {
      ps_cxn=sadl_dom->primary_nbr;
      ps_brk=sadl_dom;
    }
  }
  Elevation low1=dc*sadl_dom->saddle.elev.*inner,low2=dc*elev.*inner;
  return check_low_cxn(bs_brk,ps_brk,ca,low1<low2?low1:low2,dc,outer);
}

Domain *Basin_saddle::hit(const Domain *jct, short int dc,
			  Elev_endpt inner, Elev_endpt outer) const
{
  switch (high_side(jct,dc,inner,outer)) {
  case 1: return peak1;
  case 2: return peak2;
  case 0: return 0;
  }
  assert(0);
  return 0;
}

static Domain *hit2(Domain *a, Domain *b)
{
  if (a==b || b==0) return a;
  if (a==0) return b;
  return 0;
}

enum Hit_side {SIDE_NONE,SIDE_SHORT,SIDE_LONG};

static inline Hit_side hit_side(Elev_intvl hn, Elev_intvl hf, Elev_intvl C)
{
  if (hn.low>hf.high || hn.low>C.high) return SIDE_SHORT;
  if (hf.low>hn.high && C.low>hn.high) return SIDE_LONG;
  return SIDE_NONE;
}

static bool pair_gt(Elev_intvl& A1, Elev_intvl& B1, Elev_intvl& A2, Elev_intvl& B2,
		    short dc, Elev_endpt inner, Elev_endpt outer)
{
  return dc*(min(A1.*inner,B1.*inner)-min(A2.*outer,B2.*outer))>0;
}

// Determine the peaks that would be connected in an SWI switch
// between bsA and bsB.
Domain::Sadl_cxn
Basin_saddle::SWI_switch_cxn(Basin_saddle& bsA, Basin_saddle& bsB,
			     Domain *&cxnA, Domain *&cxnB,
			     short int dc, Elev_endpt inner, Elev_endpt outer)
{
  Domain *jctA1B=bsB.cycle_jct(bsA.peak1),*jctA2B=bsB.cycle_jct(bsA.peak2),
    *jctB1A=bsA.cycle_jct(bsB.peak1);
  Domain *brkA,*brkB;
  if (jctA1B==jctA2B) {
    // cycles are not fused
    brkA=bsA.hit(jctB1A,dc,inner,outer),brkB=bsB.hit(jctA1B,dc,inner,outer);
    if (brkA==0 || brkB==0) return Domain::AMBIG_CXN;
  }
  else {
    // cycles are fused
    bool swap=jctA2B==jctB1A;
    assert(swap!=(jctA1B==jctB1A));
    Elev_intvl A1,A2,B1,B2,C,Cx;
    bsA.low_cycle(jctA1B,jctA2B,dc,A1,A2,C);
    bsB.low_cycle(jctA1B,jctA2B,dc,B1,B2,Cx);
    assert(C.low==Cx.low && C.high==Cx.high);
    if (swap) {
      Cx=B1;
      B1=B2;
      B2=Cx;
    }
    Hit_side sideA1=hit_side(A1,A2,C),sideB1=hit_side(B1,B2,C),
      sideA2=hit_side(A2,A1,C),sideB2=hit_side(B2,B1,C);
    if (sideA1==SIDE_SHORT && sideB1==SIDE_SHORT &&
	pair_gt(A1,B1,A2,B2,dc,inner,outer)) {
      brkA=bsA.peak1;
      brkB=bsB.peak1;
    }
    else if (sideA2==SIDE_SHORT && sideB2==SIDE_SHORT &&
	     pair_gt(A2,B2,A1,B1,dc,inner,outer)) {
      brkA=bsA.peak2;
      brkB=bsB.peak2;
    }
    else if (sideA2==SIDE_LONG && sideB1==SIDE_LONG &&
	     pair_gt(A1,B2,A2,B1,dc,inner,outer)) {
      brkA=bsA.peak1;
      brkB=bsB.peak2;
    }
    else if (sideA1==SIDE_LONG && sideB2==SIDE_LONG &&
	     pair_gt(A2,B1,A1,B2,dc,inner,outer)) {
      brkA=bsA.peak2;
      brkB=bsB.peak1;
    }
    else return Domain::AMBIG_CXN;
    if (swap) brkB=bsB.other_peak(brkB);
  }
  Elevation lowA=dc*bsA.elev.*inner,lowB=dc*bsB.elev.*inner;
  cxnA=bsA.other_peak(brkA);
  cxnB=bsB.other_peak(brkB);
  return check_low_cxn(brkA,brkB,Domain::common_ancestor(brkA,brkB),
		       lowA<lowB?lowA:lowB,dc,outer);
}

static void get_leg_walk_info(Basin_saddle::Cycle_walk_info& cwinfo,
			      Domain *start, Domain *end, short int dc)
{
  while (start!=end) {
    const Elev_intvl& se=start->saddle.elev;
    if ((se.low-cwinfo.low1)*dc<0) {
      cwinfo.low2=cwinfo.low1;
      cwinfo.low1=se.low;
      cwinfo.low_src=start;
    }
    else if ((se.low-cwinfo.low2)*dc<0) cwinfo.low2=se.low;
    if ((se.high-cwinfo.high1)*dc<0) {
      cwinfo.high2=cwinfo.high1;
      cwinfo.high1=se.high;
      cwinfo.high_src=start;
    }
    else if ((se.high-cwinfo.high2)*dc<0) cwinfo.high2=se.high;
    start=start->primary_nbr;
  }
}

void Basin_saddle::get_walk_info(Cycle_walk_info& cwinfo, short int dc) const
{
  cwinfo.low1=elev.low;
  cwinfo.high1=elev.high;
  cwinfo.low_src=cwinfo.high_src=0;
  cwinfo.low2=cwinfo.high2=elev_infty*dc;
  get_leg_walk_info(cwinfo,peak1,common_ancestor,dc);
  get_leg_walk_info(cwinfo,peak2,common_ancestor,dc);
}

Elevation Basin_saddle::walk_risk(const Cycle_walk_info& cwinfo) const
{
  return elev.high-(cwinfo.low_src==0?cwinfo.low2:cwinfo.low1);
}

Elevation Basin_saddle::WR_offense(const Cycle_walk_info& cwinfo) const
{
  return (cwinfo.high_src==0?cwinfo.high2:cwinfo.high1)-elev.low;
}

Elevation Basin_saddle::walk_risk(const Cycle_walk_info& cwinfo,
				  const Domain *ps) const
{
  return ps->saddle.elev.high-(cwinfo.low_src==ps?cwinfo.low2:cwinfo.low1);
}

Elevation Basin_saddle::WR_offense(const Cycle_walk_info& cwinfo,
				   const Domain *ps) const
{
  return (cwinfo.high_src==ps?cwinfo.high2:cwinfo.high1)-ps->saddle.elev.low;
}

Domain *Basin_saddle::cycle_min(int sign, Elev_endpt outer)
{
  if (common_ancestor==0) return 0;
  Saddle *loop_min=this;
  peak1->low_saddle_leg(loop_min,common_ancestor,sign,outer,false);
  peak2->low_saddle_leg(loop_min,common_ancestor,sign,outer,false);
  return loop_min!=this?Domain::FromSadl(loop_min):0;
}

static void check_low(const Elev_intvl& elev,
		      Elev_intvl& low_elev, short int dc)
{
  if (dc*elev.high<dc*low_elev.high) low_elev.high=elev.high;
  if (dc*elev.low<dc*low_elev.low) low_elev.low=elev.low;
}

static void get_low(unsigned short side, const Domain *jct, short int dc,
		    Domain *pk, const Domain *end, Elev_intvl low_elev[])
{
  for (; pk!=end; pk=pk->primary_nbr) {
    if (pk==jct) side=3-side;
    check_low(pk->saddle.elev,low_elev[side-1],dc);
  }
}

unsigned short Basin_saddle::high_side(const Domain *jct,
		short int dc, Elev_endpt inner, Elev_endpt outer) const
{
  Elev_intvl low_elev[]={dc*elev_infty,dc*elev_infty};
  get_low(1,jct,dc,peak1,common_ancestor,low_elev);
  get_low(2,jct,dc,peak2,common_ancestor,low_elev);
  if (dc*low_elev[0].*outer<dc*low_elev[1].*inner) {
    // high saddle is on side 2
    return 2;
  }
  else if (dc*low_elev[1].*outer<dc*low_elev[0].*inner) {
    // high saddle is on side 1
    return 1;
  }
  return 0;
}

static void get_low_2(unsigned short init_side,
		      const Domain *jct1, const Domain *jct2, short int dc,
		      Domain *pk, const Domain *end, Elev_intvl low_elev[])
{
  unsigned short side=init_side,ne;
  for (; pk!=end; pk=pk->primary_nbr) {
    ne=(pk==jct1)+(pk==jct2);
    switch (ne) {
    case 1:
      if (side==init_side) side=0;
      else if (side==0) side=3-init_side;
      else assert(0);
      break;
    case 2:
      assert(side==init_side);
      side=3-init_side;
      break;
    }
    check_low(pk->saddle.elev,low_elev[side],dc);
  }
}

unsigned short Basin_saddle::high_side(const Domain *jct1, const Domain *jct2,
		short int dc, Elev_endpt inner, Elev_endpt outer) const
{
  Elev_intvl low_elev[]={dc*elev_infty,dc*elev_infty,dc*elev_infty};
  get_low_2(1,jct1,jct2,dc,peak1,common_ancestor,low_elev);
  get_low_2(2,jct1,jct2,dc,peak2,common_ancestor,low_elev);
  if (dc*(low_elev[2].*inner-low_elev[1].*outer)>0) {
    // high saddle is on side 2
    return 2;
  }
  if (dc*(low_elev[1].*inner-low_elev[2].*outer)>0) {
    // high saddle is on side 1
    return 1;
  }
  return 0;
}

void Basin_saddle::low_cycle(const Domain *jct1, const Domain *jct2, short int dc,
			     Elev_intvl& h1, Elev_intvl& h2, Elev_intvl& C)
{
  Elev_intvl low_elev[]={dc*elev_infty,dc*elev_infty,dc*elev_infty};
  get_low_2(1,jct1,jct2,dc,peak1,common_ancestor,low_elev);
  get_low_2(2,jct1,jct2,dc,peak2,common_ancestor,low_elev);
  h1=low_elev[1];
  h2=low_elev[2];
  C=low_elev[0];
}

// Set just the loop size.  Common ancestor must already be defined.
void Basin_saddle::set_cycle_length()
{
  cycle=common_ancestor ? Domain::length(peak1,common_ancestor)+
    Domain::length(peak2,common_ancestor)+1 : 0;
}

// Set both the common ancestor and the loop size.
void Basin_saddle::set_cycle_info()
{
  common_ancestor=Domain::common_ancestor(peak1,peak2);
  set_cycle_length();
}

// Determine if the saddle owned by d is on the loop closed by a basin saddle.
unsigned short Basin_saddle::on_cycle(const Domain *d) const
{
  Domain *id;
  for (id=peak1; id!=common_ancestor; id=id->primary_nbr)
    if (id==d) return 1;
  for (id=peak2; id!=common_ancestor; id=id->primary_nbr)
    if (id==d) return 2;
  return 0;
}

// find the point where the path from pk joins the cycle.
Domain *Basin_saddle::cycle_jct(Domain *pk, Domain *&ca1, Domain *&ca2)
{
  ca1=Domain::common_ancestor(pk,peak1);
  if (ca1==0) return 0;
  // find out if ca1 is on the path from peak1 to the cycle CA
  for (const Domain *ip=peak1; ip!=common_ancestor; ip=ip->primary_nbr)
    if (ip==ca1) {
      ca2=common_ancestor;
      return ca1; // yes, that's the junction point
    }
  // otherwise, if it's the cycle CA itself,
  // the junction point is the CA of peak2 and pk
  if (ca1==common_ancestor) {
    ca2=Domain::common_ancestor(pk,peak2);
    return ca2;
  }
  // otherwise, the CA of peak1 and pk must be an ancestor of the cycle CA,
  // and the junction point is the cycle CA
  ca2=ca1;
  return common_ancestor;
}

bool Basin_saddle::rotate(Domain *new_bs)
{
  Saddle temp_BS(new_bs->saddle);
  Domain *cut_parent=new_bs->primary_nbr,*child_peak,*parent_peak,*ci;

  // determine on which side of the common ancestor is the broken saddle 
  for (ci=peak1; ci!=common_ancestor; ci=ci->primary_nbr) {
    if (ci==new_bs) {
      child_peak=peak1;
      parent_peak=peak2;
      break;
    }
  }
  if (ci==common_ancestor) {
    for (ci=peak2; ci!=common_ancestor; ci=ci->primary_nbr) {
      if (ci==new_bs) {
	child_peak=peak2;
	parent_peak=peak1;
	break;
      }
    }
    if (ci==common_ancestor) return false;
  }

  // break saddle connection
  new_bs->primary_nbr=0;
  new_bs->saddle.clear();

  // connect peaks through old basin saddle
  child_peak->make_patriarch();
  child_peak->primary_nbr=parent_peak;
  child_peak->saddle=(*this);

  // Define new basin saddle
  (Saddle&)(*this)=temp_BS;
  peak1=new_bs;
  peak2=cut_parent;

  // Update neighbor and Basin saddle arrays for affected peaks
  if (cycle>2) {
    child_peak->remove_nbr(this);
    parent_peak->remove_nbr(this);
    child_peak->add_nbr(parent_peak);
    parent_peak->add_nbr(child_peak);
    new_bs->remove_nbr(cut_parent);
    cut_parent->remove_nbr(new_bs);
    new_bs->add_nbr(this);
    cut_parent->add_nbr(this);
  }

  return true;
}

void Basin_saddle::reconnect(Domain *from, Domain *to)
{
  from->remove_nbr(this);
  if (from==peak1) peak1=to;
  else if (from==peak2) peak2=to;
  else assert(0);
  to->add_nbr(this);

  set_cycle_info();
}

void Basin_saddle::remove()
{
  clear();
  peak1->remove_nbr(this);
  peak2->remove_nbr(this);
  common_ancestor=0;
  cycle=0;
}

void Basin_saddle::clear_inv(bool update_nbr)
{
  if (update_nbr) peak1->remove_nbr(this);
  while (peak1->primary_nbr && elev<peak1->saddle.elev) {
    if (peak1==common_ancestor) common_ancestor=peak1->primary_nbr;
    peak1=peak1->primary_nbr;
  }
  if (update_nbr) peak1->add_nbr(this);

  if (update_nbr) peak2->remove_nbr(this);
  while (peak2->primary_nbr && elev<peak2->saddle.elev) {
    if (peak2==common_ancestor) common_ancestor=peak2->primary_nbr;
    peak2=peak2->primary_nbr;
  }
  if (update_nbr) peak2->add_nbr(this);

  set_cycle_length();
}


// get the prominence of a basin saddle
Elev_intvl Basin_saddle::get_prom() const
{
  return Elev_intvl(extreme_prom(LO_END),extreme_prom(HI_END));
}

// find the depth of peak i from the patriarch of a tree
static void set_depth(Domain::Index i, Topo_info tree[], unsigned anc_depth[])
{
  unsigned j,d;
  for (j=i,d=0; j!=Domain::undef && anc_depth[j]==0; j=tree[j].parnt,++d);
  if (j!=Domain::undef) d+=anc_depth[j];
  for (j=i; j!=Domain::undef && anc_depth[j]==0; j=tree[j].parnt,--d)
    anc_depth[j]=d;
}

Domain::Index Basin_saddle::island_peak(const Domain *peak,
	Topo_info ipit[], const Domain dom_base[])
{
  Domain::Index i;
  for (i=peak-dom_base;
       ipit[i].parnt!=Domain::undef &&
	 !can_rotate(dom_base[ipit[i].sadl_dom].saddle);
       i=ipit[i].parnt);
  return i;
}

void Basin_saddle::inc_prom_sadl(const Domain& sadl_dom)
{
  const Domain::Sadl_prom& sp=sadl_dom.prom.saddle;
  if (sp.onmap>prom.cr_onmap) prom.cr_onmap=sp.onmap;
  if (sp.offmap>prom.cr_offmap) prom.cr_offmap=sp.offmap;
  if (can_rotate(sadl_dom.saddle)) {
    if (sp.onmap>prom.onmap) prom.onmap=sp.onmap;
    if (sp.offmap>prom.offmap) prom.offmap=sp.offmap;
  }
}

Domain::Index Basin_saddle::set_prom_long_leg(Domain::Index idom,
	unsigned d, const Domain dom_base[],
	const Topo_info ipit[], const unsigned anc_depth[])
{
  assert(anc_depth[idom]>=d);
  while (anc_depth[idom]>d) {
    inc_prom_sadl(dom_base[ipit[idom].sadl_dom]);
    idom=ipit[idom].parnt;
  }
  return idom;
}

void Basin_saddle::set_prom_leg(const Domain *start)
{
  for (; start!=common_ancestor; start=start->primary_nbr) {
    inc_prom_sadl(*start);
  }
}

void Basin_saddle::set_cycle_prom_leg(const Domain *start)
{
  for (; start!=common_ancestor; start=start->primary_nbr) {
    if (prom.onmap>start->ridge.cycle_prom)
      start->ridge.cycle_prom=prom.onmap;
    if (prom.offmap>start->ridge.cycle_offmap)
      start->ridge.cycle_offmap=prom.offmap;
    if (can_rotate(start->saddle)) {
      Domain::Sadl_prom& sp=start->prom.saddle;
      if (prom.cr_onmap>sp.cr_onmap) sp.cr_onmap=prom.cr_onmap;
      if (prom.cr_offmap>sp.cr_offmap) sp.cr_offmap=prom.cr_offmap;
    }
  }
}

void Basin_saddle::set_prom(Topo_info ipit[], const Domain dom_base[],
			    unsigned anc_depth[])
{
  assert((ipit==0) == (dom_base==0) && (ipit==0) == (anc_depth==0));
  prom.onmap=prom.offmap=prom.cr_onmap=prom.cr_offmap=-elev_infty;
  if (ipit) {
    Domain::Index
      i1=island_peak(peak1,ipit,dom_base),
      i2=island_peak(peak2,ipit,dom_base);
    set_depth(i1,ipit,anc_depth);
    set_depth(i2,ipit,anc_depth);
    unsigned d1=anc_depth[i1],d2=anc_depth[i2];
    if (d1>d2) {
      i1=set_prom_long_leg(i1,d2,dom_base,ipit,anc_depth);
    }
    else {
      i2=set_prom_long_leg(i2,d1,dom_base,ipit,anc_depth);
    }
    while (i1!=i2) {
      assert(i1!=Domain::undef && i2!=Domain::undef);
      inc_prom_sadl(dom_base[ipit[i1].sadl_dom]);
      inc_prom_sadl(dom_base[ipit[i2].sadl_dom]);
      i1=ipit[i1].parnt;
      i2=ipit[i2].parnt;
    }
  }
  else {
    set_prom_leg(peak1);
    set_prom_leg(peak2);
  }
  if (prom.onmap==-elev_infty) {
    // no proper rotation, therefore cell rotation prom is -infty
    prom.cr_onmap=-elev_infty;
    prom.cr_offmap=-elev_infty;
  }
  else {
    // proper rotation, so set ridge & cell rotation prom
    set_cycle_prom_leg(peak1);
    set_cycle_prom_leg(peak2);
  }
}

void Basin_saddle::extreme_prom_leg(Elevation& bs_prom, Domain *start,
				    Elev_endpt endpt) const
{
  Elevation sp;
  for (; start!=common_ancestor; start=start->primary_nbr) {
    if (!can_rotate(start->saddle)) continue;
    sp=start->extreme_sadl_prom(eff_stat(false)>start->saddle.eff_stat(false)?
				  elev_infty:elev.high,
				endpt);
    if (sp>bs_prom) bs_prom=sp;
  }
}

Elevation Basin_saddle::extreme_prom(Elev_endpt endpt) const
{
  Elevation bs_prom=-elev_infty;
  extreme_prom_leg(bs_prom,peak1,endpt);
  extreme_prom_leg(bs_prom,peak2,endpt);
  return bs_prom;
}

void Basin_saddle::offmap_prom_leg(Elevation& bs_omp, Domain *start) const
{
  Elevation osp;
  for (; start!=common_ancestor; start=start->primary_nbr) {
    if (!can_rotate(start->saddle)) continue;
    osp=start->offmap_sadl_prom(eff_stat(false)>start->saddle.eff_stat(false)?
				elev_infty:elev.high);
    if (osp>bs_omp) bs_omp=osp;
  }
}

Elevation Basin_saddle::offmap_prom() const
{
  Elevation bs_omp=-elev_infty;
  offmap_prom_leg(bs_omp,peak1);
  offmap_prom_leg(bs_omp,peak2);
  return bs_omp;
}

bool Basin_saddle::can_rotate(const Saddle& new_bs) const
{
  Status bs_stat=eff_stat(false),ps_stat=new_bs.eff_stat(false);
  if (ps_stat>bs_stat) return false;
  if (ps_stat<bs_stat) return true;
  return !(new_bs.elev>elev);
}

bool Basin_saddle::can_rotate() const
{
  Domain *current;
  // look for possibly lower saddle on leg 1.
  for (current=peak1; current!=common_ancestor; current=current->primary_nbr)
    if (can_rotate(current->saddle)) return true;
  // no lower saddle on leg 1. try leg 2.
  for (current=peak2; current!=common_ancestor; current=current->primary_nbr)
    if (can_rotate(current->saddle)) return true;
  // no lower saddle.
  return false;
}


void Basin_saddle::write(FILE *f) const throw(file_error)
{
  Saddle::write(f);
  fwrite(Divide_tree::writing->index(peak1),f);
  fwrite(Divide_tree::writing->index(peak2),f);
}

void Basin_saddle::read(FILE *f) throw(file_error)
{
  Index index;
  Saddle::read(f);
  fread(index,f);
  peak1=Divide_tree::reading->address(index);
  fread(index,f);
  peak2=Divide_tree::reading->address(index);
}

#ifdef io_support

void Basin_saddle::print(const Cycle_walk_info& cwinfo,
			 short int dc, FILE *f) const
{
  if (!format) {
    Feature::print(f);
    return;
  }
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	fprintf(f,Domain::id_format,Divide_tree::writing->index(*this));
	break;
      case 'W': case 'w':
	if (cycle>1)
	  fprintf(f,Elev_intvl::elem_format,
		  (dc>0)?walk_risk(cwinfo):WR_offense(cwinfo));
	else fprintf(f,"%s",WS_string);
	break;
      case 'O': case 'o':
	if (cycle>1)
	  fprintf(f,Elev_intvl::elem_format,
		  (dc>0)?WR_offense(cwinfo):walk_risk(cwinfo));
	else fprintf(f,"%s",WS_string);
	break;
      case 'C': case 'c':
	fprintf(f,cycle_format,cycle);
	break;
      case 'F': case 'f':
	Feature::print(f);
	break;
      case '1':
	fprintf(f,Domain::id_format,Divide_tree::writing->index(peak1));
	break;
      case '2':
	fprintf(f,Domain::id_format,Divide_tree::writing->index(peak2));
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Basin saddle format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}

void Basin_saddle::dump(FILE *f) const
{
  Feature::print(f);
  fprintf(f,"  P1=#%ld, P2=#%ld, CA=#%ld, cycle=%u\n",
	  Divide_tree::writing->index(peak1),Divide_tree::writing->index(peak2),
	  Divide_tree::writing->index(common_ancestor),cycle);
}


void Basin_saddle::print_header(FILE *f)
{
  if (!format) {
    Feature::print_header(f);
    return;
  }
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	fprintf(f,Domain::id_field_format,id_bs_hdr);
	break;
      case 'W': case 'w':
	fprintf(f,"%s",walk_risk_hdr);
	break;
      case 'O': case 'o':
	fprintf(f,"%s",WRO_hdr);
	break;
      case 'C': case 'c':
	fprintf(f,"%s",cycle_hdr);
	break;
      case 'F': case 'f':
	Feature::print_header(f);
	break;
      case '1':
	fprintf(f,Domain::id_field_format,id_peak1_hdr?id_peak1_hdr:Domain::id_peak_hdr);
	break;
      case '2':
	fprintf(f,Domain::id_field_format,id_peak2_hdr?id_peak2_hdr:Domain::id_peak_hdr);
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Basin saddle format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }

}

#endif // def io_support

void Basin_saddle::Cycle_iter::restart()
{
  if (current==bs->common_ancestor && last_start==bs->peak1)
    last_start=current=bs->peak2;
}


/* Domain DATA MEMBERS */

const char
  *Domain::format="peak %I: %K\n%P",
  *Domain::prom_format="  prom=%P, saddle %S\n",
  *Domain::first_prom_format=0,
  *Domain::HP_format="  high point, no saddle\n",
  *Domain::id_format="#%lu",
  *Domain::id_field_format="%s",
  *Domain::id_peak_hdr=0,
  *Domain::id_saddle_hdr=0,
  *Domain::prom_hdr=0,
  *Domain::usable_peak_hdr=0,
  *Domain::usable_saddle_hdr=0,
  *Domain::onmap_parent_string="",
  *Domain::offmap_parent_string="off-map",
  *Domain::runoff_string="off at",
  *Domain::offmap_hdr=0;
const Domain::Index Domain::undef=(~(Domain::Index)0)>>1;
const Elevation Elev_intvl::infty=32512;
bool Domain::first_prom;


/* Domain MEMBER FUNCTIONS */

void Domain::operator=(const Domain& a)
{
  peak=a.peak;
  saddle=a.saddle;
  primary_nbr=a.primary_nbr;
  neighbors.clear();
  runoffs.clear();
  bsnsdls.clear();
}

void Domain::copy_nbrs(const Domain& source)
{
  neighbors=source.neighbors;
  runoffs=source.runoffs;
  bsnsdls=source.bsnsdls;
}

static FILE *file;

static void print_dom_index(Domain *dom)
{
  fprintf(file," #%ld",Divide_tree::writing->index(dom));
}

static void print_ro_index(Runoff *ro)
{
  fprintf(file," #%ld",Divide_tree::writing->index(*ro));
}

static void print_bs_index(Basin_saddle *bs)
{
  fprintf(file," #%ld",Divide_tree::writing->index(*bs));
}

static Domain dummy;

Domain *Domain::FromPeak(Feature *peak)
{
  static const ptrdiff_t peak_offset=(char *)&dummy.peak-(char *)&dummy;
  return (Domain *)((char *)peak-peak_offset);
}

const Domain *Domain::FromPeak(const Feature *peak)
{
  static const ptrdiff_t peak_offset=(char *)&dummy.peak-(char *)&dummy;
  return (const Domain *)((char *)peak-peak_offset);
}

Domain *Domain::FromSadl(Feature *saddle)
{
  static const ptrdiff_t peak_offset=(char *)&dummy.saddle-(char *)&dummy;
  return (Domain *)((char *)saddle-peak_offset);
}

const Domain *Domain::FromSadl(const Feature *saddle)
{
  static const ptrdiff_t peak_offset=(char *)&dummy.saddle-(char *)&dummy;
  return (const Domain *)((char *)saddle-peak_offset);
}

#ifdef io_support

void Domain::dump(FILE *f) const
{
  file=f;
  fprintf(f,"  Peak   ");
  peak.print(f);
  fprintf(f,"\n  Saddle ");
  saddle.print(f);
  if (!neighbors.empty()) fprintf(f,"\n  Peak neighbors:");
  for_each(neighbors.begin(),neighbors.end(),print_dom_index);
  if (!runoffs.empty()) fprintf(f,"\n  Runoff neighbors:");
  for_each(runoffs.begin(),runoffs.end(),print_ro_index);
  if (!bsnsdls.empty()) fprintf(f,"\n  Basin saddle neighbors:");
  for_each(bsnsdls.begin(),bsnsdls.end(),print_bs_index);
  putc('\n',f);
}

#endif // def io_support

void Domain::write(FILE *f) const throw(file_error)
{
  peak.write(f);
  saddle.write(f);
  fwrite(Divide_tree::writing->index(primary_nbr),f);
}


void Domain::read(FILE *f) throw(file_error)
{
  Index indx;
  peak.read(f);
  saddle.read(f);
  fread(indx,f);
  primary_nbr=Divide_tree::reading->address(indx);
}

Elevation Domain::sadl_walk_inv_risk(Domain *a, bool drainage)
{
  Elevation elev_nbr,extreme_nbr=drainage?elev_infty:-elev_infty;
  const Domain *so=saddle_owner(a);
  for (Neighbor_iter ni(*this,a); ni; ++ni) {
    if (drainage) {
      elev_nbr=ni.saddle_owner()->saddle.elev.low;
      if (elev_nbr<extreme_nbr) extreme_nbr=elev_nbr;
    }
    else {
      elev_nbr=ni.saddle_owner()->saddle.elev.high;
      if (elev_nbr>extreme_nbr) extreme_nbr=elev_nbr;
    }
  }
  for (ROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
    if (drainage) {
      elev_nbr=(*roi)->elev.low;
      if (elev_nbr<extreme_nbr) extreme_nbr=elev_nbr;
    }
    else {
      elev_nbr=(*roi)->elev.high;
      if (elev_nbr>extreme_nbr) extreme_nbr=elev_nbr;
    }
  }
  for (BSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi) {
    if (drainage) {
      elev_nbr=(*bsi)->elev.low;
      if (elev_nbr<extreme_nbr) extreme_nbr=elev_nbr;
    }
    else {
      elev_nbr=(*bsi)->elev.high;
      if (elev_nbr>extreme_nbr) extreme_nbr=elev_nbr;
    }
  }
  return drainage?so->saddle.elev.high-extreme_nbr:
    extreme_nbr-so->saddle.elev.low;
}

Elevation Domain::SWIR_offense(Domain *a, bool drainage)
{
  const Domain *so=saddle_owner(a);
  return SWIR_offense(so->saddle.elev,drainage,a);
}

Elevation Domain::SWIR_offense(Runoff *ro, bool drainage)
{
  assert(ro->peak==this);
  return SWIR_offense(ro->elev,drainage);
}

Elevation Domain::SWIR_offense(Basin_saddle *bs, bool drainage)
{
  assert(bs->peak1==this || bs->peak2==this);
  return SWIR_offense(bs->elev,drainage);
}

Elevation Domain::SWIR_offense(Elev_intvl range, bool drainage, Domain *a)
{
  Elevation wro=elev_infty,elev=drainage?range.low:range.high,wro1;
  for (Neighbor_iter ni(*this,a); ni; ++ni) {
    if (drainage) wro1=ni.saddle_owner()->saddle.elev.high-elev;
    else wro1=elev-ni.saddle_owner()->saddle.elev.low;
    if (wro1<wro) wro=wro1;
  }
  return wro;
}


// find a maximum or minimum peak prominence
// endpt==LO_END ==> max, endpt==HI_END ==> min
Elevation Domain::extreme_pk_prom(Elev_endpt endpt)
{
  assert(peak.location);
  xplr_sup.root_LIS=elev_infty;

  for (Explorer xplr(this,endpt); xplr; ++xplr) {
    const Domain &curnt=(*xplr),&prev=*curnt.xplr_sup.root_nbr;
    curnt.xplr_sup.root_LIS=prev.xplr_sup.root_LIS;
    depress(curnt.xplr_sup.root_LIS,xplr.saddle().elev);
    if (endpt==LO_END ? curnt.peak.elev>peak.elev :
			!(curnt.peak.elev<peak.elev)) {
      // we found a higher peak.
      Elev_endpt farpt = OTHER_END(endpt);
      return peak.elev.*farpt-curnt.xplr_sup.root_LIS.*endpt;
    }
  }

  // we found no higher peak.
  return elev_infty;
}

// get the prominence of a peak
Elev_intvl Domain::peak_prom()
{
  return Elev_intvl(extreme_pk_prom(HI_END),extreme_pk_prom(LO_END));
}

// adjust the off-map prominence due to runoffs of this peak
void Domain::update_omp(const conditional_HIP& cond_HIP, Elevation& omp) const
{
  // find the highest runoff elevation
  Elev_intvl hi_ro_elev=-elev_infty;
  for (const_ROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
    if (!((*roi)->elev<xplr_sup.dpar_LIS))
      elevate(hi_ro_elev,(*roi)->elev);
  }
  if (hi_ro_elev.high==-elev_infty) return;
  // bump the runoff elev down to the root LIS
  depress(hi_ro_elev,xplr_sup.root_LIS);
  // bump the omp down to its new level
  Elevation this_omp=cond_HIP.get_HIP(hi_ro_elev)-hi_ro_elev.high;
  if (this_omp<omp) omp=this_omp;
}

Elevation Domain::offmap_pk_prom()
{
  Elevation omp=elev_infty;
  Elev_intvl lo_sadl=elev_infty;
  conditional_HIP cond_HIP(peak.elev.low);
  set_island(false,cond_HIP);

  // collect edge LISs from each runoff
  update_omp(cond_HIP,omp);
  for (Explorer xplr(this,LO_END); xplr; ) {
    const Domain &curnt=(*xplr),*root_nbr=curnt.xplr_sup.root_nbr;
    // have we left the prominence island? If so, discontinue this branch.
    if (xplr.saddle().elev<root_nbr->xplr_sup.dpar_LIS ||
	curnt.xplr_sup.root_LIS<curnt.xplr_sup.dpar_LIS) {
      xplr.stop_branch();
      continue;
    }
    // propagate the definite parent LIS out from the root
    elevate(curnt.xplr_sup.dpar_LIS,root_nbr->xplr_sup.dpar_LIS);
    // update omp
    curnt.update_omp(cond_HIP,omp);
    ++xplr;
  }

  return omp;
}

// helper function for computing saddle prominence.
static bool island_xplr(Explorer& xplr, Elev_endpt endpt,
			Elevation& pe, const Elev_intvl& use)
{
  if (!xplr) {
    // we've exhausted the prominence island.
    return true;
  }
  if (endpt==HI_END ? xplr.saddle().elev<use :
		      !(xplr.saddle().elev>use))
    xplr.stop_branch(); // we can't drop out of the prominence island
  else {
    Elevation new_pe=(*xplr).peak.elev.*endpt;
    if (new_pe>pe)
      pe=new_pe; // we found a higher peak in the prominence island
    ++xplr;
  }
  return false;
}

// find a maximum or minimum saddle prominence
// endpt==HI_END ==> max, endpt==LO_END ==> min
Elevation Domain::extreme_sadl_prom(Elevation ceiling, Elev_endpt endpt)
{
  assert(peak.location && primary_nbr);
  if (ceiling<saddle.elev.low) return -elev_infty;
  Elev_intvl use=saddle.elev;
  if (ceiling<use.high) use.high=ceiling;
  Explorer xplr1(this,primary_nbr,endpt),xplr2(primary_nbr,this,endpt);
  Elevation pe1=peak.elev.*endpt,pe2=primary_nbr->peak.elev.*endpt;
  Elev_endpt farpt = OTHER_END(endpt);
  while (true) {
    // advance the explorer on whichever side has seen the lower highest peak
    if (pe1<pe2) {
      if (island_xplr(xplr1,endpt,pe1,use)) return pe1-use.*farpt;
    }
    else {
      if (island_xplr(xplr2,endpt,pe2,use)) return pe2-use.*farpt;
    }
  }
  assert(0);
  return -elev_infty;
}

Elevation Domain::max_sadl_prom()
{
  Elevation sp=extreme_sadl_prom(HI_END),omsp=offmap_sadl_prom();
  return sp>omsp?sp:omsp;
}

Elev_intvl Domain::sadl_promx(Elevation ceiling)
{
  return Elev_intvl(extreme_sadl_prom(ceiling,LO_END),
		    extreme_sadl_prom(ceiling,HI_END));
}

static void update_sadl_omp(const Domain& d, const Elev_intvl& use, bool& off)
{
  const vector<Runoff *>& ros=d.runoffs;
  for (const_ROiter roi=ros.begin(); roi!=ros.end(); ++roi)
    if (!((*roi)->elev<use)) {
      // we found a runoff in the prominence island
      off=true;
      break;
    }
}

// helper function for computing offmap saddle prominence.
static void offmap_island_xplr(Explorer& xplr, Elev_intvl& pe,
			       bool& off, const Elev_intvl& use)
{
  assert(xplr);
  if (xplr.saddle().elev<use)
    xplr.stop_branch(); // we can't drop out of the prominence subisland
  else {
    const Elev_intvl& new_pe=(*xplr).peak.elev;
    // update subisland HP
    elevate(pe,new_pe);
    // update subisland runoff status
    update_sadl_omp(*xplr,use,off);
    ++xplr;
  }
}

Elevation Domain::offmap_sadl_prom(Elevation ceiling)
{
  assert(peak.location && primary_nbr);
  if (ceiling<saddle.elev.low) return -elev_infty;
  Elev_intvl use=saddle.elev;
  if (ceiling<use.high) use.high=ceiling;
  Explorer xplr1(this,primary_nbr,LO_END),xplr2(primary_nbr,this,LO_END);
  Elev_intvl pe1=peak.elev,pe2=primary_nbr->peak.elev;
  bool off1=false,off2=false;
  update_sadl_omp(*this,use,off1);
  update_sadl_omp(*primary_nbr,use,off2);
  while (true) {
    if (off1 && off2) return elev_infty; // runoff on both sides; offmap prominence unlimited
    if (!xplr1 && !off1 && pe1<pe2 ||
	!xplr2 && !off2 && pe2<pe1) {
      // exhausted low side w/o runoff, no effect on prominence
      return -elev_infty;
    }
    if (xplr1 && (!xplr2 || pe1.low<pe2.low)) {
      offmap_island_xplr(xplr1,pe1,off1,use);
    }
    else if (xplr2 && (!xplr1 || pe2.low<=pe1.low)) {
      offmap_island_xplr(xplr2,pe2,off2,use);
    }
    else {
      // exhausted both sides
      assert(!xplr1 && !xplr2);
      if (off1) return pe2.high-use.low;
      if (off2) return pe1.high-use.low;
      return -elev_infty;
    }
  }
  assert(0);
  return -elev_infty;
}


Domain *Domain::common_nbr(Domain *a)
{
  if (primary_nbr==a->primary_nbr) return primary_nbr;
  if (primary_nbr && primary_nbr->primary_nbr==a) return primary_nbr;
  if (a->primary_nbr && a->primary_nbr->primary_nbr==this) return a->primary_nbr;
  return 0;
}

Domain *Domain::patriarch()
{
  Domain *start;
  for (start=this; start->primary_nbr; start=start->primary_nbr);
  return start;
}

Domain *Domain::ancestor(unsigned n)
{
  Domain *d=this;
  while (n>0) {
    d=d->primary_nbr;
    --n;
  }
  return d;
}

// propagate a peripheral LIS from an outlying point back toward the root
void Domain::transmit_LIS(Elev_intvl Domain::Explorer_support::*LIS) const
{
  const Domain *d=this,*r;
  Elev_intvl pl=d->xplr_sup.*LIS;
  while (true) {
    r=d->xplr_sup.root_nbr;
    if (r==0) break;
    depress(pl,d->saddle_owner(r)->saddle.elev);
    d=r;
    if (pl.low<=(d->xplr_sup.*LIS).low &&
	pl.high<=(d->xplr_sup.*LIS).high) break;
    elevate(d->xplr_sup.*LIS,pl);
  }
}

// update highest peak & lowest saddle seen so far
void conditional_HIP::update(const Elev_intvl& LIS, Elevation pk_elev)
{
  Elevation& hi_peak=hip_lis.back().HIP;
  if (pk_elev>hi_peak) {
    const Elev_intvl& old_lo_sadl=hip_lis.back().LIS;
    if (LIS.low==old_lo_sadl.low && LIS.high==old_lo_sadl.high)
      hi_peak=pk_elev;
    else {
      assert(LIS.low<old_lo_sadl.low ||
	     LIS.low==old_lo_sadl.low && (LIS.high==LIS.low ||
					  old_lo_sadl.high>old_lo_sadl.low));
      hip_lis.push_back(HIP_LIS(pk_elev,LIS));
    }
  }
}

// get the HIP for a specific LIS
Elevation conditional_HIP::get_HIP(const Elev_intvl& LIS) const
{
  vector<HIP_LIS>::const_reverse_iterator upei;
  for (upei=hip_lis.rbegin(); !(LIS<(*upei).LIS); ++upei)
    assert(upei!=hip_lis.rend());
  return (*upei).HIP;
}

// set the root LIS and parent LISes in the prominence island
// and the path to the nearest line parent
void Domain::set_island(bool prom_sadl, conditional_HIP& cond_HIP)
{
  Elev_intvl pLIS;
  xplr_sup.dpar_LIS=xplr_sup.ppar_LIS=-elev_infty;
  xplr_sup.root_LIS=elev_infty;
  const Domain *pHIP_dom,*srcdom,*dom;

  for (Explorer xplr(this,LO_END); xplr; ) {
    const Domain &curnt=(*xplr),*root_nbr=curnt.xplr_sup.root_nbr;
    // update the definite parent LIS by drawing from the root
    if (xplr_sup.dpar_LIS>-elev_infty) {
      for (srcdom=root_nbr; srcdom->xplr_sup.dpar_LIS==-elev_infty;
	   srcdom=srcdom->xplr_sup.root_nbr);
      for (dom=root_nbr; dom!=srcdom; dom=dom->xplr_sup.root_nbr)
	dom->xplr_sup.dpar_LIS=srcdom->xplr_sup.dpar_LIS;
    }
    // have we left the prominence island? If so, discontinue this branch.
    const Elev_intvl& se=xplr.saddle().elev;
    if (se<root_nbr->xplr_sup.dpar_LIS) {
      xplr.stop_branch();
      continue;
    }
    // propagate the root LIS
    curnt.xplr_sup.root_LIS=root_nbr->xplr_sup.root_LIS;
    depress(curnt.xplr_sup.root_LIS,se);
    if (prom_sadl) {
      // Is the current peak possibly higher than the original?
      if (!(peak.elev>curnt.peak.elev)) {
	// yes, transmit the possible parent LIS back toward the root.
	curnt.xplr_sup.ppar_LIS=elev_infty;
	curnt.transmit_LIS(&Explorer_support::ppar_LIS);
      }
      else curnt.xplr_sup.ppar_LIS=-elev_infty;
      // update the peripheral HIP
      curnt.xplr_sup.pHIP=-elev_infty;
      pLIS=elev_infty;
      for (pHIP_dom=&curnt;
	   !(pLIS<pHIP_dom->xplr_sup.root_LIS) &&
	   pHIP_dom->xplr_sup.pHIP<curnt.peak.elev.high;
	   pHIP_dom=pHIP_dom->xplr_sup.root_nbr) {
	pHIP_dom->xplr_sup.pHIP=curnt.peak.elev.high;
	depress(pLIS,pHIP_dom->saddle_owner(pHIP_dom->xplr_sup.root_nbr)->saddle.elev);
      }
    }
    // Is the current peak definitely higher than the original?
    if (curnt.peak.elev>peak.elev) {
      // yes, transmit the definite parent LIS back toward the root.
      curnt.xplr_sup.dpar_LIS=elev_infty;
      curnt.transmit_LIS(&Explorer_support::dpar_LIS);
      xplr.stop_branch();
      continue;
    }
    curnt.xplr_sup.dpar_LIS=-elev_infty;
    // update highest peak & lowest saddle seen so far
    cond_HIP.update(curnt.xplr_sup.root_LIS,curnt.peak.elev.low);
    // this cannot be in the "for" statement,
    // because the continue statements above are supposed to skip it
    ++xplr;
  }
}

void Domain::set_edge_nbr_LIS() const
{
  xplr_sup.edge_LIS=-elev_infty;
  for (const_ROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    elevate(xplr_sup.edge_LIS,(*roi)->elev);
}

void Domain::set_edge_LIS()
{
  set_edge_nbr_LIS();
  for (Explorer xplr(this,LO_END); xplr; ) {
    const Domain& curnt=(*xplr);
    // have we left the prominence island?
    if (curnt.xplr_sup.root_LIS<curnt.xplr_sup.dpar_LIS) {
      // yes, stop exploring in this direction.
      xplr.stop_branch();
      continue;
    }
    // update the edge LIS
    curnt.set_edge_nbr_LIS();
    curnt.transmit_LIS(&Explorer_support::edge_LIS);
    ++xplr;
  }
}

#ifdef io_support

void Domain::list_prom_runoffs(Prom_sadl_CB pscb,
	const conditional_HIP& cond_HIP, Elevation pk_hi_elev,
	const Prom_sadl_filter& filter, const Database& data)
{
  if (!filter.runoffs) return;
  Elevation root_LIS=xplr_sup.root_LIS.high;
  Elev_intvl use;
  use.low=xplr_sup.dpar_LIS.low;
  for (const_ROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
    use.high=root_LIS;
    if (use.high<use.low || (*roi)->elev<use || !filter.runoff_filter.test(**roi,data)) continue;
    if ((*roi)->elev.high<use.high) use.high=(*roi)->elev.high;
    (*pscb)(**roi,FT_RUNOFF,Elev_intvl(cond_HIP.get_HIP(use),pk_hi_elev),use,false);
  }
}

void Domain::list_prom_sadls(Prom_sadl_CB pscb,
	const Prom_sadl_filter& filter, const Database& data)
{
  Elevation hi_upe,lo_upe;
  Elev_intvl use,off_use;
  bool offmap=filter.off_sadls || filter.runoffs;
  conditional_HIP cond_HIP(peak.elev.low);

  set_island(true,cond_HIP);
  if (offmap) set_edge_LIS();
  list_prom_runoffs(pscb,cond_HIP,peak.elev.high,filter,data);
  // The explorer must use the low end so that cond_HIP will be computed correctly.
  for (Explorer xplr(this,LO_END); xplr; ) {
    Domain& curnt=(*xplr);
    Saddle& cur_sadl=xplr.saddle();
    // did we leave the prominence island? If so, discontinue this branch.
    if (cur_sadl.elev<curnt.xplr_sup.root_nbr->xplr_sup.dpar_LIS) {
      xplr.stop_branch();
      continue;
    }
    // determine usable saddle elevation
    xplr.use(use,offmap?&off_use:0);
    // Does it pass the filter?
    if (filter.saddle_filter.test(cur_sadl,data)) {
      // does the current actual saddle elevation intersect the USE?
      if (!use.is_empty()) {
	// yes! it's a possible prom saddle with on-map parent
	hi_upe=peak.elev.high<curnt.xplr_sup.pHIP?
	       peak.elev.high:curnt.xplr_sup.pHIP;
	if (hi_upe-use.low>=filter.min_prom || filter.min_prom==0) { // enough prominence
	  lo_upe=cond_HIP.get_HIP(use);
	  if (hi_upe>=lo_upe)
	    (*pscb)(cur_sadl,FT_SADDLE,Elev_intvl(lo_upe,hi_upe),use,false);
	}
      }
      // does the actual saddle elevation intersect the off-map USE?
      if (filter.off_sadls && !off_use.is_empty()) {
	// yes. it's a possible prom saddle with off-map parent
	if (peak.elev.high-off_use.low>=filter.min_prom) { // enough prominence
	  (*pscb)(cur_sadl,FT_SADDLE,Elev_intvl(cond_HIP.get_HIP(off_use),peak.elev.high),off_use,true);
	}
      }
    }
    curnt.list_prom_runoffs(pscb,cond_HIP,peak.elev.high,filter,data);
    // propagate the definite parent LIS outward
    elevate(curnt.xplr_sup.dpar_LIS,curnt.xplr_sup.root_nbr->xplr_sup.dpar_LIS);
    ++xplr;
  }
}

static void draw_dpar_LIS(Domain *dom)
{
  // draw definite parent LIS from the root
  for (Domain *srcdom=dom; srcdom; srcdom=srcdom->xplr_sup.root_nbr)
    elevate(dom->xplr_sup.dpar_LIS,srcdom->xplr_sup.dpar_LIS);
}

static void transmit_HIP(Domain *dom, const Elev_intvl& floor)
{
  for (; dom->xplr_sup.root_nbr &&
	 dom->saddle_owner(dom->xplr_sup.root_nbr)->saddle.elev>floor &&
	 dom->xplr_sup.pHIP>dom->xplr_sup.root_nbr->xplr_sup.pHIP;
       dom=dom->xplr_sup.root_nbr) {
    dom->xplr_sup.root_nbr->xplr_sup.pHIP=dom->xplr_sup.pHIP;
  }
}

void Domain::list_parnt_runoffs(const Elev_intvl& floor, Parent_CB parent_cb,
				bool list_runoffs) const
{
  if (list_runoffs) {
    for (const_ROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
      if (!((*roi)->elev<floor))
	(*parent_cb)(**roi,FT_RUNOFF);
    }
  }
}

bool Domain::list_line_parent(const Elev_intvl& child_elev, Parent_CB parent_cb,
			      bool list_runoffs)
{
  // if this is possibly higher than the original peak, add it.
  if (!(peak.elev<child_elev))
    (*parent_cb)(peak,FT_PEAK);
  // if this is definitely higher than original peak, bail.
  if (peak.elev>child_elev)
    return false;
  // add neighboring runoffs.
  list_parnt_runoffs(xplr_sup.dpar_LIS,parent_cb,list_runoffs);
  return true;
}

void Domain::list_cell_parent(Elevation min_prom, const Elev_intvl& child_elev,
			      Parent_CB parent_cb, bool list_runoffs)
{
  // if /this/ has enough prominence, add it.
  if (prom.peak>=min_prom && !(peak.elev<child_elev))
    (*parent_cb)(peak,FT_PEAK);
  // add neighboring runoffs.
  list_parnt_runoffs(xplr_sup.dpar_LIS,parent_cb,list_runoffs);
}

void Domain::list_island_parent(const Elev_intvl& use, Parent_CB parent_cb,
				bool list_runoffs)
{
  // if dom is possibly higher than its peripheral HIP, add it.
  if (!(peak.elev<xplr_sup.pHIP))
    (*parent_cb)(peak,FT_PEAK);
  // add neighboring runoffs.
  list_parnt_runoffs(use,parent_cb,list_runoffs);
}

void Domain::list_parents(Domain& sadl_dom, const Elev_intvl& upe, const Elev_intvl& use,
			  Domain::Topology topo, Parent_CB parent_cb, bool list_runoffs)
{
  Domain *child_side_pk,*parnt_side_pk,*HIP_pk;
  Elev_intvl par_se,pk_prom;
  if (sadl_dom.xplr_sup.root_nbr==sadl_dom.primary_nbr) {
    parnt_side_pk=&sadl_dom;
  }
  else {
    assert(sadl_dom.primary_nbr->xplr_sup.root_nbr==&sadl_dom);
    parnt_side_pk=sadl_dom.primary_nbr;
  }
  child_side_pk=parnt_side_pk->xplr_sup.root_nbr;

  Explorer xplr(parnt_side_pk,child_side_pk,HI_END);
  switch (topo) {
  case Domain::TOPO_LINE:
    // dpar_LIS was already set up when the prom saddle list
    // of the original peak was generated.
    if (parnt_side_pk->list_line_parent(upe,parent_cb,list_runoffs)) {
      while (xplr) {
	Domain &curnt_dom=(*xplr),*root_nbr=curnt_dom.xplr_sup.root_nbr;
	if (xplr.saddle().elev<root_nbr->xplr_sup.dpar_LIS) {
	  // there's definitely a higher branch to a parent elsewhere.
	  // Stop exploring this branch.
	  xplr.stop_branch();
	}
	else if (!curnt_dom.list_line_parent(upe,parent_cb,list_runoffs)) {
	  // we found a definitely higher parent; stop exploring this branch.
	  xplr.stop_branch();
	}
	else ++xplr;
      }
    }
    break;
  case Domain::TOPO_CELL:
    parnt_side_pk->xplr_sup.dpar_LIS=use;
    HIP_pk=parnt_side_pk;
    pk_prom=upe-use;
    par_se.high=HIP_pk->peak.elev.low-pk_prom.low;
    par_se.low=HIP_pk->peak.elev.low-pk_prom.high;
    elevate(par_se,use);
    // we must order by HI_END because that insures that parent-sufficient
    // prominences are discovered correctly.
    while (xplr) {
      Domain &curnt_dom=(*xplr),*root_nbr=curnt_dom.xplr_sup.root_nbr;
      const Elev_intvl& se=xplr.saddle().elev;
      if (se<par_se) {
	// we're trying to leave a contour that definitely contains
	// a qualifying parent somewhere.
	// Adjust the parent LIS
	elevate(HIP_pk->xplr_sup.dpar_LIS,par_se);
	HIP_pk->transmit_LIS(&Explorer_support::dpar_LIS);
	// and reset the HIP
	par_se=-elev_infty;
	HIP_pk=0;
      }
      draw_dpar_LIS(root_nbr);
      if (se<root_nbr->xplr_sup.dpar_LIS) {
	// there's definitely a higher branch to a parent elsewhere.
	// Discontinue this branch.
	xplr.stop_branch();
	continue;
      }
      curnt_dom.xplr_sup.dpar_LIS=use;
      // update the running HIP
      if (!HIP_pk || curnt_dom.peak.elev.low>HIP_pk->peak.elev.low) {
	HIP_pk=&curnt_dom;
      }
      // update par_se
      par_se.high=HIP_pk->peak.elev.low-pk_prom.low;
      par_se.low=HIP_pk->peak.elev.low-pk_prom.high;
      elevate(par_se,use);
      ++xplr;
    }
    if (HIP_pk) {
      // this is in case explorer exhaustion constituted prom group detection
      elevate(HIP_pk->xplr_sup.dpar_LIS,par_se);
      HIP_pk->transmit_LIS(&Explorer_support::dpar_LIS);
    }
    parnt_side_pk->list_cell_parent(pk_prom.low,peak.elev,parent_cb,list_runoffs);
    for (xplr.reset(parnt_side_pk,child_side_pk,HI_END); xplr; ) {
      const Elev_intvl& se=xplr.saddle().elev;
      Domain &curnt_dom=(*xplr),*root_nbr=curnt_dom.xplr_sup.root_nbr;
      if (se<root_nbr->xplr_sup.dpar_LIS) {
	// there's definitely a higher branch to a parent elsewhere.
	// Discontinue this branch.
	xplr.stop_branch();
	continue;
      }
      // propagate dpar_LIS away from the root
      elevate(curnt_dom.xplr_sup.dpar_LIS,root_nbr->xplr_sup.dpar_LIS);
      curnt_dom.list_cell_parent(pk_prom.low,peak.elev,parent_cb,list_runoffs);
      ++xplr;
    }
    break;
  case Domain::TOPO_ISLAND:
    parnt_side_pk->xplr_sup.pHIP=parnt_side_pk->peak.elev.low;
    while (xplr) {
      const Elev_intvl& se=xplr.saddle().elev;
      if (se<use) {
	// We've dropped below the original saddle.
	// Discontinue this branch.
	xplr.stop_branch();
	continue;
      }
      Domain &curnt_dom=(*xplr);
      curnt_dom.xplr_sup.pHIP=curnt_dom.peak.elev.low;
      // transmit pHIP toward the root
      transmit_HIP(&curnt_dom,use);
      ++xplr;
    }
    // bump up the initial HIP to the peak elevation
    if (parnt_side_pk->xplr_sup.pHIP<upe.low)
      parnt_side_pk->xplr_sup.pHIP=upe.low;
    parnt_side_pk->list_island_parent(use,parent_cb,list_runoffs);
    for (xplr.reset(parnt_side_pk,child_side_pk,HI_END); xplr; ) {
      const Elev_intvl& se=xplr.saddle().elev;
      if (se<use) {
	// We've dropped below the original saddle.
	// Discontinue this branch.
	xplr.stop_branch();
	continue;
      }
      Domain &curnt_dom=(*xplr),*root_nbr=curnt_dom.xplr_sup.root_nbr;
      // propagate the HIP away from the root
      if (curnt_dom.xplr_sup.pHIP<root_nbr->xplr_sup.pHIP)
	curnt_dom.xplr_sup.pHIP=root_nbr->xplr_sup.pHIP;
      curnt_dom.list_island_parent(use,parent_cb,list_runoffs);
      ++xplr;
    }
    break;
  default:
    assert(0);
  }
}

#endif // def io_support

void Domain::make_patriarch()
{
  Domain *p=this,*a=primary_nbr,*b;
  Saddle s1=saddle,s2;
  primary_nbr=0;
  saddle.clear();
  while (a) {
    b=a->primary_nbr;
    a->primary_nbr=p;
    assert(a!=a->primary_nbr);
    s2=a->saddle;
    a->saddle=s1;
    p=a;
    a=b;
    s1=s2;
  }
}

void Domain::low_saddle_leg(Saddle *&low_sadl, const Domain *stop,
			    int sign, Elev_endpt endpt, bool splice_mode)
{
  Saddle::Status curnt_stat,low_stat;
  int elev_diff;
  Distance fs_diff;
  for (Domain *start=this; start->primary_nbr && start!=stop; start=start->primary_nbr) {
    if (low_sadl==0) {
      low_sadl=&start->saddle;
    }
    else {
      curnt_stat=start->saddle.eff_stat(splice_mode);
      low_stat=low_sadl->eff_stat(splice_mode);
      elev_diff=sign*start->saddle.elev.*endpt-low_sadl->elev.*endpt;
      fs_diff=start->saddle.flat_sep-low_sadl->flat_sep;
      if (curnt_stat<low_stat ||
	  (curnt_stat==low_stat && (elev_diff<0 ||
				    elev_diff==0 && fs_diff>0))) {
	low_sadl=&start->saddle;
      }
    }
  }
}

void Domain::change_nbr(Domain *from, Domain *to)
{
  for (Dom_iter nbri=neighbors.begin(); nbri!=neighbors.end(); ++nbri) {
    if (*nbri==from) {
      *nbri=to;
      break;
    }
  }
}

Domain::Sadl_cxn
Domain::SWI_switch_cxn(Domain *sadl1, Domain *sadl2, Domain *&ca,
		       short int dc, Elev_endpt inner, Elev_endpt outer)
{
  ca=common_ancestor(sadl1,sadl2);
  if (ca==0) return NO_CXN;
  Elevation low1=dc*sadl1->saddle.elev.*inner,low2=dc*sadl2->saddle.elev.*inner;
  return check_low_cxn(sadl1,sadl2,ca,low1<low2?low1:low2,dc,outer);
}

void Domain::SWI_switch(Domain& a, const Domain *ca)
{
  if (ca==&a) a.primary_nbr->reconnect(*this,ca);
  else a.reconnect(*this,ca);
}

// connect this to sadl
void Domain::reconnect(Domain& sadl, const Domain *ca)
{
  if (ca==&sadl) {
    // this is connected to child side of sadl
    sadl.primary_nbr->change_nbr(&sadl,this);
    sadl.remove_nbr(sadl.primary_nbr);
    add_nbr(sadl.primary_nbr);
    Saddle high_saddle=sadl.saddle;
    Domain *parnt=sadl.primary_nbr;
    sadl.saddle.clear();
    sadl.primary_nbr=0;
    make_patriarch();
    primary_nbr=parnt;
    saddle=high_saddle;
  }
  else {
    // this is connected to parent side of sadl
    sadl.change_nbr(sadl.primary_nbr,this);
    sadl.primary_nbr->remove_nbr(&sadl);
    add_nbr(&sadl);
    sadl.primary_nbr=this;
  }
}

Domain *Domain::common_ancestor(Domain *dom1, Domain *dom2)
{
  unsigned i=0;
  Domain *end1=dom1,*end2=dom2,*i1,*i2;

  while (end1!=0 || end2!=0) {
    ++i;
    if (end1!=0) {
      for (i2=dom2; i2!=end2; i2=i2->primary_nbr)
	if (end1==i2) return end1;
      end1=end1->primary_nbr;
    }
    if (end2!=0) {
      for (i1=dom1; i1!=end1; i1=i1->primary_nbr)
	if (end2==i1) return end2;
      end2=end2->primary_nbr;
    }
  }

  return 0;
}

unsigned Domain::length(const Domain *dom1, const Domain *dom2)
{
  unsigned i;
  for (i=0; dom1!=dom2; dom1=dom1->primary_nbr)
    ++i;
  return i;
}

void Domain::connect_nbr(const Basin_saddle& bs, Domain *dom1, Domain *dom2)
{
  if (bs.location) {
    if ((bs.peak1!=dom1 || bs.peak2!=dom2) &&
	(bs.peak1!=dom2 || bs.peak2!=dom1)) {
      bs.peak1->remove_nbr(bs.peak2);
      bs.peak2->remove_nbr(bs.peak1);
      dom1->add_nbr(dom2);
      dom2->add_nbr(dom1);
    }
  }
  else {
    dom1->add_nbr(dom2);
    dom2->add_nbr(dom1);
  }
}

void Domain::connect(Saddle& junct, Domain *dom1, Domain *dom2,
		     Basin_saddle& ejected_sadl, bool splice_mode, bool immune)
{
  Domain *comn_anc=0;
  Saddle *low_saddle;

  // Check for "triangle".
  if (dom1->primary_nbr==dom2->primary_nbr)
    comn_anc=dom1->primary_nbr;
  else if (dom1->primary_nbr && dom1->primary_nbr->primary_nbr==dom2)
    comn_anc=dom2;
  else if (dom2->primary_nbr && dom2->primary_nbr->primary_nbr==dom1)
    comn_anc=dom1;

  if (!comn_anc) {
    // No triangle found.  Check for more distant common ancestor.
    dom2->make_patriarch();
    comn_anc=dom1->patriarch();
    if (comn_anc!=dom2) comn_anc=0;
  }

  if (comn_anc) {
    // Common ancestor found.
    // Look for saddle lower than junction along each path to ancestor.
    // if immune flag is set, new junction saddle is immune from ejection
    low_saddle=immune?0:&junct;
    dom1->low_saddle_leg(low_saddle,comn_anc,1,HI_END,splice_mode);
    dom2->low_saddle_leg(low_saddle,comn_anc,1,HI_END,splice_mode);
  }
  else low_saddle=0; // No common ancestor.  No need to find low saddle.

  if (low_saddle==0) ejected_sadl.clear(); // not connected, no saddle ejected.
  else if (low_saddle==&junct) {
    // No lower saddle was found, so we just regurgitate attempted junction.
    (Saddle&)ejected_sadl=junct;
    ejected_sadl.peak1=dom1;
    ejected_sadl.peak2=dom2;
  }
  else {
    // Saddle lower than junction was found.  Break it.
    (Saddle&)ejected_sadl=*low_saddle;
    Domain *low_sadl_dom=Domain::FromSadl(low_saddle);
    ejected_sadl.peak1=low_sadl_dom;
    ejected_sadl.peak2=low_sadl_dom->primary_nbr;
    assert(dom1->primary_nbr!=dom2 && dom2->primary_nbr!=dom1 ||
	   ejected_sadl.peak1==dom1 && ejected_sadl.peak2==dom2 ||
	   ejected_sadl.peak2==dom1 && ejected_sadl.peak1==dom2);
    low_sadl_dom->primary_nbr=0;
    low_saddle->clear();
    comn_anc=0; // Broken link cancels common ancestor.
  }

  if (!comn_anc) {
    // No common ancestor (anymore). Connect domains.
    dom2->make_patriarch();
    dom2->primary_nbr=dom1;
    assert(dom2!=dom2->primary_nbr);
    dom2->saddle=junct;
  }

}

Domain *Domain::high_nbr_saddle(int sign)
{
  Neighbor_iter ni(*this);
  Elev_endpt low_end=sign>0 ? LO_END:HI_END, high_end=sign>0 ? HI_END:LO_END;
  Domain *max_low_sadl_nbr=*ni;
  Elev_intvl *max_low_sadl=
    &saddle_owner(max_low_sadl_nbr)->saddle.elev;
  Elevation other_high_sadl=-elev_infty;
  for (++ni; ni; ++ni) {
    if (ni.saddle_owner()->saddle.elev.*low_end*sign>max_low_sadl->*low_end*sign) {
      if (max_low_sadl->*high_end*sign>other_high_sadl)
	other_high_sadl=max_low_sadl->*high_end*sign;
      max_low_sadl_nbr=*ni;
      max_low_sadl=&saddle_owner(max_low_sadl_nbr)->saddle.elev;
    }
    else if (ni.saddle_owner()->saddle.elev.*high_end>other_high_sadl)
      other_high_sadl=ni.saddle_owner()->saddle.elev.*high_end*sign;
  }
  if (other_high_sadl>=max_low_sadl->*low_end*sign)
    return 0;
  for (ROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi)->elev.*high_end*sign>=max_low_sadl->*low_end*sign)
      return 0;
  for (BSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    if ((*bsi)->elev.*high_end*sign>=max_low_sadl->*low_end*sign)
      return 0;
  return max_low_sadl_nbr;
}


void Domain::remove_nbr(const Domain *a)
{
  Dom_iter back=std::remove(neighbors.begin(),neighbors.end(),a);
  // trim trailing junk left by remove()
  while (neighbors.end()!=back) neighbors.pop_back();
}

void Domain::add_nbr(Domain *nbr)
{
  neighbors.push_back(nbr);
}

void Domain::remove_nbr(const Basin_saddle *a)
{
  BSiter back=std::remove(bsnsdls.begin(),bsnsdls.end(),a);
  // trim trailing junk left by remove()
  while (bsnsdls.end()!=back) bsnsdls.pop_back();
}

void Domain::add_nbr(Basin_saddle *bs)
{
  assert(bs->peak1==this || bs->peak2==this);
  bsnsdls.push_back(bs);
}

void Domain::remove_nbr(const Runoff *a)
{
  ROiter back=std::remove(runoffs.begin(),runoffs.end(),a);
  // trim trailing junk left by remove()
  while (runoffs.end()!=back) runoffs.pop_back();
}

void Domain::add_nbr(Runoff *ro)
{
  assert(ro->peak==this);
  runoffs.push_back(ro);
}

// comparison function for peak sort
static bool high_peak(Domain *a, Domain *b)
{
  return a->peak.elev.low>b->peak.elev.low;
}

// Sort the neighbors of a peak
void Domain::sort_nbrs()
{
  sort(neighbors.begin(),neighbors.end(),high_peak);
}

void Domain::cut(Domain *a)
{
  Domain *so=saddle_owner(a);
  assert(so!=0);
  so->saddle.clear();
  so->primary_nbr=0;
  remove_nbr(a);
  a->remove_nbr(this);
}

void Domain::remove(Domain *elim_saddle, bool xfer_edge_effect)
{
  if (neighbors.empty()) {
    // Peak has no neighbors.  Just kick it out.
    assert(runoffs.empty()==0 && bsnsdls.empty());
    peak.clear();
    return;
  }
  assert(elim_saddle);

  // Cut the connection made by the removed saddle
  cut(elim_saddle);

  // Merge neighbor lists
  Neighbor_iter ni=*this;
  for (; ni; ++ni)
    elim_saddle->add_nbr(*ni);
  sort(neighbors.begin(),neighbors.end(),high_peak);

  // Reconnect runoffs of deleted peak to the subsuming peak.
  ROiter roi;
  for (roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
    (*roi)->peak=elim_saddle;
    if (!xfer_edge_effect) (*roi)->edge_effect=false;
  }

  // ...ditto for basin saddles
  BSiter bsi;
  for (bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi) {
    if ((*bsi)->peak1==this) (*bsi)->peak1=elim_saddle;
    if ((*bsi)->peak2==this) (*bsi)->peak2=elim_saddle;
  }

  // Merge runoff lists
  for (roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    elim_saddle->runoffs.push_back(*roi);

  // Merge basin saddle lists
  for (bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    elim_saddle->bsnsdls.push_back(*bsi);

  // Reconnect neighbors of the deleted peak to the subsuming peak.
  for (ni.reset(); ni; ++ni) {
    if ((*ni)->primary_nbr==this) {
      (*ni)->primary_nbr=elim_saddle;
      assert(*ni!=(*ni)->primary_nbr);
    }
    else {
      assert(primary_nbr==(*ni));
      assert(elim_saddle->primary_nbr==0);
      elim_saddle->primary_nbr=*ni;
      assert(elim_saddle!=elim_saddle->primary_nbr);
      elim_saddle->saddle=saddle;
    }
    // modify neighbor list of neighbors of deleted peak.
    (*ni)->change_nbr(this,elim_saddle);
  }

  neighbors.clear();
  peak.clear();
  primary_nbr=0;
  saddle.clear();
}

void Domain::remove_river(Domain *peak_end, Domain *sadl_end)
{
  Domain *ca=common_ancestor(peak_end,sadl_end),*p;
  while (peak_end!=ca) {
    p=peak_end->primary_nbr;
    peak_end->remove(p);
    peak_end=p;
  }
  if (ca!=sadl_end) {
    while (sadl_end->primary_nbr!=ca) {
      sadl_end->primary_nbr->remove(sadl_end);
    }
    ca->remove(sadl_end);
  }
  else sadl_end->remove(sadl_end->primary_nbr);
}


#ifdef io_support

#if 0

void Domain::print(FILE *f, bool offmap_sadls, bool runoffs, bool basnsadls)
{
  scout_branch(); // scout now so that we know if peak is possible tree HP
  first_prom=true;

  for (const char *fc=format; *fc; fc++) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	fprintf(f,id_format,Divide_tree::writing->index(this));
	break;
      case 'K': case 'k': case 'F': case 'f':
	peak.print(f);
	break;
      case 'P': case 'p':
	if (scratch.parent_LIS==-elev_infty) {
	  // No definite parent, this peak might be the tree HP.
	  // TODO: find usable elevation for HP.
	  if (HP_format) {
	    fprintf(f,HP_format);
	    first_prom=false;
	  }
	  else print_prom(f,Feature(),undef,peak.elev,
			  -elev_infty,onmap_parent_string);
	}
	{
	  Saddle_iter sdli(*this,runoffs);
	  for (; sdli; ++sdli) {
	    if (!sdli.runoff()) {
	      if (sdli.onmap_parent()) {
		print_prom(f,*sdli,sdli.index(),
		  sdli.usable_peak(),sdli.usable_saddle(),
		  onmap_parent_string);
	      }
	      if (sdli.offmap_parent() && offmap_sadls) {
		print_prom(f,*sdli,sdli.index(),
		  sdli.usable_peak_edge(),sdli.usable_saddle_edge(),
		  offmap_parent_string);
	      }
	    }
	    else {
	      print_prom(f,*sdli,-(long)(sdli.index()),
		sdli.usable_peak_runoff(),sdli.usable_saddle_runoff(),
		  runoff_string);
	    }
	  }
	  if (offmap_sadls && sdli.get_edge_LIS()>-elev_infty) {
	    Elev_intvl use(scratch.parent_LIS,sdli.get_edge_LIS()),
	      upe=Elev_intvl(sdli.get_edge_HIP(),peak.elev.high);
	    print_prom(f,Feature(GridPoint::undefined,use),undef,
	      upe,use,offmap_parent_string);
	  }
	}
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Domain format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}

void Domain::print_prom(FILE *f, const Feature& saddle, Index indx,
			const Elev_intvl& usable_peak,
			const Elev_intvl& usable_saddle,
			const char *om_string)
{
  const char *fc;
  if (first_prom && first_prom_format) {
    first_prom=false;
    fc=first_prom_format;
  }
  else fc=prom_format;

  for (; *fc; fc++) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	if (indx!=undef) fprintf(f,id_format,indx);
	else fprintf(f,id_field_format,"");
	break;
      case 'P': case 'p':
	(usable_peak-usable_saddle).print(f);
	break;
      case 'U': case 'u':
	usable_peak.print(f);
	putc(' ',f);
	usable_saddle.print(f);
	break;
      case 'S': case 's':
	saddle.print(f);
	break;
      case 'O': case 'o':
	fprintf(f,"%s",om_string);
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Domain prominence format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}

#endif

void Domain::print_header(FILE *f)
{
  for (const char *fc=format; *fc; fc++) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	fprintf(f,id_field_format,id_peak_hdr);
	break;
      case 'K': case 'k': case 'F': case 'f':
	Feature::print_header(f);
	break;
      case 'P': case 'p':
	print_prom_header(f);
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Domain format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}

void Domain::print_prom_header(FILE *f)
{
  const char *fc;

  for (fc=first_prom_format?first_prom_format:prom_format; *fc; fc++) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'I': case 'i':
	fprintf(f,id_field_format,id_saddle_hdr);
	break;
      case 'P': case 'p':
	fprintf(f,Elev_intvl::field_format,prom_hdr);
	break;
      case 'U': case 'u':
	fprintf(f,Elev_intvl::field_format,usable_peak_hdr);
	putc(' ',f);
	fprintf(f,Elev_intvl::field_format,usable_saddle_hdr);
	break;
      case 'S': case 's':
	Feature::print_header(f);
	break;
      case 'O': case 'o':
	fprintf(f,"%s",offmap_hdr);
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Domain HP prominence format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}

void Domain::Neighbor_iter::dump(FILE *f)
{
  fprintf(f,"Center: ");
  center->peak.print(f);
  if (*this) {
    fprintf(f,"\nSaddle: ");
    saddle_owner()->saddle.print(f);
    fprintf(f,"\nNabor:  ");
    (**this)->peak.print(f);
  }
  else fprintf(f,"\nExpired");
  putc('\n',f);
}

#endif // def io_support


#if 0

EESnbrs::EESnbrs(const GridPoint& r1, const GridPoint& r2, const GridPoint& s)
  : sadl(s)
{
  if (r1<r2) {
    ro1=r1;
    ro2=r2;
  }
  else {
    ro1=r2;
    ro2=r1;
  }
}

void EESnbrs::read(FILE *f) throw(file_error)
{
  fread(ro1,f);
  fread(ro2,f);
  fread(sadl,f);
}

void EESnbrs::write(FILE *f) const throw(file_error)
{
  fwrite(ro1,f);
  fwrite(ro2,f);
  fwrite(sadl,f);
}

#endif


/* EXPLORER MEMBER FUNCTIONS */

Boundary_saddle::Boundary_saddle(Domain *root, Domain *front, Elev_endpt endpt) :
  saddle(&root->saddle_owner(front)->saddle),domain(front),
  bdy_elev(saddle->elev.*endpt)
{
}

Explorer::Explorer(Domain *start, Elev_endpt e) : endpt(e)
{
  start->xplr_sup.root_nbr=0;
  serve(start);
}

Explorer::Explorer(Domain *start, Domain *root, Elev_endpt e) : endpt(e)
{
  start->xplr_sup.root_nbr=root; // make the initial nbr iter avoid the root
  serve(start);
  start->xplr_sup.root_nbr=0;
}

void Explorer::reset(Domain *start)
{
  while (!front.empty()) front.pop();
  start->xplr_sup.root_nbr=0;
  serve(start);
}

void Explorer::reset(Domain *start, Domain *root)
{
  while (!front.empty()) front.pop();
  start->xplr_sup.root_nbr=root; // make the initial nbr iter avoid the root
  serve(start);
  start->xplr_sup.root_nbr=0;
}

void Explorer::use(Elev_intvl& on_use, Elev_intvl *off_use) const
{
  // Determine intervening saddle elvation
  const Domain &curnt=*front.top().get_domain(),&prev=*curnt.xplr_sup.root_nbr;
  const Elev_intvl &pdpLIS=prev.xplr_sup.dpar_LIS,&se=saddle().elev;
  Elev_intvl& cdpLIS=curnt.xplr_sup.dpar_LIS;
  // propagate the definite parent LIS away from the root
  elevate(cdpLIS,pdpLIS);
  // compute the path LIS, all the way from root to any possible parent
  // except for the current saddle
  Elev_intvl path_parnt_LIS=prev.xplr_sup.root_LIS;
  depress(path_parnt_LIS,curnt.xplr_sup.ppar_LIS);
  // compute the USE
  if (se<pdpLIS || se>path_parnt_LIS) on_use.empty();
  else {
    on_use.set(pdpLIS.low,path_parnt_LIS.high);
    on_use&=se;
  }
  // compute the off-map USE, if desired
  if (off_use) {
    // compute the path LIS, all the way from root to any runoff
    // except for the current saddle
    Elev_intvl path_edge_LIS=prev.xplr_sup.root_LIS;
    depress(path_edge_LIS,curnt.xplr_sup.edge_LIS);
    // compute the offmap USE
    if (cdpLIS>path_edge_LIS) off_use->empty();
    else {
      off_use->set(cdpLIS.low,path_edge_LIS.high);
      (*off_use)&=se;
    }
  }
}

void Explorer::operator++()
{
  Domain& top=(**this);
  front.pop();
  serve(&top);
}

void Explorer::stop_branch()
{
  front.pop();
}

void Explorer::serve(Domain *a)
{
  for (Domain::Neighbor_iter ni(*a,a->xplr_sup.root_nbr); ni; ++ni) {
    (*ni)->xplr_sup.root_nbr=a;
    front.push(Boundary_saddle(a,*ni,endpt));
  }
}


/* DIVIDE TREE MEMBER FUNCTIONS */

Divide_tree *Divide_tree::reading=0;
const Divide_tree *Divide_tree::writing=0;


Domain::Index Divide_tree::ndom_inuse() const
{
  Index count=0;
  for (const_rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    if ((*domi).peak.location)
      ++count;
  return count;
}

Domain::Index Divide_tree::nrunoff_inuse() const
{
  Index count=0;
  for (const_rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi).location)
      ++count;
  return count;
}

Domain::Index Divide_tree::nbsnsdl_inuse() const
{
  Index count=0;
  for (const_rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    if ((*bsi).location)
      ++count;
  return count;
}

static void div_drg_chk(const Elev_intvl& peak, const Elev_intvl& nbr,
			Domain::Index& ndiv, Domain::Index& ndrg)
{
  if (peak>=nbr) ++ndiv;
  else if (peak<=nbr) ++ndrg;
}

void Divide_tree::div_drg_count(Index& ndiv, Index &ndrg) const
{
  ndiv=ndrg=0;
  for (const_rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    if ((*domi).primary_nbr) {
      div_drg_chk((*domi).peak.elev,(*domi).saddle.elev,ndiv,ndrg);
      div_drg_chk((*domi).primary_nbr->peak.elev,(*domi).saddle.elev,ndiv,ndrg);
    }
  }
  for (const_rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
    if ((*roi).peak) {
      div_drg_chk((*roi).peak->peak.elev,(*roi).elev,ndiv,ndrg);
    }
  }
  for (const_rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi) {
    if ((*bsi).location) {
      div_drg_chk((*bsi).peak1->peak.elev,(*bsi).elev,ndiv,ndrg);
      div_drg_chk((*bsi).peak2->peak.elev,(*bsi).elev,ndiv,ndrg);
    }
  }
}

void Divide_tree::area(Rectangl& alloc, Rectangl& defined) const
{
  for (const_rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    if ((*domi).peak.location) {
      defined|=(*domi).peak.location;
      if ((*domi).primary_nbr) {
	defined|=(*domi).saddle.location;
      }
    }
  }
  for (const_rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi).location) {
      if ((*roi).peak) defined|=(*roi).location;
      else alloc|=(*roi).location;
    }
  for (const_rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    if ((*bsi).location)
      defined|=(*bsi).location;

  alloc|=defined;
}

void Divide_tree::get_pat_steps(Domain *dom, unsigned *steps, Domain **pat) const
{
  unsigned i;
  Domain *d,*p;
  for (i=0,d=dom; d->primary_nbr!=0 && steps[index(d)]==Domain::undef;
       ++i,d=d->primary_nbr);
  if (d->primary_nbr==0) {
    steps[index(d)]=0;
    pat[index(d)]=d;
  }
  p=pat[index(d)];
  i+=steps[index(d)];
  for (d=dom; steps[index(d)]==Domain::undef; d=d->primary_nbr,--i) {
    steps[index(d)]=i;
    pat[index(d)]=p;
  }
}

void Divide_tree::clear()
{
  doms.~vector();
  runoffs.~vector();
  bsnsdls.~vector();
}

void Divide_tree::set_neighbors()
{
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    if ((*domi).peak.location && (*domi).primary_nbr) {
      assert((*domi).primary_nbr>=&doms.front() && (*domi).primary_nbr<=&doms.back());
      (*domi).add_nbr((*domi).primary_nbr);
      (*domi).primary_nbr->add_nbr(&*domi);
    }
  }

  sort_nbrs();
}

void Divide_tree::set_runoffs()
{
  for (rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi).location && (*roi).peak)
      (*roi).peak->add_nbr(&*roi);
}

void Divide_tree::clear_runoffs()
{
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    if ((*domi).peak.location)
      (*domi).clear_runoffs();
}

void Divide_tree::set_bsnsdls()
{
  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    if ((*bsi).location) {
      (*bsi).peak1->add_nbr(&*bsi);
      (*bsi).peak2->add_nbr(&*bsi);
    }
}

void Divide_tree::clear_bsnsdls()
{
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    if ((*domi).peak.location)
      (*domi).clear_bsnsdls();
}

void Divide_tree::set_cycle_info()
{
  Index n=doms.size(),i;
  unsigned *pat_steps=new unsigned[n],i1,i2;
  Domain **pat=new Domain *[n],*p1,*p2;
  for (i=0; i<n; ++i) pat_steps[i]=Domain::undef;

  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    if ((*bsi).location) {
      p1=(*bsi).peak1;
      p2=(*bsi).peak2;
      get_pat_steps(p1,pat_steps,pat);
      get_pat_steps(p2,pat_steps,pat);
      i1=pat_steps[index(p1)];
      i2=pat_steps[index(p2)];
      if (i1>i2) p1=p1->ancestor(i1-i2);
      else p2=p2->ancestor(i2-i1);
      while (p1!=p2) {
	p1=p1->primary_nbr;
	p2=p2->primary_nbr;
      }
      (*bsi).common_ancestor=p1;
      (*bsi).set_cycle_length();
    }

  delete[] pat_steps;
  delete[] pat;
}

void Divide_tree::change_ca(Domain *from, Domain *to)
{
  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi) {
    if ((*bsi).location) {
      if ((*bsi).common_ancestor==from) {
	(*bsi).common_ancestor=to;
      }
      (*bsi).set_cycle_length();
    }
  }
}

Elevation Divide_tree::min_range() const
{
  Elevation range=elev_infty;
  for (const_rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    (*domi).peak.min_range(range);
    (*domi).saddle.min_range(range);
  }
  for (const_rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    (*roi).min_range(range);
  for (const_rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    (*bsi).min_range(range);

  return range/2;
}

Elevation Divide_tree::max_peak_prom(Index i, const Topo_info island_tree[]) const
{
  Elev_intvl pk_elev=doms[i].peak.elev;
  Index j;
  for (j=i; island_tree[j].parnt!=Domain::undef &&
		  !(pk_elev<doms[island_tree[j].parnt].peak.elev);
       j=island_tree[j].parnt);
  return island_tree[j].parnt==Domain::undef ? elev_infty :
    doms[i].peak.elev.high-doms[island_tree[j].sadl_dom].saddle.elev.low;
}

Elevation *Divide_tree::min_peak_prom(const Topo_info island_tree[]) const
{
  Index i,j,n=doms.size();
  Elevation *min_pk_prom=new Elevation[n],minp;
  for (i=0; i<n; ++i)
    if (doms[i].peak.location) {
      min_pk_prom[i]=island_tree[i].parnt==Domain::undef ?
	elev_infty : doms[i].peak.elev.low-doms[island_tree[i].sadl_dom].saddle.elev.high;
    }
  for (i=0; i<n; ++i) {
    if (!doms[i].peak.location) continue;
    j=island_tree[i].parnt;
    if (j!=Domain::undef && !(doms[i].peak.elev<doms[j].peak.elev)) {
      minp=doms[j].peak.elev.low-doms[island_tree[i].sadl_dom].saddle.elev.high;
      if (minp<min_pk_prom[j]) min_pk_prom[j]=minp;
    }
  }
  return min_pk_prom;
}

void Divide_tree::assoc_peak_prom(const Topo_info tree[],
				  Elevation peak_prom[]) const
{
  Domain::Index ndom=doms.size(),jp,kp,ipeak;
  for (ipeak=0; ipeak<ndom; ++ipeak) {
    if (!doms[ipeak].peak.location) continue;
    // this fancy-schmancy loop checks if the current peak rivals its parent,
    // based on the lowest possible peak elevation.
    for (jp=ipeak; kp=tree[jp].parnt,kp!=Domain::undef; jp=kp) {
      const Elev_intvl &je=doms[jp].peak.elev,&ke=doms[kp].peak.elev;
      if (ke.low>je.low || ke>je) break;
    }
    peak_prom[ipeak] = kp==Domain::undef ? elev_infty :
      doms[ipeak].peak.elev.low-doms[jp].saddle.elev.low;
  }
}

#if 0
Elevation Divide_tree::peak_omp(Index i, const Topo_info tree[],
				Elevation island_edge_LIS[]) const
{
  Elevation HIP=-elev_infty,LIS=elev_infty,omp=elev_infty,ompx;
  Index j=i,j1;
  while (!(doms[i].peak.elev<doms[j].peak.elev)) {
    if (doms[j].peak.elev.low>HIP) HIP=doms[j].peak.elev.low;
    if (island_edge_LIS[j]>-elev_infty) {
      ompx=HIP-(LIS<island_edge_LIS[j]?LIS:island_edge_LIS[j]);
      if (ompx<omp) omp=ompx;
    }
    j1=tree[j].parnt;
    if (j1==Domain::undef) break;
    LIS=doms[tree[j].sadl_dom].saddle.elev.high;
    j=j1;
  }
  if (omp<doms[i].prom.peak) return omp;
  return elev_infty;
}

Elevation *Divide_tree::offmap_pk_prom(const Topo_info tree[]) const
{
  Index i,j,n=doms.size(),nro=runoffs.size();
  Elevation *peak_omp=new Elevation[n],minp,se;
  for (i=0; i<n; ++i) peak_omp[i]=elev_infty;
  for (i=0; i<nro; ++i) {
    const Runoff& ro=runoffs[i];
    if (!ro.location || ro.peak==0) continue;
    // clear the runoff inversion
    se=ro.elev.high;
    for (j=index(ro.peak);
	 tree[j].parnt!=Domain::undef &&
	   se<doms[tree[j].sadl_dom].saddle.elev.low;
	 j=tree[j].parnt);
    // bump the offmap prom down to the level possible due to current runoff
    while (true) {
      minp=doms[j].peak.elev.low-se;
      if (minp<peak_omp[j]) peak_omp[j]=minp;
      if (tree[j].parnt==Domain::undef) break;
      se=doms[tree[j].sadl_dom].saddle.elev.high;
      j=tree[j].parnt;
    }
  }
  return peak_omp;
}
#endif


class Cmp_peak_index {
  const Domain *dom_base;
  Elev_endpt endpt;
public:
  Cmp_peak_index(const Domain *base, Elev_endpt e) :
    dom_base(base),endpt(e) {}
  bool operator()(Domain::Index a, Domain::Index b) const
    {return dom_base[a].peak.elev.*endpt>dom_base[b].peak.elev.*endpt;}
};

class Cmp_bs_index {
public:
  const Basin_saddle *bs_base;
  Cmp_bs_index(const Basin_saddle *base) : bs_base(base) {}
  bool operator()(Domain::Index a, Domain::Index b) const
    {return bs_base[a].elev.midpt()>bs_base[b].elev.midpt();}
};

static void path_reverse(Topo_info tree[],
			 Domain::Index istart, Domain::Index isadl)
{
  if (isadl!=istart) {
    // reverse the parent-child direction for the path between istart and isadl
    // istart and isadl are both peak indices
    Domain::Index a=tree[istart].parnt,b,so1=tree[istart].sadl_dom,so2;
    Domain::Index ip=istart;
    tree[istart].sadl_dom=tree[isadl].sadl_dom;
    while (ip!=isadl) {
      b=tree[a].parnt;
      tree[a].parnt=ip;
      so2=tree[a].sadl_dom;
      tree[a].sadl_dom=so1;
      ip=a;
      a=b;
      so1=so2;
    }
  }
}

void Divide_tree::get_line_tree(Topo_info tree[],
				const vector<Domain::Index>& sorted,
				Index rank[], Elev_endpt sadl_endpt,
				Elevation Domain::Sadl_prom::*type)
{
#ifdef timer
  clock_t clk=clock();
#endif
  Index i,n=doms.size(),nu=sorted.size(),j,iparnt,isadl,ip;
  Index iro;

  // rearrange the topo tree
  Elev_intvl LIS;
  for (i=0; i<nu; ++i) {
    j=sorted[i];
    const Domain& dom=doms[j];

    // find the first higher ancestor and LIS in between
    iparnt=j;
    LIS=elev_infty;
    doms[j].xplr_sup.root_nbr=0;
    iro=Domain::undef;
    do {
      Topo_info& pd=tree[iparnt];
      if (pd.parnt>=n) {
	// patriarch
	if (pd.parnt==Domain::undef) {
	  // no final runoff
	  isadl=iparnt;
	  iparnt=Domain::undef;
	}
	else {
	  // final runoff
	  iro=pd.parnt-n;
	  const Elev_intvl& re=runoffs[iro].elev;
	  if (re.*sadl_endpt<LIS.*sadl_endpt)
	    isadl=iparnt;
	  depress(LIS,re);
	}
	break;
      }
      doms[iparnt].xplr_sup.root_LIS=LIS;
      doms[iparnt].xplr_sup.dpar_LIS=-elev_infty;
      doms[pd.parnt].xplr_sup.root_nbr=&doms[iparnt];
      const Elev_intvl& se=doms[pd.sadl_dom].saddle.elev;
      if (se.*sadl_endpt<LIS.*sadl_endpt)
	isadl=iparnt;
      depress(LIS,se);
      iparnt=pd.parnt;
    } while (rank[iparnt]>i);
    /*	We assume that if sadl_endpt is HI_END,
	then we are interested in saddle prominence.
	If so, set the highest possible prom for all saddles that might be
	the lowest between original domain and parent domain */
    if (iparnt!=Domain::undef && sadl_endpt==HI_END) {
      doms[iparnt].xplr_sup.dpar_LIS=
	iro==Domain::undef ? elev_infty : runoffs[iro].elev;
      doms[iparnt].transmit_LIS(&Domain::Explorer_support::dpar_LIS);
      for (ip=j; ip!=iparnt; ip=tree[ip].parnt) {
	const Domain& sadl_dom=doms[tree[ip].sadl_dom];
	LIS=doms[ip].xplr_sup.root_LIS;
	depress(LIS,doms[tree[ip].parnt].xplr_sup.dpar_LIS);
	if (sadl_dom.saddle.elev>LIS ||
	    sadl_dom.prom.saddle.*type!=elev_undef) continue;
	sadl_dom.prom.saddle.*type=dom.peak.elev.high-sadl_dom.saddle.elev.low;
      }
    }
    path_reverse(tree,j,isadl);
    tree[j].parnt=iparnt;
  }

#ifdef timer
  TRACE("get_line_tree() took %d clock ticks\n",clock()-clk);
#endif
}

void Domain::clear_prom_inv() const
{
  const Domain *&dcp=(const Domain *&)parent.cell;
  if (!dcp) return;
  const Domain::Sadl_prom *csp=&parent.saddle->prom.saddle,*psp;
  while (dcp->parent.cell &&
	 (psp=&dcp->parent.saddle->prom.saddle,
	  csp->onmap	 > psp->onmap		&&
	  csp->cr_onmap  > psp->cr_onmap	&&
	  csp->offmap	 > psp->offmap		&&
	  csp->cr_offmap > psp->cr_offmap)) {
    dcp->clear_prom_inv();
    dcp=dcp->parent.cell;
  }
}

void Domain::clear_sadl_inv() const
{
  if (!parent.island) return;
  const Elev_intvl& se=parent.saddle->saddle.elev;
  while (parent.island->parent.island &&
	 se<parent.island->parent.saddle->saddle.elev) {
    parent.island->clear_sadl_inv();
    parent.island=parent.island->parent.island;
  }
}

Domain *Domain::parent_with_prom(Elevation prom)
{
  Domain *p;
  for (p=this;
       p->parent.cell && p->peak_topo_prom()<prom;
       p=p->parent.cell);
  return p;
}

Domain *Domain::prom_parent()
{
  return !parent.cell ? 0 : parent.cell->
    parent_with_prom(peak.elev.low-parent.saddle->saddle.elev.high);
}

Domain *Domain::get_parent(Topology topo, Elevation prom)
{
  switch (topo) {
  case TOPO_MS: return primary_nbr;
  case TOPO_LINE: return parent.line ? parent.line->parent_with_prom(prom) : 0;
  case TOPO_CELL: return prom_parent();
  case TOPO_ISLAND: return parent.island;
  }
  assert(0);
  return 0;
}

bool Domain::is_ancestor(Domain *d, Topology topo) const
{
  while (d) {
    if (d==this) return true;
    d=d->get_parent(topo);
  }
  return false;
}

// compute highest possible onmap prom for each saddle
void Divide_tree::set_high_sadl_prom(const vector<Domain::Index>& sorted,
				     Index rank[])
{
  Topo_info *tree=init_topo();
  get_line_tree(tree,sorted,rank,HI_END,&Domain::Sadl_prom::onmap);
  delete[] tree;
}

// compute offmap prom for each saddle
void Divide_tree::set_offmap_sadl_prom(const vector<Domain::Index>& sorted,
				       Index rank[])
{
  Topo_info *tree=init_topo();
  Index i,n=doms.size(),j,iparnt,isadl,ip,nro=runoffs.size();

  // rearrange the tree, starting with runoffs
  Elev_intvl LIS;
  for (i=0; i<nro; ++i) {
    const Runoff& ro=runoffs[i];
    if (ro.peak==0) continue;
    const Domain& dom=*ro.peak;
    j=index(&dom);

    // find path across tree and LIS in between
    iparnt=j;
    isadl=Domain::undef;
    LIS=ro.elev;
    while (true) {
      Topo_info& pd=tree[iparnt];
      if (pd.parnt>=n) break;
      const Elev_intvl& se=doms[pd.sadl_dom].saddle.elev;
      if (se.high<LIS.high)
	isadl=iparnt;
      depress(LIS,se);
      iparnt=pd.parnt;
    }
    if (tree[iparnt].parnt==Domain::undef) isadl=iparnt; // no final runoff
    else {
      // final runoff
      const Runoff& ro2=runoffs[tree[iparnt].parnt-n];
      assert(index(ro2.peak)==iparnt);
      const Elev_intvl& re=ro2.elev;
      if (re.high<LIS.high)
        isadl=iparnt;
      depress(LIS,re);
      // set the unlimited prom for all saddles that might be
      // the lowest between orginal domain and parent domain
      for (ip=j; ip!=iparnt; ip=tree[ip].parnt) {
	const Domain& sadl_dom=doms[tree[ip].sadl_dom];
	if (sadl_dom.saddle.elev>LIS ||
	    sadl_dom.prom.saddle.offmap!=elev_undef) continue;
	sadl_dom.prom.saddle.offmap=elev_infty;
      }
    }
    if (isadl!=Domain::undef) {
      path_reverse(tree,j,isadl);
      tree[j].parnt=n+index(ro);
    }
  }

  // continue to rearrange the tree on internal peaks
  get_line_tree(tree,sorted,rank,HI_END,&Domain::Sadl_prom::offmap);

  delete[] tree;
}

void Divide_tree::update_sadl_prom_info(bool use_ss)
{
#ifdef timer
  clock_t clk=clock();
#endif
  // prepare array to represent the peaks in sorted order
  vector<Index> sorted;
  Index i,n=doms.size(),nbs,*rank=new Index[n];
  for (i=0; i<n; ++i) {
    if (doms[i].peak.location)
      sorted.push_back(i);
    Domain::Sadl_prom& sp=doms[i].prom.saddle;
    sp.onmap=sp.offmap=elev_undef;
    doms[i].ridge.cycle_prom=doms[i].ridge.cycle_offmap=-elev_infty;
  }
  if (!sorted.empty()) {
	  sort(sorted.begin(), sorted.end(), Cmp_peak_index(&doms.front(), HI_END));
  }
  Index nu=sorted.size();
  for (i=0; i<nu; ++i) rank[sorted[i]]=i;

  // compute the on-map prominence of all saddles
  set_high_sadl_prom(sorted,rank);
  // compute the off-map prominence of all saddles
  set_offmap_sadl_prom(sorted,rank);

  // determine the cell tree
  Topo_info *line_tree=init_topo();
  get_line_tree(line_tree,sorted,rank,LO_END,&Domain::Sadl_prom::onmap);
  delete[] rank;
  // initialize hierarchy with line tree
  for (i=0; i<n; ++i) {
    const Domain& dom=doms[i];
    if (!dom.peak.location) continue;
    Domain::Index line_par=line_tree[i].parnt;
    if (line_par!=Domain::undef) {
      dom.parent.line=dom.parent.cell=dom.parent.island=&doms[line_par];
      dom.parent.saddle=&doms[line_tree[i].sadl_dom];
    }
    else dom.parent.line=dom.parent.cell=0;
  }
  delete[] line_tree;

  // set cell rotation prominences
  Saddle::use_status=use_ss;
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (!dom.peak.location || dom.primary_nbr==0) continue;
    Domain::Sadl_prom& sp=dom.prom.saddle;
    sp.cr_onmap=sp.onmap;
    sp.cr_offmap=sp.offmap;
  }
  nbs=n_bsnsdl();
  Topo_info *ipit=get_ipit();
  unsigned *anc_depth=new unsigned[n];
  for (i=0; i<n; ++i) anc_depth[i]=0;
  for (i=0; i<nbs; ++i) {
    Basin_saddle& bs=bsnsdls[i];
    if (!bs.location) continue;
    bs.set_prom(ipit,&doms.front(),anc_depth);
  }
  delete[] ipit;
  delete[] anc_depth;

  // clear prom inversions, set children, and initialize child line prom
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (dom.peak.location) {
      dom.clear_prom_inv();
      dom.parent.island=dom.parent.cell;
      if (dom.parent.line) {
	dom.parent.saddle->parent.child=&dom;
	dom.ridge.line_child=dom.ridge.cycle_line_child=dom.prom.peak;
      }
    }
  }
  for (i=0; i<nbs; ++i) {
    Basin_saddle& bs=bsnsdls[i];
    if (!bs.location) continue;
    if (bs.prom.onmap>bs.peak1->ridge.cycle_line_child)
      bs.peak1->ridge.cycle_line_child=bs.prom.onmap;
    if (bs.prom.onmap>bs.peak2->ridge.cycle_line_child)
      bs.peak2->ridge.cycle_line_child=bs.prom.onmap;
  }
  // clear saddle inversions and update child line prom
  Domain *line_dom;
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (!dom.peak.location) continue;
    dom.clear_sadl_inv();
    for (line_dom=dom.parent.line;
	 line_dom && dom.ridge.line_child>line_dom->ridge.line_child;
	 line_dom=line_dom->parent.line)
      line_dom->ridge.line_child=dom.ridge.line_child;
    for (line_dom=dom.parent.line;
	 line_dom && dom.ridge.cycle_line_child>line_dom->ridge.cycle_line_child;
	 line_dom=line_dom->parent.line)
      line_dom->ridge.cycle_line_child=dom.ridge.cycle_line_child;
  }

#ifdef timer
  TRACE("update_sadl_prom_info() took %d clock ticks\n",clock()-clk);
#endif
}

// set peak prominence for peak icon rendering (and other stuff)
void Divide_tree::update_peak_prom_info()
{
  Topo_info *tree=get_island_tree(LO_END);
  Index i,n=n_dom();
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (!dom.peak.location) continue;
    dom.prom.peak=max_peak_prom(i,tree);
    dom.prom.pk_omp=dom.offmap_pk_prom();
  }
  delete[] tree;
  update_runoff_prom_info();
}

const Elevation Domain::Sadl_prom::*sadl_prom_types[2][2] = {
  {&Domain::Sadl_prom::onmap,	&Domain::Sadl_prom::cr_onmap},
  {&Domain::Sadl_prom::offmap,	&Domain::Sadl_prom::cr_offmap}
};

struct Ridge_scratch {
  unsigned nnbr; // number of non-truncated neighbors
  Elevation peak_prom; // ridge prom of peak
  bool peak_offmap; // is peak on an offmap ridge
  Domain *patriarch;
};

static void set_BS_ridge_info_leg(Basin_saddle& bs, Domain *start)
{
  for (; start!=bs.common_ancestor; start=start->primary_nbr) {
    if (bs.can_rotate(start->saddle)) {
      if (start->ridge.prom>bs.prom.rr_onmap)
	bs.prom.rr_onmap=start->ridge.prom;
      if (start->ridge.offmap) bs.prom.rr_offmap=true;
    }
  }
}

static void set_cycle_ridge_info_leg(const Basin_saddle& bs, Domain *start)
{
  for (; start!=bs.common_ancestor; start=start->primary_nbr) {
    // bump the ridge prom up to the level of the basin saddle prom
    // with ridge rotation effects
    if (bs.prom.rr_onmap>start->ridge.rot_prom)
      start->ridge.rot_prom=bs.prom.rr_onmap;
    if (bs.prom.rr_offmap) start->ridge.rot_offmap=true;
    // if the saddle is possibly the cycle LIS, bump up the feature prominence of the saddle
    if (bs.can_rotate(start->saddle)) {
      if (bs.prom.rr_onmap>start->prom.saddle.rr_onmap)
	start->prom.saddle.rr_onmap=bs.prom.rr_onmap;
      if (bs.prom.rr_offmap) start->prom.saddle.rr_offmap=true;
    }
  }
}

// update ridge prominence
// peak prominence must already be up to date
void Divide_tree::update_ridge_info(bool use_ss)
{
#ifdef timer
  clock_t clk=clock();
#endif
  Saddle::use_status=use_ss;
  Index i,j,k,n=n_dom(),nbs=n_bsnsdl();

  // initialize scratch
  Ridge_scratch *scratch=new Ridge_scratch[n];
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    Ridge_scratch& dom_scr=scratch[i];
    if (dom.peak.location) {
      dom_scr.peak_prom=dom.prom.peak;
      if (dom_scr.peak_prom<0) dom_scr.peak_prom=0;
      dom_scr.peak_offmap=dom.runoffs.size()>0;
      dom_scr.nnbr=dom.neighbors.size();
      dom_scr.patriarch=0;
      dom.ridge.prom=elev_undef;
      dom.ridge.offmap=false;
    }
    else dom_scr.nnbr=0;
  }
  // truncate endpoints, one by one.
  Domain *dom_base,*so,*d,*pat;
  if (!doms.empty()) {
	  dom_base = &doms.front();
  }
  for (i=0; i<n; ++i) {
    j=i;
    while (scratch[j].nnbr==1 && scratch[j].peak_prom<elev_infty) {
      // we're at a non-HP endpoint.
      Domain& dom=doms[j];
      // find the "remaining" neighbor
      Domain::Neighbor_iter ni(dom);
      for (; ni; ++ni) {
	k=(*ni)-dom_base;
	assert(k>=0 && k<n);
	if (scratch[k].nnbr>0) break;
      }
      assert(ni);
      // transfer peak ridge prominence to ridge prominence.
      so=dom.saddle_owner(*ni);
      so->ridge.prom=scratch[j].peak_prom;
      if (so->ridge.prom>scratch[k].peak_prom)
	scratch[k].peak_prom=so->ridge.prom;
      // transfer offmap flag
      so->ridge.offmap=scratch[j].peak_offmap;
      if (so->ridge.offmap)
	scratch[k].peak_offmap=true;
      // lop off the endpoint
      scratch[j].nnbr=0;
      --scratch[k].nnbr;
      j=k;
    }
  }

  // now fill in the prominence of the ridges that connect the HP candidates.
  map<Domain *, Elevation> islands;
  bool new_pat;
  Elevation sadl_prom;
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (!dom.peak.location) continue;
    Ridge_scratch& dom_scr=scratch[i];
    // find the patriarch (island key) of this peak
    pat=0;
    new_pat=false;
    for (d=&dom; pat==0; d=d->primary_nbr) {
      j=d-dom_base;
      if (scratch[j].patriarch) pat=scratch[j].patriarch;
      else if (d->primary_nbr==0) {
	pat=d;
	new_pat=true;
      }
    }
    // set the patriarch of ALL peaks from here to the patriarch,
    // so we don't have to repeat the traversal effort again for those peaks.
    for (d=&dom; d && (j=d-dom_base,scratch[j].patriarch==0); d=d->primary_nbr) {
      scratch[j].patriarch=pat;
    }
    Elevation& popup_prom=islands[pat];
    if (dom_scr.peak_prom<elev_infty) {
      // bump the island popup prom up to the current peak
      if (new_pat) popup_prom=dom_scr.peak_prom;
      else if (dom_scr.peak_prom>popup_prom) popup_prom=dom_scr.peak_prom;
    }
    if (dom.ridge.prom==elev_undef && dom.primary_nbr) {
      // we're on a ridge between HP candidates
      sadl_prom=dom.extreme_sadl_prom(HI_END);
      if (sadl_prom>popup_prom) popup_prom=sadl_prom;
    }
  }
  // now we've found the popup prominence of each island.
  // Assign it to the ridges that connect HP candidates
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (dom.peak.location && dom.primary_nbr && dom.ridge.prom==elev_undef)
      dom.ridge.prom=islands[scratch[i].patriarch];
  }

  delete[] scratch;

  // compute basin rim prominence
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (!dom.peak.location || dom.primary_nbr==0) continue;
    dom.ridge.rot_prom=dom.ridge.prom;
    dom.ridge.rot_offmap=dom.ridge.offmap;
    dom.prom.saddle.rr_onmap=-elev_infty;
    dom.prom.saddle.rr_offmap=false;
  }
  for (i=0; i<nbs; ++i) {
    Basin_saddle& bs=bsnsdls[i];
    bs.prom.rr_onmap=-elev_infty;
    bs.prom.rr_offmap=false;
    set_BS_ridge_info_leg(bs,bs.peak1);
    set_BS_ridge_info_leg(bs,bs.peak2);
    if (bs.prom.rr_onmap>-elev_infty) {
      set_cycle_ridge_info_leg(bs,bs.peak1);
      set_cycle_ridge_info_leg(bs,bs.peak2);
    }
  }
#ifdef timer
  TRACE("update_ridge_info() took %d clock ticks\n",clock()-clk);
#endif
}


void Divide_tree::purge_domains(Elevation min_prom, bool keep_name, bool keep_edit,
				bool zp, bool keep_rr, bool keep_cr, Index **xlate)
{
  // peak and saddle prom info must be up to date
  Index ndom=n_dom(),idom;
  zero_prob=zp;
  if (xlate) {
    *xlate=new Index[ndom];
    for (idom=0; idom<ndom; ++idom)
      (*xlate)[idom]=idom;
  }
  bool cont=true;
  while (cont) {
    cont=false;
    for (idom=0; idom<ndom; ++idom) {
      Domain& domain=doms[idom];
      if (domain.peak.location &&
	  (!keep_name || !domain.peak.name) &&
	  (!keep_edit || !domain.peak.edited) &&
	  domain.prom.peak<min_prom) {
	if (domain.neighbors.empty()) {
	  // Peak has no saddle-peak neighbors.
	  // Remove it if it has no runoff or basin saddle neighbors.
	  if (domain.runoffs.empty() && domain.bsnsdls.empty()) {
	    domain.remove();
	    cont=true;
	  }
	}
	else {
	  // Peak has peak-saddle neighbors.
	  // Remove it if its highest saddle does not have enough prominence.
	  Domain::Neighbor_iter ni(domain);
	  assert(ni);
	  Domain *elim_saddle=*ni,*so;
	  for (++ni; ni; ++ni) {
	    if (ni.saddle_owner()->saddle.elev.low>
		domain.saddle_owner(elim_saddle)->saddle.elev.low) {
	      elim_saddle=*ni;
	    }
	  }
	  so=domain.saddle_owner(elim_saddle);
	  const Domain::Sadl_prom& sp=so->prom.saddle;
	  if ((!keep_name || !so->saddle.name) &&
	      (!keep_edit || !so->saddle.edited) &&
	      sp.*sadl_prom_types[0][keep_cr]<min_prom &&
	      sp.*sadl_prom_types[1][keep_cr]<min_prom &&
	      (!keep_rr || sp.rr_onmap<min_prom && !sp.rr_offmap)) {
	    domain.remove(elim_saddle);
	    if (xlate) {
	      (*xlate)[index(&domain)]=index(elim_saddle);
	    }
	    cont=true;
	  }
	} // if (!domain.neighbors.empty())

      } // if (domain.peak.location && !has_min_prom[idom])
    } // for (idom)
  } // while (cont)

  if (xlate) {
    // flush out any "chains" of multiple translation
    for (idom=0; idom<ndom; ++idom) {
      Index& dom=(*xlate)[idom];
      while (dom!=(*xlate)[dom])
	dom=(*xlate)[dom];
    }
  }

  set_cycle_info();
}

void Divide_tree::purge_bs(Elevation min_prom, bool keep_name, bool keep_edit,
			   bool zp, bool keep_rr, bool keep_cr)
{
  // saddle prom info must be up to date
  Index i,ndom=doms.size(),nbs=bsnsdls.size();
  zero_prob=zp;
  for (i=0; i<nbs; ++i) {
    Basin_saddle& bs=bsnsdls[i];
    if (bs.location && (!keep_name || !bs.name) && (!keep_edit || !bs.edited) &&
	bs.prom.*sadl_prom_types[0][keep_cr]<min_prom &&
	bs.prom.*sadl_prom_types[1][keep_cr]<min_prom &&
	(!keep_rr || bs.prom.rr_onmap<min_prom && !bs.prom.rr_offmap)) {
      bs.remove();
    }
  }
}

#if 0

void Divide_tree::set_error(Elevation defalt_range,
			    const FeatureFilter& filter, const Database& db)
{
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    if ((*domi).peak.location) {
      if (filter.test((*domi).peak,db)) (*domi).peak.set_range(defalt_range);
      if ((*domi).saddle && filter.test((*domi).saddle,db))
	(*domi).saddle.set_range(defalt_range);
    }
  }
  for (rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi).location && filter.test(*roi,db))
      (*roi).set_range(defalt_range);
  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    if ((*bsi).location && filter.test(*bsi,db))
      (*bsi).set_range(defalt_range);
}

#endif

void Divide_tree::xlate_dom(Domain *& d, Domain *new_dom,
			    const Index *xlate) const
{
  d=d ? new_dom+xlate[index(d)] : 0;
}

void Divide_tree::xlate_dom(const Domain *& d, const Domain *new_dom,
			    const Index *xlate) const
{
  d=d ? new_dom+xlate[index(d)] : 0;
}

void Divide_tree::xlate_address(const Domain *&xdom, const Domain *old_base) const
{
  if (xdom!=0) xdom=xdom-old_base+&doms.front();
}

void Divide_tree::merge(const Divide_tree& intree)
{
  Dom_iter nbri;
  ROiter roi;
  BSiter bsi;
  Index total_dom=doms.size()+intree.doms.size(),
    total_ro=runoffs.size()+intree.runoffs.size(),
    total_bs=bsnsdls.size()+intree.bsnsdls.size();
    //total_ees=ees_nbrs.size()+intree.ees_nbrs.size();

  Domain *old_dom_base=&doms.front();
  const Domain *in_dom_base=&intree.doms.front()-doms.size();
  Runoff *old_ro_base=&runoffs.front();
  const Runoff *in_ro_base=&intree.runoffs.front()-runoffs.size();
  Basin_saddle *old_bs_base=&bsnsdls.front();
  const Basin_saddle *in_bs_base=&intree.bsnsdls.front()-bsnsdls.size();
  doms.reserve(total_dom);
  runoffs.reserve(total_ro);
  bsnsdls.reserve(total_bs);
  //ees_nbrs.reserve(total_ees);
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    xlate_address((const Domain *&)(*domi).primary_nbr,old_dom_base);
    for (nbri=(*domi).neighbors.begin(); nbri!=(*domi).neighbors.end(); ++nbri)
      xlate_address((const Domain *&)*nbri,old_dom_base);
    for (roi=(*domi).runoffs.begin(); roi!=(*domi).runoffs.end(); ++roi)
      if (*roi) (*roi)=(*roi)-old_ro_base+&runoffs.front();
    for (bsi=(*domi).bsnsdls.begin(); bsi!=(*domi).bsnsdls.end(); ++bsi)
      if (*bsi) (*bsi)=(*bsi)-old_bs_base+&bsnsdls.front();
  }
  for (rROiter rroi=runoffs.begin(); rroi!=runoffs.end(); ++rroi) {
    xlate_address((const Domain *&)(*rroi).peak,old_dom_base);
  }
  for (rBSiter rbsi=bsnsdls.begin(); rbsi!=bsnsdls.end(); ++rbsi) {
    xlate_address((const Domain *&)(*rbsi).peak1,old_dom_base);
    xlate_address((const Domain *&)(*rbsi).peak2,old_dom_base);
    xlate_address((const Domain *&)(*rbsi).common_ancestor,old_dom_base);
  }

  for (const_rDom_iter cdomi=intree.doms.begin();
       cdomi!=intree.doms.end(); ++cdomi) {
    doms.push_back(*cdomi);
    Domain& dom=doms.back();
    xlate_address((const Domain *&)dom.primary_nbr,in_dom_base);
    for (nbri=dom.neighbors.begin(); nbri!=dom.neighbors.end(); ++nbri)
      xlate_address((const Domain *&)*nbri,in_dom_base);
    for (roi=dom.runoffs.begin(); roi!=dom.runoffs.end(); ++roi)
      if (*roi) (*roi)=(*roi)-in_ro_base+&runoffs.front();
    for (bsi=dom.bsnsdls.begin(); bsi!=dom.bsnsdls.end(); ++bsi)
      if (*bsi) (*bsi)=(*bsi)-in_bs_base+&bsnsdls.front();
  }
  for (const_rROiter croi=intree.runoffs.begin();
       croi!=intree.runoffs.end(); ++croi) {
    runoffs.push_back(*croi);
    xlate_address((const Domain *&)runoffs.back().peak,in_dom_base);
  }
  for (const_rBSiter cbsi=intree.bsnsdls.begin();
       cbsi!=intree.bsnsdls.end(); ++cbsi) {
    bsnsdls.push_back(*cbsi);
    xlate_address((const Domain *&)bsnsdls.back().peak1,in_dom_base);
    xlate_address((const Domain *&)bsnsdls.back().peak2,in_dom_base);
    xlate_address((const Domain *&)bsnsdls.back().common_ancestor,in_dom_base);
  }
  //for (vector<EESnbrs>::const_iterator esni=intree.ees_nbrs.begin();
       //esni!=intree.ees_nbrs.end(); ++esni)
    //ees_nbrs.push_back(*esni);

  compress_ro();
  sort(runoffs.begin(),runoffs.end());
  clear_runoffs();
  set_runoffs();
}

const Feature *Divide_tree::transform(float m, float b,
				      bool xform_peak, bool xform_sadl,
				      bool xform_ro, bool xform_bs,
				      bool xform_edit)
{
  float u,v;
  rDom_iter domi;
  rROiter roi;
  rBSiter bsi;
  if (m!=0) {
    // precalculate interval for which transformation will be within range
    u=(-elev_infty-b)/m;
    v=( elev_infty-b)/m;
    if (m<0) {
      float y=u;
      u=v;
      v=y;
    }
    // check if all features would transform to a value within range
    for (domi=doms.begin(); domi!=doms.end(); ++domi) {
      if (xform_peak && !(*domi).peak.check_limit(u,v,xform_edit))
	return &(*domi).peak;
      if (xform_sadl && !(*domi).saddle.check_limit(u,v,xform_edit))
	return &(*domi).saddle;
    }
    if (xform_ro)
      for (roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
	if (!(*roi).check_limit(u,v,xform_edit)) return &*roi;
    if (xform_bs)
      for (bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
	if (!(*bsi).check_limit(u,v,xform_edit)) return &*bsi;
  }
  else {
    if (b>=elev_infty || b<=-elev_infty) {
      return &doms.front().peak;
    }
  }
  // All features transform within range.  Now do the transformation for real.
  for (domi=doms.begin(); domi!=doms.end(); ++domi) {
    if (xform_peak) (*domi).peak.transform(m,b,xform_edit);
    if (xform_sadl) (*domi).saddle.transform(m,b,xform_edit);
  }
  if (xform_ro)
    for (roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
      (*roi).transform(m,b,xform_edit);
  if (xform_bs)
    for (bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
      (*bsi).transform(m,b,xform_edit);

  return 0;
}


class Cmp_dom_location {
public:
  Feature Domain::*type;
  Cmp_dom_location(Feature Domain::*t) : type(t) {}
  bool operator()(const Domain *a, const Domain *b)
    {return (a->*type).location<(b->*type).location;}
};

class Cmp_featr_location {
public:
  bool operator()(const Feature *a, const Feature *b)
    {return a->location<b->location;}
};

void Divide_tree::add_bs(const Basin_saddle& bs)
{
  expand_nbs(1);
  bsnsdls.push_back(bs);
  bs.peak1->add_nbr(&bsnsdls.back());
  bs.peak2->add_nbr(&bsnsdls.back());
}

Runoff *Divide_tree::add_ro(const Runoff& ro)
{
  expand_nro(1);
  rROiter roi;
  for (roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi).location && ro.location<=(*roi).location) break;
  runoffs.insert(roi,ro);
  ROiter ropi;
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    for (ropi=(*domi).runoffs.begin(); ropi!=(*domi).runoffs.end(); ++ropi)
      if (*ropi && *ropi>=&*roi) ++*ropi;
  ro.peak->add_nbr(&*roi);

  return &*roi;
}

// sort domain pointers by location
void Divide_tree::sort_doms(Feature Domain::*type, vector<Domain *>& sorted)
{
  sorted.clear();
  sorted.reserve(doms.size());
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    if (((*domi).*type).location) sorted.push_back(&*domi);
  sort(sorted.begin(),sorted.end(),Cmp_dom_location(type));
}

// sort basin saddle pointers by location
void Divide_tree::sort_bs(vector<Basin_saddle *>& sorted)
{
  sorted.clear();
  sorted.reserve(bsnsdls.size());
  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    if ((*bsi).location) sorted.push_back(&*bsi);
  sort(sorted.begin(),sorted.end(),Cmp_featr_location());
}

// prepare sorted list of runoff pointers by location
//   by compressing the already sorted (except for deletions) runoff list
void Divide_tree::sort_ro(vector<Runoff *>& sorted)
{
  sorted.clear();
  sorted.reserve(runoffs.size());
  for (rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi).location) sorted.push_back(&*roi);
}

// find an exact location in a sorted list of domains
Domain::Index Divide_tree::search_dom(const GridPoint& target, const vector<Domain *>& sorted_doms,
				      Feature Domain::*type)
{
  Index jdom,lim_lo=0,lim_hi=sorted_doms.size();
  while (lim_hi>lim_lo) {
    jdom=(lim_lo+lim_hi)/2;
    if (target<(sorted_doms[jdom]->*type).location)
      lim_hi=jdom;
    else if (target>(sorted_doms[jdom]->*type).location)
      lim_lo=jdom+1;
    else lim_lo=lim_hi=jdom; // we'll automatically fall out of the loop here
  }
  return lim_lo;
}

// find a range of domain-owned features in the tree.
// use binary search to get the midpoint, then expand outward
void Divide_tree::dom_range(const GridPoint& target, const vector<Domain *>& sorted_doms,
	Feature Domain::*type, Distance radius, Index& lim_lo, Index& lim_hi)
{
  lim_lo=lim_hi=search_dom(target,sorted_doms,type);

  while (lim_lo>0 &&
	 target.ever_radius((sorted_doms[lim_lo-1]->*type).location)<=radius) --lim_lo;
  while (lim_hi<sorted_doms.size() &&
	 target.ever_radius((sorted_doms[lim_hi]->*type).location)<=radius) ++lim_hi;
}

// find an exact location in a sorted list of basin saddles
Domain::Index Divide_tree::search_bs(const GridPoint& target,
				     const vector<Basin_saddle *>& sorted_bs)
{
  Index jbs,lim_lo=0,lim_hi=sorted_bs.size();
  while (lim_hi>lim_lo) {
    jbs=(lim_lo+lim_hi)/2;
    if (target<sorted_bs[jbs]->location)
      lim_hi=jbs;
    else if (target>sorted_bs[jbs]->location)
      lim_lo=jbs+1;
    else lim_lo=lim_hi=jbs; // we'll automatically fall out of the loop here
  }
  return lim_lo;
}

// find a range of basin saddles in the tree.
// use binary search to get the midpoint, then expand outward
void Divide_tree::bs_range(const GridPoint& target, const vector<Basin_saddle *>& sorted_bs,
			   Distance radius, Index& lim_lo, Index& lim_hi)
{
  lim_lo=lim_hi=search_bs(target,sorted_bs);

  while (lim_lo>0 &&
	 target.ever_radius(sorted_bs[lim_lo-1]->location)<=radius) --lim_lo;
  while (lim_hi<sorted_bs.size() &&
	 target.ever_radius(sorted_bs[lim_hi]->location)<=radius) ++lim_hi;
}

// find an exact location in a sorted list of runoffs
Domain::Index Divide_tree::search_ro(const GridPoint& target,
				     const vector<Runoff *>& sorted_ro)
{
  Index jro,lim_lo=0,lim_hi=sorted_ro.size();
  while (lim_hi>lim_lo) {
    jro=(lim_lo+lim_hi)/2;
    if (target<sorted_ro[jro]->location)
      lim_hi=jro;
    else if (target>sorted_ro[jro]->location)
      lim_lo=jro+1;
    else lim_lo=lim_hi=jro; // we'll automatically fall out of the loop here
  }
  return lim_lo;
}

// find a range of runoffs in the tree.
// use binary search to get the midpoint, then expand outward
void Divide_tree::runoff_range(const GridPoint& target, const vector<Runoff *>& sorted_ro,
			       Distance radius, Index& lim_lo, Index& lim_hi)
{
  lim_lo=lim_hi=search_ro(target,sorted_ro);

  while (lim_lo>0 &&
	 target.ever_radius(sorted_ro[lim_lo-1]->location)<=radius) --lim_lo;
  while (lim_hi<sorted_ro.size() &&
	 target.ever_radius(sorted_ro[lim_hi]->location)<=radius) ++lim_hi;
}

void undefine_elev_array(Elevation array[], Domain::Index n)
{
  if (array==0) return;
  for (Domain::Index i=0; i<n; ++i)
    array[i]=elev_undef;
}

void depress(Elev_intvl& a, const Elev_intvl& b)
{
  if (b.low<a.low) a.low=b.low;
  if (b.high<a.high) a.high=b.high;
}

void elevate(Elev_intvl& a, const Elev_intvl& b)
{
  if (b.low>a.low) a.low=b.low;
  if (b.high>a.high) a.high=b.high;
}


// prepare for extra basin saddles
void Divide_tree::expand_nbs(Index nextra_bs)
{
  Basin_saddle *old_bs_base=&bsnsdls.front();

  bsnsdls.reserve(bsnsdls.size()+nextra_bs);

  if (old_bs_base!=&bsnsdls.front()) {
    BSiter bsi;
    for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
      for (bsi=(*domi).bsnsdls.begin(); bsi!=(*domi).bsnsdls.end(); ++bsi)
	if (*bsi) (*bsi)=(*bsi)-old_bs_base+&bsnsdls.front();
  }
}

// prepare for extra runoffs
void Divide_tree::expand_nro(Index nextra_ro)
{
  Runoff *old_ro_base=&runoffs.front();

  runoffs.reserve(runoffs.size()+nextra_ro);

  if (old_ro_base!=&runoffs.front()) {
    ROiter roi;
    for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
      for (roi=(*domi).runoffs.begin(); roi!=(*domi).runoffs.end(); ++roi)
	if (*roi) (*roi)=(*roi)-old_ro_base+&runoffs.front();
  }
}

// splice a contiguous sequence of runoffs
// it is assumed that the basin saddle vector already has enough capacity
// to hold the new ones
bool Divide_tree::splice(rROiter& roi)
{
  bool status=false;
#ifdef _DEBUG
  writing=this;
#endif
  rROiter roj;
  for (roj=roi;
       roj!=runoffs.begin() && (*(roj-1)).location==(*roi).location;
       --roj);
  roi=roj;
  for (++roi; roi!=runoffs.end() && (*roi).location==(*roj).location; ++roi) {
    Basin_saddle bs;
    (*roj).splice(*roi,bs);
    status=true;
    if (bs && !bs.edge_effect) {
      bsnsdls.push_back(bs);
      bs.peak1->add_nbr(&bsnsdls.back());
      bs.peak2->add_nbr(&bsnsdls.back());
    }
  }
  return status;
}

// splice a contiguous sequence of runoffs
// it is assumed that the basin saddle vector already has enough capacity
// to hold the new ones
// NOTE(akirmse): Added this function to fix an improper use of a vector
// pointer as an iterator at promdoc.cpp:2651.
bool Divide_tree::splice(Runoff *ro)
{
	bool status = false;
#ifdef _DEBUG
	writing = this;
#endif
	rROiter roj;
	for (roj = runoffs.begin(); roj != runoffs.end(); ++roj) {
		if (roj->location == ro->location) {
			Basin_saddle bs;
			(*roj).splice(*ro, bs);
			status = true;
			if (bs && !bs.edge_effect) {
				bsnsdls.push_back(bs);
				bs.peak1->add_nbr(&bsnsdls.back());
				bs.peak2->add_nbr(&bsnsdls.back());
			}
		}
	}

	return status;
}


void Divide_tree::splice(Distance)
{
  Index max_new_bs=0;

  // count the # of possible runoff splices;
  // that's an upper bound on the # of new basin saddles
  rROiter roi;
  for (roi=runoffs.begin(); (roi+1)!=runoffs.end(); ++roi)
    if ((*roi).location==(*(roi+1)).location)
      ++max_new_bs;
  // allocate & fill space for new basin saddles
  expand_nbs(max_new_bs);

  for (roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
    if (!(*roi).location) continue;
    splice(roi);
    --roi;
  }

  set_cycle_info();
}

void Divide_tree::splice(Runoff *ro[], unsigned nro)
{
  expand_nbs(nro-1);
  for (unsigned iro=1; iro<nro; ++iro) {
    Basin_saddle bs;
    ro[0]->splice(*ro[iro],bs);
    if (bs && !bs.edge_effect) {
      bsnsdls.push_back(bs);
      bs.peak1->add_nbr(&bsnsdls.back());
      bs.peak2->add_nbr(&bsnsdls.back());
    }
  }
  set_cycle_info();
}

void Divide_tree::rotate()
{
  // determine which basin saddles might need to be rotated
  Index nbs=bsnsdls.size(),nrot=0,irot;
  Basin_saddle **rotated=new Basin_saddle *[nbs];
  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi) {
    if ((*bsi).location && (*bsi).cycle_min(1,HI_END)) {
      rotated[nrot]=&*bsi;
      ++nrot;
    }
  }
  // rotate only those that need it
  for (irot=0; irot<nrot; ++irot) {
    rotated[irot]->set_cycle_info();
    rotated[irot]->rotate();
  }
  delete[] rotated;
  set_cycle_info();
}

// set the status of all saddles to unknown
void Divide_tree::set_sadl_stat_unknown()
{
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    (*domi).saddle.status=Saddle::STAT_UNKNOWN;
  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    (*bsi).status=Saddle::STAT_UNKNOWN;
}

// bump up the status of all edited prominence saddles
void Divide_tree::set_sadl_stat(Saddle::Status new_stat)
{
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    if ((*domi).saddle.edited && new_stat>(*domi).saddle.status)
      (*domi).saddle.status=new_stat;
}


// find runoff tree for runoff rendering
void Divide_tree::update_runoff_prom_info()
{
  // first, find the prominence island tree
  Index i,j,n=doms.size();
  Topo_info *tree=get_island_tree(HI_END);
  for (i=0; i<n; ++i) {
    Domain& dom=doms[i];
    if (!dom.peak.location) continue;
    j=tree[i].parnt;
    if (j==Domain::undef) {
      dom.prom.island_parent=0;
      dom.prom.sadl_elev=-elev_infty;
    }
    else {
      dom.prom.island_parent=&doms[j];
      dom.prom.sadl_elev=doms[tree[i].sadl_dom].saddle.elev.high;
    }
  }
  // now, clear runoff inversions
  n=runoffs.size();
  for (i=0; i<n; ++i) {
    const Runoff& ro=runoffs[i];
    const Domain *&ip=ro.island;
    if (ro.peak) {
      for (ip=ro.peak;
	   ip->prom.island_parent &&
	     ro.elev.high<doms[tree[index(ip)].sadl_dom].saddle.elev.low;
	   ip=ip->prom.island_parent);
    }
    else ip=0;
    assert(ro.island==0 || index(ro.island)<doms.size());
  }
  delete[] tree;
}

void Divide_tree::uninvert_peak(Index idom, Elev_endpt endpt,
				Topo_info tree[]) const
{
  Elev_endpt farpt;
  Index ipar,igpar,isdom,ipsdom;
  const Elev_intvl& pk_elev=doms[idom].peak.elev;
  while (ipar=tree[idom].parnt,
	 ipar!=Domain::undef &&
	 (pk_elev.*endpt > doms[ipar].peak.elev.*endpt ||
	  pk_elev.*endpt == doms[ipar].peak.elev.*endpt &&
	  (farpt=OTHER_END(endpt),
	   pk_elev.*farpt > doms[ipar].peak.elev.*farpt))) {
    uninvert_peak(ipar,endpt,tree);
    igpar=tree[ipar].parnt;
    isdom=tree[idom].sadl_dom;
    ipsdom=tree[ipar].sadl_dom;
    if (igpar==Domain::undef ||
	doms[isdom].saddle.elev.*endpt >
	doms[ipsdom].saddle.elev.*endpt) {
      // child-side saddle is higher
      tree[ipar].parnt=idom;
      tree[ipar].sadl_dom=isdom;
      tree[idom].sadl_dom=ipsdom;
    }
    tree[idom].parnt=igpar;
  }
}

void Divide_tree::uninvert_peak_ip(Index idom, Topo_info tree[]) const
{
  Index ipar,igpar,isdom,ipsdom;
  const Elev_intvl& pk_elev=doms[idom].peak.elev;
  Elev_intvl path_LIS;
  Saddle::Status path_stat,sadl_stat;
  while (ipar=tree[idom].parnt,
	 ipar!=Domain::undef &&
	 (pk_elev.low > doms[ipar].peak.elev.low ||
	  pk_elev.low == doms[ipar].peak.elev.low &&
	  pk_elev.high > doms[ipar].peak.elev.high)) {
    // original peak is (still) inverted
    uninvert_peak_ip(ipar,tree);
    igpar=tree[ipar].parnt;
    isdom=tree[idom].sadl_dom;
    ipsdom=tree[ipar].sadl_dom;
    if (igpar!=Domain::undef &&
	doms[isdom].saddle.elev<doms[ipsdom].saddle.elev &&
	doms[isdom].saddle.eff_stat(false)<=doms[ipsdom].saddle.eff_stat(false)) {
      // parent-side saddle is definitely higher.
      // clear the inversion by making the grandparent the parent.
      tree[idom].parnt=igpar;
      continue;
    }
    for (path_LIS=doms[isdom].saddle.elev,
	   path_stat=doms[isdom].saddle.eff_stat(false);
	 igpar!=Domain::undef &&
	   (!(doms[ipsdom].saddle.elev<path_LIS) ||
	    (sadl_stat=doms[ipsdom].saddle.eff_stat(false))>path_stat); ) {
      // next ancestor saddle is not definitely lower. Is the next peak higher?
      const Elev_intvl& par_elev=doms[igpar].peak.elev;
      if (par_elev.low > pk_elev.low ||
	  par_elev.low == pk_elev.low && par_elev.high > pk_elev.high) {
	// yes. We'll consider ourselves not inverted, and can bail.
	return;
      }
      // no. shift to the next saddle and continue.
      ipar=igpar;
      isdom=ipsdom;
      igpar=tree[ipar].parnt;
      ipsdom=tree[ipar].sadl_dom;
      depress(path_LIS,doms[isdom].saddle.elev);
      if (sadl_stat<path_stat) path_stat=sadl_stat;
    }
    // if we get here, we've found a definitely lower saddle
    // before finding a higher peak. Reverse the path.
    path_reverse(tree,idom,ipar);
    tree[idom].parnt=igpar;
  }
}

void Divide_tree::uninvert_peaks(Elev_endpt endpt, Topo_info tree[]) const
{
#ifdef timer
  clock_t clk=clock();
#endif
  Index i,n=doms.size();
  for (i=0; i<n; ++i)
    if (doms[i].peak.location)
      uninvert_peak(i,endpt,tree);
#ifdef timer
  TRACE("uninvert_peaks() took %d clock ticks\n",clock()-clk);
#endif
}

void Divide_tree::uninvert_saddle(Index idom, Elev_endpt endpt,
				  Topo_info tree[]) const
{
  Index ipar,igpar,isdom=tree[idom].sadl_dom,ipsdom;
  while (true) {
    ipar=tree[idom].parnt;
    if (ipar==Domain::undef) return;
    igpar=tree[ipar].parnt;
    if (igpar==Domain::undef) return;
    ipsdom=tree[ipar].sadl_dom;
    if (doms[isdom].saddle.elev.*endpt >
	doms[ipsdom].saddle.elev.*endpt) return;
    uninvert_saddle(ipar,endpt,tree);
    tree[idom].parnt=tree[ipar].parnt;
  }
}

void Divide_tree::uninvert_saddle_ip(Index idom, Topo_info tree[]) const
{
  Index ipar,igpar,isdom=tree[idom].sadl_dom,ipsdom;
  if (doms[isdom].saddle.edge_effect) return;
  Saddle::Status sadl_stat=doms[isdom].saddle.eff_stat(false);
  while (true) {
    ipar=tree[idom].parnt;
    if (ipar==Domain::undef) return;
    igpar=tree[ipar].parnt;
    if (igpar==Domain::undef) return;
    ipsdom=tree[ipar].sadl_dom;
    if (!(doms[isdom].saddle.elev < doms[ipsdom].saddle.elev) ||
	sadl_stat>doms[ipsdom].saddle.eff_stat(false))
      return;
    uninvert_saddle_ip(ipar,tree);
    tree[idom].parnt=tree[ipar].parnt;
  }
}

void Divide_tree::uninvert_saddles(Elev_endpt endpt, Topo_info tree[]) const
{
#ifdef timer
  clock_t clk=clock();
#endif
  Index i,n=doms.size();
  for (i=0; i<n; ++i)
    if (doms[i].peak.location)
      uninvert_saddle(i,endpt,tree);
#ifdef timer
  TRACE("uninvert_saddles() took %d clock ticks\n",clock()-clk);
#endif
}

Topo_info *Divide_tree::init_topo() const
{
  // initialize island info array to reflect M-S tree
  Index i,n=doms.size();
  Topo_info *tree=new Topo_info[n];
  for (i=0; i<n; ++i) {
    const Domain& dom=doms[i];
    if (!dom.peak.location) continue;
    tree[i].parnt=index(dom.primary_nbr);
    tree[i].sadl_dom=i;
  }

  return tree;
}

Topo_info *Divide_tree::get_island_tree(Elev_endpt endpt) const
{
  Topo_info *tree=init_topo();
  uninvert_peaks(endpt,tree);
  uninvert_saddles(endpt,tree);
  return tree;
}

Topo_info *Divide_tree::get_ipit() const
{
#ifdef timer
  clock_t clk=clock();
#endif
  Topo_info *tree=init_topo();

  Index i,n=doms.size();
  for (i=0; i<n; ++i)
    if (doms[i].peak.location)
      uninvert_peak_ip(i,tree);
  for (i=0; i<n; ++i)
    if (doms[i].peak.location)
      uninvert_saddle_ip(i,tree);
#ifdef timer
  TRACE("get_ipit() took %d clock ticks\n",clock()-clk);
#endif

  return tree;
}

void Divide_tree::sort_nbrs()
{
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    (*domi).sort_nbrs();
}

static inline void xlate_dom(const Domain *& d, const Domain *dom_base,
			     const Domain::Index *xlate)
{
  assert(d==0 || xlate[d-dom_base]!=Domain::undef);
  d = d ? dom_base+xlate[d-dom_base] : 0;
}

static inline void xlate_bs(const Basin_saddle *& bs, const Basin_saddle *bs_base,
			    const Domain::Index *xlate)
{
  assert(bs==0 || xlate[bs-bs_base]!=Domain::undef);
  bs = bs ? bs_base+xlate[bs-bs_base] : 0;
}

void Divide_tree::xlate_all_doms(Index xlate[], Index new_ndom)
{
  // initialize new list and transfer peaks to it
  Domain *dom_base=&doms.front();
  vector<Domain> old_doms=doms;
  doms.resize(new_ndom);
  Index i,xi;
  Dom_iter nbri;

  for (i=0; i<old_doms.size(); ++i) {
    xi=xlate[i];
    if (xi!=Domain::undef) {
      doms[xi]=old_doms[i];
      doms[xi].copy_nbrs(old_doms[i]);
      ::xlate_dom((const Domain *&)doms[xi].primary_nbr,dom_base,xlate);
      // translate neighbors
      for (nbri=doms[xi].neighbors.begin();
	   nbri!=doms[xi].neighbors.end(); ++nbri)
	     ::xlate_dom((const Domain *&)*nbri,dom_base,xlate);
    }
  }
  // translate runoffs
  for (rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    if ((*roi).location && (*roi).peak)
      ::xlate_dom((const Domain *&)(*roi).peak,dom_base,xlate);
  // translate basin saddles
  for (rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi) {
    if ((*bsi).location) {
      ::xlate_dom((const Domain *&)(*bsi).peak1,dom_base,xlate);
      ::xlate_dom((const Domain *&)(*bsi).peak2,dom_base,xlate);
      ::xlate_dom((const Domain *&)(*bsi).common_ancestor,dom_base,xlate);
    }
  }

}

void Divide_tree::xlate_all_bs(Index xlate[], Index new_nbs)
{
  // initialize new list and transfer basin saddles to it (wothout translation)
  Basin_saddle *bs_base=&bsnsdls.front();
  vector<Basin_saddle> old_bs=bsnsdls;
  bsnsdls.resize(new_nbs);
  Index i,xi;
  BSiter nbri;

  // translate basin saddles from "old" list back to new list
  for (i=0; i<old_bs.size(); ++i) {
    xi=xlate[i];
    if (xi!=Domain::undef) {
      bsnsdls[xi]=old_bs[i];
    }
  }
  // translate references from peak neighbor lists
  for (i=0; i<doms.size(); ++i) {
    for (nbri=doms[i].bsnsdls.begin(); nbri!=doms[i].bsnsdls.end(); ++nbri) {
      ::xlate_bs((const Basin_saddle *&)*nbri,bs_base,xlate);
    }
  }
}

void Divide_tree::compress_dom()
{
  Index ndom=doms.size(),*xlate=new Index[ndom],i,new_ndom=0;
  // prepare translation table
  for (i=0; i<ndom; ++i) {
    if (doms[i].peak.location) {
      xlate[i]=new_ndom;
      ++new_ndom;
    }
    else xlate[i]=Domain::undef;
  }
  if (new_ndom==ndom) {
    delete[] xlate;
    return; // do nothing if there are no open spaces
  }

  xlate_all_doms(xlate,new_ndom);

  delete[] xlate;
}

void Divide_tree::compress_ro()
{
  if (runoffs.empty()) return;
  Index nrunoff=runoffs.size(),*xlate=new Index[nrunoff],i,new_nro=0;
  // prepare translation table
  for (i=0; i<nrunoff; ++i) {
    if (runoffs[i].location) {
      xlate[i]=new_nro;
      ++new_nro;
    }
    else xlate[i]=Domain::undef;
  }
  if (new_nro==nrunoff) {
    delete[] xlate;
    return; // do nothing if there are no open spaces
  }

  // initialize new list and transfer runoffs to it
  Runoff *runoff_base=&runoffs.front();
  vector<Runoff> old_runoffs=runoffs;
  runoffs.resize(new_nro);
  for (i=0; i<nrunoff; ++i) {
    if (xlate[i]!=Domain::undef) {
      runoffs[xlate[i]]=old_runoffs[i];
    }
  }
  // translate runoff lists on domains
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    if ((*domi).peak.location) {
      for (ROiter roi=(*domi).runoffs.begin();
	   roi!=(*domi).runoffs.end(); ++roi)
	(*roi)=runoff_base+xlate[(*roi)-runoff_base];
    }
  }

  delete[] xlate;
}

void Divide_tree::compress_bs()
{
  if (bsnsdls.empty()) return;
  Index nbs=bsnsdls.size(),*xlate=new Index[nbs],i,new_nbs=0;
  // prepare translation table
  for (i=0; i<nbs; ++i) {
    if (bsnsdls[i].location) {
      xlate[i]=new_nbs;
      ++new_nbs;
    }
    else xlate[i]=Domain::undef;
  }
  if (new_nbs==nbs) {
    delete[] xlate;
    return; // do nothing if there are no open spaces
  }

  // initialize new list and transfer basin saddles to it
  Basin_saddle *bs_base=&bsnsdls.front();
  vector<Basin_saddle> old_bs=bsnsdls;
  bsnsdls.resize(new_nbs);
  Basin_saddle *new_bs_base=&bsnsdls.front();
  for (i=0; i<nbs; ++i) {
    if (xlate[i]!=Domain::undef) {
      bsnsdls[xlate[i]]=old_bs[i];
    }
  }
  // translate basin saddle lists on domains
  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    if ((*domi).peak.location) {
      for (BSiter bsi=(*domi).bsnsdls.begin();
	   bsi!=(*domi).bsnsdls.end(); ++bsi) {
	(*bsi)=new_bs_base+xlate[(*bsi)-bs_base];
      }
    }
  }

  delete[] xlate;
}

void Divide_tree::sort_doms()
{
  // blanks left over from deleted peaks will interfere with sorting
  compress_dom();
  // prepare array to represent the peaks in sorted order
  vector<Index> sorted;
  Index i,n=doms.size();
  for (i=0; i<n; ++i) sorted.push_back(i);
  sort(sorted.begin(),sorted.end(),Cmp_peak_index(&doms.front(),LO_END));
  // invert the "sorted order" array to form a translation array
  Index *xlate=new Index[n];
  for (i=0; i<n; ++i)
    xlate[sorted[i]]=i;
  // reorder the peaks according to the translation
  xlate_all_doms(xlate,n);
  delete[] xlate;
}

void Divide_tree::sort_bs()
{
  // blanks left over from deleted basin saddles will interfere with sorting
  compress_bs();
  // prepare array to represent the basin saddles in sorted order
  vector<Index> sorted;
  Index i,n=bsnsdls.size();
  for (i=0; i<n; ++i) sorted.push_back(i);
  sort(sorted.begin(),sorted.end(),Cmp_bs_index(&bsnsdls.front()));
  // invert the "sorted order" array to form a translation array
  Index *xlate=new Index[n];
  for (i=0; i<n; ++i)
    xlate[sorted[i]]=i;
  // reorder the basin saddles according to the translation
  xlate_all_bs(xlate,n);
  delete[] xlate;
}

#ifdef io_support

void Divide_tree::dump(FILE *f) const
{
  writing=this;
  fprintf(f,"         ");
  Feature::print_header(f);
  putc('\n',f);
  for (const_rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    fprintf(f,"Domain #%ld:\n",index(&*domi));
    (*domi).dump(f);
  }
  if (!runoffs.empty()) fprintf(f,"Runoffs:\n");
  for (const_rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi) {
    fprintf(f,"  #%-5ld ",index(*roi));
    (*roi).dump(f);
  }
  if (!bsnsdls.empty()) fprintf(f,"Basin saddles:\n");
  for (const_rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi) {
    fprintf(f,"  #%-5ld ",index(*bsi));
    (*bsi).dump(f);
  }
}

bool Divide_tree::dump(const char *fn) const
{
  FILE *file=fopen(fn,"w");
  if (!file) return false;
  dump(file);
  fclose(file);
  return true;
}

#endif // io_support

void Divide_tree::xlate_address(const Domain *&xdom, const Divide_tree& source) const
{
  if (xdom!=0) xdom=source.index(xdom)+&doms.front();
}

void Divide_tree::copy(const Divide_tree& source) throw(bad_alloc)
{
  Dom_iter nbri;
  ROiter roi;
  BSiter bsi;
  reading=this;

  doms.clear();
  doms=source.doms;
  runoffs=source.runoffs;
  bsnsdls=source.bsnsdls;
  Runoff *ro_base=&*runoffs.begin();
  Basin_saddle *bs_base=&*bsnsdls.begin();

  for (rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi) {
    xlate_address((const Domain *&)(*domi).primary_nbr,source);
    for (nbri=(*domi).neighbors.begin(); nbri!=(*domi).neighbors.end(); ++nbri)
      xlate_address((const Domain *&)*nbri,source);
    for (roi=(*domi).runoffs.begin(); roi!=(*domi).runoffs.end(); ++roi)
      if (*roi) (*roi)=source.index(**roi)+ro_base;
    for (bsi=(*domi).bsnsdls.begin(); bsi!=(*domi).bsnsdls.end(); ++bsi)
      if (*bsi) (*bsi)=source.index(**bsi)+bs_base;
  }

  for (rROiter rroi=runoffs.begin(); rroi!=runoffs.end(); ++rroi)
    xlate_address((const Domain *&)(*rroi).peak,source);

  for (rBSiter rbsi=bsnsdls.begin(); rbsi!=bsnsdls.end(); ++rbsi) {
    xlate_address((const Domain *&)(*rbsi).peak1,source);
    xlate_address((const Domain *&)(*rbsi).peak2,source);
    xlate_address((const Domain *&)(*rbsi).common_ancestor,source);
  }

}

void Divide_tree::read(FILE *f) throw(file_error,bad_alloc)
{
  unsigned short version;
  fread(version,f);
  Saddle::distance_io=false;
  Saddle::stat_io=0;
  switch (version) {
  case VERSION_SADL_STAT_RIVER:
    Saddle::stat_io=2;
    Saddle::distance_io=true;
    break;
  case VERSION_SADL_STAT:
    Saddle::stat_io=1;
    Saddle::distance_io=true;
    break;
  case VERSION_FS:
    Saddle::distance_io=true;
    break;
  default: fseek(f, -(int) sizeof version,SEEK_CUR); // we don't have a file version stamp
  }
  GridPoint::read_stamp(f);
  reading=this;

  Index i,n;
  fread(n,f);
  doms.clear();
  doms.resize(n); // !!! sometimes crashes if not preceeded by AfxMessageBox()
  for (i=0; i<n; ++i)
    doms[i].read(f);

  fread(n,f);
  runoffs.clear();
  runoffs.resize(n);
  for (i=0; i<n; ++i)
    runoffs[i].read(f);

  fread(n,f);
  bsnsdls.clear();
  bsnsdls.resize(n);
  for (i=0; i<n; ++i)
    bsnsdls[i].read(f);

#if 0
  // won't use this until I have a better understanding of "boundary hugging walks"
  try {
    fread(n,f); // need to handle old file format which doesn't have this
  }
  catch (...) {
    if (!feof(f)) throw;
    n=0;
  }
  ees_nbrs.clear();
  ees_nbrs.resize(n);
  for (i=0; i<n; ++i)
    ees_nbrs[i].read(f);
#endif
}

void Divide_tree::write(FILE *f) const throw(file_error)
{
  fwrite(VERSION_SADL_STAT_RIVER,f);
  GridPoint::write_stamp(f);
  writing=this;

  fwrite(doms.size(),f);
  for (const_rDom_iter domi=doms.begin(); domi!=doms.end(); ++domi)
    (*domi).write(f);

  fwrite(runoffs.size(),f);
  for (const_rROiter roi=runoffs.begin(); roi!=runoffs.end(); ++roi)
    (*roi).write(f);

  fwrite(bsnsdls.size(),f);
  for (const_rBSiter bsi=bsnsdls.begin(); bsi!=bsnsdls.end(); ++bsi)
    (*bsi).write(f);

#if 0
  // won't use this until I have a better understanding of "boundary hugging walks"
  fwrite(ees_nbrs.size(),f);
  for (vector<EESnbrs>::const_iterator esni=ees_nbrs.begin(); esni!=ees_nbrs.end(); ++esni)
    (*esni).write(f);
#endif
}

void Divide_tree::read(const char *fn) throw(file_error,bad_alloc)
{
  FILE *file=fopen(fn,"rb");
  if (!file) throw file_error();
  try {read(file);}
  catch (...) {
    fclose(file);
    throw;
  }
  fclose(file);
  set_neighbors();
  set_runoffs();
  set_bsnsdls();
  set_cycle_info();
}

void Divide_tree::write(const char *fn) const throw(file_error)
{
  FILE *file=fopen(fn,"wb");
  if (!file) throw file_error();
  try {write(file);}
  catch (...) {
    fclose(file);
    throw;
  }
  fclose(file);
}

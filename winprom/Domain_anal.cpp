// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#include "stdafx.h"

#include "domain_anal.h"
#include <float.h>
#include <set>
#include <algorithm>
using namespace std;

//#define trace_anal
//static FILE *trace_file=0;

void undefine_flat(Elev_map& elev, const Rectangl& rect_lmt,
		   const Elev_intvl& elev_lmt)
{
  assert(((Rectangl)elev).contains(rect_lmt));
  Rect_iter_all gi(rect_lmt);
  Neighbor_iter ni(elev,*gi);
  for (; gi; ++gi) {
    Elevation& elv=elev[*gi];
    if (!elev_lmt.contains(elv)) continue;
    for (ni.reset(*gi); ni; ++ni)
      if (elev[*ni]!=elev_undef && elev[*ni]!=elv) break;
    if (!ni) elv=elev_undef;
  }
}

static bool equate_nbr(Elev_map &elev, const GridPoint& gp,
		       const Elev_intvl& elev_lmt)
{
  if (elev[gp]!=elev_undef) return false;
  Elevation prev_nbr_elev=elev_undef,nbr_elev;
  for (Neighbor_iter ni(elev,gp); ni; ++ni) {
    nbr_elev=elev[*ni];
    if (!elev_lmt.contains(nbr_elev)) return false;
    if (prev_nbr_elev==elev_undef)
      prev_nbr_elev=nbr_elev;
    else if (nbr_elev!=prev_nbr_elev) return false;
  }
  if (prev_nbr_elev==elev_undef) return false;
  elev[gp]=prev_nbr_elev;
  return true;
}

void define_flat(Elev_map& elev, const Rectangl& rect_lmt,
		 const Elev_intvl& elev_lmt)
{
  assert(((Rectangl)elev).contains(rect_lmt));
  Rect_iter_all gi(rect_lmt);
  bool try_more;
  while (true) {
    try_more=false;
    for (gi.reset(); gi; ++gi) {
      if (equate_nbr(elev,*gi,elev_lmt)) try_more=true;
    }
    if (!try_more) break;
    try_more=false;
    for (gi.endset(); gi; --gi) {
      if (equate_nbr(elev,*gi,elev_lmt)) try_more=true;
    }
    if (!try_more) break;
  }
}


#include "region.h"

// Descriptor of a flat region
class Flat {
protected:
  const Elev_map& elev;
  Elevation target;
public:
  Flat(const Elev_map& e, Elevation t) : elev(e),target(t) {}
  bool path(const Neighbor_iter&) const {return true;}
  bool region(const GridPoint& gp) const {return elev[gp]==target;}
  bool step(const Neighbor_iter& ni) const {return elev[*ni]==target;}
  //void visit(const GridPoint&) const {}
  void clear() const {}
};

// Descriptor for flat region that marks visited grid points
class Flat_mark : public Flat {
protected:
  Bool_map& marker;
public:
  bool visit_mark;
  Flat_mark(const Elev_map& e, Elevation t, Bool_map& m)
    : Flat(e,t),marker(m),visit_mark(true) {}
  bool visited(const GridPoint& gp) const {return marker[gp]==visit_mark;}
  bool step(const Neighbor_iter& ni) const
    {return elev[*ni]==target && marker[*ni]!=visit_mark;}
  void visit(const GridPoint& gp) const {marker[gp]=visit_mark;}
};

struct WalkDist {
  GridPoint location;
  Distance dist;
  Domain::Index index;
  WalkDist() {}
  WalkDist(const GridPoint& loc, Domain::Index pk, Distance d) :
    location(loc),dist(d),index(pk) {}
  WalkDist(const WalkDist& wd, const GridPoint& step) :
    location(step),index(wd.index),
    dist((wd.dist>0?wd.dist:0)+(step-wd.location)) {}
  // operator<() is defined opposite from what's expected
  // so that WalkDist's will go through a priority queue in the correct order
  bool operator<(const WalkDist& a) const {return dist>a.dist;}
  void trace() const;
};

void WalkDist::trace() const
{
  location.trace();
  TRACE(" %.3g %d",dist,index);
}

GridPoint steepest_nbr(const GridPoint& gp, const Elev_map& elev,
		       const Grid& grid)
{
  float slope,steep_slope=0;
  GridPoint steep_nbr;
  Elevation gp_elev=elev[gp];
  for (Neighbor_iter ni(grid,gp); ni; ++ni) {
    if (elev[*ni]>gp_elev) {
      slope=(elev[*ni]-gp_elev)/ni.dist();
      if (slope>steep_slope) {
	steep_slope=slope;
	steep_nbr=(*ni);
      }
    }
  }
  return steep_nbr;
}

static void walkup_peak(GridPoint start, const Grid& grid, const Elev_map& elev,
			Index_map& domap, Bool_map& mark)
{
  vector<GridPoint> path;
  GridPoint next;
  Elevation start_elev;
  Domain::Index peak;
  while (peak=domap[start],peak==Domain::undef) {
    start_elev=elev[start];
    // find steepest grid neighbor
    next=steepest_nbr(start,elev,grid);
    if (!next) {
      // no higher grid neighbor? Search the entire flat region.
      priority_queue<WalkDist> walk_dist;
      Flat_mark fm(elev,start_elev,mark);
      // initialize walk_dist priority queue from rising boundaries
      for (Region_iter<Flat_mark,Grid> ri(start,fm,grid); ri; ++ri) {
	Domain::Index& flat_peak=domap[*ri];
	next=steepest_nbr(*ri,elev,grid);
	if (next) {
	  walkup_peak(next,grid,elev,domap,mark);
	  assert(flat_peak==Domain::undef || flat_peak==domap[next]);
	  flat_peak=domap[next];
	  assert(flat_peak!=Domain::undef);
	  walk_dist.push(WalkDist(*ri,flat_peak,
			 (elev[*ri]-elev[next])/(*ri-next)));
	}
      }
      // expand queue over entire flat region
      Neighbor_iter ni(grid,start);
      while (!walk_dist.empty()) {
	const WalkDist top=walk_dist.top();
	const GridPoint& location=top.location;
	if (mark[location]) {
	  for (ni.reset(location); ni; ++ni) {
	    if (mark[*ni] && elev[*ni]==start_elev) {
	      walk_dist.push(WalkDist(top,*ni));
	    }
	  }
	  mark[location]=false;
	  domap[location]=top.index;
	}
	walk_dist.pop();
      }

      break;
    }
    path.push_back(start);
    start=next;
  }
  peak=domap[start];
  for (vector<GridPoint>::iterator gpi=path.begin(); gpi!=path.end(); ++gpi) {
    domap[*gpi]=peak;
  }
}

static Distance flat_dist(const GridPoint& start, const Grid& grid,
			  const Elev_map& elev, Bool_map& mark)
{
  Elevation start_elev=elev[start];
  // otherwise return the negative of the upslope
  GridPoint stp_nbr=steepest_nbr(start,elev,grid);
  if (stp_nbr) return (start_elev-elev[stp_nbr])/(start-stp_nbr);
  priority_queue<WalkDist> walk_dist;
  walk_dist.push(WalkDist(start,0,0)); // initialize the priority queue; we don't need WalkDist.index
  Neighbor_iter ni(grid,start);
  while (!walk_dist.empty()) {
    const WalkDist top=walk_dist.top();
    const GridPoint& location=top.location;
    if (!mark[location]) {
      mark[location]=true;
      for (ni.reset(location); ni; ++ni) {
	if (mark[*ni]) continue;
	if (elev[*ni]==start_elev) walk_dist.push(WalkDist(top,*ni));
	else if (elev[*ni]>start_elev) {
	  // clear the marks
	  Flat_mark fm(elev,start_elev,mark);
	  fm.visit_mark=false;
	  for (Region_iter<Flat_mark,Grid> ri(start,fm,grid); ri; ++ri);
	  assert(top.dist>0); // if it's 0, we should have returned before!
	  // return the distance
	  return top.dist;
	}
      }
    }
    walk_dist.pop();
  }
  return 0; // no higher neighbor was found
}

#ifdef administrator

// Descriptor for flat region that marks an index map with a designated label.
class Flat_index : public Flat_mark {
  Index_map& index_map;
public:
  Domain::Index index;
  vector<GridPoint> *gp_list;
  Flat_index(const Elev_map& e, Elevation t, Bool_map& m, Index_map& d)
    : Flat_mark(e,t,m),index_map(d),gp_list(0) {}
  inline void visit(const GridPoint& gp) const;
};

void Flat_index::visit(const GridPoint& gp) const
{
  Flat_mark::visit(gp);
  index_map[gp]=index;
  if (gp_list) gp_list->push_back(gp);
}

// Descriptor for region that stays above a target elevation,
// requires marks, and clears them.
class High_clear {
protected:
  const Elev_map& elev;
  Elevation target;
  Bool_map& marker;
public:
  High_clear(const Elev_map& e, Elevation t, Bool_map& m)
    : elev(e),target(t),marker(m) {}
  bool path(const Neighbor_iter&) const {return true;}
  bool region(const GridPoint& gp) const {return elev[gp]>target;}
  bool visited(const GridPoint& gp) const {return !marker[gp];}
  bool step(const Neighbor_iter& ni) const
    {return elev[*ni]>target && marker[*ni];}
  void visit(const GridPoint& gp) const {marker[gp]=false;}
  void clear() const {}
};

struct BdyWalkDist : WalkDist {
  GridPoint start_point;
  BdyWalkDist(const Neighbor_iter& ni, Domain::Index ihbr, Distance d) :
    WalkDist(*ni,ihbr,d),start_point(ni.center()) {}
  BdyWalkDist(const BdyWalkDist& bwd, const GridPoint& step) :
    WalkDist(bwd,step),start_point(bwd.start_point) {}
  void trace() const;
};

void BdyWalkDist::trace() const
{
  WalkDist::trace();
  TRACE("  start from ");
  start_point.trace();
}

struct Saddle_cxn {
  GridPoint cxn_pt,bdy_pt1,bdy_pt2;
  Distance dist;
  Saddle_cxn(const GridPoint& c, const GridPoint& bp1, const GridPoint& bp2,
	     Distance d=0) : cxn_pt(c),bdy_pt1(bp1),bdy_pt2(bp2),dist(d) {}
};


struct BdyRecord {
  GridPoint interior,bdy;
  Distance path_dist,pop_dist;
  BdyRecord(const GridPoint& i, const GridPoint& b, Distance ad, Distance od) :
    interior(i),bdy(b),path_dist(ad),pop_dist(od*(1+FLT_EPSILON)) {}
  bool operator==(const GridPoint& a) {return interior==a;}
  void print(FILE *f=stdout) const;
  void trace() const;
};

void BdyRecord::print(FILE *f) const
{
  interior.print(f);
  fprintf(f," ");
  bdy.print(f);
  fprintf(f," %.3g",pop_dist);
}

void BdyRecord::trace() const
{
  interior.trace();
  TRACE(" ");
  bdy.trace();
  TRACE(" %.3g",pop_dist);
}

template <class K>
class searchable_queue : public queue<BdyRecord> {
public:
  typedef deque<BdyRecord>::iterator iterator;
  typedef deque<BdyRecord>::const_iterator const_iterator;
  iterator find(const K& val)
    {return std::find(c.begin(),c.end(),val);}
  const_iterator find(const K& val) const
    {return std::find(c.begin(),c.end(),val);}
  BdyRecord& top() {return front();}
  const_iterator end() const {return c.end();}
};

static Domain::Index nonpk_indx=Domain::undef-1;

#define DOMAP_SADL_BDY 0x80000000

// Do a complete walkup analysis
void Divide_tree_map::walkup_analyze(const Elev_map& elev, Index_map& domap,
				     void (*line_cb)(float), void (*peak_cb)(),
				     void (*sadl_cb)(), void (*walkup_cb)())
{
  #ifdef trace_anal
  Domain::format="Domain descent %i %K\n";
  Elev_intvl::precise_format="%d";
  Divide_tree::writing=this;
  trace_file=fopen("trace.txt","w");
  #endif
  unsigned long points_analyzed=0,lines_analyzed=0,ihb;
  double area_analyzed=0;
  Rectangl bound(elev);
  const Grid grid(elev);
  Bool_map mark((Rectangl)elev);
  mark.fill(false);
  domap.set_bound(elev);
  domap.fill(Domain::undef);
  clear();
  Elevation elv;
  GridPoint best_nbr;
  vector<GridPoint> high_bdy_pt,peak_gps;
  vector< vector<GridPoint> > high_bdys;
  vector< vector<GridPoint> >::iterator hbi;
  vector<GridPoint>::iterator gpi,gpi2;
  searchable_queue<GridPoint>::iterator bdy_pt;
  vector<Domain::Index> circuit;
  vector<Saddle_cxn> saddles;
  bool nonpeak,first_high,prev_high;
  Distance quant,best_quant,rd,s1,s2;
  float lines_i=1.0f/(((Rectangl)elev).limit[0].size()+1);
  unsigned nnp,inp,circ_from,circ_to;

  unsigned nsp=0,nsh=0,nss=0,nmp=0,nmh=0,nms=0;

  RawGrid_iter gi(elev);
  for (; gi; ++gi) { // loop over all grid points
    if (line_cb && gi.eol()) {
      ++lines_analyzed;
      (*line_cb)(lines_analyzed*lines_i);
    }
    Elevation target_elev=elev[*gi];
    // ignore points with undefined elevation
    if (target_elev==elev_undef) continue;
    ++points_analyzed;
    area_analyzed+=(*gi).area();
    // ignore points already explored
    if (domap[*gi]!=Domain::undef) continue;
    nonpeak=false;

    RawNbr_iter rni(*gi);
    first_high=bound.contains(*rni) && elev[*rni]>target_elev;
    prev_high=false;
    assert(high_bdy_pt.empty());
    for (; rni; ++rni) { // loop over neighbors
      elv=bound.contains(*rni)?elev[*rni]:elev_undef;
      if (elv==elev_undef || elv<target_elev) {
	// lower or undefined neighbor
	if (prev_high && !rni.over_triangl()) {
	  // end of high neighbor segment. Add to high boundary point list.
	  high_bdy_pt.push_back(best_nbr);
	  prev_high=false;
	}
      }
      else if (elv>target_elev) {
	// higher neighbor
	nonpeak=true;
	quant=(elv-target_elev)/rni.dist();
	if (!prev_high || quant>best_quant) {
	  // steeper neighbor continuing rising segment.
	  best_nbr=(*rni);
	  best_quant=quant;
	}
	prev_high=true;
      }
      else break; // we've got a multiple GP flat area, darnit. Bail.
    }
    if (!rni) {
      // single GP flat area.
      if (nonpeak) {
	if (prev_high) {
	  // ended high. What to do with last high bdy area?
	  if (!first_high || high_bdy_pt.empty()) // started low
	    high_bdy_pt.push_back(best_nbr);
	  else { // started high. determine whether to use starting or ending part.
	    if (best_quant>(elev[high_bdy_pt.front()]-target_elev)/
			    (high_bdy_pt.front()-(*gi)))
	      high_bdy_pt.front()=best_nbr;
	  }
	}
	nnp=high_bdy_pt.size();
	if (nnp==2) {
	  ++nss;
	  // current region is multiplicity 2 saddle.
	  // Connect the high points.
	  s1=(target_elev-elev[high_bdy_pt.front()])/(*gi-high_bdy_pt.front());
	  s2=(target_elev-elev[high_bdy_pt.back()])/(*gi-high_bdy_pt.back());
	  saddles.push_back(Saddle_cxn(*gi,high_bdy_pt.front(),high_bdy_pt.back(),
				       s1>s2?s1:s2));
	  if (sadl_cb) (*sadl_cb)();
	}
	else if (nnp>2) {
	  ++nss;
	  // our current GP has saddle multiplicity greater than 2.
	  // therefore it's a saddle.
	  /* Determine which high boundary is the steepest step from the
	     original point. That one will be connected to all others. */
	  best_quant=0;
	  for (gpi=high_bdy_pt.begin(); gpi!=high_bdy_pt.end(); ++gpi) {
	    quant=(elev[*gpi]-target_elev)/(*gpi-*gi);
	    if (quant>best_quant) {
	      best_quant=quant;
	      best_nbr=*gpi;
	    }
	  }
	  for (gpi=high_bdy_pt.begin(); gpi!=high_bdy_pt.end(); ++gpi) {
	    if (*gpi==best_nbr) continue;
	    saddles.push_back(Saddle_cxn(*gi,*gpi,best_nbr,
					 (target_elev-elev[*gpi])/(*gpi-*gi)));
	    if (sadl_cb) (*sadl_cb)();
	  }
	}
	else ++nsh;
      }
      else {
	++nsp;
	// it's a peak. add it to the list of peaks.
	domap[*gi]=doms.size();
	doms.push_back(Topo_point(*gi,target_elev));
	if (peak_cb) (*peak_cb)();
      }
      high_bdy_pt.clear();
      continue; // no need to use the full power of the flat area algorithm
    }
    high_bdy_pt.clear();

    // multiple grid point flat region
    Flat_mark fm(elev,target_elev,mark);
    for (Boundary_iter<Flat_mark,Grid> bi1(*gi,fm,grid); bi1; ++bi1) {
      // loop over boundary points higher than the original flat region
      if (elev[*bi1]>target_elev) {
	mark[*bi1]=true; // mark it
	nonpeak=true; // region is not a peak
      }
    }
    Flat_index fd(elev,target_elev,mark,domap);
    fd.visit_mark=false;
    // If we've got a peak, mark each point of its flat region with its index.
    // otherwise mark it as a non-peak.
    if (nonpeak) fd.index=nonpk_indx;
    else {
      fd.index=doms.size();
      fd.gp_list=&peak_gps;
    }

    for (Boundary_iter<Flat_index,Grid> bi2(*gi,fd,grid); bi2; ++bi2) {
      // loop over still-marked boundary points higher than the original flat region
      if (mark[*bi2] && elev[*bi2]>target_elev) {
	high_bdys.push_back(vector<GridPoint>());
	vector<GridPoint>& cur_high_bdy=high_bdys.back();
	for (Region_iter<High_clear,Grid>
	     ri(*bi2,High_clear(elev,target_elev,mark),grid);
	     ri; ++ri) { // loop over contiguous set of higher boundary points
	  // the region iterator ri automatically clears the boundary point marks
	  cur_high_bdy.push_back(*ri); // add boundary point to list
	}
      }
    }
    if (nonpeak) {
      nnp=high_bdys.size();
      if (nnp>=2) {
	++nms;
	// our current region has saddle multiplicity at least 2.
	// therefore it's a saddle. Determine where we will walk up from.
#ifdef trace_anal
	fprintf(trace_file,"New saddle @ ");
	(*gi).print(trace_file);
	fprintf(trace_file," %d  mult=%d\n",elev[*gi],nnp);
	fflush(trace_file);
#endif
	int nsa=nnp-1;
	// remark the flat area in preparation for distance walkers
	for (Region_iter<Flat_mark,Grid> ri(*gi,fm,grid); ri; ++ri);

	// initialize circuit numbers
	circuit.resize(nnp);
	for (inp=0; inp<nnp; ++inp)
	  circuit[inp]=inp;
	// initialize the walkers, one high boundary area at a time
	priority_queue<BdyWalkDist> walkers;
	vector< searchable_queue<GridPoint> > start_pts;
	start_pts.resize(nnp);
	Neighbor_iter ni(grid,*gi);
	for (hbi=high_bdys.begin(); hbi!=high_bdys.end(); ++hbi) {
	  // loop over high boundary areas
	  ihb=hbi-high_bdys.begin();
	  for (gpi=(*hbi).begin(); gpi!=(*hbi).end(); ++gpi) {
	    // loop over grid points within high boundary area
	    for (ni.reset(*gpi); ni; ++ni) {
	      if (mark[*ni]) {
		BdyWalkDist bwd(ni,ihb,(elev[*ni]-elev[*gpi])/ni.dist());
		walkers.push(bwd);
	      }
	    }
	  }
	}

	while (!walkers.empty()) {
	  const BdyWalkDist top=walkers.top();
	  const GridPoint& location=top.location;
	  assert(domap[location]>=DOMAP_SADL_BDY || domap[location]==nonpk_indx);
	  assert(mark[location]==(domap[location]==nonpk_indx));
	  if (mark[location]) {
	    // no path to this point has already been served. Serve it now.
	    best_quant=0;
	    for (ni.reset(location); ni; ++ni) {
	      BdyWalkDist bwd(top,*ni);
	      assert(mark[*ni]==(domap[*ni]==nonpk_indx && elev[*ni]==target_elev));
	      if (mark[*ni]) {
		walkers.push(bwd);
	      }
	      if (bwd.dist>best_quant) best_quant=bwd.dist;
	    }
	    mark[location]=false;
	    domap[location]=top.index+DOMAP_SADL_BDY;
	    start_pts[top.index].
	      push(BdyRecord(location,top.start_point,top.dist,best_quant));
	  }
	  else if (circuit[top.index]!=circuit[domap[location]-DOMAP_SADL_BDY]) {
	    // collision!
	    // check secondary queue to find other walkup start point
	    assert(domap[location]>=DOMAP_SADL_BDY &&
		   domap[location]<DOMAP_SADL_BDY+nnp);
	    searchable_queue<GridPoint>& bdy_pts=
	      start_pts[domap[location]-DOMAP_SADL_BDY];
	    assert(!bdy_pts.empty());
	    while (bdy_pts.top().pop_dist<top.dist) {
	      const BdyRecord& top1=bdy_pts.top();
	      bdy_pts.pop();
	      assert(!bdy_pts.empty());
	    }
	    bdy_pt=bdy_pts.find(location);
	    assert(bdy_pts.end()!=bdy_pt); // damn well better find it
	    if (top.dist>=0 && (*bdy_pt).path_dist>=0)
	      rd=top.dist+(*bdy_pt).path_dist;
	    else if (top.dist>(*bdy_pt).path_dist) rd=top.dist;
	    else rd=(*bdy_pt).path_dist;
	    saddles.push_back(Saddle_cxn(location,top.start_point,(*bdy_pt).bdy,rd));
#ifdef trace_anal
	    fprintf(trace_file,"  Walker collision @ ");
	    location.print(trace_file);
	    fprintf(trace_file," from ");
	    saddles.back().bdy_pt1.print(trace_file);
	    fprintf(trace_file," & ");
	    saddles.back().bdy_pt2.print(trace_file);
	    fprintf(trace_file," %.2f\n",saddles.back().dist);
	    fflush(trace_file);
#endif
	    if (sadl_cb) (*sadl_cb)();
	    --nsa;
	    // merge circuits of connected boundary areas
	    circ_from=circuit[top.index];
	    circ_to=circuit[domap[location]-DOMAP_SADL_BDY];
	    for (inp=0; inp<nnp; ++inp) {
	      if (circuit[inp]==circ_from)
		circuit[inp]=circ_to;
	    }
	  }
	  walkers.pop();
	}
	assert(nsa==0);

      }
      else ++nmh;
    }
    else {
      // it's a peak. add it to the list of peaks.
      ++nmp;
      // This is a multiple-GP peak. Find the "median"
      // (the GP that minimizes the max distance from itself to any other)
      // and use that as the peak location.
      best_quant=GridPoint_cart::huje;
      for (gpi=peak_gps.begin(); gpi!=peak_gps.end(); ++gpi) {
	quant=0;
	for (gpi2=peak_gps.begin(); gpi2!=peak_gps.end(); ++gpi2) {
	  rd=(*gpi).rel_dist(*gpi2);
	  if (rd>quant) quant=rd;
	}
	if (quant<best_quant ||
	    quant==best_quant && (*gpi).priority()<best_nbr.priority()) {
	  best_quant=quant;
	  best_nbr=(*gpi);
	}
      }
      doms.push_back(Topo_point(best_nbr,target_elev));
      peak_gps.clear();
      if (peak_cb) (*peak_cb)();
    }
    high_bdys.clear();
  }
  TRACE("     P      H      S\n");
  TRACE("S %6d %6d %6d\n",nsp,nsh,nss);
  TRACE("M %6d %6d %6d\n",nmp,nmh,nms);

  // translate nonpeak indices
  for (gi.reset(); gi; ++gi)
    if (domap[*gi]>=DOMAP_SADL_BDY || domap[*gi]==nonpk_indx)
      domap[*gi]=Domain::undef;

  // Connect peaks by walking up from saddles
  Domain *dom1,*dom2;
  Basin_saddle basin_saddle;
  for (vector<Saddle_cxn>::iterator sadli=saddles.begin();
       sadli!=saddles.end(); ++sadli) {
#ifdef trace_anal
    fprintf(trace_file,"Connections from saddle %d @ ",sadli-saddles.begin());
    (*sadli).cxn_pt.print(trace_file);
    fprintf(trace_file," %d\n",elev[(*sadli).cxn_pt]);
    fflush(trace_file);
#endif
    // connect 'em up, dude
    walkup_peak((*sadli).bdy_pt1,grid,elev,domap,mark);
    walkup_peak((*sadli).bdy_pt2,grid,elev,domap,mark);
    dom1=&doms[domap[(*sadli).bdy_pt1]];
    dom2=&doms[domap[(*sadli).bdy_pt2]];
    if (dom1!=dom2) {
      Domain::connect(Saddle((*sadli).cxn_pt,elev[(*sadli).cxn_pt],(*sadli).dist),
		      dom1,dom2,basin_saddle,false,false);
      if (basin_saddle) bsnsdls.push_back(basin_saddle);
    }

    if (walkup_cb) (*walkup_cb)();
  }

#ifdef trace_anal
  fclose(trace_file);
  trace_file=0;
#endif

  search_edge(elev,domap,mark);

  sort_doms();
  sort_bs();
  Domain::Index n_domain=doms.size();
  resolve_domap(domap,n_domain);
}

static void edge_saddle(Divide_tree& tree, Elevation edge_min_elev,
			Domain *dom1, Domain *dom2, Saddle *&PEES)
{
  if (dom1==dom2) return;
  Domain *so=dom1->saddle_owner(dom2);
  Saddle *new_PEES=PEES;
  if (so) {
    // we have a connecting saddle
    if (so->saddle.elev.high==edge_min_elev) {
      // connecting saddle is same height as edge min.
      // Therefore it is a possible edge effect.
      new_PEES=&so->saddle;
    }
    else assert(so->saddle.elev.high>edge_min_elev);
  }
  if (new_PEES==PEES) {
    // no suitable connecting saddle. Search basin saddles.
    for (Domain::Index i=0; i<tree.n_bsnsdl(); ++i) {
      Basin_saddle& bs=tree.bsnsdl(i);
      if ((bs.peak1==dom1 && bs.peak2==dom2 ||
	   bs.peak1==dom2 && bs.peak2==dom1) &&
	  bs.elev.high==edge_min_elev) {
	// found one that connects the right domains at the right elevation.
	// Therefore it may be an edge effect.
	new_PEES=&bs;
	break;
      }
    }
  }
  if (new_PEES!=PEES) {
    // PEES has changed
    if (PEES==0 || new_PEES->flat_sep>PEES->flat_sep)
      PEES=new_PEES;
  }
}

void Divide_tree_map::search_edge(const Elev_map& elev,
				  Index_map& domap, Bool_map& mark)
{
  // Search for edge maxima. Accumulate them in a stack.
  Grid grid(elev);
  RawEdgeIter ei(Rectangl(elev),true),midpt(elev);
  runoffs.clear();
  Saddle *PEES; // possible edge effect saddle

  Elevation previous_elev,current_elev;
  Domain *ro_dom,*previous_dom,*current_dom;
  bool climbed=false,descended=false,step_state,startup=true;
  int start_parity;
  for (; ei; ++ei) {
    current_elev=elev[*ei];
    if (current_elev!=elev_undef) {
      walkup_peak(*ei,grid,elev,domap,mark);
      current_dom=&doms[domap[*ei]];
    }
    else current_dom=0;
    if (!startup) {
      if (current_elev!=elev_undef &&
	  (previous_elev==elev_undef || current_elev>previous_elev)) {
	// step up. Start a new possible runoff stretch.
	climbed=true;
	step_state=start_parity=(*ei).parity()!=0;
	midpt=ei;
      }
      else if (climbed) {
	if (current_elev==elev_undef || current_elev<previous_elev) {
	  // step down after step up. Record runoff.
	  ro_dom=&doms[domap[*midpt]];
	  runoffs.push_back(Runoff(Topo_point(*midpt,previous_elev),ro_dom,2));
	  if (ro_dom->peak.elev==previous_elev) {
	    ro_dom->peak.edge_effect=runoffs.back().edge_effect=true;
	    runoffs.back().flat_sep=0;
	  }
	  else {
	    runoffs.back().flat_sep=flat_dist(*midpt,grid,elev,mark);
	  }
	  climbed=false;
	}
	else if (current_elev==previous_elev) {
	  // level after step up. Update running midpoint as necessary.
	  if (step_state) ++midpt;
	  if ((*ei).parity()!=start_parity) step_state=!step_state;
	}
      }
      if (current_elev!=elev_undef && previous_elev!=elev_undef) {
	if (current_elev<previous_elev) {
	  // step down. May be the beginning of an edge effect saddle.
	  PEES=0;
	  edge_saddle(*this,current_elev,previous_dom,current_dom,PEES);
	  descended=true;
	}
	else if (descended) {
	  if (current_elev>previous_elev) {
	    // step up after step down. May be the end of an edge effect saddle.
	    edge_saddle(*this,previous_elev,previous_dom,current_dom,PEES);
	    if (PEES!=0) {
	      PEES->edge_effect=true;
	      PEES=0;
	    }
	    descended=false;
	  }
	  else {
	    // Flat after step down. May be an edge effect saddle.
	    edge_saddle(*this,current_elev,previous_dom,current_dom,PEES);
	  }
	}
      }
      else descended=false;
      if ((*ei).quad_corner()) {
	// corner point. Clear the record.
	runoffs.push_back(Runoff(Topo_point(*ei,current_elev),current_dom,
				 Rectangl(elev).on_corner(*ei)?1:2));
	if (current_elev==elev_undef)
	  runoffs.back().elev.empty();
	if (current_dom) {
	  if (current_dom->peak.elev==current_elev) {
	    current_dom->peak.edge_effect=runoffs.back().edge_effect=true;
	    runoffs.back().flat_sep=0;
	  }
	  else {
	    runoffs.back().flat_sep=flat_dist(*ei,grid,elev,mark);
	  }
	}
	climbed=false;
      }
    }
    else startup=false;
    previous_elev=current_elev;
    previous_dom=current_dom;
  }

  sort(runoffs.begin(),runoffs.end());
}

#ifdef io_support

static void shore_feature(Feature& featr, const Elev_map& elev,
			  const Database& data, const FeatureFilter& filter,
			  Elevation shore_elev, bool set_edit, bool mark_edit)
{
  if (!featr.location || !filter.test(featr,data) ||
      (featr.edited && !set_edit)) return;

  for (RawNbr_iter ni(featr.location); ni; ++ni) {
    if (elev.contains(*ni) && elev[*ni]==elev_undef) {
      featr.elev=shore_elev;
      if (mark_edit) featr.edited=true;
      return;
    }
  }
}

void Divide_tree_map::shoreline_correction(const Elev_map& elev,
	const FeatureTypeFilter& filter, const Database& data,
	Elevation basin_elev, Elevation sadl_elev,
	bool set_edit, bool mark_edit)
{
  Domain::Index i,n;

  n=doms.size();
  for (i=0; i<n; ++i) {
    if (filter.test(FT_PEAK))
      shore_feature(doms[i].peak,elev,data,filter,basin_elev,set_edit,mark_edit);
    if (filter.test(FT_SADDLE))
      shore_feature(doms[i].saddle,elev,data,filter,sadl_elev,set_edit,mark_edit);
  }

  if (filter.test(FT_RUNOFF)) {
    n=runoffs.size();
    for (i=0; i<n; ++i) {
      shore_feature(runoffs[i],elev,data,filter,basin_elev,set_edit,mark_edit);
    }
  }

  if (filter.test(FT_BASINSADL)) {
    n=bsnsdls.size();
    for (i=0; i<n; ++i) {
      shore_feature(bsnsdls[i],elev,data,filter,basin_elev,set_edit,mark_edit);
    }
  }
}

#endif // io_support

#endif // def administrator


// Domain map manipulation tools

void fill_domap(const Elev_map& elev, Index_map& domap)
{
  Rectangl map_ovlp=(Rectangl)domap&(Rectangl)elev;
  RawGridSet grid_ovlp(map_ovlp);
  Grid grid(elev);
  Bool_map mark((Rectangl)elev);
  mark.fill(false);
  for (RawGridSet::Iter gi(grid_ovlp); gi; ++gi)
    if (elev[*gi]!=elev_undef)
      walkup_peak(*gi,grid,elev,domap,mark);
}

struct Domain_info {
  Elevation high_point,high_bdy;
  Domain::Index parent;
  float high_slope;
  Domain_info() : high_point(-elev_infty),
    high_bdy(-elev_infty),parent(Domain::undef) {}
};

void Divide_tree_map::consolidate_domap(const Elev_map& elev, const Divide_tree *tree,
	Index_map& domap, Domain::Index n_domain)
{
  Domain_info *dominfo=new Domain_info[n_domain];
  Domain::Index idom,nbr_dom;
  Elevation elv,nbr_elv;
  float slope;
  Rectangl tree_alloc,tree_defined;
  if (tree) tree->area(tree_alloc,tree_defined);

  // find the high point, high boundary point, and parent of each domain
  RawGridSet::Iter gi=Rectangl(domap);
  for (; gi; ++gi) {
    idom=domap[*gi];
    if (idom==Domain::undef) continue;
    assert(idom<=n_domain);
    Domain_info& dom=dominfo[idom];
    elv=elev.contains(*gi) ? elev[*gi] : elev_undef;
    if (elv==elev_undef) {
      if (!tree_defined.contains(*gi) ||
	  (idom<tree->n_dom() && (*tree)[idom].peak.location)) {
	// Domain is, or may be, part of a tree of interest.
	// Therefore we must assume that the undefined elevation could be high,
	// and we will not consolidate this domain.
	dom.high_point=elev_infty;
	continue;
      }
    }
    if (elv>dom.high_point) dom.high_point=elv;
    // now look for neighbors of the domain of the current point
    for (Neighbor_iter ni(elev,*gi); ni; ++ni) {
      nbr_dom=domap[*ni];
      if (nbr_dom==idom || nbr_dom==Domain::undef) continue;
      // we have a neighboring grid point that belongs to a different domain
      assert(nbr_dom<=n_domain);
      nbr_elv=elev[*ni];
      if (nbr_elv>=elv && elv>=dom.high_bdy) {
	// we step level or up to the neighboring domain, and
	// the current domain point is at least as high
	// as any other boundary point for that domain.
	// ties in boundary elev are broken by the steepest stepup
	slope=(nbr_elv-elv)/ni.dist();
	if (elv>dom.high_bdy || slope>dom.high_slope) {
	  // new high boundary point
	  dom.high_bdy=elv;
	  dom.parent=nbr_dom;
	  dom.high_slope=slope;
	}
      }
    }
  }

  // prepare domain translation table for the consolidation
  bool any_consol=false;
  for (idom=0; idom<n_domain; ++idom) {
    Domain_info& dom=dominfo[idom];
    if (dom.parent==Domain::undef || dom.high_bdy<dom.high_point ||
	dominfo[dom.parent].high_point==dom.high_point && dom.parent>idom)
      dom.parent=idom;
    else any_consol=true;
  }

  if (any_consol) {
    // flush out any "chains" of multiple translation
    for (idom=0; idom<n_domain; ++idom) {
      Domain_info& dom=dominfo[idom];
      while (dom.parent!=dominfo[dom.parent].parent)
	dom.parent=dominfo[dom.parent].parent;
    }
    // now translate every zero-prom domain to its parent
    for (gi.reset(); gi; ++gi) {
      Domain::Index& idom=domap[*gi];
      if (idom!=Domain::undef) idom=dominfo[idom].parent;
    }
  }

  delete[] dominfo;
}

void Divide_tree_map::purge_domap(Index_map& domap, Elevation prom,
				  bool offmap, bool rotate) const
{
  // prepare translation table
  Domain::Index i,n=n_dom(),*xlate=new Domain::Index[n];
  const Domain *dom;
  Elevation const Domain::Sadl_prom::*prom_type=
    sadl_prom_types[offmap][rotate];
  for (i=0; i<n; ++i) {
    dom=&doms[i];
    if (!dom->peak.location) continue;
    while (dom->parent.cell && dom->parent.saddle->prom.saddle.*prom_type<prom)
      dom=dom->parent.cell;
    xlate[i]=index(dom);
  }

  // apply the translation to the domain map
  xlate_domap(domap,xlate,n);

  delete[] xlate;
}

// make a domain map consistent with a divide tree.
// On input, n_domain must be larger than the highest index in the input domain map.
// On output, n_domain is one more than the highest index in the output domain map.
void Divide_tree_map::resolve_domap(Index_map& domap, Domain::Index& n_domain,
		   void (*undef_cb)(), void (*multiple_cb)()) const
{
  Domain::Index i,n=doms.size();
  Domain::Index *xlate=new Domain::Index[n_domain+n];

  // initialize translation array
  for (i=0; i<n_domain+n; ++i)
    xlate[i]=Domain::undef;
  n_domain=n;
  for (i=0; i<n; ++i) { // loop over peaks
    // define translation for the domain containing the peak
    const GridPoint& loc=doms[i].peak.location;
    if (loc && domap.contains(loc)) {
      Domain::Index& d=domap[loc];
      if (d==Domain::undef) {
	// peak does not belong to a defined domain
	if (undef_cb) {
	  undef_cb();
	  undef_cb=0;
	}
      }
      else if (xlate[d]==Domain::undef) xlate[d]=i;
      else {
	// peak belongs to a domain already assigned to another peak.
	// Use the higher.
	if (doms[i].peak.elev.low>doms[xlate[d]].peak.elev.low)
	  xlate[d]=i;
	if (multiple_cb) {
	  multiple_cb();
	  multiple_cb=0;
	}
      }
    }
  }
  for (Rect_iter_all gi(domap); gi; ++gi) {
    Domain::Index& d=domap[*gi];
    if (d!=Domain::undef) {
      if (xlate[d]==Domain::undef) {
	// Domain didn't contain a peak and doesn't yet have a translation.
	// Assign translation to next higher available index, beginning after
	// the peak domain indices.
	xlate[d]=n_domain;
	++n_domain;
      }
      d=xlate[d];
    }
  }

  delete[] xlate;
}

void xlate_domap(Index_map& domap, const Domain::Index *xlate,
		 Domain::Index n_domain)
{
  for (RawGrid_iter gi(domap); gi; ++gi) {
    Domain::Index& d=domap[*gi];
    if (d<n_domain) d=xlate[d];
  }
}

#define NONTREE(idom) ((idom)>=tree.n_dom() || !tree[idom].peak.location)

struct Nontree {
  Domain::Index parent;
  Elevation peak,saddle;
  Nontree() : parent(Domain::undef),peak(-elev_infty),saddle(-elev_infty) {}
  void make_patriarch(const Divide_tree&, Domain::Index);
};

static Nontree *nontree;

void Nontree::make_patriarch(const Divide_tree& tree, Domain::Index n_domain)
{
  assert(NONTREE(this-nontree));
  assert(this>=nontree && this-nontree<n_domain);
  if (parent!=Domain::undef) {
    nontree[parent].make_patriarch(tree,n_domain);
    nontree[parent].parent=this-nontree;
    nontree[parent].saddle=saddle;
    parent=Domain::undef;
    saddle=-elev_infty;
  }
}

// Find lowest saddle between peak /start/ and the "real" divide tree.
// /nstep/ is the number of steps to the real divide tree.
static Domain::Index low_saddle(const Divide_tree& tree,
				Domain::Index start, unsigned& nstep)
{
  Domain::Index start1=start;
  Domain::Index low_sadl_indx=Domain::undef;
  nstep=0;
  while (start!=Domain::undef && NONTREE(start)) {
    ++nstep;
    if (low_sadl_indx==Domain::undef ||
	nontree[start].saddle<nontree[low_sadl_indx].saddle)
      low_sadl_indx=start;
    start=nontree[start].parent;
    assert(start1!=start);
  }
  return low_sadl_indx;
}

static Domain::Index low_saddle_limit(Domain::Index& start, unsigned nstep)
{
  Domain::Index low_sadl_indx=Domain::undef;
  while (nstep>0) {
    if (low_sadl_indx==Domain::undef ||
	nontree[start].saddle<nontree[low_sadl_indx].saddle)
      low_sadl_indx=start;
    start=nontree[start].parent;
    --nstep;
  }
  return low_sadl_indx;
}

// translate the domain index /idom/ to the most remote possible ancestor
// without going through a saddle lower than /elv/
static void highest_parent(const Divide_tree& tree, const Elev_map& elev,
			   Domain::Index& idom, Elevation elv)
{
  const Feature *sadl;
  if (idom!=Domain::undef) {
    // translate /idom/ through the nontree portion of the domain hierarchy
    while (NONTREE(idom) && elv<=nontree[idom].saddle)
      idom=nontree[idom].parent;
    // could we get into the "real tree?
    if (!NONTREE(idom)) {
      // yep. continue promoting /idom/ up the hierarchy.
      while (tree[idom].parent.island &&
	     (sadl=&tree[idom].parent.saddle->saddle,
	      elv<=(elev.contains(sadl->location) ?
		    elev[sadl->location] : sadl->elev.low)))
	idom=tree.index(tree[idom].parent.island);
    }
  }
}

void Divide_tree_map::lpr_map(const Elev_map& elev, Index_map& domap,
			      Domain::Index n_domain) const
{
  Elevation ctr_elev,jct_elev,sadl_elev_1,sadl_elev_2;
  Domain::Index domain1,domain2,low_saddle_1,low_saddle_2,d1,d2;
  unsigned nstep1,nstep2;
  const Divide_tree& tree=*this;

  // Construct the tree for the peak indices not in the main divide tree
  Rectangl map_ovlp=(Rectangl)domap&(Rectangl)elev;
  RawGridSet grid_ovlp(map_ovlp);
  RawGridSet::Iter gi(grid_ovlp);
  Conditional_nbr_iter<RawNbr_iter,RawGridSet> ni(grid_ovlp,*gi);
  nontree=new Nontree[n_domain];
  for (; gi; ++gi) {
    domain1=domap[*gi];
    if (domain1!=Domain::undef && NONTREE(domain1)) {
      assert(domain1<n_domain);
      // domain is defined but its peak is not in tree
      ctr_elev=elev[*gi];
      // keep track of highest point in domain
      if (ctr_elev>nontree[domain1].peak) nontree[domain1].peak=ctr_elev;
      // find neighbors in a different domain
      for (ni.reset(*gi); ni; ++ni) {
	jct_elev=elev[*ni];
	if (jct_elev>ctr_elev) jct_elev=ctr_elev;
	domain2=domap[*ni];
	if (domain2!=Domain::undef && domain1!=domain2) {
	  // we found a domain boundary.
	  // Find lowest saddle on each side of the junction
	  // before the main divide tree.
	  low_saddle_1=low_saddle(tree,domain1,nstep1);
	  low_saddle_2=low_saddle(tree,domain2,nstep2);
	  if (low_saddle_1==low_saddle_2) {
	    // low saddles are equal. Loop was closed before reaching it.
	    // Find the lowest saddle before loop closure.
	    d1=domain1;
	    d2=domain2;
	    low_saddle_1=low_saddle_2=Domain::undef;
	    if (nstep1>nstep2) {
	      // side 1 is longer. Advance it until the remainder is the same
	      // length as side 2, finding the lowest saddle along the way.
	      low_saddle_1=low_saddle_limit(d1,nstep1-nstep2);
	    }
	    else if (nstep2>nstep1) {
	      // side 2 is longer. Advance it until the remainder is the same
	      // length as side 1, finding the lowest saddle along the way.
	      low_saddle_2=low_saddle_limit(d2,nstep2-nstep1);
	    }
	    // reaminder of both sides is now the same.
	    // Find the lowest saddle on each side until they converge.
	    while (d1!=d2) {
	      assert(d1!=Domain::undef && d2!=Domain::undef);
	      if (low_saddle_1==Domain::undef ||
		  nontree[d1].saddle<nontree[low_saddle_1].saddle)
		low_saddle_1=d1;
	      d1=nontree[d1].parent;
	      if (low_saddle_2==Domain::undef ||
		  nontree[d2].saddle<nontree[low_saddle_2].saddle)
		low_saddle_2=d2;
	      d2=nontree[d2].parent;
	    }
	  }

	  // Connect junction as appropriate.
	  sadl_elev_1=low_saddle_1==Domain::undef ?
	    elev_infty : nontree[low_saddle_1].saddle;
	  sadl_elev_2=low_saddle_2==Domain::undef ?
	    elev_infty : nontree[low_saddle_2].saddle;
	  if (sadl_elev_1<sadl_elev_2 && sadl_elev_1<jct_elev) {
	    nontree[low_saddle_1].parent=Domain::undef;
	    nontree[domain1].make_patriarch(tree,n_domain);
	    nontree[domain1].parent=domain2;
	    nontree[domain1].saddle=jct_elev;
	  }
	  else if (sadl_elev_2<sadl_elev_1 && sadl_elev_2<jct_elev) {
	    nontree[low_saddle_2].parent=Domain::undef;
	    nontree[domain2].make_patriarch(tree,n_domain);
	    nontree[domain2].parent=domain1;
	    nontree[domain2].saddle=jct_elev;
	  }
	} // if (jct_elev<ctr_elev)
      } // for (ni)
    } // if (dom1!=Domain::undef && NONTREE(dom1))
  } // for (gi)

  // Clear inversions in nontree domains.
  for (domain1=0; domain1<n_domain; ++domain1) {
    if (NONTREE(domain1)) {
      Domain::Index& iparent=nontree[domain1].parent;
     clear_sadl_inv:
      highest_parent(tree,elev,iparent,nontree[domain1].saddle);
      if (iparent!=Domain::undef && NONTREE(iparent) &&
	  nontree[domain1].peak>nontree[iparent].peak) {
	domain2=iparent;
	iparent=nontree[domain2].parent;
	nontree[domain2].parent=domain1;
	sadl_elev_1=nontree[domain1].saddle;
	nontree[domain1].saddle=nontree[domain2].saddle;
	nontree[domain2].saddle=sadl_elev_1;
	goto clear_sadl_inv;
      }
    }
  }

  for (gi.reset(); gi; ++gi) {
    highest_parent(tree,elev,domap[*gi],elev[*gi]);
  }

  delete[] nontree;
}

#ifdef io_support

static Domain::Index find_ovlp(Divide_tree& tree, const Feature& drg_sadl,
			       const Elev_map& elev, const Index_map& domap,
			       int drain, bool (*nonpeak_cb)())
{
  if (drg_sadl.edge_effect) {
    return Domain::undef;
  }
  set<Domain::Index> common_ovlp;
  set<Domain::Index>::const_iterator ovlpi1,ovlpi2,covlpi;
  // find any peaks that are reachable via an uphill walk from the saddle
  if (!elev.contains(drg_sadl.location)) return Domain::undef;
  Elevation sadl_elev=elev[drg_sadl.location];
  if (sadl_elev==elev_undef) return Domain::undef;
  sadl_elev*=drain;
  Grid grid(elev);
  for (Neighbor_iter ni(grid,drg_sadl.location); ni; ++ni) {
    if (elev.contains(*ni) && elev[*ni]!=elev_undef && elev[*ni]*drain>=sadl_elev &&
	domap.contains(*ni) && domap[*ni]!=Domain::undef) {
      common_ovlp.insert(domap[*ni]);
    }
  }

  // find all divide saddles that connect common overlapping domains
  vector<Basin_saddle *>::const_iterator bsi;
  Distance sadl_dist,close_sadl_dist=GridPoint::huje;
  const Basin_saddle *close_bs=0;
  const Feature *close_sadl=0;
  Domain::Index jbs;
  for (covlpi=common_ovlp.begin(); covlpi!=common_ovlp.end(); ++covlpi) {
    if (*covlpi>=tree.n_dom()) {
      if (nonpeak_cb && !tree.valid_peak(*covlpi)) {
	if (!(*nonpeak_cb)()) {
	  return Domain::undef-1;
	}
	nonpeak_cb=0; // prevent message from being displayed again
      }
      continue;
    }
    const Domain& covlp=tree[*covlpi];
    for (bsi=covlp.bsnsdls.begin(); bsi!=covlp.bsnsdls.end(); ++bsi) {
      // for each basin saddle involving a domain of interest
      if (&covlp!=(*bsi)->peak1) continue; // we'll pick it up from the other side
      jbs=tree.index((*bsi)->peak2);
      if (common_ovlp.find(jbs)!=common_ovlp.end()) {
	// the other domain is also of interest.
	sadl_dist=drg_sadl.location.rel_dist((*bsi)->location);
	if (sadl_dist<close_sadl_dist) {
	  // it's the nearest one. Replace the previous candidate.
	  close_bs=&**bsi;
	  close_sadl=0;
	  close_sadl_dist=sadl_dist;
	}
      }
    }
    if (common_ovlp.find(tree.index(covlp.primary_nbr))!=common_ovlp.end()) {
      // the peak on the other side of the domain prominence saddle is of interest.
      sadl_dist=drg_sadl.location.rel_dist(covlp.saddle.location);
      if (sadl_dist<close_sadl_dist) {
	// it's the nearest one. Replace the previous candidate.
	close_bs=0;
	close_sadl=&covlp.saddle;
	close_sadl_dist=sadl_dist;
      }
    }
  }

  return close_bs ? tree.index(*close_bs) : Domain::undef;
}

bool getNbrs(const Elev_map& elev, const GridPoint& location, double z[3][3])
{
  bool nbrs_defined=true;
  int i,j;
  // we shouldn't rely on the neighbor iterator working a particular way.
  // we should explicity set the location of each neighboring grid point
  // because we must know for sure which neighbor is being sampled.
  for (i=-1; i<=1; ++i) {
    for (j=-1; j<=1; ++j) {
      GridPoint nbr=location;
      nbr.x+=i;
      nbr.y+=j;
      z[i+1][j+1]=elev.contains(nbr)?elev[nbr]:elev_undef;
      if (z[i+1][j+1]==elev_undef) {
	// neighbors are not all defined.
	nbrs_defined=false;
      }
    }
  }
  return nbrs_defined;
}

void Divide_tree_map::purge_noise(const Elev_map& elev, float noise,
		bool keep_name, bool keep_edit, bool hess_nc, bool resid_nc,
		double& big_noise, Elevation& big_prom_noise,
		Domain::Index **xlate)
{
  Domain::Index ndom=doms.size(),idom;
  bool *keepit=new bool[ndom];
  double z[3][3],h1,h2,ra1,ra2,rs,peak_noise;
  unsigned nnc=2;
  if (hess_nc) nnc+=2;
  if (resid_nc) nnc+=1;
  noise=noise*noise/nnc;
  Elevation prom;
  big_noise=big_prom_noise=0;
  for (idom=0; idom<ndom; ++idom) {
    Domain& domain=doms[idom];
    if (domain.peak.location) {
      prom=domain.extreme_pk_prom(LO_END);
      if (prom==elev_infty) keepit[idom]=true;
      else if (!getNbrs(elev,domain.peak.location,z)) keepit[idom]=true;
      else if (keep_name && domain.peak.name || keep_edit && domain.peak.edited)
	keepit[idom]=true;
      else {
	ra1=z[0][0]+z[1][2]+z[2][1]-z[2][2]-z[0][1]-z[1][0];
	ra2=z[0][2]-z[1][2]+z[2][1]-z[2][0]-z[0][1]+z[1][0];
	peak_noise=(ra1*ra1+ra2*ra2)/6;
	if (hess_nc) {
	  h1=z[0][0]-z[0][2]-z[2][0]+z[2][2];
	  h2=z[0][1]+z[2][1]-z[1][0]-z[1][2];
	  peak_noise+=(h1*h1+h2*h2)/4;
	}
	if (resid_nc) {
	  rs=4*z[1][1]+z[0][0]+z[0][2]+z[2][0]+z[2][2]
	    -2*(z[0][1]+z[1][0]+z[1][2]+z[2][1]);
	  peak_noise+=rs*rs/36;
	}
	keepit[idom] = prom*prom >= peak_noise*noise;
	if (peak_noise>big_noise) big_noise=peak_noise;
	if (!keepit[idom]) {
	  if (prom>big_prom_noise) big_prom_noise=prom;
	}
      }
    }
  }
  big_noise=sqrt(big_noise/nnc);
  if (xlate) {
    *xlate=new Domain::Index[ndom];
    for (idom=0; idom<ndom; ++idom)
      (*xlate)[idom]=idom;
  }
  bool cont=true;
  while (cont) {
    cont=false;
    TRACE("%d peaks in use\n",ndom_inuse());
    for (idom=0; idom<ndom; ++idom) {
      Domain& domain=doms[idom];
      if (domain.peak.location && !keepit[idom]) {
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
	  // Remove it along with its highest saddle.
	  Domain::Neighbor_iter ni(domain);
	  assert(ni);
	  Domain *elim_saddle=*ni;
	  for (++ni; ni; ++ni) {
	    if (ni.saddle_owner()->saddle.elev.low>
		domain.saddle_owner(elim_saddle)->saddle.elev.low) {
	      elim_saddle=*ni;
	    }
	  }
	  domain.remove(elim_saddle);
	  if (xlate) {
	    (*xlate)[index(&domain)]=index(elim_saddle);
	  }
	  cont=true;
	} // if (!domain.neighbors.empty())

      } // if (domain.peak.location && !has_min_prom[idom])
    } // for (idom)
  } // while (cont)

  if (xlate) {
    // flush out any "chains" of multiple translation
    for (idom=0; idom<ndom; ++idom) {
      Domain::Index& dom=(*xlate)[idom];
      while (dom!=(*xlate)[dom])
	dom=(*xlate)[dom];
    }
  }

  delete[] keepit;

  set_cycle_info();
}

bool Divide_tree_map::purge_bs_drainage(const Divide_tree& drg_tree,
	const Index_map& domap, const Index_map& drg_domap,
	const Elev_map& elev, bool drain, bool (*nonpeak_cb)())
{
  Domain::Index ndom_drg=drg_tree.n_dom(),idom_drg,ibs,
    nbs_div=bsnsdls.size(),ibs_div;
  Rectangl bound=(Rectangl)domap;
  bool *matched=new bool[nbs_div];
  for (ibs_div=0; ibs_div<nbs_div; ++ibs_div) {
    const GridPoint& bs_loc=bsnsdls[ibs_div].location;
    matched[ibs_div]=!domap.contains(bs_loc) || domap[bs_loc]==Domain::undef;
  }

  // for all drainage saddles
  for (idom_drg=0; idom_drg<ndom_drg; ++idom_drg) {
    const Domain& dom_drg=drg_tree[idom_drg];
    if (dom_drg.primary_nbr) {
      ibs_div=find_ovlp(*this,dom_drg.saddle,elev,domap,drain?-1:1,nonpeak_cb);
      if (ibs_div!=Domain::undef) {
	if (ibs_div==Domain::undef-1) return false;
	assert(ibs_div<nbs_div);
	matched[ibs_div]=true;
      }
    }
  }
  // for all drainage prominence saddles
  // (which are stored and manipulated like basin saddles)
  Domain::Index nbs=drg_tree.n_bsnsdl();
  for (ibs=0; ibs<nbs; ++ibs) {
    const Basin_saddle& bs=drg_tree.bsnsdl(ibs);
    if (bs) {
      ibs_div=find_ovlp(*this,bs,elev,domap,drain?-1:1,nonpeak_cb);
      if (ibs_div!=Domain::undef) {
	if (ibs_div==Domain::undef-1) return false;
	assert(ibs_div<nbs_div);
	matched[ibs_div]=true;
      }
    }
  }

  for (ibs_div=0; ibs_div<nbs_div; ++ibs_div) {
    if (!matched[ibs_div]) {
      bsnsdls[ibs_div].remove();
    }
  }

  delete[] matched;

  return true;
}


Domain::Index Divide_tree_map::assoc_peak(
	const GridPoint& location, const Assoc_info& assoc,
	const Index_map& domap, const Elev_map& elev,
	const Grid& grid, Bool_map& mark,
	const Elevation *prom, const vector<Domain *>& sorted_peaks)
{
  assert(sorted_peaks.size()==doms.size());
  Domain::Index idom,jdom;
  // use domain, if enabled
  if (assoc.domain && domap.contains(location)) {
    idom=domap[location];
    if (idom<sorted_peaks.size() && doms[idom].peak.location &&
	(location==doms[idom].peak.location ||
	 location-doms[idom].peak.location<=assoc.radius)) {
      return idom;
    }
  }
  // can't use domain alone. try elevation map.
  if (assoc.walkup && elev.contains(location) &&
      elev[location]!=elev_undef) {
    GridPoint loc=location,loc1;
    // walkup. try to find domain or peak on each step.
    while (true) {
      // are we exactly at a peak?
      jdom=search_dom(loc,sorted_peaks,&Domain::peak);
      if (jdom<doms.size() && sorted_peaks[jdom]->peak.location==loc) {
	// yes.
	if (loc!=location && loc-location>assoc.radius)
	  goto map_failed; // but it's too far away.
	return index(sorted_peaks[jdom]); // convert from sorted-peak index to tree storage index
      }
      // no. try the domain, if enabled
      if (assoc.domain && domap.contains(loc)) {
	idom=domap[loc];
	if (idom<doms.size() && doms[idom].peak.location &&
	    (location==doms[idom].peak.location ||
	     location-doms[idom].peak.location<=assoc.radius)) {
	  return idom;
	}
      }
      // no domain success either. step up and try again.
      loc1=steepest_nbr(loc,elev,grid);
      if (!loc1) break;
      loc=loc1;
    }
    // we fall through to here if walkup/domain combo fails.
    // make one last try to find it on a flat region.
    Flat_mark fm(elev,elev[loc],mark);
    Region_iter<Flat_mark,Grid> ri(loc,fm,grid);
    for (; ri; ++ri) {
      if (loc-(*ri)>assoc.radius) continue;
      jdom=search_dom(*ri,sorted_peaks,&Domain::peak);
      if (jdom<doms.size() && sorted_peaks[jdom]->peak.location==*ri) break;
    }
    if (!ri) jdom=Domain::undef;
    fm.visit_mark=false;
    for (ri.reset(loc); ri; ++ri);
    if (jdom!=Domain::undef) {
      idom=index(sorted_peaks[jdom]); // convert from sorted-peak index to tree storage index
      return idom;
    }
  }

map_failed:
  // if we get this far, domain and walkup have both failed (or was disabled).
  // We must now try prominence or distance.

  Domain::Index lim_lo,lim_hi;
  Feature *dest_peak=0;
  Elevation dest_prom;
  Distance dist,dest_dist;
  dom_range(location,sorted_peaks,&Domain::peak,assoc.radius,lim_lo,lim_hi);
  for (jdom=lim_lo; jdom<lim_hi; ++jdom) {
    Domain& cand_dest=*sorted_peaks[jdom];
    idom=index(sorted_peaks[jdom]);
    dist=cand_dest.peak.location-location;
    if (cand_dest.peak.location!=location && dist>assoc.radius) continue;
    if (dest_peak==0) {
      // no destination peak yet. This is the first one.
      dest_peak=&cand_dest.peak;
      if (prom) dest_prom=prom[idom];
      dest_dist=dist;
    }
    else {
      // we already have a destination peak. Decide which will be used.
      if (assoc.elev_prom && prom[idom]>dest_prom ||
	  (!assoc.elev_prom || prom[idom]==dest_prom) && dist<dest_dist) {
	dest_peak=&cand_dest.peak;
	if (prom) dest_prom=prom[idom];
	dest_dist=dist;
      }
    } // if (dest_peak==0) else
  } // for (jdom)

  //location.trace();
  if (dest_peak) {
    idom=index(Domain::FromPeak(dest_peak));
    //TRACE(" associated with peak ");
    //dest_peak->trace();
    //TRACE(", %.2f\n",location-dest_peak->location);
  }
  else {
    idom=Domain::undef;
    //TRACE(" not associated to any peak\n");
  }

  return idom;
}

static Elevation get_bs_prom(const Basin_saddle& bs, const GridPoint& source_loc,
	double radius, const Divide_tree& tree, const Elevation sadl_prom[])
{
  Elevation prom=-elev_infty;
  for (Basin_saddle::Cycle_iter ci(bs); ci; ++ci) {
    if ((*ci)->saddle.location-source_loc>radius) {
      Elevation sp=sadl_prom[tree.index(*ci)];
      if (sp>prom) prom=sp;
    }
  }
  return prom;
}

static void check_dest_sadl(const GridPoint& location, const Assoc_info& assoc,
			    Domain& cand_dest, Domain *&dest_sadl,
			    const Elevation *cand_prom, Elevation& dest_prom,
			    Distance& dest_dist)
{
  Distance dist=cand_dest.saddle.location-location;
  if (cand_dest.saddle.location!=location && dist>assoc.radius) return;
  if (!dest_sadl) {
    // no destination saddle yet.  This is the first one.
    dest_sadl=&cand_dest;
    if (cand_prom) dest_prom=*cand_prom;
    dest_dist=dist;
  }
  else {
    // we already have a destination saddle. Decide which will be used.
    if (assoc.elev_prom && *cand_prom>dest_prom ||
	(!assoc.elev_prom || *cand_prom==dest_prom) && dist<dest_dist) {
      dest_sadl=&cand_dest;
      if (cand_prom) dest_prom=*cand_prom;
      dest_dist=dist;
    }
  } // if (dest_sadl==0) else
}

static inline GridPoint::Coord hemi_area(const GridPoint& c1, GridPoint c2)
{
  c2.x-=c1.x;
  c2.y-=c1.y;
  return c1.x*c2.y-c1.y*c2.x;
}

// "start" is moved to just before "end", so that its saddle represents an endpoint
static GridPoint::Coord hemi_area(const Domain *&start, const Domain *end)
{
  assert(start!=end);
  GridPoint::Coord ha=0;
  for (const Domain *d=start; d=start->primary_nbr,d!=end; start=d)
    ha+=hemi_area(start->saddle.location,d->saddle.location);
  return ha;
}

// "peak1" and "peak2" are changed to show the endpoint saddle owners
static GridPoint::Coord hemi_area(const Domain *&peak1, const Domain *&peak2,
				  const Domain *ca)
{
  assert(peak1!=peak2);
  GridPoint::Coord ha=0;
  const Domain *p1=peak1,*p2=peak2;
  if (p1!=ca) ha+=hemi_area(p1,ca);
  if (p2!=ca) ha-=hemi_area(p2,ca);
  if (p1!=ca && p2!=ca) ha+=hemi_area(p1->saddle.location,p2->saddle.location);
  if (peak1==ca) peak1=p2;
  else if (peak2==ca) peak2=p1;
  return ha;
}

static GridPoint::Coord pk_hemi_area(const Domain *peak1, const Domain *peak2,
				     const Domain *ca)
{
  GridPoint p1_loc=peak1->peak.location,p2_loc=peak2->peak.location;
  GridPoint::Coord path_ha=hemi_area(peak1,peak2,ca);
  return path_ha+
    hemi_area(p1_loc,peak1->saddle.location)+
    hemi_area(peak2->saddle.location,p2_loc);
}

static GridPoint::Coord pk_hemi_area(const Basin_saddle& bs,
	const Domain *peak1, const Domain *peak2, const Domain *ca,
	const Domain *ca_p1s1, const Domain *ca_p1s2,
	const Domain *ca_p2s1, const Domain *ca_p2s2)
{
  const Domain *ca1,*ca2,*s1,*s2;

  if (ca_p1s1!=bs.common_ancestor && ca_p1s1!=ca ||
      ca_p2s2!=bs.common_ancestor && ca_p2s2!=ca) {
    ca1=ca_p1s1;
    ca2=ca_p2s2;
    s1=bs.peak1;
    s2=bs.peak2;
  }
  else if (ca_p2s1!=bs.common_ancestor && ca_p2s1!=ca ||
	   ca_p1s2!=bs.common_ancestor && ca_p1s2!=ca) {
    ca1=ca_p1s2;
    ca2=ca_p2s1;
    s1=bs.peak2;
    s2=bs.peak1;
  }
  else assert(0);

  GridPoint::Coord ha=0;
  GridPoint p1_loc=peak1->peak.location,p2_loc=peak2->peak.location;
  if (peak1!=s1) {
    ha+=hemi_area(peak1,s1,ca1);
    ha+=hemi_area(p1_loc,peak1->saddle.location);
    ha+=hemi_area(s1->saddle.location,bs.location);
  }
  else ha+=hemi_area(p1_loc,bs.location);
  if (peak2!=s2) {
    ha+=hemi_area(s2,peak2,ca2);
    ha+=hemi_area(peak2->saddle.location,p2_loc);
    ha+=hemi_area(bs.location,s2->saddle.location);
  }
  else ha+=hemi_area(bs.location,p2_loc);
  return ha;
}

static GridPoint::Coord loop_area(const Domain *peak1, const Domain *peak2,
				  const Domain *ca, GridPoint::Coord ref_ha)
{
  return abs(pk_hemi_area(peak1,peak2,ca)-ref_ha);
}

static GridPoint::Coord loop_area(const Basin_saddle& bs,
	const Domain *peak1, const Domain *peak2,
	const Domain *ca, GridPoint::Coord ref_ha,
	const Domain *ca_p1s1, const Domain *ca_p1s2,
	const Domain *ca_p2s1, const Domain *ca_p2s2)
{
  return abs(pk_hemi_area(bs,peak1,peak2,ca,ca_p1s1,ca_p1s2,ca_p2s1,ca_p2s2)-ref_ha);
}

static bool interleaved(const Domain *start, const Domain *end,
			const Domain *jctA[], const Domain *jctB[])
{
  unsigned hitA=0,hitB=0;
  while (true) {
    if (start==jctA[0] || start==jctA[1]) ++hitA;
    if (start==jctB[0] || start==jctB[1]) ++hitB;
    if (hitA==1 && hitB==1) return true;
    if (start==end || hitA+hitB>=2) return false;
    start=start->primary_nbr;
  }
}

Domain::Index Divide_tree_map::assoc_sadl(
	const GridPoint& location, const Assoc_info& assoc,
	Domain *peak1, Domain *peak2, const Domain *ca, GridPoint::Coord ref_ha,
	const vector<Domain *>& sorted_sadls,
	Elevation sadl_prom[], Elevation *bs_prom, bool use_status,
	const vector<Basin_saddle *>& sorted_bs, Domain::Index& ibs)
{
  assert((peak1==0)==(peak2==0));
  Domain::Index lim_lo,lim_hi,j,idom;
  Domain *dest_sadl=0;
  const Domain *dest_jct[2];
  Basin_saddle *dest_bs=0;
  Elevation dest_prom;
  Distance cand_dist,dest_dist;
  GridPoint::Coord cand_path,dest_path;
  if (assoc.ps_dest) {
    if (peak1) {
      // we're using topology. Loop over saddles on the path between peaks
      Domain *cand_dest;
      for (cand_dest=peak1; cand_dest!=ca; cand_dest=cand_dest->primary_nbr) {
	idom=index(cand_dest);
	check_dest_sadl(location,assoc,*cand_dest,dest_sadl,
			sadl_prom?sadl_prom+idom:0,dest_prom,dest_dist);
      }
      for (cand_dest=peak2; cand_dest!=ca; cand_dest=cand_dest->primary_nbr) {
	idom=index(cand_dest);
	check_dest_sadl(location,assoc,*cand_dest,dest_sadl,
			sadl_prom?sadl_prom+idom:0,dest_prom,dest_dist);
      }
    }
    else {
      // not using topology. Loop over all saddles that might be within range
      dom_range(location,sorted_sadls,(Feature Domain::*)&Domain::saddle,assoc.radius,lim_lo,lim_hi);
      for (j=lim_lo; j<lim_hi; ++j) {
	Domain& cand_dest=*sorted_sadls[j];
	idom=index(&cand_dest);
	check_dest_sadl(location,assoc,cand_dest,dest_sadl,
			sadl_prom?sadl_prom+idom:0,dest_prom,dest_dist);
      } // for (j) // sadl index
    }
  } // if (assoc.ps_dest)

  if (assoc.bs_dest) {
    Domain *ca_p1s1,*ca_p1s2,*ca_p2s1,*ca_p2s2;
    const Domain *cand_jct[2];
    // loop over all basin saddles that might be within range
    bs_range(location,sorted_bs,assoc.radius,lim_lo,lim_hi);
    for (j=lim_lo; j<lim_hi; ++j) {
      Basin_saddle& cand_dest=*sorted_bs[j];
      ibs=index(cand_dest);
      cand_dist=cand_dest.location-location;
      if (cand_dest.location!=location && cand_dist>assoc.radius) continue; // out of range, not a candidate
      if (peak1) {
	// we're using topology. BS is eligible only if there exists a rotation
	// that would put it between the neighbor peaks.
	cand_jct[0]=cand_dest.cycle_jct(peak1,ca_p1s1,ca_p1s2);
	cand_jct[1]=cand_dest.cycle_jct(peak2,ca_p2s1,ca_p2s2);
	if (cand_jct[0]==cand_jct[1]) continue;
      }
      if (bs_prom)
	*bs_prom=assoc.bspt_assoc?
	  get_bs_prom(cand_dest,location,assoc.radius,*this,sadl_prom):
	  cand_dest.extreme_prom(HI_END);
      if (assoc.path) cand_path=loop_area(cand_dest,peak1,peak2,ca,ref_ha,
					  ca_p1s1,ca_p1s2,ca_p2s1,ca_p2s2);
      if (!dest_bs) goto overturn; // no destination BS yet. This is the first one.
      if (assoc.path && (interleaved(peak1,ca,cand_jct,dest_jct) ||
			 interleaved(peak2,ca,cand_jct,dest_jct))) {
	// destination saddles are compared by path likeness
	if (cand_path<dest_path) goto overturn;
	if (cand_path>dest_path) continue;
      }
      if (assoc.elev_prom) {
	// destination saddles can be compared by prominence
	assert(bs_prom);
	if (*bs_prom>dest_prom) goto overturn;
	if (*bs_prom<dest_prom) continue;
	// if they have the same prominence (possibly -infty), use the higher
	if (cand_dest.elev.high>dest_bs->elev.high) goto overturn;
	if (cand_dest.elev.high<dest_bs->elev.high) continue;
      }
      // if no decision was made on the basis of path or prominence,
      // fall through to using distance.
      if (cand_dist<dest_dist) goto overturn;
      if (cand_dist>dest_dist) continue;
    overturn:
      dest_bs=&cand_dest;
      if (bs_prom) dest_prom=*bs_prom;
      dest_dist=cand_dist;
      if (assoc.path) {
	dest_path=cand_path;
	dest_jct[0]=cand_jct[0];
	dest_jct[1]=cand_jct[1];
      }
    } // for (j) // BS index
  } // if (assoc.bs_dest)

  // do we have a saddle and a basin saddle that are both eligible?
  if (dest_sadl && dest_bs && assoc.path && dest_bs->on_cycle(dest_sadl)) {
    // yes, destination saddles are compared by path likeness.
    cand_path=loop_area(peak1,peak2,ca,ref_ha);
    if (cand_path<dest_path) dest_bs=0;
    else if (cand_path>dest_path) dest_sadl=0;
  }
  if (dest_sadl && dest_bs && assoc.elev_prom) {
    // destination saddles are compared by "prominence".
    if (sadl_prom[index(dest_sadl)]>dest_prom) dest_bs=0;
    else if (sadl_prom[index(dest_sadl)]<dest_prom) dest_sadl=0;
  }
  if (dest_sadl && dest_bs) {
    // no decision was made on the basis of path likeness or prominence.
    // fall through to using distance.
    if (dest_bs->location-location<dest_sadl->saddle.location-location) 
      dest_sadl=0;
    else dest_bs=0;
  } // if (dest_sadl && dest_bs)

  //location.trace();
  if (dest_sadl) {
    assert(dest_bs==0);
    idom=index(dest_sadl);
    //TRACE(" associated with sadl ");
    //dest_sadl->trace();
    //TRACE(", %.2f\n",location-dest_sadl->location);
  }
  else {
    idom=Domain::undef;
    //TRACE(" not associated with any saddle\n");
  }
  if (dest_bs) {
    assert(dest_sadl==0);
    ibs=index(*dest_bs);
    assert(!bs_prom || *bs_prom!=elev_undef);
    //TRACE(" associated with BS ");
    //dest_bs->trace();
    //TRACE(", %.2f\n",location-dest_bs->location);
  }
  else {
    ibs=Domain::undef;
    //TRACE(" not associated with any BS\n");
  }

  return idom;
}

Domain::Index Divide_tree_map::assoc_ro(
	const GridPoint& location, const Assoc_info& assoc,
	const vector<Runoff *>& sorted_ro)
{
  if (!assoc.ro_dest) return Domain::undef;

  Domain::Index lim_lo,lim_hi,jro;
  Runoff *dest_ro=0;
  Elevation elev_diff;
  Distance dist,dest_dist;
  runoff_range(location,sorted_ro,assoc.radius,lim_lo,lim_hi);
  for (jro=lim_lo; jro<lim_hi; ++jro) {
    Runoff& cand_dest=*sorted_ro[jro];
    dist=cand_dest.location-location;
    if (cand_dest.location!=location && dist>assoc.radius) continue;
    if (dest_ro==0) {
      // no destination runoff yet. This is the first one.
      dest_ro=&cand_dest;
      dest_dist=dist;
    }
    else {
      // we already have a destination runoff. Decide which will be used.
      elev_diff=cand_dest.elev.low-dest_ro->elev.low;
      if (assoc.elev_prom && elev_diff>0 ||
	  (!assoc.elev_prom || elev_diff==0) && dist<dest_dist) {
	dest_ro=&cand_dest;
	dest_dist=dist;
      }
    } // if (dest_ro==0 else)
  } // for (jro)

  return dest_ro?index(*dest_ro):Domain::undef;
}

// The purpose of this class is to manage the consolidation of features
// when multiple source features match a single destination feature.
class Consol_feature {
  Elevation le_elev,he_elev;
  const Feature *name_src;
protected:
  Elev_intvl elev;
  Consol_feature(Elevation sign) : name_src(0) {init(sign);}
  void init(Elevation sign) {elev=le_elev=he_elev=-sign*elev_infty;}
  void add(const Feature&, Elevation);
public:
  void update(Feature&, vector<Feature *>&) const;
};

void Consol_feature::add(const Feature& f, Elevation sign)
{
  if (f.edited) {
    if ((f.elev.low-elev.low)*sign>0) elev.low=f.elev.low;
    if ((f.elev.high-elev.high)*sign>0) elev.high=f.elev.high;
    if (f.edge_effect == (sign>0)) {
      if ((f.elev.low-le_elev)*sign>0) le_elev=f.elev.low;
    }
    else {
      if ((f.elev.high-he_elev)*sign>0) he_elev=f.elev.high;
    }
  }
  if (f.name) {
    if (name_src==0) name_src=&f;
    else if (f.edited==name_src->edited) {
      Elev_endpt name_endpt=sign>0?LO_END:HI_END;
      if ((f.elev.*name_endpt-name_src->elev.*name_endpt)*sign>0) name_src=&f;
    }
    else if (f.edited) name_src=&f;
  }
}

void Consol_feature::update(Feature& dest, vector<Feature *>& conflicts) const
{
  bool conflict=false;
  if (abs(elev.high)<elev_infty) { // did ANY source feature consolidate elevation here?
    bool edge_effect=le_elev>he_elev;
    if (dest.edited) {
      conflict = edge_effect!=dest.edge_effect ||
	dest.elev.low!=elev.low || dest.elev.high!=elev.high;
    }
    else {
      dest.elev=elev;
      dest.edge_effect=edge_effect;
      dest.edited=true;
    }
  }
  if (name_src) {
    if (!dest.name) dest=name_src->name;
    else if (strcmp(name_src->name,dest.name)) conflict=true;
  }
  if (conflict) conflicts.push_back(&dest);
}

class Consol_peak : public Consol_feature {
public:
  Consol_peak() : Consol_feature(1) {}
  void operator+=(const Feature& f) {add(f,1);}
};

class Consol_sadl : public Consol_feature {
  Saddle::Status status;
  bool prom;
  void update_status(Saddle::Status s) {if (s>status) status=s;}
public:
  Consol_sadl() : Consol_feature(-1),prom(false),status(Saddle::STAT_UNKNOWN) {}
  void operator+=(const Saddle&);
  void operator+=(const Basin_saddle&);
  void update(Saddle&, vector<Feature *>&) const;
};

void Consol_sadl::operator+=(const Saddle& s)
{
  add(s,-1);
  prom=true;
  update_status((Saddle::Status)s.status);
}

void Consol_sadl::operator+=(const Basin_saddle& bs)
{
  update_status((Saddle::Status)bs.status);
  if (prom) return; // prom saddle has already consolidated here; can't take basin saddle

  if (elev.high==elev_infty) init(1); // reset the elevation for a maximum
  add(bs,1);
}

void Consol_sadl::update(Saddle& dest, vector<Feature *>& conflicts) const
{
  Consol_feature::update(dest,conflicts);
  if (status>dest.status) dest.status=status;
}


#define ASSOC_UNK (Domain::undef-1)

void Divide_tree_map::xfer_peak_info(
	Divide_tree& source, const Rectangl& bound, const Assoc_info& assoc,
	Elevation prom[], Domain::Index peak_assoc[],
	const Index_map& domap, const Elev_map& elev,
	Bool_map& mark, const Grid& grid, vector<Domain *>& sorted_peaks,
	vector<Feature *>& orphans, vector<Feature *>& conflicts)
{
  Domain::Index iconsrc,i,nsrcdom=source.doms.size();

  for (i=0; i<nsrcdom; ++i) peak_assoc[i]=ASSOC_UNK;
  sort_doms(&Domain::peak,sorted_peaks);
  if (assoc.elev_prom) {
    Topo_info *tree=get_island_tree(LO_END);
    assoc_peak_prom(tree,prom);
    delete[] tree;
  }
  Consol_peak *consol_src=new Consol_peak[n_dom()];
  // loop over source peaks & accumulate consolidated peaks
  for (i=0; i<nsrcdom; ++i) {
    Feature& source_peak=source[i].peak;
    if (source_peak.location && bound.contains(source_peak.location) &&
	(source_peak.name || source_peak.edited)) {
      // source peak is in bounds, and has name or edited elevation
      iconsrc=assoc_peak(source_peak.location,assoc,domap,elev,grid,mark,prom,sorted_peaks);
      peak_assoc[i]=iconsrc;
      if (iconsrc!=Domain::undef) {
	// found one. update it into the consolidated source list.
	consol_src[iconsrc]+=source_peak;
      }
      else {
	// not found. add to orphan list.
	orphans.push_back(&source_peak);
      }
    }
  }

  // now transfer consolidated source to destination
  for (iconsrc=0; iconsrc<sorted_peaks.size(); ++iconsrc)
    consol_src[iconsrc].update(doms[iconsrc].peak,conflicts);

  delete[] consol_src;
}

Domain *Divide_tree_map::nbr_peak(Domain *start, Domain *root, const Feature& sadl,
	const Divide_tree& source_tree, const Assoc_info& assoc, Elevation pk_prom[],
	const Index_map& domap, const Elev_map& elev, Bool_map& mark, const Grid& grid,
	const Domain *&src_peak, const vector<Domain *>sorted_peaks, Domain::Index peak_assoc[])
{
  Domain::Index i=source_tree.index(start);
  Domain *dest_peak=0;
  const Distance cpd=assoc.radius*assoc.cpdm;
  src_peak=0;
  if (peak_assoc[i]==ASSOC_UNK)
    peak_assoc[i]=assoc_peak(start->peak.location,assoc,
      domap,elev,grid,mark,pk_prom,sorted_peaks);
  if (peak_assoc[i]!=Domain::undef) {
    src_peak=&source_tree[i];
    dest_peak=&doms[peak_assoc[i]];
    if (sadl.location-dest_peak->saddle.location>cpd) return dest_peak;
  }
  for (Explorer xplr(start,root,HI_END); xplr; ) {
    const Saddle& xsadl=xplr.saddle();
    if (xsadl.elev<sadl.elev) break;
    if (!(xsadl.elev>sadl.elev) && dest_peak &&	(xsadl.edited || xsadl.name)) {
      xplr.stop_branch();
      continue;
    }
    i=source_tree.index(&*xplr);
    if (peak_assoc[i]==ASSOC_UNK)
      peak_assoc[i]=assoc_peak((*xplr).peak.location,assoc,
	domap,elev,grid,mark,pk_prom,sorted_peaks);
    if (peak_assoc[i]!=Domain::undef) {
      src_peak=&source_tree[i];
      dest_peak=&doms[peak_assoc[i]];
      if (sadl.location-dest_peak->saddle.location>cpd) return dest_peak;
    }
    ++xplr;
  }
  return dest_peak;
}

void Divide_tree_map::consol_1sadl(Saddle& source,
	const Assoc_info& assoc, Elevation sadl_prom[],
	Domain *peak1, Domain *peak2, GridPoint::Coord ref_ha,
	bool basin, Consol_sadl consol_sadl[], Consol_sadl consol_bs[],
	const vector<Domain *>& sorted_sadls, const vector<Basin_saddle *>& sorted_bs,
	vector<Feature *>& orphans, vector<Feature *>& conflicts, bool use_status)
{
  Domain::Index isadl,ibs;
  //TRACE("connects ");
  //peak1->peak.trace();
  //TRACE("  ");
  //peak2->peak.trace();
  //TRACE("\n");
  //Sleep(10);
  Consol_sadl *dest=0;
  const Domain *ca=peak1?Domain::common_ancestor(peak1,peak2):0;
  Elevation bs_prom;
  isadl=assoc_sadl(source.location,assoc,peak1,peak2,ca,ref_ha,sorted_sadls,sadl_prom,
		   assoc.elev_prom?&bs_prom:0,use_status,sorted_bs,ibs);
  if (isadl!=Domain::undef) {
    // found a prominence saddle. record it as possible consolidation.
    dest=consol_sadl+isadl;
  }
  else if (ibs!=Domain::undef) {
    // found a basin saddle. record it as possible consolidation.
    dest=consol_bs+ibs;
  }
  if (dest) {
    if (basin) {
      Basin_saddle& bs=(Basin_saddle&)source;
      (*dest)+=bs;
    }
    else (*dest)+=source;
  }
  else orphans.push_back(&source); // not found. add to orphan list.
}

void Divide_tree_map::xfer_sadl_info(Divide_tree& source, const Rectangl& bound,
	const Assoc_info& assoc, Sadl_src sadl_src,
	Elevation sadl_prom[], Elevation pk_prom[],
	Domain::Index peak_assoc[], const Index_map& domap, const Elev_map& elev,
	Bool_map& mark, const Grid& grid, const vector<Domain *>& sorted_peaks,
	vector<Feature *>& orphan_sadl, vector<Feature *>& conflict_sadl,
	vector<Feature *>& orphan_bs, vector<Feature *>& conflict_bs,
	bool use_status)
{
  vector<Domain *> sorted_sadls;
  vector<Basin_saddle *> sorted_bs;
  sort_doms((Feature Domain::*)&Domain::saddle,sorted_sadls);
  sort_bs(sorted_bs);
  Domain::Index ndom=doms.size();
  if (assoc.elev_prom) {
    Topo_info *tree=get_island_tree(HI_END);
    Domain::Index ipeak,isadl;
    for (ipeak=0; ipeak<ndom; ++ipeak) {
      if (!doms[ipeak].peak.location ||
	  tree[ipeak].parnt==Domain::undef) continue;
      isadl=tree[ipeak].sadl_dom;
      sadl_prom[isadl]=doms[ipeak].peak.elev.high-doms[isadl].saddle.elev.high;
    }
    delete[] tree;
  }
  Consol_sadl *consol_sadl=0,*consol_bs=0;
  Domain *peak1,*peak2,*pa;
  Domain *src_pk1,*src_pk2,*ca;
  GridPoint::Coord ref_ha;
  if (assoc.ps_dest) consol_sadl=new Consol_sadl[ndom];
  if (assoc.bs_dest) consol_bs=new Consol_sadl[n_bsnsdl()];
  // loop over source saddles
  if (sadl_src!=SS_BASIN) {
    for (rDom_iter domi=source.doms.begin(); domi!=source.doms.end(); ++domi) {
      pa=(*domi).primary_nbr;
      if (!pa) continue;
      Feature& src_sadl=(*domi).saddle;
      if (!bound.contains(src_sadl.location) ||
	  !src_sadl.name && !src_sadl.edited) continue;
      //TRACE("P Source ");
      //sadl_src.trace();
      //TRACE("\n");
      peak1=nbr_peak(&*domi,pa,src_sadl,source,assoc,pk_prom,
		     domap,elev,mark,grid,(const Domain *&)src_pk1,sorted_peaks,peak_assoc);
      if (!peak1) continue;
      peak2=nbr_peak(pa,&*domi,src_sadl,source,assoc,pk_prom,
		     domap,elev,mark,grid,(const Domain *&)src_pk2,sorted_peaks,peak_assoc);
      if (!peak2) continue;
      ca=Domain::common_ancestor(src_pk1,src_pk2);
      ref_ha=hemi_area(peak1->peak.location,src_pk1->peak.location)
	+pk_hemi_area(src_pk1,src_pk2,ca)
	+hemi_area(src_pk2->peak.location,peak2->peak.location);
      consol_1sadl((*domi).saddle,assoc,sadl_prom,
		   peak1,peak2,ref_ha,false,consol_sadl,consol_bs,
		   sorted_sadls,sorted_bs,orphan_sadl,conflict_sadl,use_status);
    }
  }
  if (sadl_src!=SS_PROM) {
    Domain *ca_p1s1,*ca_p1s2,*ca_p2s1,*ca_p2s2;
    for (rBSiter bsi=source.bsnsdls.begin(); bsi!=source.bsnsdls.end(); ++bsi) {
      if (!bound.contains((*bsi).location) ||
	  !(*bsi).name && !(*bsi).edited) continue;
      //TRACE("B Source ");
      //(*bsi).trace();
      //TRACE("\n");
      peak1=nbr_peak((*bsi).peak1,0,(*bsi),source,assoc,pk_prom,
		     domap,elev,mark,grid,(const Domain *&)src_pk1,sorted_peaks,peak_assoc);
      if (!peak1) continue;
      peak2=nbr_peak((*bsi).peak2,0,(*bsi),source,assoc,pk_prom,
		     domap,elev,mark,grid,(const Domain *&)src_pk2,sorted_peaks,peak_assoc);
      if (!peak2 || peak1==peak2) continue;
      ca=Domain::common_ancestor(src_pk1,src_pk2);
      (*bsi).cycle_jct(src_pk1,ca_p1s1,ca_p1s2);
      (*bsi).cycle_jct(src_pk2,ca_p2s1,ca_p2s2);
      ref_ha=hemi_area(peak1->peak.location,src_pk1->peak.location)
	+pk_hemi_area(*bsi,src_pk1,src_pk2,ca,ca_p1s1,ca_p1s2,ca_p2s1,ca_p2s2)
	+hemi_area(src_pk2->peak.location,peak2->peak.location);
      consol_1sadl(*bsi,assoc,sadl_prom,
		   peak1,peak2,ref_ha,true,consol_sadl,consol_bs,
		   sorted_sadls,sorted_bs,orphan_bs,conflict_bs,use_status);
    }
  }

  // now transfer consolidated source to destination
  // prominence saddles...
  if (consol_sadl) {
    for (Domain::Index iconsrc=0; iconsrc<sorted_sadls.size(); ++iconsrc) {
      consol_sadl[iconsrc].update(doms[iconsrc].saddle,conflict_sadl);
    }
  }
  // basin saddles.
  if (consol_bs) {
    for (Domain::Index iconsrc=0; iconsrc<sorted_bs.size(); ++iconsrc)
      consol_bs[iconsrc].update(bsnsdls[iconsrc],conflict_bs);
  }
  delete[] consol_sadl;
  delete[] consol_bs;
}

void Divide_tree_map::xfer_ro_info(
	Divide_tree& source, const Rectangl& bound, const Assoc_info& assoc,
	vector <Feature *>& orphans, vector <Feature *>& conflicts)
{
  vector<Runoff *> sorted_ro;
  sort_ro(sorted_ro);
  Domain::Index iconsrc;
  Consol_peak *consol_src=new Consol_peak[n_runoff()];
  // loop over source runoffs & accumulate consolidations
  for (rROiter roi=source.runoffs.begin(); roi!=source.runoffs.end(); ++roi) {
    if ((*roi).location && bound.contains((*roi).location) &&
	((*roi).name || (*roi).edited)) {
      // source runoff is in bounds, and has name or edited elevation
      iconsrc=assoc_ro((*roi).location,assoc,sorted_ro);
      if (iconsrc!=Domain::undef) {
	// found one. update it into the consolidated source list.
	consol_src[iconsrc]+=(*roi);
      }
      else orphans.push_back(&*roi); // not found.  add to orphan list.
    }
  }

  // now transfer consolidated source to destination
  for (iconsrc=0; iconsrc<runoffs.size(); ++iconsrc)
    consol_src[iconsrc].update(runoffs[iconsrc],conflicts);

  delete[] consol_src;
}

void Divide_tree_map::xfer_featr_info(Divide_tree& source, const Rectangl& bound,
	const Index_map& domap, const Elev_map& elev,
	const Assoc_info& assoc, Sadl_src sadl_src,
	vector<Feature *>& orphan_peak, vector<Feature *>& orphan_sadl,
	vector<Feature *>& orphan_ro, vector<Feature *>& orphan_bs,
	vector<Feature *>& conflict_peak, vector<Feature *>& conflict_sadl,
	vector<Feature *>& conflict_ro, vector<Feature *>& conflict_bs,
	bool use_status)
{
  Elevation *pk_prom=0,*sadl_prom=0;
  Domain::Index *peak_assoc=new Domain::Index[source.doms.size()];
  const Grid grid(elev);
  Bool_map mark(elev);
  vector<Domain *> sorted_peaks;
  mark.fill(false);
  if (assoc.elev_prom) {
    pk_prom=new Elevation[doms.size()];
    sadl_prom=new Elevation[doms.size()];
  }

  // transfer peak info
  if (assoc.pk_dest)
    xfer_peak_info(source,bound,assoc,pk_prom,peak_assoc,domap,elev,
		   mark,grid,sorted_peaks,orphan_peak,conflict_peak);
  // transfer saddle info
  xfer_sadl_info(source,bound,assoc,sadl_src,sadl_prom,pk_prom,peak_assoc,
		 domap,elev,mark,grid,sorted_peaks,
		 orphan_sadl,conflict_sadl,orphan_bs,conflict_bs,
		 use_status);
  // transfer runoff info
  xfer_ro_info(source,bound,assoc,orphan_ro,conflict_ro);

  delete[] pk_prom;
  delete[] sadl_prom;
  delete[] peak_assoc;
}

#endif // def io_support


// Regression tool operations

#if 0

unsigned Statistic::Nred() const
{
  unsigned N=points.size();
  switch (methods.stat_model) {
  case Stat_methods::REGRESS: N-=2; break;
  case Stat_methods::BIAS: N-=1; break;
  default: assert(0);
  }
  return N;
}

double Statistic::mahal(const Statistic& a) const
{
  if (sigma2_res<0 || a.sigma2_res<0) return -1;
  if (methods.stat_model==Stat_methods::REGRESS &&
      a.methods.stat_model==Stat_methods::REGRESS) {
    // full regression on both
    double dCbb=Cbb+a.Cbb,dCab=-ax*Cbb-a.ax*a.Cbb,dCaa=Caa+a.Caa,
      db=beta-a.beta,da=bias-ax*beta-a.bias+a.ax*a.beta;
    return sqrt((da*da*dCbb+db*db*dCaa-2*da*db*dCab)/(dCaa*dCbb-dCab*dCab));
  }
  return fabs(bias-a.bias)/sqrt(sigma2_bias+a.sigma2_bias);
}

double Statistic::numerr(const Data_point& dp) const
{
  double err=beta*(dp.x-ax)+bias-dp.y;
  //TRACE("%8.5g",err*err);
  return err*err;
}

double Statistic::variance0(const Data_point& dp) const
{
  //TRACE("%8.5g",gamma+lambda*dp.T+dp.mu);
  double v=methods.measr_err?dp.mu:0;
  for (unsigned i=0; i<methods.nconv_inuse; ++i) v+=lambda[i]*dp.T[i];
  return v;
  //return gamma+lambda*dp.T+dp.mu;
}

/* Determine optimum alpha and beta
   if (have_gl), use the paramaterized variance model,
     with the input values of gamma and lambda.
   otherwise, use the fixed variance model
     (where gamma and lambda are ignored). */
void Statistic::summarize_ab(bool have_gl)
{
  double Sw=0,Sx=0,Sy=0,w=1;
  beta=bias=ax=0;
  Cbb=Caa=sigma2_raw=sigma2_res=sigma2_bias=-1;
  vector<Data_point>::const_iterator dpi;
  for (dpi=points.begin(); dpi!=points.end(); ++dpi) {
    if (have_gl) w=1/variance(*dpi);
    Sw+=w;
    Sy+=w*(*dpi).y;
    if (methods.stat_model>=Stat_methods::REGRESS) Sx+=w*(*dpi).x;
  }
  if (Sw==0) return; // no data
  double Swi=1/Sw,dx,dy,Sxx=0,Sxy=0,Syy=0,Sxxi,unbiased_res;
  ax=Sx*Swi;
  bias=Sy*Swi;
  for (dpi=points.begin(); dpi!=points.end(); ++dpi) {
    if (have_gl) w=1/variance(*dpi);
    dy=(*dpi).y-bias;
    Syy+=dy*dy*w;
    if (methods.stat_model>=Stat_methods::REGRESS) {
      dx=(*dpi).x-ax;
      Sxx+=dx*dx*w;
      Sxy+=dx*dy*w;
    }
  }
  sigma2_raw=(Syy+Sy*bias)*Swi;
  switch (methods.stat_model) {
  case Stat_methods::BIAS:
    if (points.size()<=1) return; // no redundancy in data
    unbiased_res=Syy/(points.size()-1);
    break;
  case Stat_methods::REGRESS:
    if (Sxx==0) return; // insufficient or ill-conditioned data
    Sxxi=1/Sxx;
    beta=Sxy*Sxxi;
    if (points.size()<=2) return; // no redundancy in data
    // redundancy in data; compute variance estimates parameters
    Cbb=Sxxi;
    Caa=Swi+ax*ax*Sxxi;
    unbiased_res=(Syy-beta*Sxy)/(points.size()-2);
    if (!have_gl) {
      Cbb*=unbiased_res;
      Caa*=unbiased_res;
    }
    break;
  case Stat_methods::NONE:
    return;
  default:
    assert(0);
  }
  if (have_gl) {
    sigma2_res=points.size()*Swi;
    sigma2_bias=Swi;
    TRACE("Variance check: %.3f\n",unbiased_res); // should be very close to 1
  }
  else {
    sigma2_res=unbiased_res;
    sigma2_bias=sigma2_res*Swi;
  }
}

/* compute chi2 for the trial values of the parameters
   meaningful only with parameterized variance
   deriv is dchi2/dgamma */
double Statistic::trial_chi2(double& deriv) const
{
  double chi2=0,w,err;
  deriv=0;
  vector<Data_point>::const_iterator dpi;
  for (dpi=points.begin(); dpi!=points.end(); ++dpi) {
    err=numerr(*dpi);
    //TRACE("/");
    w=1/variance(*dpi);
    chi2+=err*w;
    //deriv-=err*w*w*(lamderiv?(*dpi).T:1);
    deriv-=err*w*w;
  }
  return chi2;
}

// converge gamma at fixed alpha and beta, and lambda.
// return # of iterations required.
unsigned Statistic::converge_g()
{
  unsigned N=Nred(),niter=0;
  assert(N>0);
  bump_gamma();
  double deriv,chi2=trial_chi2(deriv)-N;
  gamma-=chi2/deriv;
  bump_gamma();
  chi2=trial_chi2(deriv)-N;
  while (chi2>0) {
    ++niter;
    gamma-=chi2/deriv;
    chi2=trial_chi2(deriv)-N;
  }
  return niter;
}

// bump gamma up to its minimum, if necessary, to avoid singularities
void Statistic::bump_gamma()
{
  vector<Data_point>::const_iterator dpi;
  double gam,err,Ni=1.0/Nred();
  for (dpi=points.begin(); dpi!=points.end(); ++dpi) {
    err=numerr(*dpi);
    gam=err*Ni-variance(*dpi);
    if (gam>gamma) gamma=gam;
  }
}

// converge all 4 parameters, using their input values as starting parameters
bool Statistic::converge_abgl()
{
  if (Nred()==0) return false;
  unsigned niter=0;
  int n;
  do {
    ++niter;
    if (niter>=50) {
      TRACE("No all-parameter convergence in %d iterations\n",niter);
      return false;
    }
    TRACE("G=%8.2f",gamma);
    for (unsigned j=0; j<methods.nconv_inuse; ++j) {
      TRACE(" L%d=%8.6f",j+1,lambda[j]);
    }
    TRACE(" S=%8.2f\n",sigma2_res);
    summarize_ab(true);
  } while (n=converge_gl(),n>1);
  if (n>=0) TRACE("all parameters converged in %d iterations\n",niter);
  return n>=0;
}

void Statistic::build_deriv(double& g, double df[], double dg[],
			    double ddf[][MAX_CONV+1], double ddg[][MAX_CONV+1]) const
{
  unsigned j,k;
  const unsigned& nconv=methods.nconv_inuse;
  double w,w2,w3,err,Tj,Tk;

  g=0;
  for (j=0; j<=nconv; ++j) {
    df[j]=dg[j]=0;
    for (k=0; k<=nconv; ++k) {
      ddf[j][k]=ddg[j][k]=0;
    }
  }
  for (vector<Data_point>::const_iterator dpi=points.begin();
       dpi!=points.end(); ++dpi) {
    w=1/variance(*dpi);
    assert(w>0);
    w2=w*w;
    err=numerr(*dpi);
    w3=2*err*w*w2;
    g+=err*w;
    df[0]+=w;
    dg[0]-=err*w2;
    ddf[0][0]-=w2;
    ddg[0][0]+=w3;
    for (j=1; j<=nconv; ++j) {
      Tj=(*dpi).T[j-1];
      df[j]+=w*Tj;
      dg[j]-=err*w2*Tj;
      ddf[j][0]-=w2*Tj;
      ddg[j][0]+=w3*Tj;
      for (k=1; k<=j; ++k) {
	Tk=(*dpi).T[k-1];
	ddf[j][k]-=w2*Tj*Tk;
	ddg[j][k]+=w3*Tj*Tk;
      }
    }
  }
  // fill out upper triangle of symmetric derivative matrices
  for (j=1; j<=nconv; ++j) {
    for (k=0; k<j; ++k) {
      ddf[k][j]=ddf[j][k];
      ddg[k][j]=ddg[j][k];
    }
  }
}

static void build_coeff(double A[MAX_CONV+1][MAX_CONV+1],
			unsigned xlate_param[], unsigned naparm,
			double df[], double dg[],
			double ddf[][MAX_CONV+1], double ddg[][MAX_CONV+1])
{
  unsigned j,k,ip=xlate_param[0],jp,kp;
  for (k=0; k<naparm; ++k) {
    kp=xlate_param[k];
    A[0][k]=dg[kp];
    for (j=1; j<naparm; ++j) {
      jp=xlate_param[j];
      A[j][k]=ddf[ip][kp]*dg[jp]+df[ip]*ddg[jp][kp]
	     -ddg[ip][kp]*df[jp]-dg[ip]*ddf[jp][kp];
    }
  }
}

static void trace_matrix(double A[][MAX_CONV+1], unsigned n)
{
  unsigned i,j;
  for (i=0; i<n; ++i) {
    for (j=0; j<n; ++j) {
      TRACE(" %8.4g",A[i][j]);
    }
    TRACE("\n");
  }
}

static void trace_eigval(double A[][MAX_CONV+1], unsigned n)
{
  if (n!=2) return;
  double sd=(A[0][0]+A[1][1])/2,dd=(A[0][0]-A[1][1])/2,
    D=sqrt(dd*dd+A[0][1]*A[1][0])*(sd>=0?1:-1),q=sd+D;
  TRACE("Eigenvalues: %8.4f %8.4f\n",q,(A[0][0]*A[1][1]-A[0][1]*A[1][0])/q);
}

void Statistic::trace_conv_hess() const
{
  double g,df[MAX_CONV+1],dg[MAX_CONV+1],
    ddf[MAX_CONV+1][MAX_CONV+1],ddg[MAX_CONV+1][MAX_CONV+1],
    A[MAX_CONV+1][MAX_CONV+1],AA[MAX_CONV+1][MAX_CONV+1];
  unsigned i,j,k,naparm=methods.nconv_inuse+1,xlate_param[MAX_CONV+1];

  build_deriv(g,df,dg,ddf,ddg);

  for (i=0; i<naparm; ++i)
    xlate_param[i]=i;
  build_coeff(A,xlate_param,naparm,df,dg,ddf,ddg);
  for (i=0; i<naparm; ++i) {
    for (j=0; j<naparm; ++j) {
      if (j==0) {
	A[i][j]*=sigma2_res;
	ddf[i][j]*=sigma2_res;
	ddg[i][j]*=sigma2_res;
      }
      if (i==0) {
	ddf[i][j]*=sigma2_res;
	ddg[i][j]*=sigma2_res;
      }
      else A[i][j]*=sigma2_res;
    }
  }
  TRACE("F matrix:\n");
  trace_matrix(ddf,naparm);
  trace_eigval(ddf,naparm);
  TRACE("G matrix:\n");
  trace_matrix(ddg,naparm);
  trace_eigval(ddg,naparm);
  TRACE("A matrix:\n");
  trace_matrix(A,naparm);
  for (i=0; i<naparm; ++i) {
    for (j=0; j<naparm; ++j) {
      double& x=AA[i][j];
      x=0;
      for (k=0; k<naparm; ++k)
	x+=A[k][i]*A[k][j];
    }
  }
  TRACE("AtA:\n");
  trace_matrix(AA,naparm);
  if (naparm==2) {
    double apd=A[0][0]+A[1][1],amd=A[0][0]-A[1][1],
      bpc=A[0][1]+A[1][0],bmc=A[0][1]-A[1][0],
      D=sqrt(apd*apd+bmc*bmc)+sqrt(amd*amd+bpc*bpc);
    TRACE("Eigenvalues: %8.4f %8.4f\n",D/2,2*(A[0][0]*A[1][1]-A[0][1]*A[1][0])/D);
  }
}

#endif

static bool invert(double A[MAX_REGRESS_PARAM][MAX_REGRESS_PARAM], unsigned n)
{
  assert(n<=MAX_REGRESS_PARAM);
  int i,j,k;
  unsigned perm[MAX_REGRESS_PARAM];
  double x;
#if 0
  double AA[MAX_REGRESS_PARAM][MAX_REGRESS_PARAM];
  for (i=0; i<n; ++i)
    for (j=0; j<n; ++j)
      AA[i][j]=A[i][j];
#endif
  for (i=0; i<n; ++i) {
    // find best pivot
    x=fabs(A[i][i]);
    k=i;
    for (j=i+1; j<n; ++j) {
      if (fabs(A[j][i])>x) {
	x=fabs(A[j][i]);
	k=j;
      }
    }
    if (x==0) return false;
    perm[i]=k;
    if (k!=i) {
      for (j=0; j<n; ++j) {
	x=A[i][j];
	A[i][j]=A[k][j];
	A[k][j]=x;
      }
    }
    x=1/A[i][i];
    for (j=0; j<n; ++j)
      A[i][j]*=x;
    A[i][i]=x;
    for (j=0; j<n; ++j) {
      if (j==i) continue;
      x=A[j][i];
      for (k=0; k<n; ++k) {
	if (k==i) A[j][k]=-x*A[i][i];
	else A[j][k]-=x*A[i][k];
      }
    }
  }
  // undo the column permutation
  for (i=n-1; i>=0; --i) {
    k=perm[i];
    if (k==i) continue;
    for (j=0; j<n; ++j) {
      x=A[j][i];
      A[j][i]=A[j][k];
      A[j][k]=x;
    }
  }
#if 0
  for (i=0; i<n; ++i) {
    for (j=0; j<n; ++j) {
      x=0;
      for (k=0; k<n; ++k)
	x+=A[i][k]*AA[k][j];
      TRACE(" %6.3f",x);
    }
    TRACE("\n");
  }
#endif
  return true;
}

#if 0

// converge gamma AND all lambda's to a constrained minimum
// at fixed alpha and beta.
// return number of iterations required, or -1 if operation failed
int Statistic::converge_gl()
{
  static const double epsilon=100*DBL_EPSILON;
  double g,y,Y,Ylow,df[MAX_CONV+1],dg[MAX_CONV+1],
    ddf[MAX_CONV+1][MAX_CONV+1],ddg[MAX_CONV+1][MAX_CONV+1],
    A[MAX_CONV+1][MAX_CONV+1],B[MAX_CONV+1],X[MAX_CONV+1];
  const unsigned& nconv=methods.nconv_inuse;
  unsigned i,j,k,ip,jp,naparm,xlate_param[MAX_CONV+1];
  int jplow;
  bool convgd;

  converge_g();
  if (gamma<0) gamma=0;

  trace_conv_hess();

  for (int niter=0; niter<50; ++niter) {
    // accumulate pieces of all derivatives necessary to do N-R
    build_deriv(g,df,dg,ddf,ddg);

    B[0]=g-Nred();
    // initialize parameter translation array.
    naparm=nconv+1;
    for (i=0; i<naparm; ++i)
      xlate_param[i]=i;
    do {
      // build RHS for the still-active parameters and check for convergence
      convgd=fabs(B[0])/g<epsilon;
      ip=xlate_param[0];
      //TRACE("RHS: %8.4g",B[0]);
      for (j=1; j<naparm; ++j) {
	jp=xlate_param[j];
	B[j]=df[ip]*dg[jp]-dg[ip]*df[jp];
	//TRACE(" %8.4g",B[j]);
	// check for minimization convergence in the jth coordinate
	if (convgd && fabs(B[j]/(df[ip]*dg[jp]))>epsilon) convgd=false;
      }
      //TRACE("\n");
      if (convgd) {
	TRACE("Gamma & lambdas converged in %d iterations\n",niter);
	return niter;
      }
      // build coefficient matrix for the still-active parameters
      build_coeff(A,xlate_param,naparm,df,dg,ddf,ddg);
      if (!invert(A,naparm)) {
	TRACE("Gamma & lambdas failed to converge due to singular step\n");
	return -1;
      }
      // flip components of step that increase f
      for (j=1; j<naparm; ++j) {
	Y=0;
	for (k=0; k<naparm; ++k)
	  Y+=df[xlate_param[k]]*A[k][j];
	if (Y>0)
	  B[j]*=-1;
      }
      // Calculate the step, and check if it makes any of the parameters negative.
      // Limit the step length accordingly.
      Ylow=1;
      jplow=-1;
      for (j=0; j<naparm; ++j) {
	double& x=X[j];
	x=0;
	for (k=0; k<naparm; ++k)
	  x+=A[j][k]*B[k];
	jp=xlate_param[j];
	y=jp==0?gamma:lambda[jp-1]; // reuse of w
	assert(y>=0);
	if (y==0 && x>0) {
	  // we're trying to step a parameter into negative territory!
	  // Throw it out.
	  jp=xlate_param[j];
	  //if (jp==0) TRACE("Throwing out gamma\n");
	  //else TRACE("Throwing out lambda_%d\n",jp);
	  --naparm;
	  assert(naparm>0);
	  Ylow=0;
	  for (; j<naparm; ++j) xlate_param[j]=xlate_param[j+1];
	  break;
	}
	if (jp>0 && fabs(x)>1) {
	  Y=1/fabs(x);
	  if (Y<Ylow) {
	    jplow=-1;
	    Ylow=Y;
	  }
	}
	if (y<x) {
	  Y=y/x;
	  if (Y<Ylow) {
	    jplow=jp;
	    Ylow=Y;
	  }
	}
      }
    } while (Ylow==0);
    // make the step
    for (j=0; j<naparm; ++j) {
      jp=xlate_param[j];
      if (jp==0) gamma-=Ylow*X[j];
      else lambda[jp-1]-=Ylow*X[j];
    }
    if (jplow>=0) {
      // make sure that a parameter that "hit the 0 wall" really is 0.
      // because of rounding error, it may not be.
      if (jplow==0) gamma=0;
      else lambda[jplow-1]=0;
    }
  }
  TRACE("No convergence for gamma & lambdas in %d iterations\n",niter);
  return -1;
}

/* converge alpha, beta, and gamma
   return true if successful */
bool Statistic::converge_abg()
{
  if (Nred()==0) return false;
  unsigned niter=0;
  do {
    if (++niter>=50) {
      TRACE("No abg convergence in %d iterations\n",niter);
      return false;
    }
    summarize_ab(true);
  } while (converge_g()>1); // note: converge_g() returns # of iterations it needed;
  TRACE("abg converged in %d iterations\n",niter);
  return true;
}

struct GL_minimum {
  double gamma,lambda;
  GL_minimum(double g, double l) : gamma(g),lambda(l) {}
};

void Statistic::summarize()
{
  summarize_ab(false);
  for (unsigned i=0; i<methods.nconv_inuse; ++i) lambda[i]=0;
  if (!methods.measr_err && methods.nconv_inuse==0) {
    gamma=0;
    return;
  }
  gamma=sigma2_res;
  if (methods.nconv_inuse>0) {
    if (converge_abgl()) return;
    methods.nconv_inuse=0;
  }
  if (methods.measr_err) {
    if (converge_abg()) return;
    methods.measr_err=0;
  }
  summarize_ab(false);
}

void Data_point::trace(double alpha, double beta) const
{
  TRACE("%6.4g  %6.4g  %6.4g  %6.5g  %6.4g\n",x,y,alpha+beta*x-y,T,mu);
}

// Get estimated point elevation and, if possible, RQF parameters
bool getElev(const Elev_map& elev, const GridPoint& location,
	     const Elev_methods& elevm, int hessign, bool need_rqf,
	     double& ftr_elev, double& laplacn, double conv[])
{
  ftr_elev=elev_undef;
  bool have_rqf=false;
  double Cxx,Cyy,Cxy,Cx,Cy,C1,Rsym,Ra1,Ra2,D;
  // do we want the RQF for any reason?
  if (need_rqf || elevm.RQF_gp || elevm.rqf_statpt) {
    // Yes. Get it.
    double z[3][3];
    if (getNbrs(elev,location,z)) {
      // We've got all neighbors. Compute RQF coefficients...
      Cxx=(z[0][0]+z[0][1]+z[0][2]+z[2][0]+z[2][1]+z[2][2]
	   -2*(z[1][0]+z[1][1]+z[1][2]))/6;
      Cyy=(z[0][0]+z[1][0]+z[2][0]+z[0][2]+z[1][2]+z[2][2]
	   -2*(z[0][1]+z[1][1]+z[2][1]))/6;
      Cxy=(z[0][0]-z[0][2]-z[2][0]+z[2][2])/8;
      Cx=(z[2][0]-z[0][0]+z[2][1]-z[0][1]+z[2][2]-z[0][2])/6;
      Cy=(z[0][2]-z[0][0]+z[1][2]-z[1][0]+z[2][2]-z[2][0])/6;
      C1=(5*z[1][1]-z[0][0]-z[0][2]-z[2][0]-z[2][2]
	  +2*(z[0][1]+z[2][1]+z[1][0]+z[1][2]))/9;
      // ... and residual components
      Rsym=(z[0][0]+z[0][2]+z[2][0]+z[2][2]
	    -2*(z[0][1]+z[1][0]+z[1][2]+z[2][1])+4*z[1][1])/6;
      Ra1=(z[0][0]-z[0][1]-z[1][0]+z[1][2]+z[2][1]-z[2][2])/sqrt(6);
      Ra2=(z[0][2]-z[0][1]-z[1][2]+z[1][0]+z[2][1]-z[2][0])/sqrt(6);
      laplacn=Cxx+Cyy; // Laplacian
      conv[0]=Cxx*Cxx+2*Cxy*Cxy+Cyy*Cyy; // sum of squares of hessian eigenvalues
      conv[1]=Rsym*Rsym+Ra1*Ra1+Ra2*Ra2; // residual error
      have_rqf=true; // we've got the RQF.
    }
  }
  // Do we want, and might we be able to get, the RQF at the stationary point?
  if ((D=Cxx*Cyy-Cxy*Cxy,D!=0) && have_rqf &&
      elevm.rqf_statpt!=Elev_methods::none) {
    bool good_stat=true;
    // Do we need a good hessian?
    if (elevm.rqf_statpt==Elev_methods::goodhess ||
	elevm.rqf_statpt==Elev_methods::both) {
      // Yes. Check its signature.
      switch (hessign) {
      case -2:
	if (laplacn>=0 || D<=0) good_stat=false;
	break;
      case 0:
	if (D>=0) good_stat=false;
	break;
      case 2:
	if (laplacn<=0 || D<=0) good_stat=false;
	break;
      }
    } // endif (need good hessian)
    if (good_stat) {
      // The hessian is good (or we don't require such to use the stationary point).
      // Now check if the stationary point is in range.
      D=0.5/D;
      double x=(Cxy*Cy-Cx*Cyy)*D,y=(Cxy*Cx-Cy*Cxx)*D;
      if (x*x>1 || y*y>1) {
	// Stationary point is out of range
	if (elevm.rqf_statpt==Elev_methods::inrange || elevm.rqf_statpt==Elev_methods::both) {
	  // We require that the stationary point be within range.
	  // Sorry, can't use it.
	  good_stat=false;
	}
	else {
	  // We can use an out-of-range stationary point. Bump it back to the margin.
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
	}
      }
      if (good_stat) ftr_elev=C1+(Cx*x+Cy*y)/2; // Compute height of stationary point.
    }
  }

  if (ftr_elev==elev_undef) {
    // Stationary point was not used to compute the elevation.
    // Try the RQF at the center point.
    if (elevm.RQF_gp && have_rqf) ftr_elev=C1;
    // Try the raw elevation data
    else if (elevm.elev_data && elev.contains(location))
      ftr_elev=elev[location];
  }

  return have_rqf;
}

#endif

#ifdef io_support

struct Neighbor {
  int xoff,yoff;
};

struct NeighborSet {
  unsigned nnbr;
  const Neighbor *nbrs;
};

static const Neighbor ortho[] = {
  {1,0},{-1,0},{0,1},{0,-1}
};

static const Neighbor diag[] = {
  {1,1},{1,-1},{-1,1},{-1,-1}
};

static const Neighbor ortho2[] = {
  {2,0},{-2,0},{0,2},{0,-2}
};

static const Neighbor knight[] = {
  {2,1},{2,-1},{1,2},{1,-2},{-1,2},{-1,-2},{-2,1},{-2,-1}
};

#define NBR_SET(ns) {sizeof ns/sizeof NeighborSet, ns}

static const NeighborSet nbrsets[] = {
  NBR_SET(ortho),
  NBR_SET(diag),
  NBR_SET(ortho2),
  NBR_SET(knight)
};

unsigned Stat_options::get_nnbreq(unsigned nnbr) const
{
  switch (mns) {
  case MNS_ALL: return nnbr;
  case MNS_ALLBUTONE: return nnbr-1;
  case MNS_HALF: return nnbr/2;
  case MNS_ONE: return 1;
  default: assert(0);
  }
  return 0;
}

unsigned Stat_options::get_nnbrset() const
{
  switch (model) {
  case NONE: case BIAS: return 0;
  case NBREG4: return 1;
  case NBREG8: return 2;
  case NBREG12: return 3;
  case NBREG20: return 4;
  default: assert(0);
  }
  return 0;
}

bool Stat_options::get_nbr_avg(const Elev_map& elev, const GridPoint& location,
			       double nbr_avgs[]) const
{
  if (model==NONE) return false;
  unsigned n,inbr,iset,nnbrset=get_nnbrset();
  Elevation center_elv,nbr_elv,x;
  GridPoint c;
  if (!elev.contains(location)) return false;
  center_elv=elev[location];
  if (center_elv==elev_undef) return false;
  for (iset=0; iset<nnbrset; ++iset) {
    x=n=0;
    const NeighborSet& set=nbrsets[iset];
    for (inbr=0; inbr<set.nnbr; ++inbr) {
      c.x=location.x+set.nbrs[inbr].xoff;
      c.y=location.y+set.nbrs[inbr].yoff;
      if (elev.contains(c)) {
	nbr_elv=elev[c];
	if (nbr_elv!=elev_undef) {
	  x+=nbr_elv-center_elv;
	  ++n;
	}
      }
    }
    if (n<get_nnbreq(set.nnbr)) return false;
    nbr_avgs[iset]=((double)x)/n;
  }
  return true;
}

#if 0

static bool solve(double A[MAX_REGRESS_PARAM][MAX_REGRESS_PARAM],
		  double B[MAX_REGRESS_PARAM], int n)
{
  int i,j,k;
  double x,xi;
  for (i=0; i<n; ++i) {
    // find best pivot
    x=fabs(A[i][i]);
    k=i;
    for (j=i+1; j<n; ++j) {
      if (fabs(A[j][i])>x) {
	k=j;
	x=fabs(A[j][i]);
      }
    }
    if (x==0) return false;
    if (k!=i) {
      for (j=i; j<n; ++j) {
	x=A[i][j];
	A[i][j]=A[k][j];
	A[k][j]=x;
      }
      x=B[i];
      B[i]=B[k];
      B[k]=x;
    }
    // zero the ith column after the ith row
    xi=1/A[i][i];
    for (j=i+1; j<n; ++j) {
      x=-xi*A[j][i];
      for (k=i+1; k<n; ++k)
	A[j][k]+=x*A[i][k];
      B[j]+=x*B[i];
    }
  }
  for (i=n-1; i>=0; --i) {
    B[i]/=A[i][i];
    for (j=0; j<i; ++j)
      B[j]-=A[j][i]*B[i];
  }

  return true;
}

#endif

void MultiRegress::clear(unsigned np)
{
  nparam=np;
  unsigned i,j;
  Sy=Syy=n=0;
  for (i=0; i<np; ++i) {
    Sx[i]=Sxy[i]=0;
    for (j=0; j<np; ++j) Sxx[i][j]=0;
  }
}

void MultiRegress::add_point(double x[], double y)
{
  unsigned i,j;
  if (n>0) {
    double n1=n/(n+1),ni=1/n,dy=y-Sy*ni,dx[MAX_REGRESS_PARAM];
    Syy+=n1*dy*dy;
    for (i=0; i<nparam; ++i) dx[i]=x[i]-Sx[i]*ni;
    for (i=0; i<nparam; ++i) {
      Sxy[i]+=n1*dx[i]*dy;
      for (j=0; j<nparam; ++j) Sxx[i][j]+=n1*dx[i]*dx[j];
    }
  }
  for (i=0; i<nparam; ++i) Sx[i]+=x[i];
  Sy+=y;
  ++n;
}

void MultiRegress::summarize()
{
  double ni=1/n;
  bias=Sy*ni;
  sigma2_raw=Syy*ni+bias*bias;
  if (n<=nparam) {
    sigma2_res=sigma2_bias=-1;
    return;
  }
  unsigned i,j;
  for (i=0; i<nparam; ++i) {
    for (j=0; j<nparam; ++j)
      A[i][j]=Sxx[i][j];
  }
  if (!invert(A,nparam)) {
    sigma2_res=sigma2_bias=-1;
    return;
  }
  for (i=0; i<nparam; ++i) {
    double& x=alpha[i];
    x=0;
    for (j=0; j<nparam; ++j) x+=A[i][j]*Sxy[j];
  }
#if 0
  double x;
  for (i=0; i<nparam; ++i) {
    x=0;
    for (j=0; j<nparam; ++j) x+=Sxx[i][j]*alpha[j];
    TRACE(" %7.4f",x-Sxy[i]);
  }
  TRACE("  .\n");
#endif
  if (n>nparam+1) {
    sigma2_res=Syy;
    for (i=0; i<nparam; ++i)
      sigma2_res-=alpha[i]*Sxy[i];
    //TRACE("%d  %.3f\n",nparam,sigma2_res);
    sigma2_res/=n-(nparam+1);
    sigma2_bias=sigma2_res*ni;
  }
  else sigma2_res=sigma2_bias=-1;
}

// adjust the elevation range of a single unedited feature
// to reflect any model
static void set_error(Feature& featr, const Elev_map& elev, double sigma,
		      const AnalData& data)
{
  if (featr.edited) return;
  double nbr_avgs[MAX_REGRESS_PARAM];
  if (data.options.get_nbr_avg(elev,featr.location,nbr_avgs)) {
    double x=elev[featr.location]-data.regress.bias,ni=1/data.regress.n,var;
    unsigned i,j,n=data.regress.nparam;
    for (i=0; i<n; ++i) {
      nbr_avgs[i]-=data.regress.Sx[i]*ni;
      x-=nbr_avgs[i]*data.regress.alpha[i];
    }
    var=data.regress.sigma2_bias+data.regress.sigma2_res;
    for (i=0; i<n; ++i)
      for (j=0; j<n; ++j)
	var+=nbr_avgs[i]*nbr_avgs[j]*data.regress.A[i][j]*data.regress.sigma2_res;
    sigma*=sqrt(var);
    featr.elev.set(nint(x-sigma),nint(x+sigma));
  }
  else if (data.enable_fixed) {
    featr.set_range(data.fixed_range);
  }
}

// adjust the elevation range of all unedited features on the divide tree
// to reflect the model
void Divide_tree_map::set_error(const Elev_map& elev, double sigma,
		const AnalData& peak_data, const AnalData& sadl_data,
		const Database& db)
{
  Domain::Index i,n;
  n=doms.size();
  for (i=0; i<n; ++i) {
    Domain& d=doms[i];
    if (d.peak.location) {
      if (peak_data.filter.test(d.peak,db))
	::set_error(d.peak,elev,sigma,peak_data);
      if (d.saddle && sadl_data.filter.test(d.saddle,db))
	::set_error(d.saddle,elev,sigma,sadl_data);
    }
  }
  n=runoffs.size();
  for (i=0; i<n; ++i) {
    Feature& f=runoffs[i];
    if (f.location && peak_data.filter.test(f,db))
      ::set_error(f,elev,sigma,peak_data);
  }
  n=bsnsdls.size();
  for (i=0; i<n; ++i) {
    Feature& f=bsnsdls[i];
    if (f.location && sadl_data.filter.test(f,db))
      ::set_error(f,elev,sigma,sadl_data);
  }
}

#endif // def io_support

// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#ifndef _Domain_
#define _Domain_

#pragma warning (disable : 4786)

#include <new>
using std::bad_alloc;
#include <list>
#include <queue>
#include "point.h"
using namespace std;
#include "Rectangle.h"

//#define trace

struct Runoff;
struct Basin_saddle;
struct Domain;
struct Topo_info;
class Consol_peak;
class Consol_sadl;
typedef vector<Domain *>::iterator Dom_iter;
typedef vector<Domain>::iterator rDom_iter;
typedef vector<Domain>::const_iterator const_rDom_iter;
typedef vector<Runoff *>::iterator ROiter;
typedef vector<Runoff *>::const_iterator const_ROiter;
typedef vector<Runoff>::iterator rROiter;
typedef vector<Runoff>::const_iterator const_rROiter;
typedef vector<Basin_saddle *>::iterator BSiter;
typedef vector<Basin_saddle>::iterator rBSiter;
typedef vector<Basin_saddle>::const_iterator const_rBSiter;
typedef vector<Feature *>::iterator Fiter;

typedef void (*Prom_sadl_CB)(Feature&, FT_index,
			     const Elev_intvl&, const Elev_intvl&, bool);
typedef void (*Parent_CB)(Feature&, FT_index);

#if 0
struct LIS {
  Elevation elev;
  bool closed;
  LIS() {reset();}
  void reset() {elev=elev_infty; closed=true;}
  //void unreset() {elev=-elev_infty; closed=false;}
  void update(const Elev_intvl&);
  void update(const Elev_intvl& a, Elev_endpt endpt)
    {if (a.*endpt<elev) elev=a.*endpt;}
  bool test(const Elev_intvl& a) const;
};
#endif

struct HIP_LIS {
  Elevation HIP;
  Elev_intvl LIS;
  HIP_LIS(Elevation hip, Elev_intvl lis) : HIP(hip),LIS(lis) {}
};

class conditional_HIP {
  vector<HIP_LIS> hip_lis;
public:
  conditional_HIP(Elevation init_pk)
    {hip_lis.push_back(HIP_LIS(init_pk,elev_infty));}
  void update(const Elev_intvl&, Elevation);
  Elevation get_HIP(const Elev_intvl&) const;
};

#ifdef io_support
struct Prom_sadl_filter {
  Elevation min_prom;
  FeatureFilter saddle_filter,runoff_filter;
  bool off_sadls,runoffs,basinsadls;
  Prom_sadl_filter() : min_prom(0),runoff_filter(true),
    off_sadls(false),runoffs(false),basinsadls(false) {}
};
#endif

struct Saddle : Feature {
  static bool distance_io; // whether or not flat_sep is being read from a file
  static unsigned short stat_io; // status information in file:
	// 0=none, 1=no river, 2=river
  static bool use_status; // whether or not to use status in elevation comparison
  enum Status {STAT_RIVER,STAT_UNKNOWN,STAT_REAL,STAT_PROM};
  static const char *const status_name[];
  Distance flat_sep;
  unsigned short status; // is it definitely a prominence saddle?
  Saddle() : flat_sep(0),status(STAT_UNKNOWN) {}
  Saddle(const Feature& f) : Feature(f),flat_sep(0),status(STAT_UNKNOWN) {}
  Saddle(const GridPoint& gp, Elevation e, Distance d) :
    Feature(gp,e),flat_sep(d),status(STAT_UNKNOWN) {}
  Status eff_stat(bool) const;
  void transform(float, float, bool);
  void write(FILE *) const throw(file_error);
  void read(FILE *) throw(file_error);
};


struct Domain {
  typedef unsigned long Index;
  enum Sadl_cxn {NO_CXN,AMBIG_CXN,LOW_CXN,GOOD_CXN};
  enum Topology {TOPO_MS,TOPO_LINE,TOPO_CELL,TOPO_ISLAND};
  friend class Divide_tree;

  // output formats
  static const char *format,*prom_format,*first_prom_format,*HP_format,
    *id_format,*id_field_format,*id_peak_hdr,*id_saddle_hdr,
    *prom_hdr,*usable_peak_hdr,*usable_saddle_hdr,
    *onmap_parent_string,*offmap_parent_string,*runoff_string,*offmap_hdr;
  static const Index undef;

  // attributes
  Feature peak;
  Saddle saddle;
  Domain *primary_nbr;
  vector<Domain *> neighbors;
  vector<Runoff *> runoffs;
  vector<Basin_saddle *> bsnsdls;

  // operations
  void operator=(const Domain&);
  Domain *saddle_owner(Domain *a) {return primary_nbr==a?this:this==a->primary_nbr?a:0;}
  const Domain *saddle_owner(const Domain *a) const
    {return primary_nbr==a?this:this==a->primary_nbr?a:0;}
  Domain *common_nbr(Domain *);
  void copy_nbrs(const Domain&);
  void clear_neighbors() {neighbors.clear();}
  void clear_runoffs() {runoffs.clear();}
  void clear_bsnsdls() {bsnsdls.clear();}
  void clear_all() {clear_neighbors(); clear_runoffs(); clear_bsnsdls();}
  void add_nbr(Domain *nbr);
  void add_nbr(Runoff *);
  void add_nbr(Basin_saddle *bs);
  void remove_nbr(const Domain *);
  void remove_nbr(const Runoff *);
  void remove_nbr(const Basin_saddle *);
  void sort_nbrs();
  Domain() : primary_nbr(0) {}
  Domain(const Topo_point& p) : peak(p),primary_nbr(0) {}

  // information
  const Domain *other_peak(const Domain *sadl_dom) const
    {return sadl_dom==this?primary_nbr:this;}
  Domain *other_peak(Domain *sadl_dom)
    {return sadl_dom==this?primary_nbr:this;}
  Elevation sadl_walk_inv_risk(Domain *, bool);
  Elevation SWIR_offense(Domain *, bool);
  Elevation SWIR_offense(Runoff *, bool);
  Elevation SWIR_offense(Basin_saddle *, bool);
  Elevation SWIR_offense(Elev_intvl, bool, Domain *nbr=0);
  Domain *high_nbr_saddle(int);
  Elevation extreme_pk_prom(Elev_endpt);
  Elev_intvl peak_prom();
  Elevation offmap_pk_prom();
  Elevation extreme_sadl_prom(Elevation, Elev_endpt);
  Elevation extreme_sadl_prom(Elev_endpt endpt)
    {return extreme_sadl_prom(saddle.elev.high,endpt);}
  Elevation max_sadl_prom();
  Elev_intvl sadl_promx(Elevation);
  Elev_intvl sadl_prom() {return sadl_promx(saddle.elev.high);}
  Elevation offmap_sadl_prom(Elevation);
  Elevation offmap_sadl_prom() {return offmap_sadl_prom(saddle.elev.high);}
  void set_island(bool,conditional_HIP&);
  void set_edge_nbr_LIS() const;
  void set_edge_LIS();
  Domain *ancestor(unsigned); // find the nth ancestor of a peak
#ifdef io_support
  void list_prom_runoffs(Prom_sadl_CB, const conditional_HIP&, Elevation,
			 const Prom_sadl_filter&, const Database&);
  void list_prom_sadls(Prom_sadl_CB, const Prom_sadl_filter&, const Database&);
  void list_parnt_runoffs(const Elev_intvl&, Parent_CB, bool) const;
  void list_parents(Domain&, const Elev_intvl&, const Elev_intvl&,
		    Domain::Topology, Parent_CB, bool);
  bool list_line_parent(const Elev_intvl&, Parent_CB, bool);
  void list_cell_parent(Elevation, const Elev_intvl&, Parent_CB, bool);
  void list_island_parent(const Elev_intvl&, Parent_CB, bool);
#endif

  // Divide tree manipulation
  void make_patriarch();
  Domain *patriarch();
  void change_nbr(Domain *, Domain *);
  void low_saddle_leg(Saddle *&, const Domain *, int, Elev_endpt, bool);
  static void connect(Saddle&, Domain *, Domain *, Basin_saddle&, bool, bool);
  static void connect_nbr(const Basin_saddle&, Domain *, Domain *);
  static Domain *common_ancestor(Domain *, Domain *);
  static unsigned length(const Domain *, const Domain *);
  static Domain *FromPeak(Feature *);
  static const Domain *FromPeak(const Feature *);
  static Domain *FromSadl(Feature *);
  static const Domain *FromSadl(const Feature *);
  void cut(Domain *);
  void remove(Domain *d=0, bool=false);
  static void remove_river(Domain *, Domain *);
  static Sadl_cxn SWI_switch_cxn(Domain *, Domain *, Domain *&,
				 short int, Elev_endpt, Elev_endpt);
  void SWI_switch(Domain&, const Domain *);
  void reconnect(Domain&, const Domain *);

  // binary I/O
  void read(FILE *) throw(file_error);
  void write(FILE *) const throw(file_error);
  // ascii I/O
#ifdef io_support
  void dump(FILE *) const;
  //void print(FILE *f, bool=false, bool=false, bool=false);
  //void print(bool oms=false, bool ro=false, bool bs=false)
    //{print(stdout,oms,ro,bs);}
  //static void print_prom(FILE *, const Feature&, Index,
	//const Elev_intvl&, const Elev_intvl&, const char *);
  static void print_header(FILE *f=stdout);
  static void print_prom_header(FILE *f=stdout);
#endif

  class Neighbor_iter {
    void operator++(int);
    void check_skip() {while (*this && **this==skip) ++current;}
  protected:
    const Domain *skip;
    Domain *center;
    Dom_iter current;
  public:
    Neighbor_iter(Domain& c, const Domain *s=0) {reset(c,s);}
    Neighbor_iter(Neighbor_iter *ni) {reset(***ni,ni->center);}
    Domain *operator*() const {return *current;}
    Neighbor_iter& operator++() {++current; check_skip(); return *this;}
    operator bool() const {return current!=center->neighbors.end();}
    Domain *saddle_owner() const
      {return **this==center->primary_nbr?center:**this;}
    Domain *get_center() const {return center;}
    void reset() {current=center->neighbors.begin(); check_skip();}
    void reset(const Domain *s) {skip=s; reset();}
    void reset(Domain& c, const Domain *s=0) {center=&c; reset(s);}
#ifdef io_support
    void dump(FILE *f=stdout);
#endif
  };

  struct Sadl_prom {
    Elevation onmap,offmap,cr_onmap,cr_offmap,rr_onmap;
    bool rr_offmap;
  };
  // for icon sizes
  mutable struct {
    Elevation peak,pk_omp;	// peak prom
    Sadl_prom saddle;		// saddle proms
    // for runoff prominence
    const Domain *island_parent;
    Elevation sadl_elev;
  } prom;
  // for ridge line weight
  mutable struct {
    Elevation prom,rot_prom,
      cycle_prom,cycle_offmap,		// when basin saddle ridges are shown
      line_child,cycle_line_child;	// when line topology is shown
    bool offmap,rot_offmap;
  } ridge;
  // hierarchical topology (used for cell boundary weight)
  mutable struct {
    Domain *line,*cell,*island,*saddle,*child;
  } parent;
  void clear_prom_inv() const;
  void clear_sadl_inv() const;
  Domain *parent_with_prom(Elevation); // 1st parent or self with minimum prom
  Domain *prom_parent(); // 1st parent with more prom than child
  Domain *get_parent(Topology, Elevation=-elev_infty); // parent of given type
  Elevation peak_topo_prom() const
    {return peak.elev.high-parent.saddle->saddle.elev.low;}
  Elevation sadl_topo_prom() const
    {return parent.child->peak.elev.high-saddle.elev.low;}
  bool is_ancestor(Domain *, Topology) const;

  mutable struct Explorer_support {
    // explorer traversal
    Domain *root_nbr;
    // prominence information
    Elev_intvl root_LIS,dpar_LIS,ppar_LIS,edge_LIS;
    Elevation pHIP; // peripheral HIP
  } xplr_sup;
  void transmit_LIS(Elev_intvl Explorer_support::*) const;
private:
  static bool first_prom;
  void update_omp(const conditional_HIP&, Elevation&) const;
};

struct Runoff : Saddle {
  typedef unsigned short IQ;
  typedef Domain::Index Index;
  static const char *format,*IQ_format,*id_ro_hdr,*IQ_hdr;
  Domain *peak;
  const Domain mutable *island;
  IQ interior_quads;
  Runoff() : peak(0),interior_quads(0) {}
  Runoff(const Topo_point& tp, Domain *a, IQ iq) :
    Saddle(tp),peak(a),interior_quads(iq) {}
  Runoff(const Feature& ftr, Domain *a, IQ iq) :
    Saddle(ftr),peak(a),interior_quads(iq) {}
  bool operator<(const Runoff& c) const {return location<c.location;}
  bool operator>(const Runoff& c) const {return location>c.location;}
  void reconnect(Domain *);
  void SWI_switch(Domain *);
  void splice(Runoff&, Basin_saddle&);
  void clear_inv(bool=true);
  void remove();
  void read(FILE *f) throw (file_error);
  void write(FILE *f) const throw (file_error);
#ifdef io_support
  void print(FILE *f=stdout) const;
  void dump(FILE *) const;
  static void print_header(FILE *f=stdout);
#endif
};


struct Basin_saddle : Saddle {
  typedef Domain::Index Index;
  typedef Domain::Neighbor_iter Neighbor_iter;
  static const char *format,*id_bs_hdr,*id_peak1_hdr,*id_peak2_hdr,
    *walk_risk_hdr,*WRO_hdr,*cycle_format,*cycle_hdr,*WS_string;
  Domain *peak1,*peak2;
  Domain *common_ancestor;
  Index cycle;
  mutable Domain::Sadl_prom prom;
  struct Cycle_walk_info {
    Elevation low1,low2,high1,high2;
    const Domain *low_src,*high_src;
  };
  Basin_saddle() : peak1(0),peak2(0),common_ancestor(0),cycle(0) {}
  Basin_saddle(const Saddle& s) :
    Saddle(s),peak1(0),peak2(0),common_ancestor(0),cycle(0) {}
  Domain *other_peak(const Domain *sadl_dom) const
    {return sadl_dom==peak1?peak2:peak1;}
  Domain::Sadl_cxn SWI_switch_cxn(Domain *, Domain *&, Domain *&,
	unsigned short&, short int, Elev_endpt, Elev_endpt);
  static short compare_fused_path(const Basin_saddle&, const Basin_saddle&,
	unsigned short, unsigned short, Elev_intvl[2][2], Elev_intvl,
	bool, short int, Elev_endpt, Elev_endpt);
  static Domain::Sadl_cxn
    SWI_switch_cxn(Basin_saddle&, Basin_saddle&, Domain *&, Domain *&,
		   short int, Elev_endpt, Elev_endpt);
  void get_walk_info(Cycle_walk_info&, short int) const;
  Elevation walk_risk(const Cycle_walk_info&) const;
  Elevation WR_offense(const Cycle_walk_info&) const;
  Elevation walk_risk(const Cycle_walk_info&, const Domain *) const;
  Elevation WR_offense(const Cycle_walk_info&, const Domain *) const;
  Domain *cycle_min(int, Elev_endpt);
  unsigned short high_side(const Domain *, short int, Elev_endpt, Elev_endpt) const;
  unsigned short high_side(const Domain *, const Domain *, short int, Elev_endpt, Elev_endpt) const;
  void set_cycle_info();
  void set_cycle_length();
  unsigned short on_cycle(const Domain *) const;
  Domain *cycle_jct(Domain *, Domain *&, Domain *&);
  Domain *cycle_jct(Domain *d)
    {Domain *ca1,*ca2; return cycle_jct(d,ca1,ca2);}
  Domain *hit(const Domain *, short int, Elev_endpt, Elev_endpt) const;
  void low_cycle(const Domain *, const Domain *,
		 short int, Elev_intvl&, Elev_intvl&, Elev_intvl&);
  bool rotate(Domain *);
  bool rotate()
    {Domain *cm=cycle_min(1,HI_END); return cm ? rotate(cm) : false;}
  void reconnect(Domain *, Domain *);
  void clear_inv(bool=true);
  void remove();
  bool can_rotate(const Saddle&) const;
  bool can_rotate() const;
  Elevation extreme_prom(Elev_endpt) const;
  Elev_intvl get_prom() const;
  Elevation offmap_prom() const;
  void set_prom(Topo_info[], const Domain[], unsigned[]);
  void read(FILE *f) throw(file_error);
  void write(FILE *f) const throw(file_error);
#ifdef io_support
  void print(const Cycle_walk_info&, short int, FILE *f=stdout) const;
  void dump(FILE *) const;
  static void print_header(FILE *f=stdout);
#endif

  class Cycle_iter {
    const Basin_saddle* bs;
    Domain *current,*last_start;
    void operator++(int);
    void restart();
  public:
    Cycle_iter(const Basin_saddle& bs_) {reset(&bs_);}
    Domain *operator*() const {return current;}
    void operator++() {current=current->primary_nbr; restart();}
    inline operator bool() const;
    void reset() {last_start=current=bs->peak1; restart();}
    void reset(const Basin_saddle *bs_) {bs=bs_; reset();}
  };

private:
  void extreme_prom_leg(Elevation&, Domain *, Elev_endpt) const;
  void offmap_prom_leg(Elevation&, Domain *) const;
  void inc_prom_sadl(const Domain&);
  Domain::Index island_peak(const Domain *, Topo_info[], const Domain[]);
  Domain::Index set_prom_long_leg(Domain::Index, unsigned, const Domain[],
				  const Topo_info[], const unsigned[]);
  void set_prom_leg(const Domain *);
  void set_cycle_prom_leg(const Domain *);
};

inline Basin_saddle::Cycle_iter::operator bool() const
{
  return bs->common_ancestor!=0 &&
    (current!=bs->common_ancestor || last_start!=bs->peak2);
}

// used to keep track of rearranged topology
struct Topo_info {
  Domain::Index parnt,sadl_dom;
};

// given an offmap bool and a cell rotation bool,
// this array tells which cell prominence member to get
extern const Elevation Domain::Sadl_prom::*sadl_prom_types[2][2];


// defines boundary point for explored region
class Boundary_saddle {
  Saddle *saddle; // boundary saddle
  Domain *domain; // domain just outside boundary saddle
  Elevation bdy_elev; // boundary saddle elevation for quick reference
  Elevation bdy_elev_2() const // BSE of other endpoint
    {return saddle->elev.high+saddle->elev.low-bdy_elev;}
public:
  Boundary_saddle(Domain *, Domain *, Elev_endpt);
  bool operator<(const Boundary_saddle& a) const
    {return bdy_elev<a.bdy_elev ||
	    bdy_elev==a.bdy_elev && bdy_elev_2()<a.bdy_elev_2();}
  Domain *get_domain() const {return domain;}
  Saddle *get_saddle() const {return saddle;}
};

// this class uses a priority queue, based on saddle elevation, to walk a divide tree.
class Explorer {
  priority_queue<Boundary_saddle> front;
  void serve(Domain *);
  Elev_endpt endpt;
public:
  Explorer(Domain *, Elev_endpt);
  Explorer(Domain *, Domain *, Elev_endpt);
  void reset(Domain *);
  void reset(Domain *, Domain *);
  void reset(Domain *start, Domain *root, Elev_endpt e)
    {endpt=e; reset(start,root);}
  Domain& operator*() const {return *front.top().get_domain();}
  Saddle& saddle() const {return *front.top().get_saddle();}
  void use(Elev_intvl&, Elev_intvl *) const; // only for prom sadl list support
  void operator++();
  void stop_branch();
  operator bool() const {return !front.empty();}
};


#if 0
// used to record runoffs on either side of edge effect saddle
// so that boundary saddles can be identified
struct EESnbrs {
  GridPoint sadl,ro1,ro2;
  EESnbrs() {}
  EESnbrs(const GridPoint&, const GridPoint&, const GridPoint&);
  bool operator<(const EESnbrs& a) const {return sadl<a.sadl;}
  void read(FILE *f) throw(file_error);
  void write(FILE *f) const throw(file_error);
};
#endif

class Divide_tree {
  friend struct Divide_tree_map;
  vector<Domain> doms;
  vector<Runoff> runoffs;
  vector<Basin_saddle> bsnsdls;
  //vector<EESnbrs> ees_nbrs;
  Divide_tree(const Divide_tree&);
  Divide_tree& operator=(const Divide_tree&);
  void xlate_all_doms(Domain::Index [], Domain::Index);
  void xlate_all_bs(Domain::Index [], Domain::Index);
  void expand_nbs(Domain::Index);
  void expand_nro(Domain::Index);
  static void dom_range(const GridPoint&, const vector<Domain *>&, Feature Domain::*,
			Distance, Domain::Index&, Domain::Index&);
  static void bs_range(const GridPoint&, const vector<Basin_saddle *>&,
		       Distance, Domain::Index&, Domain::Index&);
  static void runoff_range(const GridPoint&, const vector<Runoff *>&,
			   Distance, Domain::Index&, Domain::Index&);
public:
  typedef Domain::Index Index;
  static Divide_tree *reading;
  static const Divide_tree *writing;
  static Domain::Index
    search_dom(const GridPoint&, const vector<Domain *>&, Feature Domain::*);
  static Domain::Index
    search_ro(const GridPoint&, const vector<Runoff *>&);
  static Domain::Index
    search_bs(const GridPoint&, const vector<Basin_saddle *>&);

  Divide_tree() {}

  // information
  Index n_dom() const {return doms.size();}
  Index n_runoff() const {return runoffs.size();}
  Index n_bsnsdl() const {return bsnsdls.size();}
  Index ndom_inuse() const;
  Index nrunoff_inuse() const;
  Index nbsnsdl_inuse() const;
  bool valid_peak(Index idom) const
    {return idom<doms.size() && doms[idom].peak.location;}
  void area(Rectangl&, Rectangl&) const; // find allocated and defined rectangles
  void div_drg_count(Index&, Index&) const;
  bool check_prom_sadl() const;
  void get_pat_steps(Domain *, unsigned *, Domain **) const; // # of steps to patriarch
  Elevation min_range() const; // find the minimum elev range in any unedited feature
  void uninvert_peak(Index, Elev_endpt, Topo_info[]) const;
  void uninvert_peak_ip(Index, Topo_info[]) const;
  void uninvert_peaks(Elev_endpt, Topo_info[]) const;
  void uninvert_saddle(Index, Elev_endpt, Topo_info[]) const;
  void uninvert_saddle_ip(Index, Topo_info[]) const;
  void uninvert_saddles(Elev_endpt, Topo_info[]) const;
  void uninvert_saddles_ip(Topo_info[]) const;
  Topo_info *init_topo() const;
  Topo_info *get_island_tree(Elev_endpt) const;
  Topo_info *get_ipit() const;
  void get_line_tree(Topo_info[], const vector<Index>&, Index[],
		     Elev_endpt, Elevation Domain::Sadl_prom::*);
  void set_high_sadl_prom(const vector<Index>&, Index[]);
  void set_offmap_sadl_prom(const vector<Index>&, Index[]);
  Elevation max_peak_prom(Index, const Topo_info[]) const;
  Elevation *min_peak_prom(const Topo_info[]) const;
  void assoc_peak_prom(const Topo_info[], Elevation[]) const;

  // feature component and index information
  Domain& operator[](Index i) {assert(i<doms.size()); return doms[i];}
  const Domain& operator[](Index i) const
    {assert(i<doms.size()); return doms[i];}
  Runoff& runoff(Index i) {assert(i<runoffs.size()); return runoffs[i];}
  const Runoff& runoff(Index i) const
    {assert(i<runoffs.size()); return runoffs[i];}
  Basin_saddle& bsnsdl(Index i) {assert(i<bsnsdls.size()); return bsnsdls[i];}
  const Basin_saddle& bsnsdl(Index i) const
    {assert(i<bsnsdls.size()); return bsnsdls[i];}
  Index index(const Domain *d) const {return d?d-&doms.front():Domain::undef;}
  Index saddle_index(const Feature& ftr) const
    {return (Domain *)&ftr-(Domain *)&(doms.begin()->saddle);}
  Index index(const Runoff& ro) const {return &ro-&runoffs.front();}
  Index index(const Basin_saddle& bs) const {return &bs-&bsnsdls.front();}
  Domain *address(Index i) {return i!=Domain::undef&&(long)i>=0?&doms[i]:0;}
  const Domain *address(Index i) const
    {return i!=Domain::undef&&(long)i>=0?&doms[i]:0;}

  // operations
  void copy(const Divide_tree&) throw(bad_alloc);
  void set_neighbors();
  void set_runoffs();
  void set_bsnsdls();
  void clear();
  void clear_doms();
  void clear_runoffs();
  void clear_bsnsdls();
  void set_cycle_info();
  const Feature *transform(float, float, bool, bool, bool, bool, bool);
  void change_ca(Domain *, Domain *);
  void xlate_dom(Domain *&, Domain *, const Index *) const;
  void xlate_dom(const Domain *&, const Domain *, const Index *) const;
  void clear_runoff_inv();
  void clear_bsnsdl_inv();
  void purge_domains(Elevation, bool, bool, bool, bool, bool, Index **);
  void purge_bs(Elevation, bool, bool, bool, bool, bool);
  void merge(const Divide_tree **, unsigned);
  void merge(const Divide_tree&);
  void xlate_address(const Domain *&, const Divide_tree&) const;
  void xlate_address(const Domain *&, const Domain *) const;
  void add_bs(const Basin_saddle&);
  Runoff *add_ro(const Runoff&);
  void sort_doms(Feature Domain::*, vector<Domain *>&);
  void sort_bs(vector<Basin_saddle *>&);
  void sort_ro(vector<Runoff *>&);
  bool splice(rROiter&);
  bool splice(Runoff *);
  void splice(Distance=0);
  void splice(Runoff **, unsigned);
  void rotate();
  void set_sadl_stat_unknown();
  void set_sadl_stat(Saddle::Status);
  void update_peak_prom_info();
  void update_sadl_prom_info(bool);
  void update_runoff_prom_info();
  void update_ridge_info(bool);

  // I/O
  void read(FILE *) throw(file_error,bad_alloc);
  void write(FILE *) const throw(file_error);
  void read(const char *) throw(file_error,bad_alloc);
  void write(const char *) const throw(file_error);
  void dump(FILE *) const;
  bool dump(const char *) const;

  // maintenance
  void compress_dom();
  void compress_ro();
  void compress_bs();
  void compress_all() {compress_dom(); compress_ro(); compress_bs();}
  void sort_doms();
  void sort_nbrs();
  void sort_bs();
  void sort_all() {sort_doms(); sort_nbrs(); sort_bs();}
};


extern void undefine_elev_array(Elevation *, Domain::Index);
extern void depress(Elev_intvl&, const Elev_intvl&);
extern void elevate(Elev_intvl&, const Elev_intvl&);


#endif // ndef _Domain_

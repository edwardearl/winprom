#ifndef _DOMAIN_ANAL_
#define _DOMAIN_ANAL_

#include "domain.h"
#include "Grid.h"

typedef Map_cart<Domain::Index> Index_map;
typedef Map_cart<bool> Bool_map;


// treeless elevation map tools

extern void define_flat(Elev_map& elev, const Rectangl&, const Elev_intvl&);
extern void undefine_flat(Elev_map& elev, const Rectangl&, const Elev_intvl&);

// helper functions

extern GridPoint steepest_nbr(const GridPoint&, const Elev_map&, const Grid&);


// treeless domain map tools

extern void fill_domap(const Elev_map&, Index_map&);
extern void xlate_domap(Index_map&, const Domain::Index *, Domain::Index);

// statistical feature correction tools

//#define MAX_CONV 2

#if 0
struct Data_point {
  double x,y,T[MAX_CONV],mu;
  Data_point(double x_, double y_, const double T_[], double mu_) :
    x(x_),y(y_),mu(mu_) {for (unsigned i=0; i<MAX_CONV; ++i) T[i]=T_[i];}
    //x(x_),y(y_),T(T_),mu(mu_) {}
  void trace(double, double) const;
};

struct Elev_methods {
  enum RQF_statpt {none,inrange,goodhess,both,all} rqf_statpt;
  bool RQF_gp,elev_data;
  Elev_methods() : rqf_statpt(none),RQF_gp(false),elev_data(true) {}
};

struct Stat_methods {
  enum Stat_model {NONE,BIAS,REGRESS} stat_model;
  bool measr_err;
  unsigned nconv_inuse;
  Stat_methods() : stat_model(BIAS), measr_err(false),nconv_inuse(0) {}
};

struct Statistic {
  vector<Data_point> points;
  Stat_methods methods;
  double bias,beta,gamma,lambda[MAX_CONV],ax,
    sigma2_raw,sigma2_res,sigma2_bias,Cbb,Caa;
  void zero() {points.clear();}
  Statistic() {zero();}
  void add_point(double x, double y, const double T[], double mu)
    {points.push_back(Data_point(x,y,T,mu*mu));}
  unsigned Nred() const;
  double mahal(const Statistic&) const;
  double numerr(const Data_point&) const;
  double variance0(const Data_point&) const;
  double variance(const Data_point& dp) const {return variance0(dp)+gamma;}
  void summarize();
  void summarize_ab(bool);
  double trial_chi2(double&) const;
  unsigned converge_g();
  bool converge_l();
  int converge_gl();
  bool converge_abg();
  bool converge_abl();
  bool converge_abgl();
  void build_deriv(double&, double[], double[],
		   double[][MAX_CONV+1],double[][MAX_CONV+1]) const;
  void bump_gamma();
  bool bump_lambda();
  void trace_conv_hess() const;
  void trace() const;
};
#endif // 0

#define MAX_REGRESS_PARAM 6

#ifdef io_support

struct Stat_options {
  enum Model {NONE,BIAS,NBREG4,NBREG8,NBREG12,NBREG20} model;
  enum Symmetry {TWO_WAY,FOUR_WAY} symmetry;
  enum Min_nbr_set {MNS_ALL,MNS_ALLBUTONE,MNS_HALF,MNS_ONE} mns;
  bool measr_err;
  unsigned nconv_inuse;
  Stat_options() : model(BIAS),symmetry(FOUR_WAY),mns(MNS_ALLBUTONE),
    measr_err(false),nconv_inuse(0) {}
  unsigned get_nnbrset() const;
  unsigned get_nnbreq(unsigned) const;
  bool get_nbr_avg(const Elev_map&, const GridPoint&, double[]) const;
};

struct MultiRegress {
  unsigned nparam;
  double Sx[MAX_REGRESS_PARAM],Sxx[MAX_REGRESS_PARAM][MAX_REGRESS_PARAM],
    Sxy[MAX_REGRESS_PARAM],Sy,Syy,alpha[MAX_REGRESS_PARAM],
    A[MAX_REGRESS_PARAM][MAX_REGRESS_PARAM],
    n,sigma2_raw,sigma2_res,bias,sigma2_bias;
  void clear(unsigned);
  void add_point(double[], double);
  void summarize();
  double slope(unsigned i) const {return Sxy[i]/Sxx[i][i];}
  double mahal(const MultiRegress&) const {return 0;}
};

struct AnalData {
  FeatureFilter filter;
  bool enable_fixed;
  Elevation fixed_range;
  Stat_options options;
  MultiRegress regress;
  AnalData() : filter(true),enable_fixed(false),fixed_range(50) {}
};

struct Assoc_info {
  float radius,cpdm;
  bool domain,walkup,path,elev_prom;
  bool pk_dest,ps_dest,ro_dest,bs_dest,bspt_assoc;
  Assoc_info(Distance r, bool t) : radius(r),cpdm(1),
    domain(false),walkup(false),path(t),elev_prom(false),
    pk_dest(true),ps_dest(true),ro_dest(false),bs_dest(t),bspt_assoc(true) {}
};

#endif // io_support

struct Divide_tree_map : Divide_tree {
  // elevation map -> divide tree analysis tools
  #ifdef administrator
  void walkup_analyze(const Elev_map&, Index_map&,
		      void (*)(float)=0, void (*)()=0,
		      void (*)()=0, void (*)()=0);
  void search_edge(const Elev_map&, Index_map&, Bool_map&);
  #ifdef io_support
  // correction tools
  void shoreline_correction(const Elev_map&, const FeatureTypeFilter&,
			    const Database&, Elevation, Elevation, bool, bool);
  #endif
  #endif // def administrator
  // domain map tools
  void resolve_domap(Index_map&, Domain::Index&,
		     void (*)()=0, void (*)()=0) const;
  static void consolidate_domap(const Elev_map&, const Divide_tree *,
				Index_map&, Domain::Index);
  void purge_domap(Index_map&, Elevation, bool, bool) const;
  void lpr_map(const Elev_map&, Index_map&, Domain::Index) const;
  // purge tools
  void purge_noise(const Elev_map&, float, bool, bool, bool, bool,
		   double&, Elevation&, Domain::Index **xlate=0);
  bool purge_bs_drainage(const Divide_tree&,
			 const Index_map&, const Index_map&,
			 const Elev_map&, bool, bool (*nonpeak_cb)());
#ifdef io_support
  void set_error(const Elev_map&, double,
		 const AnalData&, const AnalData&, const Database&);
  // association tools
  Domain::Index
    assoc_peak(const GridPoint&, const Assoc_info&,
	       const Index_map&, const Elev_map&, const Grid&, Bool_map&,
	       const Elevation *, const vector<Domain *>&);
  Domain::Index
    assoc_sadl(const GridPoint&, const Assoc_info&,
	       Domain *, Domain *, const Domain *, GridPoint::Coord,
	       const vector<Domain *>&, Elevation *, Elevation *, bool,
	       const vector<Basin_saddle *>&, Domain::Index&);
  Domain::Index
    assoc_ro(const GridPoint&, const Assoc_info&, const vector<Runoff *>&);
  // transfer tools
  enum Sadl_src {SS_PROM,SS_BASIN,SS_ALL};
  Domain *nbr_peak(Domain *, Domain *, const Feature&,
		   const Divide_tree&, const Assoc_info&, Elevation[],
		   const Index_map&, const Elev_map&, Bool_map&, const Grid&,
		   const Domain *&, const vector<Domain *>, Domain::Index[]);
  void consol_1sadl(Saddle&, const Assoc_info&, Elevation[],
		    Domain *, Domain *, GridPoint::Coord,
		    bool, Consol_sadl[], Consol_sadl[],
		    const vector<Domain *>&, const vector<Basin_saddle *>&,
		    vector<Feature *>&, vector<Feature *>&, bool);
  void xfer_peak_info(Divide_tree&, const Rectangl&, const Assoc_info&,
		      Elevation[], Domain::Index[],
		      const Index_map&, const Elev_map&,
		      Bool_map&, const Grid&, vector<Domain *>&,
		      vector <Feature *>&, vector <Feature *>&);
  void xfer_sadl_info(Divide_tree&, const Rectangl&, const Assoc_info&, Sadl_src,
		      Elevation[], Elevation[], Domain::Index[],
		      const Index_map&, const Elev_map&,
		      Bool_map&, const Grid& grid, const vector<Domain *>&,
		      vector <Feature *>&, vector <Feature *>&,
		      vector <Feature *>&, vector <Feature *>&,
		      bool);
  void xfer_ro_info(Divide_tree&, const Rectangl&, const Assoc_info&,
		    vector<Feature *>&, vector<Feature *>&);
  void xfer_featr_info(Divide_tree&, const Rectangl&,
		       const Index_map&, const Elev_map&,
		       const Assoc_info&, Sadl_src,
		       vector<Feature *>&, vector<Feature *>&,
		       vector<Feature *>&, vector<Feature *>&,
		       vector<Feature *>&, vector<Feature *>&,
		       vector<Feature *>&, vector<Feature *>&,
		       bool);
#endif // def io_support
};

#endif // ndef _DOMAIN_ANAL_

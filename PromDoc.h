// PromDoc.h : interface of the CPromDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROMDOC_H__9927AF4E_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_PROMDOC_H__9927AF4E_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CScriptCtl_dlg;
enum File_state {NOT_LOADED,NOT_CHANGED,CHANGED};

#include "Area_info.h"
#include "domain_anal.h"
#include "Format.h"
#include "ErrorAnal_dlg.h"


struct Error_stats;

struct Clip {
  vector<Feature *> list;
  vector<FT_index> type;
  vector<Database::RecordID> orph_list;
  void add_featr(Feature& ftr, FT_index t)
    {list.push_back(&ftr); type.push_back(t);}
  unsigned ftr_count() const
    {assert(list.size()==type.size()); return list.size();}
  unsigned rec_count() const {return orph_list.size();}
  bool empty() const {return ftr_count()==0 && rec_count()==0;}
  void clear() {list.clear(); type.clear(); orph_list.clear();}
  FT_index iconType(bool) const;
};

struct Sorted_featr_lists {
  vector<Domain *> peaks,sadls;
  vector<Runoff *> ro;
  vector<Basin_saddle *> bs;
};

class script_error : public exception {
  const char *text;
public:
  script_error(const char *t) : text(t) {}
  virtual const char *what() const {return text;}
};


class CPromDoc : public CDocument {

  friend class CPromApp;
  void CalcTotalArea();
  void resolveDomap();
  void UpdateViews(Feature *);
  void BeginWork();
  void EndWork();
  void setMarkBound();
  void dt_apply_drastic_change();
  void modify_topo();
  void purge_peaks(Elevation, bool, bool, bool, bool, bool);
  void purge_noise(float, bool, bool, bool, bool, bool, double&, Elevation&);
  void purge_basin_saddles(Elevation);
  void purge_bs_prom(Elevation, bool, bool, bool, bool);
  void purge_bs_redun();
  void purge_bs_drain();
  void rotate();
  void elev_flip();
  bool elev_transform(float, float);
  bool tree_transform(float, float, bool, bool, bool, bool, bool);
  void revert_feature(Feature&);
  bool initial_drain,hell_change,feature_change,trans_change,
    bounds_change,map_change,topo_change,
    RObase_change,BSbase_change,splice_change,data_change;
  float work_m,work_b;
  bool auto_update;

protected: // create from serialization only
  CPromDoc();
  DECLARE_DYNCREATE(CPromDoc)

// Attributes
public:
  enum Use_elev {UE_LOW,UE_MIDPT,UE_HIGH};
  static char buf[1024];
  Record_location_filter init_data_lf;
  static Location_fmt status_fmt;

  // these are "dummy values" used to indicate to views that something
  // other than a specific feature has changed
  static Feature dummyModifyAll,dummyModifyTrans,dummyModifyBounds,dummyModifyMap,
    dummyModifyTopo,dummyModifyRObase,dummyModifyBSbase,dummyModifySplice,
    dummyModifyData,dummySaveCenter;

  File_state em_state,dm_state,dt_state,db_state;
  bool em_save_drastic_change,
    dm_save_drastic_change,
    dt_save_drastic_change,
    db_save_drastic_change;
  mutable bool em_script_clean,dm_script_clean,dt_script_clean;
  CString em_name,dm_name,dt_name,db_name;
  bool peak_prom_utd,sadl_prom_utd,ridge_utd,prom_zp,prom_ss;

  Elev_map elev;
  Index_map domain;
  Divide_tree_map tree;
  bool is_drainage;
  Database data;
  Domain::Index n_domain;
  Rectangl mark_bound;
  Area_info total_alloc,total_defined;

  Clip copyPaste,dragDrop;
  FT_index changed_type;
  Domain::Index old_n;
  Runoff *old_ro;
  Domain::Index new_iro;
  Basin_saddle *old_bs;
  Database::RecordID record_change;
  float trans_m,trans_b;
  bool working,tree_writeable;
  Elevation shore_basin_elev,shore_sadl_elev;
  bool shore_set_edit,shore_mark_edit;
  FeatureTypeFilter shore_filter;
  Error_stats error_stats;

// Operations
public:
  enum FileOp {FO_OPEN=0,FO_MERGE=1,FO_SAVE=2};
  static CString GetFileName(FileOp, const char *, const char *, const char *,
    CWnd *parent=NULL);
  bool readElevMap(const char *);
  bool readDomap(const char *);
  bool readDivideTree(const char *);
  void readDatabase(const char *);
  bool mergeElevMap(const char *);
  bool mergeDomap(const char *);
  bool mergeDivideTree(const char *);
  bool saveasElevMap(const char *);
  bool saveasDomap(const char *);
  bool saveasDivideTree(const char *);
  void execute_command(FILE *, CScriptCtl_dlg&) throw (script_error);
  bool runScript(const char *, CScriptCtl_dlg&);
  void LPRmap();
  void fillDomap();
  void GetDomapArea(Area_info&, Area_info&);
  void GetElevMapArea(Area_info&, Area_info&, Elev_intvl&);
  static void FileError(const char *, const char *, const char *);
  static void FileError(const char *type, const char *fn)
    {FileError(type,fn,strerror(errno));}
  static bool peak_sadl_nbr(Feature *, Feature *);
  static Domain *peak_beyond_sadl(Feature *, Feature *);
  static Elevation getUseElev(const Elev_intvl&, Use_elev);
  bool doc_CanClose(const File_state&, const char *, const CString&,
		    void (CPromDoc::*)(), bool=false);
  bool em_CanClose(bool=false);
  bool dm_CanClose(bool=false);
  bool dt_CanClose(bool=false);
  bool db_CanClose(bool=false);
  bool em_ScriptCanClose(bool=false) const;
  bool dm_ScriptCanClose(bool=false) const;
  bool dt_ScriptCanClose(bool=false) const;
  bool db_ScriptCanClose(bool=false) const;
  void SetViewTitles() const;
  bool check_overlap(Feature&, Feature&, const char *, CWnd *);
  void more_disc(Elev_intvl&, const Feature&);
  void EditFeature(Feature&, FT_index, CWnd *);
  void DeleteFeature(Feature&, FT_index, CWnd *);
  void DeletePeak(Feature&, Feature *, CWnd *, bool=false);
  void DeleteRiver(Feature *, Feature *);
  void DeleteSaddle(Feature&, CWnd *);
  void DeleteRunoff(Feature&, CWnd *);
  void DeleteBasinsadl(Feature&, CWnd *);
  void NewSaddle(const GridPoint&, Domain *, Domain *);
  void NewRunoff(const GridPoint&, Domain *);
  void EditRecord(const GridPoint&, CWnd *);
  void EditRecord(Database::RecordID, CWnd *);
  void DeleteRecord(Database::RecordID);
  void EditFields(CWnd *);
  void Rotate(Basin_saddle&, Feature *, CWnd *);
  void Splice(Runoff&, CWnd *);
  void Splice(Runoff **, unsigned, CWnd *);
  void SWI_switch(Feature&, Feature&, CWnd *);
  void SWI_switch(Feature&, Basin_saddle&, CWnd *);
  void SWI_switch(Basin_saddle&, Feature&, CWnd *);
  void SWI_switch(Basin_saddle&, Basin_saddle&, CWnd *);
  void SWI_switch(Feature&, Runoff&);
  void SWI_switch(Basin_saddle&, Runoff&, CWnd *);
  void SWI_switch_sym(Feature&, Feature&, CWnd *);
  void SWI_switch_sym(Feature&, Basin_saddle&, CWnd *);
  void SWI_switch_sym(Basin_saddle&, Basin_saddle&, CWnd *);
  void reconnect(Feature&, Feature&, CWnd *);
  void reconnect(Feature&, Basin_saddle&, CWnd *);
  void reconnect(Feature&, Runoff&, CWnd *);
  bool canTakeDrop(CWnd *, CPoint);
  void takeDrop(CWnd *, CPoint);
  void type2image(FT_index&) const;
  void image2type(FT_index&) const;
  static void imageFlip(FT_index&);
  static bool get_rect(Rectangl&, CWnd *,
		const char *, const char *, const char *, const char *,
		CEdit&, CEdit&, CEdit&, CEdit&);
  static void set_rect(const Rectangl&, CString&, CString&, CString&, CString&);
  bool *nonorphans() const;
  void elev_flip_drain();
  void tree_flip_drain();
  void update_peak_prom_info();
  bool update_sadl_prom_info();
  void update_ridge_info();
  void update_peak_prom_info_rq() {if (auto_update) update_peak_prom_info();}
  bool update_sadl_prom_info_rq()
    {return auto_update?update_sadl_prom_info():true;}
  void update_ridge_info_rq() {if (auto_update) update_ridge_info();}
  const char *peak_basin_type() const {return is_drainage?"basin":"peak";}
  Elev_endpt inner_endpt() const;
  Elev_endpt outer_endpt() const;
  short int drain_const() const {return is_drainage?-1:1;}
  static void enable_submenu(CCmdUI *, bool);
  void set_tb_bmp() const;
  void setup_popup(CMenu *, const Feature&, FT_index) const;
  static bool clip_grid_point(Clip&, const GridPoint&,
			      const Sorted_featr_lists&);
  void list_stats(AnalData *, AnalData *) const;
  void add_stat(const Feature&, AnalData *) const;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPromDoc)
  public:
  virtual BOOL OnNewDocument();
  virtual void OnCloseDocument();
  protected:
  virtual BOOL SaveModified();
  //}}AFX_VIRTUAL

// Implementation
public:
  virtual ~CPromDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  void analyze(bool);
  void post_analyze();
  void post_post_analyze();

// Generated message map functions
protected:
  //{{AFX_MSG(CPromDoc)
  afx_msg void OnOpenDLPRmap();
  afx_msg void OnOpenDLPRtree();
  afx_msg void OnOpenElevMap();
  afx_msg void OnSaveAsDLPRtree();
  afx_msg void OnSaveAsDLPRmap();
  afx_msg void OnSaveAsElevMap();
  afx_msg void OnUpdateHaveDLPRmap(CCmdUI* pCmdUI);
  afx_msg void OnUpdateHaveDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnUpdateHaveElevMap(CCmdUI* pCmdUI);
  afx_msg void OnCloseDLPRmap();
  afx_msg void OnCloseDLPRtree();
  afx_msg void OnCloseElevMap();
  afx_msg void OnCloseAll();
  afx_msg void OnSaveAll();
  afx_msg void OnSaveDLPRmap();
  afx_msg void OnSaveDLPRtree();
  afx_msg void OnSaveElevMap();
  afx_msg void OnInfoDLPRmap();
  afx_msg void OnInfoElevMap();
  afx_msg void OnInfoDLPRtree();
  afx_msg void OnInfoAll();
  afx_msg void OnListPeaks();
  afx_msg void OnListSaddles();
  afx_msg void OnListBasinsadls();
  afx_msg void OnListRunoffs();
  afx_msg void OnListGeneric();
  afx_msg void OnMergeElevMap();
  afx_msg void OnMergeDLPRtree();
  afx_msg void OnMergeDLPRmap();
  afx_msg void OnOpSplice();
  afx_msg void OnOpAnalyze();
  afx_msg void OnOpRotate();
  afx_msg void OnOpPurgeBasinsadls();
  afx_msg void OnOpPurgePeaks();
  afx_msg void OnOpCompressAll();
  afx_msg void OnOpCompressBasinsadls();
  afx_msg void OnOpCompressPeaks();
  afx_msg void OnOpCompressRunoffs();
  afx_msg void OnWindowMap();
  afx_msg void OnUpdateOpCompressBasinsadls(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOpCompressPeaks(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOpCompressRunoffs(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOpCompressAll(CCmdUI* pCmdUI);
  afx_msg void OnWindowProfile();
  afx_msg void OnOpLPRmap();
  afx_msg void OnOpResortNbrs();
  afx_msg void OnOpMaintAll();
  afx_msg void OnOpRun();
  afx_msg void OnEditCopyDLPRmap();
  afx_msg void OnEditCopyDLPRtree();
  afx_msg void OnEditCopyElevMap();
  afx_msg void OnEditPasteDLPRmap();
  afx_msg void OnUpdateEditPasteDLPRmap(CCmdUI* pCmdUI);
  afx_msg void OnEditPasteDLPRtree();
  afx_msg void OnUpdateEditPasteDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnEditPasteElevMap();
  afx_msg void OnUpdateEditPasteElevMap(CCmdUI* pCmdUI);
  afx_msg void OnEditCopyAll();
  afx_msg void OnEditPasteAll();
  afx_msg void OnUpdateEditPasteAll(CCmdUI* pCmdUI);
  afx_msg void OnOpFlatUndefine();
  afx_msg void OnOpFlatDefine();
  afx_msg void OnOpDiagErrors();
  afx_msg void OnUpdateNoFeatureEdit(CCmdUI* pCmdUI);
  afx_msg void OnUpdateWriteableDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOpAnalyze(CCmdUI* pCmdUI);
  afx_msg void OnUpdateHaveAny(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCloseAll(CCmdUI* pCmdUI);
  afx_msg void OnOpDiagDups();
  afx_msg void OnOpPurgeBsDegen();
  afx_msg void OnOpPurgeBsProm();
  afx_msg void OnOpElevTransform();
  afx_msg void OnEditTransfer();
  afx_msg void OnUpdateEditTransfer(CCmdUI* pCmdUI);
  afx_msg void OnOpDiagSWI();
  afx_msg void OnOpDLPRfill();
  afx_msg void OnOpPurgeBsRedun();
  afx_msg void OnOpResortPeaks();
  afx_msg void OnOpTreeTransform();
  afx_msg void OnOpDiagAmbOff();
  afx_msg void OnOpDiagOvlpNbrs();
  afx_msg void OnEditCopyDatabase();
  afx_msg void OnUpdateHaveDatabase(CCmdUI* pCmdUI);
  afx_msg void OnEditPasteDatabase();
  afx_msg void OnOpenDatabase();
  afx_msg void OnMergeDatabase();
  afx_msg void OnSaveDatabase();
  afx_msg void OnSaveAsDatabase();
  afx_msg void OnInfoDatabase();
  afx_msg void OnCloseDatabase();
  afx_msg void OnListDatabase();
  afx_msg void OnUpdateEditPasteDatabase(CCmdUI* pCmdUI);
  afx_msg void OnUpdateWriteableData(CCmdUI* pCmdUI);
  afx_msg void OnUpdateListPeaks(CCmdUI* pCmdUI);
  afx_msg void OnUpdateListBasinsadls(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOpPurgePeaks(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOpResortPeaks(CCmdUI* pCmdUI);
  afx_msg void OnOpDataAlign();
  afx_msg void OnUpdateOpDataAlign(CCmdUI* pCmdUI);
  afx_msg void OnUpdateNewWindow(CCmdUI* pCmdUI);
  afx_msg void OnOpElevCrop();
  afx_msg void OnOpDLPRcrop();
  afx_msg void OnOpDLPRconsol();
  afx_msg void OnOpPurgeBsDrainage();
  afx_msg void OnUpdateOpPurgeBsDrainage(CCmdUI* pCmdUI);
  afx_msg void OnOpAnalyzeBasins();
  afx_msg void OnUpdateOpPurgeBSprom(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMergeDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFlatUndefine(CCmdUI* pCmdUI);
  afx_msg void OnUpdateHaveMaps(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDLPRfill(CCmdUI* pCmdUI);
  afx_msg void OnUpdateSplice(CCmdUI* pCmdUI);
  afx_msg void OnOpErrorAnal();
  afx_msg void OnListEdited();
  afx_msg void OnOpTreeShoreline();
  afx_msg void OnUpdateHaveEMDT(CCmdUI* pCmdUI);
  afx_msg void OnOpTreeData();
  afx_msg void OnUpdateOpTreeData(CCmdUI* pCmdUI);
  afx_msg void OnOpPurgeNoise();
  afx_msg void OnUpdateHaveAll(CCmdUI* pCmdUI);
  afx_msg void OnOpRevert();
  afx_msg void OnOpResortAll();
  afx_msg void OnUpdateOpResortBS(CCmdUI* pCmdUI);
  afx_msg void OnOpResortBS();
  afx_msg void OnOpDataPurge();
  afx_msg void OnUpdateDomapPurge(CCmdUI* pCmdUI);
  afx_msg void OnDomapPurge();
  afx_msg void OnZeroProb();
  afx_msg void OnUpdateZeroProb(CCmdUI* pCmdUI);
  afx_msg void OnOpSadlstatProm();
  afx_msg void OnOpSadlstatUnknown();
  afx_msg void OnOpSadlstatReal();
  afx_msg void OnUpdateSaddleStat(CCmdUI* pCmdUI);
  afx_msg void OnSaddleStat();
  afx_msg void OnViewUpdateAuto();
  afx_msg void OnViewUpdateNow();
  afx_msg void OnUpdateViewUpdateNow(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewUpdateAuto(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOpDiagSWI(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMDOC_H__9927AF4E_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)

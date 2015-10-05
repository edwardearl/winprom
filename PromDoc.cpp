// PromDoc.cpp : implementation of the CPromDoc class
//

#include "stdafx.h"
#include "winprom.h"
#include "PromDoc.h"
#include "PromFrame.h"

#include "PeakListView.h"
#include "SaddleListView.h"
#include "RunoffListView.h"
#include "BasinSadlListView.h"
#include "GenericListView.h"
#include "PeakNbrListView.h"
#include "ErrorListView.h"
#include "SaddleWalkInvView.h"
#include "DataListView.h"
#include "LPRtreeView.h"
#include "GraphView.h"

#include "ElevMapInfo_dlg.h"
#include "DomapInfo_dlg.h"
#include "TreeInfo_dlg.h"
#include "DataInfo_dlg.h"
#include "WSinfo_dlg.h"
#include "ElevConflict_dlg.h"
#include "Purge_dlg.h"
#include "PurgeNoise_dlg.h"
#include "PurgeDomain_dlg.h"
#include "NoiseSum_dlg.h"
#include "FeatureEdit_dlg.h"
#include "Data_dlg.h"
#include "Fields_dlg.h"
#include "Transform_dlg.h"
#include "TransformTree_dlg.h"
#include "ScriptCtl_dlg.h"
#ifdef administrator
#include "AnalyzeCtl_dlg.h"
#include "Shoreline_dlg.h"
#endif
#include "XferTree_dlg.h"
#include "DataAlign_dlg.h"
#include "Crop_dlg.h"
#include "DefineFlat_dlg.h"
#include "DataImport_dlg.h"
#include "DataLocationFilter_dlg.h"
#include <direct.h>
#include <algorithm>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define timer


FT_index Clip::iconType(bool drainage) const
{
  unsigned i,n=ftr_count();
  for (i=1; i<n; ++i)
    if (type[i]!=type[0]) break;
  if (i!=type.size()) return drainage?FT_MIXED_DRAIN:FT_MIXED;
  if (!drainage) return type[0];
  switch (type[0]) {
  case FT_PEAK: return FT_BASIN;
  case FT_SADDLE: return FT_BASINSADL;
  case FT_RUNOFF: return FT_SBRO;
  case FT_BASINSADL: return FT_SADDLE;
  }
  assert(0);
  return FT_NUM_IMG;
}

// nearest integer (helper function)
inline static Elevation elev_nint(double x)
{
  return (Elevation)(x + (x>0 ? 0.5 : -0.5));
}


/////////////////////////////////////////////////////////////////////////////
// CPromDoc

IMPLEMENT_DYNCREATE(CPromDoc, CDocument)

BEGIN_MESSAGE_MAP(CPromDoc, CDocument)
  //{{AFX_MSG_MAP(CPromDoc)
  ON_COMMAND(ID_OPEN_DLPR_MAP, OnOpenDLPRmap)
  ON_COMMAND(ID_OPEN_DLPR_TREE, OnOpenDLPRtree)
  ON_COMMAND(ID_OPEN_ELEV_MAP, OnOpenElevMap)
  ON_COMMAND(ID_SAVEAS_DLPR_TREE, OnSaveAsDLPRtree)
  ON_COMMAND(ID_SAVEAS_DLPR_MAP, OnSaveAsDLPRmap)
  ON_COMMAND(ID_SAVEAS_ELEV_MAP, OnSaveAsElevMap)
  ON_UPDATE_COMMAND_UI(ID_SAVEAS_DLPR_MAP, OnUpdateHaveDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_SAVEAS_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_SAVEAS_ELEV_MAP, OnUpdateHaveElevMap)
  ON_COMMAND(ID_CLOSE_DLPR_MAP, OnCloseDLPRmap)
  ON_COMMAND(ID_CLOSE_DLPR_TREE, OnCloseDLPRtree)
  ON_COMMAND(ID_CLOSE_ELEV_MAP, OnCloseElevMap)
  ON_COMMAND(ID_CLOSE_ALL, OnCloseAll)
  ON_COMMAND(ID_SAVE_ALL, OnSaveAll)
  ON_COMMAND(ID_SAVE_DLPR_MAP, OnSaveDLPRmap)
  ON_COMMAND(ID_SAVE_DLPR_TREE, OnSaveDLPRtree)
  ON_COMMAND(ID_SAVE_ELEV_MAP, OnSaveElevMap)
  ON_COMMAND(ID_INFO_DLPR_MAP, OnInfoDLPRmap)
  ON_COMMAND(ID_INFO_ELEV_MAP, OnInfoElevMap)
  ON_COMMAND(ID_INFO_DLPR_TREE, OnInfoDLPRtree)
  ON_COMMAND(ID_INFO_ALL, OnInfoAll)
  ON_COMMAND(ID_LIST_PEAKS, OnListPeaks)
  ON_COMMAND(ID_LIST_SADDLES, OnListSaddles)
  ON_COMMAND(ID_LIST_BASINSADLS, OnListBasinsadls)
  ON_COMMAND(ID_LIST_RUNOFFS, OnListRunoffs)
  ON_COMMAND(ID_LIST_GENERIC, OnListGeneric)
  ON_COMMAND(ID_MERGE_ELEV_MAP, OnMergeElevMap)
  ON_COMMAND(ID_MERGE_DLPR_TREE, OnMergeDLPRtree)
  ON_COMMAND(ID_MERGE_DLPR_MAP, OnMergeDLPRmap)
  ON_COMMAND(ID_OP_SPLICE, OnOpSplice)
  ON_COMMAND(ID_OP_ANALYZE, OnOpAnalyze)
  ON_COMMAND(ID_OP_ROTATE, OnOpRotate)
  ON_COMMAND(ID_OP_PURGE_BASINSADLS, OnOpPurgeBasinsadls)
  ON_COMMAND(ID_OP_PURGE_PEAKS, OnOpPurgePeaks)
  ON_COMMAND(ID_OP_COMPRESS_ALL, OnOpCompressAll)
  ON_COMMAND(ID_OP_COMPRESS_BASINSADLS, OnOpCompressBasinsadls)
  ON_COMMAND(ID_OP_COMPRESS_PEAKS, OnOpCompressPeaks)
  ON_COMMAND(ID_OP_COMPRESS_RUNOFFS, OnOpCompressRunoffs)
  ON_COMMAND(ID_WINDOW_MAP, OnWindowMap)
  ON_UPDATE_COMMAND_UI(ID_OP_COMPRESS_BASINSADLS, OnUpdateOpCompressBasinsadls)
  ON_UPDATE_COMMAND_UI(ID_OP_COMPRESS_PEAKS, OnUpdateOpCompressPeaks)
  ON_UPDATE_COMMAND_UI(ID_OP_COMPRESS_RUNOFFS, OnUpdateOpCompressRunoffs)
  ON_UPDATE_COMMAND_UI(ID_OP_COMPRESS_ALL, OnUpdateOpCompressAll)
  ON_COMMAND(ID_WINDOW_PROFILE, OnWindowProfile)
  ON_COMMAND(ID_OP_LPRMAP, OnOpLPRmap)
  ON_COMMAND(ID_OP_RESORT_NBRS, OnOpResortNbrs)
  ON_COMMAND(ID_OP_MAINT_ALL, OnOpMaintAll)
  ON_COMMAND(ID_OP_RUN, OnOpRun)
  ON_COMMAND(ID_EDIT_COPY_DLPR_MAP, OnEditCopyDLPRmap)
  ON_COMMAND(ID_EDIT_COPY_DLPR_TREE, OnEditCopyDLPRtree)
  ON_COMMAND(ID_EDIT_COPY_ELEV_MAP, OnEditCopyElevMap)
  ON_COMMAND(ID_EDIT_PASTE_DLPR_MAP, OnEditPasteDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_DLPR_MAP, OnUpdateEditPasteDLPRmap)
  ON_COMMAND(ID_EDIT_PASTE_DLPR_TREE, OnEditPasteDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_DLPR_TREE, OnUpdateEditPasteDLPRtree)
  ON_COMMAND(ID_EDIT_PASTE_ELEV_MAP, OnEditPasteElevMap)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_ELEV_MAP, OnUpdateEditPasteElevMap)
  ON_COMMAND(ID_EDIT_COPY_ALL, OnEditCopyAll)
  ON_COMMAND(ID_EDIT_PASTE_ALL, OnEditPasteAll)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_ALL, OnUpdateEditPasteAll)
  ON_COMMAND(ID_OP_FLAT_UNDEFINE, OnOpFlatUndefine)
  ON_COMMAND(ID_OP_FLAT_DEFINE, OnOpFlatDefine)
  ON_COMMAND(ID_OP_DIAG_ERRORS, OnOpDiagErrors)
  ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateNoFeatureEdit)
  ON_UPDATE_COMMAND_UI(ID_CLOSE_DLPR_TREE, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_ANALYZE, OnUpdateOpAnalyze)
  ON_UPDATE_COMMAND_UI(ID_SAVE_ALL, OnUpdateHaveAny)
  ON_UPDATE_COMMAND_UI(ID_CLOSE_ALL, OnUpdateCloseAll)
  ON_COMMAND(ID_OP_DIAG_DUPS, OnOpDiagDups)
  ON_COMMAND(ID_OP_PURGE_BS_DEGEN, OnOpPurgeBsDegen)
  ON_COMMAND(ID_OP_PURGE_BS_PROM, OnOpPurgeBsProm)
  ON_COMMAND(ID_OP_ELEV_TRANSFORM, OnOpElevTransform)
  ON_COMMAND(ID_EDIT_TRANSFER, OnEditTransfer)
  ON_UPDATE_COMMAND_UI(ID_EDIT_TRANSFER, OnUpdateEditTransfer)
  ON_COMMAND(ID_OP_DIAG_SWI, OnOpDiagSWI)
  ON_COMMAND(ID_OP_DLPR_FILL, OnOpDLPRfill)
  ON_COMMAND(ID_OP_PURGE_BS_REDUN, OnOpPurgeBsRedun)
  ON_COMMAND(ID_OP_RESORT_PEAKS, OnOpResortPeaks)
  ON_COMMAND(ID_OP_TREE_TRANSFORM, OnOpTreeTransform)
  ON_COMMAND(ID_OP_DIAG_AMBOFF, OnOpDiagAmbOff)
  ON_COMMAND(ID_OP_DIAG_OVLP_NBRS, OnOpDiagOvlpNbrs)
  ON_COMMAND(ID_EDIT_COPY_DATABASE, OnEditCopyDatabase)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_DATABASE, OnUpdateHaveDatabase)
  ON_COMMAND(ID_EDIT_PASTE_DATABASE, OnEditPasteDatabase)
  ON_COMMAND(ID_OPEN_DATABASE, OnOpenDatabase)
  ON_COMMAND(ID_MERGE_DATABASE, OnMergeDatabase)
  ON_COMMAND(ID_SAVE_DATABASE, OnSaveDatabase)
  ON_COMMAND(ID_SAVEAS_DATABASE, OnSaveAsDatabase)
  ON_COMMAND(ID_INFO_DATABASE, OnInfoDatabase)
  ON_COMMAND(ID_CLOSE_DATABASE, OnCloseDatabase)
  ON_COMMAND(ID_LIST_DATABASE, OnListDatabase)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_DATABASE, OnUpdateEditPasteDatabase)
  ON_UPDATE_COMMAND_UI(ID_MERGE_DATABASE, OnUpdateWriteableData)
  ON_UPDATE_COMMAND_UI(ID_LIST_PEAKS, OnUpdateListPeaks)
  ON_UPDATE_COMMAND_UI(ID_LIST_BASINSADLS, OnUpdateListBasinsadls)
  ON_UPDATE_COMMAND_UI(ID_OP_PURGE_PEAKS, OnUpdateOpPurgePeaks)
  ON_UPDATE_COMMAND_UI(ID_OP_RESORT_PEAKS, OnUpdateOpResortPeaks)
  ON_COMMAND(ID_OP_DATA_ALIGN, OnOpDataAlign)
  ON_UPDATE_COMMAND_UI(ID_OP_DATA_ALIGN, OnUpdateOpDataAlign)
  ON_COMMAND(ID_OP_ELEV_CROP, OnOpElevCrop)
  ON_COMMAND(ID_OP_DLPR_CROP, OnOpDLPRcrop)
  ON_COMMAND(ID_OP_DLPR_CONSOL, OnOpDLPRconsol)
  ON_COMMAND(ID_OP_PURGE_BS_DRG, OnOpPurgeBsDrainage)
  ON_UPDATE_COMMAND_UI(ID_OP_PURGE_BS_DRG, OnUpdateOpPurgeBsDrainage)
  ON_COMMAND(ID_OP_ANALYZE_BASINS, OnOpAnalyzeBasins)
  ON_UPDATE_COMMAND_UI(ID_OP_PURGE_BS_PROM, OnUpdateOpPurgeBSprom)
  ON_UPDATE_COMMAND_UI(ID_MERGE_DLPR_TREE, OnUpdateMergeDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_FLAT_UNDEFINE, OnUpdateFlatUndefine)
  ON_UPDATE_COMMAND_UI(ID_OP_DLPR_CONSOL, OnUpdateHaveMaps)
  ON_UPDATE_COMMAND_UI(ID_OP_DLPR_FILL, OnUpdateDLPRfill)
  ON_UPDATE_COMMAND_UI(ID_OP_SPLICE, OnUpdateSplice)
  ON_COMMAND(ID_OP_ERROR_ANAL, OnOpErrorAnal)
  ON_COMMAND(ID_LIST_EDITED, OnListEdited)
  ON_COMMAND(ID_OP_TREE_SHORELINE, OnOpTreeShoreline)
  ON_UPDATE_COMMAND_UI(ID_OP_DIAG_ERRORS, OnUpdateHaveEMDT)
  ON_COMMAND(ID_OP_TREE_DATA, OnOpTreeData)
  ON_UPDATE_COMMAND_UI(ID_OP_TREE_DATA, OnUpdateOpTreeData)
  ON_COMMAND(ID_OP_PURGE_NOISE, OnOpPurgeNoise)
  ON_UPDATE_COMMAND_UI(ID_OP_LPRMAP, OnUpdateHaveAll)
  ON_COMMAND(ID_OP_REVERT, OnOpRevert)
  ON_COMMAND(ID_OP_RESORT_ALL, OnOpResortAll)
  ON_UPDATE_COMMAND_UI(ID_OP_RESORT_BS, OnUpdateOpResortBS)
  ON_COMMAND(ID_OP_RESORT_BS, OnOpResortBS)
  ON_COMMAND(ID_OP_DATA_PURGE, OnOpDataPurge)
  ON_UPDATE_COMMAND_UI(ID_OP_DOMAP_PURGE, OnUpdateDomapPurge)
  ON_COMMAND(ID_OP_DOMAP_PURGE, OnDomapPurge)
  ON_COMMAND(ID_ZERO_PROB, OnZeroProb)
  ON_UPDATE_COMMAND_UI(ID_ZERO_PROB, OnUpdateZeroProb)
  ON_COMMAND(ID_OP_SS_PROM, OnOpSadlstatProm)
  ON_COMMAND(ID_OP_SS_UNKNOWN, OnOpSadlstatUnknown)
  ON_COMMAND(ID_OP_SS_REAL, OnOpSadlstatReal)
  ON_UPDATE_COMMAND_UI(ID_SADDLE_STAT, OnUpdateSaddleStat)
  ON_COMMAND(ID_SADDLE_STAT, OnSaddleStat)
  ON_COMMAND(ID_VIEW_UPDATE_AUTO, OnViewUpdateAuto)
  ON_COMMAND(ID_VIEW_UPDATE_NOW, OnViewUpdateNow)
  ON_UPDATE_COMMAND_UI(ID_VIEW_UPDATE_NOW, OnUpdateViewUpdateNow)
  ON_UPDATE_COMMAND_UI(ID_VIEW_UPDATE_AUTO, OnUpdateViewUpdateAuto)
  ON_UPDATE_COMMAND_UI(ID_OP_DIAG_SWI, OnUpdateOpDiagSWI)
  ON_UPDATE_COMMAND_UI(ID_SAVE_DLPR_MAP, OnUpdateHaveDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_SAVE_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_SAVE_ELEV_MAP, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_CLOSE_DLPR_MAP, OnUpdateHaveDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_CLOSE_ELEV_MAP, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_MERGE_DLPR_MAP, OnUpdateHaveDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_MERGE_ELEV_MAP, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_INFO_DLPR_MAP, OnUpdateHaveDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_INFO_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_INFO_ELEV_MAP, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_LIST_RUNOFFS, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_LIST_GENERIC, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_RESORT_NBRS, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_DLPR_MAP, OnUpdateHaveDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_ELEV_MAP, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_ALL, OnUpdateHaveAny)
  ON_UPDATE_COMMAND_UI(ID_OP_FLAT_DEFINE, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_OPEN_DLPR_TREE, OnUpdateNoFeatureEdit)
  ON_UPDATE_COMMAND_UI(ID_OP_MAINT_ALL, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_PURGE_BASINSADLS, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_ROTATE, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_RUN, OnUpdateNoFeatureEdit)
  ON_UPDATE_COMMAND_UI(ID_OP_DIAG_DUPS, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_PURGE_BS_DEGEN, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_ELEV_TRANSFORM, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_OP_PURGE_BS_REDUN, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_TREE_TRANSFORM, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_DIAG_AMBOFF, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_DIAG_OVLP_NBRS, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_SAVE_DATABASE, OnUpdateHaveDatabase)
  ON_UPDATE_COMMAND_UI(ID_SAVEAS_DATABASE, OnUpdateHaveDatabase)
  ON_UPDATE_COMMAND_UI(ID_INFO_DATABASE, OnUpdateHaveDatabase)
  ON_UPDATE_COMMAND_UI(ID_LIST_DATABASE, OnUpdateHaveDatabase)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_CLOSE_DATABASE, OnUpdateWriteableData)
  ON_UPDATE_COMMAND_UI(ID_OPEN_DATABASE, OnUpdateNoFeatureEdit)
  ON_UPDATE_COMMAND_UI(ID_LIST_SADDLES, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_DLPR_CHECK, OnUpdateHaveMaps)
  ON_UPDATE_COMMAND_UI(ID_OP_ELEV_CROP, OnUpdateHaveElevMap)
  ON_UPDATE_COMMAND_UI(ID_OP_DLPR_CROP, OnUpdateHaveDLPRmap)
  ON_UPDATE_COMMAND_UI(ID_OP_ANALYZE_BASINS, OnUpdateOpAnalyze)
  ON_UPDATE_COMMAND_UI(ID_WINDOW_PROFILE, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_ERROR_ANAL, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_TREE_SHORELINE, OnUpdateHaveEMDT)
  ON_UPDATE_COMMAND_UI(ID_OP_PURGE_NOISE, OnUpdateHaveEMDT)
  ON_UPDATE_COMMAND_UI(ID_OP_REVERT, OnUpdateHaveEMDT)
  ON_UPDATE_COMMAND_UI(ID_OP_RESORT_ALL, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_DATA_PURGE, OnUpdateHaveDatabase)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RIDGE_OPTIONS, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_INFO_ALL, OnUpdateHaveAny)
  ON_UPDATE_COMMAND_UI(ID_LIST_EDITED, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_SS_PROM, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_SS_UNKNOWN, OnUpdateWriteableDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_OP_SS_REAL, OnUpdateWriteableDLPRtree)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPromDoc construction/destruction

CPromDoc::CPromDoc()
{
  working=false;
  auto_update=true;
  is_drainage=false;
  prom_zp=true;
  prom_ss=false;
  shore_basin_elev=shore_sadl_elev=0;
  shore_set_edit=shore_mark_edit=true;
}

CPromDoc::~CPromDoc()
{
  copyPaste.clear();
  dragDrop.clear();
}


char CPromDoc::buf[1024];

Location_fmt CPromDoc::status_fmt;

// these are used as dummy features in view updates
Feature
  CPromDoc::dummyModifyAll,	// indicates any or all features may have been modified
  CPromDoc::dummyModifyTrans,	// indicates elevation transformation
  CPromDoc::dummyModifyBounds,	// indicates workspace bounds have changed
  CPromDoc::dummyModifyMap,	// indicates elevation map or domain map changed
  CPromDoc::dummyModifyTopo,	// indicates topology of peak/saddle backbone changed
  CPromDoc::dummyModifyRObase,	// indicates runoff array moved
  CPromDoc::dummyModifyBSbase,	// indicates basin saddle array moved
  CPromDoc::dummyModifySplice,	// indicates splice (implies topology and BS array moved)
  CPromDoc::dummyModifyData,	// indicates database changed
  CPromDoc::dummySaveCenter;	// advises map views to save their positions

BOOL CPromDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  em_state=dm_state=dt_state=db_state=NOT_LOADED;
  em_save_drastic_change=dm_save_drastic_change=
    dt_save_drastic_change=db_save_drastic_change=false;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  total_alloc.empty();
  total_defined.empty();
  em_name.Empty();
  dm_name.Empty();
  dt_name.Empty();
  db_name.Empty();
  n_domain=0;
  tree_writeable=true;

  return TRUE;
}


void CPromDoc::UpdateViews(Feature *change)
{
  if (working) {
    if (change==0) hell_change=true;
    else if (change==&dummyModifyTrans) {
      work_b=trans_m*work_b+trans_b;
      work_m*=trans_m;
      trans_change=true;
    }
    else if (change==&dummyModifyBounds) bounds_change=true;
    else if (change==&dummyModifyMap) map_change=true;
    else if (change==&dummyModifyTopo) topo_change=true;
    else if (change==&dummyModifySplice) splice_change=true;
    else if (change==&dummyModifyRObase) RObase_change=true;
    else if (change==&dummyModifyBSbase) BSbase_change=true;
    else if (change==&dummyModifyData) data_change=true;
    else if (change==&dummySaveCenter) {}
    else feature_change=true;
  }
  else UpdateAllViews(NULL,(LPARAM)change);
}

void CPromDoc::BeginWork()
{
  work_m=1;
  work_b=0;
  UpdateAllViews(0,(LPARAM)&dummySaveCenter);
  hell_change=feature_change=trans_change=
    bounds_change=map_change=topo_change=
    splice_change=RObase_change=BSbase_change=data_change=false;
  initial_drain=is_drainage;
  working=true;
}

void CPromDoc::EndWork()
{
  if (hell_change) UpdateAllViews(0);
  else {
    if (trans_change) {
      trans_m=work_m;
      trans_b=work_b;
      UpdateAllViews(0,(LPARAM)&dummyModifyTrans);
    }
    if (feature_change) UpdateAllViews(0,(LPARAM)&dummyModifyAll);
    if (bounds_change) UpdateAllViews(0,(LPARAM)&dummyModifyBounds);
    if (map_change) UpdateAllViews(0,(LPARAM)&dummyModifyMap);
    if (topo_change) UpdateAllViews(0,(LPARAM)&dummyModifyTopo);
    if (splice_change) UpdateAllViews(0,(LPARAM)&dummyModifySplice);
    else if (BSbase_change) UpdateAllViews(0,(LPARAM)&dummyModifyBSbase);
    if (RObase_change) UpdateAllViews(0,(LPARAM)&dummyModifyRObase);
    if (data_change) UpdateAllViews(0,(LPARAM)&dummyModifyData);
  }

  working=false;

  if (is_drainage!=initial_drain) set_tb_bmp();
}

void CPromDoc::modify_topo()
{
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateViews(&dummyModifyTopo);
}

void CPromDoc::enable_submenu(CCmdUI *pCmdUI, bool state)
{
  pCmdUI->m_pMenu->EnableMenuItem(pCmdUI->m_nIndex,
    MF_BYPOSITION|(state?MF_ENABLED:MF_GRAYED));
}

void CPromDoc::setup_popup(CMenu *popup, const Feature& featr, FT_index ft) const
{
  switch (ft) {
  case FT_PEAK:
    popup->EnableMenuItem(ID_FEATURE_LIST_SADDLES,MF_BYCOMMAND|MF_ENABLED);
    popup->EnableMenuItem(ID_FEATURE_LIST_PARENTS,MF_BYCOMMAND|MF_ENABLED);
    popup->EnableMenuItem(ID_FEATURE_LIST_NBRS,MF_BYCOMMAND|MF_ENABLED);
    popup->EnableMenuItem(ID_FEATURE_LIST_TREE,MF_BYCOMMAND|MF_ENABLED);
    break;
  case FT_SADDLE:
    break;
  case FT_RUNOFF:
    popup->EnableMenuItem(ID_RUNOFF_SPLICE,MF_BYCOMMAND|MF_ENABLED);
    popup->EnableMenuItem(ID_FEATURE_LIST_NBRS,MF_BYCOMMAND|MF_ENABLED);
    break;
  case FT_BASINSADL:
    popup->EnableMenuItem(ID_FEATURE_LIST_SADDLES,MF_BYCOMMAND|MF_ENABLED);
    popup->ModifyMenu(ID_FEATURE_LIST_SADDLES,MF_BYCOMMAND|MF_STRING,
		      ID_FEATURE_LIST_SADDLES,"&Cycle...");
    popup->EnableMenuItem(ID_BASINSADL_ROTATE,MF_BYCOMMAND|MF_ENABLED);
    break;
  default:
    assert(0);
  }
  if (em_state!=NOT_LOADED && elev.contains(featr.location)) {
    popup->EnableMenuItem(ID_FEATURE_RQF,MF_BYCOMMAND|MF_ENABLED);
    popup->EnableMenuItem(ID_IMPRESS,MF_BYCOMMAND|MF_ENABLED);
    popup->EnableMenuItem(ID_FREE_HEIGHT,MF_BYCOMMAND|MF_ENABLED);
  }
  if (!tree_writeable || working) {
    popup->EnableMenuItem(ID_FEATURE_EDIT,MF_BYCOMMAND|MF_GRAYED);
    popup->EnableMenuItem(ID_RUNOFF_SPLICE,MF_BYCOMMAND|MF_GRAYED);
    popup->EnableMenuItem(ID_BASINSADL_ROTATE,MF_BYCOMMAND|MF_GRAYED);
    popup->EnableMenuItem(ID_FEATURE_DELETE,MF_BYCOMMAND|MF_GRAYED);
  }
}


/////////////////////////////////////////////////////////////////////////////
// CPromDoc diagnostics

#ifdef _DEBUG
void CPromDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CPromDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

Elev_endpt CPromDoc::inner_endpt() const
{
  return is_drainage ? HI_END : LO_END;
}

Elev_endpt CPromDoc::outer_endpt() const
{
  return is_drainage ? LO_END : HI_END;
}


/////////////////////////////////////////////////////////////////////////////
// CPromDoc commands

CString CPromDoc::GetFileName(FileOp op, const char *initial_name,
			      const char *title, const char *ext, CWnd *parent)
{
  static const char *op_text[]={"Open","Merge","Save"};
  CString name;
  sprintf(buf,"%s files (*.%s)|*.%s|"
	      "Prominence files (*.elv,*.domap,*.divt,*.wpd,*.wps,*.wpm)|*.elv;*.domap;*.divt;*.wpd;*.wps;*.wpm|"
	      "All files (*.*)|*.*||",
	      title,ext,ext);

  CFileDialog file_dlg(op!=FO_SAVE,ext,initial_name,
    OFN_HIDEREADONLY|(op!=FO_SAVE?OFN_FILEMUSTEXIST:OFN_OVERWRITEPROMPT),
    buf,parent);

  sprintf(buf,"%s %s file",op_text[op],title);
  file_dlg.m_ofn.lpstrTitle=buf;
  // TODO: change Open/Save button label to Merge?

  if (file_dlg.DoModal() == IDOK) name=file_dlg.GetPathName();
  return name;
}

void CPromDoc::FileError(const char *type, const char *filename, const char *text)
{
  sprintf(buf,"Error %s file \"%s\":\n%s",type,filename,text);
  AfxMessageBox(buf,MB_OK|MB_ICONERROR);
}

void CPromDoc::OnOpenElevMap()
{
  if (!em_CanClose()) return;
  CString name=GetFileName(FO_OPEN,NULL,"Elevation Map","elv");
  if (name.IsEmpty()) return;
  readElevMap(name);
}

bool CPromDoc::readElevMap(const char *name)
{
  BeginWaitCursor();
  bool status;
  em_state=NOT_LOADED;
  CPromApp::getApp()->check_all_closed();
  try {
    elev.read(name);
    em_state=NOT_CHANGED;
    em_name=name;
    status=true;
  }
  catch (file_error e) {
    FileError("reading elevation map",name,e.what());
    em_name.Empty();
    elev.set_bound(0,0);
    status=false;
  }
  catch (bad_alloc) {
    FileError("reading elevation map",name,"Memory exhaustion");
  }
  CPromApp *app=CPromApp::getApp();
  if (app->elev_map_clip==this) app->elev_map_clip=0;
  em_save_drastic_change=false;
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  SetViewTitles();
  EndWaitCursor();
  return status;
}

void CPromDoc::OnOpenDLPRmap()
{
  if (!dm_CanClose()) return;
  CString name=GetFileName(FO_OPEN,NULL,"Domain Map","domap");
  if (name.IsEmpty()) return;
  readDomap(name);
}

bool CPromDoc::readDomap(const char *name)
{
  BeginWaitCursor();
  bool status;
  dm_state=NOT_LOADED;
  CPromApp::getApp()->check_all_closed();
  try {
    domain.read(name);
    dm_state=NOT_CHANGED;
    dm_name=name;
    n_domain=0;
    Domain::Index d;
    for (Rect_iter_all gi(domain); gi; ++gi) {
      d=domain[*gi];
      if (d!=Domain::undef && d>=n_domain) n_domain=d+1;
    }
    if (dt_state!=NOT_LOADED) resolveDomap();
    status=true;
  }
  catch (file_error e) {
    FileError("reading domain map",name,e.what());
    dm_name.Empty();
    domain.set_bound(0,0);
    status=false;
  }
  catch (bad_alloc) {
    FileError("reading domain map",name,"Memory exhaustion");
  }
  CPromApp *app=CPromApp::getApp();
  if (app->domap_clip==this) app->domap_clip=0;
  dm_save_drastic_change=false;
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  SetViewTitles();
  EndWaitCursor();
  return status;
}

static bool resolve_domap_drg;

static void undef_domain_msg()
{
  sprintf(CPromDoc::buf,
	  "A %s occurs in an undefined domain.\n"
	  "You have loaded a domain map and %s tree\n"
	  "that don't correspond to each other.",
	  resolve_domap_drg?"basin":"peak",
	  resolve_domap_drg?"drainage":"divide");
  AfxGetMainWnd()->MessageBox(CPromDoc::buf,
	"Winprom Domain Map Warning",MB_OK|MB_ICONWARNING);
}

static void multiple_domain_msg()
{
  sprintf(CPromDoc::buf,
	  "There is more than one %s in the same domain.\n"
	  "You have loaded a domain map and %s tree\n"
	  "that don't correspond to each other.",
	  resolve_domap_drg?"basin":"peak",
	  resolve_domap_drg?"drainage":"divide");
  AfxGetMainWnd()->MessageBox(CPromDoc::buf,
	"Winprom Domain Map Warning",MB_OK|MB_ICONWARNING);
}

void CPromDoc::resolveDomap()
{
  resolve_domap_drg=is_drainage;
  tree.resolve_domap(domain,n_domain,undef_domain_msg,multiple_domain_msg);
}

void CPromDoc::OnOpenDLPRtree()
{
  if (!dt_CanClose()) return;
  CString name=GetFileName(FO_OPEN,NULL,"Divide Tree","divt");
  if (name.IsEmpty()) return;
  readDivideTree(name);
}

bool CPromDoc::readDivideTree(const char *name)
{
  BeginWaitCursor();
  bool status;
  dt_state=NOT_LOADED;
  CPromApp::getApp()->check_all_closed();
  try {
    tree.read(name);
    // arm here, succeeds
    dt_state=NOT_CHANGED;
    // arm here, crashes before reaching
    dt_name=name;
    if (dm_state!=NOT_LOADED) resolveDomap();
    status=true;
    Domain::Index ndiv,ndrg;
    tree.div_drg_count(ndiv,ndrg);
    is_drainage=ndrg>ndiv;
#ifdef administrator
    if (ndrg*100>ndiv && ndiv*100>ndrg) {
#else
    if (ndrg*10>ndiv && ndiv*10>ndrg) {
#endif
      sprintf(buf,"Tree is ambiguous as to whether it\'s divide or drainage\n"
		  "Divide count = %d   Drainage count = %d",ndiv,ndrg);
      AfxGetMainWnd()->MessageBox(buf,"Winprom Tree Warning",MB_ICONWARNING);
    }
    set_tb_bmp();
  }
  catch (file_error e) {
    FileError("reading divide tree",name,e.what());
    dt_name.Empty();
    tree.clear();
    status=false;
  }
  catch (bad_alloc) {
    FileError("reading divide tree",name,"Memory exhaustion");
  }
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  CPromApp *app=CPromApp::getApp();
  if (app->divide_tree_clip==this) app->divide_tree_clip=0;
  copyPaste.clear();
  dragDrop.clear();
  dt_save_drastic_change=false;
  CalcTotalArea();
  UpdateViews(0);
  SetViewTitles();
  EndWaitCursor();
  return status;
}

void CPromDoc::OnOpenDatabase()
{
  if (!db_CanClose()) return;
  CString name=GetFileName(FO_OPEN,NULL,"Database","wpd");
  if (name.IsEmpty()) return;
  readDatabase(name);
}

void CPromDoc::readDatabase(const char *name)
{
  BeginWaitCursor();
  db_state=NOT_LOADED;
  CPromApp::getApp()->check_all_closed();
  try {
    data.read(name);
    db_state=NOT_CHANGED;
    db_name=name;
  }
  catch (file_error e) {
    FileError("reading database",name,e.what());
    db_name.Empty();
    data.empty();
  }
  catch (bad_alloc) {
    FileError("reading database",name,"Memory exhaustion");
  }
  CPromApp *app=CPromApp::getApp();
  if (app->database_clip==this) app->database_clip=0;
  db_save_drastic_change=false;
  CalcTotalArea();
  record_change=REC_NONE;
  UpdateAllViews(0,(LPARAM)&dummyModifyData);
  SetViewTitles();
  EndWaitCursor();
}

void CPromDoc::OnSaveAsDLPRtree()
{
  CString name=GetFileName(FO_SAVE,dt_name,
    is_drainage ? "Drainage tree" : "Divide Tree","divt");
  if (name.IsEmpty()) return;
  saveasDivideTree(name);
}

bool CPromDoc::saveasDivideTree(const char *name)
{
  try {
    tree.write(name);
    dt_state=NOT_CHANGED;
    dt_save_drastic_change=true;
    dt_name=name;
    SetViewTitles();
  }
  catch (file_error e) {
    FileError(is_drainage ? "saving drainage tree" : "saving divide tree",name,e.what());
    return false;
  }
  return true;
}

void CPromDoc::OnSaveAsDLPRmap()
{
  CString name=GetFileName(FO_SAVE,dm_name,"Domain Map","domap");
  if (name.IsEmpty()) return;
  saveasDomap(name);
}

bool CPromDoc::saveasDomap(const char *name)
{
  try {
    domain.write(name);
    dm_state=NOT_CHANGED;
    dm_save_drastic_change=true;
    dm_name=name;
    SetViewTitles();
  }
  catch (file_error e) {
    FileError("saving domain map",name,e.what());
    return false;
  }
  return true;
}

void CPromDoc::OnSaveAsElevMap()
{
  CString name=GetFileName(FO_SAVE,em_name,"Elevation Map","elv");
  if (name.IsEmpty()) return;
  saveasElevMap(name);
}

bool CPromDoc::saveasElevMap(const char *name)
{
  try {
    elev.write(name);
    em_state=NOT_CHANGED;
    em_save_drastic_change=true;
    em_name=name;
    SetViewTitles();
  }
  catch (file_error e) {
    FileError("saving elevation map",name,e.what());
    return false;
  }
  return true;
}

void CPromDoc::OnSaveAsDatabase()
{
  CString name=GetFileName(FO_SAVE,db_name,"Database","wpd");
  if (name.IsEmpty()) return;
  try {
    data.write(name);
    db_state=NOT_CHANGED;
    db_save_drastic_change=true;
    db_name=name;
    SetViewTitles();
  }
  catch (file_error e) {
    FileError("saving database",name,e.what());
  }
}

void CPromDoc::OnUpdateHaveDLPRmap(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dm_state!=NOT_LOADED);
}

void CPromDoc::OnUpdateDLPRfill(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) enable_submenu(pCmdUI,dm_state!=NOT_LOADED);
  else OnUpdateHaveMaps(pCmdUI);
}

void CPromDoc::OnUpdateListPeaks(CCmdUI *pCmdUI)
{
  OnUpdateHaveDLPRtree(pCmdUI);
  pCmdUI->SetText(is_drainage?"&Basins...":"&Peaks...");
}

void CPromDoc::OnUpdateListBasinsadls(CCmdUI *pCmdUI)
{
  OnUpdateHaveDLPRtree(pCmdUI);
  pCmdUI->SetText(is_drainage?"&Prominence saddles...":"&Basin saddles...");
}

void CPromDoc::OnUpdateOpPurgePeaks(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) {
    pCmdUI->m_pMenu->ModifyMenu(pCmdUI->m_nIndex,MF_BYPOSITION|MF_STRING,0,
      is_drainage?"&Basins":"&Peaks");
    enable_submenu(pCmdUI,dt_state!=NOT_LOADED && tree_writeable);
  }
  else OnUpdateWriteableDLPRtree(pCmdUI);
}

void CPromDoc::OnUpdateOpResortPeaks(CCmdUI *pCmdUI)
{
  OnUpdateWriteableDLPRtree(pCmdUI);
  pCmdUI->SetText(is_drainage?"&Basins":"&Peaks");
}

void CPromDoc::OnUpdateOpResortBS(CCmdUI *pCmdUI)
{
  OnUpdateWriteableDLPRtree(pCmdUI);
  pCmdUI->SetText(is_drainage?"&Prom saddles":"&Basin saddles");
}

void CPromDoc::OnUpdateHaveDLPRtree(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dt_state!=NOT_LOADED);
  char a;
  switch (pCmdUI->m_nID) {
  case ID_SAVE_DLPR_TREE: a='S'; break;
  case ID_SAVEAS_DLPR_TREE: a='A'; break;
  case ID_INFO_DLPR_TREE: a='I'; break;
  case ID_CLOSE_DLPR_TREE: a='C'; break;
  case ID_EDIT_COPY_DLPR_TREE: a=' '; break;
  default: a='\0';
  }
  if (a) {
    sprintf(buf,"%s &tree \t%c",is_drainage?"Drainage":"Divide",a);
    pCmdUI->SetText(buf);
  }
}

void CPromDoc::OnUpdateMergeDLPRtree(CCmdUI *pCmdUI)
{
  OnUpdateWriteableDLPRtree(pCmdUI);
  pCmdUI->SetText(is_drainage?"Drainage &tree\tM":"Divide &tree\tM");
}

void CPromDoc::OnUpdateHaveDatabase(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(db_state!=NOT_LOADED);
}

void CPromDoc::OnUpdateWriteableDLPRtree(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dt_state!=NOT_LOADED && tree_writeable);
}

void CPromDoc::OnUpdateSplice(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) {
    pCmdUI->m_pMenu->ModifyMenu(pCmdUI->m_nIndex,MF_BYPOSITION|MF_STRING,0,
      is_drainage?"Drainage &tree":"Divide &tree");
    enable_submenu(pCmdUI,dt_state!=NOT_LOADED && tree_writeable);
  }
  else OnUpdateWriteableDLPRtree(pCmdUI);
}

void CPromDoc::OnUpdateNoFeatureEdit(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(tree_writeable);
}

void CPromDoc::OnUpdateWriteableData(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(db_state!=NOT_LOADED && tree_writeable);
}

void CPromDoc::OnUpdateHaveElevMap(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(em_state!=NOT_LOADED);
}

void CPromDoc::OnUpdateFlatUndefine(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) enable_submenu(pCmdUI,em_state!=NOT_LOADED);
  else OnUpdateHaveElevMap(pCmdUI);
}

void CPromDoc::OnUpdateHaveAny(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(em_state!=NOT_LOADED || dm_state!=NOT_LOADED ||
		 dt_state!=NOT_LOADED || db_state!=NOT_LOADED);
}

void CPromDoc::OnCloseDLPRmap()
{
  if (!dm_CanClose()) return;
  domain.set_bound(0,0);
  dm_state=NOT_LOADED;
  CPromApp *app=CPromApp::getApp();
  app->check_all_closed();
  if (app->domap_clip==this) app->domap_clip=0;
  dm_name.Empty();
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  SetViewTitles();
}

void CPromDoc::OnCloseDLPRtree()
{
  if (!dt_CanClose()) return;
  tree.clear();
  copyPaste.clear();
  dragDrop.clear();
  dt_state=NOT_LOADED;
  CPromApp *app=CPromApp::getApp();
  app->check_all_closed();
  if (app->divide_tree_clip==this) app->divide_tree_clip=0;
  dt_name.Empty();
  CalcTotalArea();
  UpdateViews(0);
  SetViewTitles();
}

void CPromDoc::OnCloseElevMap()
{
  if (!em_CanClose()) return;
  elev.set_bound(0,0);
  em_state=NOT_LOADED;
  CPromApp *app=CPromApp::getApp();
  app->check_all_closed();
  if (app->elev_map_clip==this) app->elev_map_clip=0;
  em_name.Empty();
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  SetViewTitles();
}

void CPromDoc::OnCloseDatabase()
{
  if (!db_CanClose()) return;
  data.empty();
  db_state=NOT_LOADED;
  CPromApp *app=CPromApp::getApp();
  app->check_all_closed();
  if (app->database_clip==this) app->database_clip=0;
  copyPaste.orph_list.clear();
  dragDrop.orph_list.clear();
  db_name.Empty();
  CalcTotalArea();
  UpdateAllViews(NULL,(LPARAM)&dummyModifyData);
  SetViewTitles();
}

void CPromDoc::OnCloseAll()
{
  OnCloseDLPRmap();
  OnCloseDLPRtree();
  OnCloseElevMap();
  OnCloseDatabase();
}

void CPromDoc::OnUpdateCloseAll(CCmdUI *pCmdUI)
{
  pCmdUI->Enable((em_state!=NOT_LOADED || dm_state!=NOT_LOADED ||
		  dt_state!=NOT_LOADED || db_state!=NOT_LOADED) &&
		 tree_writeable);
}

void CPromDoc::OnCloseDocument()
{
  assert(tree_writeable);
  CPromApp *app=CPromApp::getApp();
  if (app->elev_map_clip==this) app->elev_map_clip=0;
  if (app->domap_clip==this) app->domap_clip=0;
  if (app->divide_tree_clip==this) app->divide_tree_clip=0;
  CDocument::OnCloseDocument();
}

void CPromDoc::OnSaveDatabase()
{
  if (db_name.IsEmpty()) OnSaveAsDatabase();
  else try {
    data.write(db_name);
    db_state=NOT_CHANGED;
  }
  catch (file_error e) {
    FileError("saving database",db_name,e.what());
  }
}

void CPromDoc::OnSaveAll()
{
  OnSaveElevMap();
  OnSaveDLPRmap();
  OnSaveDLPRtree();
  OnSaveDatabase();
}

void CPromDoc::OnSaveDLPRmap()
{
  if (dm_name.IsEmpty()) OnSaveAsDLPRmap();
  else try {
    domain.write(dm_name);
    dm_state=NOT_CHANGED;
  }
  catch (file_error e) {
    FileError("saving domain map",dm_name,e.what());
  }
}

void CPromDoc::OnSaveDLPRtree()
{
  if (dt_name.IsEmpty()) OnSaveAsDLPRtree();
  else try {
    tree.write(dt_name);
    dt_state=NOT_CHANGED;
  }
  catch (file_error e) {
    FileError(is_drainage ? "saving drainage tree" : "saving divide tree",dt_name,e.what());
  }
}

void CPromDoc::OnSaveElevMap()
{
  if (em_name.IsEmpty()) OnSaveAsElevMap();
  else try {
    elev.write(em_name);
    em_state=NOT_CHANGED;
  }
  catch (file_error e) {
    FileError("saving elevation map",em_name,e.what());
  }
}

void CPromDoc::OnInfoDLPRmap()
{
  BeginWaitCursor();
  Area_info alloc,defined;
  alloc.rect=(Rectangl)domain;

  GetDomapArea(alloc,defined);

  CDomapInfo_dlg dlg(alloc,defined,dm_name,dm_state==CHANGED);
  dlg.m_ndom_total=n_domain;
  dlg.m_ndom_1peak=dlg.m_ndom_peaks=dlg.m_ndom_area=0;
  Domain::Index *npk_dom=new Domain::Index[n_domain],i_dom;
  for (i_dom=0; i_dom<n_domain; ++i_dom) npk_dom[i_dom]=0;
  if (dt_state==NOT_LOADED) {
    dlg.m_ndom_0peak=n_domain;
  }
  else {
    dlg.m_ndom_0peak=0;
    Domain::Index ndom=tree.n_dom();
    for (i_dom=0; i_dom<n_domain; ++i_dom) npk_dom[i_dom]=0;
    for (unsigned i=0; i<ndom; ++i) {
      const GridPoint& pk_loc=tree[i].peak.location;
      if (pk_loc && domain.contains(pk_loc)) {
	i_dom=domain[pk_loc];
	if (i_dom!=Domain::undef) {
	  assert(i_dom<n_domain);
	  ++npk_dom[i_dom];
	}
      }
    }
    for (i_dom=0; i_dom<n_domain; ++i_dom) {
      if (npk_dom[i_dom]==0) ++dlg.m_ndom_0peak;
      else if (npk_dom[i_dom]==1) ++dlg.m_ndom_1peak;
      else ++dlg.m_ndom_peaks;
    }
  }
  for (RawGrid_iter gi((Rectangl)domain); gi; ++gi) {
    i_dom=domain[*gi];
    if (i_dom!=Domain::undef) {
      assert(i_dom<n_domain);
      ++npk_dom[i_dom];
    }
  }
  for (i_dom=0; i_dom<n_domain; ++i_dom)
    if (npk_dom[i_dom]>0) ++dlg.m_ndom_area;
  dlg.m_ndom_0peak-=n_domain-dlg.m_ndom_area;
  delete[] npk_dom;
  EndWaitCursor();
  dlg.DoModal();
}

void CPromDoc::GetDomapArea(Area_info& alloc, Area_info& defined)
{
  float area;

  for (RawGrid_iter gi((Rectangl)domain); gi; ++gi) {
    ++alloc.ngp;
    alloc.area+=area=(float)GridPoint(*gi).area();
    if (domain[*gi]!=Domain::undef) {
      ++defined.ngp;
      defined.area+=area;
      defined.rect|=(*gi);
    }
  }
}

void CPromDoc::OnInfoElevMap()
{
  BeginWaitCursor();
  Area_info alloc,defined;
  alloc.rect=(Rectangl)elev;
  Elev_intvl elev_range;

  GetElevMapArea(alloc,defined,elev_range);

  CElevMapInfo_dlg dlg(alloc,defined,em_name,em_state==CHANGED);
  dlg.m_elev_low=elev_range.low;
  dlg.m_elev_high=elev_range.high;
  EndWaitCursor();
  dlg.DoModal();
}

void CPromDoc::GetElevMapArea(Area_info& alloc, Area_info& defined,
			      Elev_intvl& elev_range)
{
  float area;
  Elevation elv;

  for (RawGrid_iter gi((Rectangl)elev); gi; ++gi) {
    ++alloc.ngp;
    alloc.area+=area=(float)GridPoint(*gi).area();
    if ((elv=elev[*gi])!=elev_undef) {
      ++defined.ngp;
      defined.area+=area;
      defined.rect|=(*gi);
      elev_range|=elv;
    }
  }
}

void CPromDoc::CalcTotalArea()
{
  BeginWaitCursor();
  UpdateViews(&dummySaveCenter);
  Elev_intvl dummy; // keep GetElevMapArea() happy
  total_alloc.empty();
  total_defined.empty();
  if (dm_state!=NOT_LOADED) {
    total_alloc.rect=(Rectangl)domain;
    GetDomapArea(total_alloc,total_defined);
  }
  if (em_state!=NOT_LOADED) {
    total_alloc.rect|=(Rectangl)elev;
    GetElevMapArea(total_alloc,total_defined,dummy);
  }
  if (dt_state!=NOT_LOADED) {
    tree.area(total_alloc.rect,total_defined.rect);
  }
  EndWaitCursor();
}

void CPromDoc::setMarkBound()
{
  if (em_state!=NOT_LOADED) {
    mark_bound=elev;
    if (dm_state!=NOT_LOADED) {
      mark_bound|=domain;
      if (mark_bound.area()>2*(elev.area()+domain.area()))
	mark_bound=elev;
    }
  }
  else {
    if (dm_state!=NOT_LOADED) 
      mark_bound=domain;
    else mark_bound.empty();
  }
}

void CPromDoc::OnInfoDLPRtree()
{
  CTreeInfo_dlg(*this).DoModal();
}

void CPromDoc::OnInfoDatabase()
{
  CDataInfo_dlg(*this).DoModal();
}

void CPromDoc::OnInfoAll()
{
  CWSinfo_dlg dlg(total_alloc,total_defined);
  dlg.DoModal();
}

bool CPromDoc::doc_CanClose(const File_state& state, const char *title,
			    const CString& name, void (CPromDoc::*save_fn)(),
			    bool drastic) const
{
  if (state!=CHANGED) return true;
  char *lbuf=buf;
  if (drastic)
    lbuf+=sprintf(buf,
	"The operation you are about to perform is a \"drastic change\",\n"
	"which causes widespread and irreversible modifications to the\n"
	"document.  Most drastic changes are terminal operations whose\n"
	"results are temporary and usually not saved.  It is generally\n"
	"advisable to save your work before performing a drastic change.\n\n");
  if (!name.IsEmpty())
    sprintf(lbuf,"Would you like to save the %s to%c\"%s\"?",
	    title,drastic?' ':'\n',(LPCTSTR)name);
  else sprintf(lbuf,"Would you like to save the %s?",title);

  switch (AfxGetMainWnd()->MessageBox(buf,
	  drastic?"Winprom Drastic Change":"Winprom Workspace Changed",
	  MB_YESNOCANCEL|MB_ICONWARNING)) {
  case IDYES:
    (const_cast<CPromDoc *>(this)->*save_fn)();
    return state!=CHANGED;
  case IDNO:
    return true;
  case IDCANCEL:
    return false;
  }
  assert(0);
  return false;
}

bool CPromDoc::em_CanClose(bool drastic) const
{
  return doc_CanClose(em_state,"elevation map",em_name,
		      &CPromDoc::OnSaveElevMap,drastic);
}

bool CPromDoc::dm_CanClose(bool drastic) const
{
  return doc_CanClose(dm_state,"domain map",dm_name,
		      &CPromDoc::OnSaveDLPRmap,drastic);
}

bool CPromDoc::dt_CanClose(bool drastic) const
{
  if (!tree_writeable) {
    AfxGetMainWnd()->
      MessageBox("Can\'t close worskpace.  You have a feature dialog open for editing.",
		 "Winprom",MB_ICONERROR);
    return false;
  }
  return doc_CanClose(dt_state,is_drainage ? "drainage tree" : "divide tree",dt_name,
		      &CPromDoc::OnSaveDLPRtree,drastic);
}

bool CPromDoc::db_CanClose(bool drastic) const
{
  return doc_CanClose(db_state,"database",db_name,
		      &CPromDoc::OnSaveDatabase,drastic);
}

BOOL CPromDoc::SaveModified()
{
  return em_CanClose() && dm_CanClose() && dt_CanClose() && db_CanClose();
}

void CPromDoc::SetViewTitles() const
{
  CView *v;
  POSITION p=GetFirstViewPosition();
  while (p!=NULL) {
    v=GetNextView(p);
    if (v->IsKindOf(RUNTIME_CLASS(CFeatureListView)))
      ((CFeatureListView *)v)->SetViewTitle();
    else if (v->IsKindOf(RUNTIME_CLASS(CGraphView)))
      ((CGraphView *)v)->SetViewTitle();
    else if (v->IsKindOf(RUNTIME_CLASS(CLPRtreeView)))
      ((CLPRtreeView *)v)->SetViewTitle();
    else if (v->IsKindOf(RUNTIME_CLASS(CDataListView)))
      ((CDataListView *)v)->SetViewTitle();
  }
}

void CPromDoc::OnMergeElevMap()
{
  if (!em_CanClose(true)) return;
  CString name=GetFileName(FO_MERGE,NULL,"Elevation Map","elv");
  if (name.IsEmpty()) return;
  mergeElevMap(name);
}

bool CPromDoc::mergeElevMap(const char *name)
{
  FILE *new_elev_file=fopen(name,"rb");
  if (!new_elev_file) {
    FileError("reading elevation map",name);
    return false;
  }
  Rectangl new_bound,merged_bound=(Rectangl)elev;
  fread(new_bound,new_elev_file);
  merged_bound|=new_bound;
  if (merged_bound.area()>2*(Rectangl(elev).area()+new_bound.area())) {
    sprintf(buf,"The combined map is rather large compared to original maps.\n"
	    "Combined map area %ld\nOld map area %ld\nNew map area %ld\n"
	    "Do you wish to continue the operation?",
	    merged_bound.area(),Rectangl(elev).area(),new_bound.area());
    if (AfxGetMainWnd()->MessageBox(buf,"Winprom Elevation Map Merge",
		MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2)!=IDYES) {
      fclose(new_elev_file);
      return false;
    }
  }

  Elev_map temp_elev((Rectangl)elev);
  temp_elev.transfer(elev);
  try {
    elev.set_bound(merged_bound);
  }
  catch (bad_alloc) {
    fclose(new_elev_file);
    AfxGetMainWnd()->MessageBox("Memory allocation error for combined map",
		"Winprom Elevation Map Merge",MB_OK|MB_ICONERROR);
    // Recover the original elevation map from temp_elev
    elev.set_bound(Rectangl(temp_elev));
    elev.transfer(temp_elev);
    return false;
  }

  BeginWaitCursor();
  elev.fill(elev_undef);
  elev.transfer(temp_elev);
  rewind(new_elev_file);
  try {
    temp_elev.read(new_elev_file);
  }
  catch (file_error e) {
    FileError("reading elevation map",name,e.what());
    fclose(new_elev_file);
    EndWaitCursor();
    return false;
  }
  catch (bad_alloc) {
    FileError("reading elevation map",name,"Memory exhaustion");
  }
  fclose(new_elev_file);

  GridPoint::Coord nagree=0,nconflict=0,dconflict2=0,max_conflict=0;
  Elevation delta;
  CElevConflict_dlg dlg;
  dlg.m_warn=FALSE;
  dlg.m_use=4;
  status_fmt.set();
  bool warn_conflict=true;
  for (Rect_iter_all gi=Rectangl(temp_elev); gi; ++gi) {
    if (elev[*gi]==elev_undef)
      elev[*gi]=temp_elev[*gi];
    else if (elev[*gi]==temp_elev[*gi])
      ++nagree;
    else if (temp_elev[*gi]!=elev_undef) {
      ++nconflict;
      delta=abs(elev[*gi]-temp_elev[*gi]);
      if (delta>max_conflict) max_conflict=delta;
      dconflict2+=delta*delta;
      if (warn_conflict) {
	(*gi).sprint(buf);
	dlg.m_location=buf;
	dlg.m_workspace=elev[*gi];
	dlg.m_file=temp_elev[*gi];
	if (dlg.DoModal()==IDC_ABORT) {
	  break;
	}
	warn_conflict=dlg.m_warn!=FALSE;
      }
      switch (dlg.m_use) {
      case 0: break;
      case 1: elev[*gi]=temp_elev[*gi]; break;
      case 2:
	if (elev[*gi]<temp_elev[*gi]) elev[*gi]=temp_elev[*gi];
	break;
      case 3:
	elev[*gi]+=(temp_elev[*gi]-elev[*gi])/2;
	break;
      case 4:
	if (elev[*gi]>temp_elev[*gi]) elev[*gi]=temp_elev[*gi];
	break;
      }

    } // disagreement action

  } // loop over temp_elev

  // summarize
  em_state=CHANGED;
  CPromApp *app=CPromApp::getApp();
  if (app->elev_map_clip==this) app->elev_map_clip=0;
  if (!em_save_drastic_change) {
    em_name.Empty();
    SetViewTitles();
  }
  CalcTotalArea();
  setMarkBound();
  if (nconflict>0) {
    sprintf(buf,"%ld elevation agreements found.\n"
	    "%ld elevation conflicts found.\n"
	    "RMS error = %3.0lf\nMaximum error = %d",
	    nagree,nconflict,sqrt((double)dconflict2/nconflict),max_conflict);
    AfxGetMainWnd()->
      MessageBox(buf,"Winprom Elevation Map Merge",MB_OK|MB_ICONWARNING);
  }
  UpdateViews(&dummyModifyBounds);
  EndWaitCursor();
  return true;
}

void CPromDoc::OnMergeDLPRtree()
{
  if (!dt_CanClose(true)) return;
  CString name=GetFileName(FO_MERGE,NULL,
    is_drainage ? "Drainage Tree" : "Divide Tree","divt");
  if (name.IsEmpty()) return;
  mergeDivideTree(name);
}

bool CPromDoc::mergeDivideTree(const char *name)
{
  Divide_tree in_tree;
  BeginWaitCursor();
  bool status;
  try {
    in_tree.read(name);
    Domain::Index ndiv,ndrg;
    in_tree.div_drg_count(ndiv,ndrg);
    if (ndrg>ndiv != is_drainage) {
      switch (::MessageBox(NULL,"Divide/drainage type conflict.\n"
			   "Do you want to invert the incoming tree?",
			   "Winprom Tree Warning",MB_ICONWARNING|MB_YESNOCANCEL)) {
      case IDYES:
	in_tree.transform(-1,0,true,true,true,true,true);
      case IDNO:
	break;
      case IDCANCEL: return false;
      }
    }
    tree.compress_all();
    in_tree.compress_all();
    tree.merge(in_tree);
    dt_state=CHANGED;
    peak_prom_utd=sadl_prom_utd=ridge_utd=false;
    CPromApp *app=CPromApp::getApp();
    if (app->divide_tree_clip==this) app->divide_tree_clip=0;
    if (!dt_save_drastic_change) {
      dt_name.Empty();
      SetViewTitles();
    }
    CalcTotalArea();
    if (dm_state!=NOT_LOADED) resolveDomap();
    UpdateViews(0);
    status=true;
  }
  catch (file_error e) {
    FileError(is_drainage ? "reading drainage tree" : "reading divide tree",name,e.what());
    status=false;
  }
  EndWaitCursor();
  return status;
}

void CPromDoc::OnMergeDLPRmap()
{
  if (!dm_CanClose(true)) return;
  CString name=GetFileName(FO_MERGE,NULL,"Domain Map","domap");
  if (name.IsEmpty()) return;
  mergeDomap(name);
}

bool CPromDoc::mergeDomap(const char *name)
{
  FILE *new_domap_file=fopen(name,"rb");
  if (!new_domap_file) {
    FileError("reading domain map",name);
    return false;
  }
  Rectangl new_bound,merged_bound=(Rectangl)domain;
  fread(new_bound,new_domap_file);
  merged_bound|=new_bound;
  if (merged_bound.area()>2*(Rectangl(domain).area()+new_bound.area())) {
    sprintf(buf,"The combined map is rather large compared to original maps.\n"
	    "Combined map area %ld\nOld map area %ld\nNew map area %ld\n"
	    "Do you wish to continue the operation?",
	    merged_bound.area(),Rectangl(domain).area(),new_bound.area());
    if (AfxGetMainWnd()->MessageBox(buf,"Winprom Domain Map Merge",
		MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2)!=IDYES) {
      fclose(new_domap_file);
      return false;
    }
  }

  Index_map temp_domain((Rectangl)domain);
  temp_domain.transfer(domain); // temp_domain now has the old map
  try {
    domain.set_bound(merged_bound); // expand the domain map to the new dimensions
  }
  catch (bad_alloc) {
    fclose(new_domap_file);
    AfxGetMainWnd()->MessageBox("Memory allocation error for combined map",
		"Winprom Domain Map Merge",MB_OK|MB_ICONERROR);
    // Recover the original domain map from temp_domain
    domain.set_bound(Rectangl(temp_domain));
    domain.transfer(temp_domain);
    return false;
  }

  BeginWaitCursor();
  domain.fill(Domain::undef);
  domain.transfer(temp_domain); // "domain" now has the old map with expanded dimensions
  rewind(new_domap_file);
  try {
    temp_domain.read(new_domap_file); // "temp_domain" now has the new map
  }
  catch (file_error e) {
    FileError("reading domain map",name,e.what());
    fclose(new_domap_file);
    EndWaitCursor();
    return false;
  }
  fclose(new_domap_file);

  // transfer defined part of new map to merged map.
  // Domain indices of new map will be translated so as not to duplicate
  // the indices of the old map.
  Domain::Index n_new_domap=n_domain;
  for (Rect_iter_all gi=Rectangl(temp_domain); gi; ++gi) {
    if (temp_domain[*gi]!=Domain::undef) {
      Domain::Index& d=domain[*gi];
      d=temp_domain[*gi]+n_domain;
      if (d>n_new_domap) n_new_domap=d;
    }
  }
  n_domain=n_new_domap;
  if (dt_state!=NOT_LOADED) resolveDomap();

  dm_state=CHANGED;
  CPromApp *app=CPromApp::getApp();
  if (app->domap_clip==this) app->domap_clip=0;
  if (!dm_save_drastic_change) {
    dm_name.Empty();
    SetViewTitles();
  }
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  EndWaitCursor();
  return true;
}

void CPromDoc::OnMergeDatabase()
{
  if (!db_CanClose()) return;
  CString name=GetFileName(FO_MERGE,NULL,"Database","wpd");
  if (name.IsEmpty()) return;
  Database data_in;
  try {
    data_in.read(name);
    const char *msg=data.merge(data_in);
    if (msg) {
      sprintf(buf,"Type conflict for field \"%s\".",msg);
      AfxGetMainWnd()->
	MessageBox(buf,"Winprom Database Merge Error",MB_OK|MB_ICONERROR);		    
    }
    else db_state=CHANGED;
  }
  catch (file_error e) {
    FileError("reading database",name,e.what());
  }
}


void CPromDoc::OnListPeaks()
{
  CPeakListView::init_list(this);
}

void CPromDoc::OnListSaddles()
{
  CSaddleListView::init_list(this);
}

void CPromDoc::OnListRunoffs()
{
  CRunoffListView::init_list(this);
}

void CPromDoc::OnListBasinsadls()
{
  CBasinSadlListView::init_list(this);
}

void CPromDoc::OnListGeneric()
// this handles "List->Enpty", not "List->Generic"
// the name is historical
{
  CFeatureListView::init_list(this);
}

void CPromDoc::OnOpSplice()
{
  BeginWaitCursor();
  old_n=tree.n_bsnsdl();
  old_bs=old_n>0 ? &tree.bsnsdl(0) : 0;
  tree.splice();
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateViews(&dummyModifySplice);
  EndWaitCursor();
}

void CPromDoc::OnOpAnalyze()
{
  analyze(false);
}

void CPromDoc::analyze(bool drainage)
{
#ifdef administrator
  if (!dm_CanClose() || !dt_CanClose()) return;

  BeginWaitCursor();
  dm_state=dt_state=NOT_LOADED;
  CAnalyzeCtl_dlg dlg(*this);
  is_drainage=drainage;
  elev_flip_drain();
  if (dlg.DoModal()==IDOK) {
    post_analyze();
    tree_flip_drain();
  }
  else {
    domain.set_bound(0,0);
    tree.clear();
    sprintf(buf,"Error: %s",dlg.error);
    AfxGetMainWnd()->MessageBox(buf,"Winprom",MB_OK|MB_ICONERROR);
  }
  elev_flip_drain();
  post_post_analyze();
  EndWaitCursor();
#else
  sprintf(buf,"The analyzer is not implemented in this shipment of Winprom.\n"
	  "Contact the Custodian for the %s tree(s) of interest.",
	  drainage ? "drainage" : "divide");
  AfxGetMainWnd()->MessageBox(buf,"Winprom",MB_OK|MB_ICONERROR);
#endif // administrator
}

void CPromDoc::OnOpAnalyzeBasins()
{
  analyze(true);
}

void CPromDoc::post_analyze()
{
  dm_state=dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  n_domain=tree.n_dom();
  tree.set_neighbors();
  tree.set_runoffs();
  tree.set_bsnsdls();
  tree.set_cycle_info();
}

void CPromDoc::post_post_analyze()
{
  dm_name.Empty();
  dt_name.Empty();
  CalcTotalArea();
  setMarkBound();
  set_tb_bmp();
  UpdateViews(0);
  SetViewTitles();
}

void CPromDoc::rotate()
{
  tree_flip_drain();
  tree.rotate();
  tree_flip_drain();
  modify_topo();
  UpdateViews(&dummyModifyAll);
}

void CPromDoc::OnOpRotate()
{
  BeginWaitCursor();
  rotate();
  EndWaitCursor();
}

void CPromDoc::dt_apply_drastic_change()
{
  CPromApp *app=CPromApp::getApp();
  if (app->divide_tree_clip==this) app->divide_tree_clip=0;
  if (!dt_save_drastic_change) {
    dt_name.Empty();
    SetViewTitles();
  }
  modify_topo();
}

void CPromDoc::purge_basin_saddles(Elevation walk_risk)
{
  BeginWaitCursor();
  short int dc=drain_const();
  for (Domain::Index ibs=0; ibs<tree.n_bsnsdl(); ++ibs) {
    const Basin_saddle& bs=tree.bsnsdl(ibs);
    if (bs.location) {
      Basin_saddle::Cycle_walk_info cwinfo;
      bs.get_walk_info(cwinfo,dc);
      if (dc*tree.bsnsdl(ibs).walk_risk(cwinfo)<walk_risk) {
	tree.bsnsdl(ibs).remove();
      }
    }
  }
  dt_apply_drastic_change();
  UpdateViews(&dummyModifyAll);
  EndWaitCursor();
}

void CPromDoc::purge_bs_redun()
{
  BeginWaitCursor();
  int ibs,jbs;
  Domain *far_peak;
  tree_flip_drain();
  for (Domain::Index ipeak=0; ipeak<tree.n_dom(); ++ipeak) {
    Domain& peak=tree[ipeak];
    for (ibs=0; ibs<peak.bsnsdls.size(); ++ibs) {
      Basin_saddle& bs1=(*peak.bsnsdls[ibs]);
      if (bs1) {
	far_peak=bs1.other_peak(&peak);
	for (jbs=ibs+1; jbs<peak.bsnsdls.size(); ++jbs) {
	  Basin_saddle& bs2=(*peak.bsnsdls[jbs]);
	  if (bs2 && (bs2.peak1==far_peak || bs2.peak2==far_peak)) {
	    if (bs1.elev.high>=bs2.elev.high) {
	      bs2.remove();
	      --jbs; // don't skip the next neighboring basin saddle!
	    }
	    else {
	      bs1.remove();
	      --ibs; // don't skip the next neighboring basin saddle!
	      break;
	    }
	  }
	}
      }
    }
  }
  tree_flip_drain();
  dt_state=CHANGED;
  UpdateViews(&dummyModifyAll);
  EndWaitCursor();
}

void CPromDoc::OnOpPurgeBsRedun()
{
  purge_bs_redun();
}

void CPromDoc::OnOpPurgeBasinsadls()
{
  if (!dt_CanClose(true)) return;
  purge_basin_saddles(0);
}

void CPromDoc::OnOpPurgeBsDegen()
{
  purge_basin_saddles(-elev_infty+1);
}

void CPromDoc::purge_bs_prom(Elevation min_prom, bool keep_name, bool keep_edit,
			     bool keep_rr, bool keep_cr)
{
  update_sadl_prom_info();
  tree.purge_bs(min_prom,keep_name,keep_edit,prom_zp,keep_rr,keep_cr);
  dt_apply_drastic_change();
  UpdateViews(&dummyModifyAll);
}

void CPromDoc::OnOpPurgeBsProm()
{
  static bool keep_name=true,keep_edit=true,
    keep_ridge_rot=false,keep_cell_rot=false;
  if (!dt_CanClose(true)) return;

  CPurge_dlg dlg(is_drainage?IDI_SADDLE:IDI_BASINSADL);
  dlg.m_keep_name=keep_name;
  dlg.m_keep_edit=keep_edit;
  dlg.m_ridge=keep_ridge_rot;
  dlg.m_cell=keep_cell_rot;
  if (dlg.DoModal()!=IDOK) return;
  keep_name=dlg.m_keep_name!=FALSE;
  keep_edit=dlg.m_keep_edit!=FALSE;
  keep_ridge_rot=dlg.m_ridge!=FALSE;
  keep_cell_rot=dlg.m_cell!=FALSE;

  BeginWaitCursor();
  purge_bs_prom(dlg.m_prom,keep_name,keep_edit,keep_ridge_rot,keep_cell_rot);
  EndWaitCursor();
}

static bool nonpeak_domain_msg()
{
  return
    AfxGetMainWnd()->
      MessageBox("One or more domains has no peak or basin.  Therefore\n"
		 "there is a risk that a real secondary saddle will be removed.\n"
		 "Do you wish to continue with ther operation anyway?",
		 "Winprom Saddle Purge Warning",
		 MB_YESNO|MB_DEFBUTTON2|MB_ICONWARNING)==IDYES;
}

void CPromDoc::purge_bs_drain()
{
  CPromApp *app=CPromApp::getApp();
  if (tree.purge_bs_drainage(app->divide_tree_clip->tree,domain,
			app->domap_clip->domain,elev,is_drainage,nonpeak_domain_msg))
    dt_state=CHANGED;
  UpdateViews(&dummyModifyAll);
}

void CPromDoc::OnOpPurgeBsDrainage()
{
  BeginWaitCursor();
  purge_bs_drain();
  EndWaitCursor();
}

void CPromDoc::OnUpdateOpPurgeBsDrainage(CCmdUI *pCmdUI)
{
  CPromApp *app=CPromApp::getApp();
  pCmdUI->Enable(dm_state!=NOT_LOADED && dt_state!=NOT_LOADED &&
		 em_state!=NOT_LOADED &&
		 app->divide_tree_clip && app->divide_tree_clip!=this);
  pCmdUI->SetText(is_drainage?"Ridge &complement":"Drainage &complement");
}

void CPromDoc::purge_peaks(Elevation min_prom, bool keep_name, bool keep_edit,
			   bool keep_rr, bool keep_cr, bool xlate_domain)
{
  Domain::Index *xlate;
  xlate_domain=xlate_domain&&dm_state!=NOT_LOADED;
  tree_flip_drain();
  update_peak_prom_info();
  update_sadl_prom_info();
  tree.purge_domains(min_prom,keep_name,keep_edit,prom_zp,
		     keep_rr,keep_cr,xlate_domain?&xlate:0);
  tree_flip_drain();
  dt_apply_drastic_change();
  if (xlate_domain) {
    xlate_domap(domain,xlate,tree.n_dom());
    dm_state=CHANGED;
    delete[] xlate;
  }
  UpdateViews(&dummyModifyAll);
}

void CPromDoc::OnOpPurgePeaks()
{
  static bool keep_name=true,keep_edit=true,
    keep_ridge_rot=false,keep_cell_rot=false;
  if (!dt_CanClose(true)) return;

  CPurge_dlg dlg(is_drainage?IDI_BASIN:IDI_PEAK);
  dlg.m_keep_name=keep_name;
  dlg.m_keep_edit=keep_edit;
  dlg.m_ridge=keep_ridge_rot;
  dlg.m_cell=keep_cell_rot;
  if (dlg.DoModal()!=IDOK) return;
  keep_name=dlg.m_keep_name!=FALSE;
  keep_edit=dlg.m_keep_edit!=FALSE;
  keep_ridge_rot=dlg.m_ridge!=FALSE;
  keep_cell_rot=dlg.m_cell!=FALSE;

  static bool xlate_domain=false;
  if (dm_state!=NOT_LOADED) {
    switch (AfxGetMainWnd()->MessageBox(
	    "Would you like to translate the domain map?",
	    "Winprom Peak Purge",MB_ICONINFORMATION|MB_YESNOCANCEL)) {
    case IDYES: xlate_domain=true; break;
    case IDNO: break;
    case IDCANCEL: return;
    }
  }
  BeginWaitCursor();
  purge_peaks(dlg.m_prom,keep_name,keep_edit,
	      keep_ridge_rot,keep_cell_rot,xlate_domain);
  EndWaitCursor();
}

void CPromDoc::purge_noise(float noise, bool keep_name, bool keep_edit,
			   bool hess_nc, bool resid_nc, bool xlate_domain,
			   double& big_noise, Elevation& big_prom_noise)
{
  Domain::Index *xlate;
  xlate_domain=xlate_domain&&dm_state!=NOT_LOADED;
  tree_flip_drain();
  tree.purge_noise(elev,noise,keep_name,keep_edit,hess_nc,resid_nc,
		big_noise,big_prom_noise,xlate_domain?&xlate:0);
  tree_flip_drain();
  dt_apply_drastic_change();
  if (xlate_domain) {
    xlate_domap(domain,xlate,tree.n_dom());
    dm_state=CHANGED;
    delete[] xlate;
  }
  UpdateViews(&dummyModifyAll);
}

void CPromDoc::OnOpPurgeNoise()
{
  static float noise=1;
  static bool keep_name=true,keep_edit=true,hess_nc=false,resid_nc=true;
  if (!dt_CanClose(true)) return;

  CPurgeNoise_dlg dlg(is_drainage);
  dlg.m_keep_name=keep_name;
  dlg.m_keep_edit=keep_edit;
  dlg.m_noise=noise;
  dlg.m_hess_nc=hess_nc;
  dlg.m_resid_nc=resid_nc;
  if (dlg.DoModal()!=IDOK) return;
  keep_name=dlg.m_keep_name!=FALSE;
  keep_edit=dlg.m_keep_edit!=FALSE;
  noise=dlg.m_noise;
  hess_nc=dlg.m_hess_nc!=FALSE;
  resid_nc=dlg.m_resid_nc!=FALSE;

  static bool xlate_domain=false;
  if (dm_state!=NOT_LOADED) {
    switch (AfxGetMainWnd()->MessageBox(
	    "Would you like to translate the domain map?",
	    "Winprom Peak Purge",MB_ICONINFORMATION|MB_YESNOCANCEL)) {
    case IDYES: xlate_domain=true; break;
    case IDNO: break;
    case IDCANCEL: return;
    }
  }
  BeginWaitCursor();
  double big_noise;
  Elevation big_prom_noise;
  purge_noise(noise,keep_name,keep_edit,hess_nc,resid_nc,xlate_domain,
	      big_noise,big_prom_noise);
  EndWaitCursor();
  CNoiseSum_dlg nsdlg;
  nsdlg.m_big_noise=nint(big_noise);
  nsdlg.m_big_prom_noise=big_prom_noise;
  nsdlg.DoModal();
}

void CPromDoc::OnOpCompressAll()
{
  BeginWaitCursor();
  UpdateViews(&dummySaveCenter);
  tree.compress_all();
  if (dm_state!=NOT_LOADED) resolveDomap();
  dt_state=CHANGED;
  copyPaste.clear();
  dragDrop.clear();
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
  EndWaitCursor();
}

void CPromDoc::OnOpCompressBasinsadls()
{
  UpdateViews(&dummySaveCenter);
  tree.compress_bs();
  dt_state=CHANGED;
  copyPaste.clear();
  dragDrop.clear();
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
}

void CPromDoc::OnOpCompressPeaks()
{
  BeginWaitCursor();
  UpdateViews(&dummySaveCenter);
  tree.compress_dom();
  dt_state=CHANGED;
  copyPaste.clear();
  dragDrop.clear();
  if (dm_state!=NOT_LOADED) resolveDomap();
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
  EndWaitCursor();
}

void CPromDoc::OnOpCompressRunoffs()
{
  UpdateViews(&dummySaveCenter);
  tree.compress_ro();
  dt_state=CHANGED;
  copyPaste.clear();
  dragDrop.clear();
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
}

void CPromDoc::OnOpResortPeaks()
{
  UpdateViews(&dummySaveCenter);
  tree.sort_doms();
  dt_state=CHANGED;
  if (dm_state!=NOT_LOADED) resolveDomap();
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
}

void CPromDoc::OnOpResortNbrs()
{
  tree.sort_nbrs();
  // we don't consider this to modify the DT because it gets rebuilt on reload
}

void CPromDoc::OnOpResortBS()
{
  UpdateViews(&dummySaveCenter);
  tree.sort_bs();
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
}

void CPromDoc::OnOpResortAll()
{
  UpdateViews(&dummySaveCenter);
  tree.sort_all();
  dt_state=CHANGED;
  if (dm_state!=NOT_LOADED) resolveDomap();
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
}

void CPromDoc::OnOpMaintAll()
{
  UpdateViews(&dummySaveCenter);
  tree.compress_all();
  tree.sort_all();
  if (dm_state!=NOT_LOADED) resolveDomap();
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=false;
  UpdateViews(0);
}

void CPromDoc::OnUpdateOpCompressBasinsadls(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dt_state!=NOT_LOADED && tree_writeable &&
		 tree.nbsnsdl_inuse()<tree.n_bsnsdl());
  pCmdUI->SetText(is_drainage?"&Prominence saddles":"&Basin saddles");
}

void CPromDoc::OnUpdateOpCompressPeaks(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dt_state!=NOT_LOADED && tree_writeable &&
		 tree.ndom_inuse()<tree.n_dom());
  pCmdUI->SetText(is_drainage?"&Basins && saddles":"&Peaks && saddles");
}

void CPromDoc::OnUpdateOpCompressRunoffs(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dt_state!=NOT_LOADED && tree_writeable &&
		 tree.nrunoff_inuse()<tree.n_runoff());
}

void CPromDoc::OnUpdateOpCompressAll(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dt_state!=NOT_LOADED && tree_writeable &&
		 (tree.nbsnsdl_inuse()<tree.n_bsnsdl() ||
		  tree.ndom_inuse()<tree.n_dom() ||
		  tree.nrunoff_inuse()<tree.n_runoff()));
}

// widen the discrepancy between map and tree for /featr/.
void CPromDoc::more_disc(Elev_intvl& max_disc, const Feature& featr)
{
  if (elev.contains(featr.location)) {
    Elevation map_elev=elev[featr.location];
    if (map_elev==elev_undef) return;
    Elevation low_disc=abs(featr.elev.low-map_elev),
      high_disc=abs(featr.elev.high-map_elev);
    if (high_disc<low_disc) {
      Elevation temp=high_disc;
      high_disc=low_disc;
      low_disc=temp;
    }
    if (high_disc>max_disc.high) max_disc.high=high_disc;
    if (map_elev<featr.elev.low || map_elev>featr.elev.high)
      if (low_disc>max_disc.low) max_disc.low=low_disc;
  }
}

void CPromDoc::OnOpErrorAnal()
{
  CErrorAnal_dlg dlg(error_stats,*this);

  if (dlg.DoModal()!=IDOK) return;

  tree.set_error(elev,error_stats.sigma_app,
		 error_stats.peak_data,*error_stats.get_sadl_data(),data);
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateViews(&dummyModifyAll);
}

void CPromDoc::OnWindowMap()
{
  CMultiDocTemplate& MapViewTpl=CPromApp::getApp()->MapViewTpl;
  CMDIChildWnd *newmap=(CMDIChildWnd *)MapViewTpl.CreateNewFrame(this,NULL);
  MapViewTpl.InitialUpdateFrame(newmap,this);
}

Elevation CPromDoc::getUseElev(const Elev_intvl& elev, Use_elev use_elev)
{
  switch (use_elev) {
  case UE_LOW: return elev.low;
  case UE_MIDPT: return elev.midpt();
  case UE_HIGH: return elev.high;
  }
  assert(0);
  return elev_undef;
}


void CPromDoc::OnWindowProfile()
{
  CMultiDocTemplate& ProfileViewTpl=CPromApp::getApp()->ProfileViewTpl;
  CMDIChildWnd *newprof=(CMDIChildWnd *)ProfileViewTpl.CreateNewFrame(this,NULL);
  ProfileViewTpl.InitialUpdateFrame(newprof,this);
}

bool CPromDoc::peak_sadl_nbr(Feature *peak, Feature *sadl)
{
  Domain *peak_dom=Domain::FromPeak(peak),*sadl_dom=Domain::FromSadl(sadl);
  return peak_dom==sadl_dom || sadl_dom->primary_nbr==peak_dom;
}

Domain *CPromDoc::peak_beyond_sadl(Feature *peak, Feature *sadl)
{
  Domain *peak_dom=Domain::FromPeak(peak),*sadl_dom=Domain::FromSadl(sadl);
  return peak_dom==sadl_dom ? peak_dom->primary_nbr : sadl_dom;
}

bool CPromDoc::check_overlap(Feature& peak, Feature& nbr, const char *nbr_type,
			     CWnd *parentWin)
{
  if (peak.edited && nbr.edited &&
      (is_drainage?!(peak.elev<nbr.elev):!(peak.elev>nbr.elev))) {
    sprintf(buf,
	    "You have specified a %s elevation\n"
	    "%s than a neighboring %s.\n"
	    "Would you like to see a neighbor list\n"
	    "of overlapping features?",
	    peak_basin_type(),
	    is_drainage?"higher":"lower",
	    nbr_type);
    if (parentWin->MessageBox(buf,"Winprom Feature Elevation Overlap",
		   MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)==IDYES)
      CPeakNbrListView::init_list(this,*Domain::FromPeak(&peak),true);
    return true;
  }
  return false;
}

void CPromDoc::EditFeature(Feature& feature, FT_index ft, CWnd *parentWin)
{
  assert(tree_writeable);
  Domain *dom;
  CFeatureEdit_dlg dlg(parentWin,feature,ft,this);
  tree_writeable=false;
  AfxGetApp()->OnIdle(0);
  switch (dlg.DoModal()) {
  case IDOK:
    zero_prob=prom_zp;
    changed_type=ft;
    dt_state=CHANGED;
    peak_prom_utd=sadl_prom_utd=ridge_utd=false;
    UpdateAllViews(NULL,(LPARAM)&feature);
    switch (ft) {
    case FT_PEAK:
      {
	dom=Domain::FromPeak(&feature);
	for (Domain::Neighbor_iter ni(*dom); ni; ++ni)
	  if (check_overlap(dom->peak,ni.saddle_owner()->saddle,
			    is_drainage?"basin saddle":"saddle",parentWin))
	    goto end_check;
	for (ROiter roi=dom->runoffs.begin(); roi!=dom->runoffs.end(); ++roi)
	  if (check_overlap(dom->peak,**roi,
			    is_drainage?"streambed runoff":"runoff",parentWin))
	    goto end_check;
	for (BSiter bsi=dom->bsnsdls.begin(); bsi!=dom->bsnsdls.end(); ++bsi)
	  if (check_overlap(dom->peak,**bsi,
			    is_drainage?"prom saddle":"basin saddle",parentWin))
	    goto end_check;
      }
      end_check:
      break;
    case FT_SADDLE:
      check_overlap(Domain::FromSadl(&feature)->peak,feature,
		    is_drainage?"basin saddle":"saddle",parentWin);
      check_overlap(Domain::FromSadl(&feature)->primary_nbr->peak,feature,
		    is_drainage?"basin saddle":"saddle",parentWin);
      break;
    case FT_RUNOFF:
      if (((Runoff&)feature).peak!=0) // some runoffs don't have parent peaks
	check_overlap(((Runoff&)feature).peak->peak,feature,
		      is_drainage?"streambed runoff":"runoff",parentWin);
      break;
    case FT_BASINSADL:
      check_overlap(((Basin_saddle&)feature).peak1->peak,feature,
		    is_drainage?"prom saddle":"basin saddle",parentWin);
      check_overlap(((Basin_saddle&)feature).peak2->peak,feature,
		    is_drainage?"prom saddle":"basin saddle",parentWin);
      break;
    default:
      assert(0);
    }
    break;
  case IDC_DELETE:
    tree_writeable=true;
    DeleteFeature(feature,ft,parentWin);
    break;
  }

  tree_writeable=true;
}

void CPromDoc::DeleteFeature(Feature& feature, FT_index ft, CWnd *parentWin)
{
  assert(tree_writeable);
  switch (ft) {
  case FT_BASINSADL:
    DeleteBasinsadl(feature,parentWin);
    break;
  case FT_RUNOFF:
    DeleteRunoff(feature,parentWin);
    break;
  case FT_PEAK:
    DeletePeak(feature,0,parentWin);
    break;
  case FT_SADDLE:
    DeleteSaddle(feature,parentWin);
    break;
  default:
    assert(0);
  }
}

void CPromDoc::DeletePeak(Feature& peak, Feature *saddle, CWnd *parentWin,
			  bool warn_walk)
{
  assert(tree_writeable);
  Domain *peak_dom=Domain::FromPeak(&peak),*sadl_dom;

  if (saddle==0) {
    if (peak_dom->neighbors.empty()) {
      if (!peak_dom->bsnsdls.empty() || !peak_dom->runoffs.empty()) {
	parentWin->MessageBox(
	  "You are attempting to delete a peak which has\n"
	  "runoff neighbors and/or basin saddle neighbors\n"
	  "but no ordinary peak/saddle neighbors.\n"
	  "This is not possible.","Winprom Delete",MB_ICONSTOP|MB_OK);
      }
      else {
	peak_dom->remove();
	changed_type=FT_PEAK;
	dt_state=CHANGED;
	peak_prom_utd=sadl_prom_utd=ridge_utd=false;
	UpdateAllViews(NULL,(LPARAM)&peak_dom->peak);
      }
      return;
    }
    else {
      sadl_dom=peak_dom->high_nbr_saddle(drain_const());
      if (!sadl_dom) {
	sprintf(buf,
		"That %s has an ambiguous %s neighboring saddle.\n"
		"You must choose a neighboring saddle to delete along with the %s.\n"
		"Would you like to see a list of saddle neighbors?",
		peak_basin_type(),
		is_drainage?"lowest":"highest",
		peak_basin_type());
	if (parentWin->MessageBox(buf,
		"Winprom Delete",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	  CPeakNbrListView::init_list(this,*peak_dom);
	return;
      }
      saddle=&peak_dom->saddle_owner(sadl_dom)->saddle;
    }
  }
  else {
    // sadl_dom starts out as the domain that owns the deleted saddle
    sadl_dom=Domain::FromSadl(saddle);
    // sadl_dom then becomes the domain that owns the subsuming peak
    if (peak_dom==sadl_dom) sadl_dom=peak_dom->primary_nbr;
    else {assert(sadl_dom->primary_nbr==peak_dom);}
  }

  if (warn_walk && peak_dom->sadl_walk_inv_risk(sadl_dom,is_drainage)>=0) {
    sprintf(buf,"You are about to delete a %s along with a\n"
		"saddle that may not be the %s neighboring\n"
		"saddle of the peak.  This is ill-advised.\n"
		"Are you sure you really want to do this?",
		peak_basin_type(),is_drainage?"lowest":"highest");

    if (parentWin->MessageBox(buf,"Winprom Delete",
			      MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)
      return;
  }

  if (is_drainage ? sadl_dom->peak.elev.high>=peak_dom->peak.elev.low :
		    sadl_dom->peak.elev.low<=peak_dom->peak.elev.high ) {
    sprintf(buf,"You are about to delete a %s that may be %s\n"
		"than its subsuming neighbor.  This is ill-advised\n"
		"because you are probably diminishing a prominence.\n"
		"Are you sure you really want to do this?",
		peak_basin_type(),is_drainage?"lower":"higher");
    if (parentWin->MessageBox(buf,"Winprom Delete",
			      MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)
      return;
  }

  bool xlate_domain=false;
  if (dm_state!=NOT_LOADED && domain.contains(peak.location) &&
      domain[peak.location]!=Domain::undef) {
    switch (parentWin->MessageBox(
	      "You are about to delete a peak that belongs to a domain.\n"
	      "Would you like to translate that domain into the subsuming peak?",
	      "Winprom Delete",
	      MB_ICONINFORMATION|MB_YESNOCANCEL)) {
    case IDYES: xlate_domain=true; break;
    case IDNO: break;
    case IDCANCEL: return;
    }
  }

  if (xlate_domain) {
    Domain::Index ifrom=domain[peak.location],
      ito=domain[sadl_dom->peak.location];
    for (RawGrid_iter gi(domain); gi; ++gi) {
      Domain::Index& d=domain[*gi];
      if (d==ifrom) d=ito;
    }
  }

  peak_dom->remove(sadl_dom);
  tree.change_ca(peak_dom,sadl_dom);
  changed_type=FT_PEAK;
  UpdateAllViews(NULL,(LPARAM)&peak_dom->peak);
  changed_type=FT_SADDLE;
  UpdateAllViews(NULL,(LPARAM)&peak_dom->saddle);
  if (saddle!=&peak_dom->saddle)
    UpdateAllViews(NULL,(LPARAM)saddle);
  modify_topo();
}

void CPromDoc::DeleteSaddle(Feature& feature, CWnd *parentWin)
{
  assert(tree_writeable);
  Domain *sadl_dom=Domain::FromSadl(&feature);
  Feature &peak_c=sadl_dom->peak,&peak_p=sadl_dom->primary_nbr->peak;
  if (peak_c.elev.high<peak_p.elev.low) {
    DeletePeak(peak_c,&feature,parentWin,true);
  }
  else if (peak_p.elev.high<peak_c.elev.low) {
    DeletePeak(peak_p,&feature,parentWin,true);
  }
  else {
    const char *pbt=peak_basin_type();
    sprintf(buf,"That saddle has an ambiguous lower neighboring %s.\n"
	    "You must choose a %s to delete along with the saddle.",
	    pbt,pbt);
    parentWin->MessageBox(buf,"Winprom Delete",MB_ICONSTOP|MB_OK);
  }
}

void CPromDoc::DeleteRiver(Feature *peak_end, Feature *sadl_end)
{
  BeginWaitCursor();
  Domain::remove_river(Domain::FromPeak(peak_end),Domain::FromSadl(sadl_end));
  tree.set_cycle_info();
  UpdateAllViews(NULL,(LPARAM)&dummyModifyAll);
  modify_topo();
  EndWaitCursor();
}

void CPromDoc::DeleteRunoff(Feature& feature, CWnd *parentWin)
{
  assert(tree_writeable);
  if (parentWin->MessageBox("It is ill-advised to delete any runoff, ever.\n"
			    "Are you sure you really want to do this?",
			    "Winprom Delete",
			    MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)
    return;

  Runoff& ro=(Runoff&)feature;
  ro.remove();
  changed_type=FT_RUNOFF;
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&feature);
}

void CPromDoc::DeleteBasinsadl(Feature& feature, CWnd *parentWin)
{
  assert(tree_writeable);
  Basin_saddle& bs=(Basin_saddle&)feature;
  Basin_saddle::Cycle_walk_info cwinfo;
  short int dc=drain_const();
  bs.get_walk_info(cwinfo,dc);
  if (dc*bs.walk_risk(cwinfo)>=0) {
    sprintf(buf,"That basin saddle may not\n"
		"be the %s in its loop.\n"
		"Delete it anyway?",is_drainage?"highest":"lowest");
    if (parentWin->MessageBox(buf,"Winprom Delete",
			      MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)
      return;
  }

  bs.remove();
  changed_type=FT_BASINSADL;
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&feature);
}

void CPromDoc::NewSaddle(const GridPoint& loc, Domain *peak1, Domain *peak2)
{
  Saddle new_sadl;
  new_sadl.location=loc;
  new_sadl.edited=false;
  if (em_state!=NOT_LOADED && elev.contains(loc) && elev[loc]!=elev_undef)
    new_sadl.elev=elev[loc]; // use elev map
  new_sadl.flat_sep=peak1->peak.location-peak2->peak.location;

  Domain *ca=Domain::common_ancestor(peak1,peak2);
  if (ca) {
    // peaks to be connected are already connected. New saddle is BS.
    Basin_saddle bs=new_sadl;
    bs.peak1=peak1;
    bs.peak2=peak2;
    bs.common_ancestor=ca;
    bs.set_cycle_length();
    if (bs.elev.is_empty()) {
      // use cycle LIS
      bs.elev=elev_infty;
      for (Basin_saddle::Cycle_iter ci(bs); ci; ++ci)
	depress(bs.elev,(*ci)->saddle.elev);
    }
    if (sadl_prom_utd) {
      Saddle::use_status=prom_ss;
      bs.set_prom(0,0,0);
    }

    old_n=tree.n_bsnsdl();
    old_bs=&tree.bsnsdl(0);
    tree.add_bs(bs);
    UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyBSbase);
  }
  else {
    // peaks are not already connected. New saddle is PS.
    Basin_saddle bs;
    if (new_sadl.elev.is_empty())
      new_sadl.elev.set(-1280,
	peak1->peak.elev.high<peak2->peak.elev.high?
	peak1->peak.elev.high:peak2->peak.elev.high);
    Domain::connect(new_sadl,peak1,peak2,bs,false,false);
    assert(!bs.location);
    Domain::connect_nbr(bs,peak1,peak2);
    tree.set_cycle_info();
    peak_prom_utd=sadl_prom_utd=ridge_utd=false;
    UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyTopo);
  }
  dt_state=CHANGED;
}

void CPromDoc::NewRunoff(const GridPoint& loc, Domain *peak)
{
  Runoff ro;
  ro.location=loc;
  ro.edited=false;
  // set the new runoff elevation
  if (em_state!=NOT_LOADED && elev.contains(loc) && elev[loc]!=elev_undef)
    ro.elev=elev[loc]; // use elev map
  else ro.elev.set(-1280,peak->peak.elev.high);
  ro.peak=peak;
  Rectangl tree_rect;
  // Determine the extent of runoffs already in the tree
  Domain::Index i,n_runoff=tree.n_runoff();
  for (i=0; i<n_runoff; ++i)
    if (tree.runoff(i).location)
      tree_rect|=tree.runoff(i).location;
  // Find the # of interior quadrants
  bool tbx=loc.x==tree_rect.limit[0].low || loc.x==tree_rect.limit[0].high,
    tby=loc.y==tree_rect.limit[1].low || loc.y==tree_rect.limit[1].high;
  if (tbx && tby) ro.interior_quads=1; // tree corner
  else if (tbx || tby) ro.interior_quads=2; // tree edge
  else {
    // tree interior
    if (ro.location.quad_corner()) ro.interior_quads=3;
    else if (ro.location.quad_edge()) ro.interior_quads=2;
    else ro.interior_quads=0;
  }
  ro.flat_sep=loc-peak->peak.location;
  old_n=tree.n_runoff();
  old_ro=&tree.runoff(0);
  new_iro=tree.index(*tree.add_ro(ro));
  if (peak_prom_utd) tree.update_runoff_prom_info();
  UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyRObase);
  dt_state=CHANGED;
}

void CPromDoc::EditRecord(Database::RecordID rid, CWnd *parentWin)
{
  assert(tree_writeable);
  bool change=false;
  tree_writeable=false;
  AfxGetApp()->OnIdle(0);
  if (data.begin_field()==data.end_field())
    EditFields(parentWin);
  int status;
  while (true) {
    CData_dlg dlg(*this,rid,parentWin);
    status=dlg.DoModal();
    if (dlg.changed) change=true;
    if (status!=IDC_FIELDS) break;
    EditFields(parentWin);
  }
  if (status==IDC_DELETE) {
    DeleteRecord(rid);
  }
  else if (change) {
    db_state=CHANGED;
    record_change=rid;
    UpdateAllViews(0,(LPARAM)&dummyModifyData);
  }
  tree_writeable=true;
}

void CPromDoc::EditRecord(const GridPoint& gp, CWnd *parentWin)
{
  assert(tree_writeable);
  bool change=false;
  tree_writeable=false;
  if (data.begin_field()==data.end_field())
    EditFields(parentWin);
  int status;
  while (true) {
    CData_dlg dlg(*this,gp,parentWin);
    status=dlg.DoModal();
    if (dlg.changed) change=true;
    if (status!=IDC_FIELDS) break;
    EditFields(parentWin);
  }
  if (status==IDC_DELETE) {
    DeleteRecord(data.get_record(gp));
  }
  else if (change) {
    db_state=CHANGED;
    record_change=data.get_record(gp);
    UpdateAllViews(0,(LPARAM)&dummyModifyData);
  }
  tree_writeable=true;
}

void CPromDoc::DeleteRecord(Database::RecordID rid)
{
  data.remove_record(rid);
  copyPaste.orph_list.clear();
  dragDrop.orph_list.clear();
  record_change=REC_NONE;
  UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyData);
  db_state=CHANGED;
}

void CPromDoc::EditFields(CWnd *parentWin)
{
  if (CFields_dlg(data,parentWin).DoModal()!=IDOK) return;
  record_change=REC_NONE;
  UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyData);
  db_state=CHANGED;
}

void CPromDoc::Rotate(Basin_saddle& bs, Feature *saddle, CWnd *parentWin)
{
  assert(tree_writeable);
  Saddle::use_status=prom_ss;
  Domain *sadl_dom = saddle ? Domain::FromSadl(saddle) :
    bs.cycle_min(drain_const(),outer_endpt());

  if (sadl_dom) {
    bs.rotate(sadl_dom);
    tree.set_cycle_info();
    UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyAll);
    modify_topo();
  }
  else {
    if (is_drainage)
      parentWin->MessageBox("That prominence saddle is already\n"
			    "the maximum of its loop.",
			    "Winprom Rotate",MB_ICONINFORMATION|MB_OK);
    else
      parentWin->MessageBox("That basin saddle is already\n"
			    "the minimum of its loop.",
			    "Winprom Rotate",MB_ICONINFORMATION|MB_OK);
  }

}

void CPromDoc::Splice(Runoff& ro, CWnd *parentWin)
{
  assert(tree_writeable);
  old_n=tree.n_bsnsdl();
  old_bs=old_n>0 ? &tree.bsnsdl(0) : 0;
  Runoff *rop=&ro;
  if (tree.splice(rop)) {
    tree.set_cycle_info();
    dt_state=CHANGED;
    peak_prom_utd=sadl_prom_utd=ridge_utd=false;
    UpdateAllViews(0,(LPARAM)&dummyModifySplice);
  }
  else {
    parentWin->MessageBox("The location of that runoff\n"
			  "does not coincide with any other.",
			  "Winprom Splice",MB_ICONERROR|MB_OK);
  }
}

void CPromDoc::Splice(Runoff *ros[], unsigned nro, CWnd *parentWin)
{
  assert(tree_writeable);
  old_n=tree.n_bsnsdl();
  old_bs=old_n>0 ? &tree.bsnsdl(0) : 0;
  tree.splice(ros,nro);
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&dummyModifySplice);
}

static void ambig_SWI(CWnd *win)
{
  win->MessageBox("Cannot perforn SWI switch because\n"
		  "the two saddle elevations overlap.\n"
		  "It is not clear which is higher.",
		  "Winprom SWI Switch",MB_ICONERROR|MB_OK);
}

static void ambig_BS(CWnd *win)
{
  win->MessageBox("Cannot determine which side of the basin saddle\n"
		  "to connect to because it is ambiguous\n"
		  "as to which side of the cycle is higher.",
		  "Winprom SWI Switch",MB_ICONERROR|MB_OK);
}

static bool check_cxn(Domain::Sadl_cxn sc, CWnd *parentWin)
{
  switch (sc) {
  case Domain::NO_CXN:
    parentWin->MessageBox("Those two saddles aren\'t connected.",
			  "Winprom SWI Switch",MB_ICONERROR|MB_OK);
    return false;
  case Domain::AMBIG_CXN:
    ambig_BS(parentWin);
    return false;
  case Domain::LOW_CXN:
    if (parentWin->MessageBox("Those two saddles are connected via a\n"
			      "path that goes lower than both saddles.\n"
			      "SWI is meaningless under that circumstance.\n"
			      "Are you sure you want to do it anyway?",
			      "Winprom SWI Switch",
			      MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2)!=IDYES)
      return false;
    break;
  case Domain::GOOD_CXN:
    break;
  default: assert(0);
  }
  return true;
}

// Perform SWI switch on two saddles
void CPromDoc::SWI_switch_sym(Feature& sadl1, Feature& sadl2, CWnd *parentWin)
{
  short int dc=drain_const();
  Elev_endpt inner=inner_endpt(),outer=outer_endpt();
  if (dc*(sadl1.elev.*inner-sadl2.elev.*outer)>0)
    SWI_switch(sadl1,sadl2,parentWin);
  else if (dc*(sadl2.elev.*inner-sadl1.elev.*outer)>0)
    SWI_switch(sadl2,sadl1,parentWin);
  else ambig_SWI(parentWin);
}

// Perform SWI switch on one saddle and one basin saddle
void CPromDoc::SWI_switch_sym(Feature& sadl, Basin_saddle& bs, CWnd *parentWin)
{
  short int dc=drain_const();
  Elev_endpt inner=inner_endpt(),outer=outer_endpt();

  if (dc*(sadl.elev.*inner-bs.elev.*outer)>0) {
    // prom saddle is higher than basin saddle
    SWI_switch(sadl,bs,parentWin);
  }
  else if (dc*(bs.elev.*inner-sadl.elev.*outer)>0) {
    // basin saddle is higher than prom saddle
    SWI_switch(bs,sadl,parentWin);
  }
  else ambig_SWI(parentWin);
}

// Perform SWI switch on high BS and low PS.
void CPromDoc::SWI_switch(Basin_saddle& bs, Feature& sadl, CWnd *parentWin)
{
  Domain *sadl_dom=Domain::FromSadl(&sadl),*ps_cxn,*bs_cxn;
  unsigned short cycle_side;
  if (!check_cxn(bs.SWI_switch_cxn(sadl_dom,ps_cxn,bs_cxn,cycle_side,
				   drain_const(),inner_endpt(),outer_endpt()),
		 parentWin))
    return;

  bs.reconnect(bs.other_peak(bs_cxn),ps_cxn);

  tree.set_cycle_info();
  modify_topo();
}

// Perform SWI switch on high PS and low BS.
void CPromDoc::SWI_switch(Feature& sadl, Basin_saddle& bs, CWnd *parentWin)
{
  Domain *sadl_dom=Domain::FromSadl(&sadl),*ps_cxn,*bs_cxn;
  unsigned short cycle_side;
  if (!check_cxn(bs.SWI_switch_cxn(sadl_dom,ps_cxn,bs_cxn,cycle_side,
				   drain_const(),inner_endpt(),outer_endpt()),
		 parentWin))
    return;

  if (cycle_side) {
    // prom saddle is on basin saddle's loop!
    // We therefore rotate the saddles before performing the switch
    bs.rotate(sadl_dom);
    bs.reconnect(bs.other_peak(ps_cxn),bs_cxn);
  }
  else {
    bs_cxn->reconnect(*sadl_dom,Domain::common_ancestor(bs_cxn,sadl_dom));
  }

  tree.set_cycle_info();
  modify_topo();
}

// Perform SWI switch on two saddles, knowing already which is higher.
void CPromDoc::SWI_switch(Feature& hisadl, Feature& losadl, CWnd *parentWin)
{
  Domain *ca,*hidom=Domain::FromSadl(&hisadl),*lodom=Domain::FromSadl(&losadl);
  if (!check_cxn(Domain::SWI_switch_cxn(hidom,lodom,ca,drain_const(),
					inner_endpt(),outer_endpt()),
		 parentWin))
    return;

  hidom->SWI_switch(*lodom,ca);

  tree.set_cycle_info();
  modify_topo();
}

// Perform SWI switch on two basin saddles.
void CPromDoc::SWI_switch_sym(Basin_saddle& bs1, Basin_saddle& bs2, CWnd *parentWin)
{
  short int dc=drain_const();
  Elev_endpt inner=inner_endpt(),outer=outer_endpt();
  if (dc*(bs1.elev.*inner-bs2.elev.*outer)>0)
    SWI_switch(bs1,bs2,parentWin);
  else if (dc*(bs2.elev.*inner-bs1.elev.*outer)>0)
    SWI_switch(bs2,bs1,parentWin);
  else ambig_SWI(parentWin);
}

// Perform SWI switch on two basin saddles, knowing already which is higher.
void CPromDoc::SWI_switch(Basin_saddle& hibs, Basin_saddle& lobs, CWnd *parentWin)
{
  Domain *hi_cxn,*lo_cxn;
  if (!check_cxn(Basin_saddle::SWI_switch_cxn(hibs,lobs,hi_cxn,lo_cxn,
					      drain_const(),inner_endpt(),outer_endpt()),
		 parentWin))
    return;

  hibs.reconnect(hibs.other_peak(hi_cxn),lo_cxn);

  tree.set_cycle_info();
  modify_topo();
}

// Perform SWI switch on runoff and saddle.
void CPromDoc::SWI_switch(Feature& sadl, Runoff& ro)
{
  ro.SWI_switch(Domain::FromSadl(&sadl));

  tree.set_cycle_info();
  modify_topo();
}

// Perform SWI switch on runoff and basin saddle.
void CPromDoc::SWI_switch(Basin_saddle& bs, Runoff& ro, CWnd *parentWin)
{
  switch (bs.high_side(bs.cycle_jct(ro.peak),drain_const(),
		       inner_endpt(),outer_endpt())) {
  case 1:
    ro.reconnect(bs.peak2);
    break;
  case 2:
    ro.reconnect(bs.peak1);
    break;
  case 0:
    ambig_BS(parentWin);
    return;
  }

  tree.set_cycle_info();
  modify_topo();
}

// Connect a peak to a saddle. Used mainly for SWI switch.
void CPromDoc::reconnect(Feature& peak, Feature& sadl, CWnd *parentWin)
{
  Domain *ca,*pkdom=Domain::FromPeak(&peak),*sadldom=Domain::FromSadl(&sadl);
  if (sadldom==pkdom || sadldom->primary_nbr==pkdom) {
    parentWin->MessageBox("That peak and saddle are already directly connected.",
			  "Winprom SWI Switch",MB_ICONERROR|MB_OK);
    return;
  }
  ca=Domain::common_ancestor(pkdom,sadldom);
  if (ca==0) {
    parentWin->MessageBox("That peak and saddle are on disjoint trees.",
			  "Winprom SWI Switch",MB_ICONERROR|MB_OK);
    return;
  }

  pkdom->reconnect(*sadldom,ca);

  tree.set_cycle_info();
  modify_topo();
}

// Connect a peak to a basin saddle. Used mainly for SWI switch.
void CPromDoc::reconnect(Feature& peak, Basin_saddle& bs, CWnd *parentWin)
{
  Domain *pkdom=Domain::FromPeak(&peak);
  switch (bs.high_side(bs.cycle_jct(pkdom),drain_const(),
		       inner_endpt(),outer_endpt())) {
  case 1:
    bs.reconnect(bs.peak1,pkdom);
    break;
  case 2:
    bs.reconnect(bs.peak2,pkdom);
    break;
  case 0:
    ambig_BS(parentWin);
    return;
  }

  tree.set_cycle_info();
  modify_topo();
}

// Connect a peak to a runoff. Used mainly for SWI switch.
void CPromDoc::reconnect(Feature& peak, Runoff& ro, CWnd *parentWin)
{
  Domain *pkdom=Domain::FromPeak(&peak);
  if (pkdom==ro.peak) {
    parentWin->MessageBox("The selected peak and runoff are already connected.",
			  "Winprom SWI Switch",MB_ICONERROR|MB_OK);
    return;
  }
  ro.reconnect(pkdom);

  tree.set_cycle_info();
  modify_topo();
}


void CPromDoc::OnOpLPRmap()
{
  if (!dm_CanClose()) return;
  update_sadl_prom_info();
  BeginWaitCursor();
  LPRmap();
  EndWaitCursor();
}

void CPromDoc::LPRmap()
{
  elev_flip_drain();
  tree_flip_drain();
  tree.lpr_map(elev,domain,n_domain);
  elev_flip_drain();
  tree_flip_drain();
  UpdateViews(&dummyModifyMap);
  dm_state=CHANGED;
}

void CPromDoc::OnUpdateHaveAll(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(em_state!=NOT_LOADED && dm_state!=NOT_LOADED &&
		 dt_state!=NOT_LOADED);
}

void CPromDoc::OnOpDLPRfill()
{
  BeginWaitCursor();
  fillDomap();
  EndWaitCursor();
}

void CPromDoc::fillDomap()
{
  elev_flip_drain();
  fill_domap(elev,domain);
  elev_flip_drain();
  UpdateViews(&dummyModifyMap);
  dm_state=CHANGED;
}

void CPromDoc::OnOpDLPRconsol()
{
  BeginWaitCursor();
  elev_flip_drain();
  Divide_tree_map::consolidate_domap(elev,dt_state==NOT_LOADED?0:&tree,domain,n_domain);
  elev_flip_drain();
  UpdateViews(&dummyModifyMap);
  dm_state=CHANGED;
  EndWaitCursor();
}

void CPromDoc::OnUpdateHaveMaps(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(em_state!=NOT_LOADED && dm_state!=NOT_LOADED);
}


static char cmd_buf[64+FILENAME_MAX],param_buf[FILENAME_MAX];

static void get_param(FILE *file)
{
  fscanf(file,"%s",param_buf);
  strcat(cmd_buf," ");
  strcat(cmd_buf,param_buf);
}

bool CPromDoc::em_ScriptCanClose(bool drastic) const
{
  if (em_state!=CHANGED) em_script_clean=true; // in case the script cleaned the em
  if (em_script_clean) return true;
  if (!em_CanClose(drastic)) return false;
  em_script_clean=true;
  return true;
}

bool CPromDoc::dm_ScriptCanClose(bool drastic) const
{
  if (dm_state!=CHANGED) dm_script_clean=true; // in case the script cleaned the dm
  if (dm_script_clean) return true;
  if (!dm_CanClose(drastic)) return false;
  dm_script_clean=true;
  return true;
}

bool CPromDoc::dt_ScriptCanClose(bool drastic) const
{
  if (dt_state!=CHANGED) dt_script_clean=true; // in case the script cleaned the dt
  if (dt_script_clean) return true;
  if (!dt_CanClose(drastic)) return false;
  dt_script_clean=true;
  return true;
}

void CPromDoc::OnOpRun()
{
  CString name=GetFileName(FO_OPEN,NULL,"Script","wps");
  if (name.IsEmpty()) return;
  dt_script_clean=dt_state!=CHANGED;
  BeginWork();
  CScriptCtl_dlg dlg(*this,name);
  dlg.DoModal();
  EndWork();
}

static void throw_no_tree(bool is_drainage) throw(script_error)
{
  throw script_error(is_drainage?"No drainage tree":"No divide tree");
}

static void throw_drastic(bool is_drainage) throw(script_error)
{
  throw script_error(is_drainage?
		     "Cannot make drastic change on drainage tree":
		     "Cannot make drastic change on divide tree");
}

void CPromDoc::execute_command(FILE *file, CScriptCtl_dlg& control)
  throw(script_error)
{
  if (!strcmp(cmd_buf,"change_directory")) {
    fgets(param_buf,sizeof param_buf,file);
    strcat(cmd_buf,param_buf);
    // trim leading and trailing white space from the directory parameter
    char *pb;
    for (pb=param_buf; isspace(*pb); ++pb);
    for (char *pb_end=pb+strlen(pb)-1; isspace(*pb_end); --pb_end)
      *pb_end='\0';
    if (_chdir(pb)<0) throw script_error(strerror(errno));
  }
  else if (!strcmp(cmd_buf,"open_elev_map")) {
    get_param(file);
    if (!em_ScriptCanClose()) throw script_error("Cannot discard elevation map");
    if (!readElevMap(param_buf))
      throw script_error("Error reading elevation map");
  }
  else if (!strcmp(cmd_buf,"open_domain_map")) {
    get_param(file);
    if (!dm_ScriptCanClose()) throw script_error("Cannot discard domain map");
    if (!readDomap(param_buf))
      throw script_error("Error reading domain map");
  }
  else if (!strcmp(cmd_buf,"open_divide_tree") ||
	   !strcmp(cmd_buf,"open_drainage_tree")) {
    get_param(file);
    if (!dt_ScriptCanClose())
      throw script_error(is_drainage?"Cannot discard drainage tree":"Cannot discard divide tree");
    if (!readDivideTree(param_buf))
      throw script_error("Error reading divide tree");
  }
  else if (!strcmp(cmd_buf,"merge_elev_map")) {
    get_param(file);
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    if (!em_ScriptCanClose(true))
      throw script_error("Cannot make drastic change on elevation map");
    if (!mergeElevMap(param_buf))
      throw script_error("Error merging elevation map");
  }
  else if (!strcmp(cmd_buf,"merge_domain_map")) {
    get_param(file);
    if (dm_state==NOT_LOADED) throw script_error("No domain map");
    if (!dm_ScriptCanClose(true))
      throw script_error("Cannot make drastic change on domain map");
    if (!mergeDomap(param_buf))
      throw script_error("Error merging domain map");
  }
  else if (!strcmp(cmd_buf,"merge_divide_tree")) {
    get_param(file);
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!dt_ScriptCanClose(true)) throw_drastic(is_drainage);
    if (!mergeDivideTree(param_buf))
      throw script_error(is_drainage?
	"Error merging drainage tree":"Error merging divide tree");
  }
  else if (!strcmp(cmd_buf,"saveas_elev_map")) {
    get_param(file);
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    if (!saveasElevMap(param_buf))
      throw script_error("Error saving elevation map");
  }
  else if (!strcmp(cmd_buf,"saveas_domain_map")) {
    get_param(file);
    if (dm_state==NOT_LOADED) throw script_error("No domain map");
    if (!saveasDomap(param_buf))
      throw script_error("Error saving domain map");
  }
  else if (!strcmp(cmd_buf,"saveas_divide_tree")) {
    get_param(file);
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!saveasDivideTree(param_buf))
      throw script_error(is_drainage?
	"Error saving drainage tree":"Error saving divide tree");
  }
  else if (!strcmp(cmd_buf,"copy_elev_map")) {
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    OnEditCopyElevMap();
  }
  else if (!strcmp(cmd_buf,"copy_domain_map")) {
    if (dm_state==NOT_LOADED) throw script_error("No domain map");
    OnEditCopyDLPRmap();
  }
  else if (!strcmp(cmd_buf,"copy_divide_tree")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnEditCopyDLPRtree();
  }
  else if (!strcmp(cmd_buf,"paste_elev_map")) {
    CPromDoc *source=CPromApp::getApp()->elev_map_clip;
    if (source==0 || source==this)
      throw script_error("No elevation map on clipboard");
    if (!em_ScriptCanClose()) throw script_error("Cannot overwrite elevation map");
    OnEditPasteElevMap();
  }
  else if (!strcmp(cmd_buf,"paste_domain_map")) {
    CPromDoc *source=CPromApp::getApp()->domap_clip;
    if (source==0 || source==this)
      throw script_error("No domain map on clipboard");
    if (!dm_ScriptCanClose()) throw script_error("Cannot overwrite domain map");
    OnEditPasteDLPRmap();
  }
  else if (!strcmp(cmd_buf,"paste_divide_tree") ||
	   !strcmp(cmd_buf,"paste_drainage_tree")) {
    CPromDoc *source=CPromApp::getApp()->divide_tree_clip;
    if (source==0 || source==this)
      throw script_error("No divide/drainage tree on clipboard");
    if (!dt_ScriptCanClose()) throw script_error(is_drainage?
      "Cannot overwrite drainage tree":"Cannot overwrite divide tree");
    OnEditPasteDLPRtree();
  }
  else if (!strcmp(cmd_buf,"analyze_peaks")) {
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    if (!dm_ScriptCanClose()) throw script_error("Cannot discard domain map");
    if (!dt_ScriptCanClose()) throw script_error(is_drainage?
      "Cannot discard drainage tree":"Cannot discard divide tree");
#ifdef administrator
    is_drainage=false;
    tree.walkup_analyze(elev,domain);
    post_analyze();
    post_post_analyze();
#else
    throw script_error("The analyzer is not implemented in this shipment of Winprom.\n"
		       "Contact the Custodian for the divide tree(s) of interest.");
#endif
  }
  else if (!strcmp(cmd_buf,"analyze_basins")) {
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    if (!dm_ScriptCanClose()) throw script_error("Cannot discard domain map");
    if (!dt_ScriptCanClose()) throw script_error(is_drainage?
      "Cannot discard drainage tree":"Cannot discard divide tree");
#ifdef administrator
    is_drainage=true;
    elev_flip();
    tree.walkup_analyze(elev,domain);
    post_analyze();
    elev_flip();
    tree.transform(-1,0,true,true,true,true,true);
    post_post_analyze();
#else
    throw script_error("The analyzer is not implemented in this shipment of Winprom.\n"
		       "Contact the Custodian for the drainage tree(s) of interest.");
#endif
  }
  else if (!strcmp(cmd_buf,"splice")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpSplice();
  }
#if 0
  else if (!strcmp(cmd_buf,"error_allowance")) {
    Elevation error_allow;
    fscanf(file,"%d",&error_allow);
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (error_allow<0) throw script_error("Error allowance must be >=0");
    Regress dummy_data;
    dummy_data.sigma2_res=-1; // induce an error if this is used
    RQF_methods rqfm(false);
    FeatureFilter filter(false);
    set_error_allownc(tree,elev,2,false,false,error_allow,
		      dummy_data,dummy_data,rqfm,false,filter,data);
  }
#endif
  else if (!strcmp(cmd_buf,"rotate")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    rotate();
  }
  else if (!strcmp(cmd_buf,"run")) {
    get_param(file);
    if (!runScript(param_buf,control))
      throw script_error("Error running subsidiary script");
  }
  else if (!strcmp(cmd_buf,"purge_peaks") || !strcmp(cmd_buf,"purge_peaks_xlate") ||
	   !strcmp(cmd_buf,"purge_basins") || !strcmp(cmd_buf,"purge_basins_xlate")) {
    Elevation min_prom;
    fscanf(file,"%hd",&min_prom);
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!dt_ScriptCanClose(true)) throw_drastic(is_drainage);
    if (min_prom<=0) throw script_error(is_drainage?
      "Prominence cutoff for purging peaks must be >0" :
      "Prominence cutoff for purging basins must be >0");
    bool xlate=strchr(cmd_buf,'x')!=0;
    if (xlate && dm_state==NOT_LOADED) throw script_error("No domain map");
    purge_peaks(min_prom,true,true,false,false,xlate);
  }
  else if (!strcmp(cmd_buf,"purge_noise") || !strcmp(cmd_buf,"purge_noise_xlate")) {
    float noise;
    fscanf(file,"%f",&noise);
    if (noise<=0) throw script_error("Noise factor must be >0");
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    bool xlate=strchr(cmd_buf,'x')!=0;
    if (xlate && dm_state==NOT_LOADED) throw script_error("No domain map");
    Elevation edum;
    double ddum;
    purge_noise(noise,true,true,false,true,xlate,ddum,edum);
  }
  else if (!strcmp(cmd_buf,"purge_bs_walk_safe")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!dt_ScriptCanClose(true)) throw_drastic(is_drainage);
    purge_basin_saddles(0);
  }
  else if (!strcmp(cmd_buf,"purge_bs_degenerate")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!dt_ScriptCanClose(true)) throw_drastic(is_drainage);
    purge_basin_saddles(-elev_infty+1);
  }
  else if (!strcmp(cmd_buf,"purge_bs_redundant")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!dt_ScriptCanClose(true)) throw_drastic(is_drainage);
    purge_bs_redun();
  }
  else if (!strcmp(cmd_buf,"purge_bs_prominence")) {
    Elevation min_prom;
    fscanf(file,"%hd",&min_prom);
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!dt_ScriptCanClose(true)) throw_drastic(is_drainage);
    if (min_prom<=0)
      throw script_error("Prominence cutoff for purging basin saddles must be >0");
    purge_bs_prom(min_prom,true,true,false,false);
  }
  else if (!strcmp(cmd_buf,"purge_bs_compliment")) {
    CPromApp *app=CPromApp::getApp();
    if (dm_state==NOT_LOADED || dt_state==NOT_LOADED || em_state==NOT_LOADED ||
	app->divide_tree_clip==0 || app->divide_tree_clip==this)
      throw script_error("To purge saddles by divide/drainage compliment,\n"
			 "you must have an elevation map, domain map, tree,\n"
			 "and a tree on the clipboard from another workspace.");
    purge_bs_drain();
  }
  else if (!strcmp(cmd_buf,"undefine_flat")) {
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    Elev_intvl unlimit;
    unlimit.fill();
    undefine_flat(elev,(Rectangl)elev,unlimit);
    UpdateViews(&dummyModifyMap);
  }
  else if (!strcmp(cmd_buf,"define_flat")) {
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    Elev_intvl unlimit;
    unlimit.fill();
    define_flat(elev,(Rectangl)elev,unlimit);
    UpdateViews(&dummyModifyMap);
  }
  else if (!strcmp(cmd_buf,"elev_transform")) {
    float m,b;
    fscanf(file,"%f%f",&m,&b);
    if (em_state==NOT_LOADED) throw script_error("No elevation map");
    if (!elev_transform(m,b)) throw script_error("Error transforming elevation map");
  }
  else if (!strcmp(cmd_buf,"tree_transform")) {
    float m,b;
    fscanf(file,"%f%f",&m,&b);
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    if (!tree_transform(m,b,true,true,true,true,true))
      throw script_error(is_drainage?
	"Error transforming drainage tree":"Error transforming divide tree");
  }
  else if (!strcmp(cmd_buf,"lpr_map")) {
    if (dt_state==NOT_LOADED || em_state==NOT_LOADED || dt_state==NOT_LOADED)
      throw script_error("Must have divide/drainage tree, elevation map, and domain map");
    LPRmap();
  }
  else if (!strcmp(cmd_buf,"fill_domain_map")) {
    if (em_state==NOT_LOADED || dt_state==NOT_LOADED)
      throw script_error("Must have elevation map and domain map");
    fillDomap();
  }
  else if (!strcmp(cmd_buf,"compress_peak_saddle")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpCompressPeaks();
  }
  else if (!strcmp(cmd_buf,"compress_runoff")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpCompressRunoffs();
  }
  else if (!strcmp(cmd_buf,"compress_basin_saddle")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpCompressBasinsadls();
  }
  else if (!strcmp(cmd_buf,"compress_all")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpCompressAll();
  }
  else if (!strcmp(cmd_buf,"resort_peaks")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpResortPeaks();
  }
  else if (!strcmp(cmd_buf,"resort_neighbors")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpResortNbrs();
  }
  else if (!strcmp(cmd_buf,"resort_basin_saddle")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpResortBS();
  }
  else if (!strcmp(cmd_buf,"resort_all")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpResortAll();
  }
  else if (!strcmp(cmd_buf,"maint_all")) {
    if (dt_state==NOT_LOADED) throw_no_tree(is_drainage);
    OnOpMaintAll();
  }
  else if (!strcmp(cmd_buf,"clear_modify_elev_map")) {
    if (em_state==CHANGED) em_state=NOT_CHANGED;
  }
  else if (!strcmp(cmd_buf,"clear_modify_domain_map")) {
    if (dm_state==CHANGED) dm_state=NOT_CHANGED;
  }
  else if (!strcmp(cmd_buf,"clear_modify_divide_tree")) {
    if (dt_state==CHANGED) dt_state=NOT_CHANGED;
  }
  else {
    throw script_error("Unrecognized mnemonic");
  }

}

bool CPromDoc::runScript(const char *name, CScriptCtl_dlg& control)
{
  FILE *file=fopen(name,"r");
  if (file==0) {
    FileError("opening winprom script",name);
    return false;
  }

  char *b,c;
  bool error_prompt=true;
  while (fscanf(file,"%s",cmd_buf), !ferror(file) && !feof(file)) {
    AfxGetApp()->PostThreadMessage(IDT_SCRIPT_CMD,0,(WPARAM)cmd_buf);
    for (b=cmd_buf; *b; ++b)
      *b=tolower(*b);
    if (control.stop_request) break; // check for user abort
    if (!strcmp(cmd_buf,"//") || !strcmp(cmd_buf,"remark")) {
      while (c=getc(file), c!='\n' && c!=EOF); // read to end of line
    }
    else if (!strcmp(cmd_buf,"stop")) {
      break;
    }
    else if (!strcmp(cmd_buf,"error_continue")) {
      error_prompt=false;
    }
    else if (!strcmp(cmd_buf,"error_prompt")) {
      error_prompt=true;
    }
    else {
      try {
	execute_command(file,control);
      }
      catch (script_error e) {
	if (error_prompt) {
	  sprintf(buf,"%s\nat command \"%s\"\nin file \"%s\".\n"
		      "Continue processing?",e.what(),cmd_buf,name);
	  if (control.MessageBox(buf,"Winprom Script Error",
				 MB_YESNO|MB_DEFBUTTON2|MB_ICONEXCLAMATION)!=IDYES)
	    fclose(file);
	    return false;
	}
      }
    }

  }

  fclose(file);
  return true;
}


void CPromDoc::OnEditCopyDLPRmap()
{
  CPromApp::getApp()->domap_clip=this;
}

void CPromDoc::OnEditCopyDLPRtree()
{
  CPromApp::getApp()->divide_tree_clip=this;
}

void CPromDoc::OnEditCopyElevMap()
{
  CPromApp::getApp()->elev_map_clip=this;
}

void CPromDoc::OnEditCopyDatabase()
{
  CPromApp::getApp()->database_clip=this;
}

void CPromDoc::OnEditPasteDLPRmap()
{
  if (!dm_CanClose()) return;
  BeginWaitCursor();
  const Index_map& dm_source=CPromApp::getApp()->domap_clip->domain;
  domain.set_bound(dm_source);
  domain.transfer(dm_source);
  n_domain=CPromApp::getApp()->domap_clip->n_domain;
  if (dt_state!=NOT_LOADED) resolveDomap();
  dm_state=CHANGED;
  dm_save_drastic_change=false;
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  SetViewTitles();
  EndWaitCursor();
}

void CPromDoc::OnUpdateEditPasteDLPRmap(CCmdUI *pCmdUI)
{
  CPromDoc *source=CPromApp::getApp()->domap_clip;
  pCmdUI->Enable(source!=0 && source!=this);
}

void CPromDoc::OnEditPasteDLPRtree()
{
  if (!dt_CanClose()) return;
  BeginWaitCursor();
  const CPromDoc *doc_source=CPromApp::getApp()->divide_tree_clip;
  tree.copy(doc_source->tree);
  is_drainage=doc_source->is_drainage;
  if (dm_state!=NOT_LOADED) resolveDomap();
  dt_state=CHANGED;
  peak_prom_utd=doc_source->peak_prom_utd;
  sadl_prom_utd=doc_source->sadl_prom_utd;
  ridge_utd=doc_source->ridge_utd;
  dt_save_drastic_change=false;
  CalcTotalArea();
  UpdateViews(0);
  SetViewTitles();
  EndWaitCursor();
}

void CPromDoc::OnUpdateEditPasteDLPRtree(CCmdUI *pCmdUI)
{
  CPromDoc *source=CPromApp::getApp()->divide_tree_clip;
  pCmdUI->Enable(tree_writeable && source!=0 && source!=this);
  pCmdUI->SetText(source && source->is_drainage?"Drainage &tree":"Divide &tree");
}

void CPromDoc::OnEditPasteElevMap()
{
  if (!em_CanClose()) return;
  BeginWaitCursor();
  const Elev_map& em_source=CPromApp::getApp()->elev_map_clip->elev;
  elev.set_bound(em_source);
  elev.transfer(em_source);
  em_state=CHANGED;
  em_save_drastic_change=false;
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  SetViewTitles();
  EndWaitCursor();
}

void CPromDoc::OnUpdateEditPasteElevMap(CCmdUI *pCmdUI)
{
  CPromDoc *source=CPromApp::getApp()->elev_map_clip;
  pCmdUI->Enable(source!=0 && source!=this);
}

void CPromDoc::OnEditPasteDatabase()
{
  if (!db_CanClose()) return;
  data=CPromApp::getApp()->database_clip->data;
  db_state=CHANGED;
  db_save_drastic_change=false;
  record_change=REC_NONE;
  UpdateAllViews(0,(LPARAM)&dummyModifyData);
  SetViewTitles();
}

void CPromDoc::OnUpdateEditPasteDatabase(CCmdUI *pCmdUI)
{
  CPromDoc *source=CPromApp::getApp()->database_clip;
  pCmdUI->Enable(source!=0 && source!=this);
}

void CPromDoc::OnEditCopyAll()
{
  CPromApp *app=CPromApp::getApp();
  app->elev_map_clip=em_state==NOT_LOADED?0:this;
  app->domap_clip=dm_state==NOT_LOADED?0:this;
  app->divide_tree_clip=dt_state==NOT_LOADED?0:this;
  app->database_clip=db_state==NOT_LOADED?0:this;
}

void CPromDoc::OnEditPasteAll()
{
  CPromApp *app=CPromApp::getApp();
  if (app->elev_map_clip && app->elev_map_clip!=this) OnEditPasteElevMap();
  if (app->domap_clip && app->domap_clip!=this) OnEditPasteDLPRmap();
  if (app->divide_tree_clip && app->divide_tree_clip!=this) OnEditPasteDLPRtree();
  if (app->database_clip && app->database_clip!=this) OnEditPasteDatabase();
}

void CPromDoc::OnUpdateEditPasteAll(CCmdUI *pCmdUI)
{
  CPromApp *app=CPromApp::getApp();
  pCmdUI->Enable(tree_writeable &&
    ((app->elev_map_clip && app->elev_map_clip!=this) ||
     (app->domap_clip && app->domap_clip!=this) ||
     (app->divide_tree_clip && app->divide_tree_clip!=this) ||
     (app->database_clip && app->database_clip!=this)));
}

void CPromDoc::OnOpFlatUndefine()
{
  Rectangl limits=elev;
  CDefineFlat_dlg dlg("Undefine Flat Regions",limits);
  if (dlg.DoModal()!=IDOK) return;

  BeginWaitCursor();
  undefine_flat(elev,limits,dlg.elev_lmt);
  EndWaitCursor();
  UpdateViews(&dummyModifyMap);
  em_state=CHANGED;
}

void CPromDoc::OnOpFlatDefine()
{
  Rectangl limits=elev;
  CDefineFlat_dlg dlg("Define Flat Regions",limits);
  if (dlg.DoModal()!=IDOK) return;

  BeginWaitCursor();
  define_flat(elev,limits,dlg.elev_lmt);
  EndWaitCursor();
  UpdateViews(&dummyModifyMap);
  em_state=CHANGED;
}

void CPromDoc::OnUpdateHaveEMDT(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(em_state!=NOT_LOADED && dt_state!=NOT_LOADED);
}

void CPromDoc::OnOpDiagErrors()
{
  CErrorListView::init_list(this);
}

void CPromDoc::OnUpdateOpAnalyze(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) enable_submenu(pCmdUI,em_state!=NOT_LOADED && tree_writeable);
  pCmdUI->Enable(em_state!=NOT_LOADED && tree_writeable);
}

// helps sort peaks by location
struct FeatureLocation {
  Feature *featr;
  FeatureLocation(Feature *f=0) : featr(f) {}
  bool operator<(const FeatureLocation& a) const
    {return featr->location<a.featr->location;}
};

static Feature *search_featr(const vector<Feature *> featrs, const Feature& target)
{
  int lo=0,hi=featrs.size(),mid;
  while (hi>lo) {
    mid=(lo+hi)/2;
    if (target.location<featrs[mid]->location)
      hi=mid;
    else if (target.location>featrs[mid]->location)
      lo=mid+1;
    else return featrs[mid];
  }
  return 0;
}

static void AddToFeatrList(CFeatureListView *&featr_list, Feature *featr,
			   FT_index ft, CPromDoc *doc)
{
  if (featr_list==0) featr_list=CFeatureListView::init_list(doc);
  featr_list->AddItem(*featr,ft);
}

static void AddToDupList(CFeatureListView *&duplicates, vector <Feature *> search_list,
			 Feature& target, CPromDoc *doc,
			 FT_index ft_search, FT_index ft_target)
{
  if (!target.location) return;
  Feature *dup_find=search_featr(search_list,target);
  if (dup_find==0) return;
  AddToFeatrList(duplicates,dup_find,ft_search,doc);
  AddToFeatrList(duplicates,&target,ft_target,doc);
}

class Cmp_featr_loc {
public:
  bool operator()(const Feature *a, const Feature *b) const
    {return a->location<b->location;}
};

void CPromDoc::OnOpDiagDups()
{
  BeginWaitCursor();
  vector<Feature *> peaks;
  CFeatureListView *duplicates=0;
  Domain::Index ndom=tree.n_dom(),nbs=tree.n_bsnsdl(),i;
  for (i=0; i<ndom; ++i)
    if (tree[i].peak.location) peaks.push_back(&tree[i].peak);
  if (peaks.empty()) {
    EndWaitCursor();
    return;
  }
  sort(peaks.begin(),peaks.end(),Cmp_featr_loc());

  for (Fiter fi=peaks.begin(); fi!=peaks.end(); ++fi) {
    if (fi+1!=peaks.end() && (*fi)->location==(*(fi+1))->location ||
	fi!=peaks.begin() && (*fi)->location==(*(fi-1))->location)
      AddToFeatrList(duplicates,*fi,FT_PEAK,this);
  }
  for (i=0; i<ndom; ++i)
    AddToDupList(duplicates,peaks,tree[i].saddle,
		 this,FT_PEAK,FT_SADDLE);
  for (i=0; i<nbs; ++i)
    AddToDupList(duplicates,peaks,tree.bsnsdl(i),
		 this,FT_PEAK,FT_BASINSADL);

  if (duplicates==0) {
    AfxGetMainWnd()->
      MessageBox("No suspicious duplicate features were found.",
		 "Winprom",MB_OK|MB_ICONINFORMATION);
  }
  else duplicates->setItemIndicators();
  EndWaitCursor();
}

void CPromDoc::elev_flip_drain()
{
  if (is_drainage) elev_flip();
}

void CPromDoc::elev_flip()
{
  Grid grid(elev);
  Grid_iter gi(grid);
  for (; gi; ++gi)
    if (elev[*gi]!=elev_undef)
      elev[*gi]*=-1;
}

bool CPromDoc::elev_transform(float m, float b)
{
  char buf2[32];
  float y,u,v;
  Grid grid(elev);
  Grid_iter gi(grid);
  if (m!=0) {
    // precalculate interval within which transformation will be within range
    u=(-elev_infty-b)/m;
    v=( elev_infty-b)/m;
    if (m<0) {
      y=u;
      u=v;
      v=y;
    }
    // check if all grid points would transform to a value within range
    for (; gi; ++gi) {
      if (elev[*gi]!=elev_undef) {
	if (elev[*gi]>=v || elev[*gi]<=u) {
	  (*gi).sprint(buf2);
	  sprintf(buf,"Transformed elevation is out of range!\n"
		  "Location %s, elevation %d",buf2,elev[*gi]);
	  AfxGetMainWnd()->MessageBox(buf,"Winprom",MB_OK|MB_ICONERROR);
	  return false;
	}
      }
    }
  }
  else {
    if (b>=elev_infty || b<=-elev_infty) {
      sprintf(buf,"That would transform every elevation to %d,\n"
	      "which is out of range!\n",elev_nint(b));
      AfxGetMainWnd()->MessageBox(buf,"Winprom",MB_OK|MB_ICONERROR);
      return false;
    }
  }
  // All grid points transform within range.  Now do the transformation for real.
  for (gi.reset(); gi; ++gi)
    if (elev[*gi]!=elev_undef)
      elev[*gi]=elev_nint(m*elev[*gi]+b);
  em_state=CHANGED;
  UpdateViews(&dummyModifyMap);
  return true;
}

void CPromDoc::OnOpElevTransform()
{
  CTransform_dlg dlg;
  if (dlg.DoModal()==IDOK) {
    BeginWaitCursor();
    elev_transform(dlg.m_mult,dlg.m_add);
    EndWaitCursor();
  }
}

bool CPromDoc::canTakeDrop(CWnd *source, CPoint dropPt)
{
  CPoint ptRoot(dropPt);
  source->MapWindowPoints(NULL,&ptRoot,1);
  CWnd *dropWin=CWnd::WindowFromPoint(ptRoot);
  if (dropWin!=NULL && dropWin->IsKindOf(RUNTIME_CLASS(CView)) &&
      ((CView *)dropWin)->GetDocument()==this && dropWin!=source) {
    if (dropWin->IsKindOf(RUNTIME_CLASS(CFeatureListView))) {
      CFeatureListView *dropList=(CFeatureListView *)dropWin;
      if (dropList->CanPaste(dragDrop)) return true;
    }
    else if (dropWin->IsKindOf(RUNTIME_CLASS(CGraphView))) return true;
    else if (dropWin->IsKindOf(RUNTIME_CLASS(CLPRtreeView))) return true;
    else if (dropWin->IsKindOf(RUNTIME_CLASS(CDataListView))) {
      CDataListView *dropList=(CDataListView *)dropWin;
      if (dropList->CanPaste(dragDrop)) return true;
    }
  }
  return false;
}

void CPromDoc::takeDrop(CWnd *source, CPoint dropPt)
{
  CPoint ptRoot(dropPt);
  source->MapWindowPoints(NULL,&ptRoot,1);
  CWnd *dropWin=CWnd::WindowFromPoint(ptRoot);
  ReleaseCapture();
  CImageList::DragLeave(NULL);
  CImageList::EndDrag();
  if (dropWin!=NULL && dropWin->IsKindOf(RUNTIME_CLASS(CView)) &&
      ((CView *)dropWin)->GetDocument()==this && dropWin!=source) {
    if (dropWin->IsKindOf(RUNTIME_CLASS(CFeatureListView))) {
      CFeatureListView *dropList=(CFeatureListView *)dropWin;
      if (dropList->CanPaste(dragDrop))
	dropList->pasteClip(dragDrop);
    }
    else if (dropWin->IsKindOf(RUNTIME_CLASS(CGraphView)))
      ((CGraphView *)dropWin)->pasteClip(dragDrop);
    else if (dropWin->IsKindOf(RUNTIME_CLASS(CLPRtreeView)))
      ((CLPRtreeView *)dropWin)->pasteClip(dragDrop);
    else if (dropWin->IsKindOf(RUNTIME_CLASS(CDataListView))) {
      CDataListView *dropList=(CDataListView *)dropWin;
      if (dropList->CanPaste(dragDrop))
	dropList->pasteClip(dragDrop);
    }
  }
  dragDrop.clear();
}

void CPromDoc::type2image(FT_index& type) const
{
  if (is_drainage) {
    switch (type) {
    case FT_PEAK: type=FT_BASIN; break;
    case FT_SADDLE: type=FT_BASINSADL; break;
    case FT_RUNOFF: type=FT_SBRO; break;
    case FT_BASINSADL: type=FT_SADDLE; break;
    }
  }
}

void CPromDoc::image2type(FT_index& type) const
{
  if (is_drainage) {
    switch (type) {
    case FT_BASIN: type=FT_PEAK; break;
    case FT_BASINSADL: type=FT_SADDLE; break;
    case FT_SADDLE: type=FT_BASINSADL; break;
    case FT_SBRO: type=FT_RUNOFF; break;
    }
  }
}

void CPromDoc::imageFlip(FT_index& image)
{
  switch (image) {
  case FT_PEAK: image=FT_BASIN; break;
  case FT_SADDLE: image=FT_BASINSADL; break;
  case FT_RUNOFF: image=FT_SBRO; break;
  case FT_BASINSADL: image=FT_SADDLE; break;
  case FT_SBRO: image=FT_RUNOFF; break;
  case FT_BASIN: image=FT_PEAK; break;
  }
}

static void add2listView(CFeatureListView& flist, FT_index type,
			 Fiter begin, Fiter end)
{
  for (Fiter fi=begin; fi!=end; ++fi)
    flist.AddItem(**fi,type);
}

static void add2listView(CFeatureListView& flist,
		         vector<Feature *> peaks, vector<Feature *> sadls,
			 vector<Feature *> runoffs, vector<Feature *> bsnsdls)
{
  add2listView(flist,FT_PEAK,peaks.begin(),peaks.end());
  add2listView(flist,FT_SADDLE,sadls.begin(),sadls.end());
  add2listView(flist,FT_RUNOFF,runoffs.begin(),runoffs.end());
  add2listView(flist,FT_BASINSADL,bsnsdls.begin(),bsnsdls.end());
  flist.setItemIndicators();
}

void CPromDoc::OnEditTransfer()
{
#ifdef administrator
  static Assoc_info assoc(1,true);
  static Divide_tree_map::Sadl_src sadl_src=Divide_tree_map::SS_ALL;
  CXferTree_dlg dlg(em_state!=NOT_LOADED,dm_state!=NOT_LOADED);
  dlg.m_assoc=assoc;
  dlg.m_sadl_source=sadl_src;
  if (dlg.DoModal()!=IDOK) return;
  assoc=dlg.m_assoc;
  sadl_src=(Divide_tree_map::Sadl_src)dlg.m_sadl_source;

  BeginWaitCursor();
  vector<Feature *> orphan_peak,orphan_sadl,orphan_ro,orphan_bs,
    conflict_peak,conflict_sadl,conflict_ro,conflict_bs;
  CPromDoc *source_doc=CPromApp::getApp()->divide_tree_clip;
  if (is_drainage) {
    tree.transform(-1,0,true,true,true,true,true);
    source_doc->tree.transform(-1,0,true,true,true,true,true);
  }
  tree.xfer_featr_info(source_doc->tree,total_alloc.rect,domain,elev,
		       assoc,sadl_src,
		       orphan_peak,orphan_sadl,orphan_ro,orphan_bs,
		       conflict_peak,conflict_sadl,conflict_ro,conflict_bs,
		       prom_ss);
  if (is_drainage) {
    tree.transform(-1,0,true,true,true,true,true);
    source_doc->tree.transform(-1,0,true,true,true,true,true);
  }

  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyAll);
  if (!orphan_peak.empty() || !orphan_sadl.empty() ||
      !orphan_ro.empty() || !orphan_bs.empty()) {
    if (AfxGetMainWnd()->
	MessageBox("Some features in the source tree were\n"
		   "not found in the destination tree.\n"
		   "Would you like to see a list of them?",
		   "Winprom Information Transfer",
		   MB_YESNO|MB_ICONWARNING)==IDYES) {
      CFeatureListView *orphlist=
	CFeatureListView::init_list(source_doc,"Unmatched Features");
      add2listView(*orphlist,orphan_peak,orphan_sadl,orphan_ro,orphan_bs);
    }
  }
  if (!conflict_peak.empty() || !conflict_sadl.empty() ||
      !conflict_ro.empty() || !conflict_bs.empty()) {
    if (AfxGetMainWnd()->
	MessageBox("Some features in the source tree conflicted with\n"
		   "those at the same location in the destination\n"
		   "tree.  Would you like to see a list of them?",
		   "Winprom Information Transfer",
		   MB_YESNO|MB_ICONWARNING)==IDYES) {
      CFeatureListView *conflist=
	CFeatureListView::init_list(this,"Conflicting Features");
      add2listView(*conflist,conflict_peak,conflict_sadl,conflict_ro,conflict_bs);
    }
  }
  EndWaitCursor();
#else
  sprintf(buf,
    "Intertree information transfer is an administrative command\n"
    "and not implemented in this shipment of Winprom.\n"
    "Contact the Custodian for the %s tree(s) of interest.",
    is_drainage?"drainage":"divide");
  AfxGetMainWnd()->MessageBox(buf,"Winprom",MB_OK|MB_ICONERROR);
#endif
}

void CPromDoc::OnUpdateEditTransfer(CCmdUI *pCmdUI)
{
  CPromDoc *source=CPromApp::getApp()->divide_tree_clip;
  pCmdUI->Enable(dt_state!=NOT_LOADED && source!=0 && source!=this &&
		 tree_writeable && is_drainage==source->is_drainage);
}

void CPromDoc::OnOpDiagSWI()
{
  CSaddleWalkInvView::init_list(this);
}

void CPromDoc::OnUpdateOpDiagSWI(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(em_state!=NOT_LOADED && dt_state!=NOT_LOADED);
}

void CPromDoc::tree_flip_drain()
{
  if (is_drainage) tree.transform(-1,0,true,true,true,true,true);
}

bool CPromDoc::tree_transform(float m, float b, bool xform_peak, bool xform_sadl,
			      bool xform_ro, bool xform_bs, bool trans_edit)
{
  const Feature *badf=tree.transform(m,b,xform_peak,xform_sadl,
				     xform_ro,xform_bs,trans_edit);
  if (badf) {
    char buf2[32];
    badf->location.sprint(buf2);
    sprintf(buf,"Transformed feature elevation is out of range!\n"
	    "Location %s, elevation range %d-%d",buf2,badf->elev.low,badf->elev.high);
    AfxGetMainWnd()->MessageBox(buf,"Winprom",MB_OK|MB_ICONERROR);
    return false;
  }
  if (m<0) {
    is_drainage=!is_drainage;
    set_tb_bmp();
  }
  trans_m=m;
  trans_b=b;
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateViews(&dummyModifyTrans);
  return true;
}

void CPromDoc::OnOpTreeTransform()
{
  static bool xform_peak=true,xform_sadl=true,xform_ro=true,xform_bs=true,
    xform_edit=false;

  CTransformTree_dlg dlg;
  dlg.m_peaks=xform_peak;
  dlg.m_sadls=xform_sadl;
  dlg.m_runoffs=xform_ro;
  dlg.m_basnsadls=xform_bs;
  dlg.m_edited=xform_edit;
  if (dlg.DoModal()!=IDOK) return;
  xform_peak=dlg.m_peaks!=FALSE;
  xform_sadl=dlg.m_sadls!=FALSE;
  xform_ro=dlg.m_runoffs!=FALSE;
  xform_bs=dlg.m_basnsadls!=FALSE;
  xform_edit=dlg.m_edited!=FALSE;

  BeginWaitCursor();
  tree_transform(dlg.m_mult,dlg.m_add,
    xform_peak,xform_sadl,xform_ro,xform_bs,xform_edit);
  EndWaitCursor();
}

void CPromDoc::OnOpDiagAmbOff()
{
  static FeatureTypeFilter amboff_filter;
  FeatureTypeFilter m_filter=amboff_filter;
  m_filter.rec_filter.align(data);
  if (CFeatureTypeFilter_dlg(amboff_filter,NULL).DoModal()!=IDOK) return;

  update_sadl_prom_info();

  amboff_filter=m_filter;
  Domain::Index n=tree.n_dom(),i,j;
  Domain *d;
  bool *sadl_listed=new bool[n],*peak_listed=new bool[n];
  for (i=0; i<n; ++i)
    sadl_listed[i]=peak_listed[i]=false;
  CFeatureListView *amb_list=0;
  for (i=0; i<n; ++i) {
    Domain& dom=tree[i];
    if (!dom.peak.location || dom.parent.line==0) continue;
    for (d=dom.parent.line; true; d=d->parent.line) {
      j=tree.index(d);
      if (d->parent.line!=0 && d->saddle.elev.low>dom.saddle.elev.high) continue;
      if (d->peak.elev.high>=dom.peak.elev.low &&
	  d->peak.elev.low<=dom.peak.elev.high) {
	if (!peak_listed[i] && amboff_filter.test(dom.peak,FT_PEAK,data)) {
	  AddToFeatrList(amb_list,&dom.peak,FT_PEAK,this);
	  peak_listed[i]=true;
	}
	if (!peak_listed[j] && amboff_filter.test(d->peak,FT_PEAK,data)) {
	  AddToFeatrList(amb_list,&d->peak,FT_PEAK,this);
	  peak_listed[j]=true;
	}
      }
      if (d->parent.line==0 || d->saddle.elev.high<dom.saddle.elev.low) break;
      if (d->saddle.elev.low<=dom.saddle.elev.high) {
	if (!sadl_listed[i] && amboff_filter.test(dom.saddle,FT_SADDLE,data)) {
	  AddToFeatrList(amb_list,&dom.saddle,FT_SADDLE,this);
	  sadl_listed[i]=true;
	}
	if (!sadl_listed[j] && amboff_filter.test(d->saddle,FT_SADDLE,data)) {
	  AddToFeatrList(amb_list,&d->saddle,FT_SADDLE,this);
	  sadl_listed[j]=true;
	}
      }
    }
  }
  if (amb_list==0) {
    AfxGetMainWnd()->
      MessageBox("No tree-ambiguating features were found.",
		 "Winprom",MB_OK|MB_ICONINFORMATION);
  }
  else amb_list->setItemIndicators();
  delete[] peak_listed;
  delete[] sadl_listed;
}

static void AddToOvlpList(CFeatureListView *&overlaps, CPromDoc *doc,
			  Domain& peak, Feature& nbr, FT_index nbr_type)
{
  if (!peak.peak.edited || !nbr.edited ||
      peak.peak.elev.low>nbr.elev.high) return;
  AddToFeatrList(overlaps,&peak.peak,FT_PEAK,doc);
  AddToFeatrList(overlaps,&nbr,nbr_type,doc);
}

void CPromDoc::OnOpDiagOvlpNbrs()
{
  CFeatureListView *overlaps=0;

  Domain::Index i,n=tree.n_dom();
  for (i=0; i<n; ++i) {
    Domain& dom=tree[i];
    if (dom.primary_nbr) {
      AddToOvlpList(overlaps,this,dom,dom.saddle,FT_SADDLE);
      AddToOvlpList(overlaps,this,*dom.primary_nbr,dom.saddle,FT_SADDLE);
    }
  }

  n=tree.n_runoff();
  for (i=0; i<n; ++i) {
    Runoff& ro=tree.runoff(i);
    if (ro && ro.peak) AddToOvlpList(overlaps,this,*ro.peak,ro,FT_RUNOFF);
  }

  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i) {
    Basin_saddle& bs=tree.bsnsdl(i);
    if (bs) {
      AddToOvlpList(overlaps,this,*bs.peak1,bs,FT_BASINSADL);
      AddToOvlpList(overlaps,this,*bs.peak2,bs,FT_BASINSADL);
    }
  }

  if (overlaps==0) {
    AfxGetMainWnd()->
      MessageBox("No overlapping neighbors were found.",
		 "Winprom",MB_OK|MB_ICONINFORMATION);
  }
  else overlaps->setItemIndicators();
}

void CPromDoc::OnListDatabase()
{
  CDataListView::init_list(this);
}

static void not_orphan(const GridPoint& gp, const Database& data, bool *nonorph)
{
  if (!gp) return;
  Database::RecordID rid=data.get_record(gp);
  if (rid!=REC_NONE) nonorph[rid]=true;
}

// allocate an array of booleans, and fill it with an indication
// of whether or not each data record is an orphan
bool *CPromDoc::nonorphans() const
{
  Domain::Index i,n;
  Database::RecordID nrec=data.get_nrec(),rid;
  bool *nonorph=new bool[nrec];
  for (rid=0; rid<nrec; ++rid) nonorph[rid]=false;
  n=tree.n_dom();
  for (i=0; i<n; ++i) {
    not_orphan(tree[i].peak.location,data,nonorph);
    not_orphan(tree[i].saddle.location,data,nonorph);
  }
  n=tree.n_runoff();
  for (i=0; i<n; ++i)
    not_orphan(tree.runoff(i).location,data,nonorph);
  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i)
    not_orphan(tree.bsnsdl(i).location,data,nonorph);

  return nonorph;
}

static void undefine_rec_array(Database::RecordID array[], Domain::Index n)
{
  if (array==0) return;
  for (Database::RecordID i=0; i<n; ++i)
    array[i]=REC_NONE;
}

static void check_align_pri(const Database& data, Database::const_FieldID fi,
			    const GridPoint& ftr_loc, short int sign,
			    Database::RecordID& cur_rec,
			    Database::RecordID new_rec)
{
  if (cur_rec==REC_NONE)
    // no other record matched the same feature (yet)
    cur_rec=new_rec;
  else {
    // some other record matched the same feature.
    if (fi==data.begin_field()) {
      // use distance to decide which one gets it.
      if (data.get_location(new_rec)-ftr_loc <
	  data.get_location(cur_rec)-ftr_loc)
	cur_rec=new_rec;
    }
    else {
      // use selected data field to decide which one gets it.
      if (data.field_type(fi)==FLD_INTEGER) {
	int ival_old,ival_new;
	data.get_value(cur_rec,fi,ival_old);
	data.get_value(new_rec,fi,ival_new);
	if ((ival_new-ival_old)*sign>0) cur_rec=new_rec;
      }
      else if (data.field_type(fi)==FLD_REAL) {
	float rval_old,rval_new;
	data.get_value(cur_rec,fi,rval_old);
	data.get_value(new_rec,fi,rval_new);
	if ((rval_new-rval_old)*sign>0) cur_rec=new_rec;
      }
      else assert(0);
    }
  }
}

void CPromDoc::OnOpDataAlign()
{
  static Assoc_info assoc(1,false);
  static bool use_prom=true,use_dist=true,orphans=false,excise=false;
  static enum Pritype {DT_PRS,DT_PSR,DT_SPR,DT_SRP,DT_RPS,DT_RSP}
    pritype=DT_PRS;
  CDataAlign_dlg dlg(em_state!=NOT_LOADED,dm_state!=NOT_LOADED,data);
  dlg.m_assoc=assoc;
  dlg.m_prom=use_prom;
  dlg.m_distance=use_dist;
  dlg.m_pritype=pritype;
  dlg.m_orphan=orphans;
  dlg.m_excise=excise;
  if (dlg.DoModal()!=IDOK) return;
  assoc=dlg.m_assoc;
  use_prom=dlg.m_prom!=FALSE;
  use_dist=dlg.m_distance!=FALSE;
  pritype=(Pritype)dlg.m_pritype;
  orphans=dlg.m_orphan!=FALSE;
  excise=dlg.m_excise!=FALSE;

#ifdef timer
  clock_t clk=clock();
#endif
  BeginWaitCursor();
  zero_prob=prom_zp;

  // set up sorted feature and association arrays, if necessary
  Domain::Index ndom=tree.n_dom(),nro=tree.n_runoff(),nbs=tree.n_bsnsdl(),
    ipeak,isadl,iro,ibs;
  Elevation *peak_prom=0,*sadl_prom=0,bs_prom;
  const Elev_intvl high=elev_infty;
  Database::RecordID *peak_assoc=0,*sadl_assoc=0,*ro_assoc=0,*bs_assoc=0,
    rec,old_rec,nrec=data.get_nrec();
  Topo_info *island_tree;
  vector<Domain *> sorted_peaks,sorted_sadls;
  vector<Runoff *> sorted_ro;
  vector<Basin_saddle *> sorted_bs;
  Rectangl bound;
  tree_flip_drain();
  if (assoc.pk_dest) {
    tree.sort_doms(&Domain::peak,sorted_peaks);
    peak_assoc=new Database::RecordID[ndom];
    undefine_rec_array(peak_assoc,ndom);
    if (assoc.elev_prom || use_prom) {
      // calculate peak prominence
      peak_prom=new Elevation[ndom];
      island_tree=tree.get_island_tree(LO_END);
      tree.assoc_peak_prom(island_tree,peak_prom);
      delete[] island_tree;
    }
  }
  if (assoc.ps_dest || assoc.bs_dest) {
    if (assoc.ps_dest) {
      tree.sort_doms((Feature Domain::*)&Domain::saddle,sorted_sadls);
      sadl_assoc=new Database::RecordID[ndom];
      undefine_rec_array(sadl_assoc,ndom);
    }
    if (assoc.bs_dest) {
      tree.sort_bs(sorted_bs);
      bs_assoc=new Database::RecordID[nbs];
      undefine_rec_array(bs_assoc,nbs);
    }
    if (assoc.elev_prom || use_prom) {
      // calculate saddle prominence.
      // Unlike peaks, only one saddle in a group of rivals gets large prominence.
      sadl_prom=new Elevation[ndom];
      island_tree=tree.get_island_tree(HI_END);
      for (ipeak=0; ipeak<ndom; ++ipeak) {
	if (!tree[ipeak].peak.location ||
	    island_tree[ipeak].parnt==Domain::undef) continue;
	isadl=island_tree[ipeak].sadl_dom;
	sadl_prom[isadl]=tree[ipeak].peak.elev.high-tree[isadl].saddle.elev.high;
      }
      delete[] island_tree;
    }
  }
  if (assoc.ro_dest) {
    tree.sort_ro(sorted_ro);
    ro_assoc=new Database::RecordID[nro];
    undefine_rec_array(ro_assoc,nro);
  }
#ifdef timer
  TRACE("data align took %d clock ticks to point 10\n",clock()-clk);
#endif
  const Grid grid(elev);
  Bool_map mark(elev);
  mark.fill(false);
  bool *nonorph=0,warned_orphan=false;
  vector<GridPoint> orphan_list;

  if (orphans) nonorph=nonorphans();

  // find bounding rectangle
  if (assoc.pk_dest)
    for (ipeak=0; ipeak<ndom; ++ipeak) {
      const GridPoint& loc=tree[ipeak].peak.location;
      if (loc) bound|=loc;
    }
  if (assoc.ps_dest)
    for (isadl=0; isadl<ndom; ++isadl) {
      const GridPoint& loc=tree[isadl].saddle.location;
      if (loc) bound|=loc;
    }
  if (assoc.ro_dest)
    for (iro=0; iro<nro; ++iro) {
      const GridPoint& loc=tree.runoff(iro).location;
      if (loc) bound|=loc;
    }
  if (assoc.bs_dest)
    for (ibs=0; ibs<nbs; ++ibs) {
      const GridPoint& loc=tree.bsnsdl(ibs).location;
      if (loc) bound|=loc;
    }

  // find feature (if any) associated with each record
  for (rec=0; rec<nrec; ++rec) {
    if (orphans && nonorph[rec]) continue;
    const GridPoint& location=data.get_location(rec);
    if (!bound.contains(location)) continue;
    // find the associated peak (if any)
    ipeak=assoc.pk_dest?
      tree.assoc_peak(location,assoc,domain,elev,grid,mark,peak_prom,sorted_peaks):
      Domain::undef;
    // find the associated saddle (if any)
    isadl=tree.assoc_sadl(location,assoc,0,0,0,0,sorted_sadls,sadl_prom,
	assoc.elev_prom || use_prom ? &bs_prom : 0, prom_ss,sorted_bs,ibs);
    // find the associated runoff (if any)
    iro=tree.assoc_ro(location,assoc,sorted_ro);
    if (ipeak!=Domain::undef || isadl!=Domain::undef ||
	iro!=Domain::undef || ibs!=Domain::undef) {
      // we got a match. Decide which feature type will be used.
      if (use_prom) {
	// try to choose type by prominence
	Elevation
	  p_prom = ipeak!=Domain::undef ? peak_prom[ipeak] : -elev_infty ,
	  s_prom = isadl!=Domain::undef ? sadl_prom[isadl] :
		   ibs!=Domain::undef ? bs_prom : -elev_infty;
	if (p_prom>s_prom) isadl=Domain::undef;
	if (s_prom>p_prom) ipeak=Domain::undef;
      }
      if (use_dist) {
	// try to choose type by distance
	const GridPoint
	  *peak_loc = ipeak!=Domain::undef ? &tree[ipeak].peak.location : 0,
	  *sadl_loc = isadl!=Domain::undef ? &tree[isadl].saddle.location :
		      ibs!=Domain::undef ? &tree.bsnsdl(ibs).location : 0,
	  *ro_loc = iro!=Domain::undef ? &tree.runoff(iro).location : 0;
	Distance
	  p_dist = peak_loc ? *peak_loc-location : GridPoint::huje,
	  s_dist = sadl_loc ? *sadl_loc-location : GridPoint::huje,
	  r_dist = ro_loc   ? *ro_loc-location   : GridPoint::huje;
	if (p_dist>s_dist || p_dist>r_dist) ipeak=Domain::undef;
	if (s_dist>p_dist || s_dist>r_dist) isadl=ibs=Domain::undef;
	if (r_dist>p_dist || r_dist>s_dist) iro=Domain::undef;
      } // if (use_dist)
      // break any remaining ties by priority
      static short type_pri[][3] = {2,1,0, 2,0,1, 1,2,0, 0,2,1, 1,0,2, 0,1,2};
      short p_pri=type_pri[pritype][0],
	s_pri=type_pri[pritype][1],
	r_pri=type_pri[pritype][2];
      if (ipeak==Domain::undef) p_pri=-1;
      if (isadl==Domain::undef) s_pri=-1;
      if (iro  ==Domain::undef) r_pri=-1;
      if (p_pri<s_pri || p_pri<r_pri) ipeak=Domain::undef;
      if (s_pri<p_pri || s_pri<r_pri) isadl=ibs=Domain::undef;
      if (r_pri<p_pri || r_pri<s_pri) iro=Domain::undef;

      // record the match in the association array for the appropriate feature type.
      assert((ipeak!=Domain::undef) + (isadl!=Domain::undef) +
	     (iro!=Domain::undef) + (ibs!=Domain::undef) == 1);
      if (ipeak!=Domain::undef) {
	// it matched a peak
	check_align_pri(data,dlg.mrf_peak,tree[ipeak].peak.location,
			1-2*(dlg.m_mult_rec_peak%2),peak_assoc[ipeak],rec);
      }
      else if (isadl!=Domain::undef) {
	// it matched a prom saddle
	check_align_pri(data,dlg.mrf_sadl,tree[isadl].saddle.location,
			1-2*(dlg.m_mult_rec_sadl%2),sadl_assoc[isadl],rec);
      }
      else if (iro!=Domain::undef) {
	// it matched a runoff
	check_align_pri(data,dlg.mrf_peak,tree.runoff(iro).location,
			1-2*(dlg.m_mult_rec_peak%2),ro_assoc[iro],rec);
      }
      else {
	// it matched a basin saddle
	check_align_pri(data,dlg.mrf_sadl,tree.bsnsdl(ibs).location,
			1-2*(dlg.m_mult_rec_sadl%2),bs_assoc[ibs],rec);
      }
    } // if (match)
    else {
      // no match.
      if (!warned_orphan) {
	// warn about data orphan if not already done so.
	if (AfxGetMainWnd()->
	    MessageBox("A data record could not match any feature.\n"
		       "Would you like to see a list of all unmatchable data records?",
		       "Winprom",MB_YESNO|MB_ICONWARNING)==IDYES)
	  orphan_list.push_back(data.get_location(rec));
	warned_orphan=true;
      }
      else if (!orphan_list.empty()) {
	// prior orphan, and list is nonempty.
	// Therefore the user wants a list, so add to it.
	orphan_list.push_back(data.get_location(rec));
      }
    }
  } // for (rec)
#ifdef timer
  TRACE("data align took %d clock ticks to point 20\n",clock()-clk);
#endif

  // Now examine association arrays for records to be aligned to features.
  // we don't actually change record locations yet, we just make a list of changes
  // so that conflicts can be sorted out.
  bool conflict=false,*keep_rec=excise ? new bool[nrec] : 0;
  vector<GridPoint> conflict_list;
  GridPoint *new_loc=new GridPoint[nrec];
  for (rec=0; rec<nrec; ++rec) new_loc[rec]=GridPoint::undefined;
  if (assoc.pk_dest)
    for (ipeak=0; ipeak<ndom; ++ipeak)
      if (peak_assoc[ipeak]!=REC_NONE)
	new_loc[peak_assoc[ipeak]]=tree[ipeak].peak.location;
  if (assoc.ps_dest)
    for (isadl=0; isadl<ndom; ++isadl)
      if (sadl_assoc[isadl]!=REC_NONE)
	new_loc[sadl_assoc[isadl]]=tree[isadl].saddle.location;
  if (assoc.ro_dest)
    for (iro=0; iro<nro; ++iro)
      if (ro_assoc[iro]!=REC_NONE)
	new_loc[ro_assoc[iro]]=tree.runoff(iro).location;
  if (assoc.bs_dest)
    for (ibs=0; ibs<nbs; ++ibs)
      if (bs_assoc[ibs]!=REC_NONE)
	new_loc[bs_assoc[ibs]]=tree.bsnsdl(ibs).location;
#ifdef timer
  TRACE("data align took %d clock ticks to point 30\n",clock()-clk);
#endif
  // Now examine the list of changes for conflicts.
  if (excise) for (rec=0; rec<nrec; ++rec) keep_rec[rec]=true;
  for (rec=0; rec<nrec; ++rec) {
    if (!new_loc[rec]) continue; // no plans to move it
    if (new_loc[rec]==data.get_location(rec)) continue; // record was already aligned with preferred feature
    old_rec=data.get_record(new_loc[rec]);
    if (old_rec==REC_NONE) continue;
    // if we get here, another record already exists where we wanted to move this one!
    if (excise) keep_rec[old_rec]=false;
    else {
      // since we're not excising conflicts, we have to abort the change
      new_loc[rec]=GridPoint::undefined;
      if (!conflict) {
	// no prior conflict. Warn user and inquire about list
	if (AfxGetMainWnd()->
	    MessageBox("A data record was associated with a feature that already has one.\n"
		       "Would you like to see a list of all such data records?",
		       "Winprom",MB_YESNO|MB_ICONWARNING)==IDYES)
	  conflict_list.push_back(data.get_location(rec));
	conflict=true;
      }
      else if (!conflict_list.empty()) {
	// prior conflict, and list is nonempty.
	// Therefore the user wants a list, so add to it.
	conflict_list.push_back(data.get_location(rec));
      }
    }
  }
  for (rec=0; rec<nrec; ++rec)
    if (new_loc[rec])
      data.change_location(rec,new_loc[rec]);
  if (excise) data.purge(keep_rec);
  data.bubble_sort();

  db_state=CHANGED;
  tree_flip_drain();
  record_change=REC_NONE;
  UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyData);

  if (!conflict_list.empty())
    CDataListView::init_list(this,conflict_list,"Conflict data");

  if (!orphan_list.empty())
    CDataListView::init_list(this,orphan_list,"Orphaned data");

  delete[] peak_prom;
  delete[] sadl_prom;
  delete[] nonorph;
  delete[] peak_assoc;
  delete[] sadl_assoc;
  delete[] ro_assoc;
  delete[] bs_assoc;
  delete[] new_loc;
  delete[] keep_rec;

  EndWaitCursor();
#ifdef timer
  TRACE("data align took %d clock ticks\n",clock()-clk);
#endif
}

void CPromDoc::OnOpDataPurge()
{
  if (!db_CanClose(true)) return;
  Record_location_filter filter;
  filter.align(data);
  CDataLocationFilter_dlg dlg(filter);
  if (dlg.DoModal()!=IDOK) return;

  Database::RecordID rid,nrec=data.get_nrec();
  bool *keep_rec=nonorphans(); // temporary use of keep_rec for orphan status
  for (rid=0; rid<nrec; ++rid)
    keep_rec[rid]=filter.test(data,rid,keep_rec[rid]); // real use of keep_rec
  if (data.purge(keep_rec)) {
    db_state=CHANGED;
    db_save_drastic_change=false;
    record_change=REC_NONE;
    UpdateAllViews(0,(LPARAM)&CPromDoc::dummyModifyData);
  }
  delete[] keep_rec;
}

void CPromDoc::set_rect(const Rectangl& limits,
	CString& m_north, CString& m_south, CString& m_east, CString& m_west)
{
  const GridPoint sw=limits.corner1(),ne=limits.corner2();
  if (limits.limit[0].high<coord_infty) {
    ne.sprint1(buf);
    m_north=buf;
  }
  else m_north.Empty();
  if (limits.limit[0].low>-coord_infty) {
    sw.sprint1(buf);
    m_south=buf;
  }
  else m_south.Empty();
  if (limits.limit[1].high<coord_infty) {
    ne.sprint2(buf);
    m_east=buf;
  }
  else m_east.Empty();
  if (limits.limit[1].low>-coord_infty) {
    sw.sprint2(buf);
    m_west=buf;
  }
  else m_west.Empty();
}

static GridPoint::Coord get_limit(const CString& m_limit, int sign,
	GridPoint::Coord (*scan_func)(const char *), CEdit& edit_ctl,
	const char *coord_label, const char *bound_label, CWnd *parent)
{
  GridPoint::Coord limit;
  if (m_limit.IsEmpty()) limit=sign*coord_infty;
  else {
    limit=(*scan_func)(m_limit);
    if (limit==coord_infty+1) {
      char buf[128];
      sprintf(buf,"Enter a %s for the %s limit\n"
		  "or leave blank if no restriction",coord_label,bound_label);
      parent->MessageBox(buf,"Winprom Entry Error",MB_OK|MB_ICONERROR);
      edit_ctl.SetFocus();
      edit_ctl.SetSel(0,-1);
    }
  }
  return limit;
}

bool CPromDoc::get_rect(Rectangl& rect, CWnd *parent,
	const char *m_north, const char *m_south, const char *m_east, const char *m_west,
	CEdit& m_north_ctl, CEdit& m_south_ctl, CEdit& m_east_ctl, CEdit& m_west_ctl)
{
  GridPoint::Coord n_limit,s_limit,e_limit,w_limit;

  n_limit=get_limit(m_north,1,&GridPoint::sscanx,m_north_ctl,
		    GridPoint::coord1_label,GridPoint::bound1_high,parent);
  if (n_limit==coord_infty+1) return false;
  s_limit=get_limit(m_south,-1,&GridPoint::sscanx,m_south_ctl,
		    GridPoint::coord1_label,GridPoint::bound1_low,parent);
  if (s_limit==coord_infty+1) return false;
  e_limit=get_limit(m_east,1,&GridPoint::sscany,m_east_ctl,
		    GridPoint::coord2_label,GridPoint::bound2_high,parent);
  if (e_limit==coord_infty+1) return false;
  w_limit=get_limit(m_west,-1,&GridPoint::sscany,m_west_ctl,
		    GridPoint::coord2_label,GridPoint::bound2_low,parent);
  if (w_limit==coord_infty+1) return false;

  if (n_limit<s_limit) {
    sprintf(buf,"%s limits are out of order.",GridPoint::coord1_label);
    parent->MessageBox(buf,"Winprom Entry Error",MB_OK|MB_ICONERROR);
    return false;
  }

  if (e_limit<w_limit) {
    sprintf(buf,"%s limits are out of order.",GridPoint::coord1_label);
    parent->MessageBox(buf,"Winprom Entry Error",MB_OK|MB_ICONERROR);
    return false;
  }

  rect.limit[0].high=n_limit;
  rect.limit[0].low=s_limit;
  rect.limit[1].high=e_limit;
  rect.limit[1].low=w_limit;

  return true;
}

void CPromDoc::OnUpdateOpDataAlign(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) enable_submenu(pCmdUI,db_state!=NOT_LOADED);
  else pCmdUI->Enable(db_state!=NOT_LOADED && dt_state!=NOT_LOADED);
}

void CPromDoc::OnOpElevCrop()
{
  Rectangl limits=elev;
  CCrop_dlg dlg("Crop Elevation Map",limits);
  if (dlg.DoModal()!=IDOK) return;

  Elev_map temp_elev;
  temp_elev.set_bound(limits);
  temp_elev.transfer(elev);
  elev.set_bound(limits);
  elev.transfer(temp_elev);
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  em_state=CHANGED;
}

void CPromDoc::OnOpDLPRcrop()
{
  Rectangl limits=domain;
  CCrop_dlg dlg("Crop Domain Map",limits);
  if (dlg.DoModal()!=IDOK) return;

  Index_map temp_domap;
  temp_domap.set_bound(limits);
  temp_domap.transfer(domain);
  domain.set_bound(limits);
  domain.transfer(temp_domap);
  CalcTotalArea();
  setMarkBound();
  UpdateViews(&dummyModifyBounds);
  dm_state=CHANGED;
}

void CPromDoc::OnUpdateOpPurgeBSprom(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) {
    // Need to handle this to set the title of the purge secondary saddle submenu.
    pCmdUI->m_pMenu->ModifyMenu(pCmdUI->m_nIndex,MF_BYPOSITION|MF_STRING,0,
      is_drainage?"&Prom saddles":"&Basin saddles");
  }
  else OnUpdateWriteableDLPRtree(pCmdUI);
}

void CPromDoc::set_tb_bmp() const
{
  if (!working) {
    CPromFrame *main_frame=(CPromFrame *)AfxGetMainWnd();
    CToolBar& tb=main_frame->mainToolBar;
    tb.LoadBitmap(is_drainage?IDB_GRAPH_DRAIN:IDB_GRAPHTYPE);
    tb.Invalidate();
    main_frame->dt_fileBar.SetWindowText(is_drainage?"Drainage Tree":"Divide Tree");
  }
}

bool CPromDoc::clip_grid_point(Clip& clp, const GridPoint& gp,
			       const Sorted_featr_lists& sfl)
{
  Domain::Index j;
  if (!sfl.peaks.empty()) {
    j=Divide_tree::search_dom(gp,sfl.peaks,&Domain::peak);
    if (j<sfl.peaks.size() && sfl.peaks[j]->peak.location==gp) { // does it represent a peak?
      clp.add_featr(sfl.peaks[j]->peak,FT_PEAK);
      return true;
    }
  }
  if (!sfl.sadls.empty()) {
    j=Divide_tree::search_dom(gp,sfl.sadls,(Feature Domain::*)&Domain::saddle);
    if (j<sfl.sadls.size() && sfl.sadls[j]->saddle.location==gp) { // does it represent a saddle?
      clp.add_featr(sfl.peaks[j]->saddle,FT_SADDLE);
      return true;
    }
  }
  if (!sfl.ro.empty()) {
    j=Divide_tree::search_ro(gp,sfl.ro);
    if (j<sfl.ro.size() && sfl.ro[j]->location==gp) { // does it represent a runoff?
      clp.add_featr(*sfl.ro[j],FT_RUNOFF);
      return true;
    }
  }
  if (!sfl.bs.empty()) {
    j=Divide_tree::search_bs(gp,sfl.bs);
    if (j<sfl.bs.size() && sfl.bs[j]->location==gp) { // does it represent a basin saddle?
      clp.add_featr(*sfl.bs[j],FT_BASINSADL);
      return true;
    }
  }
  return false;
}

void CPromDoc::OnListEdited()
{
  CGenericListView::init_list(this);
}

void CPromDoc::OnOpTreeShoreline()
{
#ifdef administrator
  CShoreline_dlg dlg;
  dlg.m_exterme_label=is_drainage?"Basins":"Peaks";
  dlg.m_basin_elev=shore_basin_elev;
  dlg.m_sadl_elev=shore_sadl_elev;
  dlg.m_set_edit=shore_set_edit;
  dlg.m_mark_edit=shore_mark_edit;
  dlg.m_filter=shore_filter;
  dlg.m_filter.rec_filter.align(data);
  if (dlg.DoModal()!=IDOK) return;

  shore_basin_elev=dlg.m_basin_elev;
  shore_sadl_elev=dlg.m_sadl_elev;
  shore_set_edit=dlg.m_set_edit!=FALSE;
  shore_mark_edit=dlg.m_mark_edit!=FALSE;
  shore_filter=dlg.m_filter;

  tree.shoreline_correction(elev,shore_filter,data,
			    shore_basin_elev,shore_sadl_elev,
			    shore_set_edit,shore_mark_edit);
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&dummyModifyAll);
#else
  sprintf(buf,
    "Riverbed deletion is an administrative command\n"
    "and not implemented in this shipment of Winprom.\n"
    "Contact the Custodian for the %s tree(s) of interest.",
    is_drainage?"drainage":"divide");
  AfxGetMainWnd()->MessageBox(buf,"Winprom",MB_OK|MB_ICONERROR);
#endif
}

static void import_data(const Database& data, Feature& featr, FT_index ft,
			const FeatureTypeFilter& filter,
			bool edited, Saddle::Status ss,
			Database::const_FieldID name_fld, Database::const_FieldID elev_fld)
{
  if (!featr.location || !filter.test(featr,ft,data)) return;
  Database::RecordID rec=data.get_record(featr.location);
  if (rec==REC_NONE) return;
  // NOTE(akirmse): This original code doesn't compile
  // if (name_fld) {
  if (true) {
    CString name;
    data.get_value(rec,name_fld,name);
    featr=name;
  }
  // NOTE(akirmse): This original code doesn't compile
  // if (elev_fld) {
  if (true) {
	int elev;
    data.get_value(rec,elev_fld,elev);
    featr.elev=elev;
    if (edited) featr.edited=true;
  }
  if (ft==FT_SADDLE || ft==FT_BASINSADL) {
    Saddle& s=(Saddle&)featr;
    if (ss>s.status) s.status=ss;
  }
}

void CPromDoc::OnOpTreeData()
{
  static FeatureTypeFilter import_filter;
  static Saddle::Status ps_stat=Saddle::STAT_UNKNOWN,bs_stat=Saddle::STAT_UNKNOWN;
  CDataImport_dlg dlg(data);
  dlg.m_filter=import_filter;
  dlg.m_filter.rec_filter.align(data);
  dlg.m_ps_status=ps_stat;
  dlg.m_bs_status=bs_stat;
  if (dlg.DoModal()!=IDOK || dlg.name_fld==data.begin_field() && dlg.elev_fld==data.begin_field()) return;

  import_filter=dlg.m_filter;
  ps_stat=(Saddle::Status)dlg.m_ps_status;
  bs_stat=(Saddle::Status)dlg.m_bs_status;
  Domain::Index i,n;
  n=tree.n_dom();
  for (i=0; i<n; ++i) {
    import_data(data,tree[i].peak,FT_PEAK,import_filter,
		dlg.m_edit!=FALSE,ps_stat,dlg.name_fld,dlg.elev_fld);
    import_data(data,tree[i].saddle,FT_SADDLE,import_filter,
		dlg.m_edit!=FALSE,ps_stat,dlg.name_fld,dlg.elev_fld);
  }
  n=tree.n_runoff();
  for (i=0; i<n; ++i)
    import_data(data,tree.runoff(i),FT_RUNOFF,import_filter,
		dlg.m_edit!=FALSE,ps_stat,dlg.name_fld,dlg.elev_fld);
  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i)
    import_data(data,tree.bsnsdl(i),FT_BASINSADL,import_filter,
		dlg.m_edit!=FALSE,bs_stat,dlg.name_fld,dlg.elev_fld);

  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&dummyModifyAll);
}

void CPromDoc::OnUpdateOpTreeData(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dt_state!=NOT_LOADED && db_state!=NOT_LOADED);
}

void CPromDoc::add_stat(const Feature& featr, AnalData *anal_data) const
{
  if (featr.location && featr.edited && anal_data->filter.test(featr,data)) {
    double nbr_avgs[MAX_REGRESS_PARAM];
    if (anal_data->options.get_nbr_avg(elev,featr.location,nbr_avgs)) {
      anal_data->regress.add_point(nbr_avgs,
	elev[featr.location]-featr.elev.midpt());
    }
  }
}

void CPromDoc::list_stats(AnalData *peak_data, AnalData *sadl_data) const
{
  peak_data->regress.clear(peak_data->options.get_nnbrset());
  if (&peak_data!=&sadl_data)
    sadl_data->regress.clear(sadl_data->options.get_nnbrset());
  Domain::Index i,n=tree.n_dom();
  for (i=0; i<n; ++i) {
    const Feature &p=tree[i].peak,&s=tree[i].saddle;
    add_stat(p,peak_data);
    add_stat(s,sadl_data);
  }
  n=tree.n_runoff();
  for (i=0; i<n; ++i)
    add_stat(tree.runoff(i),sadl_data);
  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i)
    add_stat(tree.bsnsdl(i),sadl_data);
}

void CPromDoc::revert_feature(Feature& f)
{
  if (f.edited || !f.location && !elev.contains(f.location)) return;
  Elevation elv=elev[f.location];
  if (elv!=elev_undef) f.elev.set(elv);
}

void CPromDoc::OnOpRevert()
{
  Domain::Index i,n=tree.n_dom();
  for (i=0; i<n; ++i) {
    revert_feature(tree[i].peak);
    revert_feature(tree[i].saddle);
  }
  n=tree.n_runoff();
  for (i=0; i<n; ++i)
    revert_feature(tree.runoff(i));
  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i)
    revert_feature(tree.bsnsdl(i));
  UpdateViews(&dummyModifyAll);
  dt_state=CHANGED;
  peak_prom_utd=sadl_prom_utd=ridge_utd=false;
}

void CPromDoc::update_peak_prom_info()
{
  zero_prob=prom_zp;
  if (peak_prom_utd) return;
  tree.update_peak_prom_info();
  peak_prom_utd=true;
  ridge_utd=false;
}

bool CPromDoc::update_sadl_prom_info()
{
  zero_prob=prom_zp;
  if (sadl_prom_utd) return true;
  tree.update_sadl_prom_info(prom_ss);
  sadl_prom_utd=true;
  return false;
}

void CPromDoc::update_ridge_info()
{
  update_peak_prom_info();
  update_sadl_prom_info();
  if (ridge_utd) return;
  tree.update_ridge_info(prom_ss);
  ridge_utd=true;
}

void CPromDoc::OnUpdateDomapPurge(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(dm_state!=NOT_LOADED || dt_state!=NOT_LOADED);
}

void CPromDoc::OnDomapPurge()
{
  CPurgeDomain_dlg dlg;
  if (dlg.DoModal()!=IDOK) return;

  update_sadl_prom_info();

  tree.purge_domap(domain,dlg.m_prom,dlg.m_offmap!=FALSE,dlg.m_rotate!=FALSE);
  UpdateViews(&dummyModifyMap);
  dm_state=CHANGED;
}

void CPromDoc::OnZeroProb()
{
  prom_zp=!prom_zp;
  sadl_prom_utd=peak_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&dummyModifyAll);
}

void CPromDoc::OnUpdateZeroProb(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(prom_zp);
}

void CPromDoc::OnSaddleStat()
{
  prom_ss=!prom_ss;
  sadl_prom_utd=peak_prom_utd=ridge_utd=false;
  UpdateAllViews(0,(LPARAM)&dummyModifyAll);
}

void CPromDoc::OnUpdateSaddleStat(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(prom_ss);
}

void CPromDoc::OnOpSadlstatProm()
{
  tree.set_sadl_stat(Saddle::STAT_PROM);
  dt_state=CHANGED;
}

void CPromDoc::OnOpSadlstatReal() 
{
  tree.set_sadl_stat(Saddle::STAT_REAL);
  dt_state=CHANGED;
}

void CPromDoc::OnOpSadlstatUnknown()
{
  if (!dt_CanClose(true)) return;
  tree.set_sadl_stat_unknown();
  dt_apply_drastic_change();
}

void CPromDoc::OnViewUpdateAuto()
{
  auto_update=!auto_update;
}

void CPromDoc::OnUpdateViewUpdateAuto(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(auto_update);
}

void CPromDoc::OnViewUpdateNow()
{
  BeginWaitCursor();
  update_ridge_info();
  update_sadl_prom_info();
  UpdateAllViews(0,(LPARAM)&dummyModifyAll);
  EndWaitCursor();
}

void CPromDoc::OnUpdateViewUpdateNow(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(!auto_update);
}

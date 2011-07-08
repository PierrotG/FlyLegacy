/*
 * WindowFlightPlan.cpp
 *
 * Part of Fly! Legacy project
 *
 * Copyright 2005 Chris Wallace
 * Copyright 2007 Jean Sabatier
 *
 * Fly! Legacy is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 * Fly! Legacy is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *   along with Fly! Legacy; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "../Include/FlyLegacy.h"
#include "../Include/Utility.h"
#include "../Include/Globals.h"
#include "../Include/Fui.h"
#include "../Include/FuiUser.h"
#include "../Include/FuiParts.h"
#include "../Include/FlightPlan.h"
//==================================================================================
//
//  FlightPlan List:  List of available flight plans
//
//==================================================================================
CFuiListPlan::CFuiListPlan(Tag idn, const char* filename)
:CFuiWindow(idn,filename,0,0,0)
{ char *erm = "Incorrect FlightPlanList.WIN file";
  fPlan = globals->fpl;
  frame = (U_INT)(-1);
  //-----------Init List of plans ---------------------
  U_INT typ1 = LIST_HAS_TITLE + LIST_NOHSCROLL;
  allBOX.SetParameters(this,'allp',typ1);
  FillPlans();
  //-----------Init list of charts --------------------
  U_INT typ3 = LIST_NOHSCROLL;
  mapBOX.SetParameters(this,'chrt',typ3);
  FillChartList();
  mapBOX.SortAndDisplay();
  //-----Locate current plan --------------------------
  char *key   = fPlan->GetFileName();
  allBOX.GoToKey(key);
  //-----List of Charts ------------------------------
}
//--------------------------------------------------------------------------
//  Destroy the nav log
//--------------------------------------------------------------------------
CFuiListPlan::~CFuiListPlan()
{ 
}
//-------------------------------------------------------------------------
//  Edit the titlefor list of plans
//-------------------------------------------------------------------------
void CFuiListPlan::TitlePlan()
{ CFpnLine *slot = new CFpnLine;
  slot->SetName("Description");
  slot->SetFile("File name.......................");
  allBOX.AddSlot(slot);
  return;
}
//-------------------------------------------------------------------------
//  Fill the list of plans
//-------------------------------------------------------------------------
void CFuiListPlan::FillPlans()
{ char nfile[MAX_PATH];
  CFlightPlan fpl(1);
  CFpnLine *slot = 0;
  char     *ds   = 0;     
  char pn[MAX_PATH];
  allBOX.EmptyIt();
  TitlePlan();
  sprintf(pn,"*.FPL");
  char *fn = (char*)pfindfirst (&globals->pfs,pn);
  while (fn)
  { strncpy(nfile,fn,MAX_PATH);
    if (0 == slot)  slot = new CFpnLine;
    char *deb = strrchr(nfile,'/');
    char *end = strrchr(nfile,'.');
    if (deb)
    { deb++;
      slot->SetFile(deb);
      if (end) *end = 0;
      fpl.Open(deb);
      ds = fpl.GetDescription();
      slot->SetName(ds);
      allBOX.AddSlot(slot);
      slot = 0;
    }
    fn = (char*)pfindnext (&globals->pfs);
  }
  allBOX.Display();
  return;
}
//-------------------------------------------------------------------------
//  Select plan from Directory
//-------------------------------------------------------------------------
void CFuiListPlan::SelectPlan()
{ char fn[MAX_PATH];
  CFpnLine *lin = (CFpnLine *)allBOX.GetSelectedSlot();
  strncpy(fn,lin->GetFile(),MAX_PATH);
  char *end = strrchr(fn,'.');
  if (end) *end = 0;
  fPlan->Open(fn);
  //---Set Initial state ------------------------------
  fPlan->InitialState();
  //---Open or refresh detail -------------------------
  CFuiFlightLog *win = globals->dbc->GetLOGwindow();
  if (win)  return win->Reset();
  //---Open the detail window -------------------------
  globals->fui->CreateFuiWindow (FUI_WINDOW_FPLAN_LOG);
  return;
}
//-------------------------------------------------------------------------
//  Get All charts
//-------------------------------------------------------------------------
void CFuiListPlan::FillChartList()
{ FillOneList("JPG");
  FillOneList("PNG");
  return;
}
//-------------------------------------------------------------------------
//  Get All charts
//-------------------------------------------------------------------------
void CFuiListPlan::FillOneList(char *ext)
{ char    fn[MAX_PATH];
  sprintf(fn,"CHARTS/*.%s",ext);
  //--- Get all JPG files in the list --------------
  char  *name = (char*)pfindfirst (&globals->pfs,fn);
  while (name)
  { char  *deb  = strrchr(name,'/');
    if (deb)  AddChart(deb+1);
    name = (char*)pfindnext(&globals->pfs);
  }
  //--- Get All PNG files in the list --------------
  return;
}
//-------------------------------------------------------------------------
//  Add a line for this chart
//-------------------------------------------------------------------------
void CFuiListPlan::AddChart(char *map)
{ CMapLine *lin = new CMapLine();
  lin->SetName(map);
  mapBOX.AddSlot(lin);
  return;
}
//-------------------------------------------------------------------------
//  Select a chart
//-------------------------------------------------------------------------
void CFuiListPlan::SelectChart()
{ CMapLine *lin = (CMapLine*)mapBOX.GetSelectedSlot();
  if (0 == lin)     return;
  strncpy(globals->MapName,lin->GetName(),SLOT_NAME_DIM);
  globals->fui->CreateFuiWindow(FUI_WINDOW_CHART,0);
  return;
}
//-------------------------------------------------------------------------
//  Draw the navigation log
//-------------------------------------------------------------------------
void CFuiListPlan::Draw()
{ CFuiWindow::Draw();
  return;
}


//-------------------------------------------------------------------------
//  Close this window
//-------------------------------------------------------------------------
void CFuiListPlan::CloseMe()
{ Close();
  return;
}
//-------------------------------------------------------------------------
//  Event notification
//-------------------------------------------------------------------------
void  CFuiListPlan::NotifyChildEvent(Tag idm,Tag itm,EFuiEvents evn)
{ switch (idm)  {
  case 'sysb':
    if (EVENT_CLOSEWINDOW == evn) CloseMe();
    else  SystemHandler(evn);
    return;
  //---Scroll event on list of plans ---------
  case 'allp':
    allBOX.VScrollHandler((U_INT)itm,evn);
    return;
  //---Scroll event on list of maps ----------
  case 'chrt':
    mapBOX.VScrollHandler((U_INT)itm,evn);
    return;
  //--- Select a new plan from directory -----
  case  'sfpl':
    SelectPlan();
    return;
  //--- Select a chart -----------------------
  case 'smap':
    SelectChart();
    return;
}

  return;
}

//=======================END OF FILE ==================================================
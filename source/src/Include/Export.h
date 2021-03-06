/*
 * TAXIWAY.h
 *
 * Part of Fly! Legacy project
 *
 * Copyright 2003-2005 Chris Wallace
 * Copyright 2008      Jean Sabatier
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
#ifndef EXPORT_H
#define EXPORT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include/FlyLegacy.h"
//============================================================================
class C_QTR;
class CAptObject;
class CAirport;
class CPaveQ;
class CPolyShop;
class C_TRN;
//============================================================================
#define EXP_MSG1 0
#define EXP_INIT 1
#define EXP_FBIN 2
#define EXP_NBIN 3
#define EXP_WBIN 4
#define EXP_FSMF 5
#define EXP_NSMF 6
#define EXP_WSMF 7
#define EXP_END  8
#define EXP_OUT  9
#define EXP_RLAX 10
//---------------------------------------------------------------
#define EXP_MNOT 0
#define EXP_MW3D 1
//============================================================================
//  Class to export DATA for SQL Database
//============================================================================
class CExport {
  typedef void(CExport::*partFN)(M3D_PART_INFO &pif);  // Part function
  //-----ATTRIBUTES ---------------------------------------------
  char Mode;
  TEXT_INFO  inf;                         // Texture info
  M3D_PART_INFO pif;                      // Part info
  //-------------------------------------------------------------
  char gen;                               // Export indicator
  char elv;                               // Export ELV
  char sea;                               // Export SEA
  char txy;                               // Export taxiway
  char m3d;                               // Export model3D
  char gtx;                               // Generic texture
  char wob;                               // World Objects
  char Sep[8];                            // Separator
	char edt[PATH_MAX];
  //----3D Model control -----------------------------------------
  char *mName;                            // Model Name
  char  mStat;                            // Export state
  char  rStat;                            // State to restart
  char  mRed;                             // Reduction ON
  U_INT mCnt;                             // Model count
  U_INT face;                             // Face counter
  U_INT minp;                             // Minimum to keep
  CPolyShop *Polys;                       // PolyShop 
  C3Dmodel  *Mod;                         // Model
  //----Generic parameters -----------------------------------------
	U_INT	count;
	U_INT qKey;															// QGT key
  U_INT bx;                               // Base QGT X
  U_INT bz;                               // Base QGT Z
  U_INT qx;                               // QGT X
  U_INT qz;                               // QGT Z
	U_INT	gx;																// Global Tile X
	U_INT	gz;																// Gloabl tile Z
  U_INT ax;                               // Tile X
  U_INT az;                               // Tile Z
  //----SEA Values -----------------------------------------------
  int   row;
  int   col;
  int   qdr;
  int   total;
  COAST_REC rec;
  //---Taxiway values --------------------------------------------
  CAptObject *apo;                        // Airport Object
  CAirport   *apt;                        // Airport record
  //--------------------------------------------------------------
  U_INT noRec;                            // Record number
  C_QTR *qtr;                             // QTR file
  int    qNo;                             // QTR index
	C_TRN	*trn;															// TRN file
  //---Resources to free after usage ----------------------------
  CCamera    *Cam;                        // Camera
  //-------------------------------------------------------------
public:
  CExport();
 ~CExport();
  bool  NoDatabase(CDatabase *db);
  void  ExportGenericAsCVS();
  //-------------------------------------------------------------
  void  ListAirport(U_INT nt,ClQueue &qhd);
  void  ListNavaid (U_INT nt,ClQueue &qhd);
  void  ListILS    (U_INT nt,ClQueue &qhd);
  void  ListCOM    (U_INT nt,ClQueue &qhd);
  void  ListWPT    (U_INT nt,ClQueue &qhd);
  //----EXPORT ROUTINES     ------------------------------------
  void  ExportAirportsAsCVS();
  void  ExportRunwaysAsCVS();
  void  ExportNavaidsAsCVS();
  void  ExportILSAsCVS();
  void  ExportComAsCVS();
  void  ExportWptAsCVS();
  void  ExportCtyAsCVS();
  void  ExportStaAsCVS();
  //-----GENERIC NAVIGATION OBJECTS----------------------------
  void  ExportAPT(U_INT gx,U_INT gz,char *sep,SStream &st);
  void  ExportNAV(U_INT gx,U_INT gz,char *sep,SStream &st);
  void  ExportILS(U_INT gx,U_INT gz,char *sep,SStream &st);
  void  ExportCOM(U_INT gx,U_INT gz,char *sep,SStream &st);
  void  ExportWPT(U_INT gx,U_INT gz,char *sep,SStream &st);
  void  ExportCTY(char *sep,SStream &st);
  void  ExportSTA(char *sep,SStream &st);
  //----COAST DATA---------------------------------------------
  void  ExportCoastInDB();
  void  ExportCoastFile(char *name,int gx,int gz);
  COAST_VERTEX *CoastCount(COAST_VERTEX *pol);
  bool  NextCoastKey();
  //----3D MODELS ---------------------------------------------
  void  InitModelPosition();
  void  DrawModel();
  void  ExportBIN();
  void  ExportSMF();
  void  Export3Dmodels();
  bool  Prepare3Dmodel(char *fn,Tag type);
  void  WriteTheModel();
  void  OneM3DPart(M3D_PART_INFO &pif);
  void  Export3DMlodQ(char * name, int n);
  void  M3DMsgIntro();
  void  ExportAllSceneries();
  void  CloseSceneries();
  int   ExecuteW3D();
  void  KeyW3D(U_INT key,U_INT mod);
	//--- Update 3D models -------------------------------------
	void	Update3DModels();
	void	LoadUpdModel(char *name);
	void	UpdateTheModel(char *name);
  //-----TAXIWAYS---------------------------------------------
  void  ExportTaxiways();
  void  ExportOneTMS(CAirport *apt);
  CPaveRWY  *BuildPave(CPaveQ *qhd);
  CBaseLITE *BuildLite(CLiteQ *qhd,U_CHAR col);
  void  ExportPaveQ(char *key);
  void  ExportEdgeQ(char *key);
  void  ExportCentQ(char *key);
  void  ExportLiteQ(char *key,U_CHAR col);
  //------GENERIC TEXTURES -----------------------------------
  void  ExportGenTextures();
  void  DecodeTexture(char *name);
  void  WriteMskTexture(char *name,int dim);
  void  WriteRwyTexture(char *name,char seg,char tsp);
  void  WritePNGTexture(char *name,char tsp);
	//------COMPRESSED TEXTURES ---------------------------------
	void  CompressTextures();
	void	ScanDirectory(int x, int z);
  //-----------------------------------------------------------
  int   TimeSlice(float dT);
  void  Keyboard(U_INT key,U_INT mod);
  //----3D OBJECTS --------------------------------------------
  void  ExportWOBJ();
  void  WriteQgtOBJ();
	//--- SCENERY FILES (TRN) -----------------------------------
	void	ExportTRNSceneries();
	void	ExportTRN(char *fn);
	void	ExportSUP(C_STile *asp);
};
//============================END OF FILE =================================================
#endif  // EXPORT_H

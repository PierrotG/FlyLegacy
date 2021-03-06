/*
 * TerrainTexture.h
 *
 * Part of Fly! Legacy project
 *
 * Copyright  2003 Chris Wallace
 *            2008 Jean Sabatier
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
#ifndef TERRAINTEXTURE_H
#define TERRAINTEXTURE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include/Globals.h"
#include "../Include/Utility.h"
#include "../Include/FreeImage.h"
#include "../Include/TerrainCache.h"
#include "../Include/LightSystem.h"
//-----------------------------------------------------------------------------
#define TC_ACTSIZE 0x300
//=============================================================================
class CTarmac;
class CTextureWard;
extern U_CHAR    MaxiRES[];
//=============================================================================
//  Memory texture count
//=============================================================================
class CMemCount {
public:
  //-----------------------------------------
  int Dcnt[2];            // std Day count
  int Ncnt[2];            // std nigth count
  int dxcnt;              // Day object count
  int nxcnt;              // Nit object count
  //-------------------------------------------
  CMemCount();
};
//=============================================================================
//  Contour state:
//  nState gives the next contour state (0, RED, BLUE or RED and BLUE)
//  swap   0=> No swap  1 = > swap
//=============================================================================
struct TC_PIX_STATE 
{ U_CHAR  nState;                               // Next state
  U_CHAR  swap;                                 // Inside swap value
  U_CHAR  In;                                   // Inside value
};
//===================================================================================
//  Class to animate water
//===================================================================================
class CWater3D {
protected:
  //---ATTRIBUTES -------------------------------------------------
  U_INT       w3d;                            // 3D Texture Object
  U_INT       sob;                            // sea texture object
  float       org;                            // Texture origin
  float       exp;                            // Expansion factor
  //---------------------------------------------------------------
  char        once;                           // For debbug
  //---Sea coordinates --------------------------------------------
  int         dim;                            // Texture dimension
  float       cds;                            // S coordinate   
  float       cdt;                            // t coordinate
  float       vit;                            // moving speed
  F3_VERTEX s3d[4];                           // Moving texture
  //---Panel ------------------------------------------------
  TC_VTAB     qad[4];                         // Quad definition
  //---Camera -----------------------------------------------
  CCameraObject   *cam;
  //---------------------------------------------------------
public:
  CWater3D(int k);
 ~CWater3D();
  //---------------------------------------------------------
  void    LoadWater3D();
  void    Append(U_CHAR *buf,U_CHAR *tex,int dim,int k);
  GLuint  Get3DtexOBJ();
  U_CHAR *PickAlphaChanel(U_CHAR *rgba, int side);
  //---------------------------------------------------------
  void    InitQuad();
  void    BuildAnimation();
  void    DrawTile(CTextureDef *txn);
  void    DrawTest(CTextureDef *txn);
  //---------------------------------------------------------
  U_INT   MoveWater();
  //----------------------------------------------------------------
  inline U_INT GetWaterOBJ()  {return cam->TextureObject();}
  inline U_INT GetW3D()       {return w3d;}
};
//=============================================================================
//  Class CWaterTexture : to store fixed water file textures
//  NOTE:  The RGBA texture is managed by the shared texture object. 
//         Here this is just a pointer. So dont delete the texture here.
//=============================================================================
class CWaterTexture {
public:
  char     Name[TC_TEXNAMESIZE];        // Texture name
  U_INT   *rgba;                        // rgba texture
  int      res;                         // Resolution
  U_INT    xOBJ;                        // Texture object
  //---------Methodes ----------------------------------------
  CWaterTexture();
  //----------------------------------------------------------
 ~CWaterTexture();
 //-----------------------------------------------------------
  void    LoadTexture(TEXT_INFO &txd, char res,U_INT af);
  U_INT  *GetWaterCopy();
 //-----------------------------------------------------------
  inline   U_INT *GetRGBA()   {return rgba;} 
  inline   void   AssignOBJ(U_INT o) {xOBJ = o;}
  inline  GLuint  GetXOBJ()   {return xOBJ;}
};
//=============================================================================
//  Class CBlendTexture:  To store the transition mask
//=============================================================================
class CBlendTexture {
public:
  int      side;                         // Resolution
  GLubyte  *msk;                         // Texture
  //--------Methods ------------------------------------------
  CBlendTexture();
 ~CBlendTexture();
 //-----------------------------------------------------------
  inline GLubyte *GetMask()           {return msk;}
};
//=============================================================================
//  Class CSharedTxnTex:  Shared texture
//=============================================================================
class CSharedTxnTex: public CTextureDef {
public:
  U_INT   Use;                          // User count
  char  Blend;                          // Request blending alpha
  //---------Method ------------------------------------------
  CSharedTxnTex(char *name,U_CHAR res);
 ~CSharedTxnTex();
  //-----------------------------------------------------------------------------
  inline bool AssignOBJ(CTextureDef *txn) {txn->dOBJ = dOBJ; return (dOBJ != 0);}
  //-----------------------------------------------------------------------------
  inline void SetBlend(char b)    {Blend = b;}
  inline char GetBlend()          {return Blend;}
};
//=============================================================================
//  Class CShared3DTex for 3D texture sharing
//  NOTE:  The texture name include path ART/ so the real texture
//          ident start at path+4
//=============================================================================
class CShared3DTex {
  //-----Attribute -----------------------------------------
  U_INT         Use;                          // User count
  TEXT_INFO     x3d;                          // Texture info
  //--------------------------------------------------------
public:
  CShared3DTex(char *tn,char tsp);
 ~CShared3DTex();
 //----------------------------------------------------------
 inline TEXT_INFO     *GetInfo()                      {return &x3d;}
 inline char          *GetIdent()                     {return  (x3d.path + 4);}
 inline void           IncUser()                      {Use++;}
 inline bool           DecUser()                      {Use--; return (Use != 0);}
 inline U_INT          GetOBJ()                       {return x3d.xOBJ;}
 inline void           SetXOBJ(GLuint r)              {x3d.xOBJ = r;}
};
//=============================================================================
//  Class CArtParser to read texture files with several options
//
//   
//=============================================================================
class CArtParser {
  friend class CTextureWard;
  //-----------------------------------------------------------
  FREE_IMAGE_FORMAT    ffm;                     // File format
  int                  fop;                     // File option
  FIBITMAP            *ref;                     // File reference
  FIMEMORY            *mem;                     // Memory reference
  FREE_IMAGE_TYPE      type;                    // Image type
  U_INT     bpp;                                // Bit per plan
  U_INT     htr;                                // Heigth
  U_INT     wid;                                // Width
  U_INT     dim;                                // Full size
  U_INT     side;                               // Side in pixel
  char      *dot;                               // Dot place into name
  U_INT     *rgb;                               // RGBA data                         
  //-------Color for coast mask -------------------------------
  char      epd;																// EPD texture
  char      res;                                // Resolution
  char      abt;                                // abort option
	char      rrv;																// reverse row
  //-------Alpha channel --------------------------------------
  U_CHAR    afa;                                // Alpha channel
  U_CHAR    lay;                                // Sea layer
  //-------Main tile data -------------------------------------
  GLubyte   *act;                               // ACT bytes
  GLubyte   *raw;                               // Raw bytes
  GLubyte   *opa;                               // OPA bytes
  GLubyte   *wtx;                               // Water RGBA
  //-------Transition file 1 -(used for right transition)--------
  char       Tp1;                               // Terrain type
  GLubyte   *Tx1;                               // Texture 1
  GLubyte   *Ms1;                               // Mask 1
  //-------Transition file 2 ------------------------------------
  char       Tp2;                               // Terrain type
  GLubyte   *Tx2;                               // Texture 2
  GLubyte   *Ms2;
  //-------Transition file 3 ------------------------------------
  char       Tp3;                               // Terrain Type
  GLubyte   *Tx3;                               // Texture 3
  GLubyte   *Ms3;
  //------------Methods -------------------------------------------------
public:
  CArtParser(char res);
  CArtParser()  {}
 ~CArtParser();
  void      Abort(char *fn,char *er);
  int       SetSide(int s);
  bool      NoFile(char *fn);
  bool      LoadPOD(char *rnm,char tsp);            // Load from POD
  bool      LoadFIM(char *rnm,char tsp,FREE_IMAGE_FORMAT ff);
	GLubyte  *LoadRaw(TEXT_INFO &txd,char opt);       // Load texture
  //---------------------------------------------------------------------
  bool      LoadFFF(char *rnm,char tsp,FREE_IMAGE_FORMAT ff);
  //---------------------------------------------------------------------
  int       ConvertRGBA(U_CHAR alf);                // Convert in RGBA
  int       ByteRGBA(U_CHAR opt);
  int       IndxRGBA(U_CHAR opt);
  int       PixlRGBA(U_CHAR opt);
  int       PixlBGRO(U_CHAR opt);
  int       ByteTIFF(U_CHAR opt);
  //---------------------------------------------------------------------
  void      WriteBitmap(FREE_IMAGE_FORMAT ff,char *fn,int wd,int ht,U_CHAR *buf);
  //---------------------------------------------------------------------
  GLubyte  *TransferRGB();
  //----Normal textures -------------------------------------------------
  GLubyte  *ModlMerger(U_CHAR alf);
  GLubyte  *RGBAMerger(U_CHAR alf);
  GLubyte  *RGBAInvert(U_CHAR alf);
  //---------------------------------------------------------------------
  int       GetSize(int tot,char *fn);
  int       GetFileType(char *name);
  //---------------------------------------------------------------------
  inline U_INT GetDim()                     {return dim;}
  inline U_INT GetSide()                    {return side;}
  inline U_INT GetHeigth()                  {return htr;}
  inline U_INT GetWidth()                   {return wid;}
  inline void  SetWaterRGBA(U_INT *wt)      {wtx = (GLubyte*)wt;}
  inline U_INT GetBPP()                     {return bpp;}
  inline void  DontAbort()                  {abt = 0;}
  inline void  SetFormat(FREE_IMAGE_FORMAT f) {ffm = f;}
  inline void  SetOption(int p)               {fop = p;}
  inline void  SetEPD()                     {epd   = 1;}
	inline void	 NoReverse()									{rrv   = 0;}
  //--- Transition textures ---------------------------------------------
  inline void  SetTrans1(GLubyte *t)        {Tx1 = t;}
  inline void  SetTrans2(GLubyte *t)        {Tx2 = t;}
  inline void  SetTrans3(GLubyte *t)        {Tx3 = t;}
  //---------------------------------------------------------------------
  void         InitTransitionT1(TEXT_INFO &txd);
  void         InitTransitionT2(TEXT_INFO &txd);
  void         InitTransitionT3(TEXT_INFO &txd);
  //--- FULL Textures functions -----------------------------------------
  GLubyte     *MergeWater(GLubyte *tex);
  GLubyte     *MergeNight(GLubyte *tex);
  //--------------------------------------------------------------------
  GLubyte     *Mixer(TEXT_INFO &txd);
  //-----VECTORED ROUTINES FOR TEXTURES ------------------------------------
  GLubyte     *GetRawTexture(TEXT_INFO &txd,char opt);
  GLubyte     *LoadTextureMT(TEXT_INFO &txd);
  GLubyte     *LoadTextureFT(TEXT_INFO &txd);
  GLubyte     *GetDayTexture(TEXT_INFO &txd,char opa);  // VF 1
  GLubyte     *GetNitTexture(TEXT_INFO &txd);           // VF 2
  GLubyte     *TransitionTexture(TEXT_INFO &txd);
  //-------------------------------------------------------------------
  GLubyte     *GetModTexture(TEXT_INFO &txd);
  GLubyte     *GetAnyTexture(TEXT_INFO &txd);
  void         FreeAnyTexture(GLubyte *text);
  void         FreeFFF(FREE_IMAGE_FORMAT ff);
};

//=============================================================================
//  Class CTextureWard
//  This class stores all current textures in cache
//
//=============================================================================
class CTextureWard  {
  //-------Data Member ----------------------------------------
  U_INT   tr;                         // Trace indicator
  TCacheMGR *tcm;                     // Cache manager
  SqlMGR    *sqm;                     // Main sql manager
  bool       usq;                     // Use sql for textures
  //-----Picture number ---------------------------------------
  int     nPic;                       // Picture number for output
  //-------Working --------------------------------------------
  C_SEA  *sea;                        // Associated sea file
  C_QGT  *qgt;                        // Working Quarter Global Tile
  U_INT   gx;                         // Global Tile X composite
  U_INT   gz;                         // Global Tile Z composite
  U_INT   qz;                         // QGT Z composite
  U_INT   tx;                         // Base Tile X index
  U_INT   tz;                         // Base Tile Z index
  //-------Texture allocation ---------------------------------
  TEXT_INFO  xmd;                     // Texture loading 3D models
  TEXT_INFO  xsp;                     // Texture loading specific
  TEXT_INFO  xld;                     // Texture loading terrain
  TEXT_INFO  xds;                     // Texture loading others
  GLubyte *dTEX;                      // Day texture
  GLubyte *nTEX;                      // Night texture
  U_CHAR   Resn;                      // Requested Resolution
  //-------Options --------------------------------------------
  U_CHAR NT;                          // Nigh textures enable
  //-------State management -----------------------------------
  U_CHAR    Inside;                   // Inside Flag
  U_CHAR    State;                    // Current color state
  //-------Terrain Texture cache ------------------------------
  pthread_mutex_t	txnMux;                   // State lock
  std::map<U_INT,CSharedTxnTex*>  txnMAP;   // Texture map
  //-------3D Object cache -----------------------------------
  pthread_mutex_t	t3dMux;                   // State lock
  std::map<std::string,CShared3DTex*> t3dMAP;   // Texture map
  //-------Generic files -------------------------------------
  U_CHAR  Night;                      // Night indicator
  CWaterTexture Tank[4];              // Water bitmaps
  CBlendTexture Blend[12];            // Blending masks
  //-------Runway texture Object ------------------------------
  U_INT     RwyTX[4];                 // 4 runway textures  
  //-------Accounting -----------------------------------------
  pthread_mutex_t	ctrMux;             // State lock
  CMemCount NbTXM;                    // Texture in memory
  int       NbSHD;                    // Number of shared textures
  int       NbCUT;                    // Cut number
  int       Nb3DT;                    // 3D textures
  //-------Animated water ---------------------------------------
  U_INT     kaf;
  CWater3D *anSEA;                    // Animated sea texture
  //-------Light textures ---------------------------------------
  GLuint  LiOBJ[8];                   // Ligth Texture objects
  //-------CANVAS for drawing coast polygons --------------------
  int     Dim;                        // Dimension for resolution
  U_CHAR  alfa[2];
  U_CHAR  Color;                      // Line color
  U_CHAR  Cnv1[TC_TEXTMAXDIM];        // Canvas 1
  U_CHAR *Canvas;                     // Current canvas
  U_INT  *Water;                      // Water Texture
  TC_BOUND pBox;                      // Bounding box for polygons
  TC_BOUND iBox;                      // Initial values
  //-------Methods --------------------------------------------
public:
  CTextureWard(TCacheMGR *mgr,U_INT tr);                      // Constructor
 ~CTextureWard();                                             // Destructor
  int     Abort(char *msg,char *mse);
  int     Warn (char *msg,char *mse);
  void    TraceCTX();
  void    GetTextParam(char res,float *dto,float *dtu);
  //----Shared Key building ----------------------------------
  U_INT   KeyForTerrain(CTextureDef *txn,U_CHAR res);
  U_INT   KeyForWater(U_CHAR res);
  U_INT   KeyForRunway(U_CHAR grnd,U_CHAR seg);
  U_INT   KeyForTaxiway(U_CHAR res);
  GLuint  GetLiteTexture(U_CHAR No);
  //-----Inlines ---------------------------------------------
  inline  void    SetResolution(U_CHAR r) {Resn = r;}
  inline  U_CHAR  GetMaxRes(U_CHAR t)   {return MaxiRES[t];}
  inline  void    SetNight(U_CHAR nit)  {Night = nit;}
  inline  bool    IsNight()             {return (Night == 'N');}
  inline  bool    IsDay()               {return (Night == 'D');}
  inline  GLubyte *GetMask(U_INT m,U_INT r) {return Blend[(m | r)].GetMask();}
  //-----Animated water --------------------------------------
  inline  void    DrawWater(CTextureDef* d) {anSEA->DrawTile(d);}
  inline  U_INT   MoveWater()               {return anSEA->MoveWater();}
  inline  U_INT   GetWaterAlpha()           {return kaf;}
  //-----Helpers ---------------------------------------------
  GLubyte *LoadMSK(char *msn,int side); 
  float   GetLuminance(float *col);
  int     GetMixTexture(CTextureDef *txn,U_CHAR opt);
  int     GetShdTexture(CTextureDef *txn,char *name);
  int     GetGenTexture(CTextureDef *txn);
  int     GetRawTexture(CTextureDef *txn);
  int     GetEPDTexture(CTextureDef *txn);
  int     GetSeaTexture(CTextureDef *txn);
  //----------------------------------------------------------
  void    GetTileName (char *base);
  void    BuildName(char *gen,char *root,char res);
  void    GetMediumTexture(CTextureDef *txn);
  //----------------------------------------------------------
  void    GetShdOBJ(CTextureDef *txn);
  GLuint  GetTaxiTexture();
  void   *GetM3DPodTexture(char *fn,U_CHAR tsp);
  void   *GetM3DSqlTexture(char *fn,U_CHAR tsp);
  void    Get3DTIF(TEXT_INFO *inf);
  void    Get3DRAW(TEXT_INFO *inf);
  void   *RefTo3DTexture(char *fn);
  GLuint  Get3DObject(void *tref);
  void    Free3DTexture(void *sht);
  //------Night textures -------------------------------------
  int     NightGenTexture(CTextureDef *txd);
  int     NightRawTexture(CTextureDef *txn);
  int     DoubleNiTexture(CTextureDef *txn,U_INT *tex);
  //-----Texture Handling ------------------------------------
  int     FreeShared(CTextureDef *txn);
  int     FreeWater (CTextureDef *txn);
  int     FreeAllTextures(CSuperTile *sp);
  int     LoadTextures(U_CHAR lev,U_CHAR res,C_QGT *qgt,CSuperTile *sp);
  int     SwapTextures(CSuperTile *sp);
  void    LoadMaskTexture(int No,char *name,int dim);
  void    LoadTaxiTexture(char *name,char tsp);
  void    LoadRwyTexture(U_INT key,char *fn, char tsp);
  void    LoadLightTexture(U_CHAR No);
  //-----Helpers ----------------------------------------------
  void    WriteTexture(int wd,int ht,U_INT obj);
  void    WriteScreen();
  //-----------------------------------------------------------
	void		GetRwyTexture(CTarmac *tmac,U_CHAR gt);
  CSharedTxnTex *GetSharedTex(U_INT key);
  int     FreeSharedSlot(CTextureDef *txn);
  int     FreeWaterSlot(CTextureDef *txn);
  void    FreeSharedKey(U_INT key);
  //----------------------------------------------------------
  GLuint  GetGaugeOBJ(TEXT_DEFN &txd);
  GLuint  GetMskOBJ(TEXT_INFO &inf,U_INT mip);
  GLuint  GetTexOBJ(TEXT_INFO &inf, U_INT mip,U_INT type);
  GLuint  GetM3dOBJ(TEXT_INFO *inf);
  GLuint  GetTexOBJ(GLuint obj,int x,int y,GLubyte *tex,U_INT type);
  GLuint  GetRepeatOBJ(GLuint obj,U_CHAR res,GLubyte *tex);
  GLuint  GetTerraOBJ(GLuint obj,U_CHAR res,GLubyte *tex);
  GLuint  GetLitOBJ(GLuint obj,U_INT dim, GLubyte *tex);
  GLuint  GetWatOBJ(CTextureDef *txn);
  void    GetSupOBJ(CSuperTile *sp);
  //----------------------------------------------------------
  GLuint  GetRepeatOBJ(TEXT_INFO &xds);
  GLuint  GetLitOBJ(TEXT_INFO    &xds);
  //----------------------------------------------------------
  U_INT  *GetWaterRGBA(U_CHAR res)    {return Tank[res].rgba;}
  U_INT  *GetWaterCopy(U_CHAR res)    {return Tank[res].GetWaterCopy();}
  bool    LoadImagePNG(char *fn, S_IMAGE &ref);
  bool    LoadImageJPG(char *fn, S_IMAGE &ref);
  GLuint  LoadIconPNG(char *fn);
  void    LoadAnyTexture(char *fn,TEXT_DEFN &txd);
  //------------COAST ROUTINES       ----------------------
  void    DrawTLine (int x1,int y1,int x2, int y2);
  void    DrawVLine(int x0,int y1,int y2);
  void    DrawHLine(int x1,int x2,int y0);
  U_INT   AdjustCoordinate(U_INT val);
  COAST_VERTEX *DrawStencil(COAST_VERTEX *pol);
  void    ScanCoast();
  void    MakeStencil(char *lsp);
  void    AlphaCoastTexture(U_CHAR clr,GLubyte *land);
  void    BuildNightTexture(U_INT *tx);
  int     BuildCoastTexture(GLubyte *land);
  //--------------------------------------------------------
  //  Counters
  //--------------------------------------------------------
  void    ModDTX(char k);
  void    ModNTX(char k);
  void    IncDAY(char k);
  void    DecDAY(char k);
  void    IncNIT(char k);
  void    DecNIT(char k);
  void    PopDAY();
  void    PopNIT();
  //------Statistical data ---------------------------------
  void    GetStats(CFuiCanva *cnv);
};
//===============END OF THIS FILE =============================================
#endif  // TERRAINTEXTURE_H

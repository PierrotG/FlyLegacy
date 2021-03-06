/*
 * FileParser.h
 *
 * Part of Fly! Legacy project
 *
 * Copyright 2003 Chris Wallace
 * CopyRight 2008 Jean Sabatier
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
#include "../Include/Model3d.h"
//======================================================================================
class CAirportMgr;
class C3Dmodel;
class C3DPart;
class GroundSpot;
struct METAR_INFO;
//======================================================================================
#define SIZE_INT1   (sizeof(int))
#define SIZE_INT2   (SIZE_INT1 + sizeof(int))
#define SIZE_INT3   (SIZE_INT2 + sizeof(int))
#define SIZE_INT4   (SIZE_INT3 + sizeof(int))
#define SIZE_INT5   (SIZE_INT4 + sizeof(int))
//=============================================================================
//  SMF decoder to parse a SMF file
//  Parts are classified as
//  model 0:  Day   - Big size
//  model 1:  Night - Big size
//-----------------------------------------------------------
//  model 2:  Day   - Tin size
//  model 3:  Night - Tin  size
//-----------------------------------------------------------
//  Only part of hi resolution are kept
//=============================================================================
//=============================================================================
//  BIN STRUCTURES
//=============================================================================
struct BIN_HEADER {
  int scale;
  int unkn1;
  int unkn2;
  int nvert;                    // vertices numbers
};
struct B19_HEADER {
  int nvert;                    // Vertice count
  int nmx;                      // Normal x
  int nmy;                      // Normal y
  int nmz;                      // Normal z
  int magic;      
};

//========================================================================
//  TIF STRUCTURES
//========================================================================
typedef struct {
  unsigned short    endian;     // 'II' (Little Endian) or 'MM' (Big-Endian)
  unsigned short    signature;  // 0x0042 in appropriate endian-ness
  long              ifd;        // Image File Directory offset
} TIF_HDR;
//----Directory entry ----------------------------------------------------
typedef struct {
  unsigned short    tag;
  unsigned short    type;
  unsigned long   count;
  unsigned long   offset;
} TIF_DIR;
//------STRIP PARAMETERS -------------------------------------------------
typedef struct {
  unsigned long    ofs;                 // Strip offset
  unsigned long    cnt;                 // strip byte count
}TIF_STRIP;
//------COLOR MAP --------------------------------------------------------
typedef struct {
    GLubyte r, g, b;
  } TIF_RGB;
//------TAG VALUES -------------------------------------------------------
#define TIF_TAG_IMAGEWIDTH          (0x0100)
#define TIF_TAG_IMAGELENGTH         (0x0101)
#define TIF_TAG_BITSPERSAMPLE       (0x0102)
#define TIF_TAG_COMPRESSION         (0x0103)
#define TIF_TAG_INTERPRETATION      (0x0106)
#define TIF_TAG_STRIPOFFSETS        (0x0111)
#define TIF_TAG_SAMPLESPERPIXEL     (0x0115)
#define TIF_TAG_ROWSPERSTRIP        (0x0116)
#define TIF_TAG_STRIPBYTECOUNTS     (0x0117)
#define TIF_TAG_PLANARCONFIGURATION (0x011C)
#define TIF_TAG_COLORMAP            (0x0140)
#define TIF_TAG_NEWSUBFILETYPE      (0x00FE)
//------------------------------------------------------------------------
#define PTIF_LITTLE_ENDIAN  (0x4949)
#define PTIF_BIG_ENDIAN     (0x4D4D)
#define PTIF_SIGNATURE      (42)
//------------------------------------------------------------------------
#define TIF_RGBA            (0x01)
#define TIF_CMAP            (0x02)
//-----DATA TYPE ---------------------------------------------------------
#define TIF_TYPE_BYTE    (1)
#define TIF_TYPE_ASCII   (2)
#define TIF_TYPE_SHORT   (3)
#define TIF_TYPE_LONG    (4)
#define TIF_TYPE_RATIONAL  (5)
//========================================================================
//
// CTIF parser
//
//=========================================================================
class CTIFFparser {
  char         *Name;
  unsigned long Wd, Ht, Dp;
  U_CHAR        Tsp;              // Transparent option
  U_CHAR        Big;              // Endian option
  U_CHAR        Bps;              // Bit per sample
  U_CHAR        Spp;              // Sample per pixel          
  U_CHAR        Fmt;              // Format
  U_INT         Rps;              // Number of row per strip
  U_INT         Nst;              // Number of Strip
  //----Color MAP parameter---------------------------------
  U_INT         CMdim;            // Dimension(byte)
  U_INT         CMofs;            // Offset
  U_INT        *cMap;             // Color MAP
  //----Final Image ----------------------------------------
  U_INT         nPix;             // Number of pixels
  U_INT        *rgba;             // Final image
  U_INT        *bdst;             // Current destination
	U_INT					cnbp;							// Current pixel number
  //----Decoding structure ---------------------------------
  TIF_HDR       Hdr;              // TIF haeder
  TIF_DIR       Dir;              // Directory header
  TIF_STRIP    *Strip;            // Strip definition
  //--------------------------------------------------------
public:
  // Constructor/Destructor
   CTIFFparser (U_CHAR tsp);
  ~CTIFFparser ();
  //-------------------------------------------------------
   U_SHORT Value(short s);
  //-------------------------------------------------------
  GLubyte  *TransferRGB();
  U_INT    *TransferPIX();
  //--------------------------------------------------------
  void      CopyLine(U_INT *src,U_INT *dst);
  void      InvertPIX();
  int   Decode(char *fn);
  int   Warning(PODFILE *p,char *msg);
  short Read16 (PODFILE *p);
  long  Read32 (PODFILE *p);
  int   GetFormat(PODFILE *p,int f);
  int   GetStripOffset(PODFILE *p);
  int   GetStripCount(PODFILE *p);
  int   GetImage(PODFILE *p);
  void  GetColorMAP(PODFILE *p);
  void  GetStripFromCMAP(PODFILE *p,int k,U_INT *dst);
  void  GetStripFromRGBA(PODFILE *p,int k,U_INT *dst);
  //-------------------------------------------------------
  U_INT GetSide()       {return Wd; }
  int   GetWidth()      {return Wd; }
  int   GetHeight()     {return Ht; }
};
//=============================================================================
//  SMF decoder to parse a SMF file
//  This version will use vertex tables for parts
//  Parts are classified as
//  model 0:  Day   - Big size
//  model 1:  Night - Big size
//-----------------------------------------------------------
//  model 2:  Day   - Tin size
//  model 3:  Night - Tin  size
//-----------------------------------------------------------
//  Only part of hi resolution are kept
//=============================================================================
class CSMFparser: public CStreamObject {
  //---ATTRIBUTES ----------------------------------------
  C3Dmodel  *modl;                  // Current model
  U_CHAR     Tsp;                   // Transparency
  U_CHAR     Res;                   // Resolution
  C3DPart   *Prt;                   // Current part
  char      *fname;                 // File name
  int        nface;                 // Number of faces
  char       error;                 // Any error
  int        version;               // File version
  //----Model extension ----------------------------------
  CVector    vmax;                  // Maximum coordinates
  CVector    vmin;                  // Minimum coordinates
  //----Methods ------------------------------------------
public:
  CSMFparser(C3Dmodel *mod);
  int     Stop(char *msg);
  int     Warning(PODFILE *p,char *msg);
  int     Decode(char *fn);
  int     ReadPart(PODFILE *p,char *fn);
  int     ReadType(char *name);
  void    SaveExtension(F3_VERTEX &vtx);
  //------------------------------------------------------
 };
//=============================================================================
//  BIN decoder to parse a BIN file
//=============================================================================
class CBINparser: public CStreamObject {
  //---ATTRIBUTES ----------------------------------------
  char      error;                // Any error
  char      trace;                // Trace request
  C3Dmodel *modl;                 // 3D model
  C3DPart  *Prt;                  // Current part
  F3_VERTEX   *nVTX;                 // Vertex list
  U_CHAR    Tsp;                  // Transparency
  U_CHAR    shiny;                // Shiny texture
  U_CHAR    model;                // Part model
  U_CHAR    Res;                  // Resolution
  U_INT     xOBJ;                 // Texture OBJECT
  int       nFace;                // Number of faces
  char      Texn[20];             // Texture name
  char     *fn;                   // File name
  C3DPart  *head;                 // Heading part
  //------------------------------------------------------
  int       Tof;                  // Total faces
  int       Tov;                  // Total vertices
  //----Model extension ----------------------------------
  CVector    vmax;                  // Maximum coordinates
  CVector    vmin;                  // Minimum coordinates
  //---List of parts -------------------------------------
  C3DpartQ  prtQ;                 // Hi resolution Queue
  //------------------------------------------------------
  BIN_HEADER hdr;
  B19_HEADER b19;
  //------------------------------------------------------
public:
  CBINparser(C3Dmodel *mod);
 ~CBINparser();
  //-------------------------------------------------------
  float GetSValue(int vs);
  float GetTValue(int vs);
  int   Warning(PODFILE *p,char *msg);
  int   Stop(PODFILE *p,char *msg);
  int   Decode(char *fn,U_CHAR mod);
  int   ReadHeader(PODFILE *p);         // File header
  int   ReadVertex(PODFILE *p);         // Vertex list
  int   ReadBlock(PODFILE *p);          // Block type
  int   ReadNormal(PODFILE *p);         // Normal vectors
  int   ReadTexture(PODFILE *p);        // Texture name
  int   ReadTFaces(PODFILE *p);         // Textured faces
  int   ReadQFaces(PODFILE *p,int n);   // Textured QUAD faces
  int   ReadColor(PODFILE *p);
  int   Concatenate(PODFILE *p);
  int   UpdateHead(int nf);
  void  SaveExtension(F3_VERTEX &vtx);
  void  AddToModel(C3DPart *prt);
  //-------------------------------------------------------
  inline void Trace()     {trace = 1;}
};
//=======================================================================================
//  CLASS CRLPparser to decode Runway Light Profile
//=======================================================================================
class CRLParser: public CStreamObject {
    //--- ATTRIBUTES --------------------------------------------
    CAirportMgr *apm;
    RWY_EPF prof;                               // Runway profile
    //-----------------------------------------------------------
public:
    CRLParser(CAirportMgr *ap,char *fn);
    void  Decode(char *fn);
    int   Read(SStream *st,Tag tag);
    void  ReadFinished();
  };
//=======================================================================================
//  CLASS CMETARparser to decode metar bulletins
//=======================================================================================
class METARparser {
  //---ATTRIBUTES -----------------------------------------------
  FILE       *filr;               // File reference
  METAR_INFO *info;
  char        fld[32];
  //-------------------------------------------------------------
public:
  METARparser(FILE *f,METAR_INFO *inf,char *fn);
  int   Parse();
  int   Warn(char *msg);
  int   ParseWind(FILE *f);
  int   ParseWdP2(FILE *f);
  int   ParseUSvis(FILE *f);
  int   ParseEUvis(FILE *f);
  int   ParseRWvis(FILE *f);
  int   ParseRWend(FILE *f);
  int   ParseWeather(FILE *f);
  int   WeatherP1(FILE *f,char c);
  int   WeatherP2(FILE *f);
  int   ParseCover(FILE *f);
  int   CoverP1(FILE *f);
  int   CoverP2(FILE *f);
  int   ParseTemp(FILE *f);
  int   ParseAlti(FILE *f);
  int   ParseQNH(FILE *f);
  int   ParseBARO(FILE *f);
  //---------------------------------------------------------------
  int   ParseBody(FILE *f);
  int   SkipField(FILE *f);
};
//=======================================================================================
//  Bt header
//=======================================================================================
struct BtHead {
  char    ident[10];              // File header
  int     ncol;                   // Number of column
  int     nrow;                   // Number of rows
  short   dsiz;                   // Data size
  short   dtyp;                   // Data type
  short   unit;                   // Data unit
  short   zUTM;                   // UTM if relevent
  short   datum;                  // Data source
  double  xlef;                   // Left extend
  double  xrig;                   // Right extend
  double  xbot;                   // Bottom extend
  double  xtop;                   // Top extend
  short   proj;                   // projection
  float   scale;                  // Scale factor
};
//=======================================================================================
//  Base structure
//=======================================================================================
struct Corner {
  U_INT ax;
  U_INT ay;
};
//=======================================================================================
//  QGT region
//=======================================================================================
struct QRegion {
  //----QGT ---------------------------------
  U_INT qx0;            // Left side
  U_INT qx1;            // Right side
  U_INT qz0;            // bottom side
  U_INT qz1;            // Top side
  //-----------------------------------------
  U_INT qtx;            // Current qgt
  U_INT qtz;            // Current qgt
  //-----------------------------------------
  U_INT dtx;            // Depart X tile
  U_INT dtz;            // Depart Z tile
  U_INT ftx;            // End X tile
  U_INT ftz;            // End Z tile
  //-----------------------------------------
  U_INT nqx;
  U_INT nqz;
  //-----------------------------------------
  U_INT tx0;
  U_INT tx1;
  U_INT tz0;
  U_INT tz1;
};
//=======================================================================================
//  CLASS CBtParser to decode binary elevation BT files
//=======================================================================================
class CBtParser {
  //---- ATTRIBUTES -----------------------------------------
  FILE   *fp;                       // Disk file
  char   *fname;                    // File name
  BtHead  Head;                     // Header 
  int     fsiz;
  float  *data;
  //---- Limits in arcseconds -------------------------------
  float   lefS;                     // Left side   (including)
  float   rigS;                     // Right side  (excluding)
  float   topS;                     // Top side    (excluding)
  float   botS;                     // Bottom side (including)
  //---------------------------------------------------------
  bool    nxt;
  //----Grid spacing ----------------------------------------
  double  horz;                     // Horizontal in arcsec
  double  vert;                     // Vertical   in arcsec
  //----Target tile array -----------------------------------
  int     tDim;                     // Target array size
  U_INT   step;                     // Vertex step
  //----Limits in detail tiles keys -------------------------
  TC_BOUND mREG;                    // Region Limit
  //-----Working area ---------------------------------------
  QRegion  qReg;                    // QGT region
  int     *elPT;
  //---------------------------------------------------------
  CVector  dot;                     // Current dot
  CVector  SW;                      // SouthWest elevation
  CVector  NW;                      // NorthWest
  CVector  NE;                      // NorthEast
  CVector  SE;                      // SouthEast
  //---- Methods --------------------------------------------
public:
  CBtParser(char *fn);
 ~CBtParser();
 //--- Helpers ----------------------------------------------
 bool   GetQgtKey(U_INT &xk,U_INT &zk);
 bool   NextQgtKey();
 U_INT  NextVertexKey(U_INT vk);
 U_INT  NextTileKey(U_INT vk);
 U_INT  IncTileKey (U_INT vk,U_INT inc);
 //----------------------------------------------------------
 float  GetElevation(int c,int r);
 void   ComputeTileLimits();
 bool   VertexElevation(U_INT vx,U_INT vz);
 //----------------------------------------------------------
 bool   LowerTriangle();
 bool   UpperTriangle();
 //---------------------------------------------------------
 int    ProcessTile(U_INT tx,U_INT tz);
 bool   GetRegionElevation(REGION_REC &reg);
 //---------------------------------------------------------
 inline int Resolution()  {return tDim;}
};
//=======END OF FILE ============================================================================

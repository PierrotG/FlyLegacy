/*
 * Pod.cpp
 *
 * Part of Fly! Legacy project
 *
 * Copyright 2003-2005 Chris Wallace
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
 */

/**
 * @file Pod.cpp
 * @brief Implements Pod filesystem API functions
 */

/*!
 * \todo An occasional memory leak exists when pmount() is called, probably
 *       due to a leak in the multimap<> STL implementation.  The leak
 *       only occurs when many pod files are added to the podfile map of
 *       a given filesystem, and subsequent leaks grow in size, starting
 *       around 90K, increasing to 96K, then approx. 100K etc.  Code inspection
 *       does not reveal any potential problems, so perhaps dynamic resizing
 *       of the multimap is causing the leak to occur.
 */


#ifdef _MSC_VER
#pragma warning (disable:4189)    // Local variable initialized but not used
#pragma warning (disable:4710)    // Function not inlined
#endif

#include <plib/ul.h>
#include "../Include/FlyLegacy.h"
#include "../Include/Pod.h"
#include "../Include/Endian.h"
#include "../Include/Utility.h"

using namespace std;
//=================================================================================
enum XMLmark {
  X_SP   = 0x00000001,                    // This is a separator
  X_CH   = 0x00000002,                    // This is a character
  X_PA   = 0x00000004,                    // Space
  X_NU   = 0x00000008,                    // Digit
};
//=================================================================================
//  XML token marker
//  Each character in this table gives some makers for XML decoding
//=================================================================================
U_INT  xmlMARK[] = {
  //--00---01---02---03---04---05---06---07---08---09---0A---0B---0C---0D---0E---0F
    X_SP,                     // 00 NUL
    X_SP,                     // 01 SOH
    X_SP,                     // 02 STX
    X_SP,                     // 03 ETX
    X_SP,                     // 04 EOT
    X_SP,                     // 05 ENQ
    X_SP,                     // 06 ACK
    X_SP,                     // 07 BEL
    X_SP,                     // 08 BS
    X_SP+X_PA,                // 09 HT
    X_SP,                     // 0A LF
    X_SP,                     // 0B VT
    X_SP,                     // 0C FF
    X_SP,                     // 0D CR
    X_SP,                     // 0E SO
    X_SP,                     // 0F SI
    //--------------------------------------
    X_SP,                     // 10 DLE
    X_SP,                     // 11 DC1
    X_SP,                     // 12 DC2
    X_SP,                     // 13 DC3
    X_SP,                     // 14 DC4
    X_SP,                     // 15 NAK
    X_SP,                     // 16 SYN
    X_SP,                     // 17 ETB
    X_SP,                     // 18 CAN
    X_SP,                     // 19 EM
    X_SP,                     // 1A SUB
    X_SP,                     // 1B ESC
    X_SP,                     // 1C FS
    X_SP,                     // 1D GS
    X_SP,                     // 1E RS
    X_SP,                     // 1F US
    //------------------------------------
    X_SP+X_PA,                // 20 space
    X_SP,                     // 21 !
    X_SP,                     // 22 "
    X_SP,                     // 23 #
    X_SP,                     // 24 $
    X_SP,                     // 25 %
    X_SP,                     // 26 &
    X_SP,                     // 27 '
    X_SP,                     // 28 (
    X_SP,                     // 29 )
    X_SP,                     // 2A *
    X_SP,                     // 2B +
    X_SP,                     // 2C `
    X_SP,                     // 2D -
    X_SP,                     // 2E .
    X_SP,                     // 2F /
    //-----------------------------------
    X_NU,                     // 30 0
    X_NU,                     // 31 1
    X_NU,                     // 32 2
    X_NU,                     // 33 3
    X_NU,                     // 34 4
    X_NU,                     // 35 5
    X_NU,                     // 36 6
    X_NU,                     // 37 7
    X_NU,                     // 38 8
    X_NU,                     // 39 9
    X_SP,                     // 3A :
    X_SP,                     // 3B ;
    X_SP,                     // 3C <
    X_SP,                     // 3D =
    X_SP,                     // 3E >
    X_SP,                     // 3F ?
    //-----------------------------------
    X_SP,                     // 40 @
    X_CH,                     // 41 A
    X_CH,                     // 42 B
    X_CH,                     // 43 C
    X_CH,                     // 44 D
    X_CH,                     // 45 E
    X_CH,                     // 46 F
    X_CH,                     // 47 G
    X_CH,                     // 48 H
    X_CH,                     // 49 I
    X_CH,                     // 4A J
    X_CH,                     // 4B K
    X_CH,                     // 4C L
    X_CH,                     // 4D M
    X_CH,                     // 4E N
    X_CH,                     // 4F O
    //------------------------------------
    X_CH,                     // 50 P
    X_CH,                     // 51 Q
    X_CH,                     // 52 R
    X_CH,                     // 53 S
    X_CH,                     // 54 T
    X_CH,                     // 55 U
    X_CH,                     // 56 V
    X_CH,                     // 57 W
    X_CH,                     // 58 X
    X_CH,                     // 59 Y
    X_CH,                     // 5A Z
    X_SP,                     // 5B [
    X_SP,                     // 5C \ 
    X_SP,                     // 5D ]
    X_SP,                     // 5E ^
    X_SP,                     // 5F _
    //-----------------------------------
    X_SP,                     // 60 `
    X_CH,                     // 61 a
    X_CH,                     // 62 b
    X_CH,                     // 63 c
    X_CH,                     // 64 d
    X_CH,                     // 65 e
    X_CH,                     // 66 f
    X_CH,                     // 67 g
    X_CH,                     // 68 h
    X_CH,                     // 69 i
    X_CH,                     // 6A j
    X_CH,                     // 6B k
    X_CH,                     // 6C l
    X_CH,                     // 6D m
    X_CH,                     // 6E n
    X_CH,                     // 6F o
    //------------------------------------
    X_CH,                     // 70 p
    X_CH,                     // 71 q
    X_CH,                     // 72 r
    X_CH,                     // 73 s
    X_CH,                     // 74 t
    X_CH,                     // 75 u
    X_CH,                     // 76 v
    X_CH,                     // 77 w
    X_CH,                     // 78 x
    X_CH,                     // 79 y
    X_CH,                     // 7A z
    X_SP,                     // 7B {
    X_SP,                     // 7C |
    X_SP,                     // 7D }
    X_SP,                     // 7E ~
    X_SP,                     // 7F 
  };
//=================================================================================
// Local functions for extracting typical data types from stdio FILE*
//==================================================================================
static unsigned int freadBigUnsignedInt (FILE* f)
{
  unsigned int rc;
  fread ((char *)&rc, sizeof(unsigned int), 1, f);
  return BigEndian (rc);
}

static unsigned int freadLittleUnsignedInt (FILE* f)
{
  unsigned int rc;
  fread ((char *)&rc, sizeof(unsigned int), 1, f);
  return LittleEndian (rc);
}

/**
 * @brief Generate pod filesystem debugging log
 *
 * @param fmt... Variable arguments specifying log message
 */
static void plog (PFS *pPfs, const char* fmt, ...)
{
  if (pPfs->log != NULL) {
		va_list argp;
		va_start(argp, fmt);
		pPfs->log->Write (fmt, argp);
		va_end(argp);
  }  
}

///////////////////////////////////////////////////////////////////
//
// Standard C pod filesystem implementation
//

//
// Example:
//
// PFS pfs;
// pinit (&pfs, false);
// paddpodfolder ("C:/Fly! II/System");
// paddpodfolder ("C:/Fly! II/Aircraft", true);
// paddpodfolder ("C:/Fly! II", "System");
// padddiskfolder ("C:/Fly! II", "World");
// PFSFILE* f = popen ("World/Flyhawk.svh");
//...
//

/**
 * @brief Initialize pod filesystem
 *
 * @param pPfs  Pointer to PFS pod filesystem struct
 * @param searchPodFilesFirst If true, then calls to popen() and pexists() will check
 *                            PODs for the existence fo the file before checking disk
 *                            files.  If false, then the disk is searched first.
 */ 
// JSDEV* add logfile name to get distinct log file (normal and scenery)
//        add a lock for multithread access
void pinit (PFS* pPfs, const char *logfile,char *mod)
{ pthread_mutex_init (&pPfs->pfsMux,  NULL);             // Init lock
  pPfs->searchPodFilesFirst = false;
  // Get INI setting and open log if necessary
  pPfs->log = NULL;
  int i = 0;
  GetIniVar ("Logs", "logPodFilesystem", &i);
  pPfs->tlg   = i;
  if (i != 0) {
    pPfs->log = new CLogFile (logfile, mod);
  }

#ifdef POD_PERFORMANCE_METRICS
  // Initialize performance metrics
  pPfs->popenSuccess = pPfs->popenFailure = 0;
  pPfs->popenSuccessTotalTime.QuadPart = 0;
  pPfs->popenFailureTotalTime.QuadPart = 0;
#endif
}

/**
 * @brief Shut down pod filesystem
 *
 * @param pPfs  Pointer to PFS filesystem struct
 */
void pshutdown (PFS* pPfs)
{
  plog (pPfs, "Shutting down...");

  // Deallocate all entries in the pod list
  std::map<string,PFSPOD*>::iterator iPod;
  for (iPod=pPfs->podList.begin(); iPod!=pPfs->podList.end(); iPod++) {
    PFSPOD *pPod = iPod->second;
    if (pPod->file != NULL) fclose (pPod->file);
    delete pPod;
  }
  
  // Delete all entries in the pod file list
  std::multimap<string,PFSPODFILE*>::iterator i;
  for (i=pPfs->podFileList.begin(); i!=pPfs->podFileList.end(); i++) {
    PFSPODFILE *pf = i->second;
    delete pf;
  }

  plog (pPfs, "Shutdown complete");
  
  // Close log file
  SAFE_DELETE (pPfs->log);
}
//-------------------------------------------------------------------------------
//  Normalize pathName
//-------------------------------------------------------------------------------
void NormalizeName(char *name)
{ // Replace backslash path separators with application default forward slash
  char* sep = strchr (name, '\\');
  while (sep != NULL) {
    *sep = '/';
     sep = strchr (sep, '\\');
  }
  return;
}
//-------------------------------------------------------------------------------
//  Add a single file into the POD file system
//-------------------------------------------------------------------------------
static bool paddpodfile (PFS *pfs, PFSPODFILE *p)
{
  bool rc = true;
  NormalizeName(p->name);
  // Create pairing of filename with PFSPODFILE*
  pair<string,PFSPODFILE*>  pr;
  pr.first = p->name;
  pr.second = p;
  // JSDEV: Lock the PFS for multithread
  pthread_mutex_lock   (&pfs->pfsMux);             // Lock access
  pfs->podFileList.insert(pr);
  // Also insert filename into master filename set
  pfs->masterFileList.insert (p->name);
  // JSDEV Unlock PFS
  pthread_mutex_unlock   (&pfs->pfsMux);           // Lock access
  if (pfs->tlg > 1) plog (pfs,"......ADD file %s",p->name);
  return rc;
}
//-------------------------------------------------------------------------------
//  Remove the file from the POD file system
//-------------------------------------------------------------------------------
void prempodfile (PFS *pfs, char *fn)
{   pthread_mutex_lock   (&pfs->pfsMux);             // Lock access
    pfs->podFileList.erase(fn);
    pfs->masterFileList.erase(fn);
    pthread_mutex_unlock   (&pfs->pfsMux);           // Lock access
    if (pfs->tlg > 1) plog (pfs,"......REM file %s",fn);
    return;
}
//-------------------------------------------------------------------------------
//  Add a single file into the disk file system
//  When replace option (rep = 1) if the file name is allready registered then
//  the full path is changed with the new one.
//  NOTE: When mounting POD from scenery files, a lot of existing SMF or BIN files
//        from shared scenery are referenced and should not be replaced. 
//-------------------------------------------------------------------------------
void padddiskfile (PFS *pfs, string key, string fullFn,char rep)
{ // Search if file already exists; if so, generate log
  pthread_mutex_lock   (&pfs->pfsMux);             // Lock access
  std::map<string,string>::iterator i = pfs->diskFileList.find(key);
  if  (i == pfs->diskFileList.end())
      { plog (pfs, "  Add key %s with file %s", key.c_str(), fullFn.c_str());
        pfs->diskFileList[key] = fullFn;
        pfs->masterFileList.insert (key);         
      }
  else if (1 == rep)
      { plog (pfs, "  Rep key %s with file %s", key.c_str(), fullFn.c_str());
        pfs->diskFileList[key] = fullFn;
        pfs->masterFileList.insert (key); 
      }
  pthread_mutex_unlock   (&pfs->pfsMux);           // Lock access
  return;
}
//--------------------------------------------------------------------------------
//  Add one disk file to internal file system
//--------------------------------------------------------------------------------
void pAddDisk(PFS *pfs, char *key, char *fn)
{ padddiskfile (pfs, key, fn,0);
  return;
}
//--------------------------------------------------------------------------------
//  Remove a disk file
//--------------------------------------------------------------------------------
void pRemDisk(PFS *pfs,char *key,char *fn)
{ NormalizeName(key);
  NormalizeName(fn);
  pfs->diskFileList.erase(key);
  pfs->masterFileList.erase(key);
  // To test  check for file ---------------------------------
  //const char *n = 0;
  //std::map<string,string>::iterator i = pfs->diskFileList.find(key);
  //if (i != pfs->diskFileList.end()) n = (*i).second.c_str();
  //-----------------------------------------------------------
  return;
}

///--------------------------------------------------------------------
// @brief Mount an EPD format pod into the specified pod filesystem
//
// @param pPod  Pointer to the EPD pod details
 //-----------------------------------------------------------------------
static void pmountepd (PFS *pPfs, PFSPOD* pPod)
{
  FILE* f = pPod->file;

  // Read EPD header
  char volume[EPD_VOLUME_LENGTH+1];
  fread (volume, 1, EPD_VOLUME_LENGTH, f);
  pPod->nEntries = freadLittleUnsignedInt (f);

  // Skip version number
  freadLittleUnsignedInt (f);
  
  pPod->checksum  = freadLittleUnsignedInt (f);
  pPod->Directory = ftell(f);
  // Read EPD directory and add PFSPODFILE structs to the POD file list
  for (unsigned int i=0; i<pPod->nEntries; i++) {
    // Instantiate new PFSPODFILE struct
    PFSPODFILE* pFile = new PFSPODFILE;

    // Populate struct with EPD directory entry fields
    pFile->pod = pPod;
    fread (pFile->name, 1, EPD_FILENAME_LENGTH, f);
    pFile->size       = freadLittleUnsignedInt (f);
    pFile->offset     = freadLittleUnsignedInt (f);
    pFile->timestamp  = freadLittleUnsignedInt (f);
    pFile->checksum   = freadLittleUnsignedInt (f);
    // EPD files do not support mount priority, so use default of 1000
    pFile->priority = 1000;

    if (!paddpodfile (pPfs, pFile)) {
      // Pod file was rejected; delete it
      delete pFile;
    }
  }
}
///--------------------------------------------------------------------
// Unmount an EPD format pod from the specified pod filesystem
//
//-----------------------------------------------------------------------
void unMountEPD (PFS *pfs, PFSPOD* pod)
{ FILE* f = pod->file;
  char none[256];
  char fn[PATH_MAX];
  //---Go to directory ------------------------------------
  fseek (f,pod->Directory, SEEK_SET);
  //---  Read EPD directory and remove file list
  for (unsigned int i=0; i<pod->nEntries; i++) {
    // Instantiate new PFSPODFILE struct
    fread (fn, 1, EPD_FILENAME_LENGTH, f);
    fread (none,sizeof(unsigned int),4,f);  // Bypass
    // EPD files do not support mount priority, so use default of 1000
    NormalizeName(fn);
    prempodfile (pfs,fn); 
    
  }
}
///--------------------------------------------------------------------
// @brief Mount an POD2 format pod into the specified pod filesystem
//
// @param pPod  Pointer to the POD2 pod details
 //-----------------------------------------------------------------------

static void pmountpod2 (PFS *pPfs, PFSPOD* pPod)
{
  int i, j;
  
  // Local copy of the POD's FILE* for convenience
  FILE* f = pPod->file;

  // Read POD2 header
  pPod->checksum  = freadLittleUnsignedInt (f);
  char volume[POD_VOLUME_LENGTH+1];
  fread (volume, POD_VOLUME_LENGTH, 1, f);
  pPod->nEntries = freadLittleUnsignedInt (f);
  pPod->nAuditEntries = freadLittleUnsignedInt (f);
  pPod->revision = 0;
  pPod->priority = 1000;

  // Calculate starting offset of filename string table
  unsigned int offsetStringTable = (POD_VOLUME_LENGTH
    + (4 * sizeof(unsigned int))
    + (pPod->nEntries * 20));

  /// \todo Optimize by reading entire string table into memory
  /// \todo Read audit trail
  
  // Read POD2 directory, and add PFSPODFILE structs to POD file list
  pPod->Directory = ftell(f);
  for (i=0; i<(int)pPod->nEntries; i++) {
    // Instantiate new PFSPODFILE struct
    PFSPODFILE* pFile = new PFSPODFILE;

    pFile->pod = pPod;
    unsigned int offsetFilename = freadLittleUnsignedInt (f);
    pFile->size = freadLittleUnsignedInt (f);
    pFile->offset = freadLittleUnsignedInt (f);
    pFile->timestamp = freadLittleUnsignedInt (f);
    pFile->checksum = freadLittleUnsignedInt (f);

    // Mount priority is not supported by the POD2 format, use default of 1000
    pFile->priority = 1000;

    // Reposition file pointer to location in string table of the filename,
    //   read the filename then reposition back to the next directory entry
    long pos = ftell (f);
    fseek (f, offsetStringTable + offsetFilename, SEEK_SET);
    char c;
    for (j=0; j<POD_FILENAME_LENGTH; j++) {
      fread (&c, 1, 1, f);
      if (c == '\0') break;
      pFile->name[j] = c;
    }
    pFile->name[j] = '\0';
    fseek (f, pos, SEEK_SET);

    if (!paddpodfile (pPfs, pFile)) {
      // Pod file was rejected; delete it
      delete pFile;
    }
  }
}
///--------------------------------------------------------------------
// Unmount an POD2 format pod from the specified pod filesystem
//
//-----------------------------------------------------------------------
void unMountPOD2 (PFS *pfs, PFSPOD* pod)
{ int i, j;
  char none[128];
  FILE* f = pod->file;

  // --Position to directory ----------------------------------------
  fseek (f,pod->Directory, SEEK_SET);
  unsigned int offsetStringTable = (POD_VOLUME_LENGTH
    + (4 * sizeof(unsigned int))
    + (pod->nEntries * 20));

  char fn[PATH_MAX];
  // Read POD2 directory, and add PFSPODFILE structs to POD file list
  for (i=0; i<(int)pod->nEntries; i++) {
      unsigned int offsetFilename = freadLittleUnsignedInt (f);
      fread(none,sizeof(unsigned int),4,f);       // By pass 
      // Reposition file pointer to location in string table of the filename,
      //   read the filename then reposition back to the next directory entry
      long pos = ftell (f);
      fseek (f, offsetStringTable + offsetFilename, SEEK_SET);
      char c;
      for (j=0; j<POD_FILENAME_LENGTH; j++) {
        fread (&c, 1, 1, f);
        if (c == '\0') break;
        if (c == '\\') c = '/';
        fn[j] = c;
        }
    fn[j] = '\0';
    fseek (f, pos, SEEK_SET);
    prempodfile (pfs,fn);
  }
}
///--------------------------------------------------------------------
// @brief Mount an POD3 format pod into the specified pod filesystem
//
// @param pPod  Pointer to the POD3 pod details
 //-----------------------------------------------------------------------

static void pmountpod3 (PFS *pPfs, PFSPOD* pPod)
{
  int i, j;

  // Local copy of the POD's FILE* for convenience
  FILE* f = pPod->file;

  pPod->checksum = freadLittleUnsignedInt (f);
  char volume[POD_VOLUME_LENGTH+1];
  fread (volume, POD_VOLUME_LENGTH, 1, f);
  pPod->nEntries = freadLittleUnsignedInt (f);
  pPod->nAuditEntries = freadLittleUnsignedInt (f);
  pPod->revision = freadLittleUnsignedInt (f);
  pPod->priority = freadLittleUnsignedInt (f);
  char author[POD_AUTHOR_LENGTH+1];
  fread (author, POD_AUTHOR_LENGTH, 1, f);
  char copyright[POD_COPYRIGHT_LENGTH+1];
  fread (copyright, POD_COPYRIGHT_LENGTH, 1, f);
  //---Mark the position -----------------------------
  pPod->Directory = ftell(f);
  unsigned int offsetDirectory = freadLittleUnsignedInt (f);

  // The following POD3 header fields are ignored:
  //    checksum
  //    string table size
  //    dependency
  //    unknown4
  //    unknown5
  
  // Calculate starting offset of filename string table
  unsigned int offsetStringTable = offsetDirectory + (pPod->nEntries * 20);

  // Reposition file pointer to start of POD directory
  fseek (f, offsetDirectory, SEEK_SET);
  for (i=0; i<(int)pPod->nEntries; i++) {
    // Instantiate new PFSPODFILE struct
    PFSPODFILE* pFile = new PFSPODFILE;

    pFile->pod = pPod;
    unsigned int offsetFilename = freadLittleUnsignedInt (f);
    pFile->size = freadLittleUnsignedInt (f);
    pFile->offset = freadLittleUnsignedInt (f);
    pFile->priority = pPod->priority;
    pFile->timestamp = freadLittleUnsignedInt (f);
    pFile->checksum  = freadLittleUnsignedInt (f);

    // Reposition file pointer to location in string table of the filename,
    //   read the filename then reposition back to the next directory entry
    long pos = ftell (f);
    fseek (f, offsetStringTable + offsetFilename, SEEK_SET);
    strcpy (pFile->name, "");
    char c;
    for (j=0; j<POD_FILENAME_LENGTH; j++) {
      fread (&c, 1, 1, f);
      if (c == '\0') break;
      pFile->name[j] = c;
    }
    pFile->name[j] = '\0';
    //-------------------------------------
    fseek (f, pos, SEEK_SET);

    if (!paddpodfile (pPfs, pFile)) {
      // Pod file was rejected; delete it
      delete pFile;
    }
  }
}
///--------------------------------------------------------------------
// Unmount an POD3 format pod from the specified pod filesystem
// NOTE: Not yet tested
//-----------------------------------------------------------------------
void unMountPOD3 (PFS *pfs, PFSPOD* pod)
{ int i, j;
  char none[128];
  char fn[PATH_MAX];
  FILE* f = pod->file;
  // --Position to offset ----------------------------------------
  fseek (f,pod->Directory, SEEK_SET);

  unsigned int offsetDirectory = freadLittleUnsignedInt (f);

  // The following POD3 header fields are ignored:
  //    checksum
  //    string table size
  //    dependency
  //    unknown4
  //    unknown5
  
  // Calculate starting offset of filename string table
  unsigned int offsetStringTable = offsetDirectory + (pod->nEntries * 20);

  // Reposition file pointer to start of POD directory
  fseek (f, offsetDirectory, SEEK_SET);

  for (i=0; i<(int)pod->nEntries; i++) {

    unsigned int offsetFilename = freadLittleUnsignedInt (f);
    fread(none,sizeof(unsigned int),4,f); //Bypass
    // Reposition file pointer to location in string table of the filename,
    //   read the filename then reposition back to the next directory entry
    long pos = ftell (f);
    fseek (f, offsetStringTable + offsetFilename, SEEK_SET);
    char c;
    for (j=0; j<POD_FILENAME_LENGTH; j++) {
      fread (&c, 1, 1, f);
      if (c == '\0') break;
      if (c == '\\') c = '/';
      fn[j] = c;
    }
    fn[j] = '\0';
    fseek (f, pos, SEEK_SET);
    prempodfile (pfs,fn);
  }
  return;
}
//-----------------------------------------------------------------------
//  Mount a POD file (EPD,POD or POD3)
//-----------------------------------------------------------------------
static PFSPOD* pmount (PFS *pPfs, const char* podname)
{
  PFSPOD* pPod = new PFSPOD;
  strcpy (pPod->name, podname);
  pPod->file = NULL;
  pPod->format = PodFormatUnknown;
  pPod->refs = 0;

  FILE* f = fopen (podname, "rb");
  if (f != NULL) {
    pPod->file = f;
    
    // Read POD type signature
    unsigned int signature = freadBigUnsignedInt (f);

    // Set pod format based on signature
    switch (signature) {
    case PodSignatureEpd:
      pPod->format = PodFormatEpd;
      pmountepd (pPfs, pPod);
      break;

    case PodSignaturePod2:
      pPod->format = PodFormatPod2;
      pmountpod2 (pPfs, pPod);
      break;

    case PodSignaturePod3:
      pPod->format = PodFormatPod3;
      pmountpod3 (pPfs, pPod);
      break;

    default:
      pPod->format = PodFormatUnknown;
    }
  } else {
//    WARNINGLOG ("pmount : Could not open POD file %s", podname);
  }
  return pPod;
}
//----------------------------------------------------------------
//  Remove a POD file
//----------------------------------------------------------------
void pUnMount (PFS *pfs, PFSPOD *pod)
{
    // Unmount according to format
    switch (pod->format) {
    case PodFormatEpd:
      unMountEPD(pfs, pod);
      break;

    case PodFormatPod2:
      unMountPOD2(pfs, pod);
      break;

    case PodFormatPod3:
      unMountPOD3(pfs, pod);
      break;

    }
  return;
}
//--------------------------------------------------------------------------------
//  Add a POD file to the File SYSTEM
//  NOTE:  ONLY .POD should be supplied here
//--------------------------------------------------------------------------------
void paddpod (PFS *pPfs, const char* filename)
{ 
  PFSPOD *pPod = pmount (pPfs, filename);
  pPfs->podList[filename] = pPod;
  plog (pPfs, "Adding POD: %s", filename);
}
//-------------------------------------------------------------------------------
//  Remove POD from the File System
//  NOTE: Only .POD should be supplied here
//  Question: The file is not removed from podFileList
//-------------------------------------------------------------------------------
void premovepod (PFS *pfs, const char* fn)
{
  plog (pfs, "Removing POD: %s", fn);

  //---- Remove entry from the pod list -----------
  std::map<string,PFSPOD*>::iterator iter = pfs->podList.find(fn);
  if (iter == pfs->podList.end()) return;
  PFSPOD *pod = iter->second;
  pfs->podList.erase(fn);
    
  // Close the associated disk file if it is open
  if (pod->format != PodFormatUnknown) pUnMount(pfs,pod);
  if (pod->file) fclose (pod->file);
  delete pod;
  return;
}

//-------------------------------------------------------------------------------
// All POD files (with .POD or .EPD extension) in the specified folder are added
//   to the POD filesystem.
//-------------------------------------------------------------------------------
void paddpodfolder (PFS* pPfs, const char* folder, bool addSubdirs)
{
  plog (pPfs, "Adding POD folder: %s", folder);

/*
  // Create full POD folder by concatenating root folder with sub-folder name
  char path[PATH_MAX];
  strcpy (path, pPfs->root);
  strcat (path, "/");
  strcat (path, folder);
*/
  // Iterate over all files and subdirectories in this folder.
  ulDir* dirp = ulOpenDir (folder);
  if (dirp != NULL) {
    ulDirEnt* dp;
    while ( (dp = ulReadDir(dirp)) != NULL )
    {
      if (dp->d_isdir) {
        // If addSubdirs flag is set and this is not current or parent directory,
        //   recursively add it too
        if (addSubdirs) {
          if ((strcmp (dp->d_name, ".") != 0) && (strcmp (dp->d_name, "..") != 0)) {
            char subFolder[PATH_MAX];
            strcpy (subFolder, folder);
            strcat (subFolder, "/");
            strcat (subFolder, dp->d_name);
            paddpodfolder (pPfs, subFolder, addSubdirs);
          }
        }
      } else {
        // Check the file extension
        char *c = strrchr (dp->d_name, '.');
        if ((stricmp (c, ".EPD") == 0) || (stricmp(c, ".POD") == 0)) {
          // This is a POD file, create a new PFSPOD instance and add
          //   it to the POD list for the filesystem
          char podfilename[PATH_MAX];
          strcpy (podfilename, folder);
          strcat (podfilename, "/");
          strcat (podfilename, dp->d_name);

          /// \todo Why not use paddpod?

          PFSPOD *pPod = pmount (pPfs, podfilename);
          pPfs->podList[podfilename] = pPod;

          plog (pPfs, "Adding POD: %s", podfilename);

            // Update podFileList
//          std::map<string,PFSPODFILE*>::iterator i;
//          for (i=pPod->fileList.begin(); i!=pPod->fileList.end(); i++) {
//            pPfs->podFileList[i->second->name] = pPod;
//          }
        }
      }
    }
    ulCloseDir(dirp);
  }
}

//--------------------------------------------------------------------------------
// This function adds all non-POD files in the specified folder to the referenced
//   pod filesystem.  Sub-folders are recursively added as well.
//---------------------------------------------------------------------------------
void padddiskfolder (PFS *pPfs, const char* root, const char* folder)
{
  plog (pPfs, "Adding disk folder: \"%s\" \"%s\"", root, folder);

  // Create full folder name by concatenating root folder with sub-folder name
  char path[PATH_MAX];
  strcpy (path, root);
  if (strlen(folder) > 0) {
    strcat (path, "/");
    strcat (path, folder);
  }

  // Iterate over all files in this folder.
  ulDir* dirp = ulOpenDir (path);
  if (dirp != NULL) {
    ulDirEnt* dp;
    while ( (dp = ulReadDir(dirp)) != NULL )
    {
      if (dp->d_isdir) {
        // This is a sub-folder, recursively add it
        if ((strcmp (dp->d_name, ".") != 0) && (strcmp (dp->d_name, "..") != 0)) {
          char subfolder[1024];
          if (strlen(folder) > 0) {
            // Initial folder name is non-empty, concatenate subfolder name with slash
            strcpy (subfolder, folder);
            strcat (subfolder, "/");
            strcat (subfolder, dp->d_name);
          } else {
            // Initial folder name is empty, don't prepend opening slash
            strcpy (subfolder, dp->d_name);
          }
          padddiskfolder (pPfs, root, subfolder);
        }
      } else {
        // This is a file, check the extension
        char *c = strrchr (dp->d_name, '.');
        if ((c != NULL) && ((stricmp (c, ".EPD") == 0) || (stricmp(c, ".POD") == 0))) {
          // This is a POD file, ignore it
          continue;
        } else {
          // This is not a POD file, add it to the set of disk files
          // First construct key filename
          char keyFilename[PATH_MAX];
          if (strlen(folder) > 0) {
            // Folder name is non-empty, concatenate filename after folder name with slash
            strcpy (keyFilename, folder);
            strcat (keyFilename, "/");
            strcat (keyFilename, dp->d_name);
          } else {
            // Folder name is empty, initialize to filename only
            strcpy (keyFilename, dp->d_name);
          }
          strupper (keyFilename);
          NormalizeName(keyFilename);

          // Next construct full path and filename
          char fullFilename[PATH_MAX];
          strcpy (fullFilename, root);
          strcat (fullFilename, "/");
          if (strlen(folder) > 0) {
            strcat (fullFilename, folder);
            strcat (fullFilename, "/");
          }
          strcat (fullFilename, dp->d_name);
          NormalizeName(fullFilename);
          plog (pPfs, "Adding disk file  : \"%s\" \"%s\"", keyFilename, fullFilename);

          padddiskfile (pPfs, keyFilename, fullFilename,1);
        }
      }
    }
    ulCloseDir(dirp);
  }
}
//-----------------------------------------------------------------------
//  Search file in pod
//  WARNING:  This routine is locked for multithread
//-----------------------------------------------------------------------
static PODFILE* findinpod (PFS* pPfs, const char* filename)
{
  PODFILE* p = NULL;
  pthread_mutex_lock   (&pPfs->pfsMux);             // Lock access
  // POD filenames are case-insensitive, normalize case
  char keyFilename[PATH_MAX];
  strcpy (keyFilename, filename);
  strupper (keyFilename);

  PFSPODFILE *pf = NULL;
  std::multimap<string,PFSPODFILE*>::iterator i;
  std::multimap<string,PFSPODFILE*>::iterator lower = pPfs->podFileList.lower_bound(keyFilename);
  std::multimap<string,PFSPODFILE*>::iterator upper = pPfs->podFileList.upper_bound(keyFilename);
  unsigned long priority = 9999;
  for (i=lower; i!=upper; i++) {
    // There is at least one file with this name in the list of pod files
    // Find the highest-priority instance and return it
    if (i->second->priority < priority) {
      // This instance is the highest priority so far
      if (priority < 9999) {
        // Record priority override in pod filesystem debug log
        char debug[PATH_MAX + 100];
        sprintf (debug, "Override %s pri %d in %s with pri %d in %s",
                keyFilename,
                pf->priority, pf->pod->name,
                i->second->priority, i->second->pod->name);
        plog (pPfs, debug);
      }
      pf = i->second;
      priority = pf->priority;
    }
  }
  
  if (pf != NULL) {
    // Instantiate new PODFILE struct
    p = new PODFILE;
    
    // Link this PODFILE struct to the underlying POD
    strcpy (p->fullFilename, "");
    strcpy (p->filename, keyFilename);
    p->source = PODFILE_SOURCE_POD;
    p->pPod   = pf->pod;
    p->pFile  = pf->pod->file;
    p->offset = pf->offset;
    p->pos    = pf->offset;
    p->size   = pf->size;
    
    // Increment reference counter of containing POD
    pf->pod->refs++;
  }
  pthread_mutex_unlock   (&pPfs->pfsMux);             // Lock access
  return p;
}
//----------------------------------------------------------------------
//  Search file on disk
//  WARNING:  This routine is locked for multithread
//----------------------------------------------------------------------
static PODFILE* findondisk (PFS* pPfs, const char* keyFilename,char *md)
{ char *mode  = (md)?(md):("rb");
  PODFILE* p = NULL;
  pthread_mutex_lock   (&pPfs->pfsMux);             // Lock access
  std::map<string,string>::iterator i = pPfs->diskFileList.find(keyFilename);
  if (i != pPfs->diskFileList.end()) {
    // Found it, attempt to open the disk file
    const char *fullFilename = i->second.c_str();
    FILE* f = fopen (fullFilename, mode);
    if (f != NULL) {
      // File was successfully opened; create PODFILE for return value
      p = new PODFILE;
      strcpy (p->fullFilename, fullFilename);
      strcpy (p->filename, keyFilename);
      p->pFile = f;
      p->pPod = NULL;
      p->source = PODFILE_SOURCE_DISK;
      p->offset = 0;
      p->pos = 0;
      fseek (p->pFile, 0, SEEK_END);
      p->size = ftell (p->pFile);
      rewind (p->pFile);
    }
  }
  pthread_mutex_unlock   (&pPfs->pfsMux);           // Lock access
  return p;
}


//---------------------------------------------------------------------------
// Search all mounted pod files for the given filename
//
// Returns: true if the file exists in a pod, false otherwise
//  WARNING:  This routine is locked for multithread
//---------------------------------------------------------------------------
static bool existsinpod (PFS *pPfs, const char* filename)
{
  bool rc = false;
  //JSDEV:  Add a lock for multi thread
  pthread_mutex_lock   (&pPfs->pfsMux);             // Lock access
  // Search pod file list for normalized filename
  std::multimap<string,PFSPODFILE*>::iterator i = pPfs->podFileList.find(filename);
  rc = (i != pPfs->podFileList.end());
  pthread_mutex_unlock   (&pPfs->pfsMux);           // Unlock access
  return rc;
}

//---------------------------------------------------------------------------
// Search the disk file list for the given filename
//
// Returns: true if the file exists on disk, false otherwise
//  WARNING:  This routine is locked for multithread
//---------------------------------------------------------------------------
static bool existsondisk (PFS *pPfs, const char* filename)
{
  bool rc = false;
  pthread_mutex_lock   (&pPfs->pfsMux);             // Lock access
  // Search for existence of the filename in the disk file list set
  std::map<string,string>::iterator i = pPfs->diskFileList.find(filename);
  rc = (i != pPfs->diskFileList.end());
  pthread_mutex_unlock   (&pPfs->pfsMux);           // Unlock access
  return rc;
}
//---------------------------------------------------------------------------
// Test for a file
//
// Returns: true if the file exists on disk, false otherwise
//---------------------------------------------------------------------------

bool pexists (PFS* pPfs, const char* filename)
{
  bool rc = false;

  // Make local copy of filename, normalize path separators
  char *localFilename = new char[strlen(filename) + 1];
  strcpy (localFilename, filename);
  NormalizeName(localFilename);
  strupper (localFilename);			// JSDEV* key is upper case
  if (pPfs->searchPodFilesFirst) {
    // Search for filename in all mounted PODs.
    rc = existsinpod (pPfs, localFilename);
    if (!rc) {
      // Not found in a POD, search disk files
      rc = existsondisk (pPfs, localFilename);
    }
  } else {
    // Search for filename in disk files
    rc = existsondisk (pPfs, localFilename);
    if (!rc) {
      // Not found on disk, search all mounted PODs
      rc = existsinpod (pPfs, localFilename);
    }
  }

  // Delete local copy of filename
  delete[] localFilename;

  return rc;
}

/*!
 * Local function to compare a string against a wildcard pattern
 *
 * Compare the string \a s with the \a pattern which may contain
 *   wildcard characters * and/or ?.
 * Based on an algorithm by Jack Handy published at:
 *   http://www.codeproject.com/string/wildcmp.asp
 */
static bool compare_wildcard (const char *pattern, const char *s)
{
  const char *cp = NULL, *mp = NULL;
  if (strncmp (s, "MAPS", 4) == 0) {
    int x = 5;
  }
  // Check for match against leading chars in pattern up to first * wildcard
  while ((*s != '\0') && (*pattern != '*')) {
    if (*pattern != *s) {
      // Next character of pattern does not match next character of string
      if (*pattern == '?') {
        // Pattern is single-character wildcard, so mismatch is OK
        pattern++;
        s++;
      } else {
        // Pattern is not ? wildcard, string does not match pattern
        return false;
      }
    } else {
      // Character is an exact match to pattern, increment to next character
      pattern++;
      s++;
    }
  }

  // Check remainder of string against remainder of pattern
  while (*s != '\0') {
    if (*pattern == '*') {
      // Go to next char of pattern
      ++pattern;
      if (*pattern == '\0') {
        // End of pattern reached, the string matches
        return true;
      }
      mp = pattern;
      cp = s+1;
    } else if ((*pattern == *s) || (*pattern == '?')) {
      // This character is a match, go to next char of pattern and string
      ++pattern;
      ++s;
    } else {
      pattern = mp;
      s = cp++;
    }
  }
  
  // End of string has been reached, consume any trailing * wildcards
  while (*pattern == '*') pattern++;

  // If end of pattern has been reached then compare was successful
  return (*pattern == '\0');
}

const char* pfindfirst (PFS* pPfs, const char* pattern)
{
  const char* rc = NULL;
  
  // Assign search pattern to PFS member
  memset (pPfs->patternFind, 0, POD_FILENAME_LENGTH+1);
  strncpy (pPfs->patternFind, pattern, POD_FILENAME_LENGTH);
  strupper (pPfs->patternFind);
  
  // Search master file list for matches
  pPfs->iterFind = pPfs->masterFileList.begin();   
  while (pPfs->iterFind != pPfs->masterFileList.end() && (rc == NULL)) {
    if (compare_wildcard (pPfs->patternFind, pPfs->iterFind->c_str())) {
      // Match was found
      rc = pPfs->iterFind->c_str();
    }

    // Step to next member of the set
    pPfs->iterFind++;
  }
  
  return rc;
}

const char* pfindnext (PFS* pPfs)
{
  const char* rc = NULL;

  // Search master file list for matches
  while (pPfs->iterFind != pPfs->masterFileList.end() && (rc == NULL)) {
    if (compare_wildcard (pPfs->patternFind, pPfs->iterFind->c_str())) {
      // Match was found
      rc = pPfs->iterFind->c_str();
    }
    // Step to next member of the set
    pPfs->iterFind++;
  }

  return rc;
}
//------------------------------------------------------------------------
//  Open File:
//  NOTE: File may be open in read write mode (R+), but in this case
//        they are search only in diskfile.  We can't rewrite a pod
//        file.
//        Only single file may be modified
//------------------------------------------------------------------------
PODFILE* popen (PFS* pPfs, const char* fname, char *md)
{
  PODFILE *p = NULL;

#ifdef POD_PERFORMANCE_METRICS
  // Get timestamp
  LARGE_INTEGER start, stop, delta;
  QueryPerformanceCounter (&start);
#endif

  // Make local copy of filename, and normalize path separators
  char filename[PATH_MAX];
  strcpy (filename, fname);
  // TODO: Optimize with one-pass upper case and path separator conversion?
  strupper (filename);
  NormalizeName(filename);
  if (pPfs->searchPodFilesFirst && (md == 0)) 
  { // Search for filename in all mounted PODs.
    p = findinpod (pPfs, filename);
    if (p == NULL) {
    // Not found in a POD, search disk files
    p = findondisk (pPfs, filename,md);
  }
  } else 
  { // Search for filename in disk files
    p = findondisk (pPfs, filename,md);
    if ((p == NULL) && (0 == md))
    { // Not found on disk, search all mounted PODs
      p = findinpod (pPfs, filename);
    }
  }

#ifdef POD_PERFORMANCE_METRICS
  // Get final timestamp and update performance metrics (logging time should not
  //   be included in measurements
  delta.QuadPart = 0;
  QueryPerformanceCounter (&stop);
  if (stop.QuadPart < start.QuadPart) {
#ifdef HAVE_MAXLONGLONG
      delta.QuadPart = stop.QuadPart + (MAXLONGLONG - start.QuadPart);
#else
      gtfo ("popen : Performance counter wraparound\n");
#endif // HAVE_MAXLONGLONG
  } else {
    delta.QuadPart = stop.QuadPart - start.QuadPart;
  }

  // Record measured interval against either success or failure metric
  if (p == NULL) {
    // popen failure
    pPfs->popenFailure++;
    pPfs->popenFailureTotalTime.QuadPart += delta.QuadPart;
  } else {
    // popen success
    pPfs->popenSuccess++;
    pPfs->popenSuccessTotalTime.QuadPart += delta.QuadPart;
  }
#endif

  // Generate log if file not found
  if (p == NULL) {
    plog (pPfs, "popen() : Failed to open %s", filename);
  } else {
    if (p->source == PODFILE_SOURCE_POD) plog (pPfs, "popen() : Successful open %s (%s)", filename, p->pPod->name);
      else    plog (pPfs, "popen() : Successful open %s (%s)", filename, p->fullFilename);
  }

  return p;
}
//======================================================================
//  Read POD file
//======================================================================
size_t pread (void* buffer, size_t size, size_t count, PODFILE* f)
{
  size_t rc = 0;
  if (f) {
    switch (f->source) {
    case PODFILE_SOURCE_POD:
      // Reads to different files within the same POD may be interspersed,
      //   so the read pointer must be reset to the next expected byte for
      //   this pod file.
      _lock_file(f->pFile);                     // JS Make thread safe
      fseek (f->pFile, f->pos, SEEK_SET);
      rc = fread (buffer, size, count, f->pFile);
      f->pos = ftell (f->pFile);
      _unlock_file(f->pFile);                   // JS Make thread safe
      break;

    case PODFILE_SOURCE_DISK:
      _lock_file(f->pFile);                     // JS Make thread safe
      rc = fread (buffer, size, count, f->pFile);
      _unlock_file(f->pFile);                   // JS Make thread safe
      break;
    }
  }

  return rc;
}
//======================================================================
//  Write POD file
//======================================================================
size_t pwrite(void *buf,size_t size,size_t count, PODFILE *f)
{ size_t nf = 0;
  if (0 == f)  return 0;
  switch(f->source) {
    case PODFILE_SOURCE_POD:
      return 0;
    case PODFILE_SOURCE_DISK:
      _lock_file(f->pFile);                     // JS Make thread safe
      fseek (f->pFile, f->pos, SEEK_SET);
      nf = fwrite (buf, size, count, f->pFile);
           fflush(f->pFile);
      _unlock_file(f->pFile);                   // JS Make thread safe
      return nf;
  }
  return 0;
}
//======================================================================
//  Check for EOF
//======================================================================
int peof (PODFILE* f)
{
  int rc = true;
  if (f) {
    switch (f->source) {
    case PODFILE_SOURCE_POD:
      if (f->pos < (f->offset + f->size)) {
        rc = false;
      }
      break;
    case PODFILE_SOURCE_DISK:
      rc = feof (f->pFile);
      break;
    }
  }
  return rc;
}
//======================================================================
//  Check for error
//======================================================================
int perror (PODFILE* f)
{
  int rc = 0;
  if (f) {
    switch (f->source) {
    case PODFILE_SOURCE_POD:
      rc = ferror (f->pFile);
      break;

    case PODFILE_SOURCE_DISK:
      rc = ferror (f->pFile);
      break;
    }
  }
  return rc;
}
//======================================================================
//  Get a character
//======================================================================
int pgetc (PODFILE* f)
{
  int rc = 0;
  if (f) {
    switch (f->source) {
    case PODFILE_SOURCE_POD:
      // Reads to different files within the same POD may be interspersed,
      //   so the read pointer must be reset to the next expected byte for
      //   this pod file.
      fseek (f->pFile, f->pos, SEEK_SET);
      rc = fgetc (f->pFile);
      f->pos = ftell (f->pFile);
      break;

    case PODFILE_SOURCE_DISK:
      rc = fgetc (f->pFile);
      break;
    }
  }

  return rc;
}
//======================================================================
//  Get a string from pod file
//======================================================================
char* pgets (char* s, int n, PODFILE* f)
{
  char* rc = NULL;
  if (f && !peof(f)) {
    switch (f->source) {
    case PODFILE_SOURCE_POD:
      // Reads to different files within the same POD may be interspersed,
      //   so the read pointer must be reset to the next expected byte for
      //   this pod file.
      fseek (f->pFile, f->pos, SEEK_SET);
      rc = fgets (s, n, f->pFile);
      f->pos = ftell (f->pFile);
      break;

    case PODFILE_SOURCE_DISK:
      rc = fgets (s, n, f->pFile);
      break;
    }
  }

  return rc;
}
//======================================================================
//  Mark the position to come back
//======================================================================
void  pMark(PODFILE* f)
{ switch (f->source)  {
    case PODFILE_SOURCE_POD:
      f->back = f->pos;
      return;
    case PODFILE_SOURCE_DISK:
      f->back = ftell(f->pFile);
      return;
 }
return;
}
//======================================================================
//  Back one character on the file
//======================================================================
void  pback(PODFILE *f)
{ if (f->pos > f->offset) f->pos--;
}

//======================================================================
//  seek at position offset
//======================================================================
int pseek (PODFILE* f, long offset, int origin)
{
  int rc = 0;
  if (f) {
    rc = fseek(f->pFile, f->offset + offset, origin);
    // Adjust position
    f->pos = ftell (f->pFile);
  }

  return rc;
}
//======================================================================
//  store current position
//======================================================================
long ptell (PODFILE* f)
{
  long rc = -1;
  if (f) rc = f->pos - f->offset;
  return rc;
}
//======================================================================
//  Rewind the file
//======================================================================
void prewind (PODFILE* f)
{
  if (f) {
    switch (f->source) {
    case PODFILE_SOURCE_POD:
      // Reset the position pointer to the starting offset for this file
      //   within the POD
      f->pos = f->offset;
      fseek (f->pFile, f->pos, SEEK_SET);
      break;

    case PODFILE_SOURCE_DISK:
      rewind (f->pFile);
      break;
    }
  }
}
//======================================================================
//  Close the file
//======================================================================
void pclose (PODFILE* f)
{
  if (f != NULL) {
    switch (f->source) {
    case PODFILE_SOURCE_POD:
      {
        PFSPOD *pPod = f->pPod;
        if (pPod != NULL) {
          // Decrement pod reference count
          if (pPod->refs == 0) {
            // Ref count is already zero; generate warning
            WARNINGLOG ("pclose : Pod reference count underflow");
          } else {
            pPod->refs--;
          }
        }
      }
      break;

    case PODFILE_SOURCE_DISK:
      fclose (f->pFile);
      break;
    }
  }

  delete f;
}


//======================================================================
// Dump the directory of a pod filesystem to disk
//======================================================================
void pfsdump (PFS *pfs, FILE *f)
{
  // Dump out root folders for the filesystem
  fprintf (f, "Pod Filesystem :\n");
  fprintf (f, "  Search pod files first : %c\n", pfs->searchPodFilesFirst ? 'Y' : 'N');
  fprintf (f, "\n");

  long nTotalFiles = 0;

  // Dump out list of disk files
  fprintf (f, "FILE contents:\n");
  std::map<string,string>::iterator idf;
  for (idf=pfs->diskFileList.begin(); idf!=pfs->diskFileList.end(); idf++) {
    fprintf (f, "  %-60s\n", idf->first.c_str());
    nTotalFiles++;
  }
  fprintf (f, "\n");

  // Dump out list of pod files
  fprintf (f, "POD contents:\n");
  std::multimap<string,PFSPODFILE*>::iterator ipf;
  for (ipf=pfs->podFileList.begin(); ipf!=pfs->podFileList.end(); ipf++) {
    PFSPODFILE *pf = ipf->second;
    fprintf (f, "  %-30s %-30s %d %-30s\n",
      ipf->first.c_str(),
      pf->name, pf->priority, pf->pod->name);
    nTotalFiles++;
  }
  fprintf (f, "\n");

  // Dump out list of pods
  fprintf (f, "POD list:\n");
  std::map<string,PFSPOD*>::iterator ip;
  for (ip=pfs->podList.begin(); ip!=pfs->podList.end(); ip++) {
    PFSPOD *pod = ip->second;
    fprintf (f, "Filename  : %s\n", pod->name);
    fprintf (f, "Format    : 0x%08X\n", pod->format);
  }
  fprintf (f, "\n");
  
  // Total number of files
  fprintf (f, "Total files in POD Filesystem : %d\n\n", nTotalFiles);

#ifdef POD_PERFORMANCE_METRICS
  // Calculate and display popen() statistics
  LARGE_INTEGER perf_freq;
  if (!QueryPerformanceFrequency (&perf_freq)) {
    gtfo ("CTimeManager : QueryPerformanceFrequency failed");
  }
  float freq = (float)perf_freq.QuadPart;

  float avgTicks = 0;
  if (pfs->popenSuccess != 0) {
    avgTicks = (float)pfs->popenSuccessTotalTime.QuadPart / (float)pfs->popenSuccess;
  }
  float avg = (avgTicks / freq) * 1000.0f;
  fprintf (f, "Number of successful popen()  : %d\n", pfs->popenSuccess);
  fprintf (f, "Avg. ms per popen() success   : %f\n\n", avg);

  avgTicks = 0;
  if (pfs->popenFailure != 0) {
    avgTicks = (float)pfs->popenFailureTotalTime.QuadPart / (float)pfs->popenFailure;
  }
  avg = (avgTicks / freq) * 1000.0f;
  fprintf (f, "Number of failed popen()   : %d\n", pfs->popenFailure);
  fprintf (f, "Avg. ms per popen() failure: %f\n\n", avg);
#endif
}


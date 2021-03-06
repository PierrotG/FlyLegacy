/*
 * FlyLegacy.h
 *
 * Part of Fly! Legacy project
 *
 * Copyright 2003-2006 Chris Wallace
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

/*! \file FlyLegacy.h
 *  \brief Top-level header file for application
 */

/*
 * Doxygen documentation style
 *
 * Code documentation is generated automatically by Doxygen.  Special comments
 *   should be provided for all classes and methods, standalone functions and
 *   all files.
 * 
 * Note on standardized documentation styles for this project:
 *
 * - All comment blocks should use Qt style notation
 * - All structural commands should use the backslash leading character
 * - All brief descriptions should be grouped into the same comment block as
 *     the detailed description, and use the \brief tag
 * - All detailed descriptions should be grouped into the same comment block as
 *     the brief description
 * - Documentation of class members that follow the member should use //!<
 */
//=================================================================================

//=================================================================================
#ifndef FLYLEGACY_H
#define FLYLEGACY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

// Disable MS Visual C++ warnings
#ifdef _MSC_VER
#pragma warning(disable:4100)    // Formal parameter unused
#pragma warning(disable:4505)    // Unreferenced local function
#pragma warning(disable:4706)    // Assignment in conditional statement
#pragma warning(disable:4710)    // Function not inlined
#pragma warning(disable:4189)    // Local variable intialized but not used
#pragma warning(disable:4201)    // Nameless struct/union
#pragma warning(disable:4244)    // Data conversion, possible loss of data
#pragma warning(disable:4005)    // Formal parameter unused
#ifdef _DEBUG
#pragma warning(disable:4786)    // Browse file name truncation warnings
#endif // _DEBUG
#endif // _MSC_VER

// GLEW must be included before windows.h
#include <GL/glew.h>
#include <GL/glu.h>
#define FREEGLUT_STATIC
#include <GL/glut.h>
// --- JS: Already defined in freeglut.h
//#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#endif // _WIN32

#include "Version.h"
#if defined(_DEBUG) && defined(HAVE_CRTDBG_H)
#include <crtdbg.h>
#endif

#ifdef HAVE_OPAL
// OPAL STUFF FOR WINDOWS AND VISUAL STUDIO 2008
  #define OPAL_USE_DOUBLE
  #include <opal.h>
  //extern opal::JointData gjointData;
#endif

//
// STL Includes
//
#include <string>
#include <pthread.h>
//
// Function portability
//
#ifndef HAVE_STRICMP
#ifdef HAVE_STRCASECMP
#define stricmp strcasecmp
#else
#error No case-independent string comparison (stricmp, strcasecmp) available.
#endif
#endif // HAVE_STRICMP
#ifdef NEED_GLUT_DISABLE_ATEXIT_HACK
#define GLUT_DISABLE_ATEXIT_HACK
#endif // NEED_GLUT_DISABLE_ATEXIT_HACK

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(p)         { if(p) { free (p);       (p)=NULL; } }
#endif

// Generate warning if OpenGL error has occurred
#define CHECK_OPENGL_ERROR \
  { \
    GLenum e = glGetError (); \
    if (e != GL_NO_ERROR)     \
        WARNINGLOG ("OpenGL Error 0x%04X : %s", e, gluErrorString(e)); \
  }

//
// Workarounds for MS Visual Studio 2005 deprecated functions
//
#if _MSC_VER >= 1300
#define stricmp _stricmp
#define itoa _itoa
#endif // MSC_VER >= 1300

//
// Absolute maximum path + filename + extension for a file name
//
#ifndef PATH_MAX
#ifdef _PATH_MAX
#define PATH_MAX _PATH_MAX
#else
#define PATH_MAX 512
#endif
#endif // PATH_MAX

#include "Pod.h"

//
// Maximum size of filename only (no path components)
//
#ifndef FILENAME_MAX
#define FILENAME_MAX 260
#endif

//
// Memory leak marker
//
#ifdef _DEBUG
#define MEMORY_LEAK_MARKER(s) { char *mark = new char[32]; strcpy (mark, s); }
#else
#define MEMORY_LEAK_MARKER(s) {}
#endif

//
// Microsoft Windows headers include standard integer types in header basetsd.h
// If this header was not processed, include standard integer types from stdint.h
//
#ifndef _BASETSD_H_
#include "../Include/stdint.h"
typedef uint8_t    UINT8;
typedef int8_t     INT8;
typedef uint16_t   UINT16;
typedef int16_t    INT16;
typedef uint32_t   UINT32;
typedef int32_t    INT32;
#endif // _BASETSD_H
//==============================================================================
//
// Basic object classes
//==============================================================================
typedef unsigned long Tag;
typedef unsigned long   U_LONG;       // JSDEV*
typedef unsigned int	  U_INT;				// JSDEV*
typedef unsigned short	U_SHORT;			// JSDEV*
typedef unsigned char   U_CHAR;       // JSDEV*
//------------------------------------------------------------------------------
//	JSDEV* add macro to extract character from integer
//------------------------------------------------------------------------------
#define CHAR_N1(X)	((char)(X >> 24))
#define CHAR_N2(X)  ((char)(X >> 16))
#define CHAR_N3(X)  ((char)(X >>  8))
#define CHAR_N4(X)  ((char)(X      ))
//------------------------------------------------------------------------------
//  Number of entries in DbCache for objects on screen
//------------------------------------------------------------------------------
#define NBR_OBJ_DBC 200           
/////////////////////////////////////////////////////////////
//
//  Constants/enums
//
/////////////////////////////////////////////////////////////
//-------Globals terrain constants --------------------------
#define COVERED_RADIUS 140
#define ARCSEC_PER_GLOBE_TILE           (1.296E+6 / 256.0)
#define ARCSEC_PER_QUARTER_GLOBE_TILE   (ARCSEC_PER_GLOBE_TILE / 2.0)
#define ARCSEC_PER_SUPER_TILE           (ARCSEC_PER_QUARTER_GLOBE_TILE / 8.0)
#define ARCSEC_PER_DETAIL_TILE          (ARCSEC_PER_QUARTER_GLOBE_TILE /32.0)
//-------Globals physics constants --------------------------
const float ADJ_DRAG_COEFF = 1.000000f; /// adjustCoeffOfDrag
const float ADJ_LIFT_COEFF = 1.000000f; /// adjustCoeffOfLift
const float ADJ_MMNT_COEFF = 1.000000f; /// adjustCoeffOfMoment
const float ADJ_DHDL_COEFF = 500.0000f; /// adjustDihedralCoeff
const float ADJ_ENGN_THRST = 1.600000f; /// adjustEngineThrust
const float ADJ_PTCH_COEFF = 10.00000f; /// adjustPitchCoeff
const float ADJ_PTCH_MINE  = 1.000000f; /// adjustPitchMine
const float ADJ_ROLL_MINE  = 2.000000f; /// adjustRollMine
const float ADJ_STRG_CONST = 0.125000f; /// adjustSteeringConst
const float ADJ_BRAK_CONST = 1.000000f; /// adjustBrakingConst
const float ADJ_DIFF_CONST = 1.000000f; /// adjustDiffBrakingConst
const float ADJ_SINK_RATE  = 5.000000f; /// adjust crash sink rate
const float ADJ_CMPR_LNGT  = 2.000000f; /// adjust crash compression length 
const float ADJ_POWL_CNST  = 5.000000f; /// adjust crash WHL <powl>
const float ADJ_YAW_MINE   = 2.500000f; /// adjustYawMine
const float ADJ_AILR_FORCE = 4.750000f; /// jsAileronForceCoeff
const float ADJ_AILR_DRAG  = 0.250000f; /// jsAileronDragCoeff
const float ADJ_RUDR_FORCE = 1.650000f; /// jsRudderForceCoeff

const float ADJ_AERO_CENTR = 0.000000f; /// aero center for aeromodel
const float ADJ_ANGL_DAMPG = 0.450000f; /// OPAL angular damping
const float ADJ_GRND_FRCTN = 0.250000f; /// OPAL ground friction
const float ADJ_LINR_DAMPG = 0.017000f; /// OPAl linear damping
const float ADJ_STEP_SIZE  = 0.040000f; /// OPAl step size
const float ADJ_TOTL_DRAG  = 1.000000f; /// OPAL total drag
const int   INITIAL_SPEED  = 1;         /// initial speed y/n
const int   HAS_FAKE_ENG   = 0;         /// has fake engine
const int   WIND_EFFECT    = 1        ; /// wind effect
const int   GRND_EFFECT    = 1        ; /// ground effect
const float INIT_SPD_X     = 0.000000f; /// initial speed x
const float INIT_SPD_Y     = 0.000000f; /// initial speed y
const float INIT_SPD_Z     = 60.00000f; /// initial speed z

const float ADJ_IXX_COEFF  = 1.760000f; /// propeller inertia
const float ADJ_GEAR_RATIO = 0.150000f; /// propeller gear ratio
const float ADJ_GRND_BANK  = 10.00000f; /// fix for ground banking
const float ADJ_WIND_COEFF = 10.00000f; /// wind effect coeff   
const float ADJ_GEAR_DRAG  = 0.975000f; /// drag from gear

//
// Overall application state
//

typedef enum {
  APP_SPLASH_SCREEN,
  APP_TEST,
  APP_INIT,
  APP_GENERATE_SITUATION,
  APP_LOADING_SCREEN,
  APP_LOAD_SITUATION,
  APP_PREPARE,
  APP_SIMULATION,
  APP_EXIT_SCREEN,
  APP_EXIT,
  APP_EXPORT
} EAppState;

enum EMessageDataType
{
  TYPE_NONE = '    ',
  TYPE_CHAR = 'char',
  TYPE_CHARPTR = 'cptr',
  TYPE_INT = 'int ',
  TYPE_INTPTR = 'iptr',
  TYPE_REAL = 'real',
  TYPE_REALPTR = 'rptr',
  TYPE_VOID = 'void',
};

enum EMessageResult
{
  MSG_IGNORED   = 0,    // message was passed over
  MSG_PROCESSED = 1,    // message was processed (set/get) and should stop broadcast
  MSG_USED      = 2,    // message was used indirectly, but should continue to other objects
  MSG_ACK       = 3,    // Message is acknoledged
};

enum EMessageRequest
{
  MSG_UNKNOWN = 'none',
  MSG_GETDATA = 'getd',
  MSG_SETDATA = 'setd',
  MSG_DATACHANGED = 'chan',
  MSG_REFERENCE = 'refr',
  MSG_TRIGGER = 'trig',
  MSG_OBJECTDYING = 'die ',
  MSG_CAMERA = 'aCam'
};

enum EMessageHWType
{
  HW_UNKNOWN  = 0,
  HW_BUS    = 1,
  HW_SWITCH = 2,
  HW_LIGHT  = 3,
  HW_FUSE   = 4,
  HW_STATE  = 5,
  HW_GAUGE  = 6,
  HW_OTHER  = 7,
  HW_CIRCUIT  = 8,
  HW_RADIO  = 9,
  HW_FLAP   = 10,
  HW_HILIFT = 11,
  HW_GEAR   = 12,
  HW_BATTERY  = 13,
  HW_ALTERNATOR = 14,
  HW_ANNUNCIATOR  = 15,
  HW_GENERATOR  = 16,
  HW_CONTACTOR  = 17,
  HW_SOUNDFX  = 18,
  HW_FLASHER  = 19,
  HW_INVERTER = 20,
  HW_PID      = 21,
  HW_ENGINE   = 22,
  HW_WHEEL    = 23,
  HW_SPECIAL  = 24,
  HW_PROP     = 25,
  HW_PISTON   = 26,
  //---To increase, modify the following lines and 
  //  add one item to the hardware table hwTAB in FuiPart.cpp
  HW_MAX      = 27,
  HW_UNITLESS = 101,
  HW_UNBENT = 102,
  HW_SCALE  = 103
};

//
// The following enum lists all allowable object types that can be
//   instantiated by a third-party DLL module.
//

enum EDLLObjectType
{
  TYPE_DLL_GAUGE    = 'gage',
  TYPE_DLL_SYSTEM   = 'subs',
  TYPE_DLL_WORLDOBJECT  = 'wobj',
  TYPE_DLL_WINDOW   = 'wind',
  TYPE_DLL_CAMERA   = 'came',
  TYPE_DLL_CONTROL  = 'cont',
  TYPE_DLL_VIEW   = 'view'
};

//
// All possible types of <wobj> objects
//

enum EFlyObjectType
{
  TYPE_FLY_UNKNOWN        = 0,
  TYPE_FLY_WORLDOBJECT    = 'wobj',
  TYPE_FLY_MODELOBJECT    = 'mobj',
  TYPE_FLY_SIMULATEDOBJECT  = 'sobj',
  TYPE_FLY_VEHICLE        = 'vehi',
  TYPE_FLY_GROUNDVEHICLE  = 'gveh',
  TYPE_FLY_AIRPLANE       = 'plan',
  TYPE_FLY_HELICOPTER     = 'heli'
};
enum EBitmapType
{ TYPE_DUMMY  = 'nnnn',
  TYPE_PBM    = 'pbm',
  TYPE_PBG    = 'pbg ',
  TYPE_BMP    = 'bmp ',
  TYPE_TIF    = 'tif ',
  TYPE_NULL   = 'null',
  TYPE_INVALID  = TYPE_NULL
};

enum EStreamTagResult 
{
  TAG_IGNORED = 0,
  TAG_READ = 1,
  TAG_EXIT = 2
};

enum EClickResult
{
  MOUSE_TRACKING_OFF = 0,
  MOUSE_TRACKING_ON = 1
};

enum ECursorResult
{
  CURSOR_NOT_CHANGED = 0,
  CURSOR_WAS_CHANGED = 1
};

enum ECameraType
{
  CAMERA_INVALID    = 0,
  CAMERA_COCKPIT    = 'cock',
  CAMERA_SPOT       = 'spot',
  CAMERA_OBSERVER   = 'obsr',
  CAMERA_FLYBY      = 'flyb',
  CAMERA_TOWER      = 'towr',
  CAMERA_OVERHEAD   = 'over',
  CAMERA_ORBIT      = 'orbt',
	CAMERA_RABBIT		  = 'rabt'
};

//
// Type of pitot-static port, used in CPitotStaticPort (Subsystems)
//
typedef enum {
  PORT_PITOT,
  PORT_STATIC
} EPortType;

//
// Facing of pitot-static port, used in CPitotStaticPort (Subsystems)
//
typedef enum {
  PORT_LEFT,
  PORT_RIGHT
} EPortFace;


/*
 * Database enumerated types
 *
 * The following enumerated types are all related to the default
 *   databases of airports, runways, navigational aids, etc.
 *
 * Many have their origins in original DAFIF data from which the
 *   databases were constructed.
 */
enum EIlsTypes 
{ ILS_DME         = 0x00000010,   // 16
  ILS_LOCATOR     = 0x00000020,   // 32
  ILS_LOCALIZER   = 0x00000040,   // 64
  ILS_GLIDESLOPE  = 0x00000080,   // 128
  ILS_BACKCOURSE  = 0x00000080,   // 128
  ILS_INNERMARK   = 0x00000200,   // 512
  ILS_MIDDLMARK   = 0x00000400,   // 1024
  ILS_OUTERMARK   = 0x00000800,   // 2048
};
enum ENavaidTypes
{
  NAVAID_TYPE_UNKNOWN   = 0,
  NAVAID_TYPE_VOR     = (1 << 0),
  NAVAID_TYPE_TACAN   = (1 << 2),
  NAVAID_TYPE_NDB     = (1 << 3),
  NAVAID_TYPE_DME     = (1 << 4),
  NAVAID_TYPE_LOCATOR   = (1 << 5),
  NAVAID_TYPE_LOCALIZER   = (1 << 6),
  NAVAID_TYPE_GS      = (1 << 7),
  NAVAID_TYPE_BACKCOURSE    = (1 << 8),
  NAVAID_TYPE_INNERMARKER   = (1 << 9),
  NAVAID_TYPE_MIDDLEMARKER  = (1 << 10),
  NAVAID_TYPE_OUTERMARKER   = (1 << 11),
  NAVAID_TYPE_WAYPOINT    = (1 << 12),
  NAVAID_TYPE_VOT     = (1 << 13),
  NAVAID_TYPE_VORTAC    = (NAVAID_TYPE_VOR | NAVAID_TYPE_TACAN),
  NAVAID_TYPE_VORDME    = (NAVAID_TYPE_VOR | NAVAID_TYPE_DME),
  NAVAID_TYPE_NDBDME    = (NAVAID_TYPE_NDB | NAVAID_TYPE_DME),
  NAVAID_TYPE_ILSDME    = (NAVAID_TYPE_LOCALIZER | NAVAID_TYPE_DME)
};

enum ENavaidClasses
{
  NAVAID_CLASS_UNKNOWN    = 0,
  NAVAID_CLASS_HIGH       = 1,
  NAVAID_CLASS_LOW        = 2,
  NAVAID_CLASS_TERMINAL   = 3,
  NAVAID_CLASS_MEDIUMHOMING = 4,
  NAVAID_CLASS_HIGHHOMING   = 5,
  NAVAID_CLASS_ANTICIPATED  = 6
};

enum ENavaidUsage 
{
  NAVAID_USAGE_UNKNOWN  = 0,
  NAVAID_USAGE_HIGH = 1,
  NAVAID_USAGE_LOW  = 2,
  NAVAID_USAGE_BOTH = 3,
  NAVAID_USAGE_TERMINAL = 4,
  NAVAID_USAGE_RNAV = 5
};

enum ECommTypes
{
  COMM_UNKNOWN= 0,
  COMM_TOWER = (1 << 0),
  COMM_CLEARANCEDELIVERY = (1 << 1),
  COMM_GROUNDCONTROL = (1 << 2),
  COMM_APPROACHCONTROL = (1 << 3),
  COMM_DEPARTURECONTROL = (1 << 4),
  COMM_FLIGHTSERVICESTATION = (1 << 5),
  COMM_CENTER = (1 << 6),
  COMM_ATIS = (1 << 7),
  COMM_CTAF = (1 << 8),
  COMM_MULTICOM = (1 << 9),
  COMM_EMERGENCY = (1 << 10),
  COMM_ALL = (-1),
};

enum ENavWaypointUsage 
{
  WAYPOINT_USAGE_UNKNOWN = 0,
  WAYPOINT_USAGE_HIGH = 1,
  WAYPOINT_USAGE_LOW = 2,
  WAYPOINT_USAGE_BOTH = 3,
  WAYPOINT_USAGE_TERMINAL = 4,
  WAYPOINT_USAGE_HIGHALTRNAV = 5
};

enum ENavWaypointTypes
{
  WAYPOINT_TYPE_UNNAMED = (1 << 0),
  WAYPOINT_TYPE_NDB = (1 << 1),
  WAYPOINT_TYPE_NAMED = (1 << 2),
  WAYPOINT_TYPE_OFFROUTE = (1 << 3)
};

enum ETunedTypes
{
  TUNED_NONE = 0,
  TUNED_NAVAID = (1 << 0),
  TUNED_ILS = (1 << 1),
  TUNED_COMM = (1 << 2),
};

enum EAirportTypes
{
  AIRPORT_TYPE_UNKNOWN = 0,
  AIRPORT_TYPE_AIRPORT = (1 << 1),
  AIRPORT_TYPE_HELIPORT = (1 << 2),
  AIRPORT_TYPE_SEAPLANEBASE = (1 << 3),
  AIRPORT_TYPE_BALLOONPORT = (1 << 4),
  AIRPORT_TYPE_GLIDERPORT = (1 << 5),
  AIRPORT_TYPE_STOLPORT = (1 << 6),
  AIRPORT_TYPE_ULTRALIGHT = (1 << 7)
};

enum EAirportOwnership
{
  AIRPORT_OWNERSHIP_UNKNOWN = 0,
  AIRPORT_OWNERSHIP_PUBLIC = 1,
  AIRPORT_OWNERSHIP_PRIVATE = 2,
  AIRPORT_OWNERSHIP_AIRFORCE = 3,
  AIRPORT_OWNERSHIP_NAVY = 4,
  AIRPORT_OWNERSHIP_ARMY = 5
};

enum EAirportUsage
{
  AIRPORT_USAGE_UNKNOWN = 0,
  AIRPORT_USAGE_PUBLIC = 1,
  AIRPORT_USAGE_PRIVATE = 2
};

enum EAirportFrameService
{
  AIRPORT_FRAMESERVICE_UNKNOWN = 0,
  AIRPORT_FRAMESERVICE_NONE = 1,
  AIRPORT_FRAMESERVICE_MINOR = 2,
  AIRPORT_FRAMESERVICE_MAJOR = 3
};

enum EAirportEngineService
{
  AIRPORT_ENGINESERVICE_UNKNOWN = 0,
  AIRPORT_ENGINESERVICE_NONE = 0,
  AIRPORT_ENGINESERVICE_MINOR = 1,
  AIRPORT_ENGINESERVICE_MAJOR = 2
};

enum EAirportOxygen
{
  AIRPORT_OXYGEN_UNKNOWN = 0,
  AIRPORT_OXYGEN_NONE = 1,
  AIRPORT_OXYGEN_LOW = 2,
  AIRPORT_OXYGEN_HIGH = 3,
  AIRPORT_OXYGEN_HIGHLOW = 4
};

enum EAirportLensColor
{
  AIRPORT_BEACON_UNKNOWN = 0,
  AIRPORT_BEACON_NONE = 1,
  AIRPORT_BEACON_CLEARGREEN = 2,
  AIRPORT_BEACON_CLEARYELLOW = 3,
  AIRPORT_BEACON_CLEARGREENYELLOW = 4,
  AIRPORT_BEACON_SPLITCLEARGREEN = 5,
  AIRPORT_BEACON_YELLOW = 6,
  AIRPORT_BEACON_GREEN = 7
};

enum EAirportRegions
{
  AIRPORT_REGION_UNKNOWN = 0,
  AIRPORT_REGION_FAAALASKA = 1,
  AIRPORT_REGION_FAACENTRAL = 2,
  AIRPORT_REGION_FAAEASTERN = 3,
  AIRPORT_REGION_FAAGREATLAKES = 4,
  AIRPORT_REGION_FAAINTERNATIONAL = 5,
  AIRPORT_REGION_FAANEWENGLAND = 6,
  AIRPORT_REGION_FAANORTHWESTMOUNTAIN = 7,
  AIRPORT_REGION_FAASOUTHERN = 8,
  AIRPORT_REGION_FAASOUTHWEST = 9,
  AIRPORT_REGION_FAAWESTERNPACIFIC = 10
};

enum EAirportFuelTypes
{
  AIRPORT_FUEL_80 = (1 << 1),
  AIRPORT_FUEL_100 = (1 << 2),
  AIRPORT_FUEL_100LL = (1 << 3),
  AIRPORT_FUEL_115 = (1 << 4),
  AIRPORT_FUEL_JETA = (1 << 5),
  AIRPORT_FUEL_JETA1 = (1 << 6),
  AIRPORT_FUEL_JETA1PLUS = (1 << 7),
  AIRPORT_FUEL_JETB = (1 << 8),
  AIRPORT_FUEL_JETBPLUS = (1 << 9),
  AIRPORT_FUEL_AUTOMOTIVE = (1 << 10)
};

enum EAirportBasedAircraft
{
  AIRPORT_AIRCRAFT_SINGLEENGINE = (1 << 1),
  AIRPORT_AIRCRAFT_MULTIENGINE = (1 << 2),
  AIRPORT_AIRCRAFT_JETENGINE = (1 << 3),
  AIRPORT_AIRCRAFT_HELICOPTER = (1 << 4),
  AIRPORT_AIRCRAFT_GLIDERS = (1 << 5),
  AIRPORT_AIRCRAFT_MILITARY = (1 << 6),
  AIRPORT_AIRCRAFT_ULTRALIGHT = (1 << 7)
};

enum EAirportScheduleMonth
{
  AIRPORT_MONTH_JANUARY = (1 << 1),
  AIRPORT_MONTH_FEBRUARY = ( 1 << 2),
  AIRPORT_MONTH_MARCH = (1 << 3),
  AIRPORT_MONTH_APRIL = (1 << 4),
  AIRPORT_MONTH_MAY = (1 << 5),
  AIRPORT_MONTH_JUNE = (1 << 6),
  AIRPORT_MONTH_JULY = (1 << 7),
  AIRPORT_MONTH_AUGUST = (1 << 8),
  AIRPORT_MONTH_SEPTEMBER = (1 << 9),
  AIRPORT_MONTH_OCTOBER = (1 << 10),
  AIRPORT_MONTH_NOVEMBER = (1 << 11),
  AIRPORT_MONTH_DECEMBER = (1 << 12)
};

enum EAirportScheduleDay
{
  AIRPORT_DAY_SUNDAY = (1 << 16),
  AIRPORT_DAY_MONDAY = (1 << 17),
  AIRPORT_DAY_TUESDAY = (1 << 18),
  AIRPORT_DAY_WEDNESDAY = (1 << 19),
  AIRPORT_DAY_THURSDAY = (1 << 20),
  AIRPORT_DAY_FRIDAY = (1 << 21),
  AIRPORT_DAY_SATURDAY = (1 << 22)
};

enum EAirportScheduleRange
{
  AIRPORT_SCHEDULE_ALLDAY = (1 << 28),
  AIRPORT_SCHEDULE_SPECIFICTIME = (1 << 29),
  AIRPORT_SCHEDULE_SUNRISE = (1 << 30),
  AIRPORT_SCHEDULE_SUNSET = (1 << 31)
};

enum EAirportLightingControl
{
  AIRPORT_LIGHTING_24HOURS = (1 << 1),
  AIRPORT_LIGHTING_ATCCONTROLLED = (1 << 2),
  AIRPORT_LIGHTING_RADIOCONTROLLED = (1 << 3),
  AIRPORT_LIGHTING_RADIOREQUIRED = (1 << 4),
  AIRPORT_LIGHTING_PHONEREQUIRED = (1 << 5)
};

enum EAirportAirspace
{
  AIRPORT_AIRSPACE_A = 1,
  AIRPORT_AIRSPACE_B = 2,
  AIRPORT_AIRSPACE_C = 3,
  AIRPORT_AIRSPACE_D = 4,
  AIRPORT_AIRSPACE_E = 5,
  AIRPORT_AIRSPACE_G = 6
};

//===================================================================================
//  GLOBAL INDICES FOR DATABANK
//===================================================================================
//  Radio com Type:  Associated with comTAB in main.cpp
//-----------------------------------------------------------------------------------
enum RD_COM { COM_ZR  = 0,                    // Unknown
              COM_AT  = 1,                    // ATIS
              COM_TW  = 2,                    // TOWER
              COM_CL  = 3,                    // CLEARANCE
              COM_GR  = 4,                    // GROUND
              COM_AP  = 5,                    // APPROACH
              COM_DP  = 6,                    // DEPARTURE
              COM_SV  = 7,                    // SERVICE
              COM_CR  = 8,                    // CENTER
              COM_CF  = 9,                    // CTAF
              COM_MU  = 10,                   // MULTICOM
              COM_EM  = 11,                   // EMERGENCY
              COMDIM  = 12,                   //-Dimension --------
};
//------Define dead zone for localizer arround 90� offset -------------
	#define DEADZONE 7.0f
//====================================================================================
//
// User Interface enumerated types
//
// ??? Will these still be appropriate for an implementation using
//       PLIB?
//====================================================================================

enum EMouseCursorStyle
{
  CURSOR_ARROW = 0,
  CURSOR_CROSS = 1,
  CURSOR_FINGER = 2,
  CURSOR_HAND = 3,
  CURSOR_MOVE = 4,
  CURSOR_SIZE_H = 5,
  CURSOR_SIZE_HV = 6,
  CURSOR_SIZE_V = 7,
  CURSOR_FLIP_DOWN = 8,
  CURSOR_FLIP_UP = 9,
  CURSOR_FLIP_LEFT = 10,
  CURSOR_FLIP_RIGHT = 11,
  CURSOR_TURN_LEFT = 12,
  CURSOR_TURN_RIGHT = 13,
  CURSOR_SLIDE = 14
};

enum EMouseButton
{
  MOUSE_BUTTON_LEFT   = (1 << 0),
  MOUSE_BUTTON_RIGHT  = (1 << 1),
  MOUSE_BUTTON_MIDDLE = (1 << 2)
};

enum EWindowRegion
{ 
  IN_WINDOW_CONTENT     = 1, 
  IN_WINDOW_DRAG        = 2, 
  IN_WINDOW_CLOSE       = 3, 
  IN_WINDOW_SIZE        = 4, 
  IN_WINDOW_SIZE_RIGHT  = 5, 
  IN_WINDOW_SIZE_LEFT   = 6, 
  IN_WINDOW_SIZE_BOTTOM = 7,
  IN_WINDOW_HIDE        = 8
};

enum EWindowFlags
{
  WINDOW_HAS_TITLEBAR = (1 << 0),
  WINDOW_HAS_CLOSEBOX = (1 << 1),
  WINDOW_HAS_SIZEBOX  = (1 << 2),
  WINDOW_HAS_BORDER = (1 << 3),
  WINDOW_IS_MOVEABLE  = (1 << 4),
  WINDOW_IS_RESIZEABLE  = (1 << 5)
};

enum EWindowType
{
  WINDOW_CAMERA   = 1,
  WINDOW_RASTER_MAP = 2,
  WINDOW_VECTOR_MAP = 3,
  WINDOW_CHECKLIST  = 4,
  WINDOW_GPS    = 5,
  WINDOW_MINIPANEL  = 6,
  WINDOW_AXIS   = 7
}; 

enum EFPEntryType
{
  ENTRY_AIRPORT = 1,
  ENTRY_NAVAID  = 2,
  ENTRY_WAYPOINT  = 3,
  ENTRY_USER  = 4
};

enum ETimeOfDay
{
  TIME_DAWN = 1,
  TIME_DAYTIME  = 2,
  TIME_DUSK = 3,
  TIME_NIGHTTIME  = 4
};

enum EJoystickAxis
{
  X_AXIS_INDEX    = 0,
  Y_AXIS_INDEX    = 1,
  RUDDER_AXIS_INDEX = 2,
  TRIM_AXIS_INDEX   = 3,
  RBRAKE_AXIS_INDEX = 4,
  LBRAKE_AXIS_INDEX = 5,
  THROTTLE_AXIS_INDEX = 6,
  THROTTLE1_AXIS_INDEX  = THROTTLE_AXIS_INDEX,
  THROTTLE2_AXIS_INDEX  = 7,
  THROTTLE3_AXIS_INDEX  = 8,
  THROTTLE4_AXIS_INDEX  = 9,
  MIXTURE_AXIS_INDEX  = 10,
  MIXTURE1_AXIS_INDEX = MIXTURE_AXIS_INDEX,
  MIXTURE2_AXIS_INDEX = 11,
  MIXTURE3_AXIS_INDEX = 12,
  MIXTURE4_AXIS_INDEX = 13,
  PROP_AXIS_INDEX   = 14,
  PROP1_AXIS_INDEX  = PROP_AXIS_INDEX,
  PROP2_AXIS_INDEX  = 15,
  PROP3_AXIS_INDEX  = 16,
  PROP4_AXIS_INDEX  = 17
};

enum ERadioType
{
  RADIO_COM = (1 << 0),
  RADIO_NAV = (1 << 1),
  RADIO_DME = (1 << 2),
  RADIO_XPDR  = (1 << 3),
  RADIO_AP  = (1 << 4),
  RADIO_GPS = (1 << 5),
  RADIO_ADF = (1 << 6),
  RADIO_AUDIO = (1 << 7),
  RADIO_HF  = (1 << 8)
};

enum EFreqType
{
  FREQ_ACTIVE = 0,
  FREQ_STANDBY  = 1
};

enum 
{ RAY_COLLIDE_GROUND  = 0x01, 
  RAY_COLLIDE_MODELS  = 0x02, 
  RAY_COLLIDE_CLOUDS  = 0x04,
  RAY_COLLIDE_ALL   = 0xFF, 
  RAY_COLLIDE_SOLID_OBJECTS = (RAY_COLLIDE_ALL & ~RAY_COLLIDE_CLOUDS)
};

enum EFileSearchLocation {

  SEARCH_PODS = (1 << 0),
  SEARCH_DISK = (1 << 1),
  SEARCH_ALL_ = (SEARCH_PODS | SEARCH_DISK)
};

enum EKeyboardKeys
{
  KB_KEY_ESC    = 1,
  KB_KEY_1      = 2,
  KB_KEY_2      = 3,
  KB_KEY_3      = 4,
  KB_KEY_4      = 5,
  KB_KEY_5      = 6,
  KB_KEY_6      = 7,
  KB_KEY_7      = 8,
  KB_KEY_8      = 9,
  KB_KEY_9      = 10,
  KB_KEY_0      = 11,
  KB_KEY_MINUS  = 12,
  KB_KEY_EQUALS = 13,
  KB_KEY_BACK   = 14,
  KB_KEY_TAB    = 15,
  KB_KEY_Q      = 16,
  KB_KEY_W      = 17,
  KB_KEY_E      = 18,
  KB_KEY_R      = 19,
  KB_KEY_T      = 20,
  KB_KEY_Y      = 21,
  KB_KEY_U      = 22,
  KB_KEY_I      = 23,
  KB_KEY_O      = 24,
  KB_KEY_P      = 25,
  KB_KEY_FORWARD_BRACKET    = 26,
  KB_KEY_REVERSE_BRACKET    = 27,
  KB_KEY_ENTER      = 28,
  KB_KEY_LCTRL      = 29,
  KB_KEY_A      = 30,
  KB_KEY_S      = 31,
  KB_KEY_D      = 32,
  KB_KEY_F      = 33,
  KB_KEY_G      = 34,
  KB_KEY_H      = 35,
  KB_KEY_J      = 36,
  KB_KEY_K      = 37,
  KB_KEY_L      = 38,
  KB_KEY_SEMI_COLON   = 39,
  KB_KEY_SINGLE_QUOTE   = 40,
  KB_KEY_REVERSE_SINGLE_QUOTE = 41,
  KB_KEY_LSHIFT     = 42,
  KB_KEY_BACKSLASH    = 43,
  KB_KEY_Z      = 44,
  KB_KEY_X      = 45,
  KB_KEY_C      = 46,
  KB_KEY_V      = 47,
  KB_KEY_B      = 48,
  KB_KEY_N      = 49,
  KB_KEY_M      = 50,
  KB_KEY_COMMA      = 51,
  KB_KEY_PERIOD     = 52,
  KB_KEY_SLASH      = 53,
  KB_KEY_RSHIFT     = 54,
  KB_KEY_STAR     = 55,
  KB_KEY_LALT     = 56,
  KB_KEY_SPACE      = 57,
  KB_KEY_CAPSLOCK     = 58,
  KB_KEY_F1     = 59,
  KB_KEY_F2     = 60,
  KB_KEY_F3     = 61,
  KB_KEY_F4     = 62,
  KB_KEY_F5     = 63,
  KB_KEY_F6     = 64,
  KB_KEY_F7     = 65,
  KB_KEY_F8     = 66,
  KB_KEY_F9     = 67,
  KB_KEY_F10      = 68,
  KB_KEY_NUMLOCK      = 69,
  KB_KEY_SCROLLLOCK   = 70,
  KB_KEY_HOME     = 71,
  KB_KEY_UP     = 72,
  KB_KEY_PGUP     = 73,
  KB_KEY_KEYPAD_MINUS   = 74,
  KB_KEY_LEFT     = 75,
  KB_KEY_CENTER     = 76,
  KB_KEY_RIGHT      = 77,
  KB_KEY_KEYPAD_PLUS    = 78,
  KB_KEY_END      = 79,
  KB_KEY_DOWN     = 80,
  KB_KEY_PGDN     = 81,
  KB_KEY_INSERT     = 82,
  KB_KEY_DEL      = 83,
  KB_KEY_F11      = 87,
  KB_KEY_F12      = 88,
  KB_KEY_KEYPAD_ENTER   = 284,
  KB_KEY_RCTRL      = 285,
  KB_KEY_KEYPAD_SLASH   = 309,
  KB_KEY_RALT     = 312,
  KB_KEY_EXT_NUMLOCK    = 325,
  KB_KEY_GRAY_HOME    = 327,
  KB_KEY_GRAY_UP      = 328,
  KB_KEY_GRAY_PGUP    = 329,
  KB_KEY_GRAY_LEFT    = 331,
  KB_KEY_GRAY_RIGHT   = 333,
  KB_KEY_GRAY_END     = 335,
  KB_KEY_GRAY_DOWN    = 336,
  KB_KEY_GRAY_PGDN    = 337,
  KB_KEY_GRAY_INS     = 338,
  KB_KEY_GRAY_DEL     = 339,
  KB_KEY_META     = 340
};

enum EKeyboardModifiers
{
  KB_MODIFIER_NONE    = (0 << 0),
  KB_MODIFIER_CTRL    = (1 << 0),
  KB_MODIFIER_ALT     = (1 << 1),
  KB_MODIFIER_SHIFT   = (1 << 2),
  KB_MODIFIER_META    = (1 << 3)
};

enum EGaugeHilite
{
  GAUGE_HILITE_NONE = 0,
  GAUGE_HILITE_ARROW  = (1 << 0),
  GAUGE_HILITE_BOX  = (1 << 1),
  GAUGE_HILITE_CIRCLE = (1 << 2),
  GAUGE_HILITE_BLINK  = (1 << 8)
};

enum EWeatherObstructionTypes
{
  WEATHER_INTENSITY_VICINITY        = 0x00000001, // Weather intensity (bits 0-3)
  WEATHER_INTENSITY_LIGHT     = 0x00000002,
  WEATHER_INTENSITY_MODERATE    = 0x00000004,
  WEATHER_INTENSITY_HEAVY     = 0x00000008,
  WEATHER_DESCRIPTOR_SHALLOW    = 0x00000010, // Weather description (4-11)
  WEATHER_DESCRIPTOR_PARTIAL    = 0x00000020,
  WEATHER_DESCRIPTOR_PATCHES    = 0x00000040,
  WEATHER_DESCRIPTOR_LOW_DRIFTING   = 0x00000080,
  WEATHER_DESCRIPTOR_BLOWING    = 0x00000100,
  WEATHER_DESCRIPTOR_SHOWERS    = 0x00000200,
  WEATHER_DESCRIPTOR_THUNDERSTORMS  = 0x00000400,
  WEATHER_DESCRIPTOR_FREEZING   = 0x00000800,
  WEATHER_PHENOMENA_DRIZZLE   = 0x00001000, // Weather phenomena (bits 12-31)
  WEATHER_PHENOMENA_RAIN      = 0x00002000,
  WEATHER_PHENOMENA_SNOW      = 0x00004000,
  WEATHER_PHENOMENA_SNOW_GRAINS   = 0x00008000,
  WEATHER_PHENOMENA_ICE_CRYSTALS    = 0x00010000,
  WEATHER_PHENOMENA_ICE_PELLETS   = 0x00020000,
  WEATHER_PHENOMENA_HAIL      = 0x00040000,
  WEATHER_PHENOMENA_SMALL_HAIL_OR_SNOW  = 0x00080000,
  WEATHER_PHENOMENA_FOG     = 0x00100000,
  WEATHER_PHENOMENA_UNIDENTIFIED    = 0x00200000,
  WEATHER_PHENOMENA_VOLCANIC_ASH    = 0x00400000,
  WEATHER_PHENOMENA_SQUALL    = 0x00800000,
  WEATHER_PHENOMENA_WIDESPREAD_DUST = 0x01000000,
  WEATHER_PHENOMENA_SPRAY     = 0x02000000,
  WEATHER_PHENOMENA_SAND      = 0x04000000,
  WEATHER_PHENOMENA_DUST_SAND_WHIRLS  = 0x08000000,
  WEATHER_PHENOMENA_FUNNEL_CLOUD    = 0x10000000,
  WEATHER_PHENOMENA_SAND_STORM    = 0x20000000,
  WEATHER_PHENOMENA_DUST_STORM    = 0x40000000
};

enum EWindsAloftLayer
{
  WEATHER_WINDS_ALOFT_3000 = 0,
  WEATHER_WINDS_ALOFT_6000 = 1,
  WEATHER_WINDS_ALOFT_9000 = 2,
  WEATHER_WINDS_ALOFT_12000 = 3,
  WEATHER_WINDS_ALOFT_15000 = 4,
  WEATHER_WINDS_ALOFT_18000 = 5,
  WEATHER_WINDS_ALOFT_FL18 = WEATHER_WINDS_ALOFT_18000,
  WEATHER_WINDS_ALOFT_21000 = 6,
  WEATHER_WINDS_ALOFT_FL21 = WEATHER_WINDS_ALOFT_21000,
  WEATHER_WINDS_ALOFT_24000 = 7,
  WEATHER_WINDS_ALOFT_FL24 = WEATHER_WINDS_ALOFT_24000,
  WEATHER_WINDS_ALOFT_27000 = 8,
  WEATHER_WINDS_ALOFT_FL27 = WEATHER_WINDS_ALOFT_27000,
  WEATHER_WINDS_ALOFT_30000 = 9,
  WEATHER_WINDS_ALOFT_FL30 = WEATHER_WINDS_ALOFT_30000,
  WEATHER_WINDS_ALOFT_33000 = 10,
  WEATHER_WINDS_ALOFT_FL33 = WEATHER_WINDS_ALOFT_33000,
  WEATHER_WINDS_ALOFT_36000 = 11,
  WEATHER_WINDS_ALOFT_FL36 = WEATHER_WINDS_ALOFT_36000,
  WEATHER_WINDS_ALOFT_39000 = 12,
  WEATHER_WINDS_ALOFT_FL39 = WEATHER_WINDS_ALOFT_39000,
  WEATHER_WINDS_ALOFT_42000 = 13,
  WEATHER_WINDS_ALOFT_FL42 = WEATHER_WINDS_ALOFT_42000,
  WEATHER_WINDS_ALOFT_60000 = 14,
  WEATHER_WINDS_ALOFT_FL60 = WEATHER_WINDS_ALOFT_60000,
  WEATHER_WINDS_ALOFT_MAX
};

enum EWeatherSkyTypes
{
  WEATHER_SKY_UNKNOWN = 0,
  WEATHER_SKY_CLEAR = 1,
  WEATHER_SKY_FEW = 2,
  WEATHER_SKY_SCATTERED = 3,
  WEATHER_SKY_BROKEN = 4,
  WEATHER_SKY_OVERCAST = 5,
  WEATHER_SKY_FOG = 6
};

enum EWheelStatus
{
  WHEELS_NOTONGROUND = 0,
  WHEELS_SOMEONGROUND = 1,
  WHEELS_ALLONGROUND = 2,
  WHEELS_ONEONGROUND = 3,
  WHEELS_INWATER = 4
};

enum EGroundTypes 
{
  GROUND_CONCRETE = 1,
  GROUND_ASPHALT  = 2,
  GROUND_TURF     = 3,
  GROUND_DIRT     = 4,
  GROUND_GRAVEL   = 5,
  GROUND_METAL    = 6,
  GROUND_SAND     = 7,
  GROUND_WOOD     = 8,
  GROUND_WATER    = 9,
  GROUND_MATS     = 10,
  GROUND_SNOW     = 11,
  GROUND_ICE      = 12,
  GROUND_GROOVED  = 13,
  GROUND_TREATED  = 14
};

enum ETerrainType
{
  TERRAIN_WATER_OCEAN = 12,
  TERRAIN_ICE_CRACKED_GLACIERS = 13,
  TERRAIN_SNOW = 59,
  TERRAIN_MOUNTAIN_FOREST_CALIFORNIA = 60,
  TERRAIN_TOWNS_MIDEAST = 61,
  TERRAIN_CITY_USA = 101,
  TERRAIN_GRASS_SCRUB_LIGHT = 102,
  TERRAIN_FOREST_MIXED_SCRUB = 104,
  TERRAIN_FOREST_MIXED = 105,
  TERRAIN_MOUNTAIN_FOREST_GREEN = 106,
  TERRAIN_GRASS_SCRUB_LIGHT_2 = 107,
  TERRAIN_DESERT_BARREN = 108,
  TERRAIN_TUNDRA_UPLAND = 109,
  TERRAIN_GRASS_SCRUB_LIGHT_3 = 110,
  TERRAIN_DESERT_SCRUB_LIGHT = 111,
  TERRAIN_SNOW_HARD_PACKED = 112,
  TERRAIN_EURO_CITY_ENGLAND_HEAVY = 113,
  TERRAIN_SHRUB_EVERGREEN = 116,
  TERRAIN_SHRUB_DECIDUOUS = 117,
  TERRAIN_CITY_HEAVY_ASIAN = 118,
  TERRAIN_FOREST_EVERGREEN_W_DARK_FIELDS = 119,
  TERRAIN_RAINFOREST_TROPICAL_MIXED = 120,
  TERRAIN_CONIFER_BOREAL_FOREST = 121,
  TERRAIN_COOL_CONIFER_FOREST = 122,
  TERRAIN_EURO_FOREST_TOWNS = 123,
  TERRAIN_FOREST_RIDGES_MIXED = 124,
  TERRAIN_EURO_FOREST_DARK_TOWNS = 125,
  TERRAIN_SUBURB_USA_BROWN = 126,
  TERRAIN_CONIFER_FOREST = 127,
  TERRAIN_FOREST_YELLOWED_AFRICAN = 128,
  TERRAIN_SEASONAL_RAINFOREST = 129,
  TERRAIN_COOL_CROPS = 130,
  TERRAIN_CROPS_TOWN = 131,
  TERRAIN_DRY_TROPICAL_WOODS = 132,
  TERRAIN_RAINFOREST_TROPICAL = 133,
  TERRAIN_RAINFOREST_DEGRADED = 134,
  TERRAIN_FARM_USA_GREEN = 135,
  TERRAIN_RICE_PADDIES = 136,
  TERRAIN_HOT_CROPS = 137,
  TERRAIN_FARM_USA_MIXED = 138,
  TERRAIN_EURO_FARM_FULL_MIXED = 139,
  TERRAIN_COOL_GRASSES_W_SHRUBS = 140,
  TERRAIN_HOT_GRASSES_W_SHRUBS = 141,
  TERRAIN_COLD_GRASSLAND = 142,
  TERRAIN_SAVANNA_WOODS = 143,
  TERRAIN_SWAMP_BOG = 144,
  TERRAIN_MARSH_WETLAND = 145,
  TERRAIN_MEDITERRANEAN_SCRUB = 146,
  TERRAIN_DRY_WOODY_SCRUB = 147,
  TERRAIN_DRY_EVERGREEN_WOODS = 148,
  TERRAIN_SUBURB_USA = 149,
  TERRAIN_DESERT_SAND_DUNES = 150,
  TERRAIN_SEMI_DESERT_SHRUB = 151,
  TERRAIN_SEMI_DESERT_SAGE = 152,
  TERRAIN_MOUNTAIN_ROCKY_TUNDRA_SNOW = 153,
  TERRAIN_MIXED_RAINFOREST = 154,
  TERRAIN_COOL_FIELDS_AND_WOODS = 155,
  TERRAIN_FOREST_AND_FIELD = 156,
  TERRAIN_COOL_FOREST_AND_FIELD = 157,
  TERRAIN_FIELDS_AND_WOODY_SAVANNA = 158,
  TERRAIN_THORN_SCRUB = 159,
  TERRAIN_SMALL_LEAF_MIXED_FOREST = 160,
  TERRAIN_MIXED_BOREAL_FOREST = 161,
  TERRAIN_NARROW_CONIFERS = 162,
  TERRAIN_WOODED_TUNDRA = 163,
  TERRAIN_HEATH_SCRUB = 164,
  TERRAIN_EURO_FULL_GREEN_3 = 165,
  TERRAIN_AUSTRALIAN_CITY = 166,
  TERRAIN_CITY_HEAVY_BRAZIL = 167,
  TERRAIN_CITY_HEAVY_USA = 168,
  TERRAIN_POLAR_DESERT = 169,
  TERRAIN_EURO_TOWNS = 170,
  TERRAIN_CITY_MIDDLE_EASTERN_HEAVY = 171,
  TERRAIN_TUNDRA_BOG = 172,
  TERRAIN_EURO_FARM_FULL_MIXED_2 = 176,
  TERRAIN_TOWNS_ASIAN = 177,
  TERRAIN_ICE_CRACKED_SNOW = 178,
  TERRAIN_EURO_FARM_FOREST = 179,
  TERRAIN_FARM_USA_BROWN = 180,
  TERRAIN_FARM_MIDDLE_EASTERN_W_FOREST = 181,
  TERRAIN_DESERT_BRUSH_REDDISH = 182,
  TERRAIN_FARM_MIDDLE_EASTERN = 183,
  TERRAIN_EURO_FARM_FULL_MIXED_3 = 184,
  TERRAIN_EURO_FARM_FULL_GREEN_2 = 185,
  TERRAIN_MOUNTAIN_ROCKY_SNOW = 186,
  TERRAIN_MOUNTAIN_FOREST_LIGHT = 187,
  TERRAIN_GRASS_MEADOW = 188,
  TERRAIN_MOIST_EUCALYPTUS = 189,
  TERRAIN_RAINFOREST_HAWAIIAN = 190,
  TERRAIN_WOODY_SAVANNA = 191,
  TERRAIN_BROADLEAF_CROPS = 192,
  TERRAIN_GRASS_CROPS = 193,
  TERRAIN_CROPS_GRASS_SHRUBS = 194,
  TERRAIN_GRASSLAND = 225,
  TERRAIN_DESERT_SAVANAH_AFRICAN = 255
};

enum ETerrainSubdivision
{
  TERRAIN_SUBDIVISION_WORLD               =  0,
  TERRAIN_SUBDIVISION_QTR                 =  5,
  TERRAIN_SUBDIVISION_GLOBE_TILE          =  8,
  TERRAIN_SUBDIVISION_QUARTER_GLOBE_TILE  =  9,
  TERRAIN_SUBDIVISION_SUPER_TILE          = 12,
  TERRAIN_SUBDIVISION_DETAIL_TILE         = 14
};

enum EGestaltFlags
{
  GESTALT_OBJECT_NEEDS_TO_DRAW  = (1 << 0),
  GESTALT_OBJECT_NEEDS_TIME_SLICE = (1 << 1),
  GESTALT_OBJECT_HAS_MODEL  = (1 << 2),
  GESTALT_OBJECT_HAS_CAMERA = (1 << 3),
  GESTALT_OBJECT_CAN_ACTIVATE = (1 << 4),
  GESTALT_OBJECT_HAS_CONTROLS = (1 << 5),
  GESTALT_OBJECT_UNDER_USER_CONTROL = (1 << 6), // set by Fly!; don't modify!
  GESTALT_PRIMARY_USER_OBJECT = (1 << 7), // set by Fly!; don't modify
  GESTALT_STATIC_SCENERY_OBJECT = (1 << 8), // objects saved to scenery database
  GESTALT_DYNAMIC_SCENERY_OBJECT  = (1 << 9), // objects NOT saved to scenery database
  GESTALT_DRAW_OBJECT_NAME  = (1 << 10),  // draw popup name when pointed at with mouse
  GESTALT_OBJECT_IS_CLOUD   = (1 << 11),
  GESTALT_DO_NOT_AUTO_SNAP  = (1 << 12),  // snap object to the ground automatically 
  GESTALT_OBJECT_IS_TOWER   = (1 << 13),  // used to designate for tower camera
  GESTALT_OBJECT_ANIMATED   = (1 << 14),  // animated scenery (cars, trucks, boats, etc.)
  GESTALT_OBJECT_COLLISIONS = (1 << 15),  // collision detection with this object (default ON)
  GESTALT_DISTANCE_HIDING   = (1 << 16),  // object hidden when eye past a threshold distance
  GESTALT_SINK_UNDERGROUND  = (1 << 17),  // sink model by 8-10 feet underground (for raised sidewalks)
  GESTALT_DRAW_NIGHT_LIGHTS = (1 << 18),  // force night lights on models
  GESTALT_DRAW_SHADOW   = (1 << 19),  // draw shadows for this object??
  GESTALT_OBJECT_GENERATED  = (1 << 20),  // object was generated by an automated external source (DLL)
  GESTALT_NO_Z_BUFFER   = (1 << 21),  // object doesn't use Z-buffer, draws after taxiways and runways, but before models (ground striping)
  GESTALT_Z_SORTED    = (1 << 22),  // object must sort in Z before drawing (back to front)
  GESTALT_NO_Z_BUFFER_UNDERLAY  = (1 << 23) // object doesn't use Z-buffer, draws before taxiways and runways (ground underlay)
};
enum eWheelType {
  TRICYCLE      = 1,
  TAIL_DRAGGER  = 2,
  SNOW          = 3,
  SKIDS         = 4,
};
//=====================================================================================
//	Sound definitions
//=====================================================================================
typedef enum {
  ENGINE_STOPPED    = 0,
  ENGINE_CRANKING   = 1,
  ENGINE_RUNNING    = 2,
  ENGINE_STOPPING   = 3,
  ENGINE_FAILING    = 4,
  ENGINE_WINDMILL   = 5,
  ENGINE_CATCHING   = 6,
	ENGINE_MISSFIRE   = 7,
	ENGINE_MAX_SOUND  = 8,
} EEngineStatus;

/////////////////////////////////////////////////////////////
//
//  Structures/Typedefs
//
/////////////////////////////////////////////////////////////

typedef struct SDLLCopyright
{
  char  product[128];
  char  company[128];
  char  programmer[128];
  char  dateTimeVersion[128];
  char  email[128];
  int internalVersion;
} SDLLCopyright;

typedef struct SDLLRegisterTypeList
{
  EDLLObjectType  type;
  long    signature;
  SDLLRegisterTypeList  *next;
} SDLLRegisterTypeList;

typedef void* DLLObjectRef;
typedef void* DLLFileRef;

typedef struct SFlyObjectRef
{
  void  *objectPtr;
  void  *superSig;
  void  *classSig;
} SFlyObjectRef;

typedef struct SFlyObjectList
{
  SFlyObjectRef ref;
  SFlyObjectList  *prev;
  SFlyObjectList  *next;
} SFlyObjectList;

typedef struct SDLLObject
{
  //
  //  public; set this to any value you want to uniquely
  //  identify an "instance" of your object on the .DLL
  //  side.  this can be a gauge, system, or other 
  //  component.
  //

  DLLObjectRef  dllObject;  // set by .DLL; can be any value you want

  //
  //  private; do not use or modify the following members
  //

  DLLFileRef  dllFile;  // set by Fly!; do NOT modify this value!!!
  SFlyObjectRef flyObject;  // set by Fly!; do NOT modify this value!!!
} SDLLObject;
//---------------------------------------------------------------------------
//  JS NOTE:  replace Sender by a Tag because the field was not used
//            coherently.  Gauges and subsytems where stored in this field
//            without any mean to know which one
//---------------------------------------------------------------------------
typedef struct SMessage
{
  unsigned int      id;				      // message ID
  unsigned int      group;			    // target group ID
  EMessageDataType  dataType;       // result data type
  unsigned int      result;			    // message result code

//  DLLObjectRef  sender;       
  Tag               sender;         // Sender tag
  DLLObjectRef      receiver;
  char              dst[8];         // string destination
  float             volts;
  union 
  {
    char    charData;
    char   *charPtrData;
    int     intData;
    int    *intPtrData;
    double  realData;
    double *realPtrData;
    void   *voidData;
  };
      
  union
  {
    unsigned int  userData[8];
    struct 
    {
      unsigned int  unit;				// unit number
      unsigned int  hw;					// hardware type
      unsigned int  engine;			// engine number
      unsigned int  tank;				// tank number
      unsigned int  gear;				// gear number
      unsigned int  invert;			// Used to invert value
      unsigned int  datatag;		// data tag
      unsigned int  sw;					// switch position
    } u;
  } user;
  unsigned int  action;
  //--- Constructor -------------------------------
  SMessage()
  { memset(this,0,sizeof(SMessage)); }
} SMessage;
//==============================================================================
typedef struct SSurface
{ unsigned int   nPixel;
  unsigned int   invert;          // (ySize -1)* xSpan to reverse line order
  unsigned int  *drawBuffer;      // surface buffer
  unsigned int   xSize, ySize;    // surface dimensions
  int   xScreen, yScreen;         // screen coordinates for upper-left corner
} SSurface;

typedef struct SFont
{
  char  fontName[64];   // -> passed to APILoadFont; name of font family image file
  void  *font;      // <- returned from APILoadFont; do NOT alter!!!
} SFont;


typedef struct {
  char    bitmapName[64]; // -> passed to APILoadBitmap
  void    *bitmap;				// <- returned from APILoadBitmap; do NOT alter!!!
  EBitmapType type;				// <- returned from APILoadBitmap; do NOT alter!!!
} SBitmap;


typedef struct {
  unsigned long   magic;    // Magic code of 'PBM '
  unsigned long   width;
  unsigned long   height;
  unsigned long   x1, y1, x2, y2;
} SPBMHeader;


typedef struct {
  GLubyte r, g, b;
} RGB;


//===============================================================================
// SStream represents a stream text file, see Stream.h and Stream.cpp for details
//===============================================================================

typedef struct SStream
{
  char  filename[PATH_MAX];
  char  mode[3];
  void  *stream;
} SStream;

//===============================================================================
// SVector represents a vector in either Cartesian (x,y,z) or Polar (p,h,r)
//   coordinates
//===============================================================================

typedef struct SVector
{
  union {
    double  x;
    double  p;  // when used as a polar, pitch (in radians)
  };

  union {
    double  y;
    double  h;  // when used as a polar, heading (in radians)
  };

  union {
    double  z;
    double  r;  // when used as a polar, radius (in feet)
  };
} SVector;
;
//===============================================================================
// SPosition represents a geographical position on the earth.  The "zero" reference
//   point is 0 deg latitude, 0 deg longitude, 0 feet MSL.
//
// 'lat' is the latitude, in arcseconds north (+) or south (-) of the equator
// 'lon' is the longitude, in arcseconds west of the Prime Meridian
// 'alt' is the altitude in feed above MSL
//=================================================================================
typedef struct SPosition
{
  double  lat;
  double  lon;
  double  alt;
} SPosition;

typedef struct SPositionList
{
  SPosition pos;    // points should be stored clockwise
  SPositionList *next;
} SPositionList;

typedef struct SCollision
{
  char    collided; // 0 or 1
  SPosition pos;    // collision position
  SFlyObjectRef object;   // collision object
} SCollision;

typedef struct SMovie
{
  void  *gc;
  void  *movie;
  SSurface  surface;
} SMovie;

typedef struct SNavaid
{
  char    name[40];
  char    id[8];

  int   type;
  int   classification;
  int   usage;

  SPosition pos;
  float   freq;
  float   range;
  float   magneticVariation;
  float   slavedVariation;

  SNavaid   *prev;  // double-linked list
  SNavaid   *next;  // double-linked list
} SNavaid;

typedef struct SILS
{
  char    airportKey[10];
  char    runwayID[5];

  int   type;
  int   classification;
  SPosition pos;
  float   range;
  float   magneticVariation;

  float   glideslopeAngle;
  float   approachBearing;
  char    backCourseAvailable;

  SILS    *prev;  // double-linked list
  SILS    *next;  // double-linked list

  //  version 2.0 additions

  float   freq;
} SILS;

typedef struct SComm
{
  char    airportKey[10];

  char    name[20];
  int   type;
  float   freq[5];
  SPosition pos;

  SComm   *prev;  // double-linked list
  SComm   *next;  // double-linked list
} SComm;

typedef struct SWaypoint
{
  char    name[26];
  char    colocated;  // 0 or 1
  int   type;
  int   usage;
  float   bearingToNavaid;
  float   distanceToNavaid;
  SPosition pos;
  char    navaid[10];

  SWaypoint *prev;
  SWaypoint *next;
} SWaypoint;

typedef struct SRunwayEnd
{
  char    id[4];        // painted end marking
  float   trueHeading;      // in degrees
  float   magneticHeading;    // in degrees
  int   markings;     // numbers only, PIR, STOL, etc.
  SPosition pos;        // lat/lon position of center of runway end
  int   thresholdCrossingHeight;  // in feet
  int   displacedThreshold;   // in feet
  int   rvr;        // touchdown, midfield, rollout
  char    rvv;        // 0..1
  char    edgeLights;     // 0..1
  char    endLights;      // 0..1
  char    centerlineLights;   // 0..1
  char    touchdownLights;    // 0..1
  char    alignLights;      // 0..1
  char    thresholdLights;    // 0..1
  char    sequenceFlashing;   // 0..1
  int   numLightSystems;    // 0..1
  int   lightSystems[8];    // 0..1
  char    edgeLightIntensity;   // 0..1
  int   glideslopeType;     // 0..1
  int   glideslopePlacement;    // 0..1
  int   glideslopeConfiguration;  // 0..1

  SILS    *glideslope;      // associated GS
  SILS    *localizer;     // associated LOC
} SRunwayEnd;


typedef struct SRunway
{
  char    airportKey[10];

  float   length;     // in feet
  float   width;      // in feet
  int   surfaceType;    // surface material (asphalt, concrete, etc.)
  int   surfaceCondition; // condition (good, fair, poor)
  char    pavementType;   // rigid or flexible
  int   pcn;      // pavement classification number (FAA)
  int   subgradeStrength; // high, medium, low, very low
  int   tirePressure;   // high, medium, low, very low
  char    closed;     // 0..1
  char    pcl;      // 0..1
  char    pattern;    // left or right
  float   elevation;    // in feet

  SRunwayEnd  base;     // base end data
  SRunwayEnd  recip;      // reciprocal end data

  SRunway   *prev;      // double-linked list
  SRunway   *next;      // double-linked list
} SRunway;


typedef struct SAirport
{
  char    airportKey[10];
  char    faaID[5];
  char    icaoID[5];
  int   type;
  char    name[40];
  char    country[3];
  char    state[3];
  char    county[40];
  char    city[40];
  float   elevation;
  int   ownership;
  int   usage;
  float   magneticVariation;
  float   trafficAltitude;
  char    fssAvailable;   // 0..1
  char    notamAvailable;   // 0..1
  char    atcAvailable;   // 0..1
  char    segmentedCircle;  // 0..1
  char    landingFeesCharged; // 0..1
  char    jointUseAirport;  // 0..1
  char    militaryLandingRights;  // 0..1
  char    customsEntryPoint;  // 0..1
  int   fuelTypes;
  int   frameService;
  int   engineService;
  int   bottledOxygen;
  int   bulkOxygen;
  int   beaconLensColor;
  int   basedAircraft;
  int   annualCommercialOps;
  int   annualGeneralAvOps;
  int   annualMilitaryOps;
  int   attendanceFlags;
  int   lightingFlags;
  SPosition pos;
      
  SRunway   *runways;   // list of runways
  SComm   *comms;     // list of radio comms
    
  SAirport  *prev;      // double-linked list
  SAirport  *next;      // double-linked list
} SAirport;

typedef struct SGeneric
{
  void  *data;

  SGeneric  *prev;
  SGeneric  *next;
} SGeneric;
//=============================================================================
//  WEATHER INFO
//=============================================================================
typedef struct SWeatherInfo
{
  //
  //  weather data
  //

  double  visibility;     // in nautical miles (10.0 = 10 nautical miles)
  int surfaceTemp;        // degrees F
  int dewPointTemp;       // degrees F
  double  altimeter;      // inches Hg
  int windSpeed;          // knots
  int gustSpeed;          // knots
  int windHeading;        // degrees
  char  windFluctuates;   // 0=no fluctuation, 1=fluctuates
  int precipIntensity;    // valid values are:
          //  2=Light
          //  4=Medium
          //  8=Heavy (Thunderstorms if precipType is rain)
  int precipType;   // valid values are:
          //  0x2000=Rain (8192 decimal)
          //  0x4000=Snow (16384 decimal)

  //
  //  cloud layers
  //

  int layer1Active;   // 0 if no cloud layer, 1 if active
  int layer1Altitude;   // in feet
  int layer1Type;   // valid values are:
          //  2=Few
          //  3=Scattered
          //  4=Broken
          //  5=Overcast
          //  6=Fog
  int layer1Height;   // in feet

  int layer2Active;   // 0 if no cloud layer, 1 if active
  int layer2Altitude;   // in feet
  int layer2Type;   // valid values are:
          //  2=Few
          //  3=Scattered
          //  4=Broken
          //  5=Overcast
          //  6=Fog
  int layer2Height;   // in feet

  int layer3Active;   // 0 if no cloud layer, 1 if active
  int layer3Altitude;   // in feet
  int layer3Type;   // valid values are:
          //  2=Few
          //  3=Scattered
          //  4=Broken
          //  5=Overcast
          //  6=Fog
  int layer3Height;   // in feet
} SWeatherInfo;

typedef struct SWindsAloft
{
  int windHeading;    // in degrees
  int windSpeed;      // in knots
  int gustSpeed;      // in knots
  char  windFluctuates;   // non-zero if TRUE, zero if FALSE
} SWindsAloft;

//
// This structure represents a calendar date, with field definitions as follows:
//  month = 1..12, for January..December
//  day   = 1..31, for the calendar date in each month
//  year  = Gregorian year (positive is AD)
//
typedef struct SDate
{
  unsigned int  month;
  unsigned int  day;
  unsigned int  year;
} SDate;

//
// This structure represents a time of day, with field definitions as follows:
//  hour  = number of whole hours since midnight
//  minute  = number of whole minutes since the start of the last hour
//  second  = number of seconds since the start of the last minute
//  msecs = number of milliseconds since the start of the last second
//
typedef struct STime
{
  unsigned int  hour;
  unsigned int  minute;
  unsigned int  second;
  unsigned int  msecs;
} STime;

//
// This structure simply combines the SDate and STime structs into a single
//   entity for convenience
//
typedef struct SDateTime
{
  SDate date;
  STime time;
} SDateTime;

//
// The following structure represents a date/time difference.  Note that since
//   all fields are unsigned, this can only represent a positive difference
//   in time.
//
typedef struct SDateTimeDelta
{
  unsigned int  dYears;
  unsigned int  dMonths;
  unsigned int  dDays;
  unsigned int  dHours;
  unsigned int  dMinutes;
  unsigned int  dSeconds;
  unsigned int  dMillisecs;
} SDateTimeDelta;


typedef struct SFPAirport
{
  char    landHere; // 0 or 1
  SDateTimeDelta  layover;
  SAirport  data;
  char    depRunwayEnd[4];  // runway end ID
  char    arrRunwayEnd[4];  // runway end ID
} SFPAirport;

typedef struct SFPNavaid
{
  SNavaid   data;
} SFPNavaid;

typedef struct SFPWaypoint
{
  SWaypoint data;
} SFPWaypoint;

typedef struct SFPUser
{
  char  name[40];
} SFPUser;

typedef struct SFPEntry
{
  EFPEntryType  type;
  SPosition pos;
  SDateTime arrival;
  SDateTime departure;
  SDateTimeDelta  extend;
  float   distance;
  float   bearing;
  float   speed;
  float   altitude;
  float   effectiveSpeed;
  float   effectiveAltitude;
  float   magneticVariation;

  union   // 'type' determines which struct is populated
  {
    SFPAirport  a;
    SFPNavaid n;
    SFPWaypoint w;
    SFPUser   u;
  };
} SFPEntry;

typedef struct SFlightPlan
{
  SFPEntry  entry;
  SFlightPlan *prev;
  SFlightPlan *next;
} SFlightPlan;

// added for multiple screen management
typedef struct {
    int Width, Height, Depth, Refresh, X, Y, ID;
    bool bMouseOn;
    char full;
} sScreenParams;
//-----------------------------------------------------------------
//  IMAGE structure
//-------------------------------------------------------------------------------
typedef struct {
  U_CHAR state;                   // Drag State
  short ht;                       // Image Height (pixel)
  short wd;                       // Image width  (pixel)
  int   dim;                      // Total pixels
  short  x0;                      // Window position
  short  y0;                      // Window position
  short  mx;                      // Mouse position
  short  my;                      // Mouse position
  U_INT *rgba;                    // Pixel map
} S_IMAGE;
//-----------------------------------------------------------------
//  Damage message
//-----------------------------------------------------------------
typedef struct {
  char    Severity;               // 0 None
  char    rfu;                    // 1 Warning (orange)
  Tag     snd;                    // 2 Sound tag
  char   *msg;                    // 3 Message
} DAMAGE_MSG;
//=====================================================================
//  Common fuel weight per gallon
//=====================================================================
#define FUEL_LBS_PER_GAL (float(6.02))
#define FUEL_EMPTY_QTY   (float(0.00001))
//-------------------------------------------------------------------------------
//  Request code to database access
//-------------------------------------------------------------------------------
  enum DBCODE { NO_REQUEST      = 0,
                COM_BY_AIRPORT  = 1,
                RWY_BY_AIRPORT  = 2,
                CTY_BY_ALLLOT   = 3,
                CTY_BY_CTYKEY   = 4,
                APT_BY_FILTER   = 5,
                NAV_BY_FILTER   = 6,
                WPT_BY_FILTER   = 7,
                STA_BY_COUNTRY  = 8,
                APT_BY_OFFSET   = 9,
                NAV_BY_OFFSET   =10,
                NDB_BY_OFFSET   =11,
                ILS_BY_AIRPORT  =12,
                APT_NEARESTONE  =13,              // The first nearest airport
                GPS_NR_AIRPORT  =14,              // Nearest Airports from cache
                GPS_GT_AIRPORT  =15,              // Airport by Ident/Name
                GPS_GT_RUNWAY   =16,              // Runway for airport
                GPS_GT_COMM     =17,              // Comm for airport
                GPS_GT_ILS      =18,              // ILS for airport
                GPS_GT_VOR      =19,              // VOR by Ident/Name
                GPS_NR_VOR      =20,              // Nearest VOR from cache
                GPS_GT_NDB      =21,              // NDB by Ident/Name
                GPS_NR_NDB      =22,              // Nearest NDB from cache
                GPS_GT_WPT      =23,              // WPT by Ident/name
                GPS_NR_WPT      =24,              // Neareast WPT from cache
                GPS_ANY_WPT     =25,              // Any waypoint by Ident
                REQUEST_END     =255,
      };
  //-----------Define GPS search mode ---------------------------------
  enum GPS_MODE { GPS_NO_MATCH = 0,         // Match meaningless
                  GPS_FS_MATCH = 1,         // First match
                  GPS_NX_MATCH = 2,         // Next match
                  GPS_PV_MATCH = 3,         // Previous match
  };
  //-----------RADIO TAG INDEX -----------------------------------------
  enum TAG_RADIO {  NAV_INDEX = 1,
                    COM_INDEX = 2,
                    ADF_INDEX = 3,
  };
//==============================================================================
//
// CObject is the most basic object type and is the root ancestor of all
//   object classes
//==============================================================================
class CObject {
public:

protected:
};

//===============================================================================
//  CShared is a basic class for sharing an object
//  It provide a user count and a delete mecanism when user count is 0
//===============================================================================
class CShared {
protected:
  pthread_mutex_t		mux;					// Mutex for lock
  unsigned int Users;
  //-----------------------------------------------------
public:
           CShared();
  virtual ~CShared() {}
  //----Count management --------------------------------
  void  IncUser();
  void  DecUser();
  //-----------------------------------------------------
  virtual void TraceDelete() {};
};
//==============================================================================
//  CPTR is a smart pointer to a shared object
//  Methods are implemented in TerrainCache.cpp
//==============================================================================
class CPTR {
  //----Attribut is a pointer to a shared object --------
protected:
  CShared *obj;
  //----Methods -----------------------------------------
public:
  CPTR() {obj = 0;}
  //--------Define assignement and access operators ------
  void operator=(CShared *p);
  void operator=(CShared &n);
  void operator=(CPTR    &q);
  //--------Define Access operator ----------------------
  inline CShared* operator->() {return obj;}
  inline bool     operator==(CShared *itm) {return (obj == itm);}
  inline bool     Assigned()  {return (obj != 0);}
  inline bool     IsNull()    {return (0 == obj);}
  inline CShared *Obj()       {return obj;}
};


//============================================================================
//  Class CBaseBitmap:  Provide only virtual functions
//  This is base class for
//    CBitmapPBG
//    CBitmapPBM
//    CBitmapNUL
//============================================================================
class CBaseBitmap {
  //---This is the only attribute --------------------------------
protected:
  char    loaded;                     // Load indicator
  //--------------------------------------------------------------
public:
  virtual ~CBaseBitmap()  {loaded = 0;}
  //--------------------------------------------------------------
  virtual void DrawBitmap (SSurface *sf,int x,int y,int fr) {}
  virtual void FillTheRect(SSurface *sf,int x,int y,int wd,int ht,int fr){}
  virtual void DrawPartial(SSurface *sf,int x,int y,int x1,int y1,int x2,int y2,int fr){}
  virtual void DrawFrom   (SSurface *sf,int pos,int nbl, int x, int y){}
  virtual void GetBitmapSize(int *x, int *y) {*x = 0; *y = 0;}
  virtual int  NumBitmapFrames()  {return 0;}
  virtual void SetFrameNb(int k)  {};
  virtual U_CHAR *GetRGBA()       {return 0;}
  //--------------------------------------------------------------
  bool NotLoaded()  {return (loaded == 0);}

};
//============================================================================
//  Class  CBitmapNUL:  Substitute for CBitmapPBG or CBitmapPBM
//  Used when no bitmap is supplied or bitmap has error
//  This bitmap does nothing except intercepting call and avoid testing
//  if a bitmap exists or not
//============================================================================
class CBitmapNUL: public CBaseBitmap {
  int use;                            // Number of users
  //-------------------------------------------------------------
public:
  CBitmapNUL()  {use = 0;}
  void          IncUser()   {use++;}
  int           DecUser()   {use--; return use; }
  //--------No other supplied method ----------------------------
};
//============================================================================
//  Class CBITMAP
//============================================================================
class CBitmap
{
public:
  CBitmap(void);
  CBitmap(char * bmapname);
 ~CBitmap(void);

  // Override operators new and delete
  void  operator delete (void*);
  void *operator new (size_t size);

  const char * name(void) {return m_bm.bitmapName;};
  EBitmapType type(void) {return m_bm.type;};
  bool  Load(void);
  bool  Load_Bitmap(char * bname);

  void * Bitmap(void) {return m_bm.bitmap;};

  void  DrawBitmap (SSurface *sf,int x,int y,int fr);
  void  FillTheRect(SSurface *sf,int x,int y,int wd,int ht,int fr);
  void  DrawPartial(SSurface *sf,int x,int y,int x1,int y1,int x2,int y2,int fr);
  void  DrawFrom   (SSurface *sf,int pos,int nbl, int x, int y);
  void  GetBitmapSize(int *xSize, int *ySize);
  int   NumBitmapFrames( );
  void  FreeBitmap( );
  //------------------------------------------------------------
  void  AssignNUL();
  void  ChangeType();
  bool  NotLoaded()         {return ((CBaseBitmap*)m_bm.bitmap)->NotLoaded();   }
  void  SetFrameNb(int k)   {       ((CBaseBitmap*)m_bm.bitmap)->SetFrameNb(k); }
  U_CHAR *GetRGBA()         {return ((CBaseBitmap*)m_bm.bitmap)->GetRGBA();}
  //------------------------------------------------------------
protected:
  SBitmap m_bm;
};



/*
 * Database functions
 */

//
// These functions all return a linked list of the appropriate database entity.
//   The calling application is responsible for calling APIFree... when done
//   with the list
//
int GetLocalILS(SILS **ils);
int GetLocalComms(SComm **comms);
int GetLocalCenters(SComm **comms);
int GetLocalWaypoints(SWaypoint **waypoints);

//
// The calling application is responsible for allocating the database structs passed
//   in to the following functions
//
int GetAirport(char *airportKey, SAirport *airport);

//
// The following functions implement specific database searches, returning a linked
//   list of database records which match the specified key value.
//
int SearchNavaidsByID(char *id, int navType, SNavaid **navaids);
int SearchNavaidsByName(char *name, SNavaid **navaids);
int SearchWaypointsByName(char *name, SWaypoint **waypoints);
int SearchAirportsByICAO(char *icaoID, SAirport **airports);
int SearchAirportsByName(char *name, SAirport **airports);
int SearchILS(char *airportKey, char *runwayEndID, SILS **ils);

//
//  INI functions
//

void LoadIniSettings (void);
void UnloadIniSettings (void);
void SaveIniSettings (void);
void GetIniVar (const char *section, const char *varname, int *value);
void GetIniFloat (const char *section, const char *varname, float *value);
void GetIniString (const char *section, const char *varname, char *strvar, int maxLength);
void SetIniVar (const char *section, const char *varname, int value);
void SetIniFloat (const char *section, const char *varname, float value);
void SetIniString (const char *section, const char *varname, char *strvar);
const char *GetIniValue(const char *section, const char *key);
bool IsSectionHere(const char *section);
//===================================================================================
//	FORWARD DECLARATION FOR ALL CLASSES WITH POINTERS
//===================================================================================
//-------Forward declaration for all .H files ----------------------
//----OBJECTS ----------------------------------------------------
class CCamera;
class CCameraObject;
class CCameraRunway;
class CCameraSpot;
class ClQueue;
class CSubsystem;
class CTileCache;
class CAirport;
class CNavaid;
class CRunway;
class CPaveRWY;
class CAptObject;
class CVehicleObject;             // Vehicle object
class CAudioManager;              // Audio sustem
class CExternalLightManager;      // Light manager
class CElectricalSystem;          // electrical system
class AutoPilot;									// Autopilot system
class CMagneticModel;             // Magnetic model
class CFuelSystem;                // Fuel system
class CKeyMap;                    // Keyboard interface
class CBitmapNUL;                 // Null bitmap
class CKeyDefinition;             // Key definition
class CKeyFile;                   // Default keys
class PlaneCheckList;                    // Checklist
//---NAVIGATION -------------------------------------------------
class CILS;
//---Line box ---------------------------------------------------
class CTgxLine;
//----CONTROL ---------------------------------------------------
class CAnimatedModel;             // Level of Detail
class CSlewManager;               // Slew manager
class CCursorManager;             // Cursor manager
class CTestBed;                   // Test bed
class SqlMGR;                     // SQL MAnager
class SqlTHREAD;                  // SQL Thread
class CJoysticksManager;          // Joystick handler
//class C3DMgr;                     // 3D world manager
class CSkyManager;                // Sky manager
class CWeatherManager;            // Weather Manager
class CCameraManager;             // Camera manager
class CCockpitManager;            // Cockpit manager
class CFuiManager;                // Windows manager
class CAtmosphereModelJSBSim;     // Atmosphere
class CEngine;                    // Engine instance
class CSuspension;                // Wheel suspension
class CSoundOBJ;                  // Sound object
//----PANEL -----------------------------------------------------
class CGauge;
class CKAP140Panel;
class CCockpitPanel;
class CPanel;                     // Current panel
//----MODEL 3D --------------------------------------------------
class CWobj;
class C3Dmodel;
class C3Dworld;
//----METEO -----------------------------------------------------
class CMeteoArea;
class CCloud;
class CCloudModel;
class CCloudSystem;                         // Cloud system
//----TERRAIN CACHE ---------------------------------------------
class CDbCacheMgr;								          // Nav cache
class TCacheMGR;                            // Terrain cache
class C_QGT;                                // Quarter Global Tile
class C_QTR;                                // QTR object
class C_SEA;                                // COAST object
class CAirportMgr;                          // Airport  Model
class CVertex;                              // Vertex
class CmQUAD;                               // QUAD
class C3DMgr;                               // 3D models
class CSuperTile;                            // Super Tile
class C_CDT;                                // Coast data
class CTextureWard;                         // Texture Hangar
//----FUI INTERFACE ----------------------------------------------
class CFuiComponent;
class CFuiPicture;
class CFuiButton;
class CFuiCloseButton;
class CFuiMiniButton;
class CFuiZoomButton;
class CFuiWindowTitle;
class CFuiWindow;
class CFuiTheme;
class CFuiThemeWidget;
class CFuiPage;
class CFuiCanva;
class CChkLine;
class CListBox;
//----WINDOWS --------------------------------------------------
class CFuiCkList;                 // Window check list
class CFuiFuel;                   // Fuel loadout
class CFuiLoad;                   // Variable loads
class CFuiChart;                  // Sectional chart
class CFuiList;
class CFuiProbe;
//----TOOLS -----------------------------------------------------
class CExport;
class CImport;
//=============================================================================
//  Define VBO offset
//=============================================================================
#define OFFSET_VBO(x) ((void *)(x))
//=============================================================================
//  Structure View PORT
//=============================================================================
struct VIEW_PORT {
  int x0;
  int y0;
  int wd;
  int ht;
};
//=============================================================================
//  Structure for terrain type
//=============================================================================
struct TC_TERRA {
  int   type;                                     // Terrain type
  char  nite;                                     // Night indicator
  char *desc;                                     // Terrain description
};
//=============================================================================
// Function type declaration for key map callbacks
//
typedef bool(*KeyCallbackPtr) (int keyid, int code, int modifiers);
typedef bool(*KeyGroupCB)(CKeyDefinition *kdf, int modifiers);
//=============================================================================
//  Structure to describe a rectangle in arcsec coordinates
//=============================================================================
struct TC_RECT {
  float SW_LON;                                  // SW longitude
  float SW_LAT;                                  // SW latitude
  float NE_LON;                                  // NW longitude
  float NE_LAT;                                  // NW latitude
  float alt;                                     // Altitude      
};
//=============================================================================
//  3D POINT DEFINITION
//=============================================================================
struct F3_VERTEX {
  float VT_X;
  float VT_Y;
  float VT_Z;
	//-------------------------------------------------
};
//=============================================================================
//  POINT DEFINITION
//=============================================================================
struct TC_TCOORD {
  float VT_S;
  float VT_T;
};
//=============================================================================
//  SPECIFIC POINT DEFINITION
//=============================================================================
struct TC_SPOINT {
  int x;
  int y;
  int type;
};
//=============================================================================
//  PIXEL DEFINITION
//=============================================================================
#define NE_PIX (0)
#define NW_PIX (1)
#define SW_PIX (2)
#define SE_PIX (3)
//---------------------------------------------------------
struct S_PIXEL {
	short	x;																	// X coordinate
	short	y;																	// Y Coordinate
	//---- Constructor --------------------------------------
	S_PIXEL::S_PIXEL()
	{	x = y = 0;}
};
//=============================================================================
//  COLOR DEFINITION
//=============================================================================
struct TC_COLORS {
  U_CHAR VT_R;                               // RED COEFFICIENT
  U_CHAR VT_G;                               // GRREN COEFFICIENT
  U_CHAR VT_B;                               // BLUE COEFFICIENT
  U_CHAR VT_A;                               // ALPHA COEFFICIENT
};
//=============================================================================
//  Vector 2D float
//=============================================================================
struct TC_2DF {
  float VT_X;
  float VT_Y;
};
//=============================================================================
//  Vector 2D float
//=============================================================================
struct TC_4DF {
  float x0;
  float y0;
  float x1;
  float y1;

};

//=============================================================================
//  SCreen parameters
//=============================================================================
struct TC_SCREEN {
  char  in;                     // Inside indicator
  //-----------------------------------------------
  int   sx;                     // X width
  int   sy;                     // Y Height
  //-----------------------------------------------
  int   mx;                     // X max
  int   my;                     // Y max
};
//=============================================================================
//  VERTEX TABLE
//=============================================================================
struct TC_VTAB {
  float VT_S;                                     // S coordinate
  float VT_T;                                     // T coordinate
  float VT_X;                                     // X corrdinate
  float VT_Y;                                     // Y coordinate
  float VT_Z;                                     // Z coordinate
  //----Copy from another table -------------------------
  void  TC_VTAB::Dupplicate(TC_VTAB *s, int n)
  { TC_VTAB *dst = this;
  for (int k = 0; k<n; k++)  *dst++  = *s++;
  return; }
  //------------------------------------------------------
};
//=============================================================================
//  VERTEX TABLE FOR PANEL
//=============================================================================
struct VTP_2D {
  float VT_S;                                     // S coordinate
  float VT_T;                                     // T coordinate
  float VT_X;                                     // X corrdinate
  float VT_Y;                                     // Y coordinate
  //----Copy from another table -------------------------
  void  VTP_2D::Dupplicate(VTP_2D *s, int n)
  { VTP_2D *dst = this;
  for (int k = 0; k<n; k++)  *dst++  = *s++;
  return; }
  //------------------------------------------------------
};

//=============================================================================
//  VERTEX TABLE with NORMAL VECTOR and TEXTURE COORDINATES
//=============================================================================
struct GN_VTAB {
  float VT_S;                                     // S coordinate
  float VT_T;                                     // T coordinate
  float VN_X;                                     // X Normal
  float VN_Y;                                     // Y Normal
  float VN_Z;                                     // Z Normal
  float VT_X;                                     // X coordinate
  float VT_Y;                                     // Y coordinate
  float VT_Z;                                     // Z coordinate
	//--------------------------------------------------------------
};
//=============================================================================
//  VERTEX TABLE with 2 TEXTURE COORDINATES and color values
//=============================================================================
struct C3_VTAB {
  float VT_S;                                     // S coordinate
  float VT_T;                                     // T coordinate
	float VT_R;																			// Red value
	float	VT_G;																			// Green value
	float VT_B;																			// Blu value
  float VT_X;                                     // X coordinate
  float VT_Y;                                     // Y coordinate
  float VT_Z;                                     // Z coordinate
	//--------------------------------------------------------------
};

//=============================================================================
//  VERTEX TABLE with NORMAL VECTOR
//=============================================================================
struct VT_VTAB {
  float VN_X;                                     // X Normal
  float VN_Y;                                     // Y Normal
  float VN_Z;                                     // Z Normal
  float VT_X;                                     // X coordinate
  float VT_Y;                                     // Y coordinate
  float VT_Z;                                     // Z coordinate
	//--------------------------------------------------------------
	VT_VTAB::VT_VTAB()
	{	VN_X	= VN_Y = VN_Z = 0;
		VT_X	= VT_Y = VT_Z = 0;
	}
};

//=============================================================================
//  CAMERA CONTEXTE
//=============================================================================
struct CAMERA_CTX {
	char    sidn[12];																// Camera tag
  Tag     iden;                                   // Camera ident
  double  range;                                  // Range
  double  rmin;                                   // Min range
  double  rmax;                                   // max range
  double  theta;                                  // Angle theta (horizontal)
  double  phi;                                    // Angle phi   (vertical)
  float   fov;                                    // Field of view
	//---------------------------------------------------------------
	SVector		ori;																	// World orientation
	SPosition pos;																	// World position
};
//=====================================================================
//  Tuple for loading table lookup
//=====================================================================
struct TUPPLE {
  float inp;                    // Input value
  float out;                    // Output value
  float dtv;                    // delta (y) / delta (x)
};
//=====================================================================
//  Tuple for loading table lookup with 2 output value
//=====================================================================
struct TUPPLE2 {
  float inp;                    // Input value
  float out1;                   // Output value 1
  float out2;                   // output value 2
};
//=====================================================================
//  Tuple for loading table lookup with 3 output value
//=====================================================================
struct TUPPLE3 {
  float inp;                    // Input value
  float out1;                   // Output value 1
  float out2;                   // output value 2
  float out3;                   // Output value 3
};

//============================================================================
//  Struct vertex 2D
//============================================================================
struct TC_BOUND {
  U_INT xmin;
  U_INT zmin;
  U_INT xmax;
  U_INT zmax;
};
//==============================================================================
//  PLOT PARAMETERS
//==============================================================================
#define PLOT_MENU_SIZE  (16)
#define PLOT_MENU_WIDTH (64)
class CDependent;
//-----------------------------------------------------------------------------
typedef struct {
   Tag  iden;                     // Destination
   Tag  type;                     // Data type
   char menu[PLOT_MENU_WIDTH];    // Menu type
} PLOT_PM;
//-----------------------------------------------------------------------------
typedef struct {
  CDependent *dpnd;               // Plotted component
  float *pvl;                     // pointer to value
  float yUnit;                    // number of unit in Y axis
  float val;                      // Value to plot
  float sign;                     // Sign of value
  char  mask[PLOT_MENU_WIDTH];    // Edit mask
} PLOT_PP;                        // Plot parameters

//=============================================================================
//  A simple 2D rectangle
//=============================================================================
class C_2DRECT {
  //-----public ---------------------
public:
  int x0;
  int y0;
  int x1;
  int y1;
  //---------------------------------
  C_2DRECT();
  void  Init(int x0,int y0, int x1,int y1);
  bool  IsHit(int x,int y);
};
//=============================================================================
//  POLYGON TABLE
//=============================================================================
class CPolygon {
  //-----Attributes ------------------------------------------------------
  short    nbv;                                      // Number of vertices
  short    rfu;                                      // Reserved
  U_INT    mode;
  TC_VTAB *vtb;                                      // Vertex table
  //----------------------------------------------------------------------
public:
  CPolygon();
 ~CPolygon();
  //------------------------------------------------------
  void      Draw();
  void      SetPolygons(int n);
  //------------------------------------------------------
  TC_VTAB  *GetVTAB()               {return vtb;}
  int       GetNBVT()               {return nbv;}
  //------------------------------------------------------
  inline    void  ClearCount()      {nbv = 0;}
  inline    void  AddCount(int k)   {nbv += k;}
  inline    void  SetMode(U_INT m)  {mode = m;}
  inline    void  IncCount()        {nbv++;}
};

//=============================================================================
//  Rectangular POLYGON
//=============================================================================
class CPolyREC {
  //----Attributes ------------------------------------------------------
  short nbv;                                        // Number of vertices
  short rfu;                                        // Reserved
  TC_VTAB vtab[4];                                  // 4 vertices
  //------------------------------------------------------------------
public:
  CPolyREC();
  //------------------------------------------------------------------
  void  InitQuad(CPanel *panel, SSurface *sf);
  void  Draw(bool bld);
  void  DrawStrip();
  //------------------------------------------------------------------
  TC_VTAB  *GetVTAB()               {return vtab;}
  float     GetX(int k)             {return vtab[k].VT_X;}
  float     GetY(int k)             {return vtab[k].VT_Y;}
};
//=============================================================================
//  POLYGON TABLE with normals
//=============================================================================
class CPoly3D {
  //----Attributes ------------------------------------------
  short    nbv;                                       // Number of vertices
  short    rf1;                                       // RFU
  GLenum   pmd;                                       // Polygon mode
  GN_VTAB *vtb;                                       // Vertice table
  //-----------------------------------------------------------------
public:
  CPoly3D();
  CPoly3D(int nv);
 ~CPoly3D();
  void      Allocate(int nv);
  void      Draw(U_INT txo);
  GN_VTAB  *GetVTAB(int k);
  GN_VTAB  *SetVTAB(int k,GN_VTAB *v);      
 //------------------------------------------------------------------
 inline GN_VTAB *GetVTAB()                    {return vtb;}
 inline void     SetMode(GLenum m)            {pmd = m;}
};
//=============================================================================
//  Texture Info
//=============================================================================
#define TC_TEXTURE_NAME_DIM (512)
#define TC_TEXTURE_NAME_NAM (64)
#define TC_LAST_INFO_BYTE   (TC_TEXTURE_NAME_DIM - 1)
//-------------------------------------------------------------------
struct  TEXT_INFO {
	U_INT			key;
  short     wd;                           // Texture width
  short     ht;                           // Texture height
  U_CHAR    apx;                          // alpha when pixel
	U_CHAR		azp;													// alpha when 0 pixle
  char      bpp;                          // Byte per plan
  char      res;                          // Resolution     (if needed)
  char      type;                         // Terrain type   (if needed)
  U_INT     xOBJ;                         // Texture object (if needed)
  GLubyte  *mADR;                         // RGBA Memory addresse
  char      name[TC_TEXTURE_NAME_NAM];    // Texture name
  char      path[TC_TEXTURE_NAME_DIM];    // Texture full name
	TEXT_INFO::TEXT_INFO()
	{	key			= 0;
		wd = ht = 0;
		apx			= 0xFF;
		azp			= 0;
		xOBJ		= 0;
		name[0]	= 0;
	}
};
//---------------------------------------------------------------------
struct TEXT_DEFN  {
  short wd;                               // Texture width
  short ht;                               // Texture height
  short nf;                               // Number of frames
  short pm;                               // gauge parameter
  U_INT xo;                               // Texture object
	U_INT bo;																// Buffer Object
  int   dm;                               // Dimension in pixel
  GLubyte *rgba;                          // RGBA pixels
  //-------------------------------------------------------------------
  TEXT_DEFN::TEXT_DEFN()
  { xo    = 0;
		bo		= 0;
    rgba  = 0;
    nf    = 1;
    dm    = 0;
  }
  //-------------------------------------------------------------------
  TEXT_DEFN::~TEXT_DEFN()
  { Free(); }
  //-------------------------------------------------------------------
  void  TEXT_DEFN::Copy(TEXT_INFO &t)
  { wd    = t.wd;
    ht    = t.ht;
    xo    = t.xOBJ;
    dm    = wd * ht;
    rgba  = t.mADR;
  }
  //-------------------------------------------------------------------
  void  TEXT_DEFN::Copy(TEXT_DEFN &t)
  { wd    = t.wd;
    ht    = t.ht;
    xo    = t.xo;
		bo		= t.bo;
    dm    = t.dm;
    nf    = t.nf;
    rgba  = t.rgba;
  }
  //-------------------------------------------------------------------
  void  TEXT_DEFN::Dupplicate(TEXT_DEFN &t)
  { Copy(t);
    if (0 == t.rgba)  return;
    int     nb  = dm * nf * 4;
    U_CHAR *bf  = new U_CHAR[nb];
    U_CHAR *src = t.rgba;
    U_CHAR *dst = bf;
    for (int k=0; k<nb; k++) *dst++ = *src++;
    rgba  = bf;
    return;
  }

  //------------------------------------------------------------------
  void TEXT_DEFN::Free()
  { if (xo) glDeleteTextures(1,&xo);
		if (bo) glDeleteBuffers(1,&bo);
    if (rgba) delete [] rgba;
    xo    = 0;
    rgba  = 0;
  }
  //------------------------------------------------------------------
  int Clamp(int fr)
  { if (fr <   0)     return 0;
    if (fr >= nf)     return (nf - 1);
    return fr;
  }
  //-------------------------------------------------------------------
};
//==============================================================================
// A CStreamObject is one which can be loaded and saved from/to a stream
//   file.  See Stream\Stream.h for more details and the API for stream
//   files
//==============================================================================
class CStreamFile;
//==============================================================================
class CStreamObject : public CObject {
public:
  virtual ~CStreamObject (void) {}
  virtual const char *GetClassName(void) {return "UnKnown";}	// JSDEV*
  virtual       char *GetIdString(void)  {return "????";}		// JSDEV*
  virtual int   Read (SStream *stream, Tag tag) {return TAG_IGNORED;}
  virtual void  ReadFinished (void) {}
  virtual void  Write (SStream *stream) {}
  virtual int   Read  (CStreamFile *sf, Tag tag){return TAG_IGNORED;}
  //-----------------------------------------------------------------
  void    ReadMonoStrip(SStream *str,TEXT_DEFN &txd,int *px, int *py);
  void    DecodeMonoName(SStream *str,char *fn,int *px,int *py);
  void    DecodeStripName(SStream *str, char *fn, TEXT_DEFN &txd);
  void    ReadStrip(SStream *str,TEXT_DEFN &txd);
	void		ReadBMAP(SStream *str,TEXT_DEFN &txd);
  void    ReadCADR(SStream *str,CGauge *mgg,TC_VTAB *qd);
  void    ReadSIZE(SStream *str,int *px,int *py, int *wd, int *ht);
  void    ReadSIZE(SStream *str,short *px,short *py, short *wd, short *ht);
	void		ReadSIZE(SStream *str,S_PIXEL *t, short *wd, short *ht);
  //-------Helper ----------------------------------------------------
  float Clamp(float lim, float val)
  { if (val < -lim)  return -lim;
    if (val > +lim)  return +lim;
    return val;
  }
  //------------------------------------------------------------------

};
//============================================================================
// Local classes which encapsulate the state information about an open
//   stream file.  An instance of one of these classes is stored in the
//   'stream' member of the SStream data structure.
//-------------------------------------------------------------------------------------
//  JS: Add a low level read and decode functions for files like BGR and TSM
//      that does not separate TAG and item by a new line
//      Move the class definition in FlyLegacy.h
//============================================================================
class CStreamFile {
public:
  // Constructor
  CStreamFile (void);

  // Status methods
  virtual bool      IsReadable (void)  { return readable; }
  virtual bool      IsWriteable (void) { return writeable; }

  // Open/close methods
  virtual void      OpenRead (const char* filename, PFS *pfs);
  virtual void      OpenWrite (const char* filename);
  virtual void      Close (void);

  // Low level Read methods--------------------------
  bool              Refill();
  bool              GetChar(char *st,int nb);
  bool              NextCharacter(short nc);
  bool              NextToken();
  bool              IsValid(char car);
  bool              NextTag();
  bool              GetTag();
  bool              ParseError();
  void              ReadFrom(CStreamObject *object);
  void              ReadDouble(double &nd);
  void              ReadLong(long &nb);
  void              ReadFloat(float &nb);
  // Write methods------------------------------------
  void              WriteBlankLines (int i);
  void              WriteComment (const char *comment);
  void              WriteTag (Tag tag, const char *comment);
  void              WriteInt (int *value);
  void              WriteUInt (unsigned int *value);
  void              WriteFloat(float *value);
  void              WriteDouble(double *value);
  void              WriteString(const char *value);
  void              WriteVector(SVector *value);
  void              WritePosition(SPosition *value);
  void              WriteTime(SDateTime *value);
  void              WriteTimeDelta(SDateTimeDelta *value);
  void              WriteMessage(SMessage *message);
  //---------------------------------------------------
  inline int        GetLine()   {return line;}
  inline void       IncLine()   {line++;}  
protected:
  void              WriteIndent (void);

public:
  PODFILE*  podfile;        // POD filesystem reference
  FILE*     f;              // Normal filesystem reference

protected:
  bool      readable;       // Whether this stream is readable
  bool      writeable;      // Whether this stream is writeable
  int       indent;
  int       line;           // Line number about
  //----------For low level decoding ----------------------------------------
  char            buf[256];     // Read buffer
  int             nBytes;       // Number of bytes left
  char           *rpos;         // Character position in buffer
  Tag             tag;          // Decoded tag

};
//======================================================================
/*
 * Stream functions implemented in Stream.cpp
 */

int   DoesFileExist(const char *filename, EFileSearchLocation where);
int   DoesPodVolumeExist(const char *volumeName);

int   OpenStream(SStream *stream);
int   OpenStream(PFS *pfs, SStream *stream);
void  CloseStream(SStream *stream);
long  sTell(SStream *st);
void  sSeek(SStream *st,long pos);
//void  WriteTo(SDLLObject *object, SStream *stream);
void  ReadFrom(CStreamObject *object, SStream *stream);
void  SkipObject(SStream *stream);
bool  AdvanceToTag(unsigned int tag, SStream *stream);

bool  ReadInt (int *value, SStream *stream);
void  ReadShort (short *value, SStream *stream);
bool  ReadUInt (unsigned int *value, SStream *stream);
void  ReadFloat (float *value, SStream *stream);
void  ReadDouble (double *value, SStream *stream);
void  ReadString (char *value, int maxLength, SStream *stream);
void  ReadVector (SVector *value, SStream *stream);
void  ReadLatLon (SPosition *value, SStream *stream);
void  ReadPosition (SPosition *value, SStream *stream);
void  ReadTime (SDateTime *value, SStream *stream);
void  ReadTimeDelta (SDateTimeDelta *value, SStream *stream);
void  ReadMessage (SMessage *message, SStream *stream);
void  ReadTag (Tag *tag, SStream *stream);
int	  ReadInvertedTag (Tag *tag, SStream *stream);				// JSDEV* 
void  WriteComment (const char *comment, SStream *stream);
void  WriteTag (Tag tag, SStream *stream);
void  WriteTag (Tag tag, const char *comment, SStream *stream);
void  WriteInt(int *value, SStream *stream);
void  WriteUInt(unsigned int *value, SStream *stream);
void  WriteFloat(float *value, SStream *stream);
void  WriteDouble(double *value, SStream *stream);
void  WriteString(const char *value, SStream *stream);
void  WriteVector(SVector *value, SStream *stream);
void  WritePosition(SPosition *value, SStream *stream);
void  WriteTime(SDateTime *value, SStream *stream);
void  WriteTimeDelta(SDateTimeDelta *value, SStream *stream);
void  WriteMessage(SMessage *message, SStream *stream);

//===================================================================================
//	CDrawByCamera:
//  Is a virtual class for object that may be drawed into small windows by a camera
//===================================================================================
class CDrawByCamera {
public:
  CDrawByCamera() {}
  virtual void PreDraw(CCamera *cam){}
  virtual void CamDraw(CCamera *cam){}
  virtual void EndDraw(CCamera *cam){}
};
//========================================================================
//  RANDOM GENERATOR (SYSTEM DEPENDENT)
//========================================================================
inline int RandomNumber(int mod) {return (rand() % mod);}
//==============================================================================
// CRandomizer:
//  Produce a random number N in the requested range R every tim T.  The 
//  value N is reached in T secondes from the previous one
//  a:     the lower bound
//  a:     The upper bound
//  T:     Time constant to reach the next random value
//==============================================================================
class CRandomizer {
protected:
  //--- ATTRIBUTES -----------------------------------------
  int   amp;                  // Amplitutde
  float vdeb;                 // Deb value
  float aval;                 // Actual value
  float tval;                 // Target value
  float dval;                 // delta value
  float cTim;                 // Time constant
  float timr;                 // Internal timer
  //--- Methods --------------------------------------------
public:
  CRandomizer();
  //--------------------------------------------------------
  void  Set(float db,int amp, float T);     // Start new serie
  void  Range(float  db, int amp);
  float TimeSlice(float dT);                // Time slice
  //---------------------------------------------------------
  inline float GetValue() {return aval;}
};
//===================================================================================
//  CValuator:  Flatten a value according to time and target
//===================================================================================
class CValuator {
protected:
  //--- ATTRIBUTE ------------------------------------------
  char   mode;                            // Mode computing
  float  Targ;                            // Target value
  float  cVal;                            // Current value
  float  timK;                            // Time constant
  //--- METHODS --------------------------------------------
public:
  CValuator(char md, float tm);
  CValuator();
  float   TimeSlice(float dT);
  //--------------------------------------------------------
  inline void   Conf(char md,float tm) {mode = md; timK = tm;}
  inline void   Set(float v)    {Targ = v;}
  inline float  Get()           {return cVal;}
};
//===================================================================================
//	CmHead is the base class for all managed objects in the data
//  base Cache manager 
//===================================================================================
//-----------------------------------------------------------------
//		Define Queue type
//		Add more when needed
//-----------------------------------------------------------------
enum QTYPE {	TIL = 0,					// Tile cache
				      APT = 1,					// Airport
				      VOR = 2,					// VOR
				      NDB	= 3,					// NDB
				      ILS	= 4,					// ILS
				      COM = 5,					// Commm radio
				      RWY	= 6,					// RUNWAYS
				      WPT	= 7,					// Waypoints
              OBN = 8,          // Obstruction
				      QDIM =9,					// Queue size
              //----OTHER OBJECT ------------
              WOB = 20,         // 3D Object       
              OTH = 99,         // Other object
				};
//-----------------------------------------------------------------
//  Define object type
//-----------------------------------------------------------------
enum OTYPE {  ANY = 0x00,       // Any
              SHR = 0x01,       // Shared
              DBM = 0x03,       // DBM
};
//----------SIGNAL TYPE -------------------------------------------
#define SIGNAL_OFF 0
#define SIGNAL_VOR 1
#define SIGNAL_ILS 2
#define SIGNAL_COM 4
#define SIGNAL_WPT 8
//===================================================================================
//  NOTE:  dLon and dLat are working area used for several purposes 
//===================================================================================
class CmHead {
protected:
  friend class ClQueue;
	//------------define state ---------------------------------
#define FREE 0						// Free state
#define ACTV 1						// Active
#define TDEL 2						// To delete
	//------------Attributes -----------------------------------
	pthread_mutex_t		mux;					// Mutex for lock
	U_CHAR				State;					  // State
	U_CHAR				uCount;					  // User count
  U_CHAR        oTyp;             // Object type
  U_CHAR        qAct;             // Index of active queue
	U_INT				NoFrame;				    // Frame stamp
  //------------Distance -------------------------------------
  short         dLon;             // Longitudinal component
  short         dLat;             // Lattitude component
  //------------Cosinus latitude------------------------------
  float         nmFactor;         // 1nm at latitude xx
  //----------------------------------------------------------
	CmHead			*Cnext;					  // queue Q1 next item
	CmHead			*Cprev;					  // queue Q1 previous item
	CmHead			*Tnext;					  // queue Q2 next item
	CmHead			*Tprev;					  // queue Q2 previous item
	//------------------------------------------------------------
public:
	CmHead(OTYPE qo,QTYPE qa);			        // New object
  CmHead();                               // Default constructor
  virtual ~CmHead() {}                    // Virtual destructor
  virtual char      *GetName()        {return 0;}  // Get Name
	virtual char      *GetKey()					{return "NONE";} // Database key
  virtual char      *GetIdent()       {return "";} // ICAO identification
  virtual char      *GetCountry()     {return "";}
  virtual char      *GetEFreq()       {return "";}
	virtual int	       GetType()	      {return 0; }// Object type
  virtual float      GetMagDev()      {return 0; }
  virtual float      GetRadial()      {return 0; }
  virtual float      GetNmiles()      {return 0; }
  virtual float      GetElevation()   {return 0;}
  virtual U_INT      GetRecNo()       {return 0;}
	virtual double     GetRefDirection(){return 0;}
	virtual float      GetVrtDeviation(){return 0;}
	virtual double		 Sensibility()		{return 0;}
	virtual U_CHAR		 SignalType()     {return SIGNAL_OFF;}
	virtual float			 GetFeetDistance(){return 0;}
	//------------------------------------------------------------
  virtual void       Refresh(U_INT FrNo) {return;}
  virtual float      GetLatitude()    {return 0;}
  virtual float      GetLongitude()   {return 0;}
  virtual float      GetFrequency()   {return 0;}
  virtual SPosition  GetPosition();
  virtual SPosition *ObjPosition()    {return 0;}
	virtual SPosition *GetFarPoint()    {return 0;}
	//------------------------------------------------------------
	virtual void			 SetNavOBS(float d)   {;}
	virtual void       SetRefDirection(float d)	{;}
	virtual void			 SetNmiles(float m) {;}
	//------------------------------------------------------------
	        void  IncUser(void);					    // Increment user count
	        void  DecUser();				          // Decrement user count
	        bool  NeedUpdate(U_INT FrNo);		  // Need update
          int   GetRadioIndex();
          void  SetState(U_CHAR sta);
	virtual	void  Trace(char *op,U_INT FrNo= 0,U_INT key= 0){};
  //----inline method ------------------------------------------
  inline int    GetDistLat(void)        {return dLat; }
  inline void   SetDistLat(short lat)   {dLat = lat;  }  
  inline int    GetDistLon(void)        {return dLon; }
  inline void   SetDistLon(short lon)   {dLon = lon;  }
	//------------------------------------------------------------
  inline float  GetNmFactor()           {return nmFactor; }
  inline  CmHead*  NextInQ1()        {return Cnext; }
  inline  CmHead*  PrevInQ1()        {return Cprev; }
  inline  CmHead*  NextInQ2()        {return Tnext; }
  inline QTYPE  GetActiveQ(void)        {return (QTYPE)(qAct); }
  inline OTYPE  GetObjType(void)        {return (OTYPE)(oTyp); }
	//------------------------------------------------------------
  inline void   Lock()                  {pthread_mutex_lock  (&mux);}
  inline void   Unlock()                {pthread_mutex_unlock(&mux);}
  inline bool   IsUsed()                {return (0 != uCount);}
	//-------------------------------------------------------------
	inline bool   IsNot(U_CHAR t)					{return (qAct != t);}
	inline bool   Isa(char t)							{return (qAct == t);}
  //-------------------------------------------------------------
  inline bool   IsShared()              {return (oTyp & SHR);}
};
//=====================================================================
//--------Structure Runway end --------------------------------
struct RWEND { 
    SPosition pos;                  // END Position
    char  rwid[4];                  // Runway ID
		float	ifrq;											// ILS frequency
		char  ilsD[8];									// Frequency edited
    int   dx;                       // Distance to origin
    int   dy;                       // (dito)
    int   cx;                       // Corner coordinate
    int   cy;                       // (dito)
		//----------------------------------------------------
		RWEND::RWEND()
		{	ifrq = 0; ilsD[0] = 0;}
};
//====================================================================
//  Data to control ILS
//====================================================================
typedef struct {
    CILS      *ils;                           // ILS object
    SPosition  lndP;                          // Landing point (on tarmac)
    SPosition  refP;                          // reference point
    SPosition  farP;                          // Far point for drawing
    float      disF;                          // Distance in feet
    float      errG;                          // Glide error (in tan unit)
    float      gTan;                          // Tan of glide slope
    float      altT;                          // altitude above threshold
		//--- LAnding direction ------------------------
		float      lnDIR;												  // Landing direction
    //---Distances to define this ILS --------------
    float      d1;                            // Landing distance
    float      d2;                            // reference distance
    float      d3;                            // Far distance
} ILS_DATA;
//====================================================================
#define RWY_HI_END 0
#define RWY_LO_END 1
//====================================================================
#define SLOT_NAME_DIM (64)
#define SLOT_NO_MVUP  (0x01)        // No move up
#define SLOT_NO_MVDN  (0x02)        // No move down
#define SLOT_NO_INSR  (0x04)        // No insert
#define SLOT_NO_DELT  (0x08)        // No delete
#define SLOT_NO_OPER  (0x0F)        // No operation allowed
//====================================================================
//  CSLot is used for individual line in a list box
//  This is the base class.
//  Each list box should supply a derived class of CSlot
//====================================================================
class CSlot {
  //============SLOT ATTRIBUTES ===================================
 protected:
    //------Sequence number ---------------------------------------
    U_INT        Seq;                             // Slot number    
    //-------------------------------------------------------------
    U_CHAR       Rfu1;                            // Reserved
    U_CHAR       Fixed;                           // 0 slot is dynamic, 1 is fixed
    //-------------------------------------------------------------
    U_CHAR       nLIN;                            // Total line
    U_CHAR       cLIN;                            // Current line
    //-------------------------------------------------------------
    U_LONG       Offset;                          // Record offset
    char         Name[SLOT_NAME_DIM];             // Name of object;
 //--------------Vector to line edition ---------------------------
 public:
   CSlot();
   int    GetTotLines() {return nLIN;}
   int    GetCurLine()  {return cLIN;}
 //-----------Virtual functions --------------------------------
   virtual ~CSlot() {}
   virtual  void    GetPosition(SPosition &p)   {}
   virtual  char *  GetKey()  {return "";}
   virtual  void    Clean() {}
   virtual  bool    Match(void *key)  {return false;}
   virtual  void    Print(CFuiList *w,U_CHAR ln) {}
   virtual  void    Title(CFuiList *w) {Print(w,0);}
   //-----------------------------------------------------------
   virtual  void    SetKey(char *k)   {}
 //-----------Generic access -----------------------------------
  inline   void    SetOFS(U_LONG of)            {Offset = of;}
  inline   U_LONG  GetOFS()                     {return Offset;}
  inline   void    FixeIt()                     {Fixed = 1;} 
  inline   bool    IsNotFixed()                 {return  (Fixed == 0);}
  inline   bool    IsFixed()                    {return  (Fixed != 0);}
  inline   void    SetCurLine(char nl)          {cLIN = nl;}
  inline   void    SetTotLine(char nl)          {nLIN = nl;}
  inline   void    SetSeq(U_INT n)              {Seq  = n;}
  //--------------------------------------------------------------
  inline   void    SetName(char *nm)            {strncpy(Name,nm,SLOT_NAME_DIM); Name[SLOT_NAME_DIM-1] = 0;}
  inline   char   *GetName()                    {return Name;}
  inline   int     GetSeq()                     {return Seq;}
};
//=====================================================================
//  Generic queued item : Base class for all queued items
//=====================================================================
class CqItem {
  friend class CQueue;
  //----------Attributes ---------------------------------------
  CqItem *Next;                            // Next Queued item
  //------------------------------------------------------------
public:
  CqItem() {Next = 0;}
  //------------------------------------------------------------
  inline void         SetNext(CqItem *n)  {Next = n;}
  inline CqItem      *GetNext()           {return Next;}
};
//=====================================================================
//  Generic Queue
//=====================================================================
class CQueue {
protected:
  //-----------Attributes ---------------------------------------
  pthread_mutex_t		mux;					          // Mutex for lock
	U_INT				  NbObj;					            // Item number
	CqItem		   *First;					            // First object in queue
	CqItem		   *Last;					              // Last  object in queue
  CqItem       *Prev;                       // Previous in Queue
  //---------------------------------------------------------------
public:
  CQueue();                                 // Constructor
 ~CQueue();
  //----------------------------------------------------------------
  inline   int NbObjects() {return NbObj;}
  //-------------Queue Management ----------------------------------
  U_INT   Copy(CQueue *q);
  void    Append (CQueue &q);
  void    PutEnd (CqItem *item);
  void    PutHead(CqItem *item);
  CqItem *Detach(CqItem *itm);
  void    Clear();
  void    Flush();
  CqItem *Pop();
  CqItem *GetFirst();
  CqItem *GetLast()             {return Last;}
  CqItem *GetNext(CqItem *itm);
  //-------------Not empty -----------------------------------------
  bool    NotEmpty()            {return (First != 0);}
  bool    IsEmpty()             {return (First == 0);}
};
//=====================================================================
//	Queue Header. Used for queued objects
//=====================================================================
class ClQueue {
protected:
	//--------------Attributes ------------------------------------
	pthread_mutex_t		mux;					          // Mutex for lock
	U_SHORT			NbrOb;					              // Item number
	CmHead		 *First;					              // First object in queue
	CmHead		 *Last;					                // Last  object in queue
public:
  ClQueue();
 ~ClQueue();
  //---------inline ---------------------------------------------
  inline  void        Lock()  {pthread_mutex_lock   (&mux); }
  inline  void        Unlock(){pthread_mutex_unlock (&mux); }
  inline  CmHead*  GetFirst()                {return First;}
  inline  CmHead*  GetLast()                 {return Last; }
  inline  CmHead*  NextInQ1(CmHead *ob)   {return (ob)?(ob->Cnext):(0);}
  inline  CmHead*  PrevInQ1(CmHead *ob)   {return (ob)?(ob->Cprev):(0);}
  inline  U_SHORT     GetNbrObj()               {return NbrOb;}
  inline  bool        NotEmpty()  {return (0 != NbrOb);}
	//------------------------------------------------------------
  void        Clear();
  void        LastInQ1(CmHead *obj);
  void        LastInQ2(CmHead *obj);
  void        HeadInQ1(CmHead *obj);
  void        InsertInQ1(CmHead *nob,CmHead *obj);
  CmHead  *DetachFromQ1(CmHead *obj);
  CmHead  *PopFromQ1();
  CmHead  *PopFromQ2();
};
//=====================================================================
//  SMart pointer to CmHead
//  All components should use this class pointer to access
//  a CILS object allocated  by the DataBase cache Mgr
//=====================================================================
class CObjPtr {
  //----Attributes ---------------------------------------
private:
  CmHead *ptr;
public:
  CObjPtr(CmHead *p);
  CObjPtr();
  CObjPtr(CmHead &n);
 ~CObjPtr();
  //--------Define assignement and access operators ------
  void operator=(CmHead *p);
  void operator=(CmHead &n);
  void operator=(CObjPtr   &q);
  //--------Define Access operator ----------------------
  inline CmHead* operator->() {return ptr;}
  inline bool       operator==(CmHead *obj) {return (ptr == obj);}
  inline bool       Assigned()  {return (ptr != 0);}
  inline bool       IsNull()    {return (0 == ptr);}
  inline CmHead *Pointer()      {return ptr;}
};

//==============================================================================
//  Option 
//==============================================================================
class COption {
protected:
  //---Attribute is the set of options ---------------------------
  U_INT   prop;
public:
  COption::COption() {prop = 0;}
  U_INT Get(U_INT p)    {return (prop & p);}    // Return property
  void  Rep(U_INT p)    {prop  =  p;}           // Replace property
  void  Set(U_INT p)    {prop |= p;}            // Set property
  void  Raz(U_INT p)    {prop &= (-1 - p);}     // Clear property
  void  Toggle(U_INT p);                        // Swap property
  //-------------------------------------------------------------
  char  Has(U_INT p)    {return (prop & p)?(1):(0);}
  char  Not(U_INT p)    {return (prop & p)?(0):(1);}
};
//============================================================================
//  STRUCTURE FOR COAST DETAIL TILE
//============================================================================
struct COAST_VERTEX {
  U_SHORT Nbv;                        // Number of vertices
  U_CHAR  Out;                        // Outside indicator
  U_CHAR  Num;                        // Vertex number
  short   xPix;                       // Pixel X coordinate
  short   zPix;                       // Pixel Z coordinate
};
//=========================================================================
//  Region RECORD
//=========================================================================
typedef struct {
    C_QGT *qgt;
    U_INT key;
    U_INT qtx;                                      // QGT X index
    U_INT qtz;                                      // QGT Z index
    U_INT dtx;                                      // Base X key
    U_INT dtz;                                      // Base Z key
    U_INT lgx;                                      // X-Dimension
    U_INT lgz;                                      // Z-Dimension
    U_INT sub;                                      // Sub division level
    U_INT type;                                     // Region type
    U_INT val;                                      // Value
    int   nbv;                                      // Number of values
    int *data;                                     // Array of elevation
} REGION_REC ;
//-------------------------------------------------------------------------
typedef enum {
  EL_UNIC = 0,
  EL_MONO = 1,
  EL_MULT = 2,
} ElevationEtype;
//=========================================================================
//  COAST RECORD
//=========================================================================
typedef struct {
    U_INT qtk;                                      // QGT key
    U_INT dtk;                                      // QGT detail
    short nbv;                                      // Number of vertices
    U_INT dim;                                      // Dimension
    char *data;                                     // Polygon definitions
} COAST_REC ;

//===================================================================================
//    Structure used for makink continuous text from a list of pointers
//===================================================================================
class TXT_LIST {
  public:
    SFont    *font;                     // Font
    U_INT     color;                    // Text color
    U_SHORT   NbPtr;                    // Number of pointers
    U_SHORT   CuPtr;                    // Current pointer
    U_SHORT   wkPtr;                    // Working index
    char     *data;                     // Current data
    char    **List;                     // Pointer to a list of pointers
    //----------Methods -----------------------------------------------------------
    void    Init(U_SHORT nb, char **lst);
    void    Restart();
    void    Reset();
    char   *GetNextAddr(char *txt);
    void    Increment();
    //-----------------------------------------------------------------------------
    inline  bool  HasMore()       {return (*data != 0);}
};
//=============================================================================
//  Radio Interface
//=============================================================================
//-----------COMPUTED VALUE FOR EXTERNAL GAUGES -----------------------
struct BUS_RADIO {U_CHAR    rnum;       // Radio num
                  U_CHAR    actv;       // Activity
                  U_CHAR    flag;       // Flag type
                  U_CHAR    ntyp;       // Nav Type
                  short     xOBS;       // External OBS
                  double    hREF;       // Lateral reference
                  double    radi;       // ILS/VOR radial
                  double    hDEV;       // Lateral Deviation
                  double    gDEV;       // Glide     Deviation
                  double    mdis;       // Distance in miles
                  double    fdis;       // Distance in feet
                  double    mdev;       // Magnetic deviation
                  double    aDir;       // Aircraft direction (relative to VOR/ILS)
                  double    iAng;       // Intercept angle plane to OBS/ILS
                  double    sens;       // Horizontal sensibility
                  CmHead    *nav;       // Nav object
	//--- Set OBS ----------------------------------------------
	void BUS_RADIO::SetOBS(short dir)
	{ xOBS = dir;
		if (nav)	nav->SetNavOBS(dir);
		return;
	}
	//----------------------------------------------------------
} ;
//=============================================================
//  Structure to describe a waypoint for GPS Bendix King
//=============================================================
typedef struct {
  short    wpNO;                // Number when wp is from a flight plan
  CObjPtr  wpAD;                // Waypoint description
  float    wpRT;                // Desired Route
  float    wpDR;                // Direction To waypoint
} GPSpoint;
//=====================================================================================
//  Define callback functions for Database Cache
//=====================================================================================
typedef void (HdrFunCB(CmHead   *obj,CTileCache *tc));          // Call back for object
typedef void (RwyFunCB(CRunway  *apt,CTileCache *tc));          // Call back for runways
typedef void (LinFunCB(CmHead   *obj,void *pm));                // Call back for LinSlot
typedef void (AptFunCB(CmHead   *obj));                         // Call back for airports
typedef void (PavFunCB(CPaveRWY *pav,CAptObject *apo));         // Call back for Pavement
typedef void (tgxFunCB(CTgxLine *tgx));                         // Call back for generic texture
typedef void (ElvFunCB(REGION_REC &));                          // Call back for elevation
//======================================================================================
/*! \brief "Go To Failure Output" :-) Exit application due to fatal error.
 *
 * Exit the application ("Go To Failure Output"...definitely not "Get the &#^$ out"
 *   as some people might have you believe :-)  This simple class definition allows
 *   varargs to be used to generate the failure message.  The macro below
 *   will automatically fill in the source code filename and line number
 *
 * Thanks to Paul Mclachlan's article at:
 * http://www.codeproject.com/debug/location_trace.asp
 */
//--------------------------------------------------------------------------------------

class GTFO {
private:
  char      m_file[1024];
  int       m_line;

public:
    GTFO (const char* file = NULL, int line = 0) : m_line(line) { strcpy (m_file, file); }
  void operator() (const char *fmt = NULL, ...);
};

#define gtfo (GTFO(__FILE__, __LINE__))

//--------------------------------------------------------------------------
/*! \brief Log a warning about unexpected (but not fatal) situations
 *q
 * If logging is enabled, then referencing the \a WARNINGLOG macro will
 *   result in the appropriate message being written to the log.  Warnings
 *   should be used for situations which are unexpected but not fatal.  It
 *   is expected that end-users may need to refer to the warning log to
 *   troubleshoot why a particular add-on is not working on their system.
 */
//--------------------------------------------------------------------------
class WARN {
private:
  char      m_file[1024];
  int       m_line;

public:
  WARN (const char* file = NULL, int line = 0) : m_line(line) { strcpy (m_file, file); }
  void operator() (const char *fmt = NULL, ...);
};

#define WARNINGLOG (WARN(__FILE__, __LINE__))
//-------------------------------------------------------------------------
//	JSDEV* TERRA log.   This log serve to trace changed tiles
//
//--------------------------------------------------------------------------
class TERRA {
private:
	char msg[1024];
public:
	TERRA(const char *fmt = NULL, ...);
};

//-------------------------------------------------------------------------
//	JSDEV* Trace log.   This log serve to trace various process
//
//--------------------------------------------------------------------------
class TRACE {
private:
	char msg[1024];
public:
	TRACE(const char *fmt = NULL, ...);
};
//-------------------------------------------------------------------------
/*! \brief Log a debugging message
 *
 * If debug logging is enabled, then referencing the \a DEBUGLOG macro will
 *   result in the message being written to the debug log.  Debug messages
 *   are only intended for use by the development team and would not typically
 *   be used by end-users.
 */
class DEBUG {
private:
  char      m_file[1024];
  int       m_line;

public:
  DEBUG (const char* file = NULL, int line = 0) : m_line(line) { strcpy (m_file, file); }
  void operator() (const char *fmt = NULL, ...);
};

#define DEBUGLOG (DEBUG(__FILE__, __LINE__))



#if defined(_DEBUG) && defined(HAVE_CRTDBG_H)
//
// Declare global for memory state debugging, defined in Main.cpp
//
extern _CrtMemState memoryState;
#endif // _DEBUG

//===============================================================================================
// Application API
//===============================================================================================
void InitApplication (void);
void ExitApplication (void);

void InitSimulation (void);
void CleanupSimulation (void);
int  RedrawSimulation ();

void InitSplashScreen (void);
void RedrawSplashScreen (void);
void CleanupSplashScreen (void);

void InitExitScreen (void);
void RedrawExitScreen (void);
void CleanupExitScreen (void);

//
// Window Manager API
//
void InitWindowManager (int argc, char **argv);
void EnterWindowManagerMainLoop (void);
void CleanupWindowManager (void);

#endif  // FLYLEGACY_H

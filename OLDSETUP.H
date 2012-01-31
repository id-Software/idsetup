#define FRENCH       // define if FRENCH VERSION

#ifdef FRENCH

#include "french2\idcard.i"     //
//#include "french2\gusdma.i"    //
//#include "french2\gusirq.i"    //
//#include "french2\gusport.i"   //
#include "french2\mcard.i"     //
//#include "french2\midiirq.i"
#include "french2\midiport.i"  //
#include "french2\numdig.i"
#include "french2\sbdma.i"     //
#include "french2\sbirq.i"     //
#include "french2\sbport.i"    //
#include "french2\control.i"   //
#include "french2\show.i"
#include "french2\consel.i"
#include "french2\quitwin.i"
#include "french2\netsave.i"
#include "french2\netplay.i"
#include "french2\macros.i"
#include "french2\modsave.i"
#include "french2\sersave.i"
#include "french2\netplay2.i"

#define S_NONE "AUCUNE"
#define S_PCSP "Haut-parleur PC"
#define S_CON1 "Clavier seul"
#define S_CON2 "Clavier + Souris"
#define S_CON3 "Clavier + Joystick"

#else

#include "idcard.i"     //
#include "gusdma.i"    //
#include "gusirq.i"    //
#include "gusport.i"   //
#include "mcard.i"     //
#include "midiirq.i"
#include "midiport.i"  //
#include "numdig.i"
#include "sbdma.i"     //
#include "sbirq.i"     //
#include "sbport.i"    //
#include "control.i"   //
#include "show.i"
#include "consel.i"
#include "quitwin.i"
#include "netsave.i"
#include "gusirq.i"
#include "netplay.i"
#include "macros.i"
#include "modsave.i"
#include "sersave.i"
#include "netplay2.i"

#define S_NONE "None"
#define S_PCSP "PC Speaker"
#define S_CON1 "Keyboard only"
#define S_CON2 "Keyboard + Mouse"
#define S_CON3 "Keyboard + Joystick"

#endif

#ifdef DOOM2

#ifdef FRENCH
#include "french2\idmain2.i"
#include "french2\cmodem.i"
#include "french2\cserial.i"
#include "french2\cwarp.i"
#include "french2\netwk2.i"
#else
#include "idmain2.i"
#include "cmodem.i"
#include "cserial.i"
#include "cwarp.i"
#include "netwk2.i"
#endif

#else

#include "idmain.i"
#include "warp.i"
#include "modem.i"
#include "serial.i"
#include "network.i"

#endif

#include "keys.h"

typedef enum { FALSE, TRUE }	BOOL;

typedef enum
{
   M_NONE,
   M_PC,
   M_ADLIB,
   M_SB,
   M_PAS,
   M_GUS,
   M_WAVE,
   M_CANVAS,
   M_GMIDI,
   M_SBAWE32,
   M_LAST
}SCARD;
  
typedef enum
{
   C_KEY,
   C_MOUSE,
   C_JOYSTICK,
   C_LAST
}CONTROL;
  
typedef struct
{
   SCARD card;
   short port;
   short midiport;
   short irq;
   short dma;
} DMXCARD;

typedef struct
{
   CONTROL control;
   DMXCARD m;
   DMXCARD d;
   short numdig;
} DMXINFO;

typedef struct
{
   int   numberOfPlayers;
   int   skillLevel;
   int   deathMatch;
   unsigned  networkSocket;
   int   episode;
} net_t;

typedef struct
{
   int   skillLevel;
   int   deathMatch;
   int   episode;
   int   comport;
   int   comtype;  // 0 = connected,1 = answer,2 = call
   char  phonenum[16];
} serial_t;

typedef struct
{
   int      up;
   int      down;
   int      left;
   int      right;
   int      fire;
   int      use;     // ID = USE         CYGNUS = USE SPECIAL WEAPON
   int      key1;    // ID = STRAFE ON   CYGNUS = CHANGE SPECIAL WEAPON
   int      key2;    // ID = SPEED ON    CYGNUS = MEGA BOMB HOT KEY
   int      key3;    // ID = STRAFE LEFT
   int      key4;    // ID = STRAFE RIGHT
   int      mouse[3];
   int      joy[4];
} CONTS;

typedef enum
{
   ID_FIRE,        // def button 1
   ID_STRAFE,      // def button 3
   ID_FORWARD,     // def button 2
   ID_USE          // def button 4 joystick only
}IDCONTS;

extern int  usemouse;
extern int  usejoystick;

typedef struct
{
   char  name[64];
   char  init[64];
   char  hangup[64];
   char  baud[8];
} modem_t;

#define MAXMODEMS 200

typedef struct
{
   char  name[32];
   char  number[16];
} phonelist_t;

#define MAXPHNLIST 40

extern   int   cdrom;
extern   char **myargv;
extern   int   myargc;


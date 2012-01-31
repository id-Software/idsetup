

//===================================================
//
// Enter modem savegame info
//
//===================================================
int RestoreModem(void)
{
   char * wstr = "MODSAVE";
   short *window;
   short field;
   int   rval = 0;
   char  *args[MAXARGS];
   int   argcount;
   char  string[MAXARGS*2][16];
   union REGS r;
   int   i;
   int   numplayers[6];
   int   saveslot;
   int   handle;
   char  name[32];
	char  p1,p2,p3,p4;


   minfo = modeminfo;       // save copy of netinfo

   field = MS_SAVEGAME0;        // FIRST FIELD TO LIGHT BAR
  
   if ( ( window = IWD_OpenWindow( NULL, wstr ) ) == NULL )
   {
      IWD_CloseWRF ( NULL );
      printf( "\nError opening %s window.\n", wstr );
      exit(1);
   }
  
   saveslot = -1;
   memset(savenames,0,6*24);

   //
   // Read in savegame strings
   //
   for (i = 0;i < 6;i++)
   {
      sprintf(name,SAVENAME,i);
      handle = open (name, O_BINARY | O_RDONLY);
		if (handle == -1)
		   continue;

		read(handle,savenames[i],24);
      lseek(handle,27+16,SEEK_SET);
      read(handle,&p1,1);
      read(handle,&p2,1);
      read(handle,&p3,1);
      read(handle,&p4,1);
      numplayers[i] = p1+p2+p3+p4;
      close(handle);

      IWD_PutFieldData(window,MS_SAVEGAME0+i,savenames[i]);
   }
  	                 	
   //
   // Set defaults
   //
   IWD_SetFieldMark(window,MS_CONTYPE0M,0);
   IWD_SetFieldMark(window,MS_CONTYPE1M,1);
   IWD_SetFieldMark(window,MS_CONTYPE2M,0);
   minfo.comtype = 1;

   IWD_SetFieldMark(window,MS_COM1M,1);
   IWD_SetFieldMark(window,MS_COM2M,0);
   IWD_SetFieldMark(window,MS_COM3M,0);
   IWD_SetFieldMark(window,MS_COM4M,0);
   minfo.comport = 1;
   
   IWD_SetFieldMark(window,MS_DEATHNOM2,1);
   IWD_SetFieldMark(window,MS_DEATHYESM2,0);
   minfo.deathMatch = 0;

   strcpy(minfo.phonenum,"");
   IWD_PutFieldDataType(window,MS_PHONENUM, FTYPE_STRING,&minfo.phonenum);

   IWD_ShowWindow ( window );

   for (;;)
   {
      switch ( IWD_Dialog ( window, &field, DialogControl, hot_keys ) )
      {
         case KEY_ESC:
            rval = -1;
            goto func_exit;
  
exitandsave:
         case KEY_F10:

            if (saveslot < 0)
            {
               short *window;

               if ( ( window = IWD_OpenWindow( NULL, "NETSERR" ) ) == NULL )
                  exit(1);
               IWD_ShowWindow ( window );
               while(kbhit());
                  getch();
               IWD_CloseWindow( window );
               break;
            }

            IWD_GetFieldDataType(window,MS_PHONENUM,FTYPE_STRING,&minfo.phonenum);
            modeminfo = minfo;
            
            M_SaveDefaults();
            IWD_CloseWRF( NULL );
            IWD_Shutdown();

            argcount = 1;

            args[0] = "sersetup.exe ";

               if (cdrom)
                  args[argcount++] = "-cdrom";

            args[argcount++] = "-nodes";
            sprintf(string[argcount],"%d",numplayers[saveslot]);
            args[argcount] = string[argcount];
            argcount++;

            if (modeminfo.deathMatch)
               args[argcount++] = "-deathmatch";

            sprintf(string[argcount],"-com%d",modeminfo.comport);
            args[argcount] = string[argcount];
            argcount++;
            
            switch(modeminfo.comtype)
            {
               case 0:     // no parameter if already connected!
                  break;
               case 1:
                  args[argcount++] = "-answer";
                  break;
               case 2:
                  args[argcount++] = "-dial";
                  sprintf(string[argcount],"%s",minfo.phonenum);
                  args[argcount] = string[argcount];
                  argcount++;
                  break;
            }

            args[argcount++] = "-loadgame";
            sprintf(string[argcount],"%d",saveslot);
            args[argcount] = string[argcount];
            argcount++;
            
            for (i = 1;i < myargc; i++)
               args[argcount++] = myargv[i];

            args[argcount] = NULL;

            r.x.ax = 3;
            int86(0x10,&r,&r);

            execv("sersetup.exe",args);

            //
            // ERROR EXECing!
            //
            printf("Problem EXECing SERSETUP for netplay. Need to be in same directory!");
            exit(0);

         case KEY_CR:
         case 32:
         switch ( field )
         {
            case MS_COM1:
               IWD_SetFieldMark(window,MS_COM1M,1);
               IWD_SetFieldMark(window,MS_COM2M,0);
               IWD_SetFieldMark(window,MS_COM3M,0);
               IWD_SetFieldMark(window,MS_COM4M,0);
               minfo.comport = 1;
               break;

            case MS_COM2:
               IWD_SetFieldMark(window,MS_COM1M,0);
               IWD_SetFieldMark(window,MS_COM2M,1);
               IWD_SetFieldMark(window,MS_COM3M,0);
               IWD_SetFieldMark(window,MS_COM4M,0);
               minfo.comport = 2;
               break;

            case MS_COM3:
               IWD_SetFieldMark(window,MS_COM1M,0);
               IWD_SetFieldMark(window,MS_COM2M,0);
               IWD_SetFieldMark(window,MS_COM3M,1);
               IWD_SetFieldMark(window,MS_COM4M,0);
               minfo.comport = 3;
               break;

            case MS_COM4:
               IWD_SetFieldMark(window,MS_COM1M,0);
               IWD_SetFieldMark(window,MS_COM2M,0);
               IWD_SetFieldMark(window,MS_COM3M,0);
               IWD_SetFieldMark(window,MS_COM4M,1);
               minfo.comport = 4;
               break;
               

            case MS_CONTYPE0:
               IWD_SetFieldMark(window,MS_CONTYPE0M,1);
               IWD_SetFieldMark(window,MS_CONTYPE1M,0);
               IWD_SetFieldMark(window,MS_CONTYPE2M,0);
               minfo.comtype = 0;
               break;

            case MS_CONTYPE1:
               IWD_SetFieldMark(window,MS_CONTYPE0M,0);
               IWD_SetFieldMark(window,MS_CONTYPE1M,1);
               IWD_SetFieldMark(window,MS_CONTYPE2M,0);
               minfo.comtype = 1;
               break;

            case MS_CONTYPE2:
               IWD_SetFieldMark(window,MS_CONTYPE0M,0);
               IWD_SetFieldMark(window,MS_CONTYPE1M,0);
               IWD_SetFieldMark(window,MS_CONTYPE2M,1);
               minfo.comtype = 2;
               field = MS_PHONENUM;
               break;


            case MS_SAVEGAME0:
               if (!savenames[0][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 0;
               IWD_SetFieldMark(window,MS_SAVEGAME0M,1);
               IWD_SetFieldMark(window,MS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME5M,0);
               break;

            case MS_SAVEGAME1:
               if (!savenames[1][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 1;
               IWD_SetFieldMark(window,MS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME1M,1);
               IWD_SetFieldMark(window,MS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME5M,0);
               break;


            case MS_SAVEGAME2:
               if (!savenames[2][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 2;
               IWD_SetFieldMark(window,MS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME2M,1);
               IWD_SetFieldMark(window,MS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME5M,0);
               break;

            case MS_SAVEGAME3:
               if (!savenames[3][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 3;
               IWD_SetFieldMark(window,MS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME3M,1);
               IWD_SetFieldMark(window,MS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME5M,0);
               break;

            case MS_SAVEGAME4:
               if (!savenames[4][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 4;
               IWD_SetFieldMark(window,MS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME4M,1);
               IWD_SetFieldMark(window,MS_SAVEGAME5M,0);
               break;

            case MS_SAVEGAME5:
               if (!savenames[5][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 5;
               IWD_SetFieldMark(window,MS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,MS_SAVEGAME5M,1);
               break;

            //
            // Deathmatch
            //
            case MS_DEATHNO2:
               minfo.deathMatch = 0;
               IWD_SetFieldMark(window,MS_DEATHNOM2,1);
               IWD_SetFieldMark(window,MS_DEATHYESM2,0);
               break;

            case MS_DEATHYES2:
               minfo.deathMatch = 1;
               IWD_SetFieldMark(window,MS_DEATHNOM2,0);
               IWD_SetFieldMark(window,MS_DEATHYESM2,1);
               break;

            default:
               break;
         }
            break;
      }
   }
  
   func_exit:
  
   IWD_CloseWindow(window);
  
   return ( rval );
}


//===================================================
//
// Enter serial-link savegame info
//
//===================================================
int RestoreSerial(void)
{
   char * wstr = "SERSAVE";
   short *window;
   short field;
   int   rval = 0;
   char  *args[MAXARGS];
   int   argcount;
   char  string[MAXARGS*2][16];
   union REGS r;
   int   i;
   int   numplayers[6];
   int   saveslot;
   int   handle;
   char  name[32];
   char  p1,p2,p3,p4;


   sinfo = serialinfo;       // save copy of netinfo

   field = SS_SAVEGAME0;        // FIRST FIELD TO LIGHT BAR
  
   if ( ( window = IWD_OpenWindow( NULL, wstr ) ) == NULL )
   {
      IWD_CloseWRF ( NULL );
      printf( "\nError opening %s window.\n", wstr );
      exit(1);
   }
  
   saveslot = -1;
   memset(savenames,0,6*24);

   //
   // Read in savegame strings
   //
   for (i = 0;i < 6;i++)
   {
      sprintf(name,SAVENAME,i);
      handle = open (name, O_BINARY | O_RDONLY);
		if (handle == -1)
		   continue;

      read(handle,savenames[i],24);
      lseek(handle,27+16,SEEK_SET);
      read(handle,&p1,1);
      read(handle,&p2,1);
      read(handle,&p3,1);
      read(handle,&p4,1);
      numplayers[i] = p1+p2+p3+p4;
      close(handle);

      IWD_PutFieldData(window,SS_SAVEGAME0+i,savenames[i]);
   }
  	                 	
   //
   // Set defaults
   //
   IWD_SetFieldMark(window,SS_COM1M,1);
   IWD_SetFieldMark(window,SS_COM2M,0);
   IWD_SetFieldMark(window,SS_COM3M,0);
   IWD_SetFieldMark(window,SS_COM4M,0);
   sinfo.comport = 1;
   
   IWD_SetFieldMark(window,SS_DEATHNOM2,1);
   IWD_SetFieldMark(window,SS_DEATHYESM2,0);
   sinfo.deathMatch = 0;

   IWD_ShowWindow ( window );

   for (;;)
   {
      switch ( IWD_Dialog ( window, &field, DialogControl, hot_keys ) )
      {
         case KEY_ESC:
            rval = -1;
            goto func_exit;
  
exitandsave:
         case KEY_F10:

            if (saveslot < 0)
            {
               short *window;

               if ( ( window = IWD_OpenWindow( NULL, "NETSERR" ) ) == NULL )
                  exit(1);
               IWD_ShowWindow ( window );
               while(kbhit());
                  getch();
               IWD_CloseWindow( window );
               break;
            }

            serialinfo = sinfo;
            
            M_SaveDefaults();
            IWD_CloseWRF( NULL );
            IWD_Shutdown();

            argcount = 1;

            args[0] = "sersetup.exe ";

               if (cdrom)
                  args[argcount++] = "-cdrom";

            args[argcount++] = "-nodes";
            sprintf(string[argcount],"%d",numplayers[saveslot]);
            args[argcount] = string[argcount];
            argcount++;

            if (modeminfo.deathMatch)
               args[argcount++] = "-deathmatch";

            sprintf(string[argcount],"-com%d",serialinfo.comport);
            args[argcount] = string[argcount];
            argcount++;

            args[argcount++] = "-loadgame";
            sprintf(string[argcount],"%d",saveslot);
            args[argcount] = string[argcount];
            argcount++;
            
            for (i = 1;i < myargc; i++)
               args[argcount++] = myargv[i];

            args[argcount] = NULL;

            r.x.ax = 3;
            int86(0x10,&r,&r);

            execv("sersetup.exe",args);

            //
            // ERROR EXECing!
            //
            printf("Problem EXECing SERSETUP for netplay. Need to be in same directory!");
            exit(0);

         case KEY_CR:
         case 32:
         switch ( field )
         {
            case SS_COM1:
               IWD_SetFieldMark(window,SS_COM1M,1);
               IWD_SetFieldMark(window,SS_COM2M,0);
               IWD_SetFieldMark(window,SS_COM3M,0);
               IWD_SetFieldMark(window,SS_COM4M,0);
               sinfo.comport = 1;
               break;

            case SS_COM2:
               IWD_SetFieldMark(window,SS_COM1M,0);
               IWD_SetFieldMark(window,SS_COM2M,1);
               IWD_SetFieldMark(window,SS_COM3M,0);
               IWD_SetFieldMark(window,SS_COM4M,0);
               sinfo.comport = 2;
               break;

            case SS_COM3:
               IWD_SetFieldMark(window,SS_COM1M,0);
               IWD_SetFieldMark(window,SS_COM2M,0);
               IWD_SetFieldMark(window,SS_COM3M,1);
               IWD_SetFieldMark(window,SS_COM4M,0);
               sinfo.comport = 3;
               break;

            case SS_COM4:
               IWD_SetFieldMark(window,SS_COM1M,0);
               IWD_SetFieldMark(window,SS_COM2M,0);
               IWD_SetFieldMark(window,SS_COM3M,0);
               IWD_SetFieldMark(window,SS_COM4M,1);
               sinfo.comport = 4;
               break;
               

            case SS_SAVEGAME0:
               if (!savenames[0][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 0;
               IWD_SetFieldMark(window,SS_SAVEGAME0M,1);
               IWD_SetFieldMark(window,SS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME5M,0);
               break;

            case SS_SAVEGAME1:
               if (!savenames[1][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 1;
               IWD_SetFieldMark(window,SS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME1M,1);
               IWD_SetFieldMark(window,SS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME5M,0);
               break;


            case SS_SAVEGAME2:
               if (!savenames[2][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 2;
               IWD_SetFieldMark(window,SS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME2M,1);
               IWD_SetFieldMark(window,SS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME5M,0);
               break;

            case SS_SAVEGAME3:
               if (!savenames[3][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 3;
               IWD_SetFieldMark(window,SS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME3M,1);
               IWD_SetFieldMark(window,SS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME5M,0);
               break;

            case SS_SAVEGAME4:
               if (!savenames[4][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 4;
               IWD_SetFieldMark(window,SS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME4M,1);
               IWD_SetFieldMark(window,SS_SAVEGAME5M,0);
               break;

            case SS_SAVEGAME5:
               if (!savenames[5][0])
               {
                  IWD_Sound(1000,6);
                  break;
               }
               saveslot = 5;
               IWD_SetFieldMark(window,SS_SAVEGAME0M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME1M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME2M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME3M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME4M,0);
               IWD_SetFieldMark(window,SS_SAVEGAME5M,1);
               break;

            //
            // Deathmatch
            //
            case SS_DEATHNO2:
               sinfo.deathMatch = 0;
               IWD_SetFieldMark(window,SS_DEATHNOM2,1);
               IWD_SetFieldMark(window,SS_DEATHYESM2,0);
               break;

            case SS_DEATHYES2:
               sinfo.deathMatch = 1;
               IWD_SetFieldMark(window,SS_DEATHNOM2,0);
               IWD_SetFieldMark(window,SS_DEATHYESM2,1);
               break;

            default:
               break;
         }
            break;
      }
   }
  
   func_exit:
  
   IWD_CloseWindow(window);
  
   return ( rval );
}




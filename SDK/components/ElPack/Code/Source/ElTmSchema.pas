{====================================================}
{                                                    }
{   EldoS Visual Components                          }
{                                                    }
{   Copyright (c) 2001, Microsoft                    }
{   Delphi Translation (c) 2001, EldoS               }
{                                                    }
{====================================================}

{$include elpack2.inc}
{$ifdef ELPACK_SINGLECOMP}
{$I ElPack.inc}
{$else}
{$ifdef LINUX}
{$I ../ElPack.inc}
{$else}
{$I ..\ElPack.inc}
{$endif}
{$endif}

unit ElTmSchema;

interface

const

  BT_IMAGEFILE  = 0;
  BT_BORDERFILL = 1;
  BT_NTLFILE    = 2;
  BT_NONE       = 3;

  IL_HORIZONTAL = 0;
  IL_VERTICAL   = 1;

  BT_RECT       = 0;
  BT_ROUNDRECT  = 1;
  BT_ELLIPSE    = 2;

  FT_SOLID         = 0;
  FT_VERTGRADIENT  = 1;
  FT_HORZGRADIENT  = 2;
  FT_RADIALGRADIENT= 3;
  FT_TILEIMAGE     = 4;

  ST_TRUESIZE      = 0;
  ST_STRETCH       = 1;
  ST_TILE          = 2;

  HA_LEFT          = 0;
  HA_CENTER        = 1;
  HA_RIGHT         = 2;

  CA_LEFT          = 0;
  CA_CENTER        = 1;
  CA_RIGHT         = 2;

  VA_TOP           = 0;
  VA_CENTER        = 1;
  VA_BOTTOM        = 2;

  OT_TOPLEFT       = 0;
  OT_TOPRIGHT      = 1;
  OT_TOPMIDDLE     = 2;
  OT_BOTTOMLEFT    = 3;
  OT_BOTTOMRIGHT   = 4;
  OT_BOTTOMMIDDLE  = 5;
  OT_MIDDLELEFT    = 6;
  OT_MIDDLERIGHT   = 7;
  OT_LEFTOFCAPTION = 8;
  OT_RIGHTOFCAPTION= 9;
  OT_LEFTOFLASTBUTTON = 10;
  OT_RIGHTOFLASTBUTTON= 11;
  OT_ABOVELASTBUTTON  = 12;
  OT_BELOWLASTBUTTON  = 13;

  ICE_NONE            = 0;
  ICE_GLOW            = 1;
  ICE_SHADOW          = 2;
  ICE_PULSE           = 3;
  ICE_ALPHA           = 4;

  TST_NONE            = 0;
  TST_SINGLE          = 1;
  TST_CONTINUOUS      = 2;

  GT_NONE             = 0;
  GT_IMAGEGLYPH       = 1;
  GT_FONTGLYPH        = 2;

  IST_NONE            = 0;
  IST_SIZE            = 1;
  IST_DPI             = 2;

  TSST_NONE           = 0;
  TSST_SIZE           = 1;
  TSST_DPI            = 2;

  GFST_NONE           = 0;
  GFST_SIZE           = 1;
  GFST_DPI            = 2;

  TMT_BASE200         = 200;
  TMT_STRING          = 201;
  TMT_INT             = 202;
  TMT_BOOL            = 203;
  TMT_COLOR           = 204;
  TMT_MARGINS         = 205;
  TMT_FILENAME        = 206;
  TMT_SIZE            = 207;
  TMT_POSITION        = 208;
  TMT_RECT            = 209;
  TMT_FONT            = 210;
  TMT_INTLIST         = 211;

  TMT_BASE400         = 400;
  TMT_COLORSCHEMES    = 401;
  TMT_SIZES           = 402;
  TMT_CHARSET         = 403;
                   
  TMT_BASE600         = 600;
  TMT_DISPLAYNAME     = 601;
  TMT_TOOLTIP         = 602;
  TMT_COMPANY         = 603;
  TMT_AUTHOR          = 604;
  TMT_COPYRIGHT       = 605;
  TMT_URL             = 606;
  TMT_VERSION         = 607;
  TMT_DESCRIPTION     = 608;

  TMT_FIRST_RCSTRING_NAME = TMT_DISPLAYNAME;
  TMT_LAST_RCSTRING_NAME  = TMT_DESCRIPTION;

  TMT_BASE800         = 800;
  TMT_CAPTIONFONT     = 801;
  TMT_SMALLCAPTIONFONT= 802;
  TMT_MENUFONT        = 803;
  TMT_STATUSFONT      = 804;
  TMT_MSGBOXFONT      = 805;
  TMT_ICONTITLEFONT   = 806;

  TMT_FIRSTFONT       = TMT_CAPTIONFONT;
  TMT_LASTFONT        = TMT_ICONTITLEFONT;

  TMT_BASE1000        = 1000;
  TMT_FLATMENUS       = 1001;
  TMT_SYSTEMSIZE      = 1002;
  TMT_SYSTEMPOS       = 1003;

  TMT_FIRSTBOOL       = TMT_FLATMENUS;
  TMT_LASTBOOL        = TMT_FLATMENUS;

  TMT_BASE1200          = 1200;      
  TMT_BORDERWIDTH       = 1201;
  TMT_SCROLLBARWIDTH    = 1202;
  TMT_SCROLLBARHEIGHT   = 1203;
  TMT_CAPTIONBARWIDTH   = 1204;
  TMT_CAPTIONBARHEIGHT  = 1205;
  TMT_SMCAPTIONBARWIDTH = 1206;
  TMT_SMCAPTIONBARHEIGHT= 1207;
  TMT_MENUBARWIDTH      = 1208;
  TMT_MENUBARHEIGHT     = 1209;

  TMT_FIRSTSIZE         = TMT_BORDERWIDTH;
  TMT_LASTSIZE          = TMT_MENUBARHEIGHT;

  TMT_BASE1300          = 1300;
  TMT_DPIX              = 1301;
  TMT_DPIY              = 1302;
  TMT_MINCOLORDEPTH     = 1303;

  TMT_FIRSTINT          = TMT_DPIX;
  TMT_LASTINT           = TMT_MINCOLORDEPTH;

  TMT_BASE1400          = 1400;
  TMT_CSSNAME           = 1401;
  TMT_XMLNAME           = 1402;

  TMT_FIRSTSTRING       = TMT_CSSNAME;
  TMT_LASTSTRING        = TMT_XMLNAME;

  //---- theme metrics: colors ----

  TMT_BASE1600            = 1600;
  TMT_SCROLLBAR           = 1601;
  TMT_BACKGROUND          = 1602;
  TMT_ACTIVECAPTION       = 1603;
  TMT_INACTIVECAPTION     = 1604;
  TMT_MENU                = 1605;
  TMT_WINDOW              = 1606;
  TMT_WINDOWFRAME         = 1607;
  TMT_MENUTEXT            = 1608;
  TMT_WINDOWTEXT          = 1609;
  TMT_CAPTIONTEXT         = 1610;
  TMT_ACTIVEBORDER        = 1611;
  TMT_INACTIVEBORDER      = 1612;
  TMT_APPWORKSPACE        = 1613;
  TMT_HIGHLIGHT           = 1614;
  TMT_HIGHLIGHTTEXT       = 1615;
  TMT_BTNFACE             = 1616;
  TMT_BTNSHADOW           = 1617;
  TMT_GRAYTEXT            = 1618;
  TMT_BTNTEXT             = 1619;
  TMT_INACTIVECAPTIONTEXT = 1620;
  TMT_BTNHIGHLIGHT        = 1621;
  TMT_DKSHADOW3D          = 1622;
  TMT_LIGHT3D             = 1623;
  TMT_INFOTEXT            = 1624;
  TMT_INFOBK              = 1625;
  TMT_BUTTONALTERNATEFACE = 1626;
  TMT_HOTTRACKING         = 1627;
  TMT_GRADIENTACTIVECAPTION   = 1628;
  TMT_GRADIENTINACTIVECAPTION = 1629;
  TMT_MENUHILIGHT             = 1630;
  TMT_MENUBAR                 = 1631;

  TMT_FIRSTCOLOR = TMT_SCROLLBAR;
  TMT_LASTCOLOR  = TMT_MENUBAR;

  //---- hue substitutions ----

  TMT_BASE1800 = 1800;
  TMT_FROMHUE1 = 1801;
  TMT_FROMHUE2 = 1802;
  TMT_FROMHUE3 = 1803;
  TMT_FROMHUE4 = 1804;
  TMT_FROMHUE5 = 1805;
  TMT_TOHUE1   = 1806;
  TMT_TOHUE2   = 1807;
  TMT_TOHUE3   = 1808;
  TMT_TOHUE4   = 1809;
  TMT_TOHUE5   = 1810;

  //---- color substitutions ----

  TMT_BASE2000   = 2000;
  TMT_FROMCOLOR1 = 2001;
  TMT_FROMCOLOR2 = 2002;
  TMT_FROMCOLOR3 = 2003;
  TMT_FROMCOLOR4 = 2004;
  TMT_FROMCOLOR5 = 2005;
  TMT_TOCOLOR1   = 2006;
  TMT_TOCOLOR2   = 2007;
  TMT_TOCOLOR3   = 2008;
  TMT_TOCOLOR4   = 2009;
  TMT_TOCOLOR5   = 2010;

  //---- rendering BOOL properties ----

  TMT_BASE2200            = 2200;
  TMT_TRANSPARENT         = 2201;
  TMT_AUTOSIZE            = 2202;
  TMT_BORDERONLY          = 2203;
  TMT_COMPOSITED          = 2204;
  TMT_BGFILL              = 2205;
  TMT_GLYPHTRANSPARENT    = 2206;
  TMT_GLYPHONLY           = 2207;
  TMT_ALWAYSSHOWSIZINGBAR = 2208;
  TMT_MIRRORIMAGE         = 2209;
  
  TMT_UNIFORMSIZING       = 2210;
  TMT_INTEGRALSIZING      = 2211;
  TMT_SOURCEGROW          = 2212;
  TMT_SOURCESHRINK        = 2213;
  

  (*
  TMT_CACHEBRUSHES        = 2210;
  TMT_SOLIDBORDER         = 2211;
  TMT_SOLIDCONTENT        = 2212;
  TMT_DIRECTBITS          = 2213;
  TMT_MANUALTILING        = 2214;
  TMT_DIRECTBRUSH         = 2215;
  *)
  
  //---- rendering INT properties ----

  TMT_BASE2400            = 2400;
  TMT_IMAGECOUNT          = 2401;
  TMT_ALPHALEVEL          = 2402;
  TMT_BORDERSIZE          = 2403;
  TMT_ROUNDCORNERWIDTH    = 2404;
  TMT_ROUNDCORNERHEIGHT   = 2405;
  TMT_GRADIENTRATIO1      = 2406;
  TMT_GRADIENTRATIO2      = 2407;
  TMT_GRADIENTRATIO3      = 2408;
  TMT_GRADIENTRATIO4      = 2409;
  TMT_GRADIENTRATIO5      = 2410;
  TMT_PROGRESSCHUNKSIZE   = 2411;
  TMT_PROGRESSSPACESIZE   = 2412;
  TMT_SATURATION          = 2413;
  TMT_TEXTBORDERSIZE      = 2414;
  TMT_ALPHATHRESHOLD      = 2415;
  TMT_WIDTH               = 2416;
  TMT_HEIGHT              = 2417;
  TMT_NORMALINDEX         = 2418;
  TMT_GLYPHINDEX          = 2419;

  TMT_TRUESIZESTRETCHMARK = 2419;
  TMT_MINDPI1             = 2420;
  TMT_MINDPI2             = 2421;
  TMT_MINDPI3             = 2422;
  TMT_MINDPI4             = 2423;
  TMT_MINDPI5             = 2424;

  TMT_BASE2600            = 2600;
  TMT_GLYPHFONT           = 2601;

  TMT_BASE2800            = 2800;
  TMT_FILESIZES           = 2801;

  TMT_BASE3000            = 3000;
  TMT_IMAGEFILE           = 3001;
  TMT_IMAGEFILE1          = 3002;
  TMT_IMAGEFILE2          = 3003;
  TMT_IMAGEFILE3          = 3004;
  TMT_IMAGEFILE4          = 3005;
  TMT_IMAGEFILE5          = 3006;
  TMT_STOCKIMAGEFILE      = 3007;
  TMT_GLYPHIMAGEFILE      = 3008;

  TMT_BASE3200            = 3200;
  TMT_TEXT                = 3201;


  TMT_BASE3400            = 3400;
  TMT_OFFSET              = 3401;
  TMT_TEXTSHADOWOFFSET    = 3402;
  TMT_MINSIZE             = 3403;
  TMT_MINSIZE1            = 3404;
  TMT_MINSIZE2            = 3405;
  TMT_MINSIZE3            = 3406;
  TMT_MINSIZE4            = 3407;
  TMT_MINSIZE5            = 3408;
  TMT_NORMALSIZE          = 3409;

  //---- rendering MARGIN properties ----

  TMT_BASE36000           = 3600;
  TMT_SIZINGMARGINS       = 3601;
  TMT_CONTENTMARGINS      = 3602;
  TMT_CAPTIONMARGINS      = 3603;

  //---- rendering COLOR properties ----

  TMT_BASE3800            = 3800;
  TMT_BORDERCOLOR         = 3801;
  TMT_FILLCOLOR           = 3802;
  TMT_TEXTCOLOR           = 3803;
  TMT_EDGELIGHTCOLOR      = 3804;
  TMT_EDGEHIGHLIGHTCOLOR  = 3805;
  TMT_EDGESHADOWCOLOR     = 3806;
  TMT_EDGEDKSHADOWCOLOR   = 3807;
  TMT_EDGEFILLCOLOR       = 3808;
  TMT_TRANSPARENTCOLOR    = 3809;
  TMT_GRADIENTCOLOR1      = 3810;
  TMT_GRADIENTCOLOR2      = 3811;
  TMT_GRADIENTCOLOR3      = 3812;
  TMT_GRADIENTCOLOR4      = 3813;
  TMT_GRADIENTCOLOR5      = 3814;
  TMT_SHADOWCOLOR         = 3815;
  TMT_GLOWCOLOR           = 3816;
  TMT_TEXTBORDERCOLOR     = 3817;
  TMT_TEXTSHADOWCOLOR     = 3818;
  TMT_GLYPHTEXTCOLOR      = 3819;
  TMT_GLYPHTRANSPARENTCOLOR = 3820;

  TMT_FILLCOLORHINT       = 3821;
  TMT_BORDERCOLORHINT     = 3822;
  TMT_ACCENTCOLORHINT     = 3823;

  //---- rendering enum properties (must be declared in TM_ENUM section above) ----

  TMT_BASE4000            = 4000;
  TMT_BGTYPE              = 4001;
  TMT_BORDERTYPE          = 4002;
  TMT_FILLTYPE            = 4003;
  TMT_SIZINGTYPE          = 4004;
  TMT_HALIGN              = 4005;
  TMT_CONTENTALIGNMENT    = 4006;
  TMT_VALIGN              = 4007;
  TMT_OFFSETTYPE          = 4008;
  TMT_ICONEFFECT          = 4009;
  TMT_TEXTSHADOWTYPE      = 4010;
  TMT_IMAGELAYOUT         = 4011;
  TMT_GLYPHTYPE           = 4012;
  TMT_IMAGESELECTTYPE     = 4013;
  TMT_GLYPHFONTSIZINGTYPE = 4014;
  TMT_TRUESIZESCALINGTYPE = 4015;

  TMT_USERPICTURE         = 5001;
  TMT_DEFAULTPANESIZE     = 5002;
  TMT_BLENDCOLOR          = 5003;

  GP_LINEVERT             = 30;
  GP_LINEHORZ             = 31;
  GP_BORDER               = 32;

  LVS_FLAT                = 1;
  LVS_RAISED              = 2;
  LVS_SUNKEN              = 3;

  LHS_FLAT                = 1;
  LHS_RAISED              = 2;
  LHS_SUNKEN              = 3;

  BSS_FLAT                = 1;
  BSS_RAISED              = 2;
  BSS_SUNKEN              = 3;

  WINDOWFiller0           = 0;
  WP_FRAME                = 1;
  WP_SMALLFRAME           = 2;
  WP_CAPTION              = 3;
  WP_SMALLCAPTION         = 4;
  WP_MINCAPTION           = 5;
  WP_SMALLMINCAPTION      = 6;
  WP_MAXCAPTION           = 7;
  WP_SMALLMAXCAPTION      = 8;
  WP_SYSBUTTON            = 9;
  WP_SMALLSYSBUTTON       = 10;
  WP_MDISYSBUTTON         = 11;
  WP_MINBUTTON            = 12;
  WP_SMALLMINBUTTON       = 13;
  WP_MDIMINBUTTON         = 14;
  WP_MAXBUTTON            = 15;
  WP_SMALLMAXBUTTON       = 16;
  WP_CLOSEBUTTON          = 17;
  WP_SMALLCLOSEBUTTON     = 18;
  WP_MDICLOSEBUTTON       = 19;
  WP_RESTOREBUTTON        = 20;
  WP_SMALLRESTOREBUTTON   = 21;
  WP_MDIRESTOREBUTTON     = 22;
  WP_HELPBUTTON           = 23;
  WP_SMALLHELPBUTTON      = 24;
  WP_MDIHELPBUTTON        = 25;
  WP_HORZSCROLL           = 26;
  WP_HORZTHUMB            = 27;
  WP_VERTSCROLL           = 28;
  WP_VERTTHUMB            = 29;
  WP_DIALOG               = 30;
  WP_CAPTIONSIZINGTEMPLATE= 31;
  WP_SMALLCAPTIONSIZINGTEMPLATE = 32;
  WP_FRAMESIZINGTEMPLATE  = 33;
  WP_SMALLFRAMESIZINGTEMPLATE = 34;

  FS_ACTIVE               = 1;
  FS_INACTIVE             = 2;
  FS_DISABLED             = 3;

  CS_ACTIVE               = 1;
  CS_INACTIVE             = 2;
  CS_DISABLED             = 3;

  MXCS_ACTIVE             = 1;
  MXCS_INACTIVE           = 2;
  MXCS_DISABLED           = 3;

  MNCS_ACTIVE             = 1;
  MNCS_INACTIVE           = 2;
  MNCS_DISABLED           = 3;

  HSS_NORMAL              = 1;
  HSS_HOT                 = 2;
  HSS_PUSHED              = 3;
  HSS_DISABLED            = 4;

  HTS_NORMAL              = 1;
  HTS_HOT                 = 2;
  HTS_PUSHED              = 3;
  HTS_DISABLED            = 4;

  VSS_NORMAL              = 1;
  VSS_HOT                 = 2;
  VSS_PUSHED              = 3;
  VSS_DISABLED            = 4;

  VTS_NORMAL              = 1;
  VTS_HOT                 = 2;
  VTS_PUSHED              = 3;
  VTS_DISABLED            = 4;

  SBS_NORMAL              = 1;
  SBS_HOT                 = 2;
  SBS_PUSHED              = 3;
  SBS_DISABLED            = 4;

  MINBS_NORMAL            = 1;
  MINBS_HOT               = 2;
  MINBS_PUSHED            = 3;
  MINBS_DISABLED          = 4;

  MAXBS_NORMAL            = 1;
  MAXBS_HOT               = 2;
  MAXBS_PUSHED            = 3;
  MAXBS_DISABLED          = 4;

  RBS_NORMAL              = 1;
  RBS_HOT                 = 2;
  RBS_PUSHED              = 3;
  RBS_DISABLED            = 4;

  HBS_NORMAL              = 1;
  HBS_HOT                 = 2;
  HBS_PUSHED              = 3;
  HBS_DISABLED            = 4;

  CBS_NORMAL              = 1;
  CBS_HOT                 = 2;
  CBS_PUSHED              = 3;
  CBS_DISABLED            = 4;

  GBS_NORMAL              = 1;
  GBS_DISABLED            = 2;

  BP_PUSHBUTTON           = 1;
  BP_RADIOBUTTON          = 2;
  BP_CHECKBOX             = 3;
  BP_GROUPBOX             = 4;
  BP_USERBUTTON           = 5;

  PBS_NORMAL              = 1;
  PBS_HOT                 = 2;
  PBS_PRESSED             = 3;
  PBS_DISABLED            = 4;
  PBS_DEFAULTED           = 5;

  RBS_UNCHECKEDNORMAL     = 1;
  RBS_UNCHECKEDHOT        = 2;
  RBS_UNCHECKEDPRESSED    = 3;
  RBS_UNCHECKEDDISABLED   = 4;
  RBS_CHECKEDNORMAL       = 5;
  RBS_CHECKEDHOT          = 6;
  RBS_CHECKEDPRESSED      = 7;
  RBS_CHECKEDDISABLED     = 8;

  CBS_UNCHECKEDNORMAL     = 1;
  CBS_UNCHECKEDHOT        = 2;
  CBS_UNCHECKEDPRESSED    = 3;
  CBS_UNCHECKEDDISABLED   = 4;
  CBS_CHECKEDNORMAL       = 5;
  CBS_CHECKEDHOT          = 6;
  CBS_CHECKEDPRESSED      = 7;
  CBS_CHECKEDDISABLED     = 8;
  CBS_MIXEDNORMAL         = 9;
  CBS_MIXEDHOT            = 10;
  CBS_MIXEDPRESSED        = 11;
  CBS_MIXEDDISABLED       = 12;

  RP_GRIPPER              = 1;
  RP_GRIPPERVERT          = 2;
  RP_BAND                 = 3;
  RP_CHEVRON              = 4;

  CHEVS_NORMAL            = 1;
  CHEVS_HOT               = 2;
  CHEVS_PRESSED           = 3;

  TP_BUTTON               = 1;
  TP_DROPDOWNBUTTON       = 2;
  TP_SPLITBUTTON          = 3;
  TP_SPLITBUTTONDROPDOWN  = 4;
  TP_SEPARATOR            = 5;
  TP_SEPARATORVERT        = 6;

  TS_NORMAL               = 1;
  TS_HOT                  = 2;
  TS_PRESSED              = 3;
  TS_DISABLED             = 4;
  TS_CHECKED              = 5;
  TS_HOTCHECKED           = 6;

  SP_PANE                 = 1;
  SP_GRIPPERPANE          = 2;
  SP_GRIPPER              = 3;

  MP_MENUITEM             = 1;
  MP_MENUDROPDOWN         = 2;
  MP_MENUBARITEM          = 3;
  MP_MENUBARDROPDOWN      = 4;
  MP_CHEVRON              = 5;
  MP_SEPARATOR            = 6;

  MS_NORMAL               = 1;
  MS_SELECTED             = 2;
  MS_DEMOTED              = 3;

  LVP_LISTITEM            = 1;
  LVP_LISTGROUP           = 2;
  LVP_LISTDETAIL          = 3;
  LVP_LISTSORTEDDETAIL    = 4;
  LVP_EMPTYTEXT           = 5;

  LIS_NORMAL              = 1;
  LIS_HOT                 = 2;
  LIS_SELECTED            = 3;
  LIS_DISABLED            = 4;
  LIS_SELECTEDNOTFOCUS    = 5;

  HP_HEADERITEM           = 1;
  HP_HEADERITEMLEFT       = 2;
  HP_HEADERITEMRIGHT      = 3;
  HP_HEADERSORTARROW      = 4;

  HIS_NORMAL              = 1;
  HIS_HOT                 = 2;
  HIS_PRESSED             = 3;

  HILS_NORMAL             = 1;
  HILS_HOT                = 2;
  HILS_PRESSED            = 3;

  HIRS_NORMAL             = 1;
  HIRS_HOT                = 2;
  HIRS_PRESSED            = 3; 

  HSAS_SORTEDUP           = 1;
  HSAS_SORTEDDOWN         = 2;

  PP_BAR                  = 1;
  PP_BARVERT              = 2;
  PP_CHUNK                = 3;
  PP_CHUNKVERT            = 4;

  TABP_TABITEM            = 1;
  TABP_TABITEMLEFTEDGE    = 2;
  TABP_TABITEMRIGHTEDGE   = 3;
  TABP_TABITEMBOTHEDGE    = 4;
  TABP_TOPTABITEM         = 5;
  TABP_TOPTABITEMLEFTEDGE = 6;
  TABP_TOPTABITEMRIGHTEDGE= 7;
  TABP_TOPTABITEMBOTHEDGE = 8;
  TABP_PANE               = 9;
  TABP_BODY               = 10;

  TIS_NORMAL              = 1;
  TIS_HOT                 = 2;
  TIS_SELECTED            = 3;
  TIS_DISABLED            = 4;
  TIS_FOCUSED             = 5;

  TILES_NORMAL            = 1;
  TILES_HOT               = 2;
  TILES_SELECTED          = 3;
  TILES_DISABLED          = 4;
  TILES_FOCUSED           = 5;

  TIRES_NORMAL            = 1;
  TIRES_HOT               = 2;
  TIRES_SELECTED          = 3;
  TIRES_DISABLED          = 4;
  TIRES_FOCUSED           = 5;

  TIBES_NORMAL            = 1;
  TIBES_HOT               = 2;
  TIBES_SELECTED          = 3;
  TIBES_DISABLED          = 4;
  TIBES_FOCUSED           = 5;

  TTIS_NORMAL              = 1;
  TTIS_HOT                 = 2;
  TTIS_SELECTED            = 3;
  TTIS_DISABLED            = 4;
  TTIS_FOCUSED             = 5;

  TTILES_NORMAL            = 1;
  TTILES_HOT               = 2;
  TTILES_SELECTED          = 3;
  TTILES_DISABLED          = 4;
  TTILES_FOCUSED           = 5;

  TTIRES_NORMAL            = 1;
  TTIRES_HOT               = 2;
  TTIRES_SELECTED          = 3;
  TTIRES_DISABLED          = 4;
  TTIRES_FOCUSED           = 5;

  TTIBES_NORMAL            = 1;
  TTIBES_HOT               = 2;
  TTIBES_SELECTED          = 3;
  TTIBES_DISABLED          = 4;
  TTIBES_FOCUSED           = 5;

  TKP_TRACK                = 1;
  TKP_TRACKVERT            = 2;
  TKP_THUMB                = 3;
  TKP_THUMBBOTTOM          = 4;
  TKP_THUMBTOP             = 5;
  TKP_THUMBVERT            = 6;
  TKP_THUMBLEFT            = 7;
  TKP_THUMBRIGHT           = 8;
  TKP_TICS                 = 9;
  TKP_TICSVERT             = 10;

  TKS_NORMAL               = 1;
  TRS_NORMAL               = 1;
  TRVS_NORMAL              = 1;

  TUS_NORMAL               = 1;
  TUS_HOT                  = 2;
  TUS_PRESSED              = 3;
  TUS_FOCUSED              = 4;
  TUS_DISABLED             = 5;

  TUTS_NORMAL              = 1;
  TUTS_HOT                 = 2;
  TUTS_PRESSED             = 3;
  TUTS_FOCUSED             = 4;
  TUTS_DISABLED            = 5;

  TUBS_NORMAL              = 1;
  TUBS_HOT                 = 2;
  TUBS_PRESSED             = 3;
  TUBS_FOCUSED             = 4;
  TUBS_DISABLED            = 5;

  TUVS_NORMAL              = 1;
  TUVS_HOT                 = 2;
  TUVS_PRESSED             = 3;
  TUVS_FOCUSED             = 4;
  TUVS_DISABLED            = 5;

  TUVLS_NORMAL             = 1;
  TUVLS_HOT                = 2;
  TUVLS_PRESSED            = 3;
  TUVLS_FOCUSED            = 4;
  TUVLS_DISABLED           = 5;

  TUVRS_NORMAL             = 1;
  TUVRS_HOT                = 2;
  TUVRS_PRESSED            = 3;
  TUVRS_FOCUSED            = 4;
  TUVRS_DISABLED           = 5;

  TSS_NORMAL               = 1;
  TSVS_NORMAL              = 1;

  TTP_STANDARD             = 1;
  TTP_STANDARDTITLE        = 2;
  TTP_BALLOON              = 3;
  TTP_BALLOONTITLE         = 4;

  TTSS_NORMAL              = 1;
  TTSS_LINK                = 2;

  TTBS_NORMAL              = 1;
  TTBS_LINK                = 2;

  TVP_TREEITEM             = 1;
  TVP_GLYPH                = 2;
  TVP_BRANCH               = 3;

  TREIS_NORMAL             = 1;
  TREIS_HOT                = 2;
  TREIS_SELECTED           = 3;
  TREIS_DISABLED           = 4;
  TREIS_SELECTEDNOTFOCUS   = 5;

  GLPS_CLOSED              = 1;
  GLPS_OPENED              = 2;

  SPNP_UP                  = 1;
  SPNP_DOWN                = 2;
  SPNP_UPHORZ              = 3;
  SPNP_DOWNHORZ            = 4;

  UPS_NORMAL               = 1;
  UPS_HOT                  = 2;
  UPS_PRESSED              = 3;
  UPS_DISABLED             = 4;

  DNS_NORMAL               = 1;
  DNS_HOT                  = 2;
  DNS_PRESSED              = 3;
  DNS_DISABLED             = 4;

  UPHZS_NORMAL             = 1;
  UPHZS_HOT                = 2;
  UPHZS_PRESSED            = 3;
  UPHZS_DISABLED           = 4;

  DNHZS_NORMAL             = 1;
  DNHZS_HOT                = 2;
  DNHZS_PRESSED            = 3;
  DNHZS_DISABLED           = 4;

  PGRP_UP                  = 1;
  PGRP_DOWN                = 2;
  PGRP_UPHORZ              = 3;
  PGRP_DOWNHORZ            = 4;

  SBP_ARROWBTN             = 1;
  SBP_THUMBBTNHORZ         = 2;
  SBP_THUMBBTNVERT         = 3;
  SBP_LOWERTRACKHORZ       = 4;
  SBP_UPPERTRACKHORZ       = 5;
  SBP_LOWERTRACKVERT       = 6;
  SBP_UPPERTRACKVERT       = 7;
  SBP_GRIPPERHORZ          = 8;
  SBP_GRIPPERVERT          = 9;
  SBP_SIZEBOX              = 10;

  ABS_UPNORMAL             = 1;
  ABS_UPHOT                = 2;
  ABS_UPPRESSED            = 3;
  ABS_UPDISABLED           = 4;
  ABS_DOWNNORMAL           = 5;
  ABS_DOWNHOT              = 6;
  ABS_DOWNPRESSED          = 7;
  ABS_DOWNDISABLED         = 8;
  ABS_LEFTNORMAL           = 9;
  ABS_LEFTHOT              = 10;
  ABS_LEFTPRESSED          = 11;
  ABS_LEFTDISABLED         = 12;
  ABS_RIGHTNORMAL          = 13;
  ABS_RIGHTHOT             = 14;
  ABS_RIGHTPRESSED         = 15;
  ABS_RIGHTDISABLED        = 16;

  SCRBS_NORMAL             = 1;
  SCRBS_HOT                = 2;
  SCRBS_PRESSED            = 3;
  SCRBS_DISABLED           = 4;

  SZB_RIGHTALIGN           = 1;
  SZB_LEFTALIGN            = 2;

  EP_EDITTEXT              = 1;
  EP_CARET                 = 2;

  ETS_NORMAL               = 1;
  ETS_HOT                  = 2;
  ETS_SELECTED             = 3;
  ETS_DISABLED             = 4;
  ETS_FOCUSED              = 5;
  ETS_READONLY             = 6;
  ETS_ASSIST               = 7;

  CP_DROPDOWNBUTTON        = 1;
  
  CBXS_NORMAL              = 1;
  CBXS_HOT                 = 2;
  CBXS_PRESSED             = 3;
  CBXS_DISABLED            = 4;


  CLP_TIME                 = 1;
  CLS_NORMAL               = 1;

  TNP_BACKGROUND           = 1;
  TNP_ANIMBACKGROUND       = 2;

  TBP_BACKGROUNDBOTTOM     = 1;
  TBP_BACKGROUNDRIGHT      = 2;
  TBP_BACKGROUNDTOP        = 3;
  TBP_BACKGROUNDLEFT       = 4;
  TBP_SIZINGBARBOTTOM      = 5;
  TBP_SIZINGBARRIGHT       = 6;
  TBP_SIZINGBARTOP         = 7;
  TBP_SIZINGBARLEFT        = 8;

  TDP_GROUPCOUNT           = 1;
  TDP_FLASHBUTTON          = 2;
  TDP_FLASHBUTTONGROUPMENU = 3;

  SPP_USERPANE             = 1;
  SPP_MOREPROGRAMS         = 2;
  SPP_MOREPROGRAMSARROW    = 3;
  SPP_PROGLIST             = 4;
  SPP_PROGLISTSEPARATOR    = 5;
  SPP_PLACESLIST           = 6;
  SPP_PLACESLISTSEPARATOR  = 7;
  SPP_LOGOFF               = 8;
  SPP_LOGOFFBUTTONS        = 9;
  SPP_USERPICTURE          = 10;
  SPP_PREVIEW              = 11;

  SPS_NORMAL               = 1;
  SPS_HOT                  = 2;
  SPS_PRESSED              = 3;

  SPLS_NORMAL              = 1;
  SPLS_HOT                 = 2;
  SPLS_PRESSED             = 3;

  EBP_HEADERBACKGROUND     = 1;
  EBP_HEADERCLOSE          = 2;
  EBP_HEADERPIN            = 3;
  EBP_IEBARMENU            = 4;
  EBP_NORMALGROUPBACKGROUND= 5;
  EBP_NORMALGROUPCOLLAPSE  = 6;
  EBP_NORMALGROUPEXPAND    = 7;
  EBP_NORMALGROUPHEAD      = 8;
  EBP_SPECIALGROUPBACKGROUND=9;
  EBP_SPECIALGROUPCOLLAPSE =10;
  EBP_SPECIALGROUPEXPAND   =11;
  EBP_SPECIALGROUPHEAD     =12;

  EBHC_NORMAL              = 1;
  EBHC_HOT                 = 2;
  EBHC_PRESSED             = 3;

  EBHP_NORMAL              = 1;
  EBHP_HOT                 = 2;
  EBHP_PRESSED             = 3;
  EBHP_SELECTEDNORMAL      = 4;
  EBHP_SELECTEDHOT         = 5;
  EBHP_SELECTEDPRESSED     = 6;

  EBM_NORMAL               = 1;
  EBM_HOT                  = 2;
  EBM_PRESSED              = 3;

  EBNGC_NORMAL             = 1;
  EBNGC_HOT                = 2;
  EBNGC_PRESSED            = 3;

  EBNGE_NORMAL             = 1;
  EBNGE_HOT                = 2;
  EBNGE_PRESSED            = 3;

  EBSGC_NORMAL             = 1;
  EBSGC_HOT                = 2;
  EBSGC_PRESSED            = 3;

  MDP_NEWAPPBUTTON         = 1;
  MDP_SEPERATOR            = 2;

  MDS_NORMAL               = 1;
  MDS_HOT                  = 2;
  MDS_PRESSED              = 3;
  MDS_DISABLED             = 4;
  MDS_CHECKED              = 5;
  MDS_HOTCHECKED           = 6;
  

implementation

end.

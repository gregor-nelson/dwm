#include <X11/XF86keysym.h>
#define BROWSER "chromium"
#define FILEMANAGER "thunar"

/* Appearance */
static const unsigned int borderpx  = 1;
static const unsigned int default_border = 1;
static const unsigned int snap      = 32;
static const unsigned int gappih    = 10;
static const unsigned int gappiv    = 10;
static const unsigned int gappoh    = 10;
static const unsigned int gappov    = 10;
static       int smartgaps          = 0;
static const unsigned int systraypinning = 0;
static const unsigned int systrayspacing = 2;
static const int systraypinningfailfirst = 1;
static const int showsystray        = 1;
static const int showbar            = 1;
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always };
static const int showtab            = showtab_auto;
static const int toptab             = True;
static const int topbar             = 1;
static const int horizpadbar        = 5;
static const int vertpadbar         = 11;
static const int vertpadtab         = 33;
static const int horizpadtabi       = 15;
static const int horizpadtabo       = 15;
static const int scalepreview       = 4;
static       int tag_preview        = 0;

static const char *fonts[]          = { "JetBrainsMono Nerd Font:style:medium:size=10" };
static const int colorfultag        = 1;

/* Volume */
static const char *upvol[]   = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+1%", NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-1%", NULL };
static const char *mutevol[] = { "pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle", NULL };

/* Brightness */
static const char *light_up[]   = { "brightnessctl", "set", "5%+", NULL };
static const char *light_down[] = { "brightnessctl", "set", "5%-", NULL };

#include "themes/onedark.h"

static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm]       = { gray3, black, gray2 },
    [SchemeSel]        = { gray4, blue,  blue  },
    [TabSel]           = { blue, gray2,  black  },
    [TabNorm]          = { gray3, black, black },
    [SchemeTag]        = { gray3, black, black },
    [SchemeTag1]       = { blue,  black, black },
    [SchemeTag2]       = { red,   black, black },
    [SchemeTag3]       = { orange, black,black },
    [SchemeTag4]       = { green, black, black },
    [SchemeTag5]       = { pink,  black, black },
    [SchemeLayout]     = { green, black, black }, 
    [SchemeBtnPrev]    = { green, black, black }, 
    [SchemeBtnNext]    = { yellow, black, black }, 
    [SchemeBtnClose]   = { red, black, black }, 
};

static char *tags[] = {"  ", "  ", "  ", "  ", "  "};

static const int tagschemes[] = { SchemeTag1, SchemeTag2, SchemeTag3,
                                  SchemeTag4, SchemeTag5
                                };

static const unsigned int ulinepad	= 5;
static const unsigned int ulinestroke	= 2;
static const unsigned int ulinevoffset	= 0;
static const int ulineall 		= 0;

static const Rule rules[] = {
	/* class      instance    title       tags mask     iscentered   isfloating   monitor */
	{ NULL,       NULL,       NULL,       0,            0,           0,           -1 },
};

/* Layout */
static const float mfact     = 0.50;
static const int nmaster     = 1;
static const int resizehints = 0;
static const int lockfullscreen = 1;

#define FORCE_VSPLIT 1
#include "functions.h"

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },
    { "[M]",      monocle },
    { "[@]",      spiral },
    { "[\\]",     dwindle },
    { "H[]",      deck },
    { "TTT",      bstack },
    { "===",      bstackhoriz },
    { "HHH",      grid },
    { "###",      nrowgrid },
    { "---",      horizgrid },
    { ":::",      gaplessgrid },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
    { "><>",      NULL },
};

#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* Commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *rofidrun[] = { "rofi", "-show", "drun", NULL };
static const char *rofirun[]  = { "rofi", "-show", "run", NULL };
static const char *roficalc[] = { "rofi", "-show", "calc", "-modi", "calc", "-no-show-match", "-no-sort", NULL };

static Key keys[] = {
    /* modifier                     key                          function        argument */

    /* Media keys */
    { 0,                            XF86XK_AudioLowerVolume,     spawn,          {.v = downvol} },
    { 0,                            XF86XK_AudioMute,            spawn,          {.v = mutevol} },
    { 0,                            XF86XK_AudioRaiseVolume,     spawn,          {.v = upvol} },
    { 0,                            XF86XK_MonBrightnessUp,      spawn,          {.v = light_up} },
    { 0,                            XF86XK_MonBrightnessDown,    spawn,          {.v = light_down} },

    /* Launchers */
    { MODKEY,                       XK_w,      spawn,          {.v = (const char*[]){ BROWSER, NULL } } },
    { MODKEY,                       XK_r,      spawn,          {.v = (const char*[]){ FILEMANAGER, NULL } } },
    { MODKEY,                       XK_c,      spawn,          {.v = rofidrun } },
    { MODKEY|ControlMask,           XK_c,      spawn,          {.v = rofirun } },
    { MODKEY|ShiftMask,             XK_c,      spawn,          {.v = roficalc } },
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY|ControlMask,           XK_w,      tabmode,        { -1 } },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_Left,   focusdir,       {.i = 0 } }, // left
    { MODKEY,                       XK_Right,  focusdir,       {.i = 1 } }, // right
    { MODKEY,                       XK_Up,     focusdir,       {.i = 2 } }, // up
    { MODKEY,                       XK_Down,   focusdir,       {.i = 3 } }, // down
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
    { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
    { MODKEY,                       XK_Tab,    view,           {0} },

    /* Gaps */
    { MODKEY|ControlMask,              XK_i,      incrgaps,       {.i = +1 } },
    { MODKEY|ControlMask,              XK_d,      incrgaps,       {.i = -1 } },
    { MODKEY|ShiftMask,                XK_i,      incrigaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
    { MODKEY|ControlMask,              XK_o,      incrogaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
    { MODKEY|ControlMask,              XK_6,      incrihgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_7,      incrivgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_8,      incrohgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_9,      incrovgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_t,      togglegaps,     {0} },
    { MODKEY|ControlMask|ShiftMask,    XK_d,      defaultgaps,    {0} },

    { MODKEY,                       XK_q,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY|ControlMask,           XK_g,      setlayout,      {.v = &layouts[10]} },
    { MODKEY|ControlMask|ShiftMask, XK_t,      setlayout,      {.v = &layouts[13]} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ControlMask,		    XK_comma,  cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_minus, 		setborderpx,    {.i = -1 } },
    { MODKEY|ShiftMask,             XK_p, 	        setborderpx,    {.i = +1 } },
    { MODKEY|ShiftMask,             XK_w, 	        setborderpx,    {.i = default_border } },

    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)

    { MODKEY|ControlMask,           XK_q,      quit,           {0} },
    { MODKEY|ShiftMask,             XK_r,      quit,           {1} },
    { MODKEY,                       XK_e,      hidewin,        {0} },
    { MODKEY|ShiftMask,             XK_e,      restorewin,     {0} },

};

static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         ControlMask,    Button1,        dragmfact,      {0} },
    { ClkClientWin,         ControlMask,    Button3,        dragcfact,      {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabPrev,           0,              Button1,        movestack,      { .i = -1 } },
    { ClkTabNext,           0,              Button1,        movestack,      { .i = +1 } },
    { ClkTabClose,          0,              Button1,        killclient,     {0} },
};

/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include "fibonacci.c"

/* appearance */
static const unsigned int borderpx = 2;        /* border pixel of windows */
static const unsigned int snap     = 32;       /* snap pixel */
static const int showbar           = 1;        /* 0 means no bar */
static const int topbar            = 1;        /* 0 means bottom bar */
static const char *fonts[]         = { "FantasqueSansMono Nerd Font:size=10" };
static const char dmenufont[]      = "FantasqueSansMono Nerd Font:size=10";

static const char col_gray1[]      = "#222222";
static const char col_gray2[]      = "#444444";
static const char col_gray3[]      = "#bbbbbb";
static const char col_gray4[]      = "#eeeeee";
static const char col_accent[]     = "#005577";

static const char gruv_dark0[]     = "#282828";
static const char gruv_dark1[]     = "#504945";
static const char gruv_dark2[]     = "#928374";
static const char gruv_dark3[]     = "#ebdbb2";
static const char gruv_darka[]     = "#d75d0e";

static const char gruv_light0[]    = "#fbf1c7";
static const char gruv_light1[]    = "#d5c4a1";
static const char gruv_light2[]    = "#928374";
static const char gruv_light3[]    = "#3c3836";
static const char gruv_lighta[]    = "#d75d0e";

static const char *colors[][3]     = {
    /*               fg           bg           border      */
    //[SchemeNorm] = { col_gray3,   col_gray1,   col_gray2   },
    //[SchemeSel]  = { col_gray4,   col_accent,  col_accent  },
    { gruv_dark2,  gruv_dark0,  gruv_dark1  },
    { gruv_dark3,  gruv_darka,  gruv_darka  },
    { gruv_light2, gruv_light0, gruv_light1 },
    { gruv_light3, gruv_lighta, gruv_lighta },
    { col_gray3,   col_gray1,   col_gray2   },
    { col_gray4,   col_accent,  col_accent  },
};

/* tagging */
static const char *tags[]    = { "d", "w", "s", "o", "c", "m", "v", "g", "x" };
static const char *alttags[] = { ":d", ":w", ":s", ":o", ":c", ":m", ":v", ":g", ":x" };

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
     */
    /* class         instance    title       tags mask     isfloating   monitor */
    { "Gimp",        NULL,       NULL,       0,            1,           -1 },
    { "qutebrowser", NULL,       NULL,       2,            0,           -1 },
    { "mpv",         "gl",       NULL,       7,            0,           -1 },
};

/* layout(s) */
static const float mfact        = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[=]",      tile },    /* first entry is default */
    { "[@]",      spiral },
    { "[\\]",     dwindle },
    { "[~]",      NULL },    /* no layout function means floating behavior */
    { "[o]",      monocle },
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
    { MODKEY,                       CHAIN,      KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           CHAIN,      KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             CHAIN,      KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, CHAIN,      KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]   = "0"; /* component of dmenucmd, manipulated in spawn() */
static char dmenuprompt[] = "run: ";
static char terminal[]    = "alacritty";
static char vim[]         = "nvim";
static char browser[]     = "qutebrowser";
static const char *dmenucmd[]         = { "dmenu_run", "-i", "-m", dmenumon, "-p", dmenuprompt, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_accent, "-sf", col_gray4, topbar ? NULL : "-b", NULL };
static const char *termcmd[]          = { terminal, NULL };
static const char *vimcmd[]           = { terminal, "-e", vim, NULL };
static const char *browsercmd[]       = { browser, NULL };
static const char *touchpadlockcmd[]  = { "/home/carbon/.scripts/toggle-touchpad.sh", NULL };
static const char *togglemutecmd[]    = { "pactl", "--", "set-sink-mute", "0", "toggle", NULL };
static const char *voldowncmd[]       = { "pactl", "--", "set-sink-volume", "0", "-5%", NULL };
static const char *volupcmd[]         = { "pactl", "--", "set-sink-volume", "0", "+5%", NULL };
static const char *backlightdowncmd[] = { "xbacklight", "-dec", "5" };
static const char *backlightupcmd[]   = { "xbacklight", "-inc", "5" };
static const char *backlightmaxcmd[]  = { "xbacklight", "-set", "100" };
static const char *backlightmincmd[]  = { "xbacklight", "-set", "0" };

static Key keys[] = {
    /* modifier                     chain key    key                       function        argument */
    { MODKEY|ShiftMask,             -1,          XK_Return,                spawn,          {.v = dmenucmd } },
    { MODKEY,                       -1,          XK_Return,                spawn,          {.v = termcmd } }, 
    { MODKEY,                       -1,          XK_b,                     togglebar,      {0} },
    { MODKEY,                       -1,          XK_j,                     focusstack,     {.i = +1 } },
    { MODKEY,                       -1,          XK_k,                     focusstack,     {.i = -1 } },
    { MODKEY|ShiftMask,             -1,          XK_j,                     inplacerotate,  {.i = +1 } },
    { MODKEY|ShiftMask,             -1,          XK_k,                     inplacerotate,  {.i = -1 } },
    { MODKEY|ShiftMask,             -1,          XK_h,                     inplacerotate,  {.i = +2 } },
    { MODKEY|ShiftMask,             -1,          XK_l,                     inplacerotate,  {.i = -2 } },
    { MODKEY,                       -1,          XK_i,                     incnmaster,     {.i = +1 } },
    { MODKEY,                       -1,          XK_d,                     incnmaster,     {.i = -1 } },
    { MODKEY,                       -1,          XK_h,                     setmfact,       {.f = -0.05} },
    { MODKEY,                       -1,          XK_l,                     setmfact,       {.f = +0.05} },
    { MODKEY,                       -1,          XK_Tab,                   view,           {0} },
    { MODKEY,                       -1,          XK_c,                     killclient,     {0} },
    { MODKEY,                       -1,          XK_F1,                    setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       -1,          XK_F2,                    setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       -1,          XK_F3,                    setlayout,      {.v = &layouts[2]} },
    { MODKEY|ControlMask,           -1,          XK_comma,                 cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,           -1,          XK_period,                cyclelayout,    {.i = +1 } },
    { MODKEY,                       -1,          XK_space,                 togglefullscr,  {0} },
    { MODKEY|ShiftMask,             -1,          XK_space,                 togglefloating, {0} },
    { MODKEY,                       -1,          XK_0,                     view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             -1,          XK_0,                     tag,            {.ui = ~0 } },
    { MODKEY|ShiftMask,             -1,          XK_m,                     zoom,           {0} },
    { MODKEY,                       -1,          XK_m,                     focusmaster,    {0} },
    { MODKEY,                       -1,          XK_comma,                 focusmon,       {.i = -1 } },
    { MODKEY,                       -1,          XK_period,                focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             -1,          XK_comma,                 tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             -1,          XK_period,                tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             -1,          XK_t,                     schemeToggle,   {0} },
    { MODKEY|ShiftMask,             -1,          XK_z,                     schemeCycle,    {0} },
    { MODKEY|ShiftMask,             -1,          XK_q,                     quit,           {0} },
    { MODKEY,                       XK_x,        XK_b,                     spawn,          {.v = browsercmd } },
    { MODKEY,                       XK_x,        XK_v,                     spawn,          {.v = vimcmd } },
    { MODKEY,                       -1,          XF86XK_MonBrightnessDown, spawn,          {.v = backlightmincmd } },
    { MODKEY,                       -1,          XF86XK_MonBrightnessUp,   spawn,          {.v = backlightmaxcmd } },
    { 0,                            -1,          XF86XK_AudioMute,         spawn,          {.v = togglemutecmd } },
    { 0,                            -1,          XF86XK_AudioLowerVolume,  spawn,          {.v = voldowncmd } },
    { 0,                            -1,          XF86XK_AudioRaiseVolume,  spawn,          {.v = volupcmd } },
    { 0,                            -1,          XF86XK_MonBrightnessDown, spawn,          {.v = backlightdowncmd } },
    { 0,                            -1,          XF86XK_MonBrightnessUp,   spawn,          {.v = backlightupcmd } },
    { 0,                            -1,          XF86XK_TouchpadToggle,    spawn,          {.v = touchpadlockcmd } },
    TAGKEYS(                        -1,          XK_1,                      0)
    TAGKEYS(                        -1,          XK_2,                      1)
    TAGKEYS(                        -1,          XK_3,                      2)
    TAGKEYS(                        -1,          XK_4,                      3)
    TAGKEYS(                        -1,          XK_5,                      4)
    TAGKEYS(                        -1,          XK_6,                      5)
    TAGKEYS(                        -1,          XK_7,                      6)
    TAGKEYS(                        -1,          XK_8,                      7)
    TAGKEYS(                        -1,          XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


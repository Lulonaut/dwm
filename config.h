#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 0;     /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ "discord",  NULL,	      NULL,	      0,            0, 	          1 },
};

static const char *const autostart[] = {
	"dunst", NULL,
	"picom", "-config", "~/.config/picom/picom.conf", "-b", "--no-fading-open-close", "-I 1", "-O 1", NULL,
	"discord-canary", NULL,
	"sh", "-c", "~/.config/screenlayout.sh", NULL,
	"sh", "-c", "~/applications/dwm/misc/status.sh", NULL,
    "pipewire", NULL,
    "pipewire-pulse", NULL,
    "pipewire-media-session", NULL,
    "refreshbg", NULL,
    "trayfix", NULL
};


/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browsercmd[] = {"brave", NULL };
static const char *roficmd[] = { "rofi" , "-show", "run", NULL };
static const char *fmcmd[] = { "pcmanfm" , NULL};
static const char *screenshotcmd[] = { "flameshot", "gui", NULL };
static const char *powermenucmd[] = { "sh", "-c", "~/.config/i3blocks/scripts/powermenu", NULL };
static const char *musicpausecmd[] = { "playerctl", "play-pause", NULL };
static const char *musicnextcmd[] = { "playerctl", "next", NULL };
static const char *musicprevcmd[] = { "playerctl", "previous", NULL };
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+1%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-1%",     NULL };

static Key keys[] = {
	{ MODKEY,                       XK_p,                                   spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                              spawn,          {.v = termcmd } },
    { MODKEY,                       XK_e,                                   spawn,          {.v = fmcmd} },
	{ MODKEY,                       XK_b,                                   spawn,     	    {.v = browsercmd } },
    { MODKEY,                       XK_Print,                               spawn,          {.v = screenshotcmd } },
	{ MODKEY|ShiftMask,		        XK_e,	                                spawn,	        {.v = powermenucmd } },
	{ 0,				            XF86XK_AudioPlay,                       spawn,          {.v = musicpausecmd } },
	{ 0, 				            XF86XK_AudioNext,                       spawn,          {.v = musicnextcmd } },
	{ 0, 				            XF86XK_AudioPrev,                       spawn,          {.v = musicprevcmd } },
	{ 0,                            XF86XK_AudioLowerVolume,                spawn,          {.v = downvol } },
	{ 0,                            XF86XK_AudioRaiseVolume,                spawn,          {.v = upvol   } },
	{ MODKEY,                       XK_j,                                   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                                   focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                                   incnmaster,     {.i = +1 } },
//	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY, 			            XK_d,                                   spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_h,                                   setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                                   setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,                                 zoom,           {0} },
	{ ControlMask, 			        XK_q,	                                killclient,     {0} },
	{ MODKEY,                       XK_t,                                   setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                                   setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                                   setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                               setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                               togglefloating, {0} },
	{ MODKEY,                       XK_0,                                   view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                                   tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_Left,                                focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_Right,                               focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,                                tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,		        XK_Right,                               tagmon,	        {.i = +1 } },
	
    TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
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


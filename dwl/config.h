/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */

static const int smartgaps 		   = 0;
static int gappx			   = 5;
static int gaps 			   = 1;

static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x000000ff);
static const float bordercolor[]           = COLOR(0x1c1c1cff);
static const float focuscolor[]            = COLOR(0xcc241dff);
static const float urgentcolor[]           = COLOR(0xff0000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.f, 0.f, 0.f, 1.0f}; /* You can also use glsl colors */

/* tagging - TAGCOUNT must be no greater than 31 */
// Add tag 10 - an invisible firefox tag to start firefox faster 
#define TAGCOUNT (10)

/* logging */
static int log_level = WLR_ERROR;

/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	/* examples: */
	{ "Gimp_EXAMPLE",     NULL,       0,            1,           -1 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 }, /* Start on ONLY tag "9" */
	// Start firefox in an invisible tag to startup faster
	//{ "firefox",          NULL,       1 << 9,       0,           -1 },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ " \uef4c ",      tile },
	{ " \ueb23 ",      NULL },    /* no layout function means floating behavior */
	{ " \ueb7f ",      monocle },
	{ " \uef4d ",      tilevertical },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact  nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	//{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ "HDMI-A-1", 0.55,  1,      1,    &layouts[3], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },  // vertical monitor layout
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.layout = "fi",
	.options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
// Track command is used to send an symbol to status bar informing that the operation is being done
static const char *termcmd[] = { "foot", NULL };
static const char *menucmd[] = { "wmenu-run", "-N", "000000ff", "-S", "cc241dff", NULL };
static const char *browsercmd[] = { "firefox", NULL };

static const char *lockcmd[] = { "lock", NULL };
static const char *suspendcmd[] = { "locksuspend", NULL };

static const char *clipscreenshotcmd[] = { "track", "\uf50c \uf061 \uf07f", "--", "clipscreenshot", NULL };
static const char *savescreenshotcmd[] = { "track", "\uf50c \uf061 \uf4a5", "--", "savescreenshot", NULL };
static const char *btconnectlastcmd[] = { "track", "+\uf294", "0.5", "--", "bt-last-device", "connect", NULL };
static const char *btdisconnectlastcmd[] = { "track", "-\uf294", "0.5", "--", "bt-last-device", "disconnect", NULL };

static const char *mediaplaypausecmd[] = { "track", "\ueb2c\uf04c", "0.5", "--", "media-control", "play-pause", NULL };
static const char *medianextcmd[] = { "track", "\uf051", "0.5", "--", "media-control", "next", NULL };
static const char *mediaprevcmd[] = { "track", "\uf048", "0.5", "--", "media-control", "prev", NULL };

static const char *volumeupcmd[] = { "track", "\uf028+", "0.5", "--", "media-control", "volume", "5", NULL };
static const char *volumedowncmd[] = { "track", "\uf027-", "0.5", "--", "media-control", "volume", "-5", NULL };
static const char *brightnessupcmd[] = { "track", "\uf400+", "0.5", "--", "brightness-control", "up", NULL };
static const char *brightnessdowncmd[] = { "track", "\uf400-", "0.5", "--", "brightness-control", "down", NULL };

void
toggledwlb(const Arg *arg)
{
    Monitor *m;
    char cmd[256];
    
    if (!(m = selmon))
        return;
    
    // Send toggle command to dwlb for this monitor
    snprintf(cmd, sizeof(cmd), "dwlb -toggle-visibility %s", m->wlr_output->name);
    system(cmd);
}

void
incgaps(const Arg *arg)
{
	gappx = MAX(gappx + arg->i, 0);
	arrange(selmon);
}

static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	{ MODKEY,                    XKB_KEY_p,          spawn,          {.v = menucmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,     spawn,          {.v = termcmd} },
	{ MODKEY,		     XKB_KEY_w,          spawn, 	 {.v = browsercmd} },

	{ MODKEY,                    XKB_KEY_a,          toggledwlb, 	{0} },
	{ MODKEY,                    XKB_KEY_g,          togglegaps, 	{0} },
	{ MODKEY,                    XKB_KEY_s,          spawn,		{.v = clipscreenshotcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_S,          spawn,		{.v = savescreenshotcmd} },
	{ MODKEY,		     XKB_KEY_q,          spawn, 	{.v = lockcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Q,          spawn, 	{.v = suspendcmd} },
	{ MODKEY,                    XKB_KEY_minus,      incgaps, 	{.i = -5} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_underscore, incgaps, 	{.i = 5} },

	{ MODKEY,                    XKB_KEY_b,          spawn,		{.v = btconnectlastcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_B,		 spawn, 	{.v = btdisconnectlastcmd} },

	{ MODKEY,                    XKB_KEY_Up,         spawn,		{.v = volumeupcmd } },
	{ MODKEY,                    XKB_KEY_Down,       spawn,		{.v = volumedowncmd } },
	{ MODKEY,                    XKB_KEY_Right,      spawn,		{.v = brightnessupcmd} },
	{ MODKEY,                    XKB_KEY_Left,       spawn,		{.v = brightnessdowncmd} },

	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Right,      spawn,		{.v = medianextcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Left,       spawn,		{.v = mediaprevcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Up,         spawn,		{.v = mediaplaypausecmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Down,       spawn,		{.v = mediaplaypausecmd} },

	{ MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_i,          incnmaster,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_d,          incnmaster,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05f} },
	{ MODKEY,                    XKB_KEY_Return,     zoom,           {0} },
	{ MODKEY,                    XKB_KEY_Tab,        view,           {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_X,          killclient,     {0} },

	{ MODKEY,                    XKB_KEY_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    XKB_KEY_f,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_v,          setlayout,      {.v = &layouts[3]} },

	{ MODKEY,                    XKB_KEY_space,      setlayout,      {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,      togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_e,          togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_0,          view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_equal,      tag,            {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_semicolon,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_colon,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_quotedbl,                   1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                 2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_currency,                   3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_dollar,                     4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_ampersand, 	         5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_slash,                      6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_parenleft,                  7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenright,                  8),
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Escape,          quit,           {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};

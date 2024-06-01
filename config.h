/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 0;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const unsigned int borderpx         = 3;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x0d0d0dff);
static const float bordercolor[]           = COLOR(0x0d0d0dff);
static const float focuscolor[]            = COLOR(0xcec2a4ff);
static const float urgentcolor[]           = COLOR(0xff0000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */
static const int floating_relative_to_monitor = 0; /* 0 means center floating relative to the window area */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
	"sh", "-c", "dbus-update-activation-environment --systemd DISPLAY WAYLAND_DISPLAY SWAYSOCK XDG_CURRENT_DESKTOP", NULL,
	"sh", "-c", "kanshi", NULL,
	"sh", "-c", "swaybg -i $HOME/.wallpapers/MoonlightMeditation.jpg -m fill", NULL,
	"sh", "-c", "mako", NULL,
	"sh", "-c", "redshift", NULL,
	"sh", "-c", "wl-paste -t text --watch clipman store", NULL,
	"sh", "-c", "LD_LIBRARY_PATH=/usr/lib/yambar yambar -c $HOME/.config/yambar/config-dwl.yml", NULL,
	NULL /* terminate */
};


static const Rule rules[] = {
	/* x, y, width, heigh are floating only */
	/* when x or y == 0 the client is placed at the center of the screen */
	/* when width or height == 0 the default size of the client is used */
	/* app_id     title       tags mask     isfloating  isterm          noswallow   monitor  x    y    width height scratchkey   */
	/* examples:
	{ "Gimp",     NULL,       0,            1,          0,              1,         -1,       0,   0,   0,    0,   0 },
	*/
	{ "Alacritty", NULL,      0,            0,          1,              0,         -1,       0,   0,   0,    0,   0 },
	{ NULL,       "termbox",  0,            1,          0,              0,         -1,       0,   0,   1400, 800, 't'  },
	{ NULL,       "chatbox",  0,            1,          0,              0,         -1,       0,   0,   1400, 800, 'c' },
	{ NULL,       "calcbox",  0,            1,          0,              0,         -1,       0,   0,   800,  300, 'a' },
	{ NULL,       "htopbox",  0,            1,          0,              0,         -1,       0,   15,   1920, 800, 'i' },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

/* monitors */
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact  nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.5f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.layout="us,ca",
	.options = "grp:alt_shift_toggle",
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 1;
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
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

static const int cursor_timeout = 8;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define ALCMD(cmd) { .v = (const char*[]){ "alacritty", "-t", cmd, "-e", cmd, NULL } }

/* commands */
// for changing the volume via pulsemixer //
static const char *volup[] = { "pulsemixer", "--change-volume", "+1", NULL };
static const char *voldown[] = { "pulsemixer", "--change-volume", "-1", NULL };
// for muting/unmuting //
static const char *mute[] = { "pulsemixer", "--toggle-mute", NULL };

static const char *termcmd[] = { "alacritty", NULL };
static const char *menucmd[] = { "wofi", "--show", "drun", "-i", "-p", "Execute:", NULL };
static const char *passcmd[] = { "passmenu", "-i", "-b", "-h", "26", "-fn", "'Fira Code 14'", "-sf", "#cec2a4", "-nf", "#cec2a4", "-nb", "#0d0d0d", "-sf", "#0d0d0d", "-sb", "#cec2a4", NULL};
static const char *firecmd[] = { "librewolf", NULL };
static const char *chrocmd[] = { "chromium", "--enable-features=UseOzonePlatform", "--ozone-platform=wayland", NULL };
// static const char *phoncmd[] = { "alacritty", "-e", "linphone-wl", NULL };
// static const char *phoncmd[] = { "QT_QPA_PLATFORM= linphone", NULL };
// TODO add keybinding for importing xwayland clipboard to wl clipboard (or do it automatically?)
// xclip -selection clipboard -o | wl-copy

/* scratchpads */
static const char *tmuxcmd[] = { "t", "alacritty", "-t", "termbox", "-e", "tmuxdd", NULL };
static const char *chatcmd[] = { "c", "alacritty", "-t", "chatbox", "-e", "weechat", NULL };
static const char *calccmd[] = { "a", "alacritty", "-t", "calcbox", "-e", "simplecalc", NULL };
static const char *htopcmd[] = { "i", "alacritty", "-t", "htopbox", "-e", "htop", NULL };

static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_at,                         1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                 2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                     3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                    4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_asciicircum,                5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_ampersand,                  6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_asterisk,                   7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenleft,                  8),
	TAGKEYS(          XKB_KEY_0, XKB_KEY_parenright,                 9),
	{ MODKEY,                    XKB_KEY_F1,         spawn,          SHCMD("alacritty -t configedit -e nvim ~/.local/src/dwl/config.h") },
	{ MODKEY,                    XKB_KEY_F3,         spawn,          ALCMD("bluetoothctl") },
	{ MODKEY,                    XKB_KEY_F4,         spawn,          ALCMD("bashmount") },
	{ MODKEY,                    XKB_KEY_F5,         spawn,          ALCMD("wlay") },
	{ MODKEY,                    XKB_KEY_Tab,        view,           {0} },
	// { MODKEY,                    XKB_KEY_semicolon,  shiftview,      { .i = 1 } },
	{ MODKEY,                    XKB_KEY_q,          killclient,     {0} },
	{ MODKEY,                    XKB_KEY_w,          spawn,          ALCMD("nmtui") },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_W,          spawn,          {.v = firecmd} },
	// { MODKEY,                    XKB_KEY_e,          togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_e,          spawn,          ALCMD("vifm") },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_E,          spawn,          {.v = chrocmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_R,          setlayout,      {.v = &layouts[3]} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_T,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    XKB_KEY_y,          spawn,          ALCMD("xplr") },
	{ MODKEY,                    XKB_KEY_u,          togglescratch,  { .v = tmuxcmd } },
	{ MODKEY,                    XKB_KEY_i,          togglescratch,  {.v = htopcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_I,          incnmaster,     {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_O,          incnmaster,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_p,          spawn,          SHCMD("QT_QPA_PLATFORM=xcb linphone") },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_P,          spawn,          {.v = passcmd} },
	{ MODKEY,                    XKB_KEY_a,          togglescratch,  { .v = calccmd } },
	// { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_A,          shiftview,      { .i = -1 } },
	{ MODKEY,                    XKB_KEY_s,          spawn,          ALCMD("sessionpicker") },
	{ MODKEY,                    XKB_KEY_d,          spawn,          {.v = menucmd} },
	{ MODKEY,                    XKB_KEY_f,          togglefullscreen, {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_F,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_J,          movestack,      {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_K,          movestack,      {.i = -1} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05f} },
	{ MODKEY,                    XKB_KEY_Return,     spawn,          {.v = termcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,     zoom,           {0} },
	{ MODKEY,                    XKB_KEY_x,          spawn,          SHCMD("lockdwl") },
	{ MODKEY,                    XKB_KEY_c,          togglescratch,  {.v = chatcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_C,          spawn,          SHCMD("bemoji -t") },
	{ MODKEY,                    XKB_KEY_v,          spawn,          SHCMD("clipman pick --tool=bemenu --tool-args='-i'") },
	{ MODKEY,                    XKB_KEY_b,          spawn,          SHCMD("calibre") },
	{ MODKEY,                    XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_M,          spawn,          SHCMD("env LD_PRELOAD=/usr/lib/spotify-adblock.so spotify --uri=%U")},
	{ MODKEY,                    XKB_KEY_space,      setlayout,      {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,      togglefloating, {0} },
	// { MODKEY,                    XKB_KEY_0,          view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright, tag,            {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Delete,     quit,           {0} },
	{ 0,                         XKB_KEY_Print,      spawn,          SHCMD("grim $(xdg-user-dir PICTURES)/$(date +'%Y-%m-%d-%H%M%S_screenshot.png')") },
	{ WLR_MODIFIER_SHIFT,        XKB_KEY_Print,      spawn,          SHCMD("grim -g \"$(slurp)\" ~/Pictures/$(date +'%Y-%m-%d-%H%M%S')_screenshot.png") },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
	{ 0,                  XKB_KEY_XF86AudioRaiseVolume,   spawn,          {.v = volup } },
	{ 0,                  XKB_KEY_XF86AudioLowerVolume,   spawn,          {.v = voldown } },
	{ 0,                  XKB_KEY_XF86AudioMute,          spawn,          {.v = mute } },
	{ 0,                  XKB_KEY_XF86AudioPrev,          spawn,          SHCMD("playerctl previous") },
	{ 0,                  XKB_KEY_XF86AudioPlay,          spawn,          SHCMD("playerctl play-pause") },
	{ 0,                  XKB_KEY_XF86AudioNext,          spawn,          SHCMD("playerctl next") },
	{ 0,                  XKB_KEY_XF86MonBrightnessUp,    spawn,          SHCMD("backlightctl 10") },
	{ WLR_MODIFIER_CTRL,  XKB_KEY_XF86MonBrightnessUp,    spawn,          SHCMD("backlightctl 1") },
	{ 0,                  XKB_KEY_XF86MonBrightnessDown,  spawn,          SHCMD("backlightctl -10") },
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};

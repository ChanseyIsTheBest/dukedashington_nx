/* config.c -- see config.h. MIT licensed, see LICENSE. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "config.h"
#include "log.h"

DdConfig g_cfg;

static const char *const INPUT_NAMES[IN_COUNT] = {
    "a", "b", "x", "y", "l", "r", "zl", "zr", "plus", "minus",
    "dpad_left", "dpad_right", "dpad_up", "dpad_down",
    "stick_left", "stick_right", "stick_up", "stick_down",
};

static const char *const CONTROL_NAMES[CTL_COUNT] = {
    "none", "left", "right", "up", "down",
    "enter", "space", "back", "restart", "pause",
};

const char *config_input_name(int i)   { return (i >= 0 && i < IN_COUNT) ? INPUT_NAMES[i] : "?"; }
const char *config_control_name(int c) { return (c >= 0 && c < CTL_COUNT) ? CONTROL_NAMES[c] : "?"; }

#define M(c) (1u << (c))

void config_defaults(void)
{
    int i;
    for (i = 0; i < IN_COUNT; i++)
        g_cfg.map[i] = 0;

    g_cfg.map[IN_A]      = M(CTL_ENTER);    /* confirm, as on any Switch game */
    g_cfg.map[IN_B]      = M(CTL_BACK);
    g_cfg.map[IN_X]      = M(CTL_UP);       /* X sits above A: dash up */
    g_cfg.map[IN_Y]      = M(CTL_RESTART);
    g_cfg.map[IN_R]      = M(CTL_UP);
    g_cfg.map[IN_PLUS]   = M(CTL_PAUSE);
    g_cfg.map[IN_MINUS]  = M(CTL_BACK);
    g_cfg.map[IN_DLEFT]  = M(CTL_LEFT);
    g_cfg.map[IN_DRIGHT] = M(CTL_RIGHT);
    g_cfg.map[IN_DUP]    = M(CTL_UP);
    g_cfg.map[IN_DDOWN]  = M(CTL_DOWN);
    g_cfg.map[IN_SLEFT]  = M(CTL_LEFT);
    g_cfg.map[IN_SRIGHT] = M(CTL_RIGHT);
    g_cfg.map[IN_SUP]    = M(CTL_UP);
    g_cfg.map[IN_SDOWN]  = M(CTL_DOWN);

    g_cfg.input_mode = DD_IN_BOTH;

    /* Where the game draws its on-screen control bar, as fractions of the
     * screen. ESTIMATES: the game builds this bar at runtime, so unlike the
     * rest of the port these were not read out of the game files. They are
     * settings for exactly that reason -- see README.md. x < 0 means the
     * control has no on-screen button. */
    for (i = 0; i < CTL_COUNT; i++) {
        g_cfg.touch_pos[i][0] = -1.0f;
        g_cfg.touch_pos[i][1] = -1.0f;
    }
    g_cfg.touch_pos[CTL_LEFT][0]  = 0.080f;  g_cfg.touch_pos[CTL_LEFT][1]  = 0.860f;
    g_cfg.touch_pos[CTL_RIGHT][0] = 0.240f;  g_cfg.touch_pos[CTL_RIGHT][1] = 0.860f;
    g_cfg.touch_pos[CTL_UP][0]    = 0.900f;  g_cfg.touch_pos[CTL_UP][1]    = 0.860f;
    g_cfg.touch_pos[CTL_PAUSE][0] = 0.950f;  g_cfg.touch_pos[CTL_PAUSE][1] = 0.080f;

    /* Fixed. */
    g_cfg.log_level      = DD_LOG_OFF;
    g_cfg.resolution     = DD_RES_AUTO;   /* 720p handheld, 1080p docked */
    g_cfg.button_layout  = DD_LAYOUT_LABEL;
    g_cfg.dpad           = DD_DPAD_HAT;
    g_cfg.stick_deadzone = 7000;
    g_cfg.touch          = 1;
    g_cfg.touch_mouse    = 1;
    g_cfg.rumble         = 1;
    g_cfg.exit_combo     = 0;              /* no quit chord: use HOME */
    g_cfg.minus_back_key = 0;
    g_cfg.dpi            = 160;
    g_cfg.game_stack_mb  = 16;
    g_cfg.gamepad        = 1;
    g_cfg.vsync          = 1;
    g_cfg.frame_stats    = 0;
    g_cfg.gc_working_mb  = 64;
    g_cfg.gc_free_mb     = 24;
    g_cfg.game_core      = -1;
    g_cfg.pad_reconnects = 4;
    g_cfg.touch_buttons  = 1;
    g_cfg.pointer        = 1;
    g_cfg.pointer_speed  = 1100;
}

static const char DEFAULT_TEXT[] =
"# Duke Dashington Remastered -- Nintendo Switch port\n"
"#\n"
"# Which Switch input drives which of the game's controls.\n"
"#\n"
"# Controls, and the key each one is bound to. These bindings are the game's\n"
"# own, read out of the Stencyl config compiled into libApplicationMain.so:\n"
"#   left      dash left, menu left      LEFT   (or A)\n"
"#   right     dash right, menu right    RIGHT  (or D)\n"
"#   up        dash up, menu up          UP     (or W)\n"
"#   down      menu down                 DOWN   (or S)\n"
"#   enter     confirm                   ENTER\n"
"#   space     confirm (the game takes either)   SPACE\n"
"#   back      back out, pause           ESCAPE\n"
"#   restart   restart the level         R\n"
"#   pause     the on-screen pause button -- no key, touch only\n"
"#   none      nothing\n"
"#\n"
"# An input may drive several controls at once, comma separated. Only combine\n"
"# controls that cannot both matter in the same place: 'a = up, enter' looks\n"
"# tempting, but on the level-select screens up moves the selection AND enter\n"
"# confirms it, so one press would do both.\n"
"#\n"
"# ZL and ZR together always raise the on-screen cursor, whatever they are\n"
"# assigned to here. Delete this file to restore the defaults.\n"
"\n"
"a = enter\n"
"b = back\n"
"x = up\n"
"y = restart\n"
"l = none\n"
"r = up\n"
"zl = none\n"
"zr = none\n"
"plus = pause\n"
"minus = back\n"
"\n"
"dpad_left = left\n"
"dpad_right = right\n"
"dpad_up = up\n"
"dpad_down = down\n"
"\n"
"stick_left = left\n"
"stick_right = right\n"
"stick_up = up\n"
"stick_down = down\n"
"\n"
"# How a press reaches the game.\n"
"#   keys   send the key the control is bound to. Every control except pause\n"
"#          has one, and the bindings above are the game's own.\n"
"#   touch  press the game's on-screen button instead, at the positions below.\n"
"#   both   key where the control has one, on-screen button where it does not\n"
"#          (default). In practice that means keys for everything except pause.\n"
"#\n"
"# 'both' deliberately does NOT do both for the same control. The positions\n"
"# below are estimates, and the level-select screens are covered in buttons --\n"
"# a stray finger there would tap a level while you were only moving the\n"
"# selection. Use 'touch' only if the keys turn out not to work.\n"
"input_mode = both\n"
"\n"
"# Where the game's on-screen buttons are, as fractions of the screen\n"
"# (0,0 = top left, 1,1 = bottom right). THESE ARE ESTIMATES -- the game\n"
"# builds its control bar at runtime, so unlike the key bindings they could\n"
"# not be read out of the game files. Only 'pause' is used by default.\n"
"# Set a position to 'none' to give that control no on-screen button.\n"
"touch_pause = 0.950, 0.080\n"
"touch_left = 0.080, 0.860\n"
"touch_right = 0.240, 0.860\n"
"touch_up = 0.900, 0.860\n"
"touch_back = none\n"
"touch_enter = none\n"
"touch_space = none\n"
"touch_down = none\n"
"touch_restart = none\n";

static char *trim(char *s)
{
    char *e;
    while (*s && isspace((unsigned char)*s))
        s++;
    e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1]))
        *--e = '\0';
    return s;
}

static int control_by_name(const char *v)
{
    int c;
    for (c = 0; c < CTL_COUNT; c++)
        if (!strcasecmp(v, CONTROL_NAMES[c]))
            return c;
    return -1;
}

/* "up, enter" -> M(CTL_UP)|M(CTL_ENTER). Returns 0 if nothing parsed. */
static int parse_control_set(const char *v, unsigned int *out)
{
    char buf[128], *p;
    unsigned int set = 0;
    int any = 0;

    snprintf(buf, sizeof(buf), "%s", v);
    p = buf;
    while (*p) {
        char *tok, *end;
        int c;
        while (*p == ',' || isspace((unsigned char)*p))
            p++;
        if (!*p)
            break;
        tok = p;
        while (*p && *p != ',')
            p++;
        end = p;
        if (*p)
            *p++ = '\0';
        while (end > tok && isspace((unsigned char)end[-1]))
            *--end = '\0';
        c = control_by_name(tok);
        if (c < 0) {
            LOGI("config: '%s' is not a control name", tok);
            continue;
        }
        if (c != CTL_NONE)
            set |= 1u << c;
        any = 1;
    }
    if (!any)
        return 0;
    *out = set;
    return 1;
}

/* "0.08, 0.86" or "none". */
static int parse_pos(const char *v, float *x, float *y)
{
    char *e1, *e2;
    float a, b;

    if (!strcasecmp(v, "none") || !strcasecmp(v, "off")) {
        *x = *y = -1.0f;
        return 1;
    }
    a = strtof(v, &e1);
    if (e1 == v)
        return 0;
    while (*e1 == ',' || isspace((unsigned char)*e1))
        e1++;
    b = strtof(e1, &e2);
    if (e2 == e1)
        return 0;
    *x = a;
    *y = b;
    return 1;
}

static void apply(const char *k, const char *v)
{
    int i, c;

    for (i = 0; i < IN_COUNT; i++) {
        unsigned int set;
        if (strcasecmp(k, INPUT_NAMES[i]) != 0)
            continue;
        if (parse_control_set(v, &set))
            g_cfg.map[i] = set;
        else
            LOGI("config: leaving %s as it was", INPUT_NAMES[i]);
        return;
    }
    if (!strncasecmp(k, "touch_", 6)) {
        c = control_by_name(k + 6);
        if (c > CTL_NONE && c < CTL_COUNT) {
            float x, y;
            if (parse_pos(v, &x, &y)) {
                g_cfg.touch_pos[c][0] = x;
                g_cfg.touch_pos[c][1] = y;
            } else {
                LOGI("config: '%s' is not a position", v);
            }
            return;
        }
    }
    if (!strcasecmp(k, "input_mode")) {
        if (!strcasecmp(v, "keys"))       g_cfg.input_mode = DD_IN_KEYS;
        else if (!strcasecmp(v, "touch")) g_cfg.input_mode = DD_IN_TOUCH;
        else if (!strcasecmp(v, "both"))  g_cfg.input_mode = DD_IN_BOTH;
        else LOGI("config: input_mode '%s' is not keys, touch or both", v);
        return;
    }
    /* Undocumented, for working out why something misbehaves: log_level = 1
     * (or 2) turns duke.log back on. */
    if (!strcasecmp(k, "log_level")) {
        g_cfg.log_level = atoi(v);
        return;
    }
    LOGI("config: unknown key '%s'", k);
}

int config_load(const char *path)
{
    char line[256];
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fp = fopen(path, "w");
        if (fp) {
            fputs(DEFAULT_TEXT, fp);
            fclose(fp);
        }
        return 0;
    }
    while (fgets(line, sizeof(line), fp)) {
        char *s = trim(line), *eq;
        if (!*s || *s == '#' || *s == ';')
            continue;
        eq = strchr(s, '=');
        if (!eq)
            continue;
        *eq = '\0';
        apply(trim(s), trim(eq + 1));
    }
    fclose(fp);
    return 1;
}

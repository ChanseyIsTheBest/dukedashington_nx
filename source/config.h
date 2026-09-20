/* config.h -- settings.
 *
 * Two things about this port are adjustable, and both are in config.txt:
 *
 *   1. which Switch input drives which of the game's controls, and
 *   2. where the game's on-screen buttons sit, for the touch fallback.
 *
 * (2) is a setting here, unlike in the Total Party Kill port where the HUD
 * positions were fixed: Duke Dashington builds its control bar at runtime
 * rather than placing it in the scene data, so the positions could not be
 * read out of the game files and have to be confirmable on hardware.
 *
 * Everything else is fixed at build time. The file is written with the
 * defaults on first run so the choices are discoverable.
 *
 * MIT licensed, see LICENSE.
 */
#ifndef DD_CONFIG_H
#define DD_CONFIG_H

enum { DD_RES_AUTO = 0, DD_RES_720 = 720, DD_RES_1080 = 1080 };
enum { DD_LAYOUT_LABEL = 0, DD_LAYOUT_POSITION = 1 };
enum { DD_DPAD_HAT = 0, DD_DPAD_STICK = 1, DD_DPAD_BOTH = 2 };

/* How a press reaches the game.
 *
 *   keys   synthesize the key the control is bound to
 *   touch  press the game's on-screen button at its screen position
 *   both   key where the control has one, on-screen button where it does not
 *          (the default)
 *
 * "both" is not "do each of them". Doing both for the same control would land
 * a finger somewhere on every press, and the on-screen positions are estimates
 * while the key bindings are the game's own -- on a level-select screen full of
 * buttons that would tap a level while you were only moving the selection. The
 * only control with no key is CTL_PAUSE, so in practice "both" means keys for
 * everything and a real press for the pause button.
 */
enum { DD_IN_KEYS = 0, DD_IN_TOUCH = 1, DD_IN_BOTH = 2 };

/* The game's controls.
 *
 * These are the control names the game's own behaviours listen on, read out
 * of libApplicationMain.so: DukeControls handles Up/Left/Right and Restart,
 * and the menu behaviours (TitleKeyboardControls, LevelSelectMenuKeyboard01,
 * DungeonSelectKeyboardControls) handle Left/Right/Up/Down/Enter/Space/Esc.
 * CTL_PAUSE has no key -- the pause button is on-screen only.
 */
enum {
    CTL_NONE = 0,
    CTL_LEFT, CTL_RIGHT, CTL_UP, CTL_DOWN,
    CTL_ENTER, CTL_SPACE, CTL_BACK, CTL_RESTART, CTL_PAUSE,
    CTL_COUNT
};

/* What a player can press. */
enum {
    IN_A, IN_B, IN_X, IN_Y, IN_L, IN_R, IN_ZL, IN_ZR, IN_PLUS, IN_MINUS,
    IN_DLEFT, IN_DRIGHT, IN_DUP, IN_DDOWN,
    IN_SLEFT, IN_SRIGHT, IN_SUP, IN_SDOWN, IN_COUNT
};

typedef struct {
    /* The editable settings.
     *
     * map[] is a set, not a single control: one input may drive several, so
     * that A can be both "dash up" in a level and "confirm" in a menu. The
     * game only ever acts on the one that means something where it is, so
     * the two never collide. Bit (1u << CTL_*). */
    unsigned int map[IN_COUNT];
    float touch_pos[CTL_COUNT][2];   /* screen fractions; x < 0 = no button */
    int input_mode;

    /* fixed at build time */
    int log_level;
    int resolution;
    int button_layout;
    int dpad;
    int stick_deadzone;
    int touch;
    int touch_mouse;
    int rumble;
    int exit_combo;
    int minus_back_key;
    int dpi;
    int game_stack_mb;
    int gamepad;
    int vsync;
    int frame_stats;
    int gc_working_mb;
    int gc_free_mb;
    int game_core;
    int pad_reconnects;
    int touch_buttons;
    int pointer;
    int pointer_speed;
} DdConfig;

extern DdConfig g_cfg;

void config_defaults(void);
/* Reads the settings; writes the file with the defaults if absent. */
int  config_load(const char *path);

const char *config_input_name(int input);
const char *config_control_name(int control);

#endif

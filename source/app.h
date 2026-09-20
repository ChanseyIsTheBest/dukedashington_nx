/* app.h -- process lifecycle shared by the shims.
 *
 * libnx's exit path has to run on the main thread (it restores the loader's
 * context), but the game asks to exit from its own thread: exit() from hxcpp's
 * Sys.exit, abort(), Activity.moveTaskToBack from Lime's System.exit, the
 * HOME-menu close request, or the + and - combo. All of those funnel into
 * dd_request_exit(), which wakes the main thread and parks the caller.
 *
 * MIT licensed, see LICENSE.
 */
#ifndef DD_APP_H
#define DD_APP_H

void dd_request_exit(int code);
int  dd_exit_requested(void);
int  dd_exit_code(void);
void dd_park_forever(void) __attribute__((noreturn));

/* The game has been handed SDL_QUIT. If it has not exited a few seconds
 * later (a stuck shutdown), the main thread exits anyway. */
void dd_note_quit(void);

/* Seconds since start, for rate-limited logging. */
double dd_uptime(void);

#endif

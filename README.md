# Duke Dashington Remastered — Nintendo Switch port (Stencyl / OpenFL / Lime wrapper)
 
This is a native wrapper / loader that runs the original ARM64 Android build of Duke Dashington Remastered on Switch homebrew. It contains **no game code and no game assets** — it loads the game's own libraries and recreates, natively, the Android layer underneath them: bionic's C library, SDL, OpenGL ES, audio, input, and the Java/JNI side the Lime runtime expects.
 
## Install & run
 
You need files from the Duke Dashington Remastered Android v1.2.3 release (arm64-v8a).
 
Put the `.nro` in any folder under `sdmc:/switch/` and place your game files next to it — the loader finds its folder at runtime, so the name is up to you:
 
```
sdmc:/switch/dukedashington_nx
├── dukedashington.nro
├── liblime.so
├── libApplicationMain.so
├── cursor.png                              <- optional
└── assets
```
 
Launch via title override (hold R while starting an installed game).
 
Optionally drop a `cursor.png` (up to 64×64, transparency respected) in the same folder to replace the on-screen cursor with your own.
 
## Controls
 
The Android build of this game reads a keyboard, a mouse and a touch screen — its Stencyl input layer is control-driven, and the map from each control to the key that triggers it is compiled into the binary. So the pad presses the game's own keys. That works everywhere the game does: in a level, on the title screen, in the dungeon and level select, and in the pause menu.
 
| Input | Action |
|---|---|
| Touchscreen | Direct multi-touch — the game as designed (handheld) |
| Left stick, D-pad | The arrow keys — dash left, right and up, and the menu cursor |
| X, R | Dash up |
| A | Enter — confirms in menus |
| B, − | Escape — back out, and pause in a level |
| Y | Restart the level |
| + | The on-screen pause button |
| ZL + ZR | Toggle the on-screen cursor |
| Left stick (cursor up) | Move the cursor |
| A (cursor up) | Tap at the cursor |
 
A confirms and X dashes up, rather than A doing both, because the game's own controls collide there: on the dungeon- and level-select screens `up` moves the selection and `enter` confirms it, so one button driving both would do both at once. `pause` is the one control the game gives no key, which is why `+` presses the on-screen button instead of typing.
 
## Remapping — `config.txt`
 
```
a = enter
b = back
x = up
y = restart
l = none
r = up
zl = none
zr = none
plus = pause
minus = back
 
dpad_left = left
dpad_right = right
dpad_up = up
dpad_down = down
 
stick_left = left
stick_right = right
stick_up = up
stick_down = down
 
input_mode = both
 
touch_pause = 0.950, 0.080
touch_left = 0.080, 0.860
touch_right = 0.240, 0.860
touch_up = 0.900, 0.860
touch_back = none
touch_enter = none
touch_space = none
touch_down = none
touch_restart = none
```
 
Assignable controls are `left`, `right`, `up`, `down`, `enter`, `space`, `back`, `restart`, `pause` and `none`. Several inputs may share one control — the D-pad and the stick both drive the arrows, X and R both dash up; a shared control stays down until every input holding it lets go. One input may also drive several controls, comma separated (`a = up, enter`), but only combine controls that cannot both matter in the same place. Delete the file to restore the defaults.
 
`input_mode` is `keys`, `touch` or `both`. `both` is the default and sends the key where the control has one and presses the game's on-screen button where it does not — never both for the same control, so a press never also lands a stray finger on a level-select screen. The `touch_*` lines are those on-screen button positions as fractions of the screen; only `pause` uses one by default.
 
## Building
 
Requires devkitPro with the `switch-dev` group plus these portlibs:
 
```
pacman -S switch-dev
pacman -S switch-mesa switch-libdrm_nouveau switch-sdl2 switch-libpng switch-zlib
 
export DEVKITPRO=/opt/devkitpro
make                        # -> dukedashington.nro
```
 
## Credits
 
The loader/shim infrastructure (`so_util`, the bionic shims, `jni_env`/`jni_classes`, `dl_bridge`) derives from the open-source Switch `.so`-loader lineage — Andy Nguyen and fgsfds, building on TheOfficialFloW's Vita/Switch loader tradition — with the Bloons Pop, Mulmash, PvZ Ultimate and MBHaxe Switch ports as references. The on-screen cursor (`nx_pointer`) comes from the Happy Wheels Switch port, with its cursor.png loader and GL overlay intact. All MIT-licensed.
 
This port is a sibling of the Total Party Kill Switch port and shares its wrapper layer unchanged — `liblime.so` is byte-identical between the two releases. The input layer is the part that differs, for the reason described above.
 
`source/sdl_procs.h` is generated from the function list in SDL 2.0.12's `src/dynapi/SDL_dynapi_procs.h` (zlib licensed, © 1997-2020 Sam Lantinga) — function names only, no SDL code.
 
Thanks to everyone in that lineage for making this approach possible.
 

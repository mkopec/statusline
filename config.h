/* config.h.  Generated from config.h.in by configure.  */
/* See LICENSE file for copyright and license details. */

/* Refresh rate */
static const int sleepduration = 1;

/* ALSA soudcard */
static const char soundcard[] = "default";
static const char channel[]   = "Master";

/* Temperature */
static const char tempfile[] = "/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp1_input";

/* Battery */
static const char batterynowfile[]  = "/sys/class/power_supply/BAT0/energy_now";
static const char batteryfullfile[] = "/sys/class/power_supply/BAT0/energy_full";

/* Music player */
static char musicplayername[] = "spotify";

/* Date & time */
static const char timeformat[] = "%a %d %b, %H:%M";

/* Statusline formatting */
#define FORMATSTRING "  %s   |     %s   |     %s   |     %s   |   %s\n"
#define ARGUMENTS temperature, battery, volume, music, datetime

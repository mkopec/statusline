/* See LICENSE file for copyright and license details. */

/* Global libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <alsa/asoundlib.h>
#include <playerctl/playerctl.h>

/* Functions */
char *get_battery();
char *get_volume();
char *get_music();
char *get_datetime();
char *get_temperature();

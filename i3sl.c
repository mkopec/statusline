/* See LICENSE file for copyright and license details. */

/* Global libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/* Local libraries */
#include "config.h"
#include "blocks.h"

/* Functions */
void signalhandler(int signum);

/* Basic signal handling */
void signalhandler(int signum)
{
    /* Log signals to stderr */
    fprintf(stderr, "Signal received: %d\n", signum);

    /* playerctl needs a small delay to get current track */
    if (signum == SIGUSR2)
        usleep(100000);
}

/* Main funtion */
int main(int argc, char const *argv[]) {
    /* Locale for date and time */
    setlocale(LC_ALL, "");

    /* Signal handlers for hotkeys */
    signal(SIGUSR1, signalhandler);
    signal(SIGUSR2, signalhandler);

    char *temperature = NULL;
    char *battery = NULL;
    char *volume = NULL;
    char *music = NULL;
    char *datetime = NULL;

    /* Main loop */
    while(1) {
        /* Get new data */
        temperature = get_temperature();
        battery = get_battery();
        volume = get_volume();
        music = get_music();
        datetime = get_datetime();

        /* Print information */
        fprintf(stdout, FORMATSTRING, ARGUMENTS);
            /* TODO: Print according to i3bar JSON protocol */
        fflush(stdout);

        /* Free unneeded pointers */
        free(temperature);
        free(battery);
        free(volume);
        free(music);
        free(datetime);

        /* Wait for a specified duration unless a signal is received */
        sleep(sleepduration);
    }

    /* Exit successfully */
    return EXIT_SUCCESS;
}

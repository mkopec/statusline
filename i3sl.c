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
void pretty_print();

/* Basic signal handling */
void signalhandler(int signum)
{
    /* Log signals to stderr */
    fprintf(stderr, "Signal received: %d\n", signum);

    /* playerctl needs a small delay to get current track */
    if (signum == SIGUSR2)
        usleep(100000);
}

/* Print in i3bar JSON format */
void pretty_print()
{
    int i;
    int n_arguments;

    char *temperature = NULL;
    char *battery = NULL;
    char *volume = NULL;
    char *music = NULL;
    char *datetime = NULL;

    char **arguments[] = { ARGUMENTS };

    n_arguments = sizeof(arguments) / sizeof(arguments[1]);

    /* First line needs to be empty */
    printf("{\"version\":1}\n[[]\n");

    /* Main loop */
    while(1) {
        /* Get new data */
        temperature = get_temperature();
        battery = get_battery();
        volume = get_volume();
        music = get_music();
        datetime = get_datetime();

        /* Print information */
        printf(",[");
        for (i = 0; i < n_arguments; ++i) {
            printf("{\"full_text\": \" %s \",\"separator_block_width\":15}", *arguments[i]);
            if (i < n_arguments-1)
                printf(",");
        }
        printf("]\n");
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
}

/* Main funtion */
int main() {
    /* Locale for date and time */
    setlocale(LC_ALL, "");

    /* Signal handlers for hotkeys */
    signal(SIGUSR1, signalhandler);
    signal(SIGUSR2, signalhandler);

    pretty_print();

    /* Exit successfully */
    return EXIT_SUCCESS;
}

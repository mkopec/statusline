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

/* Local libraries */
#include "config.h"
#include "blocks.h"

/* Battery percentage */
char *get_battery()
{
    int energy_now, energy_full;
    char *battery_string = malloc(1024);

    FILE *fp;

    /* Read current battery level */
    if (!(fp = fopen(batterynowfile, "r"))) {
        fprintf(stderr, "Error opening battery file.");
        exit(1);
    }

    fscanf(fp, "%i", &energy_now);
    fclose(fp);

    /* Read maximum battery level */
    if (!(fp = fopen(batteryfullfile, "r"))) {
        fprintf(stderr, "Error opening battery file.");
        exit(1);
    }

    fscanf(fp, "%i", &energy_full);
    fclose(fp);

    /* Format battery level nicely */
    sprintf(battery_string, "%d%%", energy_now / (energy_full / 100));

    return battery_string;
}

/* Volume percentage */
char *get_volume()
{
    int mute = 0;
    long volume_now = 0, volume_max = 0, volume_min = 0;
    char *volume_string = malloc(1024);

    /* Create new ALSA mixer and connect it to the specified soundcard */
    snd_mixer_t *handle;
    snd_mixer_elem_t *pcm_mixer, *mas_mixer;
    snd_mixer_selem_id_t *vol_info, *mute_info;
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, soundcard);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);
    snd_mixer_selem_id_malloc(&vol_info);
    snd_mixer_selem_id_malloc(&mute_info);
    snd_mixer_selem_id_set_name(vol_info, channel);
    snd_mixer_selem_id_set_name(mute_info, channel);
    pcm_mixer = snd_mixer_find_selem(handle, vol_info);
    mas_mixer = snd_mixer_find_selem(handle, mute_info);

    /*  Get current volume and mute status */
    snd_mixer_selem_get_playback_volume_range((snd_mixer_elem_t *)pcm_mixer,
                    &volume_min, &volume_max);
    snd_mixer_selem_get_playback_volume((snd_mixer_elem_t *)pcm_mixer,
                    SND_MIXER_SCHN_MONO, &volume_now);
    snd_mixer_selem_get_playback_switch(mas_mixer, SND_MIXER_SCHN_MONO,
                    &mute);
    if (vol_info)
        snd_mixer_selem_id_free(vol_info);
    if (mute_info)
        snd_mixer_selem_id_free(mute_info);
    if (handle)
        snd_mixer_close(handle);

    if (!mute)
        strcpy(volume_string, "mute");
    else
        sprintf(volume_string, "%ld%%", (volume_now * 100) / volume_max);

    return volume_string;
}

/* Currently playing song info */
char *get_music()
{
    char *music_string = malloc(2048);

    /* Initialize new PlayerctlPlayer object */
    PlayerctlPlayer *player = playerctl_player_new(musicplayername, NULL);

    /* Get info from player */
    char *music_title = playerctl_player_get_title(player, NULL);
    char *music_artist = playerctl_player_get_artist(player, NULL);

    /* Show only if something is playing */
    if (music_title != NULL) {
        sprintf(music_string, "%s - %s", music_artist, music_title);
    } else {
        sprintf(music_string, "none");
    }

    return music_string;
}

/* Date and time */
char *get_datetime()
{
    char *datetime_string = malloc(1024);

    time_t rawtime;
    struct tm * timeinfo;

    /* Get time from system */
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    /* Format the time nicely */
    strftime(datetime_string, 1024, timeformat, timeinfo);

    return datetime_string;
}

/* Onboard temperature */
char *get_temperature()
{
    int temperature_now;
    char *temperature_string = malloc(1024);

    FILE *fp;

    /* Get current temperature from sensor */
    if (!(fp = fopen(tempfile, "r"))) {
        fprintf(stderr, "Error opening temperature file.");
        exit(1);
    }

    fscanf(fp, "%i", &temperature_now);
    fclose(fp);

    /* Format the temperature nicely */
    sprintf(temperature_string, "%d °C", temperature_now / 1000);

    return temperature_string;
}

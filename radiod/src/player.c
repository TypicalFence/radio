/*
 * This file is part of RadioD.
 * Copyright (C) 2021 fence.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "player.h"

#include <stdlib.h>
#include <mpv/client.h>

#include "stations.h"

static char *current_station = NULL;
static mpv_handle *mpv; 

static void play_stream() {
    RadioStation *station = get_station(current_station);

    if (station != NULL) {
        const char *cmd[] = {"loadfile", station->stream_url, NULL};
        mpv_command(mpv, cmd);
    }
}

void init_player() {
    mpv = mpv_create();
    mpv_set_option_string(mpv, "vid", "no");
    mpv_initialize(mpv);
}

PlayerState player_get_state() {
    PlayerState state;
    state.current_station = current_station;
    return state;
}

void player_switch_station(char *id) {
    current_station = id;
    play_stream();
}

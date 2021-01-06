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

#include "log.h"
#include "config.h"
#include "socket.h"
#include "stations.h"
#include "player.h"

RadioStation *station;

int main() {
    load_config();
    init_stations();
    init_player();

    station = get_station("cyberia");
    log_info(station->name);

    station = get_station("plazaone");
    log_info(station->name);
    
    RadioStation **my_stations = get_stations();
    log_info("%i", get_station_count()); 
    for(int i = 0; i < get_station_count(); i++) {
        log_info(my_stations[i]->name);
    }

    int socket_status = socket_init();

    if (socket_status == 0) {
        socket_listen();
        return 0;
    }

    return 1;
}

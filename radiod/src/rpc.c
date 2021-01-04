/*
 * This file is part of RadioD.
 * Copyright (C) 2020 fence.
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

#include "rpc.h"

#include <string.h>

#include "jsonrpc.h"
#include "player.h"

char *handle_request(char *string) {
    int msg_type = check_rpc_string(string);

    if (msg_type != 1) {
        return "no";
    }
    
    Request request = parse_request(string);

    if (strcmp(request.method, "switch_station") == 0) {
        log_debug("switch_station called");
        cJSON *id_json = cJSON_GetObjectItemCaseSensitive(request.params, "id");

        if (id_json) {
            char *id = id_json->valuestring; 
            player_switch_station(id);
        }
    }

    return "yes";
}


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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "log.h"
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

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
static bool has_version_attr(cJSON *json) {
    cJSON *jsonrpc = cJSON_GetObjectItemCaseSensitive(json, "jsonrpc");

    if (cJSON_IsString(jsonrpc) && (jsonrpc->valuestring != NULL)) {
        return strcmp(jsonrpc->valuestring, "2.0") == 0;
    }
    
    return false;
}

static bool has_params_attr(cJSON *json) {
    cJSON *params = cJSON_GetObjectItemCaseSensitive(json, "params");
    return params != NULL;
}

static bool has_result_attr(cJSON *json) {
    cJSON *result = cJSON_GetObjectItemCaseSensitive(json, "result");
    return result != NULL;
}

 
// TODO check for method attribute
int check_rpc_string(char *string) {
    cJSON *json = cJSON_Parse(string);    

    if(json != NULL) {
        if (has_version_attr(json)) {
            if (has_params_attr(json)) {
                cJSON_Delete(json);
                return 1;
            }
            
            if (has_result_attr(json)) {
                cJSON_Delete(json);
                return 2;
            }
        }
    }

    cJSON_Delete(json);
    return 0;
}

// TODO makes this return a pointer
// return Null on failure
Request parse_request(char *string) {
    cJSON *json = cJSON_Parse(string);
    Request request;
    
    cJSON *method = cJSON_GetObjectItemCaseSensitive(json, "method");
    request.method = malloc(strlen(method->valuestring));
    strcpy(request.method, method->valuestring);
    
    cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "id");
    request.id = malloc(strlen(id->valuestring));
    strcpy(request.id, id->valuestring);


    // I think this leaks memory, look into cloning that json
    cJSON *params = cJSON_GetObjectItemCaseSensitive(json, "params");
    request.params = params;

    return request;
}




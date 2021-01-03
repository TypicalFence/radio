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

#include "jsonrpc.h"
#include "log.h"


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


Request parse_request(char *string) {
    cJSON *json = cJSON_Parse(string);
    Request request;
    
    return request;
}

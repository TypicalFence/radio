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

static char *method_switch_station(Request *req) {
    Response response;
    char *result;

    log_debug("switch_station called");

    response.id = req->id;
    response.method = "switch_station";
    response.result = NULL;
    response.error = NULL;
    
    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(req->params, "id");

    if (id_json) {
        char *id = id_json->valuestring; 
        player_switch_station(id);
        response.result = cJSON_CreateString("ok");
    } else {
        response.error = create_error(400, "no id in params", NULL);
    }


    return encode_response(&response);
}


char *handle_request(char *string) {
    int msg_type = check_rpc_string(string);
    Response response;

    if (msg_type != 1) {
        response.id = NULL;
        response.method = NULL;
        response.result = NULL;
        response.error = create_error(400, "invalid request", NULL);
        char *result = encode_response(&response);
        return result;
    }
    
    Request request = parse_request(string);
    
    if (strcmp(request.method, "switch_station") == 0) {
        return method_switch_station(&request);
    }
    
    // send not found
    response.id = NULL;
    response.method = NULL;
    response.result = NULL;
    response.error = create_error(404, "method not found", NULL);
    char *result = encode_response(&response);
    return result;
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

static bool has_method_attr(cJSON *json) {
    cJSON *result = cJSON_GetObjectItemCaseSensitive(json, "method");
    return result != NULL;
}
 
int check_rpc_string(char *string) {
    cJSON *json = cJSON_Parse(string);    

    if(json != NULL) {
        if (has_version_attr(json)) {
            if (has_params_attr(json) && has_method_attr(json)) {
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

// ----------------------------------------------------------------------------
// Response building
// ----------------------------------------------------------------------------
char *encode_response(Response *resp) {
    cJSON *response_json = cJSON_CreateObject();

    cJSON *jsonrpc = cJSON_CreateString("2.0");
    cJSON_AddItemToObject(response_json, "jsonrpc", jsonrpc);

    cJSON *id;

    if (resp->id == NULL) {
        id = cJSON_CreateNull();
    } else {
        id = cJSON_CreateString(resp->id);
    }

    cJSON_AddItemToObject(response_json, "id", id);

    cJSON *method;

    if (resp->method == NULL) {
        method = cJSON_CreateNull();
    } else {
        method = cJSON_CreateString(resp->method);
    }

    cJSON_AddItemToObject(response_json, "method", method);
    
    if (resp->result != NULL) {
        cJSON_AddItemToObject(response_json, "result", resp->result);
    }
    
    if (resp->error != NULL) {
        cJSON_AddItemToObject(response_json, "error", resp->error);
    }

    char *string = cJSON_PrintUnformatted(response_json);
    cJSON_Delete(response_json);
    return string;
}

cJSON *create_error(int code, char *message, cJSON *data) {
    cJSON *error = cJSON_CreateObject();
    cJSON *code_json = cJSON_CreateNumber(code);
    cJSON_AddItemToObject(error, "code", code_json);

    cJSON *message_json; 
    
    if (message == NULL) {
        message_json = cJSON_CreateNull();
    } else {
        message_json = cJSON_CreateString(message);
    }

    cJSON_AddItemToObject(error, "message", message_json);
    
    if (data == NULL) {
        cJSON_AddItemToObject(error, "data", cJSON_CreateNull());
    } else {
        cJSON_AddItemToObject(error, "data", data);
    } 

    return error;
}

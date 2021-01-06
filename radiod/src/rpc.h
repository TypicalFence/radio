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

#ifndef RPC_H
#define RPC_H

#include <cJSON.h>

/**
 * This is hardly a complete implementation of jsonrpc.
 * it assumes some things, but it should be mostly compliant.
 * I am lazy and C is to hard for this, so yeah.
 * 
 * Assumtaions:
 * - We only support 2.0
 * - We assume id to be a string, for simplicity.  
 */
typedef struct {
    char* id;
    char* method;
    cJSON* params;
} Request;

typedef struct {
    char* id;
    char* method;
    cJSON* result;
    cJSON* error;
} Response;

/**
 * Checks if a given string is a valid rpc message.
 * Returns:
 *     - 0 not a rpc message
 *     - 1 rpc request
 *     - 2 rpc response
 */
int check_rpc_string(); 
Request parse_request(char *string);
cJSON *create_error(int code, char *message, cJSON *data); 
char *encode_response(Response *resp);
char *handle_request(char *);

#endif

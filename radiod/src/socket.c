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

#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "log.h"
#include "rpc.h"

#define DEFAULT_PORT 8069


int sock;
char buffer[BUF_SIZE];
int rec_value, length;

int socket_init() {
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock < 0) {
        log_error("can not open socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(config.socket_port);
    
    if(bind(sock, &addr, sizeof(struct sockaddr_in))) {
        log_error("can not bind socket");
        return 1;
    }

    listen(sock, MAX_QUEUE);
    return 0;
}

static int socket_handle_client() {
    int temp_sock = accept(sock, 0, 0);
    if (temp_sock < 0) {
        return 1;
    }
    

    int bytesRead = 0;
    char *message = malloc(BUF_SIZE);
    
    while (rec_value=read(temp_sock, buffer, BUF_SIZE)) {
        if(rec_value < 0) {
            printf("fug :DDDD");
            exit(1);
        } else {
            realloc(message, strlen(message) + strlen(buffer));
            log_debug(buffer);
            strcat(message, buffer);
            bytesRead += rec_value;
        }
    }
    
    log_info(message);
    log_info(handle_request(message)); 
    free(message);

    log_info("Ending connection.");
    close(temp_sock);
    return 0;
}

int socket_listen() {
    while(true) {
        log_info("listening on port: %i", config.socket_port);
        socket_handle_client();
    }
}

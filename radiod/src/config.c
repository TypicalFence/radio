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

#include "./config.h"

Config load_config() {
    Config config;
    dictionary *daemon_ini = iniparser_load(CONFIG_PATH "/daemon.ini");
    
    config.socket_port = iniparser_getint(daemon_ini, "socket:port", DEFAULT_SOCKET_PORT);

    iniparser_freedict(daemon_ini);
    return config;
}

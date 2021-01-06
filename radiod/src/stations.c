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

#include "stations.h"

#include <stdbool.h>
#include <string.h>
#include <iniparser.h>

#include "log.h"
#include "config.h"

#define TABLE_SIZE 100

/*
 * We store RadioStations in a HashTable.
 *
 * This should allow changing the channel quickly even if the user has thousands 
 * of stations in the config.
 *
 * Just using a linked list would have probably also have worked fine...
 */
typedef struct {
    char *key;
    RadioStation *value;
    struct TableItem *next;
} TableItem;

typedef struct {
    TableItem **items;
    char **keys;
    int count;
} StationTable;

static StationTable *stations;

void free_station(RadioStation *station) {
    free(station->id);
    free(station->name);
    free(station->stream_url);
    free(station);
}

RadioStation  *copy_station(RadioStation *station) {
    RadioStation *new_station = malloc(sizeof(RadioStation) * 1);
    new_station->id = malloc(strlen(station->id) + 1);
    new_station->name = malloc(strlen(station->name) + 1);
    new_station->stream_url = malloc(strlen(station->stream_url) + 1);

    strcpy(new_station->id, station->id);
    strcpy(new_station->stream_url, station->stream_url);
    strcpy(new_station->name, station->name);

    return new_station;
}

TableItem *new_table_item(RadioStation *station) {
    TableItem *item = malloc(sizeof(TableItem) * 1);
    item->key = malloc(strlen(station->id) + 1);
    
    strcpy(item->key, station->id);
    item->value = copy_station(station);
    item->next = NULL;
    
    return item;
}

unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int key_len = strlen(key);

    for (unsigned int i = 0; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    value = value % TABLE_SIZE;

    return value;
}

// this is a bit funky because the RadioStation struct already knows its id.
void set_station(RadioStation *station) {
    unsigned int slot = hash(station->id);

    TableItem *item = stations->items[slot];

    if (item == NULL) {
        stations->items[slot] = new_table_item(station);
        goto end;
    }
    
    // handle colisions
    TableItem *prev;
    
    while (item != NULL) {
        if (strcmp(item->key, station->id) == 0) {
            // replace value
            free(item->value);
            item->value = copy_station(station);
            goto end;
        }

        prev = item;
        item = item->next;
    }
    
    // add new
    item->next = new_table_item(station);

    // update keys and count
    end:
        track_keys(station->id);
    };

void track_keys(char *new_key) {
    char **keys = realloc(stations->keys, sizeof(char *) * stations->count + 1);
    keys[stations->count] = malloc(strlen(new_key) + 1);
    strcpy(keys[stations->count], new_key); 
    stations->keys = keys;
    stations->count = stations->count + 1;
}

void load_stations_from_config() {
    dictionary *stations = iniparser_load(CONFIG_PATH "/stations.ini");
    
    /*
     * In the config each section represents 1 radiostation.
     * The keys of the station corespond to the fields of the RadioStation struct.
     */
    int station_count = iniparser_getnsec(stations);
    
    for(int i = 0; i < station_count; i++) {
        const char *id = iniparser_getsecname(stations, i);
        
        log_debug("loading station: %s", id);

        // name
        char name_key[1024];
        strcpy(name_key, id);
        strcat(name_key, ":name");
        const char *name = iniparser_getstring(stations, name_key, NULL);
        
        if (name == NULL) {
            log_warn("%s is missing the name attribute", id);
            continue;
        }

        // stream
        char stream_key[1024];
        strcpy(stream_key, id);
        strcat(stream_key, ":stream");
        const char *stream_url = iniparser_getstring(stations, stream_key, NULL);
        
        if (stream_url == NULL) {
            log_warn("%s is missing the stream attribute", id);
            continue;
        }

        RadioStation station;
        station.id = id;
        station.name = name;
        station.stream_url = stream_url;


        set_station(&station);
    }

    iniparser_freedict(stations);
}

// ----------------------------------------------------------------------------
// Public Stuff
// ----------------------------------------------------------------------------
void init_stations() {
    stations = malloc(sizeof(StationTable) * 1);
    stations->items = malloc(sizeof(TableItem*) * TABLE_SIZE);
    stations->keys = malloc(sizeof(char*) * 1);
    stations->count = 0;
    
    for (int i = 0; i < TABLE_SIZE; ++i) {
        stations->items[i] = NULL;
    }
    
    load_stations_from_config();
}

RadioStation *get_station(char *id) {
    unsigned int slot = hash(id);

    TableItem *item = stations->items[slot];

    if (item == NULL) {
        return NULL;
    }
    
    // handle colisions
    while (item != NULL) {
        if (strcmp(item->key, id) == 0) {
            return item->value;
        }

        item = item->next;
    }

    return NULL;
}

RadioStation **get_stations() {
    RadioStation **station_array = malloc(sizeof(RadioStation *) * stations->count);

    for(int i = 0; i < stations->count; i++) {
        log_debug(stations->keys[i]);
        station_array[i] = get_station(stations->keys[i]);
        log_debug(station_array[i]->name);
    }

    return station_array;
}

int get_station_count() {
    return stations->count;
};


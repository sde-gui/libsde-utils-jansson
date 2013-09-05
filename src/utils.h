/**
 * Copyright (c) 2013 Vadim Ushakov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __SDE_UTILS_JANSSON__UTILS_H
#define __SDE_UTILS_JANSSON__UTILS_H

#include <glib.h>
#include <gtk/gtk.h>
#include "gdkrgba.h"
#include <jansson.h>

/******************************************************************************/

typedef struct _su_enum_pair {
    int num;
    char * str;
} su_enum_pair;

extern su_enum_pair bool_pair[];

int su_str_to_enum(const su_enum_pair *_p, const char * str, int defval);
const char * su_enum_to_str(const su_enum_pair * p, int num, const char * defval);

/******************************************************************************/

int      su_json_dot_get_enum(json_t * json, const char * key, const su_enum_pair * pairs, int default_value);
int      su_json_dot_get_int(json_t * json, const char * key, int default_value);
gboolean su_json_dot_get_bool(json_t * json, const char * key, gboolean default_value);
void     su_json_dot_get_color(json_t * json, const char * key, const GdkColor * default_value, GdkColor * result);
void     su_json_dot_get_rgba(json_t * json, const char * key, const GdkRGBA * default_value, GdkRGBA * result);
void     su_json_dot_get_string(json_t * json, const char * key, const char * default_value, char ** result);

void     su_json_dot_set_enum(json_t * json, const char * key, const su_enum_pair * pairs, int value);
void     su_json_dot_set_int(json_t * json, const char * key, int value);
void     su_json_dot_set_bool(json_t * json, const char * key, gboolean value);
void     su_json_dot_set_color(json_t * json, const char * key, const GdkColor * value);
void     su_json_dot_set_rgba(json_t * json, const char * key, const GdkRGBA * value);
void     su_json_dot_set_string(json_t * json, const char * key, const char * value);


typedef enum {
    su_json_type_enum,
    su_json_type_int,
    su_json_type_bool,
    su_json_type_color,
    su_json_type_rgba,
    su_json_type_string
} su_json_type_t;

typedef struct {
    su_json_type_t type;
    void * structure_offset;
    const char * key;
    const su_enum_pair * pairs;
} su_json_option_definition;

#define SU_JSON_OPTION(type, name) \
    { su_json_type_##type, (void *) &(((SU_JSON_OPTION_STRUCTURE *) NULL)->name), #name, NULL}
#define SU_JSON_OPTION_ENUM(pairs, name) \
    { su_json_type_enum, (void *) &(((SU_JSON_OPTION_STRUCTURE *) NULL)->name), #name, pairs}
#define SU_JSON_OPTION2(type, name, key) \
    { su_json_type_##type, (void *) &(((SU_JSON_OPTION_STRUCTURE *) NULL)->name), key, NULL}
#define SU_JSON_OPTION2_ENUM(pairs, name, key) \
    { su_json_type_enum, (void *) &(((SU_JSON_OPTION_STRUCTURE *) NULL)->name), key, pairs}

void su_json_read_options(json_t * json, su_json_option_definition * options, void * structure);
void su_json_write_options(json_t * json, su_json_option_definition * options, void * structure);

/******************************************************************************/

#ifndef json_array_foreach
    #define json_array_foreach(array, index, value) \
        for(index = 0; \
            index < json_array_size(array) && (value = json_array_get(array, index)); \
            index++)
#endif

/******************************************************************************/

#endif

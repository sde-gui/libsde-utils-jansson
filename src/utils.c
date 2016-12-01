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

#include <sde-utils-jansson/utils.h>

/********************************************************************/

static guint32 gcolor2rgb24(const GdkColor *color)
{
    guint32 i;
    guint16 r, g, b;

    r = color->red * 0xFF / 0xFFFF;
    g = color->green * 0xFF / 0xFFFF;
    b = color->blue * 0xFF / 0xFFFF;

    i = r & 0xFF;
    i <<= 8;
    i |= g & 0xFF;
    i <<= 8;
    i |= b & 0xFF;

    return i;
}


/********************************************************************/

su_enum_pair bool_pair[] = {
    { 0, "false" },
    { 1, "true" },
    { 0, "0" },
    { 1, "1" },
    { 0, NULL },
};

/********************************************************************/

int su_str_to_enum(const su_enum_pair *_p, const char * str, int defval)
{
    const su_enum_pair * p;

    for (p = _p; p && p->str; p++) {
        if (!g_ascii_strcasecmp(str, p->str))
            return p->num;
    }

    const gchar * s;
    for (s = str; *s; s++) {
        if (*s < '0' || *s > '9')
            return defval;
    }

    int num = atoi(str);

    for (p = _p; p && p->str; p++) {
        if (p->num == num)
            return p->num;
    }

    return defval;
}

const char * su_enum_to_str(const su_enum_pair * p, int num, const char * defval)
{
    for (;p && p->str; p++) {
        if (num == p->num)
            return p->str;
    }
    return defval;
}

/********************************************************************/

int su_json_dot_get_enum(json_t * json, const char * key, const su_enum_pair * pairs, int default_value)
{
    json_t * json_value = json_object_get(json, key);
    if (!json_value)
        return default_value;

    if (!json_is_string(json_value))
        return default_value;

    return su_str_to_enum(pairs, json_string_value(json_value), default_value);
}

int su_json_dot_get_int(json_t * json, const char * key, int default_value)
{
    json_t * json_value = json_object_get(json, key);
    if (!json_value)
        return default_value;

    if (json_is_integer(json_value))
        return json_integer_value(json_value);

    if (json_is_real(json_value))
        return json_real_value(json_value);

    if (json_is_string(json_value))
    {
        return atoi(json_string_value(json_value)); // FIXME: check string format
    }

    return default_value;
}

gboolean su_json_dot_get_bool(json_t * json, const char * key, gboolean default_value)
{
    json_t * json_value = json_object_get(json, key);
    if (!json_value)
        return default_value;

    if (json_is_true(json_value))
        return TRUE;

    if (json_is_false(json_value))
        return FALSE;

    return su_json_dot_get_int(json, key, default_value) ? TRUE : FALSE;
}

void su_json_dot_get_color(json_t * json, const char * key, const GdkColor * default_value, GdkColor * result)
{
    json_t * json_value = json_object_get(json, key);
    if (!json_value)
        goto def;

    if (!json_is_string(json_value))
        goto def;

    if (gdk_color_parse(json_string_value(json_value), result))
        return;

def:
    *result = *default_value;
}

void su_json_dot_get_rgba(json_t * json, const char * key, const GdkRGBA * default_value, GdkRGBA * result)
{
    json_t * json_value = json_object_get(json, key);
    if (!json_value)
        goto def;

    if (!json_is_string(json_value))
        goto def;

    if (gdk_rgba_parse(result, json_string_value(json_value)))
        return;

def:
    *result = *default_value;
}

void su_json_dot_get_string(json_t * json, const char * key, const char * default_value, char ** result)
{
    const char * value = default_value;

    json_t * json_value = json_object_get(json, key);
    if (!json_value)
        goto end;

    if (!json_is_string(json_value))
        goto end;

    value = json_string_value(json_value);

end:

    if (value != *result)
    {
        char * allocated_value = g_strdup(value);
        g_free(*result);
        *result = allocated_value;
    }
}

void su_json_dot_set_enum(json_t * json, const char * key, const su_enum_pair * pairs, int value)
{
    json_object_set_new_nocheck(json, key, json_string(su_enum_to_str(pairs, value, "")));
}

void su_json_dot_set_int(json_t * json, const char * key, int value)
{
    json_object_set_new_nocheck(json, key, json_integer(value));
}

void su_json_dot_set_bool(json_t * json, const char * key, gboolean value)
{
    json_object_set_new_nocheck(json, key, json_boolean(value));
}

void su_json_dot_set_color(json_t * json, const char * key, const GdkColor * value)
{
    char s[256];
    sprintf(s, "#%06x", gcolor2rgb24(value));

    su_json_dot_set_string(json, key, s);
}

void su_json_dot_set_rgba(json_t * json, const char * key, const GdkRGBA * value)
{
    gchar * s = gdk_rgba_to_string(value);
    su_json_dot_set_string(json, key, s);
    g_free(s);
}

void su_json_dot_set_string(json_t * json, const char * key, const char * value)
{
    json_object_set_new_nocheck(json, key, json_string(value ? value : ""));
}

void su_json_read_options(json_t * json, su_json_option_definition * options, void * structure)
{
    for (; options->key; options++)
    {
        void * p = (void *) ((char *) structure + (unsigned) options->structure_offset);
        switch (options->type)
        {
            case su_json_type_enum:
                *((int *) p) = su_json_dot_get_enum(json, options->key, options->pairs, *(int *) p);
                break;
            case su_json_type_int:
                *((int *) p) = su_json_dot_get_int(json, options->key, *(int *) p);
                break;
            case su_json_type_bool:
                *((gboolean *) p) = su_json_dot_get_bool(json, options->key, *(gboolean *) p);
                break;
            case su_json_type_color:
                su_json_dot_get_color(json, options->key, (GdkColor *) p, (GdkColor *) p);
                break;
            case su_json_type_rgba:
                su_json_dot_get_rgba(json, options->key, (GdkRGBA *) p, (GdkRGBA *) p);
                break;
            case su_json_type_string:
                su_json_dot_get_string(json, options->key, *(char **) p, (char **) p);
                break;
            default:
                g_warning("su_json_read_options: invalid option type %d", options->type);
        }
    }
}

void su_json_write_options(json_t * json, su_json_option_definition * options, void * structure)
{
    for (; options->key; options++)
    {
        void * p = (void *) ((char *) structure + (unsigned) options->structure_offset);
        switch (options->type)
        {
            case su_json_type_enum:
                su_json_dot_set_enum(json, options->key, options->pairs, *(int *) p);
                break;
            case su_json_type_int:
                su_json_dot_set_int(json, options->key, *(int *) p);
                break;
            case su_json_type_bool:
                su_json_dot_set_bool(json, options->key, *(gboolean *) p);
                break;
            case su_json_type_color:
                su_json_dot_set_color(json, options->key, (GdkColor *) p);
                break;
            case su_json_type_rgba:
                su_json_dot_set_rgba(json, options->key, (GdkRGBA *) p);
                break;
            case su_json_type_string:
                su_json_dot_set_string(json, options->key, *(char **) p);
                break;
            default:
                g_warning("su_json_write_options: invalid option type %d", options->type);
        }
    }
}


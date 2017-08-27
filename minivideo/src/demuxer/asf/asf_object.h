/*!
 * COPYRIGHT (C) 2017 Emeric Grange - All Rights Reserved
 *
 * This file is part of MiniVideo.
 *
 * MiniVideo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MiniVideo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with MiniVideo.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \file      asf_object.h
 * \author    Emeric Grange <emeric.grange@gmail.com>
 * \date      2017
 */

#ifndef PARSER_ASF_OBJECT_H
#define PARSER_ASF_OBJECT_H

// minivideo headers
#include "asf_struct.h"
#include "../../bitstream.h"

/* ************************************************************************** */

int parse_asf_object(Bitstream_t *bitstr, AsfObject_t *asf_object);

void print_asf_object(AsfObject_t *asf_object);

void write_asf_object(AsfObject_t *asf_object, FILE *xml, const char *title);

/* ************************************************************************** */

int parse_unknown_object(Bitstream_t *bitstr, AsfObject_t *asf_object, FILE *xml);

/* ************************************************************************** */

int jumpy_asf(Bitstream_t *bitstr, AsfObject_t *parent, AsfObject_t *current);

/* ************************************************************************** */
#endif // PARSER_ASF_OBJECT_H
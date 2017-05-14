/*!
 * COPYRIGHT (C) 2016 Emeric Grange - All Rights Reserved
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
 * \file      wave_convert.h
 * \author    Emeric Grange <emeric.grange@gmail.com>
 * \date      2016
 */

#ifndef PARSER_WAVE_CONVERT_H
#define PARSER_WAVE_CONVERT_H

// minivideo headers
#include "wave_struct.h"
#include "../../bitstream.h"
#include "../../minivideo_mediafile.h"

/* ************************************************************************** */

int wave_indexer(MediaFile_t *media, wave_t *wave);

int wave_indexer_initmap(MediaFile_t *media, wave_t *wave);

/* ************************************************************************** */

int wave_clean(wave_t *wave);

/* ************************************************************************** */
#endif // PARSER_WAVE_CONVERT_H

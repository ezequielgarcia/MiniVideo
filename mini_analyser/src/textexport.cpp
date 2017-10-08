/*!
 * COPYRIGHT (C) 2017 Emeric Grange - All Rights Reserved
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
 *
 * \file      textexport.cpp
 * \author    Emeric Grange <emeric.grange@gmail.com>
 * \date      2017
 */

#include "textexport.h"

// minivideo library
#include <minivideo.h>

// minianalyser
#include "utils.h"

#include <QMessageBox>
#include <QFontDatabase>
#include <QFileDialog>
#include <QDateTime>
#include <QFile>
#include <QDebug>

/* ************************************************************************** */

textExport::textExport()
{
    //
}

textExport::~textExport()
{
    //
}

/* ************************************************************************** */

int textExport::generateExportDatas_text(MediaFile_t &media, QString &exportDatas, bool detailed)
{
    int retcode = 0;

    exportDatas += "Full path     : ";
    exportDatas += media.file_path;

    exportDatas += "\n\nTitle         : ";
    exportDatas += media.file_name;
    exportDatas += "\nDuration      : ";
    exportDatas += getDurationString(media.duration);
    exportDatas += "\nSize          : ";
    exportDatas += getSizeString(media.file_size);
    exportDatas += "\nContainer     : ";
    exportDatas += getContainerString(media.container, true);
    if (media.creation_app)
    {
        exportDatas += "\nCreation app  : ";
        exportDatas += media.creation_app;
    }
    if (media.creation_lib)
    {
        exportDatas += "\nCreation lib  : ";
        exportDatas += media.creation_lib;
    }
    if (media.creation_time)
    {
        QDate date(1904, 1, 1);
        QTime time(0, 0, 0, 0);
        QDateTime datetime(date, time);
        datetime = datetime.addSecs(media.creation_time);
        exportDatas += "\nCreation time : ";
        exportDatas += datetime.toString("dddd d MMMM yyyy, hh:mm:ss");
    }

    // VIDEO TRACKS
    ////////////////////////////////////////////////////////////////////////

    for (unsigned i = 0; i < media.tracks_video_count; i++)
    {
        MediaStream_t *t = media.tracks_video[i];
        if (t == nullptr)
            break;

        // Section title
        if (media.tracks_video_count == 1)
        {
            exportDatas += "\n\nVIDEO\n-----";
        }
        else
        {
            exportDatas += "\nVIDEO TRACK #";
            exportDatas += QString::number(i);
            exportDatas += "\n--------------";
        }

        // Datas
        if (detailed == true)
        {
            exportDatas += "\nTrack ID      : ";
            exportDatas += QString::number(t->track_id);

            if (t->stream_fcc)
            {
                exportDatas += "\nFourCC        : ";
                exportDatas += getFourccString(t->stream_fcc);
            }
        }
        exportDatas += "\nCodec         : ";
        exportDatas += getCodecString(stream_VIDEO, t->stream_codec, true);
        exportDatas += "\nSize          : ";
        exportDatas += getTrackSizeString(t, media.file_size, detailed);
        exportDatas += "\nDuration      : ";
        exportDatas += getDurationString(t->stream_duration_ms);
        exportDatas += "\nWidth         : ";
        exportDatas += QString::number(t->width);
        exportDatas += "\nHeight        : ";
        exportDatas += QString::number(t->height);

        if (detailed == true)
        {
            if (t->pixel_aspect_ratio_h || t->pixel_aspect_ratio_v)
            {
                exportDatas += "\nPixel Aspect ratio   : ";
                exportDatas += QString::number(t->pixel_aspect_ratio_h) + ":" + QString::number(t->pixel_aspect_ratio_v);
            }
            if (t->video_aspect_ratio > 0.0)
            {
                exportDatas += "\nVideo Aspect ratio   : ";
                exportDatas += getAspectRatioString(t->video_aspect_ratio, false);
            }
            exportDatas += "\nDisplay Aspect ratio : ";
            exportDatas += getAspectRatioString(t->display_aspect_ratio, true);
        }
        else
        {
            exportDatas += "\nAspect ratio  : ";
            exportDatas += getAspectRatioString(t->display_aspect_ratio, detailed);
        }

        if (detailed == true)
        {
            exportDatas += "\nFramerate     : ";
            exportDatas += QString::number(t->framerate) + " fps";
            exportDatas += "\nFramerate mode: ";
            exportDatas += getFramerateModeString(t->framerate_mode);

            exportDatas += "\nBitrate       : ";
            exportDatas += getBitrateString(t->bitrate_avg);
            exportDatas += "\nBitrate mode  : ";
            exportDatas += getBitrateModeString(t->bitrate_mode);
            if (t->bitrate_mode != BITRATE_CBR)
            {
                exportDatas += "\nBitrate (min) : ";
                exportDatas += getBitrateString(t->bitrate_min);
                exportDatas += "\nBitrate (max) : ";
                exportDatas += getBitrateString(t->bitrate_max);
            }
        }
        else
        {
            exportDatas += "\nFramerate     : ";
            exportDatas += QString::number(t->framerate) + " fps (";
            exportDatas += getFramerateModeString(t->framerate_mode) + ")";

            exportDatas += "\nBitrate       : ";
            exportDatas += getBitrateString(t->bitrate_avg);
            exportDatas += " (" + getBitrateModeString(t->bitrate_mode) + ")";
        }

        exportDatas += "\nColor depth   : ";
        exportDatas += QString::number(t->color_depth) + " bits";
/*
        exportDatas += "\nColor matrix  : ";
        if (t->color_range == 0)
            exportDatas += "\nColor range   : Limited";
        else
            exportDatas += "\nColor range   : Full";
*/
    }

    // AUDIO TRACKS
    ////////////////////////////////////////////////////////////////////////

    for (unsigned i = 0; i < media.tracks_audio_count; i++)
    {
        MediaStream_t *t = media.tracks_audio[i];
        if (t == nullptr)
            break;

        // Section title
        exportDatas += "\n\nAUDIO TRACK #";
        exportDatas += QString::number(i);
        exportDatas += "\n--------------";

        // Datas
        if (detailed == true)
        {
            exportDatas += "\nTrack ID      : ";
            exportDatas += QString::number(t->track_id);

            if (t->stream_fcc)
            {
                exportDatas += "\nFourCC        : ";
                exportDatas += getFourccString(t->stream_fcc);
            }
        }
        exportDatas += "\nCodec         : ";
        exportDatas += getCodecString(stream_AUDIO, t->stream_codec, true);
        exportDatas += "\nSize          : ";
        exportDatas += getTrackSizeString(t, media.file_size, detailed);
        exportDatas += "\nDuration      : ";
        exportDatas += getDurationString(t->stream_duration_ms);
        if (t->track_title)
        {
            exportDatas += "\nTitle         : ";
            exportDatas += QString::fromLocal8Bit(t->track_title);
        }
        if (t->track_languagecode)
        {
            exportDatas += "\nLanguage      : ";
            exportDatas += QString::fromLocal8Bit(t->track_languagecode);
        }
        exportDatas += "\nChannels      : ";
        exportDatas += QString::number(t->channel_count);
        exportDatas += "\nBit per sample: ";
        exportDatas += QString::number(t->bit_per_sample);
        exportDatas += "\nSamplerate    : ";
        exportDatas += QString::number(t->sampling_rate) + " Hz";
        if (detailed == true)
        {
            exportDatas += "\nBitrate       : ";
            exportDatas += getBitrateString(t->bitrate_avg);
            exportDatas += "\nBitrate mode  : ";
            exportDatas += getBitrateModeString(t->bitrate_mode);
            if (t->bitrate_mode != BITRATE_CBR)
            {
                exportDatas += "\nBitrate (min) : ";
                exportDatas += getBitrateString(t->bitrate_min);
                exportDatas += "\nBitrate (max) : ";
                exportDatas += getBitrateString(t->bitrate_max);
            }
        }
        else
        {
            exportDatas += "\nBitrate       : ";
            exportDatas += getBitrateString(t->bitrate_avg);
            exportDatas += " (" + getBitrateModeString(t->bitrate_mode) + ")";
        }
    }

    // SUBTITLES TRACKS
    ////////////////////////////////////////////////////////////////////////

    for (unsigned i = 0; i < media.tracks_subtitles_count; i++)
    {
        MediaStream_t *t = media.tracks_subt[i];
        if (t == nullptr)
            break;

        // Section title
        exportDatas += "\n\nSUBTITLES TRACK #";
        exportDatas += QString::number(i);
        exportDatas += "\n------------------";

        // Datas
        exportDatas += "\nFormat        : sub";
        exportDatas += "\nSize          : ";
        exportDatas += getTrackSizeString(t, media.file_size, detailed);
        exportDatas += "\nTitle         : ";
        exportDatas += QString::fromLocal8Bit(t->track_title);
        exportDatas += "\nLanguage      : ";
        exportDatas += QString::fromLocal8Bit(t->track_languagecode);
    }

    // OTHER TRACKS
    ////////////////////////////////////////////////////////////////////////

    for (unsigned i = 0; i < media.tracks_others_count; i++)
    {
        MediaStream_t *t = media.tracks_others[i];
        if (t == nullptr)
            break;

        // Section title
        if (t->stream_type == stream_TEXT)
            exportDatas += "\n\nTEXT TRACK #";
        else if (t->stream_type == stream_MENU)
            exportDatas += "\n\nMENU TRACK #";
        else if (t->stream_type == stream_TMCD)
            exportDatas += "\n\nTMCD TRACK #";
        else if (t->stream_type == stream_META)
            exportDatas += "\n\nMETA TRACK #";
        else if (t->stream_type == stream_HINT)
            exportDatas += "\n\nHINT TRACK #";

        exportDatas += QString::number(i);
        exportDatas += "\n-------------";

        // Datas
        exportDatas += "\nSize          : ";
        exportDatas += getTrackSizeString(t, media.file_size, detailed);
        exportDatas += "\nTitle         : ";
        exportDatas += QString::fromLocal8Bit(t->track_title);
        exportDatas += "\nLanguage      : ";
        exportDatas += QString::fromLocal8Bit(t->track_languagecode);
    }

    retcode = 1;

    return retcode;
}

int textExport::generateExportDatas_json(MediaFile_t &media, QString &exportDatas, bool detailed)
{
    int retcode = 0;

    Q_UNUSED(media);
    Q_UNUSED(exportDatas);
    Q_UNUSED(detailed);

    // TODO

    return retcode;
}

int textExport::generateExportDatas_xml(MediaFile_t &media, QString &exportDatas, bool detailed)
{
    int retcode = 0;

    Q_UNUSED(media);
    Q_UNUSED(exportDatas);
    Q_UNUSED(detailed);

    // TODO

    return retcode;
}

int textExport::generateExportMapping_xml(MediaFile_t &media, QString &exportDatas)
{
    int status = 1;

    QFile xmlMapFile;
    QString filename;

    // Load XML file (from given file descriptor)
    if (media.container_mapper_fd == nullptr ||
        xmlMapFile.open(media.container_mapper_fd, QIODevice::ReadOnly) == false)
    {
        status = 0;
        qDebug() << "xmlFile.open(FILE*) > error";
    }

    // Load XML file (fallback from file path / DEPRECATED)
    if (status == 0)
    {
        filename = "/tmp/" + QString::fromLocal8Bit(media.file_name) + "_mapped.xml";
        xmlMapFile.setFileName(filename);

        if (xmlMapFile.exists() == false ||
            xmlMapFile.open(QIODevice::ReadOnly) == false)
        {
            qDebug() << "xmlFile.open(" << filename << ") > error";
            status = 0;
        }
    }

    if (status == true)
    {
        xmlMapFile.seek(0);
        exportDatas = xmlMapFile.readAll();
    }

    return status;
}
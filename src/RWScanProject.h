/**
 ** This file is part of the ScanRect project.
 ** Copyright 2016 Rainer Wiesenfarth <rainer@rainer-wiesenfarth.de>.
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **
 ** 1. Redistributions of source code must retain the above copyright notice,
 **    this list of conditions and the following disclaimer.
 **
 ** 2. Redistributions in binary form must reproduce the above copyright
 **    notice, this list of conditions and the following disclaimer in the
 **    documentation and/or other materials provided with the distribution.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 ** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 ** DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 ** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 ** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 ** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 ** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 ** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 ** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 ** SUCH DAMAGE.
 **/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QDataStream>
#include <QtCore/QString>

#include "RWScanImageModel.h"


class RWScanProject : public QObject
{
    Q_OBJECT

  public:
    RWScanProject( const QString &filename = QString() );

    QString filename() const;
    QString titlename() const;
    bool valid() const;
    bool modified() const;
    bool canSave() const;

    bool load();
    bool save();
    bool saveAs( const QString &filename );

    const RWScanImageModel &imageModel() const;
    RWScanImageModel &imageModel();

  public slots:
    void imageModelModified();

  signals:
    void modifiedChanged( bool modified );
    void filenameChanged( const QString &filename );

  private:
    RWScanProject( const RWScanProject & ) = delete;
    RWScanProject &operator=( const RWScanProject & ) = delete;

    friend QDataStream& operator<<( QDataStream &stream, const RWScanProject &project );
    friend QDataStream& operator>>( QDataStream &stream, RWScanProject &project );

    void setModified( bool modified );

    QString m_filename;
    bool    m_valid;
    bool    m_modified;

    RWScanImageModel m_imageModel;

    static constexpr quint64 s_magic          = 0xa7fe3901b5224098;
    static constexpr quint32 s_version_001    = 1;
    static constexpr quint32 s_currentVersion = s_version_001;
};


QDataStream& operator<<( QDataStream &stream, const RWScanProject &project );
QDataStream& operator>>( QDataStream &stream, RWScanProject &project );

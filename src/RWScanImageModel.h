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

#include "RWScanImageList.h"

#include <QtCore/QAbstractListModel>
#include <QtCore/QDataStream>
#include <QtGui/QBrush>
#include <QtGui/QFont>

#include <memory>


class RWScanImageModel : public QAbstractListModel
{
  public:
    RWScanImageModel( QObject *parent = 0 );

    std::weak_ptr<RWScanImageEntry> itemAt( const QModelIndex &index ) const;

    void addImages( const QStringList &imageList );

    // from QAbstractListModel
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

  private:
    RWScanImageList m_imageList;

    friend QDataStream& operator<<( QDataStream &stream, const RWScanImageModel &model );
    friend QDataStream& operator>>( QDataStream &stream, RWScanImageModel &model );

    QBrush m_nullImageBrush;
    QFont  m_nullImageFont;
};


QDataStream& operator<<( QDataStream &stream, const RWScanImageModel &model );
QDataStream& operator>>( QDataStream &stream, RWScanImageModel &model );

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

#include "RWScanImageModel.h"

#include "RWScanImageList.h"


RWScanImageModel::RWScanImageModel( QObject *parent )
  : QAbstractListModel( parent )
  , m_pImageList( new RWScanImageList )
  , m_nullImageBrush( QColor( 128, 128, 128 ) )
  , m_nullImageFont()
{
  m_nullImageFont.setItalic( true );
}


//=================================================================================================================
const RWScanImageEntry &RWScanImageModel::entry(const QModelIndex &index) const
{
  //! \todo Add error handling (return ref to invalid object)
  return m_pImageList->at( index.row() );
}


//=================================================================================================================
void RWScanImageModel::addImages(const QStringList &imageList)
{
  beginInsertRows( QModelIndex(), m_pImageList->count(), m_pImageList->count() + imageList.count() + 1 );

  for( int i = 0; i < imageList.count(); i++ )
  {
    m_pImageList->append( RWScanImageEntry( imageList[i] ) );
  }

  endInsertRows();
}


//=================================================================================================================
int RWScanImageModel::rowCount( const QModelIndex & /*parent*/ ) const
{
  return m_pImageList->count();
}


//=================================================================================================================
QVariant RWScanImageModel::data( const QModelIndex &index, int role ) const
{
  if( !index.isValid() || ( index.column() != 0 ) )
  {
    return QVariant();
  }

  auto image = m_pImageList->at( index.row() );

  if( role == Qt::DisplayRole )
  {
    return image.name();
  }
  else if( role == Qt::ToolTipRole )
  {
    return image.filename();
  }
  else if( role == Qt::ForegroundRole )
  {
    return image.image().isNull() ? m_nullImageBrush : QVariant();
  }
  else if( role == Qt::FontRole )
  {
    return image.image().isNull() ? m_nullImageFont : QVariant();
  }

  return QVariant();
}


//=================================================================================================================
QVariant RWScanImageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if( ( section != 0 ) || ( orientation != Qt::Horizontal ) || ( role != Qt::DisplayRole ) )
  {
    return QVariant();
  }

  return "Image Name";
}


//=================================================================================================================
QDataStream& operator<<( QDataStream &stream, const RWScanImageModel &model )
{
  stream << *model.m_pImageList;

  return stream;
}


//=================================================================================================================
QDataStream& operator>>( QDataStream &stream, RWScanImageModel &model )
{
  if( !model.m_pImageList->isEmpty() )
  {
    model.beginRemoveRows( QModelIndex(), 0, model.m_pImageList->count() - 1 );
    model.m_pImageList->clear();
    model.endRemoveRows();
  }

  stream >> *model.m_pImageList;

  if( !model.m_pImageList->isEmpty() )
  {
    model.beginInsertRows( QModelIndex(), 0, model.m_pImageList->count() - 1 );
    model.endInsertRows();
  }

  return stream;
}

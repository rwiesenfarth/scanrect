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

#include "RWScanProject.h"

#include <QtCore/QFile>


//=================================================================================================================
RWScanProject::RWScanProject( const QString &filename )
  : m_filename( filename )
  , m_valid( true )
  , m_modified( false )
  , m_imageModel()
{
  connect( &m_imageModel, &QAbstractItemModel::dataChanged, this, &RWScanProject::imageModelModified );
  connect( &m_imageModel, &QAbstractItemModel::rowsInserted, this, &RWScanProject::imageModelModified );
  connect( &m_imageModel, &QAbstractItemModel::rowsMoved, this, &RWScanProject::imageModelModified );
  connect( &m_imageModel, &QAbstractItemModel::rowsRemoved, this, &RWScanProject::imageModelModified );

  if( !m_filename.isEmpty() )
  {
    m_valid = load();
  }
}


//=================================================================================================================
QString RWScanProject::filename() const
{
    return m_filename;
}


//=================================================================================================================
QString RWScanProject::titlename() const
{
  return m_filename.isEmpty() ? "(unbenannt)" : m_filename;
}


//=================================================================================================================
bool RWScanProject::valid() const
{
  return m_valid;
}


//=================================================================================================================
bool RWScanProject::modified() const
{
  return m_modified;
}


//=================================================================================================================
bool RWScanProject::canSave() const
{
  return m_valid && !m_filename.isEmpty();
}


//=================================================================================================================
bool RWScanProject::load()
{
  bool  result = false;
  QFile f( m_filename );

  if( f.open( QIODevice::ReadOnly ) )
  {
    QDataStream s( &f );
    s >> *this;
    result = valid();
  }

  return result;
}


//=================================================================================================================
bool RWScanProject::save()
{
  bool  result = false;
  QFile f( m_filename );

  if( !m_filename.isEmpty() && valid() && f.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
  {
    QDataStream s( &f );
    s << *this;
    result = true;
    setModified( false );
  }

  return result;
}


//=================================================================================================================
bool RWScanProject::saveAs(const QString &filename)
{
  m_filename = filename;
  emit filenameChanged( m_filename );
  return save();
}


//=================================================================================================================
const RWScanImageModel &RWScanProject::imageModel() const
{
  return m_imageModel;
}


//=================================================================================================================
RWScanImageModel &RWScanProject::imageModel()
{
  return m_imageModel;
}


//=================================================================================================================
void RWScanProject::imageModelModified()
{
  setModified( true );
}


//=================================================================================================================
void RWScanProject::setModified( bool modified )
{
  if( m_modified != modified )
  {
    m_modified = modified;
    emit modifiedChanged( m_modified );
  }
}


//=================================================================================================================
QDataStream& operator<<( QDataStream &stream, const RWScanProject &project )
{
  stream << RWScanProject::s_magic;
  stream << RWScanProject::s_currentVersion;
  stream << project.m_imageModel;

  return stream;
}


//=================================================================================================================
QDataStream& operator>>( QDataStream &stream, RWScanProject &project )
{
  quint64 magic;
  quint32 version;

  project.m_valid = false;

  stream >> magic;
  if( magic != RWScanProject::s_magic )
  {
    return stream;
  }

  stream >> version;

  //! \todo Add handling of versioned read for project and the contained elements
  if( version == RWScanProject::s_version_001 )
  {
    project.m_valid =true;
  }

  stream >> project.m_imageModel;

  return stream;
}

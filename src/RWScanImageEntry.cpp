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

#include "RWScanImageEntry.h"

#include <QtCore/QFileInfo>


int RWScanImageEntry::s_defaultRotation_deg = 90;
int RWScanImageEntry::s_defaultWidth_mm     = 210;
int RWScanImageEntry::s_defaultHeight_mm    = 297;


//=================================================================================================================
RWScanImageEntry::RWScanImageEntry()
  : m_filename()
  , m_image()
  , m_initialRotation_deg( defaultRotation_deg() )
  , m_initialWidth_mm( defaultWidth_mm() )
  , m_initialHeight_mm( defaultHeight_mm() )
{ }


//=================================================================================================================
RWScanImageEntry::RWScanImageEntry(const QString &filename)
  : m_filename( filename )
  , m_image()
  , m_initialRotation_deg( defaultRotation_deg() )
  , m_initialWidth_mm( defaultWidth_mm() )
  , m_initialHeight_mm( defaultHeight_mm() )
{
  initImage();
}


//=================================================================================================================
QString RWScanImageEntry::filename() const
{
  return m_filename;
}


//=================================================================================================================
QString RWScanImageEntry::name() const
{
  return QFileInfo( m_filename ).completeBaseName();
}


//=================================================================================================================
const QImage &RWScanImageEntry::image() const
{
  return m_image;
}


//=================================================================================================================
int RWScanImageEntry::initialRotation_deg() const
{
  return m_initialRotation_deg;
}


//=================================================================================================================
int RWScanImageEntry::initialWidth_mm() const
{
  return m_initialWidth_mm;
}


//=================================================================================================================
int RWScanImageEntry::initialHeight_mm() const
{
  return m_initialHeight_mm;
}


//=================================================================================================================
void RWScanImageEntry::initImage()
{
  //! \todo Asynchronous load of image
  m_image = QImage( m_filename );
}


//=================================================================================================================
int RWScanImageEntry::defaultRotation_deg()
{
  return s_defaultRotation_deg;
}


//=================================================================================================================
int RWScanImageEntry::defaultWidth_mm()
{
  return s_defaultWidth_mm;
}


//=================================================================================================================
int RWScanImageEntry::defaultHeight_mm()
{
  return s_defaultHeight_mm;
}


//=================================================================================================================
void RWScanImageEntry::setDefaultRotation_deg(int value)
{
  s_defaultRotation_deg = value;
}


//=================================================================================================================
void RWScanImageEntry::setDefaultWidth_mm(int value)
{
  s_defaultWidth_mm = value;
}


//=================================================================================================================
void RWScanImageEntry::setDefaultHeight_mm(int value)
{
  s_defaultHeight_mm = value;
}


//=================================================================================================================
QDataStream& operator<<( QDataStream &stream, const RWScanImageEntry &entry )
{
  stream << entry.m_filename;
  stream << entry.m_initialRotation_deg;
  stream << entry.m_initialWidth_mm;
  stream << entry.m_initialHeight_mm;

  return stream;
}


//=================================================================================================================
QDataStream& operator>>( QDataStream &stream, RWScanImageEntry &entry )
{
  stream >> entry.m_filename;
  stream >> entry.m_initialRotation_deg;
  stream >> entry.m_initialWidth_mm;
  stream >> entry.m_initialHeight_mm;

  entry.initImage();
  return stream;
}

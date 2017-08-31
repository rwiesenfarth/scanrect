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

#include "RWScanDocumentEntry.h"
#include "RWScanImageList.h"


//=================================================================================================================
RWScanDocumentEntry::RWScanDocumentEntry( const QString &documentName )
  : m_name( documentName )
  , m_images()
{ }


//=================================================================================================================
RWScanDocumentEntry::RWScanDocumentEntry( QDataStream &stream,
  const RWScanImageList &imageList, int version )
  : m_name()
  , m_images()
{
  size_t  count = 0;
  bool    imagePresent;
  QString imageName;

  //! \todo Check magic number to avoid bad reads
  //! \todo Add versioned read
  stream >> m_name;
  stream >> count;
  for( size_t i = 0; i < count; i++ )
  {
    stream >> imagePresent;
    if( imagePresent )
    {
      stream >> imageName;
      m_images.push_back( imageList.findEntry( imageName ) );
    }
  }
}


//=================================================================================================================
void RWScanDocumentEntry::save( QDataStream &stream ) const
{
  //! \todo Add magic number to avoid bad reads
  stream << m_name;
  stream << m_images.size();
  for( const auto &image : m_images )
  {
    auto locked = image.lock();

    if( locked )
    {
      stream << true;
      stream << locked->name();
    }
    else
    {
      stream << false;
    }
  }
}

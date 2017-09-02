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

#include "RWScanImageList.h"


//=================================================================================================================
RWScanImageList::RWScanImageList()
  : m_valid( true )
{ }


//=================================================================================================================
RWScanImageList::RWScanImageList(QDataStream &stream, quint32 version)
{
  clear();
  m_valid = false;

  size_t count = 0;

  if( version == 1 )
  {
    // version 1 used int
    int intCount;

    stream >> intCount;
    count = (size_t) intCount;
  }
  else
  {
    // version 2 adds magic numbers and uses size_t
    quint32 magic;

    stream >> magic;
    if( magic != s_magic )
    {
      return;
    }
    stream >> count;
  }

  for( size_t i = 0; i < count; i++ )
  {
    auto entry = new RWScanImageEntry( stream, version );
    if( entry->valid() )
    {
      push_back( std::shared_ptr<RWScanImageEntry>( entry ) );
    }
  }
  m_valid = true;
}


//=================================================================================================================
bool RWScanImageList::valid() const
{
  return m_valid;
}


//=================================================================================================================
void RWScanImageList::save(QDataStream &stream)
{
  stream << s_magic;
  stream << list.size();
  for( const auto &entry : list )
  {
    if( entry->valid() )
    {
      entry->save( stream );
    }
  }
}


//=================================================================================================================
std::weak_ptr<RWScanImageEntry> RWScanImageList::findEntry(const QString &name) const
{
  auto it = std::find_if( begin(), end(), [name]( const std::shared_ptr<RWScanImageEntry> &elem ) -> bool {
    return ( elem && ( elem->name() == name ) );
  } );

  return ( it == end() ) ? std::weak_ptr<RWScanImageEntry>() : *it;
}

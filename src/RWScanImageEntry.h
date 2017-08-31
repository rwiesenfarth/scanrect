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

#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtGui/QImage>


class RWScanImageEntry
{
  public:
    RWScanImageEntry();
    RWScanImageEntry( const QString &filename );
    RWScanImageEntry( const RWScanImageEntry & ) = delete;

    RWScanImageEntry &operator=( const RWScanImageEntry & ) = delete;

    //! \brief The (complete) filename of the image
    QString filename() const;
    //! \brief The base name of the images to be displayed in lists
    QString name() const;

    const QImage &image() const;
    /*! \brief The rotation of the raw image
     *
     *  The raw image has to be rotated clockwise by this value to be displayed correctly
     */
    int initialRotation_deg() const;
    //! \brief The width of the scanned paper
    int initialWidth_mm() const;
    //! \brief The height of the scanned paper
    int initialHeight_mm() const;

    //! \brief The initial rotation assumed for newly added images
    static int defaultRotation_deg();
    //! \brief The initial width assumed for newly added images
    static int defaultWidth_mm();
    //! \brief The initial height assumed for newly added images
    static int defaultHeight_mm();

    //! \brief Set the rotation to be used as initial value for new images
    static void setDefaultRotation_deg(int value);
    //! \brief Set the width to be used as initial value for new images
    static void setDefaultWidth_mm(int value);
    //! \brief Set the height to be used as initial value for new images
    static void setDefaultHeight_mm(int value);

  private:
    //! \brief Initialize a freshly created image
    void initImage();

    friend QDataStream& operator<<( QDataStream &stream, const RWScanImageEntry &entry );
    friend QDataStream& operator>>( QDataStream &stream, RWScanImageEntry &entry );

    QString m_filename;
    QImage  m_image;

    int     m_initialRotation_deg; //!< 0, 90, 180, 270
    int     m_initialWidth_mm;     //!< "paper width" in mm, e.g. 210 for A4
    int     m_initialHeight_mm;    //!< "paper height" in mm, e.g. 297 for A4

    static int s_defaultRotation_deg;
    static int s_defaultWidth_mm;
    static int s_defaultHeight_mm;
};


QDataStream& operator<<( QDataStream &stream, const RWScanImageEntry &entry );
QDataStream& operator>>( QDataStream &stream, RWScanImageEntry &entry );

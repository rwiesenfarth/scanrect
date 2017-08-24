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

#include "RWScanRect.h"
#include "ui_RWScanRect.h"

#include "RWScanProject.h"
#include "RWScanImageEntry.h"

#include <QtWidgets/QFileDialog>
#include <QtGui/QGuiApplication>
#include <QtGui/QImage>


//=================================================================================================================
RWScanRect::RWScanRect( QWidget *parent )
  : QMainWindow( parent )
  , ui( new Ui::RWScanRect )
  , m_pProject( new RWScanProject )
  , m_currentRawImageScale( 0.1 )
{
  ui->setupUi( this );

  setWindowTitle( "" );
  QGuiApplication::setApplicationDisplayName( "ScanRect" );

  ui->rawImageScaleCB->addItem( "100%", 1.0 );
  ui->rawImageScaleCB->addItem( "70%", 0.7 );
  ui->rawImageScaleCB->addItem( "50%", 0.5 );
  ui->rawImageScaleCB->addItem( "35%", 0.35 );
  ui->rawImageScaleCB->addItem( "25%", 0.25 );
  ui->rawImageScaleCB->addItem( "17%", 0.17 );
  ui->rawImageScaleCB->addItem( "12%", 0.12 );
  ui->rawImageScaleCB->addItem( "9%", 0.09 );
  ui->rawImageScaleCB->addItem( "6%", 0.06 );

  connect( ui->projectNewACT, &QAction::triggered, this, &RWScanRect::newProject );
  connect( ui->projectOpenACT, &QAction::triggered, this, &RWScanRect::openProject );
  connect( ui->projectSaveACT, &QAction::triggered, this, &RWScanRect::saveProject );
  connect( ui->projectSaveAsACT, &QAction::triggered, this, &RWScanRect::saveAsProject );

  connect( ui->imageAddPB, &QPushButton::clicked, this, &RWScanRect::addImages );

  connect( ui->imageLV, &QListView::clicked, this, &RWScanRect::updateImage );

  connect( ui->rawImageScaleCB, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this, &RWScanRect::updateRawImageScale );

  ui->rawImageScaleCB->setCurrentIndex( 0 );
  updateRawImageScale( 0 );

  projectChanged();
}


//=================================================================================================================
RWScanRect::~RWScanRect()
{
  delete ui;
}


//=================================================================================================================
void RWScanRect::newProject()
{
  // Save current project if necessary and wanted
  if( !closeProject() )
  {
    return;
  }

  // Replace the current project with a new one
  m_pProject.reset( new RWScanProject );
  projectChanged();
}


//=================================================================================================================
void RWScanRect::openProject()
{
  // Save current project if necessary and wanted
  if( !closeProject() )
  {
    return;
  }

  // Get the filename
  QString filename = QFileDialog::getOpenFileName( this, "Projekt öffnen", QString(), "ScanRect Projekte (*.srp)" );
  if( filename.isEmpty() )
  {
    return;
  }

  // Create the new project
  auto pNewProject = new RWScanProject( filename );

  // If loaded successfully, replace the current project
  if( pNewProject->valid() )
  {
    m_pProject.reset( pNewProject );
    projectChanged();
  }
  else
  {
    delete pNewProject;
  }
}


//=================================================================================================================
void RWScanRect::saveProject()
{
  if( m_pProject->canSave() )
  {
    m_pProject->save();
  }
  else
  {
    saveAsProject();
  }
}


//=================================================================================================================
void RWScanRect::saveAsProject()
{
  QString filename = QFileDialog::getSaveFileName( this, "Projekt speichern", QString(),
    "ScanRect Projekte (*.srp)" );

  if( !filename.isEmpty() )
  {
    m_pProject->saveAs( filename );
  }
}


//=================================================================================================================
void RWScanRect::addImages()
{
  QStringList images = QFileDialog::getOpenFileNames( this, "Bilder hinzufügen...", QString(),
    "JPEG-Bilder (*.jpg);;PNG Bilder (*.png);;Alle Dateien (*.*)" );

  if( !images.isEmpty() )
  {
    m_pProject->imageModel().addImages( images );
  }
}


//=================================================================================================================
void RWScanRect::updateTitle()
{
  setWindowFilePath( m_pProject->titlename() );
}


//=================================================================================================================
void RWScanRect::updateImage(const QModelIndex &index)
{
  if( !index.isValid() )
  {
    return;
  }

  const auto &image = m_pProject->imageModel().entry( index );

  if( image.image().isNull() )
  {
    ui->imagePropertyL->setText( QString( "Bild: %1\nDatei nicht vorhanden oder unbekanntes Format." )
      .arg( image.name() ) );
    ui->rawImageL->setPixmap( QPixmap() );
  }
  else
  {
    ui->imagePropertyL->setText( QString( "Bild: %1\nGröße: %2 x %3\nOriginal: %4mm x %5mm\nRotation: %6°" )
      .arg( image.name() ).arg( image.image().width() ).arg( image.image().height() )
      .arg( image.initialWidth_mm() ).arg( image.initialHeight_mm() ).arg( image.initialRotation_deg() ) );

    QTransform rotation = QTransform().rotate( image.initialRotation_deg() );
    QTransform scale    = rotation.scale( m_currentRawImageScale, m_currentRawImageScale );
    ui->rawImageL->setPixmap( QPixmap::fromImage( image.image().transformed( scale ) ) );
  }
}


//=================================================================================================================
void RWScanRect::updateRawImageScale(int comboBoxIndex)
{
  m_currentRawImageScale = ui->rawImageScaleCB->itemData( comboBoxIndex ).toDouble();
  updateImage( ui->imageLV->currentIndex() );
}


//=================================================================================================================
void RWScanRect::projectChanged()
{
  updateTitle();

  ui->imageLV->setModel( &m_pProject->imageModel() );

  connect( m_pProject.get(), &RWScanProject::modifiedChanged, this, &QWidget::setWindowModified );
  connect( m_pProject.get(), &RWScanProject::filenameChanged, this, &RWScanRect::updateTitle );

  //connect( ui->imageAddPB, &QAbstractButton::clicked, m_pProject.get(), &RWScanProject::addImages );
}


//=================================================================================================================
bool RWScanRect::closeProject()
{
  if( m_pProject->modified() )
  {
    saveProject();
  }
  return !m_pProject->modified();
}


#include <QtCore/QCoreApplication>
#include <QtGui/QImage>

#include <iostream>
#include <vector>


QRgb maximumWhiteValue( const QImage &image )
{
  const QRgb *data         = (const QRgb*) image.constBits();
  auto        count        = image.width() * image.height();
  int         maximumRed   = 0;
  int         maximumGreen = 0;
  int         maximumBlue  = 0;

  for( int i = 0; i < count; i++ )
  {
    QRgb pixel = data[i];
    int  red   = qRed( pixel );
    int  green = qGreen( pixel );
    int  blue  = qBlue( pixel );

    if( red > maximumRed )
    {
      maximumRed = red;
    }
    if( green > maximumGreen )
    {
      maximumGreen = green;
    }
    if( blue > maximumBlue )
    {
      maximumBlue = blue;
    }
  }
  return qRgb( maximumRed, maximumGreen, maximumBlue );
}


int main( int argc, char *argv[] )
{
  QCoreApplication  app( argc, argv );

  auto args = app.arguments();
  if( args.count() < 4 )
  {
    std::cerr << "Usage: " << args[0].toStdString() << " <inImage>  <diagImage>  <outImage>" << std::endl;
    return 1;
  }

  QImage image( args[1] );
  if( image.isNull() )
  {
    std::cerr << "Failed to open image '" << args[1].toStdString() << "'" << std::endl;
    return 1;
  }

  QImage inImage = image.convertToFormat( QImage::Format_ARGB32 );
  QImage scaledImage;
  double pixelScale = 1.;
  if( inImage.width() > inImage.height() )
  {
    auto sw = inImage.width();

    while( sw > 1024 )
    {
      sw /= 2;
      pixelScale *= 2.;
    }
    scaledImage = inImage.scaledToWidth( sw, Qt::SmoothTransformation );
  }
  else
  {
    auto sh = inImage.height();

    while( sh > 1024 )
    {
      sh /= 2;
      pixelScale *= 2.;
    }
    scaledImage = inImage.scaledToHeight( sh, Qt::SmoothTransformation );
  }

  const QRgb *data = (const QRgb*) scaledImage.constBits();
  auto        w = scaledImage.width();
  auto        h = scaledImage.height();
  auto        maxWhite = maximumWhiteValue( scaledImage );
  std::vector<int> leftBorderPixel( h );
  std::vector<int> rightBorderPixel( h );
  std::vector<int> topBorderPixel( w );
  std::vector<int> bottomBorderPixel( w );

  std::cout << "Size ( " << w << " x " << h << " ), maximum white is ( " <<
    qRed( maxWhite ) << " " << qGreen( maxWhite ) << " " << qBlue( maxWhite ) << " )" << std::endl;

  for( int row = 0; row < h; row++ )
  {
    const QRgb *line = data + row * w;

    QRgb first = line[0];
    int  firstRed   = qRed  ( first );
    int  firstGreen = qGreen( first );
    int  firstBlue  = qBlue ( first );
    //std::cout << "  line " << row << ": left distance is ( " << qRed( maxWhite ) - firstRed << " " <<
    //  qGreen( maxWhite ) - firstGreen << " " << qBlue( maxWhite ) - firstBlue << " )" << std::endl;
    int  dRed   = firstRed   + (int) ( .6f * (float) ( qRed  ( maxWhite ) - firstRed   ) );
    int  dGreen = firstGreen + (int) ( .6f * (float) ( qGreen( maxWhite ) - firstGreen ) );
    int  dBlue  = firstBlue  + (int) ( .6f * (float) ( qBlue ( maxWhite ) - firstBlue  ) );

    leftBorderPixel[row] = 0;
    for( int col = 1; col < w - 1; col++ )
    {
      QRgb current = line[col];
      int  red     = qRed  ( current );
      int  green   = qGreen( current );
      int  blue    = qBlue ( current );

      if( ( red >= dRed ) && ( green >= dGreen ) && ( blue >= dBlue ) )
      {
        scaledImage.setPixel( col, row, qRgb( 255, 0, 0 ) );
        leftBorderPixel[row] = col;
        break;
      }
    }
  }
  for( int row = 0; row < h; row++ )
  {
    const QRgb *line = data + row * w;

    QRgb last = line[w - 1];
    int  lastRed   = qRed  ( last );
    int  lastGreen = qGreen( last );
    int  lastBlue  = qBlue ( last );
    //std::cout << "  line " << row << ": left distance is ( " << qRed( maxWhite ) - firstRed << " " <<
    //  qGreen( maxWhite ) - firstGreen << " " << qBlue( maxWhite ) - firstBlue << " )" << std::endl;
    int  dRed   = lastRed   + (int) ( .6f * (float) ( qRed  ( maxWhite ) - lastRed   ) );
    int  dGreen = lastGreen + (int) ( .6f * (float) ( qGreen( maxWhite ) - lastGreen ) );
    int  dBlue  = lastBlue  + (int) ( .6f * (float) ( qBlue ( maxWhite ) - lastBlue  ) );

    rightBorderPixel[row] = 0;
    for( int col = w - 2; col > 0; col-- )
    {
      QRgb current = line[col];
      int  red     = qRed  ( current );
      int  green   = qGreen( current );
      int  blue    = qBlue ( current );

      if( ( red >= dRed ) && ( green >= dGreen ) && ( blue >= dBlue ) )
      {
        scaledImage.setPixel( col, row, qRgb( 255, 0, 0 ) );
        rightBorderPixel[row] = col;
        break;
      }
    }
  }
  for( int col = 0; col < w; col++ )
  {
    const QRgb *line = data + col;

    QRgb first = line[0];
    int  firstRed   = qRed  ( first );
    int  firstGreen = qGreen( first );
    int  firstBlue  = qBlue ( first );
    //std::cout << "  line " << row << ": left distance is ( " << qRed( maxWhite ) - firstRed << " " <<
    //  qGreen( maxWhite ) - firstGreen << " " << qBlue( maxWhite ) - firstBlue << " )" << std::endl;
    int  dRed   = firstRed   + (int) ( .6f * (float) ( qRed  ( maxWhite ) - firstRed   ) );
    int  dGreen = firstGreen + (int) ( .6f * (float) ( qGreen( maxWhite ) - firstGreen ) );
    int  dBlue  = firstBlue  + (int) ( .6f * (float) ( qBlue ( maxWhite ) - firstBlue  ) );

    topBorderPixel[col] = 0;
    for( int row = 1; row < h - 1; row++ )
    {
      QRgb current = line[w*row];
      int  red     = qRed  ( current );
      int  green   = qGreen( current );
      int  blue    = qBlue ( current );

      if( ( red >= dRed ) && ( green >= dGreen ) && ( blue >= dBlue ) )
      {
        scaledImage.setPixel( col, row, qRgb( 255, 0, 0 ) );
        topBorderPixel[col] = row;
        break;
      }
    }
  }
  for( int col = 0; col < w; col++ )
  {
    const QRgb *line = data + col;

    QRgb last = line[w*(h-1)];
    int  lastRed   = qRed  ( last );
    int  lastGreen = qGreen( last );
    int  lastBlue  = qBlue ( last );
    //std::cout << "  line " << row << ": left distance is ( " << qRed( maxWhite ) - firstRed << " " <<
    //  qGreen( maxWhite ) - firstGreen << " " << qBlue( maxWhite ) - firstBlue << " )" << std::endl;
    int  dRed   = lastRed   + (int) ( .6f * (float) ( qRed  ( maxWhite ) - lastRed   ) );
    int  dGreen = lastGreen + (int) ( .6f * (float) ( qGreen( maxWhite ) - lastGreen ) );
    int  dBlue  = lastBlue  + (int) ( .6f * (float) ( qBlue ( maxWhite ) - lastBlue  ) );

    bottomBorderPixel[col] = 0;
    for( int row = h - 2; row > 0; row-- )
    {
      QRgb current = line[w*row];
      int  red     = qRed  ( current );
      int  green   = qGreen( current );
      int  blue    = qBlue ( current );

      if( ( red >= dRed ) && ( green >= dGreen ) && ( blue >= dBlue ) )
      {
        scaledImage.setPixel( col, row, qRgb( 255, 0, 0 ) );
        bottomBorderPixel[col] = row;
        break;
      }
    }
  }

  int leftBorderCenter = leftBorderPixel[h/2];
  int topLeftRow = h / 2 - 1;
  int topLeftCol = leftBorderCenter;
  for( ; topLeftRow > 0; topLeftRow-- )
  {
    if( ( leftBorderPixel[topLeftRow] < topLeftCol - 1 ) || ( leftBorderPixel[topLeftRow] > topLeftCol + 1 ) )
    {
      topLeftRow++;
      break;
    }
    else
    {
      topLeftCol = leftBorderPixel[topLeftRow];
    }
  }
  int bottomLeftRow = h / 2 - 1;
  int bottomLeftCol = leftBorderCenter;
  for( ; bottomLeftRow < h; bottomLeftRow++ )
  {
    if( ( leftBorderPixel[bottomLeftRow] < bottomLeftCol - 1 ) || ( leftBorderPixel[bottomLeftRow] > bottomLeftCol + 1 ) )
    {
      bottomLeftRow--;
      break;
    }
    else
    {
      bottomLeftCol = leftBorderPixel[bottomLeftRow];
    }
  }

  int rightBorderCenter = rightBorderPixel[h/2];
  int topRightRow = h / 2 - 1;
  int topRightCol = rightBorderCenter;
  for( ; topRightRow > 0; topRightRow-- )
  {
    if( ( rightBorderPixel[topRightRow] < topRightCol - 1 ) || ( rightBorderPixel[topRightRow] > topRightCol + 1 ) )
    {
      topRightRow++;
      break;
    }
    else
    {
      topRightCol = rightBorderPixel[topRightRow];
    }
  }
  int bottomRightRow = h / 2 - 1;
  int bottomRightCol = rightBorderCenter;
  for( ; bottomRightRow < h; bottomRightRow++ )
  {
    if( ( rightBorderPixel[bottomRightRow] < bottomRightCol - 1 ) || ( rightBorderPixel[bottomRightRow] > bottomRightCol + 1 ) )
    {
      bottomRightRow--;
      break;
    }
    else
    {
      bottomRightCol = rightBorderPixel[bottomRightRow];
    }
  }

  int topBorderCenter = topBorderPixel[w/2];
  int leftTopCol = w / 2 - 1;
  int leftTopRow = topBorderCenter;
  for( ; leftTopCol > 0; leftTopCol-- )
  {
    if( ( topBorderPixel[leftTopCol] < leftTopRow - 1 ) || ( topBorderPixel[leftTopCol] > leftTopRow + 1 ) )
    {
      leftTopCol++;
      break;
    }
    else
    {
      leftTopRow = topBorderPixel[leftTopCol];
    }
  }
  int rightTopCol = w / 2 - 1;
  int rightTopRow = topBorderCenter;
  for( ; rightTopCol < w; rightTopCol++ )
  {
    if( ( topBorderPixel[rightTopCol] < rightTopRow - 1 ) || ( topBorderPixel[rightTopCol] > rightTopRow + 1 ) )
    {
      rightTopCol--;
      break;
    }
    else
    {
      rightTopRow = topBorderPixel[rightTopCol];
    }
  }

  int bottomBorderCenter = bottomBorderPixel[w/2];
  int leftBottomCol = w / 2 - 1;
  int leftBottomRow = bottomBorderCenter;
  for( ; leftBottomCol > 0; leftBottomCol-- )
  {
    if( ( bottomBorderPixel[leftBottomCol] < leftBottomRow - 1 ) || ( bottomBorderPixel[leftBottomCol] > leftBottomRow + 1 ) )
    {
      leftBottomCol++;
      break;
    }
    else
    {
      leftBottomRow = bottomBorderPixel[leftBottomCol];
    }
  }
  int rightBottomCol = w / 2 - 1;
  int rightBottomRow = bottomBorderCenter;
  for( ; rightBottomCol < w; rightBottomCol++ )
  {
    if( ( bottomBorderPixel[rightBottomCol] < rightBottomRow - 1 ) || ( bottomBorderPixel[rightBottomCol] > rightBottomRow + 1 ) )
    {
      rightBottomCol--;
      break;
    }
    else
    {
      rightBottomRow = bottomBorderPixel[rightBottomCol];
    }
  }

  scaledImage.setPixel( topLeftCol, topLeftRow, qRgb( 0, 255, 0 ) );
  scaledImage.setPixel( bottomLeftCol, bottomLeftRow, qRgb( 0, 255, 0 ) );
  scaledImage.setPixel( topRightCol, topRightRow, qRgb( 0, 255, 0 ) );
  scaledImage.setPixel( bottomRightCol, bottomRightRow, qRgb( 0, 255, 0 ) );
  scaledImage.setPixel( leftTopCol, leftTopRow, qRgb( 0, 255, 0 ) );
  scaledImage.setPixel( leftBottomCol, leftBottomRow, qRgb( 0, 255, 0 ) );
  scaledImage.setPixel( rightTopCol, rightTopRow, qRgb( 0, 255, 0 ) );
  scaledImage.setPixel( rightBottomCol, rightBottomRow, qRgb( 0, 255, 0 ) );
  scaledImage.save( args[2], "PNG" );

  float x[5][5];
  float y[5][5];
  x[0][0] = .5 * ( topLeftCol + leftTopCol );
  y[0][0] = .5 * ( topLeftRow + leftTopRow );
  x[0][4] = .5 * ( topRightCol + rightTopCol );
  y[0][4] = .5 * ( topRightRow + rightTopRow );
  x[4][0] = .5 * ( bottomLeftCol + leftBottomCol );
  y[4][0] = .5 * ( bottomLeftRow + leftBottomRow );
  x[4][4] = .5 * ( bottomRightCol + rightBottomCol );
  y[4][4] = .5 * ( bottomRightRow + rightBottomRow );

  for( int i = 1; i <= 3; i++ )
  {
    x[0][i] = ( 4 - i ) * .25 * x[0][0] + i * .25 * x[0][4];
    y[0][i] = topBorderPixel[(int)rint( x[0][i] )];
    y[i][0] = ( 4 - i ) * .25 * y[0][0] + i * .25 * y[4][0];
    x[i][0] = leftBorderPixel[(int)rint( y[i][0] )];
    y[i][4] = ( 4 - i ) * .25 * y[0][4] + i * .25 * y[4][4];
    x[i][4] = rightBorderPixel[(int)rint( y[i][4] )];
    x[4][i] = ( 4 - i ) * .25 * x[4][0] + i * .25 * x[4][4];
    y[4][i] = bottomBorderPixel[(int)rint( x[4][i] )];
  }

  //x[2][2] = .25 * ( x[2][0] + .5 * ( x[0][2] - x[0][0] + x[4][2] - x[4][0] ) )
  //        + .25 * ( x[2][4] + .5 * ( x[0][2] - x[0][4] + x[4][2] - x[4][4] ) )
  //        + .25 * ( x[0][2] + .5 * ( x[2][0] - x[0][0] + x[2][4] - x[0][4] ) )
  //        + .25 * ( x[4][2] + .5 * ( x[2][0] - x[4][0] + x[2][4] - x[4][4] ) );
  x[2][2] = .5 * ( x[2][0] + x[2][4] + x[0][2] + x[4][2] ) - .25 * ( x[0][0] + x[0][4] + x[4][0] + x[4][4] );
  y[2][2] = .5 * ( y[2][0] + y[2][4] + y[0][2] + y[4][2] ) - .25 * ( y[0][0] + y[0][4] + y[4][0] + y[4][4] );

  x[2][1] = .5 * ( x[2][0] + x[2][2] + x[0][1] + x[4][1] ) - .25 * ( x[0][0] + x[0][2] + x[4][0] + x[4][2] );
  y[2][1] = .5 * ( y[2][0] + y[2][2] + y[0][1] + y[4][1] ) - .25 * ( y[0][0] + y[0][2] + y[4][0] + y[4][2] );
  x[2][3] = .5 * ( x[2][2] + x[2][4] + x[0][3] + x[4][3] ) - .25 * ( x[0][2] + x[0][4] + x[4][2] + x[4][4] );
  y[2][3] = .5 * ( y[2][2] + y[2][4] + y[0][3] + y[4][3] ) - .25 * ( y[0][2] + y[0][4] + y[4][2] + y[4][4] );
  x[1][2] = .5 * ( x[1][0] + x[1][4] + x[0][2] + x[2][2] ) - .25 * ( x[0][0] + x[0][4] + x[2][0] + x[2][4] );
  y[1][2] = .5 * ( y[1][0] + y[1][4] + y[0][2] + y[2][2] ) - .25 * ( y[0][0] + y[0][4] + y[2][0] + y[2][4] );
  x[3][2] = .5 * ( x[3][0] + x[3][4] + x[2][2] + x[4][2] ) - .25 * ( x[2][0] + x[2][4] + x[4][0] + x[4][4] );
  y[3][2] = .5 * ( y[3][0] + y[3][4] + y[2][2] + y[4][2] ) - .25 * ( y[2][0] + y[2][4] + y[4][0] + y[4][4] );

  x[1][1] = .5 * ( x[1][0] + x[1][2] + x[0][1] + x[2][1] ) - .25 * ( x[0][0] + x[0][2] + x[2][0] + x[2][2] );
  y[1][1] = .5 * ( y[1][0] + y[1][2] + y[0][1] + y[2][1] ) - .25 * ( y[0][0] + y[0][2] + y[2][0] + y[2][2] );
  x[1][3] = .5 * ( x[1][2] + x[1][4] + x[0][3] + x[2][3] ) - .25 * ( x[0][2] + x[0][4] + x[2][2] + x[2][4] );
  y[1][3] = .5 * ( y[1][2] + y[1][4] + y[0][3] + y[2][3] ) - .25 * ( y[0][2] + y[0][4] + y[2][2] + y[2][4] );
  x[3][1] = .5 * ( x[3][0] + x[3][2] + x[2][1] + x[4][1] ) - .25 * ( x[2][0] + x[2][2] + x[4][0] + x[4][2] );
  y[3][1] = .5 * ( y[3][0] + y[3][2] + y[2][1] + y[4][1] ) - .25 * ( y[2][0] + y[2][2] + y[4][0] + y[4][2] );
  x[3][3] = .5 * ( x[3][2] + x[3][4] + x[2][3] + x[4][3] ) - .25 * ( x[2][2] + x[2][4] + x[4][2] + x[4][4] );
  y[3][3] = .5 * ( y[3][2] + y[3][4] + y[2][3] + y[4][3] ) - .25 * ( y[2][2] + y[2][4] + y[4][2] + y[4][4] );

  //std::cout << std::fixed;
  //for( int i = 0; i <= 4; i++ )
  //{
  //  for( int j = 0; j <= 4; j++ )
  //  {
  //    std::cout.precision( 2 );
  //    std::cout << "( " << x[i][j] << " ; " << y[i][j] << " )   ";
  //  }
  //  std::cout << std::endl;
  //}
  for( int i = 0; i <= 4; i++ )
  {
    for( int j = 0; j <= 4; j++ )
    {
      x[i][j] *= pixelScale;
      y[i][j] *= pixelScale;
    }
  }

  double targetWidth  = ( ( w > h ) ? 297. : 210. ) /*mm*/ / 25.4 /*mm/inch*/ * 200. /*pixel/inch*/;
  double targetHeight = ( ( w > h ) ? 210. : 297. ) /*mm*/ / 25.4 /*mm/inch*/ * 200. /*pixel/inch*/;
  int    tw = (int)floor( targetWidth );
  int    th = (int)floor( targetHeight );

  QImage outImage( tw, th, QImage::Format_ARGB32 );

  for( int row = 0; row < th; row++ )
  {
    double yTarget = 4. * row / targetHeight;
    int    yCell   = (int)floor( yTarget );
    double yOffset = yTarget - yCell;

    for( int col = 0; col < tw; col++ )
    {
      double xTarget = 4. * col / targetWidth;
      int    xCell   = (int)floor( xTarget );
      double xOffset = xTarget - xCell;

      double xSource = ( 1. - xOffset ) * ( 1. - yOffset ) * x[ yCell ][ xCell ]
                     + (   xOffset    ) * ( 1. - yOffset ) * x[ yCell ][xCell+1]
                     + ( 1. - xOffset ) * (   yOffset    ) * x[yCell+1][ xCell ]
                     + (   xOffset    ) * (   yOffset    ) * x[yCell+1][xCell+1];
      double ySource = ( 1. - xOffset ) * ( 1. - yOffset ) * y[ yCell ][ xCell ]
                     + (   xOffset    ) * ( 1. - yOffset ) * y[ yCell ][xCell+1]
                     + ( 1. - xOffset ) * (   yOffset    ) * y[yCell+1][ xCell ]
                     + (   xOffset    ) * (   yOffset    ) * y[yCell+1][xCell+1];
      outImage.setPixel( col, row, inImage.pixel( (int)rint( xSource ), (int)rint( ySource ) ) );
    }
  }

  outImage.save( args[3], "PNG" );
}

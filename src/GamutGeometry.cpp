/* 
 * File:   GamutGeometry.cpp
 * Author: allanws
 * 
 * Created on March 10, 2016, 9:24 AM
 */

#include <sys/param.h>
#include <assert.h>

#include <osg/MatrixTransform>

#include "GamutGeometry.h"

#define SPECTRUM_FIRST_WL 380
#define SPECTRUM_LAST_WL 780
#define SPECTRUM_INC_WL 5

const osg::Matrixd GamutGeometry::_rgbToXYZMatrix = osg::Matrixd( 
0.490, 0.177, 0.000, 0.000,
0.310, 0.813, 0.010, 0.000,
0.200, 0.011, 0.990, 0.000,
0.000, 0.000, 0.000, 1.000 );

const osg::Matrixd GamutGeometry::_rgbToSRGBMatrix = osg::Matrixd( 
0.4124564, 0.2126729, 0.0193339, 0.0000000,
0.3575761, 0.7151522, 0.1191920, 0.0000000,
0.1804375, 0.0721750, 0.9503041, 0.0000000,
0.0000000, 0.0000000, 0.0000000, 1.0000000 );

GamutGeometry::GamutGeometry( RGBSpectrum* rgbSpectrum, DiscreteSpectrum* illuminant ) :
    _rgbSpectrum( rgbSpectrum ),
    _illuminant( illuminant ),
    _renderMode( POINTS )
{
    calculateK();
    
    setColorMode( RGB );
    
    buildGeometry();
}


GamutGeometry::~GamutGeometry() 
{
    delete _illuminant;
    _illuminant = 0;
    
    delete _rgbSpectrum;
    _rgbSpectrum = 0;
}


void GamutGeometry::setColorMode( ColorMode colorMode )
{
    if( colorMode == COLOR_NONE )
        return;
    
    switch( colorMode )
    {
        case RGB:
            _currentTransform = &GamutGeometry::identityTransform;
            break;
            
        case XYZ:
            _currentTransform = &GamutGeometry::rgbToXYZTransform;
            break;
            
        case LAB:
            _currentTransform = &GamutGeometry::rgbToLabTransform;
            break;
            
        case SRGB:
            _currentTransform = &GamutGeometry::rgbToSRGBTransform;
            break;
            
        default:
            return;
    }
    
    buildGeometry();
}


void GamutGeometry::setRenderMode( RenderMode renderMode )
{
    if( renderMode == RENDER_NONE )
        return;
    
    _renderMode = renderMode;
    
    buildGeometry();
}


void GamutGeometry::calculateK()
{
    int wavelength;
    float invK = 0;
    
    for( wavelength = SPECTRUM_FIRST_WL; 
         wavelength <= SPECTRUM_LAST_WL;
         wavelength += SPECTRUM_INC_WL )
    {
        float r, g, b;        
        getRGB( wavelength, r, g, b );
        
        invK += _illuminant->valueOf( wavelength ) * b;//( _isXYZ ? g : b );
    }
    
    _k = 1./( invK * SPECTRUM_INC_WL );
}


void GamutGeometry::buildGeometry()
{
    VertexArrayPtr vertices = new osg::Vec3Array;
    ColorArrayPtr colors = new osg::Vec4Array; 
    
    getPrimitiveSetList().clear();
    
    switch( _renderMode )
    {
        case POINTS:
            pointPlot( vertices, colors );
            break;
            
        case LINES:
            linePlot( vertices, colors );
            break;
            
        case POLYGON:
            polygonPlot( vertices, colors );
            break;
            
        default:
            break;
    }    
    
    setVertexArray( vertices );    
    setColorArray( colors );
    
    setColorBinding( osg::Geometry::BIND_PER_VERTEX );
}


void GamutGeometry::pointPlot( VertexArrayPtr vertexArray, ColorArrayPtr colorArray )
{
    PrimitiveArrayPtr primitiveArray = new osg::DrawElementsUInt( osg::PrimitiveSet::POINTS, 0 );   
    
    int boxSize, boxStart, index = 0; 
    
    for( boxSize = SPECTRUM_INC_WL; 
         boxSize <= ( SPECTRUM_LAST_WL - SPECTRUM_FIRST_WL ) / SPECTRUM_INC_WL;
         boxSize += SPECTRUM_INC_WL )
    { 
        for( boxStart = SPECTRUM_FIRST_WL; 
             boxStart <= SPECTRUM_LAST_WL;
             boxStart += SPECTRUM_INC_WL )
        {
            float x, y, z;
            getXYZ( boxStart, boxSize, x, y, z );
            
            osg::Vec3 point( x, y, z );
            point *= SPECTRUM_INC_WL;
            point *= _k;
            
            osg::Vec3 transformedPoint( 0.f, 0.f, 0.f );
            
            if( _currentTransform( point, transformedPoint ) )
            {               
                colorArray->push_back( osg::Vec4( point, 1.0f ) );

                vertexArray->push_back( transformedPoint );            
                primitiveArray->push_back( index++ );
            }
        }        
    }
    
    addPrimitiveSet( primitiveArray );
}


void GamutGeometry::linePlot( VertexArrayPtr vertexArray, ColorArrayPtr colorArray )
{ 
    int boxSize, boxStart, index = 0; 
    
    for( boxSize = SPECTRUM_INC_WL; 
         boxSize <= ( SPECTRUM_LAST_WL - SPECTRUM_FIRST_WL ) / SPECTRUM_INC_WL;
         boxSize += SPECTRUM_INC_WL )
    { 
        PrimitiveArrayPtr primitiveArray = new osg::DrawElementsUInt( osg::PrimitiveSet::LINE_LOOP, 0 );
        
        for( boxStart = SPECTRUM_FIRST_WL; 
             boxStart <= SPECTRUM_LAST_WL;
             boxStart += SPECTRUM_INC_WL )
        {
            float x, y, z;
            getXYZ( boxStart, boxSize, x, y, z );
            
            osg::Vec3 point( x, y, z );
            point *= SPECTRUM_INC_WL;
            point *= _k;
            
            osg::Vec3 transformedPoint( 0.f, 0.f, 0.f );
            
            if( _currentTransform( point, transformedPoint ) )
            {               
                colorArray->push_back( osg::Vec4( point, 1.0f ) );

                vertexArray->push_back( transformedPoint );            
                primitiveArray->push_back( index++ );
            }
        }
        
        addPrimitiveSet( primitiveArray );
    }
        
    setColorBinding( osg::Geometry::BIND_PER_VERTEX );
}


void GamutGeometry::polygonPlot( VertexArrayPtr vertexArray, ColorArrayPtr colorArray )
{
    int boxSize, boxStart, index = 0; 
    
    for( boxSize = 0; 
         boxSize < ( SPECTRUM_LAST_WL - SPECTRUM_FIRST_WL ) / SPECTRUM_INC_WL;
         boxSize += SPECTRUM_INC_WL )
    { 
        PrimitiveArrayPtr primitiveArray = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_STRIP, 0 );
        
        for( boxStart = SPECTRUM_FIRST_WL; 
             boxStart <= SPECTRUM_LAST_WL;
             boxStart += SPECTRUM_INC_WL )
        {
            float x, y, z;
            osg::Vec3 point;
            osg::Vec3 transformedPoint;
            
            getXYZ( boxStart, boxSize, x, y, z );
            
            point.set( x, y, z );
            point *= SPECTRUM_INC_WL;
            point *= _k;
            
            transformedPoint.set( 0.f, 0.f, 0.f );
            
            if( _currentTransform( point, transformedPoint ) )
            {               
                colorArray->push_back( osg::Vec4( point, 1.0f ) );

                vertexArray->push_back( transformedPoint );            
                primitiveArray->push_back( index++ );
            }
            
            getXYZ( boxStart, boxSize + SPECTRUM_INC_WL, x, y, z );
            
            point.set( x, y, z );
            point *= SPECTRUM_INC_WL;
            point *= _k;
            
            transformedPoint.set( 0.f, 0.f, 0.f );
            
            if( _currentTransform( point, transformedPoint ) )
            {               
                colorArray->push_back( osg::Vec4( point, 1.0f ) );

                vertexArray->push_back( transformedPoint );            
                primitiveArray->push_back( index++ );
            }
        }
        
        addPrimitiveSet( primitiveArray );
    }
        
    setColorBinding( osg::Geometry::BIND_PER_VERTEX );
}
    
    
void GamutGeometry::getRGB( int wavelength, float& r, float& g, float& b )
{
    r = _rgbSpectrum->_r->valueOf( wavelength ) * 700.;
    g = _rgbSpectrum->_g->valueOf( wavelength ) * 546.;
    b = _rgbSpectrum->_b->valueOf( wavelength ) * 435.8;
}


void GamutGeometry::getXYZ( int boxStart, int boxSize, float& x, float& y, float& z )
{
    int wavelength;
    x = 0;
    y = 0;
    z = 0;
    int sumSize = 0;

    if( boxSize == 0 )
        return;
    
    int boxEnd = boxStart + boxSize * SPECTRUM_INC_WL;

    if( boxEnd > SPECTRUM_LAST_WL )
    {
        int loopEnd = SPECTRUM_FIRST_WL + ( boxEnd - SPECTRUM_LAST_WL );
        boxEnd = SPECTRUM_LAST_WL;

        for( wavelength = SPECTRUM_FIRST_WL;
             wavelength < loopEnd;
             wavelength += SPECTRUM_INC_WL )
        {            
            float r, g, b;
            getRGB( wavelength, r, g, b );
            
            x += _illuminant->valueOf( wavelength ) * r;
            y += _illuminant->valueOf( wavelength ) * g;
            z += _illuminant->valueOf( wavelength ) * b;
            
            sumSize++;
        }
    }

    for( wavelength = boxStart;
         wavelength < boxEnd;
         wavelength += SPECTRUM_INC_WL )
    {        
        float r, g, b;
        getRGB( wavelength, r, g, b );
        
        x += _illuminant->valueOf( wavelength ) * r;
        y += _illuminant->valueOf( wavelength ) * g;
        z += _illuminant->valueOf( wavelength ) * b;       
        
        sumSize++;
    }
    
    assert( sumSize == boxSize );
}


const std::vector< float > GamutGeometry::referenceWhite = { 0.95047f,  1.0f, 1.08883f }; //d65


bool GamutGeometry::identityTransform( const osg::Vec3& rgb, osg::Vec3& rgb0 )
{
    rgb0 = rgb * osg::Matrix::identity();
    
    return true;
}


bool GamutGeometry::rgbToXYZTransform( const osg::Vec3& rgb, osg::Vec3& xyz )
{
    xyz = rgb * _rgbToXYZMatrix;
    
    return true;
}

    
bool GamutGeometry::rgbToLabTransform( const osg::Vec3& rgb, osg::Vec3& lab )
{
    auto gammaLab = []( double t )
    {
        double ft, e = 216.0 / 24389, k = 24389.0 / 27;
        
	if( t > e ) 
            ft = pow( t, 1.0 / 3 ); 
	else
            ft = ( k * t + 16 ) / 116;
        
	return ft;
    };
    
    if( rgbToXYZTransform( rgb, lab ) )
    {    
        double xr = (double)lab.x() / GamutGeometry::referenceWhite[ 0 ];
        double yr = (double)lab.y() / GamutGeometry::referenceWhite[ 1 ];
        double zr = (double)lab.z() / GamutGeometry::referenceWhite[ 2 ];

        double fx = gammaLab( xr );
        double fy = gammaLab( yr );
        double fz = gammaLab( zr );

        lab.set( 
            (float)( 116 * fy - 16 ), 
            (float)( 500 * ( fx - fy ) ), 
            (float)( 200 * ( fy - fz ) ) 
        );

        return true;
    }
    
    return false;
}


bool GamutGeometry::rgbToSRGBTransform( const osg::Vec3& rgb, osg::Vec3& srgb )
{
    auto gammasRGB = []( double x )
    {
	double ft, t = ( x > 0 ) ? x : -x;
        
	if( t > 0.0031308 ) 
            ft = 1.055 * pow( t, 1.0 / 2.4 ) - 0.055;
	else                  
            ft = 12.92 * t;

	return ( x > 0 ) ? ft : -ft;
    };
    
    if( rgbToXYZTransform( rgb, srgb ) )
    {
        double r,g,b;
        double X = srgb.x();
        double Y = srgb.y();
        double Z = srgb.z();

        r = 3.2404542 * X - 1.5371385 * Y - 0.4985314 * Z;
        g =-0.9692660 * X + 1.8760108 * Y + 0.0415560 * Z;
        b = 0.0556434 * X - 0.2040259 * Y + 1.0572252 * Z;

        r = gammasRGB( r );
        g = gammasRGB( g );
        b = gammasRGB( b );

        srgb.set(
            (float) r,
            (float) g,
            (float) b
        );
        
        return true;
    }    
    
    return false;
}
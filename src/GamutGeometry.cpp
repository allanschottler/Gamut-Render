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

GamutGeometry::GamutGeometry( RGBSpectrum* rgbSpectrum, DiscreteSpectrum* illuminant ) :
    _rgbSpectrum( rgbSpectrum ),
    _illuminant( illuminant ),
    _isXYZ( false )
{
    buildGeometry();
}


GamutGeometry::~GamutGeometry() 
{
    delete _illuminant;
    _illuminant = 0;
    
    delete _rgbSpectrum;
    _rgbSpectrum = 0;
}


void GamutGeometry::setRenderMode( RenderMode renderMode )
{
    switch( renderMode )
    {
        case RGB:
            _isXYZ = false;
            break;
            
        case XYZ:
            _isXYZ = true;
            break;
            
        default:
            return;
    }
    
    buildGeometry();
}


void GamutGeometry::buildGeometry()
{
    osg::ref_ptr< osg::Vec3Array > vertices = new osg::Vec3Array;
    osg::ref_ptr< osg::Vec4Array > colors = new osg::Vec4Array;    
    osg::ref_ptr< osg::DrawElementsUInt > primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::POINTS, 0 );        
    
    int boxSize, boxStart, wavelength, index = 0;
    float invK = 0;
    
    for( wavelength = SPECTRUM_FIRST_WL; 
         wavelength <= SPECTRUM_LAST_WL;
         wavelength += SPECTRUM_INC_WL )
    {
        float r, g, b;        
        getRGB( wavelength, r, g, b );
        
        invK += _illuminant->valueOf( wavelength ) * b;//( _isXYZ ? g : b );
    }
    
    float k = 1./( invK * SPECTRUM_INC_WL );
    
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
            point *= k;
            
            colors->push_back( osg::Vec4( point, 1.0f ) );
            
            if( _isXYZ )
            {
                point = point * _rgbToXYZMatrix;
            }
            
            vertices->push_back( point );            
            primitiveSet->push_back( index++ );
        }        
    }
    
    addPrimitiveSet( primitiveSet );
    setVertexArray( vertices );
    
    setColorArray( colors );
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
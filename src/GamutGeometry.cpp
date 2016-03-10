/* 
 * File:   GamutGeometry.cpp
 * Author: allanws
 * 
 * Created on March 10, 2016, 9:24 AM
 */

#include "GamutGeometry.h"

#define SPECTRUM_FIRST_WL 380
#define SPECTRUM_LAST_WL 780
#define SPECTRUM_INC_WL 5

GamutGeometry::GamutGeometry( RGBSpectrum& rgbSpectrum, DiscreteSpectrum* illuminant ) :
    _rgbSpectrum( rgbSpectrum ),
    _illuminant( illuminant )
{
    buildGeometry();
}


GamutGeometry::~GamutGeometry() 
{
    delete _illuminant;
    _illuminant = 0;
}


void GamutGeometry::buildGeometry()
{
    osg::ref_ptr< osg::Vec3Array > vertices = new osg::Vec3Array;
    osg::ref_ptr< osg::Vec4Array > colors = new osg::Vec4Array;    
    osg::ref_ptr< osg::DrawElementsUInt > primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::POINTS, 0 );
    
    unsigned int wavelength, index = 0;
    
    for( wavelength = SPECTRUM_FIRST_WL; 
         wavelength < SPECTRUM_LAST_WL;
         wavelength += SPECTRUM_INC_WL )
    {
        float r, g, b;
        
        if( getRGB( wavelength, r, g, b ) )
        {
            float f = 1.f / ( r + g + b );
            
            osg::Vec3 pointRGB( r, g, b );            
            osg::Vec3 pointXYZ( r, g, b );
            
            pointXYZ *= f;
            
            vertices->push_back( pointXYZ );            
            colors->push_back( osg::Vec4( pointXYZ, 1.0f ) );
            primitiveSet->push_back( index++ );
        }
    }
    
    addPrimitiveSet( primitiveSet );
    setVertexArray( vertices );
    
    setColorArray( colors );
    setColorBinding( osg::Geometry::BIND_PER_VERTEX );
}


bool GamutGeometry::getRGB( int wavelength, float& r, float& g, float& b )
{
    //float l = _illuminant->valueOf( wavelength );
    float l = 1.f;
    
    r = l * _rgbSpectrum._r->valueOf( wavelength ) * 700.;
    g = l * _rgbSpectrum._g->valueOf( wavelength ) * 546.;
    b = l * _rgbSpectrum._b->valueOf( wavelength ) * 435.8;
    
    return true;
}
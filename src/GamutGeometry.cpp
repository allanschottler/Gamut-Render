/* 
 * File:   GamutGeometry.cpp
 * Author: allanws
 * 
 * Created on March 10, 2016, 9:24 AM
 */

#include <sys/param.h>

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
    
    int boxSize, boxStart, wavelength, index = 0;
    float invK = 0;
    
    for( wavelength = SPECTRUM_FIRST_WL; 
         wavelength <= SPECTRUM_LAST_WL;
         wavelength += SPECTRUM_INC_WL )
    {
        float r, g, b;
        
        if( getRGB( wavelength, r, g, b ) )
        {
            invK += _illuminant->valueOf( wavelength ) * g;
        }
    }
    
    float k = 1./( invK * SPECTRUM_INC_WL );
    
    for( boxSize = SPECTRUM_INC_WL; 
         boxSize <= SPECTRUM_LAST_WL - SPECTRUM_FIRST_WL;
         boxSize += SPECTRUM_INC_WL )
    { 
        for( boxStart = SPECTRUM_FIRST_WL; 
             boxStart <= SPECTRUM_LAST_WL;
             boxStart += SPECTRUM_INC_WL )
        {
            float x = 0;
            float y = 0;
            float z = 0;
            
            int boxEnd = boxStart + boxSize * SPECTRUM_INC_WL;
            
            if( boxEnd > SPECTRUM_LAST_WL )
            {
                int loopEnd = SPECTRUM_FIRST_WL + boxEnd - SPECTRUM_LAST_WL;
                boxEnd = SPECTRUM_LAST_WL;
                
                for( wavelength = SPECTRUM_FIRST_WL;
                     wavelength <= loopEnd;
                     wavelength += SPECTRUM_INC_WL )
                {
                    float r, g, b;
                    
                    if( getRGB( wavelength, r, g, b ) )
                    {
                        x += _illuminant->valueOf( wavelength ) * r;
                        y += _illuminant->valueOf( wavelength ) * g;
                        z += _illuminant->valueOf( wavelength ) * b;
                    }
                }
            }
            
            for( wavelength = boxStart;
                 wavelength <= boxEnd;
                 wavelength += SPECTRUM_INC_WL )
            {
                float r, g, b;
                    
                if( getRGB( wavelength, r, g, b ) )
                {
                    x += _illuminant->valueOf( wavelength ) * r;
                    y += _illuminant->valueOf( wavelength ) * g;
                    z += _illuminant->valueOf( wavelength ) * b;
                }
            }
            
            osg::Vec3 point( x, y, z );
            point *= SPECTRUM_INC_WL;

            point *= k;

            vertices->push_back( point );            
            colors->push_back( osg::Vec4( point, 1.0f ) );
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
    r = _rgbSpectrum._r->valueOf( wavelength );// * 700.;
    g = _rgbSpectrum._g->valueOf( wavelength );// * 546.;
    b = _rgbSpectrum._b->valueOf( wavelength );// * 435.8;
    
    return true;
}


float GamutGeometry::box( int wl0, int wli, int wl )
{    
    if( wl0 + wli > SPECTRUM_LAST_WL )
    {
        if( wl >= SPECTRUM_FIRST_WL && wl < SPECTRUM_FIRST_WL + ( SPECTRUM_LAST_WL - wl0 + wli ) )
            return 1.0f;
        
        return 0.0f;    
    }
    
    if( wl >= wl0 && wl < wl0 + wli )
        return 1.0f;
                
    return 0.0f;
}
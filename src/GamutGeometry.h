/* 
 * File:   GamutGeometry.h
 * Author: allanws
 *
 * Created on March 10, 2016, 9:23 AM
 */

#ifndef GAMUTGEOMETRY_H
#define	GAMUTGEOMETRY_H

#include "DiscreteSpectrum.h"

#include <osg/Geometry>

class DiscreteSpectrum;

class GamutGeometry : public osg::Geometry
{
public:
    
    class RGBSpectrum
    {
    public:
        
        RGBSpectrum( DiscreteSpectrum* r, 
                     DiscreteSpectrum* g, 
                     DiscreteSpectrum* b ) :
            _r( r ),
            _g( g ),
            _b( b ) {};
        
        ~RGBSpectrum() 
        {
            delete _r;
            delete _g;
            delete _b;
            
            _r = 0;
            _g = 0;
            _b = 0;
        };
        
        DiscreteSpectrum *_r, *_g, *_b;
    };
    
    GamutGeometry( RGBSpectrum& rgbSpectrum, DiscreteSpectrum* illuminant );
    
    virtual ~GamutGeometry();
    
private:
    
    void buildGeometry();
    
    bool getRGB( int wavelength, float& r, float& g, float& b );
    
    float box( int wl0, int wli, int wl );
    

    RGBSpectrum& _rgbSpectrum;
    
    DiscreteSpectrum* _illuminant;
};

#endif	/* GAMUTGEOMETRY_H */


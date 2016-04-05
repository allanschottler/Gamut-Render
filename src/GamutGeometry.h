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
    
    enum RenderMode
    {
        RGB = 0,
        XYZ,
        LAB,
        SRGB,
        TOTAL
    };
    
    GamutGeometry( RGBSpectrum* rgbSpectrum, DiscreteSpectrum* illuminant );
    
    virtual ~GamutGeometry();
    
    void setRenderMode( RenderMode renderMode );
        
    static const osg::Matrixd _rgbToXYZMatrix;
    
private:
    
    void buildGeometry();
    
    void getRGB( int wavelength, float& r, float& g, float& b );
    
    void getXYZ( int boxStart, int boxSize, float& x, float& y, float& z );
    

    RGBSpectrum* _rgbSpectrum;
    
    DiscreteSpectrum* _illuminant;
    
    bool _isXYZ;
};

#endif	/* GAMUTGEOMETRY_H */


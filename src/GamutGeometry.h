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
#include <functional>

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
    
    enum ColorMode
    {
        RGB = 0,
        XYZ,
        XYY,
        LAB,
        SRGB,
        COLOR_NONE
    };
    
    enum RenderMode
    {
        POINTS = 0,
        LINES,
        POLYGON,
        RENDER_NONE
    };
    
    GamutGeometry( RGBSpectrum* rgbSpectrum, DiscreteSpectrum* illuminant );
    
    virtual ~GamutGeometry();
    
    void setColorMode( ColorMode colorMode );
    
    void setRenderMode( RenderMode renderMode );
        
    static const osg::Matrixd _rgbToXYZMatrix;
    
    static const osg::Matrixd _rgbToLabMatrix;
    
    static const osg::Matrixd _rgbToSRGBMatrix;
    
    static const std::vector< float > referenceWhite; //d65
    
private:    
    
    typedef osg::ref_ptr< osg::DrawElementsUInt > PrimitiveArrayPtr;
    typedef osg::ref_ptr< osg::Vec3Array >        VertexArrayPtr;
    typedef osg::ref_ptr< osg::Vec4Array >        ColorArrayPtr;
    
    void calculateK();
    
    void buildGeometry();
    
    void pointPlot( VertexArrayPtr vertexArray, ColorArrayPtr colorArray );
    
    void linePlot( VertexArrayPtr vertexArray, ColorArrayPtr colorArray );
    
    void polygonPlot( VertexArrayPtr vertexArray, ColorArrayPtr colorArray );    
    
    void getRGB( int wavelength, float& r, float& g, float& b );
    
    void getXYZ( int boxStart, int boxSize, float& x, float& y, float& z );
    
    
    static bool identityTransform( const osg::Vec3& rgb, osg::Vec3& rgb0 );
    
    static bool rgbToXYZTransform( const osg::Vec3& rgb, osg::Vec3& xyz );
    
    static bool rgbToXYYTransform( const osg::Vec3& rgb, osg::Vec3& xyz );
    
    static bool rgbToLabTransform( const osg::Vec3& rgb, osg::Vec3& lab );
    
    static bool rgbToSRGBTransform( const osg::Vec3& rgb, osg::Vec3& srgb );
    

    RGBSpectrum* _rgbSpectrum;
    
    DiscreteSpectrum* _illuminant;
    
    typedef std::function< bool ( const osg::Vec3&, osg::Vec3& ) > ColorTransform;
    
    ColorTransform _currentTransform;
    
    float _k;
    
    RenderMode _renderMode;
};

#endif	/* GAMUTGEOMETRY_H */


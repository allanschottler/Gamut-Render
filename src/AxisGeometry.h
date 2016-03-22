/* 
 * File:   AxisGeometry.h
 * Author: allanws
 *
 * Created on March 10, 2016, 10:31 AM
 */

#ifndef AXISGEOMETRY_H
#define	AXISGEOMETRY_H

#include <osg/Geometry>

class AxisGeometry : public osg::Geometry
{
public:
    
    enum RenderMode
    {
        RGB,
        XYZ
    };
    
    AxisGeometry();
    
    virtual ~AxisGeometry();
    
    void setRenderMode( RenderMode renderMode );
    
private:
    
    void buildGeometry();

    bool _isXYZ;
};

#endif	/* AXISGEOMETRY_H */


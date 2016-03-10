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
    
    AxisGeometry();
    
    virtual ~AxisGeometry();
    
private:
    
    void buildGeometry();

};

#endif	/* AXISGEOMETRY_H */


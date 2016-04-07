/* 
 * File:   GamutGeometryUpdater.h
 * Author: allan
 *
 * Created on April 6, 2016, 9:47 PM
 */

#ifndef GAMUTGEOMETRYUPDATER_H
#define GAMUTGEOMETRYUPDATER_H

#include <osg/NodeVisitor>

#include "GamutGeometry.h"

class GamutGeometryUpdater : public osg::NodeVisitor
{
public:
    
    GamutGeometryUpdater();
    
    virtual ~GamutGeometryUpdater() {};    

    void apply( osg::Node& node );
    
    void setColorMode( GamutGeometry::ColorMode mode );
    
    void setRenderMode( GamutGeometry::RenderMode mode );
    
private:
    
    GamutGeometry::ColorMode _colorMode;
    
    GamutGeometry::RenderMode _renderMode;

};

#endif /* GAMUTGEOMETRYUPDATER_H */


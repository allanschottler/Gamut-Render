/* 
 * File:   GamutRenderModeVisitor.h
 * Author: allan
 *
 * Created on April 3, 2016, 11:53 AM
 */

#ifndef GAMUTRENDERMODEVISITOR_H
#define GAMUTRENDERMODEVISITOR_H

#include <osg/NodeVisitor>

#include "GamutGeometry.h"

class GamutRenderModeVisitor : public osg::NodeVisitor
{
public:
    
    GamutRenderModeVisitor( GamutGeometry::ColorMode renderMode );
    
    virtual ~GamutRenderModeVisitor() {};    

    void apply( osg::Node& node );
    
private:
    
    GamutGeometry::ColorMode _renderMode;

};

#endif /* GAMUTRENDERMODEVISITOR_H */


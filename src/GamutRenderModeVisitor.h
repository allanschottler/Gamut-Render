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
    
    GamutRenderModeVisitor( GamutGeometry::RenderMode renderMode );
    
    virtual ~GamutRenderModeVisitor() {};    

    void apply( osg::Node& node );
    
private:
    
    GamutGeometry::RenderMode _renderMode;

};

#endif /* GAMUTRENDERMODEVISITOR_H */


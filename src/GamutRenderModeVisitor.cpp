/* 
 * File:   GamutRenderModeVisitor.cpp
 * Author: allan
 * 
 * Created on April 3, 2016, 11:53 AM
 */

#include <osg/Geode>

#include "GamutRenderModeVisitor.h"
#include "GamutGeometry.h"

GamutRenderModeVisitor::GamutRenderModeVisitor( GamutGeometry::RenderMode renderMode ) : 
    osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ),
    _renderMode( renderMode )
{
}


void GamutRenderModeVisitor::apply( osg::Node& node )
{
    osg::ref_ptr< osg::Geode > geode = node.asGeode();
    
    if( geode )
    {
        const std::vector< osg::ref_ptr< osg::Drawable > > list = geode->getDrawableList();
        
        for( auto drawable : list )
        {
            osg::ref_ptr< GamutGeometry > gamutGeometry = osg::dynamic_pointer_cast< GamutGeometry >( drawable );
            
            if( gamutGeometry )
            {
                gamutGeometry->setRenderMode( _renderMode );
                
                return;
            }
        }
    }
    
    traverse( node );
}


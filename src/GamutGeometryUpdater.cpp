/* 
 * File:   GamutGeometryUpdater.cpp
 * Author: allan
 * 
 * Created on April 6, 2016, 9:47 PM
 */

#include "GamutGeometryUpdater.h"

#include <osg/Geode>

GamutGeometryUpdater::GamutGeometryUpdater() :
    osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ),    
    _colorMode( GamutGeometry::COLOR_NONE ),
    _renderMode( GamutGeometry::RENDER_NONE )
{
}

void GamutGeometryUpdater::apply( osg::Node& node )
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
                if( _renderMode != GamutGeometry::RENDER_NONE )
                    gamutGeometry->setRenderMode( _renderMode );
                
                if( _colorMode != GamutGeometry::COLOR_NONE )
                    gamutGeometry->setColorMode( _colorMode );
                
                return;
            }
        }
    }
    
    traverse( node );
}
    

void GamutGeometryUpdater::setColorMode( GamutGeometry::ColorMode mode )
{
    _colorMode = mode;
}
    

void GamutGeometryUpdater::setRenderMode( GamutGeometry::RenderMode mode )
{
    _renderMode = mode;
}

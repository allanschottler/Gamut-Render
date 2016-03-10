/* 
 * File:   AxisGeometry.cpp
 * Author: allanws
 * 
 * Created on March 10, 2016, 10:31 AM
 */

#include "AxisGeometry.h"

AxisGeometry::AxisGeometry() 
{
    buildGeometry();
}


AxisGeometry::~AxisGeometry() 
{
}


void AxisGeometry::buildGeometry()
{
    osg::ref_ptr< osg::Vec3Array > vertices = new osg::Vec3Array;
    osg::ref_ptr< osg::Vec4Array > colors = new osg::Vec4Array;
    
    osg::Vec3 o( 0.0f, 0.0f, 0.0f );
    osg::Vec3 xu( 1.0f, 0.0f, 0.0f );
    osg::Vec3 yu( 0.0f, 1.0f, 0.0f );
    osg::Vec3 zu( 0.0f, 0.0f, 1.0f );
    
    vertices->push_back( o );
    vertices->push_back( xu );
    vertices->push_back( yu );
    vertices->push_back( zu );
    
    setVertexArray( vertices );
    
    for( unsigned int i = 1; i < 4; i++ )
    {
        osg::ref_ptr< osg::DrawElementsUInt > primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 0 );
        
        primitiveSet->push_back( 0 );
        primitiveSet->push_back( i );

        addPrimitiveSet( primitiveSet );
    }
    
    colors->push_back( osg::Vec4( xu, 1.0 ) );
    colors->push_back( osg::Vec4( yu, 1.0 ) );
    colors->push_back( osg::Vec4( zu, 1.0 ) );
            
    setColorArray( colors );
    setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );
}

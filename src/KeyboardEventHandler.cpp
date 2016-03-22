/* 
 * File:   KeyboardEventHandler.cpp
 * Author: allanws
 * 
 * Created on March 22, 2016, 10:11 AM
 */

#include "KeyboardEventHandler.h"

#include "GamutGeometry.h"
#include "AxisGeometry.h"

KeyboardEventHandler::KeyboardEventHandler( osg::ref_ptr< osg::Geometry > geometry, osg::ref_ptr< osg::Geometry > axis ) :
    _geometry( geometry ),
    _axis( axis )
{
}


KeyboardEventHandler::~KeyboardEventHandler() 
{
}


bool KeyboardEventHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    switch( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::KEYDOWN:
        {            
            switch( ea.getKey() )
            {
                case '0':
                {
                    osg::ref_ptr< GamutGeometry > gamutGeometry = osg::dynamic_pointer_cast< GamutGeometry >( _geometry );
                    osg::ref_ptr< AxisGeometry > axisGeometry = osg::dynamic_pointer_cast< AxisGeometry >( _axis );
                    
                    if( gamutGeometry )
                        gamutGeometry->setRenderMode( GamutGeometry::RGB );
                    
                    if( axisGeometry )
                        axisGeometry->setRenderMode( AxisGeometry::RGB );
                    
                    
                    break;
                }
                case '1':
                {
                    osg::ref_ptr< GamutGeometry > gamutGeometry = osg::dynamic_pointer_cast< GamutGeometry >( _geometry );                    
                    osg::ref_ptr< AxisGeometry > axisGeometry = osg::dynamic_pointer_cast< AxisGeometry >( _axis );
                    
                    if( gamutGeometry )
                        gamutGeometry->setRenderMode( GamutGeometry::XYZ );
                    
                    if( axisGeometry )
                        axisGeometry->setRenderMode( AxisGeometry::XYZ );
                    
                    break;
                }
                
                default:
                    return false;
            }
        }
        
        default:
            return false;
    }
    
    return true;
}
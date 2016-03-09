/* 
 * File:   GamutApplication.cpp
 * Author: allanws
 * 
 * Created on March 9, 2016, 7:53 PM
 */

#include "GamutApplication.h"

#include <osgGA/TrackballManipulator>

GamutApplication* GamutApplication::_instance = 0;

GamutApplication::GamutApplication() 
{
    _viewer.setUpViewInWindow( 0, 0, 800, 800 );
        
    osg::ref_ptr< osgGA::TrackballManipulator > manipulator = new osgGA::TrackballManipulator();
    _viewer.setCameraManipulator( manipulator );
    _viewer.getCamera()->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
    
    _sceneNode = new osg::Group;
	
    _viewer.setSceneData( _sceneNode );    
}


GamutApplication::~GamutApplication() 
{
}


GamutApplication* GamutApplication::getInstance()
{
    if( !_instance )
        _instance = new GamutApplication();
    
    return _instance;
}


void GamutApplication::mainLoop()
{
    _viewer.realize();
    
    while( !_viewer.done() )
    {        
        _viewer.frame();
    }
}

/* 
 * File:   GamutApplication.cpp
 * Author: allanws
 * 
 * Created on March 9, 2016, 7:53 PM
 */

#include "GamutApplication.h"
#include "SpectrumsLoader.h"
#include "GamutGeometry.h"
#include "AxisGeometry.h"
#include "KeyboardEventHandler.h"

#include <osgGA/TrackballManipulator>
#include <osg/Point>

#include <iostream>

GamutApplication* GamutApplication::_instance = 0;

GamutApplication::GamutApplication() 
{
    _viewer.setUpViewInWindow( 0, 0, 800, 800 );
        
    osg::ref_ptr< osgGA::TrackballManipulator > manipulator = new osgGA::TrackballManipulator();
    
    osg::Vec3d eye, center, up, newEye( 0.0f, 0.0f, 1.0f );
    manipulator->getHomePosition( eye, center, up );    
    manipulator->setHomePosition( newEye, center, up );
    
    _viewer.setCameraManipulator( manipulator );
    _viewer.getCamera()->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
    _viewer.getCamera()->setCullingMode( 
        _viewer.getCamera()->getCullingMode() & ~osg::CullSettings::SMALL_FEATURE_CULLING );
    
    _sceneNode = new osg::Group;
	
    _viewer.setSceneData( _sceneNode );    
    
    initScene();
}


GamutApplication::~GamutApplication() 
{
}


void GamutApplication::initScene()
{
    GamutGeometry::RGBSpectrum* rgbSpectrum = new GamutGeometry::RGBSpectrum( 
        SpectrumsLoader::getCIERed(),
        SpectrumsLoader::getCIEGreen(),
        SpectrumsLoader::getCIEBlue() );
        
    osg::ref_ptr< osg::Geode > gamutGeode = new osg::Geode;
    osg::ref_ptr< osg::Geometry > gamutGeometry = new GamutGeometry( rgbSpectrum, SpectrumsLoader::getCIED65() );    
    osg::ref_ptr< osg::Geometry > axisGeometry = new AxisGeometry();  
    
    osg::ref_ptr< osg::Point > point = new osg::Point;
    point->setSize( 6 );
    
    gamutGeode->getOrCreateStateSet()->setAttribute( point, osg::StateAttribute::ON );
    gamutGeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF ); 

    gamutGeode->addDrawable( gamutGeometry );    
    gamutGeode->addDrawable( axisGeometry );
    
    _sceneNode->addChild( gamutGeode );
           
    _viewer.addEventHandler( new KeyboardEventHandler( gamutGeometry, axisGeometry ) );
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

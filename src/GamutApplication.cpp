/* 
 * File:   GamutApplication.cpp
 * Author: allanws
 * 
 * Created on March 9, 2016, 7:53 PM
 */

#include "GamutApplication.h"
#include "SpectrumsLoader.h"

#include <osgGA/TrackballManipulator>

#include <iostream>

#define SPECTRUM_FIRST_WL 380
#define SPECTRUM_LAST_WL 780
#define SPECTRUM_INC_WL 5

GamutApplication* GamutApplication::_instance = 0;

GamutApplication::GamutApplication() 
{
    _viewer.setUpViewInWindow( 0, 0, 800, 800 );
        
    osg::ref_ptr< osgGA::TrackballManipulator > manipulator = new osgGA::TrackballManipulator();
    _viewer.setCameraManipulator( manipulator );
    _viewer.getCamera()->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
    
    _sceneNode = new osg::Group;
	
    _viewer.setSceneData( _sceneNode );    
    
    /****************************************************/
    DiscreteSpectrum* d65 = SpectrumsLoader::getCIED65();    
    
    unsigned int iWavelength;
    for( iWavelength = SPECTRUM_FIRST_WL;
         iWavelength < SPECTRUM_LAST_WL;
         iWavelength += SPECTRUM_INC_WL )
    {
        float value = d65->valueOf( iWavelength );
        
        std::cout << value << std::endl;
    }
    /****************************************************/
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

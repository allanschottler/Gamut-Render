/* 
 * File:   GamutApplication.h
 * Author: allanws
 *
 * Created on March 9, 2016, 7:53 PM
 */

#ifndef GAMUTAPPLICATION_H
#define	GAMUTAPPLICATION_H

#include <osgViewer/Viewer>

#include "GamutMainWindow.h"

class GamutApplication 
{
public:
    
    static GamutApplication* getInstance();    
    
    virtual ~GamutApplication();    
    
    void mainLoop();
    
private:    
    
    GamutApplication();
    
    void initScene();
    
    
    static GamutApplication* _instance;
    
    //osgViewer::Viewer _viewer;
    
    GamutMainWindow* _window;
    
    osg::ref_ptr< osg::Group > _sceneNode;
};

#endif	/* GAMUTAPPLICATION_H */


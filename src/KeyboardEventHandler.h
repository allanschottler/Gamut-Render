/* 
 * File:   KeyboardEventHandler.h
 * Author: allanws
 *
 * Created on March 22, 2016, 10:11 AM
 */

#ifndef KEYBOARDEVENTHANDLER_H
#define	KEYBOARDEVENTHANDLER_H

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:
    
    KeyboardEventHandler( osg::ref_ptr< osg::Geometry > geometry, osg::ref_ptr< osg::Geometry > axis );
    
    virtual ~KeyboardEventHandler();
    
    bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );

private:

    osg::ref_ptr< osg::Geometry > _geometry, _axis;
};

#endif	/* KEYBOARDEVENTHANDLER_H */


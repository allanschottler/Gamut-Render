/* 
 * File:   GamutMainWindow.cpp
 * Author: allan
 * 
 * Created on April 2, 2016, 4:41 PM
 */

#include <string>
#include <iostream>

#include "GamutMainWindow.h"
#include "GamutRenderModeVisitor.h"
#include "GamutGeometryUpdater.h"

GamutMainWindow::GamutMainWindow() 
{
    GError* error = NULL;
    GtkBuilder* builder = gtk_builder_new();
    
    std::string gladePath( g_get_current_dir() );
    gladePath.append( "/data/glade/MainWindow.glade" );
    
    if( !gtk_builder_add_from_file( builder, gladePath.c_str(), &error ) )
    {
        g_warning( "Nao foi possivel abrir o arquivo: %s", error->message );
        g_error_free( error );
    }
    
    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );
    
    _rgbButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton1" ) );
    _xyzButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton2" ) );
    _xyyButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton8" ) );
    _labButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton3" ) );
    _srgbButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton4" ) );
    
    _pointsButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton5" ) );
    _linesButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton6" ) );
    _polygonButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton7" ) );

    GtkWidget* canvasBox = GTK_WIDGET( gtk_builder_get_object( builder, "canvasBox" ) );
    
    if( _canvas.createWidget( 800, 800 ) ) 
    {
        gtk_box_pack_start( GTK_BOX( canvasBox ), _canvas.getWidget(), true, true, 2 );        
    }
    
    g_timeout_add( 15, (GSourceFunc)( &GamutMainWindow::onIdle ), this );
    
    g_signal_connect( G_OBJECT( _dialog ), "destroy", G_CALLBACK( &GamutMainWindow::onDestroy ), NULL );
    g_signal_connect( G_OBJECT( _dialog ), "delete_event", G_CALLBACK( &GamutMainWindow::onDestroy ), NULL );
    
    g_signal_connect( G_OBJECT( _rgbButton ), "toggled", G_CALLBACK( &GamutMainWindow::onColorRadioToggle ), _dialog );
    g_signal_connect( G_OBJECT( _xyzButton ), "toggled", G_CALLBACK( &GamutMainWindow::onColorRadioToggle ), _dialog );
    g_signal_connect( G_OBJECT( _xyyButton ), "toggled", G_CALLBACK( &GamutMainWindow::onColorRadioToggle ), _dialog );
    g_signal_connect( G_OBJECT( _labButton ), "toggled", G_CALLBACK( &GamutMainWindow::onColorRadioToggle ), _dialog );
    g_signal_connect( G_OBJECT( _srgbButton ), "toggled", G_CALLBACK( &GamutMainWindow::onColorRadioToggle ), _dialog );
    
    g_signal_connect( G_OBJECT( _pointsButton ), "toggled", G_CALLBACK( &GamutMainWindow::onRenderRadioToggle ), _dialog );
    g_signal_connect( G_OBJECT( _linesButton ), "toggled", G_CALLBACK( &GamutMainWindow::onRenderRadioToggle ), _dialog );
    g_signal_connect( G_OBJECT( _polygonButton ), "toggled", G_CALLBACK( &GamutMainWindow::onRenderRadioToggle ), _dialog );
    
    g_object_set_data( G_OBJECT( _dialog ), "THIS", (gpointer)this );
    
    g_object_unref( G_OBJECT( builder ) );
}


GamutMainWindow::~GamutMainWindow() 
{
    g_object_unref( G_OBJECT( _dialog ) );
}


void GamutMainWindow::show()
{    
    gtk_widget_show_all( GTK_WIDGET( _dialog ) );
}


OSGGTKDrawingArea& GamutMainWindow::getCanvas()
{
    return _canvas;
}


gboolean GamutMainWindow::onIdle( gpointer pointer )
{
    GamutMainWindow* dialog = reinterpret_cast< GamutMainWindow* >( pointer );
    
    dialog->_canvas.queueDraw();
    
    return TRUE;
}


gboolean GamutMainWindow::onDestroy()
{
    gtk_main_quit();
    
    return FALSE;
}


gboolean GamutMainWindow::onColorRadioToggle( GtkWidget* widget, gpointer pointer )
{
    gpointer result = g_object_get_data( G_OBJECT( pointer ), "THIS" );

    if( !result ) 
        return FALSE;

    GamutMainWindow* dialog = reinterpret_cast< GamutMainWindow* >( result );
        
    typedef GamutGeometry::ColorMode Mode;
    Mode colorMode;      
        
    if( widget == dialog->_rgbButton )
    {  
        colorMode = GamutGeometry::RGB;
    }
    else if( widget == dialog->_xyzButton )
    {
        colorMode = GamutGeometry::XYZ;
    }
    else if( widget == dialog->_xyyButton )
    {
        colorMode = GamutGeometry::XYY;
    }
    else if( widget == dialog->_labButton )
    {
        colorMode = GamutGeometry::LAB;
    }
    else if( widget == dialog->_srgbButton )
    {
        colorMode = GamutGeometry::SRGB;
    }
    else
    {
        return FALSE;
    }
    
    GamutGeometryUpdater updater;
    updater.setColorMode( colorMode );
    
    osg::ref_ptr< osg::Node > scene = dialog->_canvas.getSceneData();

    if( scene )
        scene->accept( updater );

    return TRUE;
}


gboolean GamutMainWindow::onRenderRadioToggle( GtkWidget* widget, gpointer pointer )
{
    gpointer result = g_object_get_data( G_OBJECT( pointer ), "THIS" );

    if( !result ) 
        return FALSE;

    GamutMainWindow* dialog = reinterpret_cast< GamutMainWindow* >( result );
        
    typedef GamutGeometry::RenderMode Mode;
    Mode renderMode;      
        
    if( widget == dialog->_pointsButton )
    {  
        renderMode = GamutGeometry::POINTS;
    }
    else if( widget == dialog->_linesButton )
    {
        renderMode = GamutGeometry::LINES;
    }
    else if( widget == dialog->_polygonButton )
    {
        renderMode = GamutGeometry::POLYGON;
    }
    else
    {
        return FALSE;
    }
    
    GamutGeometryUpdater updater;
    updater.setRenderMode( renderMode );
    
    osg::ref_ptr< osg::Node > scene = dialog->_canvas.getSceneData();

    if( scene )
        scene->accept( updater );

    return TRUE;
}
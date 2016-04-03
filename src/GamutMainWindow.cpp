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
    _radioButton = GTK_WIDGET( gtk_builder_get_object( builder, "radiobutton1" ) );

    GtkWidget* canvasBox = GTK_WIDGET( gtk_builder_get_object( builder, "canvasBox" ) );
    
    if( _canvas.createWidget( 800, 800 ) ) 
    {
        gtk_box_pack_start( GTK_BOX( canvasBox ), _canvas.getWidget(), true, true, 2 );        
    }
    
    g_timeout_add( 15, (GSourceFunc)(&GamutMainWindow::onIdle), this );
    
    gtk_signal_connect( GTK_OBJECT( _dialog ), "destroy", GTK_SIGNAL_FUNC(&GamutMainWindow::onDestroy), NULL );
    gtk_signal_connect( GTK_OBJECT( _dialog ), "delete_event", GTK_SIGNAL_FUNC(&GamutMainWindow::onDestroy), NULL );
    gtk_signal_connect( GTK_OBJECT( _radioButton ), "toggled", GTK_SIGNAL_FUNC(&GamutMainWindow::onRadioToggle), _radioButton );
    
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


gboolean GamutMainWindow::onIdle()
{
    _canvas.queueDraw();
    
    return TRUE;
}


gboolean GamutMainWindow::onDestroy()
{
    gtk_main_quit();
    
    return FALSE;
}


gboolean GamutMainWindow::onRadioToggle( GtkWidget* widget, gpointer pointer )
{
    GSList* radioList = gtk_radio_button_get_group( GTK_RADIO_BUTTON( widget ) );
    unsigned int iRadio = 0;
    
    while( radioList )
    {
        GtkRadioButton* currentRadio = GTK_RADIO_BUTTON( radioList->data );
        
        bool isActive = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( currentRadio ) );
        
        if( isActive )
        {
            typedef GamutGeometry::RenderMode Mode;
            
            Mode renderMode = static_cast< Mode >( iRadio );
            GamutRenderModeVisitor visitor( renderMode );
            
            osg::ref_ptr< osg::Node > scene = _canvas.getSceneData();
            
            if( scene )
                scene->accept( visitor );
            
            return TRUE;
        }
        
        radioList = g_slist_next( radioList );
        iRadio++;
    }
    
    return FALSE;
}
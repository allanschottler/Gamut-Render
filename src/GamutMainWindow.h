/* 
 * File:   GamutMainWindow.h
 * Author: allan
 *
 * Created on April 2, 2016, 4:41 PM
 */

#ifndef GAMUTMAINWINDOW_H
#define GAMUTMAINWINDOW_H

#include <gtk/gtk.h>

#include "osggtkdrawingarea.h"

class GamutMainWindow 
{
public:
    
    GamutMainWindow();
    
    virtual ~GamutMainWindow();
    
    void show();
    
    OSGGTKDrawingArea& getCanvas();
    
private:

    static gboolean onIdle( gpointer user_data );
    
    static gboolean onDestroy();
    
    static gboolean onColorRadioToggle( GtkWidget* widget, gpointer pointer );
    
    static gboolean onRenderRadioToggle( GtkWidget* widget, gpointer pointer );
    
    GtkWidget* _dialog;
    
    GtkWidget* _rgbButton;
    GtkWidget* _xyzButton;
    GtkWidget* _labButton;
    GtkWidget* _srgbButton;
    
    GtkWidget* _pointsButton;
    GtkWidget* _linesButton;
    GtkWidget* _polygonButton;
    
    OSGGTKDrawingArea _canvas;
};

#endif /* GAMUTMAINWINDOW_H */


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

    gboolean onIdle();
    
    gboolean onDestroy();
    
    gboolean onRadioToggle( GtkWidget* widget, gpointer pointer );
    
    GtkWidget* _dialog;
    GtkWidget* _radioButton;
    
    OSGGTKDrawingArea _canvas;
};

#endif /* GAMUTMAINWINDOW_H */


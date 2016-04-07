#include <iostream>

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include "GamutApplication.h"

int main( int argc, char** argv ) 
{
    gtk_init( &argc, &argv );
    gtk_gl_init( &argc, &argv );
    
    GamutApplication::getInstance()->mainLoop();
    
    gtk_main();

    return 0;
}
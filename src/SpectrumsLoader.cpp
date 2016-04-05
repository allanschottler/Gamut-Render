/* 
 * File:   SpectrumsLoader.cpp
 * Author: allanws
 * 
 * Created on March 9, 2016, 9:16 PM
 */

#include <string>
#include <assert.h>
#include <glib-2.0/glib.h>
#include <fstream>

#include "SpectrumsLoader.h"

const std::string SpectrumsLoader::APP_PATH = std::string( g_get_current_dir() );
const std::string SpectrumsLoader::CIE_R_CHANNEL = "/data/cier.txt";
const std::string SpectrumsLoader::CIE_G_CHANNEL = "/data/cieg.txt";
const std::string SpectrumsLoader::CIE_B_CHANNEL = "/data/cieb.txt";
const std::string SpectrumsLoader::CIE_D65_ILLUM = "/data/d65.txt";

DiscreteSpectrum* SpectrumsLoader::load( const std::string& filePath )
{    
    std::fstream fs;
    fs.open( filePath.c_str(), std::ios::in );

    if( fs.fail() )  
        return 0;

    int initial, garbage, increment;
    float value;
    
    std::string line;
    
    fs >> initial;
    std::getline( fs, line );
    fs >> garbage;
    std::getline( fs, line );
    fs >> increment;
    
    DiscreteSpectrum* spectrum = new DiscreteSpectrum( initial, increment );
        
    while( std::getline( fs, line ) )
    {
        fs >> value;
        spectrum->push( value );
    }
    
    /*while( fscanf( fp, "%f", &value ) != EOF )
        spectrum->push( value );*/
    
    //fclose( fp );
    
    fs.close();
    
    //assert( spectrum->getFinalWavelength() == garbage );
    
    return spectrum;
}

DiscreteSpectrum* SpectrumsLoader::getCIERed()
{
    return load( APP_PATH + CIE_R_CHANNEL );
}
    
DiscreteSpectrum* SpectrumsLoader::getCIEGreen()
{
    return load( APP_PATH + CIE_G_CHANNEL );
}

DiscreteSpectrum* SpectrumsLoader::getCIEBlue()
{
    return load( APP_PATH + CIE_B_CHANNEL );
}

DiscreteSpectrum* SpectrumsLoader::getCIED65()
{
    return load( APP_PATH + CIE_D65_ILLUM );
}

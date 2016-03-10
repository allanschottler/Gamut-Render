/* 
 * File:   SpectrumsLoader.cpp
 * Author: allanws
 * 
 * Created on March 9, 2016, 9:16 PM
 */

#include <string>
#include <assert.h>

#include "SpectrumsLoader.h"

const std::string SpectrumsLoader::CIE_R_CHANNEL = "data/cier.txt";
const std::string SpectrumsLoader::CIE_G_CHANNEL = "data/cieg.txt";
const std::string SpectrumsLoader::CIE_B_CHANNEL = "data/cieb.txt";
const std::string SpectrumsLoader::CIE_D65_ILLUM = "data/d65.txt";

DiscreteSpectrum* SpectrumsLoader::load( const std::string& filePath )
{
    FILE* fp = fopen( filePath.c_str(), "r" );
    
    assert( fp );
    
    int initial, final, increment;
    float value;
    
    fscanf( fp, "%d", &initial );
    fscanf( fp, "%d", &final );
    fscanf( fp, "%d", &increment );
    
    DiscreteSpectrum* spectrum = new DiscreteSpectrum( initial, increment );
        
    while( fscanf( fp, "%f", &value ) != EOF )
        spectrum->push( value );
    
    fclose( fp );
    
    assert( spectrum->getFinalWavelength() == final );
    
    return spectrum;
}

DiscreteSpectrum* SpectrumsLoader::getCIERed()
{
    return load( CIE_R_CHANNEL );
}
    
DiscreteSpectrum* SpectrumsLoader::getCIEGreen()
{
    return load( CIE_G_CHANNEL );
}

DiscreteSpectrum* SpectrumsLoader::getCIEBlue()
{
    return load( CIE_B_CHANNEL );
}

DiscreteSpectrum* SpectrumsLoader::getCIED65()
{
    return load( CIE_D65_ILLUM );
}

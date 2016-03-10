/* 
 * File:   DiscreteSpectrum.cpp
 * Author: allanws
 * 
 * Created on March 9, 2016, 8:58 PM
 */

#include "DiscreteSpectrum.h"

DiscreteSpectrum::DiscreteSpectrum( int initial, int increment ) :
    _initial( initial ),
    _final( initial ),
    _increment( increment )
{
}


DiscreteSpectrum::~DiscreteSpectrum() 
{
}


void DiscreteSpectrum::push( float value )
{
    _values.push_back( value );
    
    _final = _initial + ( _values.size() - 1 ) * _increment;
}


float DiscreteSpectrum::valueOf( int wavelength )
{
    unsigned int index = ( wavelength - _initial ) / _increment;
    
    if( wavelength < _initial )
        index = 0;
    
    if( wavelength > _final )
        index = _values.size() - 1;
    
    return _values[ index ];        
}


int DiscreteSpectrum::getFinalWavelength()
{
    return _final;
}
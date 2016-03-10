/* 
 * File:   DiscreteSpectrum.h
 * Author: allanws
 *
 * Created on March 9, 2016, 8:58 PM
 */

#ifndef DISCRETESPECTRUM_H
#define	DISCRETESPECTRUM_H

#include <vector>

class DiscreteSpectrum 
{
public:
    
    DiscreteSpectrum( int initial, int increment );
    
    virtual ~DiscreteSpectrum();
    
    void push( float value );
    
    float valueOf( int wavelength );
    
    int getFinalWavelength();
    
private:
    
    int _initial, _final, _increment;
    
    std::vector< float > _values;
};

#endif	/* DISCRETESPECTRUM_H */


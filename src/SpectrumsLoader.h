/* 
 * File:   SpectrumsLoader.h
 * Author: allanws
 *
 * Created on March 9, 2016, 9:16 PM
 */

#ifndef SPECTRUMSLOADER_H
#define	SPECTRUMSLOADER_H

#include "DiscreteSpectrum.h"


class SpectrumsLoader 
{
public:
    
    static DiscreteSpectrum* getCIERed();
    
    static DiscreteSpectrum* getCIEGreen();
    
    static DiscreteSpectrum* getCIEBlue();
    
    static DiscreteSpectrum* getCIED65();
    
private:
    
    static const std::string CIE_R_CHANNEL;
    static const std::string CIE_G_CHANNEL;
    static const std::string CIE_B_CHANNEL;
    static const std::string CIE_D65_ILLUM;
    
    static DiscreteSpectrum* load( const std::string& filePath );

};

#endif	/* SPECTRUMSLOADER_H */


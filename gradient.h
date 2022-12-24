#pragma once
#include <Imagine/Graphics.h>
#include <iostream>
#include <math.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
using namespace Imagine;
using namespace std;

int grad_2pixels(int i1, int j1, byte* image1, int width1, int height1, int i2, int j2, byte* image2, int width2, int height2, int patch_size, int ordre_distance){

    // patch_size is an odd number
    if (patch_size%2 == 0) {
        std::ostringstream oss; oss << patch_size;
        throw std::out_of_range("Not a pair patch_size : "+oss.str());
    }

    // patch size is not bigger than the images
    else if (width1<patch_size || width2<patch_size || height1<patch_size || height2<patch_size){
        std::ostringstream oss; oss << patch_size;
        throw std::out_of_range("Too big patch_size : "+oss.str());
    }

    // i1, j1, i2, j2 are not too close of the border
    else if (i1<(patch_size/2) || i2<(patch_size/2) || j1<(patch_size/2) || j2<(patch_size/2) || i1>(height1-(patch_size/2)-1) || i2>(height2-(patch_size/2)-1) || j1>(width1-(patch_size/2)-1) || j2>(width2-(patch_size/2)-1)){
        std::ostringstream oss; oss << i1<<" "<<i2<<" "<<" "<<j1<<" "<<j2<<" "<<width1<< " "<<height1;
        throw std::out_of_range("Not valid positions : "+oss.str());
    }

    else{
        int distance=0;
        for (int i=(-patch_size/2); i<=(patch_size/2); i++){
            for (int j=(-patch_size/2); j<=(patch_size/2); j++){
                int im1=image1[(i1+i)*width1+(j1+j)];
                int im2=image2[(i2+i)*width2+(j2+j)];
                distance += abs(pow(im1-im2,ordre_distance));
            }
        }
        return (int)pow(distance,1.0/ordre_distance);
    }
}



int grad_2pixels_simple(int i1, int j1, byte* image1, int i2, int j2, byte* image2, int width){
    int im1=image1[i1*width+j1];
    int im2=image2[i2*width+j2];
    return abs(im1-im2);
}


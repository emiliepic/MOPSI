#include "tree.h"
#include "gradient.h"
#include "shortest_path.h"

#include <Imagine/Graphics.h>
#include <iostream>
#include <math.h>

using namespace Imagine;
using namespace std;

// The path and name of the town file
const char* image_left_path = srcPath("/horse_left.png");
const char* image_right_path = srcPath("/horse_right.png");




int main(int argc, char **argv) {

    //load image in black and white
    string image_left_file = (argc>1)? argv[1]: image_left_path;
    string image_right_file = (argc>1)? argv[1]: image_right_path;
    byte* image_left;
    byte* image_right;
    int width_r, height_r;
    int width_l, height_l;
    cout << "Loading image: " << image_right_file << endl;
    if(! loadGreyImage(image_right_file, image_right, width_r, height_r)) {
        cerr << "Error loading image file " << image_right_file << endl;
        return 1;
    }

    cout << "Loading image: " << image_left_file << endl;
    if(! loadGreyImage(image_left_file, image_left, width_l, height_l)) {
        cerr << "Error loading image file " << image_left_file << endl;
        return 1;
    }

    cout<<width_r<<endl<<width_l<<endl<<height_r<<endl<<height_l<<endl;
    int* association_table = new int [width_l];
    int* D = new int [width_l*width_l];
    for (int k=0; k<(width_l*width_l); k++){
        D[k]=-1;
    }
/*
    int width=7;
    int height=3;
    byte* im1= new byte[width*height];
    byte* im2= new byte[width*height];
    byte* im_dist= new byte[width*height];

    for (int i=0; i<height;i++){
        for (int j=0; j<width; j++){
            if (j<3){
                im1[i*width+j]=0;
                im2[i*width+j]=0;
            }
            else if (j==3){
                im1[i*width+j]=0;
                im2[i*width+j]=1;
            }
            else{
                im1[i*width+j]=1;
                im2[i*width+j]=1;
            }
        }
    }
    cout<<"1, 1 " <<grad_2pixels(1,1,im1, width,height,1,1,im2,width,height, 3,1)<<endl;
    cout<<"2, 1 " <<grad_2pixels(1,2,im1, width,height,1,1,im2,width,height, 3,1)<<endl;
    cout<<"3, 2 " <<grad_2pixels(1,3,im1, width,height,1,2,im2,width,height, 3,1)<<endl;
    cout<<"4, 3 " <<grad_2pixels(1,4,im1, width,height,1,3,im2,width,height, 3,1)<<endl;
    cout<<"5, 4 " <<grad_2pixels(1,5,im1, width,height,1,4,im2,width,height, 3,1)<<endl;
    for (int ligne=1; ligne<height-1;ligne++){
        //int sp = shortest_path_it_with_memoisation_simple(ligne, im1, im2, im_dist, width);
        int sp = shortest_path_it_with_memoisation(ligne, im1, im2, im_dist, width,height,3,1);

        if (ligne%10==0){
            cout<<"ligne : "<<ligne<<"  sp : "<<sp<<endl;
        }
    }

    Window window_1 = openWindow(width, height);
    putGreyImage(IntPoint2(0,0), im1, width, height);
    click();

    Window window_2 = openWindow(width, height);
    setActiveWindow(window_2);
    putGreyImage(IntPoint2(0,0), im2, width, height);
    click();

    Window window_dist = openWindow(width, height);
    setActiveWindow(window_dist);
    putGreyImage(IntPoint2(0,0), im_dist, width, height);
    click();
*/
    byte* image_dist = new byte [width_l*height_l];
    for (int ligne=1; ligne<height_l-1;ligne++){
        //int sp = shortest_path_it_with_memoisation_simple(ligne, image_left, image_right, image_dist, width_l);
        int sp = shortest_path_it_with_memoisation(ligne, image_left, image_right, image_dist, width_l,height_l,3,2);

        if (ligne%10==0){
            cout<<"ligne : "<<ligne<<"  sp : "<<sp<<endl;
        }
    }

    Window window_r = openWindow(width_r, height_r);
    putGreyImage(IntPoint2(0,0), image_right, width_r, height_r);
    click();

    Window window_l = openWindow(width_l, height_l);
    setActiveWindow(window_l);
    putGreyImage(IntPoint2(0,0), image_left, width_l, height_l);
    click();

    Window window_dist = openWindow(width_l, height_l);
    setActiveWindow(window_dist);
    putGreyImage(IntPoint2(0,0), image_dist, width_l, height_l);
    click();


    endGraphics();
    return 0;
}



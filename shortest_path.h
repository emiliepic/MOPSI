#pragma once
#include "gradient.h"



int shortest_path_rec_with_memoisation_simple(int ligne, int j1, int j2, byte* image1, byte* image2, int width, int* association_table, int* D){
    if (j1==0){
        D[j1*width]=grad_2pixels_simple(ligne, j1, image1, ligne, j2, image2, width);
        return D[j2*width];
    }
    else if (j1==width){
        if (D[j1-1]==-1){
            D[j1-1]=shortest_path_rec_with_memoisation_simple(ligne, j1-1, 0, image1, image2, width, association_table, D);
        }
        int min = D[j1-1];
        association_table[j1-1]=0;
        for (int j=1; j<width; j++){
            if (D[j*width+j1-1]==-1){
                D[j*width+j1-1]=shortest_path_rec_with_memoisation_simple(ligne, j1-1, j, image1, image2, width, association_table, D);
            }
            if (D[j*width+j1-1]<min){
                min=D[j*width+j1-1];
                association_table[j1-1]=j;
            }
        }
        return min;
    }
    else {
        int dist=grad_2pixels_simple(ligne, j1, image1, ligne, j2, image2, width);
        if (D[j1-1]==-1){
            D[j1-1]=shortest_path_rec_with_memoisation_simple(ligne, j1-1, 0, image1, image2, width, association_table, D);
        }
        int min=D[j1-1];
        association_table[j1-1]=0;
        for (int j=1; j<=j2; j++){
            if (D[j*width+j1-1]==-1){
                D[j*width+j1-1]=shortest_path_rec_with_memoisation_simple(ligne, j1-1, j, image1, image2, width, association_table, D);
            }
            if (D[j*width+j1-1]<min){
                min=D[j*width+j1-1];
                association_table[j1-1]=j;
            }
        }
        D[j2*width+j1-1]=min+dist;
        return min+dist;
    }
}



int shortest_path_it_with_memoisation_simple(int ligne, byte* image1, byte* image2, byte* image_dist, int width){
    int* association_table = new int [width];
    int* D = new int [width*width];
    /*
    for (int k=0; k<(width*width); k++){
        D[k]=-1;
    }
    */
    for (int j2=0; j2<width; j2++){
        D[j2*width]=grad_2pixels_simple(ligne, 0, image1, ligne, j2, image2, width);
        //cout <<"j1="<<0<<"  j2="<<j2<<"  dist="<<D[j2*width]<<"  D="<<D[j2*width]<<endl;

    }
    for (int j1=1; j1<width; j1++){
        for (int j2=0; j2<width; j2++){
            int dist=grad_2pixels_simple(ligne, j1, image1, ligne, j2, image2, width);
            int min=D[j1-1];
            for (int k=1; k<=j2; k++){
                if (min>D[k*width+j1-1]){
                    min=D[k*width+j1-1];
                }
            }
            D[j2*width+j1]=min+dist;
            //cout <<"j1="<<j1<<"  j2="<<j2<<" min="<<min<<"  dist="<<dist<<"  D="<<D[j2*width+j1]<<endl;
        }
    }
    int val_short_path=D[width-1];
    association_table[width-1]=0;
    for (int k=1; k<width; k++){
        if (val_short_path>D[k*width+width-1]){
            val_short_path=D[k*width+width-1];
            association_table[width-1]=k;
        }

    }
    //cout<<"val sp: "<<val_short_path<<endl;
    for (int k=width-2; k>=0; k--){
        int min = D[k];
        association_table[k]=0;
        for (int l=1; l<=association_table[k+1]; l++){
            if (D[l*width+k]<min){
                association_table[k]=l;
                min=D[l*width+k];
            }
        }
    }

    for (int k=0; k<width; k++){
        //cout<<association_table[k]<<endl;
        image_dist[ligne*width+k]=255-abs(k-association_table[k]);
    }
    return val_short_path;

}


int shortest_path_it_with_memoisation(int ligne, byte* image1, byte* image2, byte* image_dist, int width, int height, int patch_size, int ordre_distance){
    int* association_table = new int [width];
    int* D = new int [width*width];
    /*
    for (int k=0; k<(width*width); k++){
        D[k]=-1;
    }
    */
    for (int j2=patch_size/2; j2<width-(patch_size/2); j2++){
        D[j2*width+(patch_size/2)]=grad_2pixels(ligne, patch_size/2, image1, width, height, ligne, j2, image2, width, height, patch_size, ordre_distance);
        //cout <<"j1="<<patch_size/2<<"  j2="<<j2<<"  dist="<<D[j2*width+patch_size/2]<<"  D="<<D[j2*width+patch_size/2]<<endl;

    }
    for (int j1=(patch_size/2)+1; j1<width-(patch_size/2); j1++){
        for (int j2=patch_size/2; j2<width-(patch_size/2); j2++){
            int dist=grad_2pixels(ligne, j1, image1, width, height, ligne, j2, image2, width, height, patch_size, ordre_distance);
            int min=D[(patch_size/2)*width+j1-1];
            //cout<<min<<endl;
            for (int k=(patch_size/2)+1; k<=j2; k++){
                if (min>D[k*width+j1-1]){
                    min=D[k*width+j1-1];
                }
            }
            D[j2*width+j1]=min+dist;
            //cout <<"j1="<<j1<<"  j2="<<j2<<" min="<<min<<"  dist="<<dist<<"  D="<<D[j2*width+j1]<<endl;
        }
    }
    int val_short_path=D[width*(patch_size/2)+width-1-(patch_size/2)];
    association_table[width-(patch_size/2)-1]=patch_size/2;
    for (int k=1+(patch_size/2); k<width-(patch_size/2); k++){
        if (val_short_path>D[k*width+width-1-(patch_size/2)]){
            val_short_path=D[k*width+width-1-(patch_size/2)];
            association_table[width-(patch_size/2)-1]=k;
        }

    }
    //cout<<width-patch_size/2-1<<" "<<association_table[width-patch_size/2-1]<<endl;
    //cout<<"val sp: "<<val_short_path<<endl;
    for (int k=width-2-(patch_size/2); k>=patch_size/2; k--){
        int min = D[width*(patch_size/2)+k];
        //cout<<k<<" "<<patch_size/2<<" "<<D[width*(patch_size/2)+k]<<endl;
        association_table[k]=patch_size/2;
        for (int l=(patch_size/2)+1; l<=association_table[k+1]; l++){
            //cout<<k<<" "<<l<<" "<<D[l*width+k]<<endl;
            if (D[l*width+k]<min){
                association_table[k]=l;
                min=D[l*width+k];
            }
        }
        //cout <<"asso "<<k<<" "<<association_table[k]<<endl;
    }

    for (int k=patch_size/2; k<width-(patch_size/2); k++){
        //cout<<association_table[k]<<endl;
        image_dist[ligne*width+k]=255-abs(k-association_table[k]);
    }
    return val_short_path;

}




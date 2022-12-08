#include <Imagine/Graphics.h>
#include <iostream>
#include "quadtree/quadtree.h"

using namespace Imagine;
using namespace std;

// The path and name of the town file
const char* default_image_file=srcPath("/running-horse-rect.png");
    const char* lena=srcPath("/lena.png");
    const char* ppd=srcPath("/ppd.jpg");
    QuadLeaf<bool> Blanc(true);
QuadLeaf<bool> Noir(false);
QuadLeaf<byte> * leaves_greylevels_byte[256];






int x_leaf(int x_parent,int leaf, int taille_leaf){
    return ((leaf==NW || leaf==SW)? x_parent:x_parent+taille_leaf);
}





int y_leaf(int y_parent,int leaf, int taille_leaf){
    return ((leaf==NW || leaf==NE)? y_parent : y_parent+taille_leaf);
}



int calc_taille_virtual_image(int width, int height){
    int max=std::max(width,height);
    int new_size=pow(2,(int)log2(max));
    return (new_size==max? new_size:new_size*2);
}




QuadTree<bool
> * encoder_image_rec(byte* image, int width, int height, int taille_virtual_image, int x, int y, int taille_sousimage){
    if (x>=width || y>=height){
        return &Blanc;
    }
    if (taille_sousimage==1){
        return ((int)image[y*width+x]==0? &Noir:&Blanc);
    }
    else{
        int new_taille_sousimage = taille_sousimage/2;

        QuadTree<bool> *node;
        node = new QuadNode<bool>();
        for (int k=0;k<nQuadDir;k++){
            node->child(k)=encoder_image_rec(image, width, height, taille_virtual_image,x_leaf(x,k,new_taille_sousimage),y_leaf(y,k,new_taille_sousimage), new_taille_sousimage);
        }

        bool test=true; //stay true if all the children of node are leaves
        int k=0;
        bool val;
        while(test && k<nQuadDir){ //stops when all the children are the same leaves or when one children is not a leaf or when a leaf is different from the others
            if (!node->child(k)->isLeaf())
                test=false;
            else{
                bool leaf_value=node->child(k)->value();
                if (k==0)
                    val=leaf_value;
                else{
                    test=(val==leaf_value? true:false);
                }
            }
            k++;
        }
        if (test)
            return node->child(0);
        return node;
    }
}



QuadTree<bool> * encoder_image(byte* image, int width, int height){
    QuadLeaf<bool>::protect_leaves_from_destruction=true;
    int taille_virtual_image=calc_taille_virtual_image(width,height);
    return encoder_image_rec(image, width, height, taille_virtual_image, 0, 0, taille_virtual_image);
}

void decoder_image_rec(QuadTree<bool> * image_encodee, byte *image_decodee,int taille_image, int x, int y, int taille_sousimage){
    if (image_encodee->isLeaf()){
        int col_carre=(image_encodee->value()? 255:0);
        int col_contour=(image_encodee->value()? 0:255);

        for (int i=0;i<taille_sousimage;i++){
            for (int j=0;j<taille_sousimage;j++){
                image_decodee[(j+y)*taille_image+x+i]=((i==taille_sousimage-1 || j==taille_sousimage-1)? col_contour:col_carre);
            }
        }

    }
    else{
        int new_taille_sousimage = taille_sousimage/2;
        for (int k=0;k<nQuadDir;k++){
            decoder_image_rec(image_encodee->child(k),image_decodee,taille_image,x_leaf(x,k,new_taille_sousimage),y_leaf(y,k,new_taille_sousimage),new_taille_sousimage);
        }
    }
}

void decoder_image(QuadTree<bool> * image_encodee, byte *image_decodee,int taille_image){
    decoder_image_rec(image_encodee,image_decodee,taille_image,0,0,taille_image);
}



int taille_quadtree_rec(QuadTree<bool> * image_encodee){
    int taille=0;
    if (!image_encodee->isLeaf()){
        taille=sizeof(image_encodee);
        for (int k=0;k<nQuadDir;k++){
            taille+=taille_quadtree_rec(image_encodee->child(k));
        }
    }
    return taille;
}

int taille_quadtree(QuadTree<bool> * image_encodee){
    return taille_quadtree_rec(image_encodee)+sizeof(Blanc)+sizeof(Noir);
}


QuadTree<byte> * encoder_image_seuil_rec(int seuil, byte* image, int width, int height, int taille_virtual_image, int x, int y, int taille_sousimage){
    if (x>=width || y>=height){
        return leaves_greylevels_byte[255];
    }
    if (taille_sousimage==1){
        return leaves_greylevels_byte[(int)image[y*width+x]];
    }
    else{
        int new_taille_sousimage = taille_sousimage/2;

        QuadTree<byte> *node;
        node = new QuadNode<byte>();
        for (int k=0;k<nQuadDir;k++){
            node->child(k)=encoder_image_seuil_rec(seuil, image, width, height, taille_virtual_image,x_leaf(x,k,new_taille_sousimage),y_leaf(y,k,new_taille_sousimage), new_taille_sousimage);
        }

        bool test=true;
        int k=0, sum=0, min, max;
        while(test && k<nQuadDir){
            if (!node->child(k)->isLeaf())
                test=false;
            else{
                int leaf_value=node->child(k)->value();
                sum+=leaf_value;
                min=(k==0? leaf_value:std::min(leaf_value,min));
                max=(k==0? leaf_value:std::max(leaf_value,max));
            }
            k++;
        }
        if (test){
            if (max-min<=seuil)
                return leaves_greylevels_byte[sum/4];
        }
        return node;
    }
}


QuadTree<byte> * encoder_image_seuil(int seuil, byte* image, int width, int height){
    int taille_virtual_image=calc_taille_virtual_image(width,height);
    return encoder_image_seuil_rec(seuil, image, width, height, taille_virtual_image, 0, 0, taille_virtual_image);
}

void decoder_image_seuil_rec(QuadTree<byte> * image_encodee, byte *image_decodee,int taille_image, int x, int y, int taille_sousimage){
    if (image_encodee->isLeaf()){
        int col_carre=image_encodee->value();
        for (int i=0;i<taille_sousimage;i++){
            for (int j=0;j<taille_sousimage;j++){
                image_decodee[(j+y)*taille_image+x+i]=col_carre;
            }
        }

    }
    else{
        int new_taille_sousimage = taille_sousimage/2;
        for (int k=0;k<nQuadDir;k++){
            decoder_image_seuil_rec(image_encodee->child(k),image_decodee,taille_image,x_leaf(x,k,new_taille_sousimage),y_leaf(y,k,new_taille_sousimage),new_taille_sousimage);
        }
    }
}

void decoder_image_seuil(QuadTree<byte> * image_encodee, byte *image_decodee,int taille_image){
    decoder_image_seuil_rec(image_encodee,image_decodee,taille_image,0,0,taille_image);
}





int main(int argc, char **argv) {

    //Initialization of leaves_greylevels_byte[256]
    for (int k=0; k<256; k++){
        leaves_greylevels_byte[k]=new QuadLeaf<byte> (k);
    }

    //load image in black and white
    string image_file = (argc>1)? argv[1]: default_image_file;
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl; 
    if(! loadGreyImage(image_file, image, width, height)) {
        cerr << "Error loading image file " << image_file << endl;
        return 1;
    }

    // Print statistics
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;

    // Display image
    Window window = openWindow(width, height);
    putGreyImage(IntPoint2(0,0), image, width, height);
    click();

    //encoded image
    QuadTree<bool> * image_encodee =encoder_image(image,width,height);

    //compute virtual size
    int taille_virtual=calc_taille_virtual_image(width,height);
    byte * image_decodee= new byte [taille_virtual*taille_virtual];

    //decoded image
    Window window2 = openWindow(taille_virtual,taille_virtual);
    setActiveWindow(window2);
    decoder_image(image_encodee,image_decodee,taille_virtual);
    putGreyImage(IntPoint2(0,0), image_decodee, taille_virtual, taille_virtual);



    //Display the sizes
    int Taille_image_encodee=taille_quadtree(image_encodee);
    cout<<"la taille de l'image compressee est :"<<Taille_image_encodee<<endl;

    int Taille_image=width*height*sizeof(byte);
    cout << "la taille de l'image de reference est :" << Taille_image<<endl;

    cout << "le taux de compression est :" << (Taille_image_encodee/(float)Taille_image)*100 << "%" << endl;
    delete image_encodee;
    click();





    //Image in levels of grey

    //load image
    string image_file_lena = (argc>1)? argv[1]: lena;
    cout << "Loading image: " << image_file_lena << endl;
    if(! loadGreyImage(image_file_lena, image, width, height)) {
        cerr << "Error loading image file " << image_file_lena << endl;
        return 1;
    }

    //Display image
    Window window3 = openWindow(width, height);
    setActiveWindow(window3);
    putGreyImage(IntPoint2(0,0), image, width, height);
    click();


    // Encoded and decoded images, display
    Window window4 = openWindow(width, height);
    setActiveWindow(window4);
    byte *image_decodee_lena=new byte[width*height];

    QuadTree<byte> *image_encodee_lena;
    int seuils[4]={3,10,20,60};
    for (int k=0;k<4;k++){
        image_encodee_lena=encoder_image_seuil(seuils[k], image, width, height);
        decoder_image_seuil(image_encodee_lena,image_decodee_lena,width);
        putGreyImage(IntPoint2(0,0), image_decodee_lena, width, height);
        click();
    }


    //Image in colors

    //load image
    string image_file_color = (argc>1)? argv[1]: ppd;
    cout << "Loading image: " << image_file_color << endl;
    byte*r, *g, *b;
    if(! loadColorImage(image_file_color, r, g, b, width, height)) {
        cerr << "Error loading image file " << image_file_color << endl;
        return 1;
    }

    //Display image
    Window window5 = openWindow(width, height);
    setActiveWindow(window5);
    putColorImage(IntPoint2(0,0), r, g, b, width, height);
    click();

    // Encoded and decoded images, display
    Window window6 = openWindow(width, height);
    setActiveWindow(window6);
    byte *image_couleur_decodee_r=new byte[width*height], *image_couleur_decodee_g=new byte[width*height], *image_couleur_decodee_b=new byte[width*height];
    QuadTree<byte> *image_couleur_encodee_r, *image_couleur_encodee_g, *image_couleur_encodee_b;

    for (int k : seuils){
        image_couleur_encodee_r=encoder_image_seuil(k, r, width, height);
        image_couleur_encodee_g=encoder_image_seuil(k, g, width, height);
        image_couleur_encodee_b=encoder_image_seuil(k, b, width, height);
        decoder_image_seuil(image_couleur_encodee_r,image_couleur_decodee_r,width);
        decoder_image_seuil(image_couleur_encodee_g,image_couleur_decodee_g,width);
        decoder_image_seuil(image_couleur_encodee_b,image_couleur_decodee_b,width);
        putColorImage(IntPoint2(0,0), image_couleur_decodee_r,image_couleur_decodee_g,image_couleur_decodee_b, width, height);
        click();
    }


    endGraphics();
    return 0;
}

#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>

class Node{
    int n_children;
    int dist;
    int s;
    Node* children;
public :
    Node(){
        n_children=0;
        dist=0;
        s=0;
        children=nullptr;
    }

    Node(int N_Children, int Dist, int S, Node* Children){
        n_children = N_Children;
        children = new Node[n_children];
        dist = Dist;
        s = S;
        for (int k=0; k<n_children;k++){
            children[k]=Children[k];
        }
    }

    Node Leaf(int Dist, int S){
        return Node(0, Dist, S, nullptr);
    }

    int Get_n_children(){return n_children;}
    int Get_dist(){return dist;}
    int Get_s(){return s;}
    Node* Get_children(){return children;}
    Node* Get_child(int pos){
        if (!(0 <= pos && pos < n_children)) {
            std::ostringstream oss; oss << pos;
            throw std::out_of_range("Not a valid direction: "+oss.str());
        }
        return &children[pos];
    }

    void Set_n_children(int N_Children){n_children=N_Children;}
    void Set_dist(int Dist){ dist=Dist;}
    void Set_s(int S){ s=S;}
    void Set_children(int N_Children, Node* Children){
        n_children = N_Children;
        children = new Node[n_children];
        for (int k=0; k<n_children;k++){
            children[k]=Children[k];
        }
    }
    void Set_child(int pos, Node child){
        if (!(0 <= pos && pos < n_children)) {
            std::ostringstream oss; oss << pos;
            throw std::out_of_range("Not a valid direction: "+oss.str());
        }
        else{
            children[pos]=child;
        }
    }

};

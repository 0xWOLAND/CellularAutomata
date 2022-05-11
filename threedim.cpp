#include<stdio.h>
#include<iostream>
#include<math.h>
#include "threedim.h"
#include "square.h"
using namespace std;

Threedim::Threedim()
{

size=0;

  
}

Threedim::Threedim(int k){

  size=k;
  
}


int Threedim::getSize(){

  return size;
  
}

void Threedim::changeSquare(int a, int b, int c, float d, float e, float f){
 gol[a][b][c].setColor1(d);
 gol[a][b][c].setColor2(e);
 gol[a][b][c].setColor3(f);
  
}

void Threedim::iteration(){
  cout<<"iteration was called"<<endl;

  Square next [5][5][5];
  for(int q=0;q<5;q++){
    for(int r=0; r<5; r++){
      for(int s=0;s<5;s++){
        next[q][r][s]=gol[q][r][s];
      }
    }
  }

  for(int i=0;i<5;i++){
    for(int j=0; j<5; j++){
      for(int k=0;k<5;k++){
        
        int adj=0;
        for(int a=-1;a<=1;a++){
          for(int b=-1;b<=1;b++){
            if(gol[((i+a)%5+5)%5][((j+b)%5+5)%5][k].getColor1()!=0){
              int count=adj;
              adj++;
              if(k==4&&adj>count){
                //cout<<"("<<(i+a)%5<<", "<<(j+b)%5<<", "<<k<<")";
              }
              
            }
            
          }
        }

        if(gol[i][j][k].getColor1()>0){
          adj=adj-1;
        }
        
        if(adj<=1){
           next[i][j][k].setColor1(0);
        }

        if(adj==3){
          next[i][j][k].setColor1(1);
        }
        if(adj>3){
          next[i][j][k].setColor1(0);
        }
        

        //next[i][j][k].setColor1(adj);
        
        



        
      }
    }
  }
  
  for(int e=0;e<5;e++){
    for(int f=0;f<5;f++){
      for(int g=0;g<5;g++){
        gol[e][f][g]=next[e][f][g];
        
      }}}

  
  
}

void Threedim::display(){
  

  for(int c=0;c<5;c++){
    for(int b=0;b<5;b++){
      for(int a=0;a<5;a++){
        (gol[a][b][c]).printMe();
      }
      cout<<endl;
    }
    cout<<endl;
  
  }

cout<<"_______________"<<endl;
  
}
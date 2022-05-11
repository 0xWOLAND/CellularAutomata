#include<stdio.h>
#include<iostream>
#include<math.h>
#include "lifegrid.h"
#include "square.h"
using namespace std;

Lifegrid::Lifegrid()
{

size=0;

  
}

Lifegrid::Lifegrid(int k){

  size=k;
  
}


int Lifegrid::getSize(){

  return size;
  
}

void Lifegrid::changeSquare(int a, int b, int c){
  gol[a][b].setColor(c);

  
}

void Lifegrid::iteration(){

  Square next [20][20];
int j=getSize();
for(int c=0;c<j;c++){
    for(int d=0;d<j;d++){
next[c][d]=gol[c][d];
}
}

  for(int a=0;a<j;a++){
    for(int b=0;b<j;b++){

      int adj=0;
      if(gol[(a+1)%20][(b)%20].getColor()==1){
        adj++;
      }

       if(gol[(a+1)%20][(b+1)%20].getColor()==1){
        adj++;
      }

       if(gol[(a)%20][(b+1)%20].getColor()==1){
        adj++;
      }

       if(gol[(a)%20][(b-1)%20].getColor()==1){
        adj++;
      }

       if(gol[(a-1)%20][(b-1)%20].getColor()==1){
        adj++;
      }

       if(gol[(a-1)%20][(b)%20].getColor()==1){
        adj++;
      }

       if(gol[(a+1)%20][(b-1)%20].getColor()==1){
        adj++;
      }
      
        if(gol[(a-1)%20][(b+1)%20].getColor()==1){
        adj++;
      }
        //next[a][b].setColor(adj);
      
        if(adj<=1){
           next[a][b].setColor(0);
        }

        if(adj==3){
          next[a][b].setColor(1);
        }
        if(adj>3){
          next[a][b].setColor(0);
        }
    


      
      
    }
    
  }

for(int e=0;e<j;e++){
    for(int f=0;f<j;f++){
gol[e][f]=next[e][f];
}
}
  
}

void Lifegrid::display(){
  

  int j=getSize();
  for(int a=0;a<j;a++){
    for(int b=0;b<j;b++){
      
      (gol[a][b]).printMe();
    }
    cout<<endl;
  }


  
}
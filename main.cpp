
#include<stdio.h>
#include<iostream>
#include<math.h>
#include "lifegrid.h"
#include "threedim.h"
#include "square.h"
using namespace std;

int main() {
  std::cout << "Hello World!\n";
  Lifegrid* hi=new Lifegrid(20);
  
  hi->display();
  cout<<endl;
  cout<<endl;

  Threedim* bye=new Threedim(5);
  bye->display();
  
  /*
  for(int i=10;i<17;i++){
    hi->changeSquare(4,i,1);
  }
  hi->display();
  hi->iteration();
  cout<<endl;
  hi->display();
  hi->iteration();
  cout<<endl;
  hi->display();
  hi->iteration();
  cout<<endl;
  hi->display();
  hi->iteration();
  cout<<endl;
  hi->display();
  hi->iteration();
  cout<<endl;
  hi->display();
  
*/

float simple=3.55;
  cout<<simple<<endl;
 
    bye->changeSquare(2,3,0,1,0,0);
    bye->changeSquare(2,4,0,1,0,0);
    bye->changeSquare(2,2,0,1,0,0);
  bye->display();
  bye->iteration();

  bye->display();
  /*
  bye->iteration();
  bye->display();
*/




  
}


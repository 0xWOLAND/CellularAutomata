#ifndef Threedim_H
#define Threedim_H
#include<stdio.h>
#include<string.h>
#include<iostream>
#include "square.h"

class Threedim
{
public:
	int size;
  Square gol [5][5][5];
	
public:
	Threedim();
  Threedim(int);
	int getSize();
  void changeSquare(int,int,int, float, float, float);
  void iteration();
	void display();
  

};

#endif // GRID_H
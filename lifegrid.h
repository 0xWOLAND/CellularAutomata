#ifndef Lifegrid_H
#define Lifegrid_H
#include<stdio.h>
#include<string.h>
#include<iostream>
#include "square.h"

class Lifegrid
{
public:
	int size;
  Square gol [20][20];
	
public:
	Lifegrid();
  Lifegrid(int);
	int getSize();
  void changeSquare(int,int,int);
  void iteration();
	void display();
  

};

#endif // GRID_H

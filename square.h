#ifndef SQUARE_H
#define SQUARE_H
#include<stdio.h>
#include<string.h>
#include<iostream>

class Square
{


	
public:
 int color;
float color1;
float color2;
float color3;
	Square();
  Square(int);
	int getColor();
	void setColor(int);
  float getColor1();
	void setColor1(float);
float getColor2();
	void setColor2(float);
float getColor3();
	void setColor3(float);
	void printMe();

};

#endif // GRID_H

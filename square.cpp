#include<stdio.h>
#include<iostream>
#include<math.h>
#include"square.h"
using namespace std;

Square::Square()
{
  setColor(0);
}

Square::Square(int k)
{
  setColor(k);
}

int Square::getColor(){
  return color;
}

void Square::setColor(int h){
  color=h;
}

float Square::getColor1(){
  return color1;
}

void Square::setColor1(float i){
  color1=i;
}

float Square::getColor2(){
  return color2;
}

void Square::setColor2(float i){
  color2=i;
}

float Square::getColor3(){
  return color2;
}

void Square::setColor3(float i){
  color3=i;
}
  
void Square::printMe(){
  cout<<getColor1()<<" ";
}
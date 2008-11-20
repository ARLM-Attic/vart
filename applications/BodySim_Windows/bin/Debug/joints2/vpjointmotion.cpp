///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpjointmotion.cpp
//  DESCRIPTION.: Contain the VPJointMotion class definitions.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: June/06/2001
//  DESCRIPTION.: Methods definition.
//
///////////////////////////////////////////////////////////////////
#include	<vpvector3d.h>
#include	<stdio.h>
#include <vpjointmotion.h>

#include	<string>
using namespace std;


VPJointMotion::VPJointMotion(){

  jointName = "";
  motionType = FLEX;
  t0 = 0.0;
  tf = 0.0;
  parameter = 0.0;
  deltaS = 0.0;
}

VPJointMotion::VPJointMotion( string jn, int mt, float tini, float tfin, float param ){

  jointName = jn;
  motionType = mt;
  t0 = tini;
  tf = tfin;
  parameter = param;
  deltaS = 0.0;
}

VPJointMotion::~VPJointMotion(){
}

string
VPJointMotion::vpGetJointName(){
  return jointName;
}

int
VPJointMotion::vpGetMotionType(){
  return motionType;
}

float
VPJointMotion::vpGetTimeIni(){
  return t0;
}

float
VPJointMotion::vpGetTimeFin(){
  return tf;
}

float
VPJointMotion::vpGetParameter(){
  return parameter;
}

float
VPJointMotion::vpGetDeltaS(){
  return deltaS;
}

void
VPJointMotion::vpSetJointName( std::string s ){
  jointName = s;
}

void
VPJointMotion::vpSetMotionType( int mt ){
  motionType = mt;
}

void
VPJointMotion::vpSetTimeIni( float ti ){
  t0 = ti;
}

void
VPJointMotion::vpSetTimeFin( float tfin ){
  tf = tfin;
}

void
VPJointMotion::vpSetParameter( float param ){
  parameter = param;
}

void
VPJointMotion::vpSetDeltaS( float ds ){
  deltaS = ds;
}

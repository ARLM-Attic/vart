///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpmodifier.cpp
//  DESCRIPTION.: Contain the VPModifier class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 28/February/2001
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////

#include "vpmodifier.h"
#include "vpdof.h"
//#include	<stdio.h>

VPModifier::VPModifier(){
	dofList = NULL;
	maxPonderatorList = NULL;
	minPonderatorList = NULL;
	numDofs = 0;
}


VPModifier::VPModifier( VPDof **dofs, VPCurve *mins, VPCurve *maxs, int numD ){
	dofList = dofs;
	maxPonderatorList = maxs;
	minPonderatorList = mins;
	numDofs = numD;
}


VPModifier::~VPModifier(){
	
	if( dofList != NULL ) delete [] dofList;
	if( minPonderatorList != NULL ) delete [] minPonderatorList;
	if( maxPonderatorList != NULL ) delete [] maxPonderatorList;

}

VPCurve*
VPModifier::vpGetMinPonderatorList(){
	return minPonderatorList;
}
		
VPCurve*
VPModifier::vpGetMaxPonderatorList(){
	return maxPonderatorList;
}

float
VPModifier::vpGetMin(){

	float min = MINANG;
	for( int ind = 0; ind < numDofs; ind++ ){
		// Use getpointaspoit from bezier
		float aux = ( ((VPBezier*)&(minPonderatorList[ ind ]))->vpGetPointAsPoint( dofList[ind]->vpGetCurrent() ) ).vpGetY();
		if( aux > min ) min = aux;
	}
	return min;
}


float
VPModifier::vpGetMax(){

	float max = MAXANG;
	for( int ind = 0; ind < numDofs; ind++ ){
		// Use getpointaspoit from bezier
		float aux = ( ((VPBezier*)&(maxPonderatorList[ ind ]))->vpGetPointAsPoint( dofList[ind]->vpGetCurrent() ) ).vpGetY();
		if( aux < max ) max = aux;
	}
	return max;
}


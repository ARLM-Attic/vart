
//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vptable.cpp
//  DESCRIPTION.: Contain the VPTable class implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/19/2000
//  DESCRIPTION.: Implementation of class methods.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: June/05/2001
//  DESCRIPTION.: Implementation of new methods.
//
///////////////////////////////////////////////////////////////////

#include <vptable.h>
#include <cmath>


///////////////////////////////////////////////////////////////////
// Description: Class "VPTable" constructor without parameter. Sets
//				initial values for the attributes.
// Parameters.: -
// Return.....: -

VPTable::VPTable () {

	int i=0, r=0, g=0, b=0;


	/////////////////////
	// Set opacity table
	minVolumeDensity = 0;
	maxVolumeDensity = 255;


	/////////////////////////////////////////
	// Default control points initialization
	numControlPoints=0;
	controlPoints[numControlPoints] = 0;
	colorControlPoints[numControlPoints].vpSetColor(0,0,0,0);
	numControlPoints++;
	controlPoints[numControlPoints] = 25;
	colorControlPoints[numControlPoints].vpSetColor(0,0,0,0);
	numControlPoints++;
	controlPoints[numControlPoints] = 80;
	colorControlPoints[numControlPoints].vpSetColor(220,160,90,0);
	numControlPoints++;
	controlPoints[numControlPoints] = 210;
	colorControlPoints[numControlPoints].vpSetColor(255,0,0,0);
	numControlPoints++;
	controlPoints[numControlPoints] = 255;
	colorControlPoints[numControlPoints].vpSetColor(255,255,255,0);
	

	///////////////////////////////
	// Set a default opacity table
	for (i=0; i<DIM; i++) {
		if (i==controlPoints[0])
			opacity[i] = (float) 0;		
		else if (i<controlPoints[1])
			opacity[i] = ( 0.01 * i) / 25;
		else if (i<controlPoints[2])
			opacity[i] = ( (0.15-0.01) * (i-25) ) / (80-25) + 0.01;
		else if (i<controlPoints[3])
			opacity[i] = ( (0.95-0.15) * (i-80) ) / (210-80) + 0.15;
		else if (i<controlPoints[4]) 
			opacity[i] = ( (1.0-0.95) * (i-210) ) / (255-210) + 0.95;
		else 
			opacity[i] = (float) 1;
	} // for


	/////////////////////////////
	// Set a default color table
	for (i=0; i<DIM; i++) {

		if (i<controlPoints[1]) {
			colors[i].vpSetColor(0,0,0,0); // preto		
		}
		else if (i<controlPoints[2]) { 					
			r = ( (220-0) * (i-25) ) / (80-25) + 0;
			g = ( (160-0) * (i-25) ) / (80-25) + 0;
			b = ( (90-0) * (i-25) ) / (80-25) + 0;
			colors[i].vpSetColor(r,g,b,0); // bege 
		}
		else if (i<controlPoints[3]) {
			r = ( (255-220) * (i-80) ) / (210-80) + 220;
			g = ( (0-160) * (i-80) ) / (210-80) + 160;
			b = ( (0-90) * (i-80) ) / (210-80) + 90;
			colors[i].vpSetColor(r,g,b,0); // vermelho
		}
		else if (i<controlPoints[4]) {
			r = (int) ( (255-255) * (i-210) ) / (255-210) + 255;
			g = (int) ( (255-0) * (i-210) ) / (255-210) + 0;
			b = (int) ( (255-0) * (i-210) ) / (255-210) + 0;
			colors[i].vpSetColor(r,g,b,0);
		}
		else 
			colors[i].vpSetColor(255,255,255,0); // branco;

// bege ou rosa?? 255,192,203
// bege = 220 160 90
	}


/*	
	/////////////////////////////////////////
	// Default control points initialization
	numControlPoints=0;
	controlPoints[numControlPoints] = 0;
	numControlPoints++;
	controlPoints[numControlPoints] = 25;
	numControlPoints++;
	controlPoints[numControlPoints] = 90;
	numControlPoints++;
	controlPoints[numControlPoints] = 100;
	numControlPoints++;
	controlPoints[numControlPoints] = 110;
	numControlPoints++;
	controlPoints[numControlPoints] = 130;
	numControlPoints++;
	controlPoints[numControlPoints] = 140;
	numControlPoints++;
	controlPoints[numControlPoints] = 150;
	numControlPoints++;
	controlPoints[numControlPoints] = 160;
	numControlPoints++;
	controlPoints[numControlPoints] = 170;
	numControlPoints++;
	controlPoints[numControlPoints] = 180;
	numControlPoints++;
	controlPoints[numControlPoints] = 200;
	numControlPoints++;
	controlPoints[numControlPoints] = 255;
	

	///////////////////////////////
	// Set a default opacity table
	for (i=0; i<DIM; i++) {
		if (i==controlPoints[0])
			opacity[i] = (float) 0;		
		else if (i<controlPoints[1])
			opacity[i] = ( 0.1 * i) / 25;
		else if (i<controlPoints[2])
			opacity[i] = ( (0.28-0.1) * (i-25) ) / (90-25) + 0.1;
		else if (i<controlPoints[3])
			opacity[i] = ( (0.31-0.28) * (i-90) ) / (100-90) + 0.28;
		else if (i<controlPoints[4])
			opacity[i] = ( (0.37-0.31) * (i-100) ) / (110-100) + 0.31;
		else if (i<controlPoints[5])
			opacity[i] = ( (0.43-0.37) * (i-110) ) / (130-110) + 0.37;
		else if (i<controlPoints[6])
			opacity[i] = ( (0.49-0.43) * (i-130) ) / (140-130) + 0.43;
		else if (i<controlPoints[7])
			opacity[i] = ( (0.55-0.49) * (i-140) ) / (150-140) + 0.49;
		else if (i<controlPoints[8])
			opacity[i] = ( (0.62-0.55) * (i-150) ) / (160-150) + 0.55;
		else if (i<controlPoints[9])
			opacity[i] = ( (0.67-0.62) * (i-160) ) / (170-160) + 0.62;
		else if (i<controlPoints[10])
			opacity[i] = ( (0.73-0.67) * (i-170) ) / (180-170) + 0.67;
		else if (i<controlPoints[11])
			opacity[i] = ( (0.88-0.73) * (i-180) ) / (200-180) + 0.73;
		else if (i<controlPoints[12]) 
			opacity[i] = ( (0.95-0.88) * (i-200) ) / (255-200) + 0.88;
		else 
			opacity[i] = (float) 1;
*/
	



/*
		if ( (i>25) && (i<90) ) 
			opacity[i] = (float) 0.28;
		else if ( (i>=90) && (i<100) ) 
			opacity[i] = (float) 0.31;
		else if ( (i>=100) && (i<110) ) 
			opacity[i] = (float) 0.37;
		else if ( (i>=110) && (i<130) ) 
			opacity[i] = (float) 0.43;
		else if ( (i>=130) && (i<140) ) 
			opacity[i] = (float) 0.49;
		else if ( (i>=140) && (i<150) ) 
			opacity[i] = (float) 0.55;
		else if ( (i>=150) && (i<160) ) 
			opacity[i] = (float) 0.62;
		else if ( (i>=160) && (i<170) ) 
			opacity[i] = (float) 0.67;
		else if ( (i>=170) && (i<180) ) 
			opacity[i] = (float) 0.73;
		else if ( (i>=180) && (i<200) )
			opacity[i] = (float) 0.88;
		else if (i>=200)
			opacity[i] = (float) 0.94;
		else
			opacity[i] = (float) 0;
*/




/*
		if ( (i>20) && (i<50) ) 
			opacity[i] = (float) 0.2;
		else if ( (i>=50) && (i<90) ) 
			opacity[i] = (float) 0.3;
		else if ( (i>=100) && (i<130) ) 
			opacity[i] = (float) 0.3;
		else if ( (i>=130) && (i<160) ) 
			opacity[i] = (float) 0.4;
		else if ( (i>=130) && (i<140) ) 
			opacity[i] = (float) 0.49;
		else if ( (i>=180) && (i<190) ) 
			opacity[i] = (float) 0.7;
		else if ( (i>=190) && (i<200) ) 
			opacity[i] = (float) 0.75;
		else if ( (i>=200) && (i<210) ) 
			opacity[i] = (float) 0.8;
		else if ( (i>=210) && (i<220) )
			opacity[i] = (float) 0.85;
		else if ( (i>=220) && (i<230) )
			opacity[i] = (float) 0.9;
		else if (i>=230)
			opacity[i] = (float) 1.0;
		else
			opacity[i] = (float) 0;
*/


/*
		if ( (i>=190) && (i<215) ) 
			opacity[i] = (float) 0.8;
		else if ( (i>=216) && (i<230) ) 
			opacity[i] = (float) 0.9;
		else if ( (i>=231) && (i<243) ) 
			opacity[i] = (float) 0.95;
		else if ( i>=243 ) 
			opacity[i] = (float) 1.0;
		else
			opacity[i] = (float) 0;
*/	


/*
		if ( (i>=120) && (i<130) ) 
			opacity[i] = (float) 0.38;
		else if ( (i>=130) && (i<140) ) 
			opacity[i] = (float) 0.48;
		else if ( (i>=140) && (i<150) ) 
			opacity[i] = (float) 0.58;
		else if ( (i>=150) && (i<160) ) 
			opacity[i] = (float) 0.65;
		else if ( (i>=160) && (i<170) ) 
			opacity[i] = (float) 0.78;
		else if ( i>=170 ) 
			opacity[i] = (float) 0.98;
		else
			opacity[i] = (float) 0;
*/

/*
		if ( (i>=100) && (i<130) ) 
			opacity[i] = (float) 0.1;
		else if ( (i>=130) && (i<140) ) 
			opacity[i] = (float) 0.13;
		else if ( (i>=140) && (i<150) ) 
			opacity[i] = (float) 0.18;
		else if ( (i>=150) && (i<160) ) 
			opacity[i] = (float) 0.28;
		else if ( (i>=160) && (i<170) ) 
			opacity[i] = (float) 0.38;
		else if ( (i>=170) && (i<180) ) 
			opacity[i] = (float) 0.48;
		else if ( (i>=180) && (i<190) ) 
			opacity[i] = (float) 0.92;
		else if ( (i>=190) && (i<200) ) 
			opacity[i] = (float) 0.98;
		else if ( i>=200 ) 
			opacity[i] = (float) 1;
		else
			opacity[i] = (float) 0;
*/

/*
		if ( (i>20) && (i<80) ) 
			opacity[i] = (float) 0.25;
		else if ( (i>=80) && (i<90) ) 
			opacity[i] = (float) 0.31;
		else if ( (i>=90) && (i<100) ) 
			opacity[i] = (float) 0.36;
		else if ( (i>=110) && (i<120) ) 
			opacity[i] = (float) 0.42;
		else if ( (i>=120) && (i<130) ) 
			opacity[i] = (float) 0.47;
		else if ( (i>=130) && (i<140) ) 
			opacity[i] = (float) 0.54;
		else if ( (i>=140) && (i<150) ) 
			opacity[i] = (float) 0.62;
		else if ( (i>=150) && (i<160) ) 
			opacity[i] = (float) 0.67;
		else if ( (i>=160) && (i<170) ) 
			opacity[i] = (float) 0.73;
		else if ( i>=170 ) 
			opacity[i] = (float) 0.80;
		else
			opacity[i] = (float) 0;
*/

/*

		if ( (i>=170) && (i<200) )
			opacity[i] = (float) 0.7;
		else if ( (i>=200) && (i<215) )
			opacity[i] = (float) 0.82;
		else if ( (i>=215) && (i<230) )
			opacity[i] = (float) 0.89;
		else if ( (i>=230) && (i<242) )
			opacity[i] = (float) 0.94;
		else if ( i>=242 )
			opacity[i] = (float) 0.99;
		else
			opacity[i] = (float) 0;
*/

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetMinVolumeDensity" sends a new  
//				value to the minVolumeDensity attribute 
// Parameters.: int d
// Return.....: -

void VPTable::vpSetMinVolumeDensity(int d) {
	minVolumeDensity = d;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetMaxVolumeDensity" sends a new  
//				value to the maxVolumeDensity attribute 
// Parameters.: int d
// Return.....: -

void VPTable::vpSetMaxVolumeDensity(int d) {
	maxVolumeDensity = d;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpProcessMaxGradient" process the maximum
//				gradient in accordance with the min and max volume
//				density. 
// Parameters.: -
// Return.....: -

void VPTable::vpProcessMaxGradient() {
	maxGradient.x = maxGradient.y = 
	maxGradient.z = (maxVolumeDensity-minVolumeDensity) * sqrt(3);
	//maxGradient.vpNormalize(); // ?!?! Tem ou não que normalizar?
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetMaxGradient" sends a new value for 
//				the gradient. 
// Parameters.: VPVector3D g, maximum gradient
// Return.....: -

void  VPTable::vpSetMaxGradient(VPVector3D g) {
	maxGradient = g;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLinearOpacityValue" return the opacity  
//				value computed (proportional to the color received).
// Parameters.: unsigned int color
// Return.....: float opacity

float VPTable::vpGetLinearOpacityValue(float color) {

	if ( color < 40)
		return ( (float) 0);
	else
		return ((float) (color-40) / (float)(maxVolumeDensity-40) );

/* TESTE PARA O VOLUME "CTHEAD" COM NÍVEIS DE TRANSPARÊNCIA
	if ((color > 65) && (color < 85))
		return ( (float) 0.06); //color / 255 );
	else if (color > 190)
		return ( (float) 1.0);//color / 255 );
	else 
		return ( (float) 0);
*/

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetTableOpacityValue" return the opacity  
//				value in accordance with the opacity table 
//				(indirect method).
// Parameters.: unsigned int color
// Return.....: float opacity

float VPTable::vpGetTableOpacityValue(float color) {
	return ( opacity[int(color+0.5)] ); // round sample point
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetGradientOpacityValue" return the opacity  
//				value computed in accordance with the sample 
//				gradient.
// Parameters.: VPVector g
// Return.....: float opacity

float VPTable::vpGetGradientOpacityValue(int color, VPVector3D g){

// Obs.: Mais tarde já deixar calculado para poupar tempo


	// IMPLEMENTAÇÃO DE ACORDO COM A DISSERTAÇÃO DO MRMS
	if ( color < 40)
		return ( (float) 0);
	else
		return ( g.vpModule() / maxGradient.vpModule() );


/**** 
	// IMPLEMENTAÇÃO DE ACORDO COM O LIVRO VOLUME RENDERING (que não funcionou)
	float gradientMagnitude = g.vpModule();
	float maxGradientMagnitude = maxGradient.vpModule(); 
	return ( 1 - ( (maxVolumeDensity-color) / (maxGradientMagnitude*gradientMagnitude) ) );
****/

/**** 
// IMPLEMENTAÇÃO DE ACORDO COM O CÓDIGO DO MRMS

	float og = g.vpModule()/maxGradient.vpModule();
	float oi = (float) color / maxVolumeDensity;

//	if (color > 0)
//		printf("x");
//	if ( color < 40)
//		oi = 0;
//	else
//		oi = (float) color / maxVolumeDensity;

	return ( oi + 1.0 * (og - oi) );
****/
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetOpacityTable" sends new values for 
//				the opacity table.
// Parameters.: float t[]
// Return.....: -

void  VPTable::vpSetOpacityTable (float t[]) {
	for (int i=0; i<DIM; i++)
		opacity[i] = t[i];
} 


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetOpacityTable" return the opacity 
//				table.
// Parameters.: -
// Return.....: float opacity (vector with the opacity table)

float * VPTable::vpGetOpacityTable() {
	return opacity;
} 


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetColorTable" sends new values for 
//				the color table.
// Parameters.: VPColor c[]
// Return.....: -

void  VPTable::vpSetColorTable (VPColor c[]){
	for (int i=0; i<DIM; i++)
		colors[i].vpSetColor(c[i].vpGetR(),c[i].vpGetG(),c[i].vpGetB(),0);
} 


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetColorTable" return the color table.
// Parameters.: -
// Return.....: VPColor colors (vector with the color table)

VPColor * VPTable::vpGetColorTable(){
	return colors;
} 


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetControlPoints" sends a new value for 
//				the tables control points.
// Parameters.: int cp[], int n
// Return.....: -

void VPTable::vpSetControlPoints (int cp[], int n) {
	int i, j, auxR=0, auxG=0, auxB=0;
	numControlPoints = n;
	for (i=0; i<=numControlPoints; i++) {
		controlPoints[i] = cp[i];
		colorControlPoints[i].vpSetColor(colors[cp[i]].vpGetR(),colors[cp[i]].vpGetG(),colors[cp[i]].vpGetB(),0);
	}

	// Color table update in accordance with the new control points
	for (i=0; i<numControlPoints; i++) {
		colors[i].vpSetColor(colorControlPoints[i].vpGetR(), colorControlPoints[i].vpGetG(), colorControlPoints[i].vpGetB(), 0);
		for (j=controlPoints[i]+1; j<controlPoints[i+1]; j++) {
				auxR = ( ((colorControlPoints[i].vpGetR() - colorControlPoints[i+1].vpGetR())*(j-controlPoints[i+1])) / (controlPoints[i] - controlPoints[i+1]) ) + colorControlPoints[i+1].vpGetR();
				auxG = ( ((colorControlPoints[i].vpGetG() - colorControlPoints[i+1].vpGetG())*(j-controlPoints[i+1])) / (controlPoints[i] - controlPoints[i+1]) ) + colorControlPoints[i+1].vpGetG();
				auxB = ( ((colorControlPoints[i].vpGetB() - colorControlPoints[i+1].vpGetB())*(j-controlPoints[i+1])) / (controlPoints[i] - controlPoints[i+1]) ) + colorControlPoints[i+1].vpGetB();
				colors[j].vpSetColor(auxR, auxG, auxB, 0);
		}
	}

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetControlPoints" return the tables 
//				control points.
// Parameters.: -
// Return.....: int cp[], int &n

void VPTable::vpGetControlPoints(int cp[], int &n) {
	n = numControlPoints;
	for (int i=0; i<=numControlPoints; i++)
		cp[i] = controlPoints[i];

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetControlPoints" sends a new value for 
//				the tables control points.
// Parameters.: int cp[], VPColor c[], int n
// Return.....: -

void VPTable::vpSetControlPoints (int cp[], VPColor c[], int n) {
	numControlPoints = n;
	for (int i=0; i<=numControlPoints; i++) {
		controlPoints[i] = cp[i];
		colorControlPoints[i].vpSetColor(c[i].vpGetR(),c[i].vpGetG(),c[i].vpGetB(),0);
	}
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetControlPoints" return the tables 
//				control points.
// Parameters.: -
// Return.....: int cp[], VPColor c[], int &n

void VPTable::vpGetControlPoints(int cp[], VPColor c[], int &n) {
	n = numControlPoints;
	for (int i=0; i<=numControlPoints; i++) {
		cp[i] = controlPoints[i];
		c[i].vpSetColor(colorControlPoints[i].vpGetR(),colorControlPoints[i].vpGetG(),colorControlPoints[i].vpGetB(),0);	
	}
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetColor" return the color value  
//				in accordance with the color table.
// Parameters.: float color, interpolated color (density)
// Return.....: VPColor c, color (RGB)

VPColor VPTable::vpGetColor(float color) {
 VPColor c( colors[int(color+0.5)].vpGetR(), colors[int(color+0.5)].vpGetG(), 
			colors[int(color+0.5)].vpGetB(), colors[int(color+0.5)].vpGetI());
 return ( c ) ;
}

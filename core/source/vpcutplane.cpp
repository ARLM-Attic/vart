//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcutplane.cpp
//  DESCRIPTION.: Contain the VPCutPlane class implementation.
//
//  AUTHOR......: Fernando Sola Pereira
//  DATE........: January/23/2001
//  DESCRIPTION.: Implementation of class methods.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: March/01/2001
//  DESCRIPTION.: File and variables rename.
//
///////////////////////////////////////////////////////////////////


#include <vpcutplane.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPCutPlane" constructor without parameter.
// Parameters.: -
// Return.....: -

VPCutPlane::VPCutPlane() {

}


///////////////////////////////////////////////////////////////////
// Description: Class "VPCutPlane" constructor with parameter.
// Parameters.: VPBoundingBox b (bounding box of the volume that
//								 will be cut)
// Return.....: -

VPCutPlane::VPCutPlane(VPBoundingBox b) {
	boundingBox = b;
	vpPlaneComputation();
}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: 
// Return.....: -					

void VPCutPlane::vpSetBoundingBox(VPBoundingBox b) {
	boundingBox = b;
	vpPlaneComputation();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetNormal" returns the value of "normal"
//				attribute.
// Parameters.: -
// Return.....: VPVector3D normal

VPVector3D VPCutPlane::vpGetNormal(void) {
	return normal;
}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: 
// Return.....: 

void VPCutPlane::vpPlaneComputation(void) {
	float maiorX,maiorY,maiorZ,menorX,menorY,menorZ,aux;
	float cx,cy,cz;

	cx = boundingBox.vpGetCenter()->vpGetX();
	cy = boundingBox.vpGetCenter()->vpGetY();
	cz = boundingBox.vpGetCenter()->vpGetZ();

	maiorX = boundingBox.vpGetGreaterX();
	maiorY = boundingBox.vpGetGreaterY();
	maiorZ = boundingBox.vpGetGreaterZ();
	menorX = boundingBox.vpGetSmallerX();
	menorY = boundingBox.vpGetSmallerY();
	menorZ = boundingBox.vpGetSmallerZ();

	if((maiorX)>(maiorY))
	{
		if((maiorX)>(maiorZ))
			aux = maiorX - cx;
		else
			aux = maiorZ - cz;
	}
	else
	{
		if((maiorY)>(maiorZ))
			aux = maiorY - cy;
		else
			aux = maiorZ - cz;
	}

	plane[0] = VPPoint3D(cx-aux,cy,cz-aux);
	plane[1] = VPPoint3D(cx+aux,cy,cz-aux);
	plane[2] = VPPoint3D(cx+aux,cy+(aux/2),cz+aux);
	plane[3] = VPPoint3D(cx-aux,cy+(aux/2),cz+aux);

	vpNormalComputation
		    (plane[0].vpGetX(),plane[0].vpGetY(),plane[0].vpGetZ(),
			 plane[1].vpGetX(),plane[1].vpGetY(),plane[1].vpGetZ(),
			 plane[2].vpGetX(),plane[2].vpGetY(),plane[2].vpGetZ());
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpNormalComputation" process the normal 
//				attribute value.
// Parameters.: 
// Return.....: -

void VPCutPlane::vpNormalComputation(float x1,float y1,float z1,
			                    float x2,float y2,float z2,
				                float x3,float y3,float z3){

	float vax, vay, vaz, vbx, vby, vbz, nx, ny, nz;
	float modulo;

	// normal vector computation: N=(v2-v1)x(v3-v1)

	vax = x2-x1; //vector A va = (v2-v1)
	vay = y2-y1;
	vaz = z2-z1;

	vbx = x3-x1; //vector B vb = (v3-v1)
	vby = y3-y1;
	vbz = z3-z1;

	nx = (vay*vbz) - (vaz*vby);
	ny = (vaz*vbx) - (vax*vbz);
	nz = (vax*vby) - (vay*vbx);

	modulo = (float) sqrt(nx*nx + ny*ny + nz*nz);
	
	nx = nx / modulo;
	ny = ny / modulo;
	nz = nz / modulo;

	normal.vpSetVector3D(nx,ny,nz);
}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: 
// Return.....: 

VPRotation* VPCutPlane::vpGetRotation(void){
	return &rotation;
}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: 
// Return.....: 

VPTranslation* VPCutPlane::vpGetTranslation(void){
	return &translation;
}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: 
// Return.....: 
void VPCutPlane::vpRender(void){

	glPushMatrix();
		glTranslatef(translation.vpGetPositionX(),0.0,0.0);
		glTranslatef(0.0,-translation.vpGetPositionY(),0.0);
		
		glPushMatrix();
			glRotatef(rotation.vpGetAngleX(),0.0,1.0,0.0);
			glRotatef(rotation.vpGetAngleY(),1.0,0.0,0.0);

			glPushMatrix();
				glBegin(GL_QUADS);
					glColor3ub(150,200,200);
					glNormal3f(normal.vpGetVector3DX(),normal.vpGetVector3DY(),normal.vpGetVector3DZ());
					glVertex3f(plane[0].vpGetX(),plane[0].vpGetY(),plane[0].vpGetZ());
					glVertex3f(plane[1].vpGetX(),plane[1].vpGetY(),plane[1].vpGetZ());
					glVertex3f(plane[2].vpGetX(),plane[2].vpGetY(),plane[2].vpGetZ());
					glVertex3f(plane[3].vpGetX(),plane[3].vpGetY(),plane[3].vpGetZ());
				glEnd();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: 
// Return.....: 

void VPCutPlane::vpDComputation(void){
	float nx,ny,nz;

	nx = normal.vpGetVector3DX();
	ny = normal.vpGetVector3DY();
	nz = normal.vpGetVector3DZ();

	nD = -(nx*plane[0].vpGetX() + ny*plane[0].vpGetY() + nz*plane[0].vpGetZ());
}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: 
// Return.....: 

float VPCutPlane::vpGetD(){
	return nD;
}


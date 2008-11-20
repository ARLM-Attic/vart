//deprecated
#include    <stdio.h>
#include    <math.h>
#include    <vpmatrix.h>
#include    <vppoint.h>


VPMatrix::VPMatrix( void ){

	// set the identity matrix
	data[0][1] = data[0][2] = data[0][3] =
	data[1][0] = data[1][2] = data[1][3] =
	data[2][0] = data[2][1] = data[2][3] =
	data[3][0] = data[3][1] = data[3][2] = 0.0;
	data[0][0] = data[1][1] = data[2][2] = data[3][3] = 1.0;

}

VPMatrix::VPMatrix( float fLines[] ){

	for( int l = 0; l < 4; l++ ){
		for( int c = 0; c < 4; c++ ){
			data[l][c] = fLines[l*4+c];
		}
	}
}

VPMatrix::VPMatrix( float fm44[][4] ){

        for( int l = 0; l < 4; l++ ){
        	for( int c = 0; c < 4; c++ ){
			data[l][c] = fm44[l][c];
        	}
        }
}


VPMatrix::~VPMatrix( void ){
	// do nothing
}

float
VPMatrix::vpGetValueAt( int iL, int iC ){

        return data[iL][iC];
}

void
VPMatrix::vpSetValueAt( int iL, int iC, float fValue ){

        data[iL][iC] = fValue;
}


/*mat44*
VPMatrix::vpGetMatrixF( void ){

	float **M;
	M = new float*[4];

  for( int l = 0; l < 4; l++ ){
  	M[l] = new float[4];
  	for( int c = 0; c < 4; c++ ){
    	M[l][c] = data[c][l];
    }
  }
  return (mat44*)M;

}*/

void
VPMatrix::vpGetMatrixF( float **M ){
	
  for( int l = 0; l < 4; l++ ){
  	for( int c = 0; c < 4; c++ ){
    	M[l][c] = data[c][l];
    }
  }


}

float*
VPMatrix::vpGetMatrixVF( void ){

	float *M = new float[16];

        for( int l = 0; l < 4; l++ ){
        	for( int c = 0; c < 4; c++ ){
	        	M[l*4+c] = data[l][c];
                }
        }
        return M;
}


// Calculate inverse matrix
//
//   -1

//  A  =____1__ adjoint A
//       det A
//
VPMatrix*
VPMatrix :: vpGetInverse( void ){

	VPMatrix *Inv;
	float fDet;

	// calculate the adjoint matrix
	Inv = vpGetAdjoint();

	// calculate the 4x4 determinent
	// if the determinent is zero, then the inverse matrix is not unique
	fDet = Inv->vpGetDeterminent();
	if (fabs(fDet) < 0.0001){
		printf("Non-singular matrix, no inverse!");
                return Inv;
        }
	// scale the adjoint matrix to get the inverse
        Inv->vpMultiplyScalar( 1/fDet );

	return Inv;
}


// calculate the adjoint of a 4x4 matrix
//
// Let a  denote the minor determinant of matrix A obtained by
//     ij
// deleting the ith row and jth column from A.
//             i+j
// let b  = (-1)   a
//     ij          ji
// The matrix B = (b  ) is the adjoint of A
//                  ij

VPMatrix*
VPMatrix::vpGetAdjoint(){

        VPMatrix	*A = new VPMatrix();
        float		a1, a2, a3, a4, b1, b2, b3, b4,
        		c1, c2, c3, c4, d1, d2, d3, d4;

        // Assign to individual variable names to aid selecting correct values
        a1 = data[0][0]; b1 = data[0][1];
        c1 = data[0][2]; d1 = data[0][3];

        a2 = data[1][0]; b2 = data[1][1];
        c2 = data[1][2]; d2 = data[1][3];

        a3 = data[2][0]; b3 = data[2][1];
        c3 = data[2][2]; d3 = data[2][3];

        a4 = data[3][0]; b4 = data[3][1];
        c4 = data[3][2]; d4 = data[3][3];

        // Row column labeling reversed since we transpose rows & columns
        A->vpSetValueAt( 0, 0,  vpGetDet33(b2, b3, b4, c2, c3, c4, d2, d3, d4) );
        A->vpSetValueAt( 1, 0, -vpGetDet33(a2, a3, a4, c2, c3, c4, d2, d3, d4) );
        A->vpSetValueAt( 2, 0,  vpGetDet33(a2, a3, a4, b2, b3, b4, d2, d3, d4) );
        A->vpSetValueAt( 3, 0, -vpGetDet33(a2, a3, a4, b2, b3, b4, c2, c3, c4) );

        A->vpSetValueAt( 0, 1, -vpGetDet33(b1, b3, b4, c1, c3, c4, d1, d3, d4) );
        A->vpSetValueAt( 1, 1,  vpGetDet33(a1, a3, a4, c1, c3, c4, d1, d3, d4) );
        A->vpSetValueAt( 2, 1, -vpGetDet33(a1, a3, a4, b1, b3, b4, d1, d3, d4) );
        A->vpSetValueAt( 3, 1,  vpGetDet33(a1, a3, a4, b1, b3, b4, c1, c3, c4) );

        A->vpSetValueAt( 0, 2,  vpGetDet33(b1, b2, b4, c1, c2, c4, d1, d2, d4) );
        A->vpSetValueAt( 1, 2, -vpGetDet33(a1, a2, a4, c1, c2, c4, d1, d2, d4) );
        A->vpSetValueAt( 2, 2,  vpGetDet33(a1, a2, a4, b1, b2, b4, d1, d2, d4) );
        A->vpSetValueAt( 3, 2, -vpGetDet33(a1, a2, a4, b1, b2, b4, c1, c2, c4) );

        A->vpSetValueAt( 0, 3, -vpGetDet33(b1, b2, b3, c1, c2, c3, d1, d2, d3) );
        A->vpSetValueAt( 1, 3,  vpGetDet33(a1, a2, a3, c1, c2, c3, d1, d2, d3) );
        A->vpSetValueAt( 2, 3, -vpGetDet33(a1, a2, a3, b1, b2, b3, d1, d2, d3) );
        A->vpSetValueAt( 3, 3,  vpGetDet33(a1, a2, a3, b1, b2, b3, c1, c2, c3) );

        return A;
}


// calculate the determinent of a 4x4 matrix
float
VPMatrix::vpGetDeterminent( void ){

	float ans, a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;


        // assign to individual variable names to aid selecting correct elements
        a1 = data[0][0]; b1 = data[0][1];
        c1 = data[0][2]; d1 = data[0][3];

        a2 = data[1][0]; b2 = data[1][1];
        c2 = data[1][2]; d2 = data[1][3];

        a3 = data[2][0]; b3 = data[2][1];
        c3 = data[2][2]; d3 = data[2][3];

        a4 = data[3][0]; b4 = data[3][1];
        c4 = data[3][2]; d4 = data[3][3];

        ans =	a1 * vpGetDet33(b2, b3, b4, c2, c3, c4, d2, d3, d4) -
		b1 * vpGetDet33(a2, a3, a4, c2, c3, c4, d2, d3, d4) +
                c1 * vpGetDet33(a2, a3, a4, b2, b3, b4, d2, d3, d4) -
                d1 * vpGetDet33(a2, a3, a4, b2, b3, b4, c2, c3, c4);

        return ans;
}

//
// calcule the determinent of a 3x3 matrix in the form
//
//	| a1, b1, c1 |
//	| a2, b2, c2 |
//	| a3, b3, c3 |
//
float
VPMatrix::vpGetDet33(float a1, float a2, float a3, float b1, float b2, float b3,
float c1, float c2, float c3){

        float ans;
        float aux1, aux2, aux3;

        aux1 = vpGetDet22(b2, b3, c2, c3);
        aux2 = vpGetDet22(a2, a3, c2, c3);
        aux3 = vpGetDet22(a2, a3, b2, b3);
        ans = a1 * aux1 - b1 * aux2 + c1 * aux3;

        return ans;
}


//
// calculate the determinent of a 2x2 matrix
//
float
VPMatrix::vpGetDet22(float a, float b, float c, float d){

	return (a * d - b * c);
}


///////////////////////////////////////////////////////////////
// Return the result of x.T
//
VPMatrix*
VPMatrix::vpMultiplyScalar( float x ){

        VPMatrix *M = new VPMatrix();

        for( int l = 0; l < 4; l++ ){
        	for( int c = 0; c < 4; c++ ){
	        	M->vpSetValueAt( l, c, x * data[l][c] );
                }
        }
        return M;
}

///////////////////////////////////////////////////////////////
// Return the result of T.B
//
VPMatrix*
VPMatrix::vpMultiply( VPMatrix B ){

	VPMatrix *M = new VPMatrix();

	M->vpSetValueAt( 0, 0,	data[0][0]*B.vpGetValueAt( 0, 0 )+
        			data[0][1]*B.vpGetValueAt( 1, 0 )+
        			data[0][2]*B.vpGetValueAt( 2, 0 )+
        			data[0][3]*B.vpGetValueAt( 3, 0 ) );
	M->vpSetValueAt( 0, 1,	data[0][0]*B.vpGetValueAt( 0, 1 )+
        			data[0][1]*B.vpGetValueAt( 1, 1 )+
        			data[0][2]*B.vpGetValueAt( 2, 1 )+
        			data[0][3]*B.vpGetValueAt( 3, 1 ) );
	M->vpSetValueAt( 0, 2,	data[0][0]*B.vpGetValueAt( 0, 2 )+
        			data[0][1]*B.vpGetValueAt( 1, 2 )+
        			data[0][2]*B.vpGetValueAt( 2, 2 )+
        			data[0][3]*B.vpGetValueAt( 3, 2 ) );
	M->vpSetValueAt( 0, 3,	data[0][0]*B.vpGetValueAt( 0, 3 )+
        			data[0][1]*B.vpGetValueAt( 1, 3 )+
        			data[0][2]*B.vpGetValueAt( 2, 3 )+
        			data[0][3]*B.vpGetValueAt( 3, 3 ) );

	M->vpSetValueAt( 1, 0,	data[1][0]*B.vpGetValueAt( 0, 0 )+
        			data[1][1]*B.vpGetValueAt( 1, 0 )+
        			data[1][2]*B.vpGetValueAt( 2, 0 )+
        			data[1][3]*B.vpGetValueAt( 3, 0 ) );
	M->vpSetValueAt( 1, 1,	data[1][0]*B.vpGetValueAt( 0, 1 )+
        			data[1][1]*B.vpGetValueAt( 1, 1 )+
        			data[1][2]*B.vpGetValueAt( 2, 1 )+
        			data[1][3]*B.vpGetValueAt( 3, 1 ) );
	M->vpSetValueAt( 1, 2,	data[1][0]*B.vpGetValueAt( 0, 2 )+
        			data[1][1]*B.vpGetValueAt( 1, 2 )+
        			data[1][2]*B.vpGetValueAt( 2, 2 )+
        			data[1][3]*B.vpGetValueAt( 3, 2 ) );
	M->vpSetValueAt( 1, 3,	data[1][0]*B.vpGetValueAt( 0, 3 )+
        			data[1][1]*B.vpGetValueAt( 1, 3 )+
        			data[1][2]*B.vpGetValueAt( 2, 3 )+
        			data[1][3]*B.vpGetValueAt( 3, 3 ) );

	M->vpSetValueAt( 2, 0,	data[2][0]*B.vpGetValueAt( 0, 0 )+
        			data[2][1]*B.vpGetValueAt( 1, 0 )+
        			data[2][2]*B.vpGetValueAt( 2, 0 )+
        			data[2][3]*B.vpGetValueAt( 3, 0 ) );
	M->vpSetValueAt( 2, 1,	data[2][0]*B.vpGetValueAt( 0, 1 )+
        			data[2][1]*B.vpGetValueAt( 1, 1 )+
        			data[2][2]*B.vpGetValueAt( 2, 1 )+
        			data[2][3]*B.vpGetValueAt( 3, 1 ) );
	M->vpSetValueAt( 2, 2,	data[2][0]*B.vpGetValueAt( 0, 2 )+
        			data[2][1]*B.vpGetValueAt( 1, 2 )+
        			data[2][2]*B.vpGetValueAt( 2, 2 )+
        			data[2][3]*B.vpGetValueAt( 3, 2 ) );
	M->vpSetValueAt( 2, 3,	data[2][0]*B.vpGetValueAt( 0, 3 )+
        			data[2][1]*B.vpGetValueAt( 1, 3 )+
        			data[2][2]*B.vpGetValueAt( 2, 3 )+
        			data[2][3]*B.vpGetValueAt( 3, 3 ) );

	M->vpSetValueAt( 3, 0,	data[3][0]*B.vpGetValueAt( 0, 0 )+
        			data[3][1]*B.vpGetValueAt( 1, 0 )+
        			data[3][2]*B.vpGetValueAt( 2, 0 )+
        			data[3][3]*B.vpGetValueAt( 3, 0 ) );
	M->vpSetValueAt( 3, 1,	data[3][0]*B.vpGetValueAt( 0, 1 )+
        			data[3][1]*B.vpGetValueAt( 1, 1 )+
        			data[3][2]*B.vpGetValueAt( 2, 1 )+
        			data[3][3]*B.vpGetValueAt( 3, 1 ) );
	M->vpSetValueAt( 3, 2,	data[3][0]*B.vpGetValueAt( 0, 2 )+
        			data[3][1]*B.vpGetValueAt( 1, 2 )+
        			data[3][2]*B.vpGetValueAt( 2, 2 )+
        			data[3][3]*B.vpGetValueAt( 3, 2 ) );
	M->vpSetValueAt( 3, 3,	data[3][0]*B.vpGetValueAt( 0, 3 )+
        			data[3][1]*B.vpGetValueAt( 1, 3 )+
        			data[3][2]*B.vpGetValueAt( 2, 3 )+
        			data[3][3]*B.vpGetValueAt( 3, 3 ) );

	return M;
}

VPMatrix*
VPMatrix::vpSubtract( VPMatrix B ){

	VPMatrix *M = new VPMatrix();

        for( int l = 0; l < 4; l++ ){
        	for( int c = 0; c < 4; c++ ){
	        	M->vpSetValueAt( l, c,
                        		data[l][c] - B.vpGetValueAt( l, c ) );
                }
        }
        return M;
}

VPMatrix*
VPMatrix::vpAdd( VPMatrix B ){

	VPMatrix *M = new VPMatrix();

        for( int l = 0; l < 4; l++ ){
        	for( int c = 0; c < 4; c++ ){
	        	M->vpSetValueAt( l, c,
                        		data[l][c] + B.vpGetValueAt( l, c ) );
                }
        }
        return M;
}

VPPoint3D*
VPMatrix::vpMultiply( VPPoint3D p ){

	VPPoint3D *temp = new VPPoint3D();
	float x,y,z,w;

	w =	data[3][0] * p.vpGetX() + data[3][1] * p.vpGetY() +
        	data[3][2] * p.vpGetZ() + data[3][3] * 1;
	x =	(data[0][0] * p.vpGetX() + data[0][1] * p.vpGetY() +
        	data[0][2] * p.vpGetZ() + data[0][3] * 1)/w;
	y =	(data[1][0] * p.vpGetX() + data[1][1] * p.vpGetY() +
        	data[1][2] * p.vpGetZ() + data[1][3] * 1)/w;
	z =	(data[2][0] * p.vpGetX() + data[2][1] * p.vpGetY() +
        	data[2][2] * p.vpGetZ() + data[2][3] * 1)/w;

	temp->vpSetXYZ(x,y,z);

	return temp;
}

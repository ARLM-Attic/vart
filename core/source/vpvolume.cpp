//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpvolume.cpp
//  DESCRIPTION.: Contain the VPVolume class implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: November/13/2000
//  DESCRIPTION.: Implementation and changes of some methods.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: January/16/2000
//  DESCRIPTION.: Inclusion and changes of some methods for 
//				  dynamical image window allocation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: June/04/2001
//  DESCRIPTION.: Inclusion and changes of some methods to 
//				  identify the active camera, which is 
//				  determined by the view, and to get/set
//				  some attributes.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: October/28/2002
//  DESCRIPTION.: Inclusion and changes of some methods to 
//				  implement Gouraud shading.Code "adjust".
//
///////////////////////////////////////////////////////////////////


#include <vpvolume.h>
#include <iostream>
#include <cmath>
#include <string>	//STL include
using namespace std;


///////////////////////////////////////////////////////////////////
// Description: Class "VPVolume" constructor without parameter. 
// Parameters.: -
// Return.....: -

VPVolume::VPVolume () {
		zDimension = maxVolumeColor = 0;
		fileType = NONE;
		visualizationType = DEFAULTVIS;
		opacityComputation = LINEAROPAC;
		topSlice = 1;
		sideSlice = 1;
		frontSlice = 1;
		maxVolumeColor = 255;
		minVolumeColor = 0;
		grayImage = NULL;
		colorImage = NULL;
		activeCamera = NULL;
		volumeMatrix = NULL;
		volumeLightedMatrix = NULL;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPVolume" constructor with parameters for
//				monomodal visualization.
// Parameters.: char *n (path and file name)
//				int t (file type - constant MRI,CT...)
//				int dh (horizontal slice dimension)
//				int dv (vertical slice dimension)
//				int s (slice number)
//				int b (number of bits/pixel)
// Return.....: -

VPVolume::VPVolume (const char *n, int t, int dh, int dv, int s, int b) {

  // local variables
  int i=0, j=0, k=0, d=0; 
  float valTmp=0;
  const char *aux;
  char extension[4] = {'\0'},
	   description[100] = {'\0'},
	   file[20] = {'\0'}, 
	   path[70] = {'\0'};

  // Initialize some atributes
  grayImage = NULL;
  colorImage = NULL;
  activeCamera = NULL;
  maxVolumeColor = 0;
  maxVolumeColor = 0;
  minVolumeColor = 4095;

  // Set other attributes in accordance with the parameters
  fileType = t;
  visualizationType = DEFAULTVIS;
  opacityComputation = LINEAROPAC;
  fileBits = b;
  xDimension = dh;
  yDimension = s; 
  zDimension = dv;


  // Check file format in accordance with the file extension
  int baka=0;
  aux = n;
  while ( (*aux != '.') && (*aux) ) {
			aux++;
			baka++;
  }
  if (aux != NULL) {
			aux++;
			baka++;
			strcat(extension, aux);
  }  // else TRATAMENTO DE ERRO/EXCEÇÃO

  // Set the file path 
  while ( (*aux != '\\') && (*aux) ) {
			aux--;
			baka--;
  }
  baka++;
  strncpy(path, n, baka);

  // set the superclass attribute "description"
  strcpy(description, n);
  switch (fileType) {
			case ARQCT: strcat(description, ", CT data.");
						break;
			case ARQMRI:strcat(description, ", MRI data.");
						break;
			case ARQPET:strcat(description, ", PET data.");
						break;
			case ARQSPECT:strcat(description, ", SPECT data.");
						break;
  }

  if ( !strcmp (extension,"raw") ) { /////// "raw" file format
			  // finish to set the superclass attribute "description"
			  strcat(description, ", raw file");
			  vpSetDescription(description);
			  // to read the file .raw
			  vpReadRawFile(n);
  }

  else if ( !strcmp(extension,"i") ) { /////// "i" file format
			  // finish to set the superclass attribute "description"
			  strcat(description, ", i file");
			  vpSetDescription(description);
			  // to read the files .i
			  vpReadIFile(n, path);
  }
		  
  else if ( !strcmp(extension,"vol") ) { /////// "vol" file format
			  // finish to set the superclass attribute "description"
			  strcat(description, ", vol file");
			  vpSetDescription(description);
			  // to read the file .vol
			  vpReadVolFile(n);
  }
		  
  else { /////// file format not know
  }

  topSlice = (int) yDimension/2;
  sideSlice = (int) xDimension/2;
  frontSlice = (int) zDimension/2;

  controlTables.vpSetMinVolumeDensity(minVolumeColor); 
  controlTables.vpSetMaxVolumeDensity(maxVolumeColor); 
  controlTables.vpProcessMaxGradient();

  vpProcessCenterFocalPoint();
  vpProcessHistogram();
}


///////////////////////////////////////////////////////////////////
// Description: Method used to read raw file.
// Parameters.: -
// Return.....: -

void VPVolume::vpReadRawFile(const char *n) {
	  int i=0, j=0, k=0;
	  unsigned char aux1, aux2;
	  FILE *fp;

	  // Memory allocation for data loading
	  allocMemory();
	  allocMemory2();
 
	  // File opening
	  fp = fopen(n,"rb");
	  if ( fp == NULL ) {
			perror ("Unable to read dataset file."); 
			return;
	  }

	  switch (fileBits) {
	     case 8:
			for (k=0; k<yDimension; k++) {
				for (i=0; i<zDimension; i++) {
					// Density read
					for (j=0; j<xDimension; j++) {
						// Read the density
						fread(&aux1, 1, 1, fp);
						volumeMatrix2[j][k][i] = (unsigned int) aux1;
						if (volumeMatrix2[j][k][i] > maxVolumeColor) maxVolumeColor=volumeMatrix2[j][k][i];
						if (volumeMatrix2[j][k][i] < minVolumeColor) minVolumeColor=volumeMatrix2[j][k][i];
					}
				}
			}	
			break;
	     case 16:
			for (k=0; k<yDimension; k++) {
				for (i=0; i<zDimension; i++) {
					// Density read
					for (j=0; j<xDimension; j++) {
						// Read the density
						fread(&aux1, 1, 1, fp);
						fread(&aux2, 1, 1, fp);
						volumeMatrix2[j][k][i] = (unsigned int) (aux1*256) + aux2;
						if (volumeMatrix2[j][k][i] > maxVolumeColor) maxVolumeColor=volumeMatrix2[j][k][i];
						if (volumeMatrix2[j][k][i] < minVolumeColor) minVolumeColor=volumeMatrix2[j][k][i];
					}
				}
			}	
			break;
	  } // switch

	  fclose(fp);

	  for (j=0; j<yDimension; j++)
		for (i=0; i<xDimension; i++)
			for (k=0; k<zDimension; k++)
				volumeMatrix[i][j][k] = volumeMatrix2[i][yDimension-1-j][k];

	  delete [] volumeMatrix2;


}


///////////////////////////////////////////////////////////////////
// Description: Method used to read .i files.
// Parameters.: -
// Return.....: -

void VPVolume::vpReadIFile(const char *n, char *p) {
	  int i=0, j=0, k=0;
	  char file[20] = {'\0'}, 
		   pathFile[70] = {'\0'};
	  unsigned char aux1, aux2;
	  FILE *fp, *filesNames;

	  // Memory allocation for data loading
	  allocMemory();

	  // First open the file that has the name of all slices files
	  filesNames = fopen(n,"r");
	  if ( filesNames == NULL ) {
			perror ("Unable to read dataset file."); 
			return;
	  }

	  switch (fileBits) {
	     case 8:
				  for (k=0; k<yDimension; k++) {
					fscanf(filesNames,"%s\n", file); // slice file name
					strcpy(pathFile,p);
					strcat(pathFile, file);
					fp = fopen (pathFile, "rb");
					if ( fp == NULL ) {
						perror ("Unable to read dataset file."); 
						return;
					}
					fread(&aux1, 1, 1, fp);
					fread(&aux2, 1, 1, fp); // image width
					fread(&aux1, 1, 1, fp);
					fread(&aux2, 1, 1, fp); // imagem height
					for (i=0; i<zDimension; i++) {
						// Density read - JUST FOR 8 BITS!!!
						for (j=0; j<xDimension; j++) {
							// Read the density
							fread(&aux1, 1, 1, fp);
							volumeMatrix[j][k][i] = (unsigned int) aux1;
							if (volumeMatrix[j][k][i] > maxVolumeColor) maxVolumeColor=volumeMatrix[j][k][i];
							if (volumeMatrix[j][k][i] < minVolumeColor) minVolumeColor=volumeMatrix[j][k][i];
						}
					}
					fclose(fp);
				  }	
					break;
	     case 16:
				  for (k=0; k<yDimension; k++) {
					fscanf(filesNames,"%s\n", file); // slice file name
					strcpy(pathFile,p);
					strcat(pathFile, file);
					fp = fopen (pathFile, "rb");
					if ( fp == NULL ) {
						perror ("Unable to read dataset file."); 
						return;
					}
					for (i=0; i<zDimension; i++) {
						for (j=0; j<xDimension; j++) {
							// Read the density
							fread(&aux1, 1, 1, fp);
							fread(&aux2, 1, 1, fp);
							volumeMatrix[j][k][i] = (unsigned int) (aux1*256) + aux2; // para brain.i
							// volumeMatrix[j][k][i] = (unsigned int) ( (aux2*256) + aux1 ) / 256; // para tooth.i
							if (volumeMatrix[j][k][i] > maxVolumeColor) maxVolumeColor=volumeMatrix[j][k][i];
							if (volumeMatrix[j][k][i] < minVolumeColor) minVolumeColor=volumeMatrix[j][k][i];
						}
					}
					fclose(fp);
				  }	
				  break;
	  }

	  fclose(filesNames);

}


///////////////////////////////////////////////////////////////////
// Description: Method used to read .vol files.
// Parameters.: -
// Return.....: -

void VPVolume::vpReadVolFile(const char *n) {
	  #define SEEK_SET 0
	  int i=0, j=0, k=0;
	  FILE *fp;
	  xyz VolumeDimensions, Size;
	  int   N;
	  int NBytes;
	  unsigned char *V8;
	  sword *V16, V, Ib; // tipo sword
	  int x1, y1; //z1;
	  int MagicNumber;
	  int HeaderLength;
	  int         PixelDepth;
	  int		  PixelType;         // type
	  int         IndexBits;         // number of "object" index bits in each pixel                 */
	  float       ScaleX;            // base scale factors
	  float       ScaleY;
	  float       ScaleZ;
	  float       RotX;              // base rotation
	  float       RotY;
	  float       RotZ;
	  int         totalDepth;


	  // File opening
	  fp = fopen(n,"rb");
	  if ( fp == NULL ) {
			perror ("Unable to read dataset file."); 
			return;
	  }


	  fread((void *)&MagicNumber, sizeof(int), 1, fp);
	  vpSwapBytes(&MagicNumber, sizeof(int));

	  if ((MagicNumber != C_VR_MAGIC_NUMBER) &&
			(MagicNumber != C_VR_MAGIC_NUMBER_OLD)) {
		//	return -2;
	  }
	  fread((void *)&HeaderLength , sizeof(int), 1, fp);
 	  fread((void *)&VolumeDimensions , sizeof(xyz), 1, fp);
	  fread((void *)&PixelDepth , sizeof(int), 1, fp);
	  fread((void *)&IndexBits, sizeof(int), 1, fp);

	  vpSwapBytes ((int *)&HeaderLength, sizeof(int));
	  vpSwapBytes ((int *)&VolumeDimensions, sizeof(xyz));
	  vpSwapBytes ((int *)&PixelDepth,sizeof(int));
	  vpSwapBytes ((int *)&IndexBits, sizeof(int));


	  // Set some volume attributes in accordance with the data read
	  xDimension = VolumeDimensions.x;
	  yDimension = VolumeDimensions.z; 
	  zDimension = VolumeDimensions.y;

	  // Memory allocation for data loading
	  allocMemory();


	  PixelDepth -= IndexBits;
	  if(MagicNumber == C_VR_MAGIC_NUMBER) {
		fread((void *)&ScaleX , sizeof(float), 1, fp);
		fread((void *)&ScaleY , sizeof(float), 1, fp);
		fread((void *)&ScaleZ , sizeof(float), 1, fp);
		fread((void *)&RotX , sizeof(float), 1, fp);
		fread((void *)&RotY , sizeof(float), 1, fp);
		fread((void *)&RotZ , sizeof(float), 1, fp);
		vpSwapBytes ((int *)&ScaleX, sizeof(float));
		vpSwapBytes ((int *)&ScaleY, sizeof(float));
		vpSwapBytes ((int *)&ScaleZ, sizeof(float));
		vpSwapBytes ((int *)&RotX, sizeof(float));
		vpSwapBytes ((int *)&RotY, sizeof(float));
		vpSwapBytes ((int *)&RotZ, sizeof(float));
	  } else {
		ScaleX = 1.0;
		ScaleY = 1.0;
		ScaleZ = 1.0;
		RotX = 0.0;
		RotY = 0.0;
		RotZ = 0.0;
	  } // else


	  fseek(fp, HeaderLength, SEEK_SET);

	  totalDepth = PixelDepth + IndexBits;

	  switch(totalDepth) {
		case 8:
			PixelType = C_UNSIGNED_BYTE;
			break;
		case 16:
			if(IndexBits == 0) {
			    PixelType = C_UNSIGNED_SHORT;
			} else if (IndexBits == 4) {
				PixelType = C_UNSIGNED_SHORT;
			} else if (IndexBits == 8) {
	            PixelType = C_UNSIGNED_SHORT;
		    } else {
			 //   return -3;
			} // else
		  break;
		//  default:
       //  return -3;
	  } // switch

	  // determine the size of the dataset 
      if (PixelType == C_UNSIGNED_BYTE) {
			NBytes = 1;
	  } else {
			NBytes = 2;
	  }

	   if(NBytes == 1) {
		  N = 1;
		  if(RotY == 0.0 && RotZ == 0.0) {
			 if(RotX == 0) {
				Size.x = yDimension;
				Size.y = zDimension;
				Size.z = xDimension;
				V8 = new unsigned char[Size.y];
         		for(j=0; j<Size.x; j++) {
				   for(k=0; k<Size.z; k++) {
					   fread(&V8[0], NBytes, Size.y, fp);
					   y1 = Size.y-1;
               		for(i=0; i<Size.y; i++) {
         				 volumeMatrix[k][j][i] = V8[y1];
						 if(V8[y1] > maxVolumeColor) 
							maxVolumeColor = V8[y1];
						 else if(V8[y1] < minVolumeColor)
							minVolumeColor = V8[y1];
						 y1--;
					  } // for i
				   } // for k
				} // for j
			 } // if RotX == 0.0
			 else if(RotX == -90.0) {
				Size.x = VolumeDimensions.y;
				Size.y = VolumeDimensions.x;
				Size.z = VolumeDimensions.z;
				V8 = new unsigned char[Size.x];
         		for(k = Size.z-1; k >= 0; k--) {
				   for(i = Size.y-1; i >= 0; i--) {
					   fread(&V8[0], NBytes, Size.x, fp);
					   x1 = Size.x-1;
               		   for(j = 0; j < Size.x; j++) {
         				 volumeMatrix[j][k][i] = V8[x1];
						 if(V8[x1] > maxVolumeColor) 
							maxVolumeColor = V8[x1];
						 else if(V8[x1] < minVolumeColor)
							minVolumeColor = V8[x1];
						 x1--;
					   } // for j
				   } // for i
				} // for k
			 } // else
		  } // if
		  delete[] V8;
	   } else {
		  N = 1; // DOESN´T WORK!?!?!?
				//Size.X = VolumeDimensions.z;
				//Size.Y = VolumeDimensions.y;
				//Size.Z = VolumeDimensions.x;
				//AdjustScale(XYZR(ScaleZ, ScaleY, ScaleX));
				//Alloc();
				V16 = new sword[yDimension+1];
         		for(j=0; j<xDimension; j++) {
				   for(k=0; k<yDimension; k++) {
					   fread(&V16[1], NBytes, yDimension, fp);
					  y1 = yDimension;
               		for(i=0; i<zDimension; i++) {
						 V = (sword)((V16[y1] & (sword)0xFF00) >> 8);
						 Ib = (sword)((V16[y1] & (sword)0x03) << 4);      
						 if(Ib != 0)
            				 volumeMatrix[j][k][i] = (sword)(V | (Ib << 8));                   
						 else
            				 volumeMatrix[j][k][i] = V;                   
						 y1--;
						 if(V > maxVolumeColor)
							maxVolumeColor = V;
						 else if(V < minVolumeColor)
							minVolumeColor = V;
					  } // for x
				   } // for y
				} // for z
		  delete[] V16;
	   } // if

	  fclose(fp);

	  // Auxiliary memory allocation for data loading
	  allocMemory2();

	  for (j=0; j<yDimension; j++)
		for (i=0; i<xDimension; i++)
			for (k=0; k<zDimension; k++)
				volumeMatrix2[i][j][k] = volumeMatrix[i][yDimension-1-j][k];

	  delete [] volumeMatrix;

      volumeMatrix = volumeMatrix2;
}


///////////////////////////////////////////////////////////////////
// Description: Method used to allocate memory to put the volume 
//				data set and to put the light voxel information.  
//				The space is allocated acording to the xDimension, 
//				yDimension and zDimension attributes.
// Parameters.: -
// Return.....: bool (false if it doesn't allocate the necessary
//					  memory)

bool VPVolume::allocMemory() {

 int i, j;
//  Incluir tratamento de exceções!!!


 // Volume matrix allocation
 volumeMatrix = (unsigned int ***) new unsigned short int **[sizeof(unsigned short int **) * xDimension]; 
 if (volumeMatrix == NULL)
	return false;

 for (i=0; i<xDimension; i++) {
	volumeMatrix[i] = (unsigned int **) new unsigned short int *[sizeof(unsigned short int *) * yDimension];
	if (volumeMatrix[i] == NULL)
		return false;

	for (j=0; j<yDimension; j++) {
		volumeMatrix[i][j] = new unsigned int[sizeof(unsigned short int) * zDimension];
		if (volumeMatrix[i][j] == NULL)
			return false;
	}
 }

 // "Light" matrix allocation
 volumeLightedMatrix = (unsigned char ***) new unsigned char **[sizeof(unsigned char **) * xDimension]; 
 if (volumeLightedMatrix == NULL)
	return false;

 for (i=0; i<xDimension; i++) {
	volumeLightedMatrix[i] = (unsigned char **) new unsigned char *[sizeof(unsigned char *) * yDimension];
	if (volumeLightedMatrix[i] == NULL)
		return false;

	for (j=0; j<yDimension; j++) {
		volumeLightedMatrix[i][j] = new unsigned char[sizeof(unsigned char) * zDimension];
		if (volumeLightedMatrix[i][j] == NULL)
			return false;
	}
 }

 return true;

}


bool VPVolume::allocMemory2() {

 int i, j;
//  Incluir tratamento de exceções!!!


 // Volume matrix allocation
 volumeMatrix2 = (unsigned int ***) new unsigned short int **[sizeof(unsigned short int **) * xDimension]; 
 if (volumeMatrix2 == NULL)
	return false;

 for (i=0; i<xDimension; i++) {
	volumeMatrix2[i] = (unsigned int **) new unsigned short int *[sizeof(unsigned short int *) * yDimension];
	if (volumeMatrix2[i] == NULL)
		return false;

	for (j=0; j<yDimension; j++) {
		volumeMatrix2[i][j] = new unsigned int[sizeof(unsigned short int) * zDimension];
		if (volumeMatrix2[i][j] == NULL)
			return false;
	}
 }

 // "Light" matrix allocation
 volumeLightedMatrix = (unsigned char ***) new unsigned char **[sizeof(unsigned char **) * xDimension]; 
 if (volumeLightedMatrix == NULL)
	return false;

 for (i=0; i<xDimension; i++) {
	volumeLightedMatrix[i] = (unsigned char **) new unsigned char *[sizeof(unsigned char *) * yDimension];
	if (volumeLightedMatrix[i] == NULL)
		return false;

	for (j=0; j<yDimension; j++) {
		volumeLightedMatrix[i][j] = new unsigned char[sizeof(unsigned char) * zDimension];
		if (volumeLightedMatrix[i][j] == NULL)
			return false;
	}
 }

 return true;

}

///////////////////////////////////////////////////////////////////
// Description: Class "VPVolume" destructor.
// Parameters.: -
// Return.....: -

VPVolume::~VPVolume () {
	delete [] volumeMatrix;
	delete [] volumeLightedMatrix;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpProcessLightedVolume" do the volume
//				gradients processing.
//				.
// Parameters.: VPVector3D ld - light direction
//				float al - ambient light
// Return.....: -

void VPVolume::vpProcessLightedVolume(VPVector3D ld, float al) {

	int i, j, k, intLight;
	float dot=0, light=0;
	VPVector3D gradient, previousGradient, maxGradient;

	for (j=0; j<yDimension; j++) {
		for (k=0; k<zDimension; k++) {
			previousGradient.x=0; previousGradient.y=0; previousGradient.z=0;
			for (i=0; i<xDimension; i++) {

				previousGradient = gradient;
				
				// Gradient calculation uses the Sobel three-dimensional operator 
				 gradient = vpGetSobelGradient(i, j, k);

				gradient.vpNormalize();

				// Next steps: lighting processing

				// Dot product between the gradient and the light direction
				dot = gradient.vpDotProduct(ld); 
          
				if (dot < 0.0)
					dot = (float) 0.0;	// no negative contributions

				light = al + (0.9 * dot); // 0.9 = diffuse

				if (light > 1) 
					light = 1;

				// One byte to load light value: 
				//  7 bits for the color and 1 bit for the flag

				// Color (light value): 0-100
				intLight = floor ( (light * 100) + 0.5 ); // "+0.5" to round light value

				// flag = 1 (true) if the 8 neighbor voxels have the same intensity
				// flag = 0 (false) if the 8 neighbor voxels don't have the same intensity
				// Set the bit flag if the 8 neighbor voxels have the same intensity
				// if (8 neighbor have the same value)
				// primeiro bit = 1
				// "else" it's not necessary because the first bit is already 0
				volumeLightedMatrix[i][j][k] = (unsigned char) intLight;
			}
		}
	}	

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetSobelGradient" process and returns 
//				Sobel gradient.
// Parameters.: int i, int j, int k (matrix index)
// Return.....: VPVector3D gradient

VPVector3D VPVolume::vpGetSobelGradient(int i, int j, int k) 
{
	int iMinus, jMinus, kMinus, iPlus, jPlus, kPlus, x, y, z;
	VPVector3D gradient;

	// For boundaries adjustment
	iPlus = ( (i+1) < xDimension ) ? i+1 : i;
	jPlus = ( (j+1) < yDimension ) ? j+1 : j;
	kPlus = ( (k+1) < zDimension ) ? k+1 : k;
	iMinus = ( (i-1) >= 0 ) ? i-1 : i;
	jMinus = ( (j-1) >= 0 ) ? j-1 : j;
	kMinus = ( (k-1) >= 0 ) ? k-1 : k;

	x = 6 * ( volumeMatrix[iMinus][j][k] - volumeMatrix[iPlus][j][k] ) +
		3 * ( volumeMatrix[iMinus][jMinus][k] + volumeMatrix[iMinus][jPlus][k] +
			  volumeMatrix[iMinus][j][kPlus] + volumeMatrix[iMinus][j][kMinus] -
			  volumeMatrix[iPlus][jMinus][k] - volumeMatrix[iPlus][jPlus][k] -
			  volumeMatrix[iPlus][j][kPlus] - volumeMatrix[iPlus][j][kMinus]    ) +
		2 * ( volumeMatrix[iMinus][jMinus][kMinus] + volumeMatrix[iMinus][jMinus][kPlus] +
			  volumeMatrix[iMinus][jPlus][kPlus] + volumeMatrix[iMinus][jPlus][kMinus] -
			  volumeMatrix[iPlus][jMinus][kMinus] - volumeMatrix[iPlus][jMinus][kPlus] -
			  volumeMatrix[iPlus][jPlus][kPlus] - volumeMatrix[iPlus][jPlus][kMinus]    );

	y = 6 * ( volumeMatrix[i][jMinus][k] - volumeMatrix[i][jPlus][k] ) +
        3 * ( volumeMatrix[i][jMinus][kMinus] + volumeMatrix[iMinus][jMinus][k] +
              volumeMatrix[iPlus][jMinus][k] + volumeMatrix[i][jMinus][kPlus] -
			  volumeMatrix[i][jPlus][kMinus] - volumeMatrix[iMinus][jPlus][k] -
              volumeMatrix[iPlus][jPlus][k] - volumeMatrix[i][jPlus][kPlus]   ) +
        2 * ( volumeMatrix[iMinus][jMinus][kMinus] + volumeMatrix[iPlus][jMinus][kMinus] +
              volumeMatrix[iMinus][jMinus][kPlus] + volumeMatrix[iPlus][jMinus][kPlus] -
			  volumeMatrix[iMinus][jPlus][kMinus] - volumeMatrix[iPlus][jPlus][kMinus] -
              volumeMatrix[iMinus][jPlus][kPlus] - volumeMatrix[iPlus][jPlus][kPlus] );

	z = 6 * (volumeMatrix[i][j][kMinus] - volumeMatrix[i][j][kPlus] ) +
        3 * (volumeMatrix[iMinus][j][kMinus] + volumeMatrix[iPlus][j][kMinus] +
             volumeMatrix[i][jPlus][kMinus] + volumeMatrix[i][jMinus][kMinus] -
             volumeMatrix[iMinus][j][kPlus] - volumeMatrix[iPlus][j][kPlus] -
             volumeMatrix[i][jPlus][kPlus] - volumeMatrix[i][jMinus][kPlus]    ) +
        2 * (volumeMatrix[iMinus][jMinus][kMinus] + volumeMatrix[iMinus][jPlus][kMinus] +
             volumeMatrix[iPlus][jPlus][kMinus] + volumeMatrix[iPlus][jMinus][kMinus] -
             volumeMatrix[iMinus][jMinus][kPlus] - volumeMatrix[iMinus][jPlus][kPlus] -
             volumeMatrix[iPlus][jPlus][kPlus] - volumeMatrix[iPlus][jMinus][kPlus] );

	gradient.x = (float) x / 26.0;
	gradient.y = (float) y / 26.0;
	gradient.z = (float) z / 26.0; 

	return gradient;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLinearGradientEstimation" process and  
//				returns the gradient estimation based on 4D linear
//				regression.
// Parameters.: int i, int j, int k (matrix index)
// Return.....: VPVector3D gradient

VPVector3D VPVolume::vpGetLinearGradientEstimation(int i, int j, int k) {

	int iMinus, jMinus, kMinus, iPlus, jPlus, kPlus;
	VPVector3D gradient;

	// For boundaries adjustment
	iPlus = ( (i+1) < xDimension ) ? i+1 : i;
	jPlus = ( (j+1) < yDimension ) ? j+1 : j;
	kPlus = ( (k+1) < zDimension ) ? k+1 : k;
	iMinus = ( (i-1) >= 0 ) ? i-1 : i;
	jMinus = ( (j-1) >= 0 ) ? j-1 : j;
	kMinus = ( (k-1) >= 0 ) ? k-1 : k;

	gradient.x = (float)
    - volumeMatrix[iMinus][j][k] //*  ( 1 / (sqrt( (i-iMinus)+(i-iMinus)  )) )
    - volumeMatrix[iMinus][j][kPlus] 
    - volumeMatrix[iMinus][j][kMinus]   
    - volumeMatrix[iMinus][jMinus][k]  
    - volumeMatrix[iMinus][jMinus][kPlus] 
	- volumeMatrix[iMinus][jMinus][kMinus] 
	- volumeMatrix[iMinus][jPlus][k] 
	- volumeMatrix[iMinus][jPlus][kPlus] 
	- volumeMatrix[iMinus][jPlus][kMinus]  
	+ volumeMatrix[iPlus][j][k]  
	+ volumeMatrix[iPlus][j][kPlus]  
	+ volumeMatrix[iPlus][j][kMinus] 
	+ volumeMatrix[iPlus][jMinus][k]  
	+ volumeMatrix[iPlus][jMinus][kPlus]  
	+ volumeMatrix[iPlus][jMinus][kMinus] 
    + volumeMatrix[iPlus][jPlus][k]  
    + volumeMatrix[iPlus][jPlus][kPlus] 
    + volumeMatrix[iPlus][jPlus][kMinus]  ;

	gradient.y = (float)
	- volumeMatrix[i][jMinus][k]  
	- volumeMatrix[i][jMinus][kPlus]   
	- volumeMatrix[i][jMinus][kMinus] 
 	- volumeMatrix[iPlus][jMinus][k]  
	- volumeMatrix[iPlus][jMinus][kPlus]   
  	- volumeMatrix[iPlus][jMinus][kMinus]  
    - volumeMatrix[iMinus][jMinus][k]  
    - volumeMatrix[iMinus][jMinus][kPlus]   
    - volumeMatrix[iMinus][jMinus][kMinus]   
    + volumeMatrix[i][jPlus][k] 
    + volumeMatrix[i][jPlus][kPlus] 
    + volumeMatrix[i][jPlus][kMinus] 
	+ volumeMatrix[iMinus][jPlus][k] 
	+ volumeMatrix[iMinus][jPlus][kPlus] 
	+ volumeMatrix[iMinus][jPlus][kMinus] 
    + volumeMatrix[iPlus][jPlus][k] 
    + volumeMatrix[iPlus][jPlus][kPlus] 
    + volumeMatrix[iPlus][jPlus][kMinus] ;

	gradient.z = (float)
	- volumeMatrix[i][j][kMinus] 
	- volumeMatrix[i][jPlus][kMinus]  
	- volumeMatrix[i][jMinus][kMinus]  
	- volumeMatrix[iPlus][j][kMinus]  
	- volumeMatrix[iPlus][jPlus][kMinus]  
	- volumeMatrix[iPlus][jMinus][kMinus]  
	- volumeMatrix[iMinus][j][kMinus]  
	- volumeMatrix[iMinus][jMinus][kMinus] 
	- volumeMatrix[iMinus][jPlus][kMinus]  
	+ volumeMatrix[i][j][kPlus]  
	+ volumeMatrix[i][jPlus][kPlus]  
	+ volumeMatrix[i][jMinus][kPlus]      
	+ volumeMatrix[iPlus][j][kPlus] 
	+ volumeMatrix[iPlus][jPlus][kPlus]  
	+ volumeMatrix[iPlus][jMinus][kPlus] 
	+ volumeMatrix[iMinus][j][kPlus]  
	+ volumeMatrix[iMinus][jMinus][kPlus] 
	+ volumeMatrix[iMinus][jPlus][kPlus] ;

	return gradient;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetCentralDifferenceGradient" process and  
//				returns the gradient estimation based on central
//				difference method.
// Parameters.: int i, int j, int k (matrix index)
// Return.....: VPVector3D gradient

VPVector3D VPVolume::vpGetCentralDifferenceGradient(int i, int j, int k) {

	int iMinus, jMinus, kMinus, iPlus, jPlus, kPlus;
	VPVector3D gradient;

	// For boundaries adjustment
	iPlus = ( (i+1) < xDimension ) ? i+1 : i;
	jPlus = ( (j+1) < yDimension ) ? j+1 : j;
	kPlus = ( (k+1) < zDimension ) ? k+1 : k;
	iMinus = ( (i-1) >= 0 ) ? i-1 : i;
	jMinus = ( (j-1) >= 0 ) ? j-1 : j;
	kMinus = ( (k-1) >= 0 ) ? k-1 : k;

	// Technique: Central Difference Gradient Estimator
	// 
	//        5-----------------6		
	//       /|           	   /|		
	//      / |               / |		
	//     /  |              /  |		
	// V(i,j,k)-------------2   |		
	//    |   |             |   |		
	//    |   |             |   |		
	//    |   |             |   |		
	//    |   8-------------|---V(i+1,j+1,k+1)     		
	//    |  /              |  /		
	//    | / 	            | /		
	//    |/                |/		
	//    4-----------------3     	
	//					
	// Gx = V(i+1,j,k) - V(i-1,j,k)
	// Gy = V(i,j+1,k) - V(i,j-1,k)
	// Gz = V(i,j,k+1) - V(i,j,k-1)

	gradient.x = volumeMatrix[iPlus][j][k] - volumeMatrix[iMinus][j][k]; 
	gradient.y = volumeMatrix[i][jPlus][k] - volumeMatrix[i][jMinus][k]; 
    gradient.z = volumeMatrix[i][j][kPlus] - volumeMatrix[i][j][kMinus]; 

	return gradient;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetValue" returns the value of a volume
//				matrix position.
// Parameters.: int i, int j, int k (matrix index)
// Return.....: short int volumeMatrix[i][j][k]

unsigned int VPVolume::vpGetValue(int i, int j, int k) {
	unsigned int color;

	switch (fileBits) {
		case 8: color = (unsigned int) ((float)volumeMatrix[i][j][k]);
				break;
		case 16:float temp = (float) (maxVolumeColor/255);	
				// Density mapping to a gray value
				// "Three Rule" for the convertion to 8 bits (0-65536 to 0-256
				// gray levels): (256 * ponto)/65536
				color = (unsigned int) ((float)volumeMatrix[i][j][k]/temp);
				break;
	}

	return (color);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLightValue" returns the value of a 
//				volume lighted matrix position.
// Parameters.: int i, int j, int k (matrix index)
// Return.....: float volumeMatrix[i][j][k]

float VPVolume::vpGetLightValue(int i, int j, int k) {
	float light;
	// CUIDAR A QUESTÃO QUE O PRIMEIRO BIT É FLAG!!!!
	light = volumeLightedMatrix[i][j][k] & (unsigned char)0x7F; //( (int)volumeLightedMatrix[i][j][k])/100.0;char
	light /= 100.0;
	return (light);
}


///////////////////////////////////////////////////////////////////
// Description: Method for image rendering.
// Parameters.: -
// Return.....: -

void VPVolume::vpRender() {

	int viewHeight, viewWidth;
	activeCamera->vpGetViewDimension(viewHeight, viewWidth);

	switch(visualizationType) {
		case MIPVIS: 
						vpAllocateGrayImage(viewHeight, viewWidth);
						rayCasting.vpRenderMIP(scene, activeCamera, grayImage);
						vpDisplayGray();
						break;
		case MONOCOLORVIS:
						vpAllocateColorImage(viewHeight, viewWidth);
						rayCasting.vpRenderLivroMonoColor(scene, activeCamera, opacityComputation, colorImage);
						vpDisplayColor();
						break;
		case NOLIGHTMONOGRAYVIS:
						vpAllocateGrayImage(viewHeight, viewWidth);
						rayCasting.vpRenderNoLightMonoGray(scene, activeCamera, grayImage);
						vpDisplayGray();
						break;
		case MONOGRAYVIS:
						vpAllocateGrayImage(viewHeight, viewWidth);
						rayCasting.vpRenderLivroMonoGray(scene, activeCamera, MONOGRAYVIS, opacityComputation, grayImage);
						vpDisplayGray();
						break;
		case MULTIMODALVIS:
						vpAllocateColorImage(viewHeight, viewWidth);
						rayCasting.vpRenderLivroMultiModal(scene, activeCamera, opacityComputation, colorImage);
						vpDisplayColor();
						break;
		case MULTINNERSTRUCTURES:
						vpAllocateColorImage(viewHeight, viewWidth);
						rayCasting.vpRenderLivroMultiInnerStructures(scene, activeCamera, opacityComputation, colorImage); 
						vpDisplayColor();
						break;
		case MONOINNERSTRUCTURES:
						vpAllocateGrayImage(viewHeight, viewWidth);
						rayCasting.vpRenderLivroInnerStructures(scene, activeCamera, opacityComputation, grayImage); 
						vpDisplayGray();
						break;
		case MONOCOLORINNERSTRUCTURES:
						vpAllocateColorImage(viewHeight, viewWidth);
						rayCasting.vpRenderLivroMonoColorInnerStructures(scene, activeCamera, opacityComputation, colorImage); 
						vpDisplayColor();
						break;
		case DEFAULTVIS:
						vpAllocateGrayImage(viewHeight, viewWidth);
						rayCasting.vpRenderLivroMonoGray(scene, activeCamera, DEFAULTVIS, opacityComputation, grayImage);
						vpDisplayGray();
						break;

		case INNERSTRTOPSLICE:
						vpAllocateColorImage(viewHeight, viewWidth);
						vpSetSliceTopCamera();
						slice.vpTopInnerStructRender(activeCamera, this, topSlice, colorImage);
						vpDisplayColor();
						break;

		case SLICETOP:	
						vpAllocateGrayImage(viewHeight, viewWidth);
						vpSetSliceTopCamera();
						slice.vpTopSliceRender(activeCamera, this, topSlice, grayImage);
						vpDisplayGray();
						break;

		case INNERSTRFRONTSLICE:

		case SLICEFRONT:
						vpAllocateGrayImage(viewHeight, viewWidth);
						vpSetSliceFrontCamera();
						slice.vpFrontSliceRender(activeCamera, this, frontSlice, grayImage);
						vpDisplayGray();
						break;

		case INNERSTRSIDESLICE:
						vpAllocateColorImage(viewHeight, viewWidth);
						vpSetSliceSideCamera();
						slice.vpSideInnerStructRender(activeCamera, this, sideSlice, colorImage);
						vpDisplayColor();
						break;

		case SLICESIDE: 
						vpAllocateGrayImage(viewHeight, viewWidth);
						vpSetSliceSideCamera();
						slice.vpSideSliceRender(activeCamera, this, sideSlice, grayImage);
						vpDisplayGray();
						break;

		case LINETOPSLICE:	
						vpAllocateGrayImage(viewHeight, viewWidth);
						vpSetSliceTopCamera();
						slice.vpLineTopSliceRender(activeCamera, this, topSlice, grayImage);
						vpDisplayGray();
						break;
		case LINEFRONTSLICE:
						vpAllocateGrayImage(viewHeight, viewWidth);
						vpSetSliceFrontCamera();
						slice.vpLineFrontSliceRender(activeCamera, this, frontSlice, grayImage);
						vpDisplayGray();
						break;
		case LINESIDESLICE: 
						vpAllocateGrayImage(viewHeight, viewWidth);
						vpSetSliceSideCamera();
						slice.vpLineSideSliceRender(activeCamera, this, sideSlice, grayImage);
						vpDisplayGray();
						break;
	}
	
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpAllocateGrayImage" is used to allocate
//				and initialize an array for gray image store.
// Parameters.: -
// Return.....: -

void VPVolume::vpAllocateGrayImage(int h, int w) {

	grayImage = (unsigned int **) new unsigned int[h]; 
 	for (int c=0; c<h; c++)
		grayImage[c] = new unsigned int[w];

	for (int i=0; i<h; i++)
		for (int j=0; j<w; j++)
			grayImage[i][j] = 0;

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpAllocateColorImage" is used to allocate
//				and initialize an array for color image store.
// Parameters.: -
// Return.....: -

void VPVolume::vpAllocateColorImage(int h, int w) {

 int i=0, j=0;

 colorImage = (unsigned int ***) new unsigned short int **[sizeof(unsigned short int **) * h]; 

 for (i=0; i<h; i++) {
	colorImage[i] = (unsigned int **) new unsigned short int *[sizeof(unsigned short int *) * w];

	for (j=0; j<w; j++) {
		colorImage[i][j] = new unsigned int[sizeof(unsigned short int) * 3];
	}
 }

 for (i=0; i<h; i++)
	for (j=0; j<w; j++) {
		colorImage[i][j][red] = 0;
		colorImage[i][j][green] = 0;
		colorImage[i][j][blue] = 0;
	}
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetSliceTopCamera" set the camera
//				attributes values for a top slice visualization.
// Parameters.: -
// Return.....: -

void VPVolume::vpSetSliceTopCamera() {
	list<VPCamera*>::const_iterator iter;
	list<VPCamera*> cameras = scene->vpGetCameras();
	iter=cameras.begin();
	while ( *iter != activeCamera ) 
		iter++;

	// Get a pointer to the right camera 
	VPCamera *camera = *iter;
	VPPoint3D p;

	if ( (p == camera->vpGetTarget()) && (p == camera->vpGetLocation()) ) {
		// set camera parameters
		VPPoint3D target((float) vpGetXDimension()/2, 
						 (float) vpGetYDimension()/2,
						 (float) vpGetZDimension()/2);
		VPPoint3D location ((float) vpGetXDimension()/2, 
							(float) 0,
							(float) vpGetZDimension()/2);
		VPVector3D up(0.0,0.0,-1.0);

		camera->vpSetLocation(location);
		camera->vpSetTarget(target);
		camera->vpSetProjectionType(ORTHOGRAPHIC);
		camera->vpSetUp(up);
		camera->vpSetWinTopRight(vpGetXDimension()-1, vpGetZDimension()-1);
 
		camera->vpSetFarPlane(vpGetYDimension()+1);
		camera->vpSetNearPlane(0);
	}
	
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetSliceFrontCamera" set the camera
//				attributes values for a front slice visualization.
// Parameters.: -
// Return.....: -

void VPVolume::vpSetSliceFrontCamera() {
	int virtualYDimension=0;
	list<VPCamera*>::const_iterator iter;
	list<VPCamera*> cameras = scene->vpGetCameras();
	iter=cameras.begin();
	while ( *iter != activeCamera ) 
		iter++;

	// Get a pointer to the right camera 
	VPCamera *camera = *iter;
	VPPoint3D p;

	// To verify the smallest dimension and set the virtualVolumeDimension
	if ( vpGetXDimension() < vpGetZDimension() )
		virtualYDimension = vpGetZDimension();
	else 
		virtualYDimension = vpGetXDimension();	

	if ( (p == camera->vpGetTarget()) && (p == camera->vpGetLocation()) ) {
		// set camera parameters	
		VPPoint3D target((float) vpGetXDimension()/2, 
						 (float) virtualYDimension/2,
						 (float) vpGetZDimension()/2);
		VPPoint3D location ((float) vpGetXDimension()/2, 
							(float) virtualYDimension/2,
							(float) vpGetZDimension());
		VPVector3D up(0.0,1.0,0.0);

		camera->vpSetLocation(location);
		camera->vpSetTarget(target);
		camera->vpSetProjectionType(ORTHOGRAPHIC);
		camera->vpSetUp(up);
		camera->vpSetWinTopRight(vpGetXDimension()-1, virtualYDimension-1);
 
		camera->vpSetFarPlane(vpGetZDimension()+1);
		camera->vpSetNearPlane(0);
	}
	
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetSliceSideCamera" set the camera
//				attributes values for a front slice visualization.
// Parameters.: -
// Return.....: -

void VPVolume::vpSetSliceSideCamera() {
	int virtualYDimension=0;
	list<VPCamera*>::const_iterator iter;
	list<VPCamera*> cameras = scene->vpGetCameras();
	iter=cameras.begin();
	while ( *iter != activeCamera ) 
		iter++;

	// Get a pointer to the right camera 
	VPCamera *camera = *iter;
	VPPoint3D p;

	// To verify the smallest dimension and set the virtualVolumeDimension
	if ( vpGetXDimension() < vpGetZDimension() )
		virtualYDimension = vpGetZDimension();
	else 
		virtualYDimension = vpGetXDimension();	

	if ( (p == camera->vpGetTarget()) && (p == camera->vpGetLocation()) ) {
		// set camera parameters	
		VPPoint3D target((float) vpGetXDimension()/2, 
						 (float) virtualYDimension/2,
						 (float) vpGetZDimension()/2);
		VPPoint3D location ((float) 0, 
							(float) virtualYDimension/2,
							(float) vpGetZDimension()/2);
		VPVector3D up(0.0,1.0,0.0);

		camera->vpSetLocation(location);
		camera->vpSetTarget(target);
		camera->vpSetProjectionType(ORTHOGRAPHIC);
		camera->vpSetUp(up);
		camera->vpSetWinTopRight(vpGetZDimension()-1, virtualYDimension-1);
	
		camera->vpSetFarPlane(vpGetXDimension()+1);
		camera->vpSetNearPlane(0);
	}
}


///////////////////////////////////////////////////////////////////
// Description: Method for gray level image display.
// Parameters.: -
// Return.....: -

void VPVolume::vpDisplayGray(){
 int i,j,k=0;
 int color=0;
 int viewHeight, viewWidth;
 
 activeCamera->vpGetViewDimension(viewHeight, viewWidth);

  for (i=0; i<viewHeight; i++,k+=3) { 
	 for (j=0; j<viewWidth; j++) { 

		glBegin(GL_POINTS);
			glColor3ub((GLubyte)grayImage[i][j],(GLubyte)grayImage[i][j],(GLubyte)grayImage[i][j]);
			glVertex3f(j+1, i+1, 1.0f); // j+1 because the first array of data position
										// is 0 and the first screen position is 1
		glEnd();
	}
 }						

 delete [] grayImage; 
}


///////////////////////////////////////////////////////////////////
// Description: Method for color image display.
// Parameters.: -
// Return.....: -

void VPVolume::vpDisplayColor(){
 int i,j;
 int color=0;
 int viewHeight, viewWidth;
 
 activeCamera->vpGetViewDimension(viewHeight, viewWidth);

  for (i=0; i<viewHeight; i++) { 
	 for (j=0; j<viewWidth; j++) { 

		glBegin(GL_POINTS);
			glColor3ub((GLubyte)colorImage[i][j][red],(GLubyte)colorImage[i][j][green],(GLubyte)colorImage[i][j][blue]);
			glVertex3f(j+1, i+1, 1.0f); // j+1 because the first array of data position
										// is 0 and the first screen position is 1
		glEnd();
	}
 }	
  
 if (INNERSTRTOPSLICE == visualizationType) {
		VPPoint2D p1, p2;

		// Draw a rectangle at SBand and TBand position
		p1 = slice.vpGetViewSBandPoint();
		p2 = slice.vpGetViewTBandPoint();
		glColor3f(1.0f, 0.0f, 0.0f);
		glRectf(p1.y-2, p1.x-2, p1.y+2, p1.x+2);
		glColor3f(1.0f, 1.0f, 0.0f);
		glRectf(p2.y-2, p2.x-2, p2.y+2, p2.x+2);

		// Draw the line that join SBand and TBand
		glLineWidth(1);
		glBegin(GL_LINES); 
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(p1.y, p1.x, 1.0f);
			glColor3f(1.0f, 1.0f, 0.0f);	
			glVertex3f(p2.y, p2.x, 1.0f);
		glEnd();
		
		glLineWidth(2);

		// Draw the SBand line
		VPPoint2D *line;
		int points;
		slice.vpGetLineSBand(&line, points);
		glColor3f(1.0f, 0.0f, 0.0f);
		for (i=0; i<points-1; i++) {
			glBegin(GL_LINES); 
				glVertex3f(line[i].y, line[i].x, 1.0f);
				glVertex3f(line[i+1].y, line[i+1].x, 1.0f);
			glEnd();
		}
		// Draw the TBand line
		slice.vpGetLineTBand(&line, points);
		glColor3f(1.0f, 1.0f, 0.0f);
		for (i=0; i<points-1; i++) {
			glBegin(GL_LINES); 
				glVertex3f(line[i].y, line[i].x, 1.0f);
				glVertex3f(line[i+1].y, line[i+1].x, 1.0f);
			glEnd();
		}
		
 }
 
 else if (INNERSTRSIDESLICE == visualizationType) {
		VPPoint2D p1, p2;

		// Draw a rectangle at SBand and TBand position
		p1 = slice.vpGetViewSBandPoint();
		p2 = slice.vpGetViewTBandPoint();
		glColor3f(1.0f, 0.0f, 0.0f);
		glRectf(p1.y-2, p1.x-2, p1.y+2, p1.x+2);
		glColor3f(1.0f, 1.0f, 0.0f);
		glRectf(p2.y-2, p2.x-2, p2.y+2, p2.x+2);

		// Draw the line that join SBand and TBand
		glLineWidth(1);
		glBegin(GL_LINES); 
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(p1.y, p1.x, 1.0f);
			glColor3f(1.0f, 1.0f, 0.0f);	
			glVertex3f(p2.y, p2.x, 1.0f);
		glEnd();
		
		glLineWidth(2);

		// Draw the SBand line
		VPPoint2D *line;
		int points;
		slice.vpGetLineSBand(&line, points);
		glColor3f(1.0f, 0.0f, 0.0f);
		for (i=0; i<points-1; i++) {
			glBegin(GL_LINES); 
				glVertex3f(line[i].y, line[i].x, 1.0f);
				glVertex3f(line[i+1].y, line[i+1].x, 1.0f);
			glEnd();
		}
		// Draw the TBand line
		slice.vpGetLineTBand(&line, points);
		glColor3f(1.0f, 1.0f, 0.0f);
		for (i=0; i<points-1; i++) {
			glBegin(GL_LINES); 
				glVertex3f(line[i].y, line[i].x, 1.0f);
				glVertex3f(line[i+1].y, line[i+1].x, 1.0f);
			glEnd();
		}
		
 }
 else if (INNERSTRFRONTSLICE == visualizationType) {
		VPPoint2D p1, p2;

		// Draw a rectangle at SBand and TBand position
		p1 = slice.vpGetViewSBandPoint();
		p2 = slice.vpGetViewTBandPoint();
		glColor3f(1.0f, 0.0f, 0.0f);
		glRectf(p1.y-2, p1.x-2, p1.y+2, p1.x+2);
		glColor3f(1.0f, 1.0f, 0.0f);
		glRectf(p2.y-2, p2.x-2, p2.y+2, p2.x+2);

		// Draw the line that join SBand and TBand
		glLineWidth(1);
		glBegin(GL_LINES); 
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(p1.y, p1.x, 1.0f);
			glColor3f(1.0f, 1.0f, 0.0f);	
			glVertex3f(p2.y, p2.x, 1.0f);
		glEnd();
		
		glLineWidth(2);

		// Draw the SBand line
		VPPoint2D *line;
		int points;
		slice.vpGetLineSBand(&line, points);
		glColor3f(1.0f, 0.0f, 0.0f);
		for (i=0; i<points-1; i++) {
			glBegin(GL_LINES); 
				glVertex3f(line[i].y, line[i].x, 1.0f);
				glVertex3f(line[i+1].y, line[i+1].x, 1.0f);
			glEnd();
		}
		// Draw the TBand line
		slice.vpGetLineTBand(&line, points);
		glColor3f(1.0f, 1.0f, 0.0f);
		for (i=0; i<points-1; i++) {
			glBegin(GL_LINES); 
				glVertex3f(line[i].y, line[i].x, 1.0f);
				glVertex3f(line[i+1].y, line[i+1].x, 1.0f);
			glEnd();
		}
		
 }

 delete [] colorImage; 
}


///////////////////////////////////////////////////////////////////
// Description: Method to rotate an object.
// Parameters.: -
// Return.....: -

void VPVolume::vpRotate() {

}


///////////////////////////////////////////////////////////////////
// Description: Method to change an object scale.
// Parameters.: -
// Return.....: -

void VPVolume::vpScale(VPPoint3D s) {

}


///////////////////////////////////////////////////////////////////
// Description: Method to translate an object
// Parameters.: -
// Return.....: -

void VPVolume::vpTranslate(VPPoint3D t) {

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetVisualizationType" sends a new value 
//				to the "visualizationType" attribute.
// Parameters.: int vp (new attribute value)
// Return.....: -

void VPVolume::vpSetVisualizationType(int vp) {
	visualizationType = vp;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetVisualizationType" returns the value  
//				of visualizationType attribute.
// Parameters.: -
// Return.....: int visualizationType (attribute value)

int VPVolume::vpGetVisualizationType() {
	return visualizationType;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetTopSlice" sends a new value 
//				to the "topSlice" attribute.
// Parameters.: int v (new attribute value)
// Return.....: -

void VPVolume::vpSetTopSlice(int v) {
	topSlice = v;
	scene->vpSetChanged(true, this);
	scene->vpSetTypeOfChange(NEWTOPSLICE);

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetTopSlice" returns the value of 
//				topSlice attribute.
// Parameters.: -
// Return.....: int topSlice (attribute value)

int VPVolume::vpGetTopSlice() {
	return topSlice;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetSideSlice" sends a new value 
//				to the "sideSlice" attribute.
// Parameters.: int v (new attribute value)
// Return.....: -

void VPVolume::vpSetSideSlice(int v) {
	sideSlice = v;
	scene->vpSetChanged(true, this);
	scene->vpSetTypeOfChange(NEWSIDESLICE);

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetSideSlice" returns the value of 
//				sideSlice attribute.
// Parameters.: -
// Return.....: int sideSlice (attribute value)

int VPVolume::vpGetSideSlice() {
	return sideSlice;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetFrontSlice" sends a new value 
//				to the "frontSlice" attribute.
// Parameters.: int v (new attribute value)
// Return.....: -

void VPVolume::vpSetFrontSlice(int v) {
	frontSlice = v;
	scene->vpSetChanged(true, this);
	scene->vpSetTypeOfChange(NEWFRONTSLICE);

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetFrontSlice" returns the value of 
//				frontSlice attribute.
// Parameters.: -
// Return.....: int frontSlice (attribute value)

int VPVolume::vpGetFrontSlice() {
	return frontSlice;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetActiveCamera" sends a new value 
//				to the activeCamera attribute.
// Parameters.: -
// Return.....: -

void VPVolume::vpSetActiveCamera(VPCamera *c) {
	activeCamera = c;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingSBand" sends a new value for the end
//				of S-Band (used for inner structures visualization).
// Parameters.: VPPoint3D sb, int vt(visualization type)
// Return.....: -

void VPVolume::vpSetRayCastingSBand(VPPoint3D sb, int vt) {
	rayCasting.vpSetEndSBand(sb, this, vt);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingSBand" get the value of the end
//				of S-Band (used for inner structures visualization).
// Parameters.: -
// Return.....: int

int VPVolume::vpGetRayCastingSBand(){ 
	return rayCasting.vpGetEndSBand();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingTBand" sends a new value for the end
//				of T-Band (used for inner structures visualization).
// Parameters.: VPPoint3D tb, int vt(visualization type)
// Return.....: -

void VPVolume::vpSetRayCastingTBand(VPPoint3D tb, int vt) {
	rayCasting.vpSetEndTBand(tb, this, vt);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingTBand" get the value of the end
//				of T-Band (used for inner structures visualization).
// Parameters.: -
// Return.....: int

int VPVolume::vpGetRayCastingTBand() {
	return rayCasting.vpGetEndTBand();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingSampleStep" sends a new value 
//				to the object attribute (sampleStep).
// Parameters.: float s
// Return.....: -

void VPVolume::vpSetRayCastingSampleStep(float s) {
	rayCasting.vpSetSampleStep(s);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingSampleStep" get the 
//				object attribute value (sampleStep).
// Parameters.: -
// Return.....: float sampleStep

float VPVolume::vpGetRayCastingSampleStep() {
	return rayCasting.vpGetSampleStep();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingAmbientLight" sends a new  
//				value to the object attribute (ambientLight).
// Parameters.: float a
// Return.....: -

void VPVolume::vpSetRayCastingAmbientLight(float a) {
	rayCasting.vpSetAmbientLight(a);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingAmbientLight" get the 
//				object attribute (ambientLight).
// Parameters.: -
// Return.....: float ambientLight

float VPVolume::vpGetRayCastingAmbientLight() {
	return rayCasting.vpGetAmbientLight();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingDiffuseLight" sends a new  
//				value to the object attribute (diffuseLight).
// Parameters.: float a
// Return.....: -

void VPVolume::vpSetRayCastingDiffuseLight(float d) {
	rayCasting.vpSetDiffuseLight(d);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingDiffuseLight" get the 
//				object attribute (diffuseLight).
// Parameters.: -
// Return.....: float diffuseLight

float VPVolume::vpGetRayCastingDiffuseLight() {
	return rayCasting.vpGetDiffuseLight();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingSpecularExponent" sends a   
//				new value to the object attribute (specularExponent).
// Parameters.: int s
// Return.....: -

void  VPVolume::vpSetRayCastingSpecularExponent(int s) {
	rayCasting.vpSetSpecularExponent(s);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingSpecularExponent" get the 
//				object attribute (specularExponent).
// Parameters.: -
// Return.....: int specularExponent

int   VPVolume::vpGetRayCastingSpecularExponent() {
	return rayCasting.vpGetSpecularExponent();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingSpecular" sends a new  
//				value to the object attribute (specular).
// Parameters.: bool s
// Return.....: -

void  VPVolume::vpSetRayCastingSpecular(bool s) {
	rayCasting.vpSetSpecular(s);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingSpecular" get the 
//				object attribute (specular).
// Parameters.: -
// Return.....: bool specular

bool  VPVolume::vpGetRayCastingSpecular() {
	return rayCasting.vpGetSpecular();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingShadingMethod" sends a new  
//				value to the object attribute (shadingMethod).
// Parameters.: int sm
// Return.....: -

void  VPVolume::vpSetRayCastingShadingMethod(int sm) {
		rayCasting.vpSetShadingMethod(sm);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingShadingMethod" get the 
//				object attribute (shadingMethod).
// Parameters.: -
// Return.....: int shadingMethod

int VPVolume::vpGetRayCastingShadingMethod() {
	return rayCasting.vpGetShadingMethod();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingTypeOfCuttingTool" sends a   
//				new value to the object attribute (typeOfCuttingTool).
// Parameters.: int sm
// Return.....: -

void VPVolume::vpSetRayCastingTypeOfCuttingTool(int t) {
		rayCasting.vpSetTypeOfCuttingTool(t);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingTypeOfCuttingTool" get the 
//				object attribute (typeOfCuttingTool).
// Parameters.: -
// Return.....: int typeOfCuttingTool

int VPVolume::vpGetRayCastingTypeOfCuttingTool(){
	return rayCasting.vpGetTypeOfCuttingTool();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetOpacityComputation" sends a new  
//				value to the object attribute (opacityComputation).			
// Parameters.: float a
// Return.....: -

void VPVolume::vpSetOpacityComputation(int oc){
	opacityComputation = oc;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetOpacityComputation" get the object
//				attribute (opacityComputation) that has the
//				kind of opacity computation that will be used.
// Parameters.: -
// Return.....: int opacityComputation

int VPVolume::vpGetOpacityComputation(){
	return opacityComputation;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetOpacityTable" sends a new  
//				value for the opacity table.			
// Parameters.: float t[]
// Return.....: -

void  VPVolume::vpSetOpacityTable(float t[]) {
	controlTables.vpSetOpacityTable(t); 
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetOpacityTable" gets the 
//				opacity table.
// Parameters.: -
// Return.....: float *

float * VPVolume::vpGetOpacityTable() {
	return controlTables.vpGetOpacityTable();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetColorTable" sends a new  
//				value for the color table.			
// Parameters.: VPColor c[]
// Return.....: -

void  VPVolume::vpSetColorTable(VPColor c[]) {
	controlTables.vpSetColorTable(c); 
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetColorTable" gets the 
//				colot table.
// Parameters.: -
// Return.....: VPColor *

VPColor * VPVolume::vpGetColorTable() {
	return controlTables.vpGetColorTable();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetControlPoints" sends a new  
//				value for the tables control points.
// Parameters.: int cp[], int n
// Return.....: -

void VPVolume::vpSetControlPoints (int cp[], int n) {
	controlTables.vpSetControlPoints(cp, n);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetControlPoints" return the 
//				tables control points.
// Parameters.: -
// Return.....: int cp[], int &n

void VPVolume::vpGetControlPoints(int cp[], int &n) {
	controlTables.vpGetControlPoints(cp, n);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetControlPoints" sends a new  
//				value for the tables control points.
// Parameters.: int cp[], VPColor c[], int n
// Return.....: -

void VPVolume::vpSetControlPoints (int cp[], VPColor c[], int n) {
	controlTables.vpSetControlPoints(cp, c, n);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetControlPoints" return the 
//				tables control points.
// Parameters.: -
// Return.....: int cp[], VPColor c[], int &n

void VPVolume::vpGetControlPoints(int cp[], VPColor c[], int &n) {
	controlTables.vpGetControlPoints(cp, c, n);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpProcessHistogram" process the histogram 
//				and save it in a file.
// Parameters.: -
// Return.....: -

void  VPVolume::vpProcessHistogram() {
	int i, j, k, histogram[256], maxSample=0;
    FILE *fp, *fp1;

	for (i=0; i<256; i++)
		histogram[i] = 0;

	for (k=0; k<yDimension; k++) 
		for (i=0; i<zDimension; i++) 
			for (j=0; j<xDimension; j++) 
				if (volumeMatrix[j][k][i]<256)
					histogram[volumeMatrix[j][k][i]]++;

	for (i=0; i<256; i++)
		if (histogram[i] > maxSample)
			maxSample = histogram[i];

	fp = fopen("histograma.txt", "w");
	fp1 = fopen("excel.txt", "w");

	for (i=0; i<256; i++)
	{
		fprintf(fp, "%d - ", i);
		fprintf(fp1, "%d \n", histogram[i]);
		k = ( 3500 * histogram[i] ) / maxSample; 
		for (j=0; j<k; j++)
			fprintf(fp, "*");
		fprintf(fp, "\n");
	}

	fclose(fp);
	fclose(fp1); 
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpProcessCenterFocalPoint" calcules the 
//				value of the centerFocalPoint attribute (CENTRO DO
//				OBJETO/ÓRGÃO).
// Parameters.: -
// Return.....: -

void VPVolume::vpProcessCenterFocalPoint() {
	int i=0, k=0, x=0, z=0, min=0, max=0, density=0, distance=0, sum=0, aux=0;

	// X processing
	while ( (density<50) && (min<(xDimension-1)) ) {
		density = volumeMatrix[min][yDimension/2][zDimension/2];
		min++;
	}
	max = xDimension-1;
	density = 0;
	while ( (density<50) && (max>0) ) {
		density = volumeMatrix[max][yDimension/2][zDimension/2];
		max--;
	}
	centerFocalPoint.x = ((float)max + (float)min) / 2.0;

	// Y processing
	x = xDimension/3;
	z = zDimension/3;
	for (i=x; i<x*2; i++) {
		for (k=z; k<z*2; k++) {
			min = density = 0;
			while ( (density<50) && (min<(yDimension-1)) ) {
				density = volumeMatrix[i][min][k];
				min++;
			}
			max = yDimension-1;
			density = 0;
			while ( (density<50) && (max>0) ) {
				density = volumeMatrix[i][max][k];
				max--;
			}
			aux = max - min;
			if (distance < aux) {
				distance = aux;
				sum = max + min;
			}
		}
	}
	centerFocalPoint.y = (float)sum / 2.0;

	// Z processing
	// OBS: POR ENQUANTO NÃO LEVAR EM CONTA O PROBLEMA DO NARIZ...
	density = min = 0;
	while ( (density<50) && (min<(zDimension-1)) ) {
		density = volumeMatrix[(int)centerFocalPoint.x][(int)centerFocalPoint.y][min];
		min++;
	}
	density = 0;
	max = zDimension-1;
	while (density<50) {
		density = volumeMatrix[(int)centerFocalPoint.x][(int)centerFocalPoint.y][max];
		max--;
	}
	centerFocalPoint.z = ((float)max + (float)min) / 2.0;

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetCenterFocalPoint" returns the value 
//				of the centerFocalPoint attribute.
// Parameters.: -
// Return.....: VPPoint3D

VPPoint3D VPVolume::vpGetCenterFocalPoint() {
	return centerFocalPoint;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetMinVolumeColor" returns the value 
//				of the minVolumeColor attribute.
// Parameters.: -
// Return.....: unsigned int

unsigned int VPVolume::vpGetMinVolumeColor() {
	return minVolumeColor;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetMaxVolumeColor" returns the value 
//				of the maxVolumeColor attribute.
// Parameters.: -
// Return.....: unsigned int

unsigned int VPVolume::vpGetMaxVolumeColor() {
	return maxVolumeColor;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetControlTables" returns the value 
//				of the controlTables attribute.
// Parameters.: -
// Return.....: VPTable controlTables

VPTable VPVolume::vpGetControlTables() {
	return controlTables;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpvpSwapBytes" is used because the datasets
//				from the "Introduction to Volume Rendering" cd-rom
//				are all written on a unix system, which has reverse 
//				endian.
// Parameters.: GLint *intPtr, pointer of words requiring byte swapping
//				int sizeBytes, number of bytes in array
// Return.....: -

GLvoid VPVolume::vpSwapBytes(GLint *intPtr, int sizeBytes) {
    GLint       sizeInts, i;
    GLubyte     *bytePtr;

    sizeInts = sizeBytes / 4;
    for (i = 0; i < sizeInts; i++)
    {
        bytePtr = (GLubyte *) intPtr;
        *intPtr++ = ((bytePtr[0]<<24) | (bytePtr[1]<<16) | (bytePtr[2]<<8) |
            bytePtr[3]);
    }
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpAdjustScale" is used to set scale 
//				attribute, which is used because the volume is
//				no isotropic.
// Parameters.: int x, int y, int z, virtual volume dimensions
// Return.....: -

void VPVolume::vpAdjustScale(int x, int y, int z) {

	if ( (xDimension>=yDimension) && (xDimension>=zDimension) ) 
	{
		scale.x = 1;
		scale.y = (float) yDimension/xDimension;
		scale.z = (float) zDimension/xDimension;
	}
	else if ( (yDimension>=xDimension) && (yDimension>=zDimension) )
	{
		scale.y = 1;
		scale.x = (float) xDimension/yDimension;
		scale.z = (float) zDimension/yDimension;
	}
	else 
	{
		scale.z = 1;
		scale.x = (float) xDimension/zDimension;
		scale.y = (float) yDimension/zDimension;
	}

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetScale" returns the value of the
//				scale attribute.
// Parameters.: -
// Return.....: VPPoint3D scale

VPPoint3D VPVolume::vpGetScale() {
	return scale;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetSliceViewSBandPoint" returns the value  
//				of enfOfSBand attribute.
// Parameters.: -
// Return.....: VPPoint2D enfOfSBand (attribute value)

VPPoint2D VPVolume::vpGetSliceViewSBandPoint() {
	return slice.vpGetViewSBandPoint(); // enfOfSBand ("view coordinate")
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetSliceViewTBandPoint" returns the value  
//				of enfOfTBand attribute.
// Parameters.: -
// Return.....: VPPoint2D enfOfTBand (attribute value)

VPPoint2D VPVolume::vpGetSliceViewTBandPoint() {
	return slice.vpGetViewTBandPoint(); // enfOfTBand ("view coordinate")
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetFileType" returns the value of the
//				fileType attribute.
// Parameters.: -
// Return.....: int fileType (attribute value)

int VPVolume::vpGetFileType() {
	return fileType; 
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetCameraLocationForInnerStructure" sends    
//				a new value for the cameraLocationForInnerStructure 
//				attribute.
// Parameters.: -
// Return.....: -

void VPVolume::vpSetCameraLocationForInnerStructure(VPPoint3D c) {
	cameraLocationForInnerStructure = c;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetCameraLocationForInnerStructure" 
//				returns the value of the
//				cameraLocationForInnerStructure attribute.
// Parameters.: -
// Return.....: VPPoint3D cameraLocationForInnerStructure 
//				(attribute value)

VPPoint3D VPVolume::vpGetCameraLocationForInnerStructure(){
	return cameraLocationForInnerStructure;
}

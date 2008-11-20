///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpgraphicbj.h
//  DESCRIPTION.: Contain the VPGraphicObj class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 25/July/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPGRAPHICOBJ_H
#define __VPGRAPHICOBJ_H


#ifndef __VPSCENE_H
class	VPScene;
#endif

#include	<vpmatrix.h>
#include	<vppoint.h>
#include	<string>	//STL include
//using namespace std;

//#define	IVPATH "/home/amaciel/Mestrado/Dissertacao/BONES/"
//#define	IVPATH "/home/amaciel/VPATBONES/"
//#define	IVPATH "/home/amaciel/VPATBONES/joelho/"
#define IVPATH "/vpat/data/"

///////////////////////////////////////////////////////////////////
// Class Name: VPGraphicObj
// Superclass: none
// Subclasses: VPImage, VPMesh, VPSphere, VPCylinder, VPBox, VPCone, VPCurve

class VPGraphicObj {

	private:
		bool show;
		
	protected:
		std::string		fileName;
		std::string		description;
		VPMatrix 	localInstanceMatrix;
		VPMatrix 	globalInstanceMatrix;
		VPScene*	scene;

	public:
		VPGraphicObj( void );
		VPGraphicObj( VPScene *sc );
		virtual ~VPGraphicObj( void );

		std::string		vpGetFileName();
		void			vpSetFileName( std::string fName );
		std::string		vpGetDescription();
		void			vpSetDescription( std::string desc );
		VPMatrix*	vpGetLocalInstanceMatrix();
		void			vpSetLocalInstanceMatrix( VPMatrix* );
		VPMatrix*	vpGetGlobalInstanceMatrix();
		void			vpSetGlobalInstanceMatrix( VPMatrix* );
		VPScene*	vpGetScene();
		void		vpSetScene( VPScene* sc );
		void		vpShow( void );
		void		vpHide( void );
		//virtual	void	vpRender() = 0;

		//virtual int	vpGetXDimension();
		//virtual int	vpGetYDimension();
		//virtual int	vpGetZDimension();
		//virtual void	vpTranslate( VPPoint3D );
		//virtual void	vpScale( VPPoint3D );
		//virtual void	vpRotateX( float );
		//virtual void	vpRotateY( float );
		//virtual void	vpRotateZ( float );

};

#endif


#ifndef	__VPINVENTORVIEW_H
#define	__VPINVENTORVIEW_H

#include	<math.h>
#include	<Inventor/nodes/SoSeparator.h>
#include	<Inventor/nodes/SoTransform.h>

#include	<vpview.h>
#include	<vpscene.h>
#include	<vpbody.h>
#include	<vpmovement.h>


class VPInventorView : public VPView{

	private:
	
		//VPScene *vpatScene;
		//VPBody	*vpatBody;
		VPMovement simulation;
		

	public:
		
		VPInventorView( VPScene * );
		VPInventorView( VPScene *, VPBody * );
		SoSeparator* loadFile( const char *fileName );
		//void WorkProc ( void *, SoSensor * );
		void MainLoop ();
		SoSeparator*	loadVPScene( VPScene * );
		
		void		vpDraw();
		void		vpRefresh();
};

#endif
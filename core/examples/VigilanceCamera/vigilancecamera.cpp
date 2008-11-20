/// \file vigilancecamera.cpp
/// \brief Example class for VPAT (implementation)
/// \version $Revision: 1.3 $

// ChangeLog
// Mar 17, 2006 - Bruno de Oliveira Schneider
// - Uses new method VPCylinder::SetPartsVisibility.
// Mar 21, 2005 - Bruno de Oliveira Schneider
// - Added ComputeBoundingBox.

#include "vigilancecamera.h"
#include "pi.h"

//?
#include <iostream>
using namespace std;

clVigilanceCamera::clVigilanceCamera()
{
    VPMaterial lGreyPlastic(VPColor(204,204,220));
    VPMaterial dGreyPlastic(VPColor(127,127,127));
    base.SetHeight(1);
    base.SetRadius(1.5);
    base.SetDescription("base");
    base.SetMaterial(dGreyPlastic);
    base.SetPartsVisibility(VPCylinder::ALL);
    globeTrans.MakeTranslation(0,0,1.5001);
    globeTrans.SetDescription("globeTrans");
    base.AddChild(globeTrans);
    flexion.Set(VPPoint4D::Z(), VPPoint4D::ORIGIN(), -PI_FLOAT, PI_FLOAT);
    flexion.SetDescription("flexion");
    aduction.Set(VPPoint4D::X(), VPPoint4D::ORIGIN(), 0, PIS2_F);
    aduction.SetDescription("aduction");
    joint.AddDof(&flexion);
    joint.AddDof(&aduction);
    joint.SetDescription("joint");
    globeTrans.AddChild(joint);
    globe.SetRadius(1.5);
    globe.SetMaterial(lGreyPlastic);
    joint.AddChild(globe);
    lensTrans.MakeTranslation(0,0,1.4);
    joint.AddChild(lensTrans);
    lens.SetHeight(0.2);
    lens.SetRadius(0.4);
    lens.SetPartsVisibility(VPCylinder::NO_BOTTOM);
    lens.SetDescription("lens");
    lens.SetMaterial(lGreyPlastic);
    lensTrans.AddChild(lens);
    lensTrans.SetDescription("lensTrans");
}

void clVigilanceCamera::SimulationStep()
{
}

void clVigilanceCamera::Flex(float variance)
{
    flexion.Move(variance);
}
void clVigilanceCamera::Adduct(float variance)
{
    aduction.Move(variance);
}

bool clVigilanceCamera::DrawInstanceOGL() const
{
    base.DrawOGL();
    return true;
}

void clVigilanceCamera::ComputeBoundingBox() {
    //FixMe: Computing the bbox according to the current joint state would be better.
    bBox.SetBoundingBox(-1.5, -1.5, 0, 1.5, 1.5, 3.1);
}

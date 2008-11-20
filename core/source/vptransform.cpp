/// \file vptransform.cpp
/// \brief Implementation file for VPTransform VPAT class.
/// \version $Revision: 1.9 $

// ChangeLog
// May 23, 2006 - Bruno de Oliveira Schneider
// - Fixed a few bugs in RecursiveBoundingBox;
// - RecursiveBoundingBox now checks the type of its children to call the right
//   bounding box method.
// May 08, 2006 - Dalton Reis
// - Added a RecursiveBoundingBox method
// Mar 13, 2006 - Bruno de Oliveira Schneider 
// - Attribute "matrix" changed from private to protected.
// Feb 20, 2006 - Andreia Schneider
// - Added the method SetData, that modifies all data of the matrix.
// - Removed const of the VPSceneNode in method DrawOGL.
// May 17, 2005 - Bruno de Oliveira Schneider
// - Added HasNaN for debugging purposes.
// Sep 15, 2004 - Bruno de Oliveira Schneider
// - File created.

// Internal matrix element organization reference
//             [ matrix[0] matrix[4] matrix[8]  matrix[12] ]
// Transform = [ matrix[1] matrix[5] matrix[9]  matrix[13] ]
//             [ matrix[2] matrix[6] matrix[10] matrix[14] ]
//             [ matrix[3] matrix[7] matrix[11] matrix[15] ]

#include "vptransform.h"
#include "vpboundingbox.h"
#include "vpgraphicobj.h"

#ifdef VP_OGL
#include <GL/gl.h>
#endif

using namespace std;

VPTransform::VPTransform()
{
}

void VPTransform::SetData(double* data)
{
    int i;

    for (i=0; i<16; i++)
    {
        matrix[i] = (*data);
        data++;
    }
}

VPTransform::VPTransform(const VPTransform &trans)
{
    for (int i=0; i<16; ++i)
        matrix[i] = trans.matrix[i];
}

void VPTransform::MakeIdentity()
{
    for (int i=0; i<16; ++i)
        matrix[i] = 0.0;
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
}

void VPTransform::MakeTranslation(const VPPoint4D& translationVector)
{
    MakeIdentity();
    matrix[12] = translationVector.GetX();
    matrix[13] = translationVector.GetY();
    matrix[14] = translationVector.GetZ();
}

void VPTransform::MakeXRotation(double radians)
{
    MakeIdentity();
    matrix[5] =   cos(radians);
    matrix[9] =  -sin(radians);
    matrix[6] =   sin(radians);
    matrix[10] =  cos(radians);
}

void VPTransform::MakeYRotation(double radians)
{
    MakeIdentity();
    matrix[0] =   cos(radians);
    matrix[8] =   sin(radians);
    matrix[2] =  -sin(radians);
    matrix[10] =  cos(radians);
}

void VPTransform::MakeZRotation(double radians)
{
    MakeIdentity();
    matrix[0] =  cos(radians);
    matrix[4] = -sin(radians);
    matrix[1] =  sin(radians);
    matrix[5] =  cos(radians);
}

void VPTransform::MakeScale(double sX, double sY, double sZ)
{
    MakeIdentity();
    matrix[0] =  sX;
    matrix[5] =  sY;
    matrix[10] = sZ;
}

void VPTransform::MakeShear(double shX, double shY)
{
    MakeIdentity();
    matrix[8] = shX;
    matrix[9] = shY;
}

VPPoint4D VPTransform::operator *(const VPPoint4D& point) const
{
    VPPoint4D result;

    return VPPoint4D(  matrix[0]*point.GetX()  + matrix[4]*point.GetY()
                     + matrix[8]*point.GetZ()  + matrix[12]*point.GetW(),
                       matrix[1]*point.GetX()  + matrix[5]*point.GetY()
                     + matrix[9]*point.GetZ()  + matrix[13]*point.GetW(),
                       matrix[2]*point.GetX()  + matrix[6]*point.GetY()
                     + matrix[10]*point.GetZ() + matrix[14]*point.GetW(),
                       matrix[3]*point.GetX()  + matrix[7]*point.GetY()
                     + matrix[11]*point.GetZ() + matrix[15]*point.GetW());
}

VPTransform VPTransform::operator*(const VPTransform &t) const
{
    VPTransform resultado;
    for (int i=0; i < 16; ++i)
        resultado.matrix[i] =
              matrix[i%4]    *t.matrix[i/4*4]  +matrix[(i%4)+4] *t.matrix[i/4*4+1]
            + matrix[(i%4)+8]*t.matrix[i/4*4+2]+matrix[(i%4)+12]*t.matrix[i/4*4+3];
    return resultado;
}

VPTransform& VPTransform::operator=(const VPTransform& t)
{
    for (int i=0; i < 16; ++i)
        matrix[i] = t.matrix[i];
    return *this;
}

void VPTransform::ApplyTo(VPPoint4D* ptPoint) const
{
    ptPoint->SetXYZW(
        matrix[0]*ptPoint->GetX()
            + matrix[4]*ptPoint->GetY()
            + matrix[8]*ptPoint->GetZ()
            + matrix[12]*ptPoint->GetW(),
        matrix[1]*ptPoint->GetX()
            + matrix[5]*ptPoint->GetY()
            + matrix[9]*ptPoint->GetZ()
            + matrix[13]*ptPoint->GetW(),
        matrix[2]*ptPoint->GetX()
            + matrix[6]*ptPoint->GetY()
            + matrix[10]*ptPoint->GetZ()
            + matrix[14]*ptPoint->GetW(),
        matrix[3]*ptPoint->GetX()
            + matrix[7]*ptPoint->GetY()
            + matrix[11]*ptPoint->GetZ()
            + matrix[15]*ptPoint->GetW()
        );
}

void VPTransform::MakeRotation(const VPPoint4D& refVec, const float radians)
{
    VPPoint4D projEmY;
    VPPoint4D vetTemp;
    VPTransform tTemp;
    double anguloY;
    double anguloZ;

    //Decompor refVec
    //Angulo em Y (para levar o vetor ao plano XY)
    if (Zero(refVec.GetZ()))
    {
        anguloY = 0.0;
        this->MakeIdentity();
        vetTemp = refVec;
    }
    else
    {
        // se o vetor nao esta no plano XY...
        projEmY.SetXYZW(refVec.GetX(), 0, refVec.GetZ(), 0);
        projEmY.Normalize();
        if (refVec.GetZ() < 0)
            anguloY = -(projEmY.AngleTo(VPPoint4D(1,0,0,0)));
        else
            anguloY = projEmY.AngleTo(VPPoint4D(1,0,0,0));
        this->MakeYRotation(anguloY);
        vetTemp = (*this) * refVec; //refVec no plano XY
    }
    //Angulo em Z (para levar o vetor ao X)
    if (vetTemp.GetY() < 0)
        anguloZ = vetTemp.AngleTo(VPPoint4D(1,0,0,0));
    else
        anguloZ = -(vetTemp.AngleTo(VPPoint4D(1,0,0,0)));
    tTemp.MakeZRotation(anguloZ);
    (*this) = tTemp * (*this);
    //Rodar
    tTemp.MakeXRotation(radians);
    (*this) = tTemp * (*this);
    //voltar
    tTemp.MakeZRotation(-anguloZ);
    (*this) = tTemp * (*this);
    tTemp.MakeYRotation(-anguloY);
    (*this) = tTemp * (*this);
}

void VPTransform::MakeRotation(const VPPoint4D& refPoint, const VPPoint4D& refVec,
                               const float radians)
{
    VPTransform tTemp;

    this->MakeTranslation(-refPoint);
    tTemp.MakeRotation(refVec, radians);
    (*this) = tTemp * (*this);
    tTemp.MakeTranslation(refPoint);
    (*this) = tTemp * (*this);
}

void VPTransform::GetVectorX(VPPoint4D* result) const
{
    result->SetX(matrix[0]);
    result->SetY(matrix[1]);
    result->SetZ(matrix[2]);
    result->SetW(matrix[3]);
}

void VPTransform::GetVectorY(VPPoint4D* result) const
{
    result->SetX(matrix[4]);
    result->SetY(matrix[5]);
    result->SetZ(matrix[6]);
    result->SetW(matrix[7]);
}

void VPTransform::GetVectorZ(VPPoint4D* result) const
{
    result->SetX(matrix[8]);
    result->SetY(matrix[9]);
    result->SetZ(matrix[10]);
    result->SetW(matrix[11]);
}

void VPTransform::GetTranslation(VPPoint4D* result) const
{
    result->SetX(matrix[12]);
    result->SetY(matrix[13]);
    result->SetZ(matrix[14]);
    result->SetW(matrix[15]);
}

bool VPTransform::DrawOGL() const
{
#ifdef VP_OGL
    bool result = true;
    list<VPSceneNode*>::const_iterator iter;

    glPushMatrix();
    glMultMatrixd(matrix);

    for (iter = childList.begin(); iter != childList.end(); ++iter)
        result &= (*iter)->DrawOGL();
    glPopMatrix();
    return result;
#else
    return false;
#endif
}

bool VPTransform::RecursiveBoundingBox(VPBoundingBox* bBox) {
// virtual method

// Note: Bounding boxes from children should be transformed first and then merged.
// If merged before transforming, errors will occour because VPBoundingBox::MergeWith
// expects aligned bounding boxes.

    VPBoundingBox box; // must not be static because it is used in the recursion
    bool initBBox = false;
    list<VPSceneNode*>::const_iterator iter;
    VPGraphicObj* objPtr;
    VPTransform* transPtr;

    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        objPtr = dynamic_cast<VPGraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ComputeRecursiveBoundingBox();
            box = objPtr->GetRecursiveBoundingBox();
            box.Transform(*this);
            if (initBBox)
                bBox->MergeWith(box);
            else {
                bBox->CopyGeometryFrom(box);
                initBBox = true;
            }
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VPTransform*>(*iter);
            if (transPtr) { // object is a transform
                if (transPtr->RecursiveBoundingBox(&box)) { // object has graphic descendents
                    box.Transform(*this);
                    if (initBBox)
                        bBox->MergeWith(box);
                    else {
                        bBox->CopyGeometryFrom(box);
                        initBBox = true;
                    }
                }
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
    if (initBBox)
        bBox->ProcessCenter();
    return initBBox;
}

#ifndef NDEBUG
bool VPTransform::HasNaN() const
{
    for (int i=0; i < 16; ++i)
    {
        if (isnan(matrix[i]))
            return true;
    }
    return false;
}
#endif

ostream& operator<<(ostream& output, const VPTransform& t)
{
    int i, j;

    output.setf(ios::showpoint|ios::fixed);
    output.precision(6);
    for (i=0; i<4; ++i)
    {
        for (j=0; j<4; ++j)
            output << setw(12) << t.matrix[i+j*4] << " ";
        output << endl;
    }
    output.unsetf(ios::showpoint|ios::fixed);
    return output;
}

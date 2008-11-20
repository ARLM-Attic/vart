/// \file vppoint4d.cpp
/// \brief Implementation file for VPVppoint4d VPAT class.
/// \version 1.5

// ChangeLog
// Sep 27, 2005 - Bruno de Oliveira Schneider
// - Included cmath.
// Apr 29, 2005 - Bruno de Oliveira Schneider
// - Changed "AngleTo" - it was still returning "NaN".
// Apr 14, 2005 - Bruno de Oliveira Schneider
// - Changed "AngleTo" - it was returning "NaN" because of floating point errors.
//   This used to make the scene disappear from the viewer when rotating the camera.
// Nov 24, 2004 - Bruno de Oliveira Schneider
// - Added configurable order comparison methods: operator<, XThenZLess and
//   LexicographicalLess.
// Sep 28, 2004 - Bruno de Oliveira Schneider
// - File was created from the translation to english of an old class of mine.
// - Added cast constructors and some implementations of "operator=" to easily
//   blend into VPAT.

// FixMe: Some comments still need to be translated to english.

#include "vppoint4d.h"
#include <cmath>

using namespace std;

// Default "less then" comparison uses "LexicographicalLess".
VPPoint4D::tCompareOper VPPoint4D::operatorLess = &VPPoint4D::LexicographicalLess;

VPPoint4D::VPPoint4D()
{
    vetCoord[0]=vetCoord[1]=vetCoord[2]=0;
    vetCoord[3]=1;
}

VPPoint4D::VPPoint4D(const VPPoint4D &point)
{
    vetCoord[0] = point.vetCoord[0];
    vetCoord[1] = point.vetCoord[1];
    vetCoord[2] = point.vetCoord[2];
    vetCoord[3] = point.vetCoord[3];
}

VPPoint4D::VPPoint4D(const VPVertex3D& v)
{
    vetCoord[0] = v.vpGetX();
    vetCoord[1] = v.vpGetY();
    vetCoord[2] = v.vpGetZ();
    vetCoord[3] = 1;
}

VPPoint4D::VPPoint4D(const VPPoint3D& p)
{
    vetCoord[0] = p.vpGetX();
    vetCoord[1] = p.vpGetY();
    vetCoord[2] = p.vpGetZ();
    vetCoord[3] = 1;
}

VPPoint4D::VPPoint4D(const VPVector3D& v)
{
    vetCoord[0] = v.vpGetX();
    vetCoord[1] = v.vpGetY();
    vetCoord[2] = v.vpGetZ();
    vetCoord[3] = 0;
}

VPPoint4D::VPPoint4D(double x, double y, double z, double w)
// w = 1 (point) by default (see constructor signature)
{
    SetXYZW(x, y, z, w);
}

const VPPoint4D& VPPoint4D::X()
{
    static const VPPoint4D x(1.0,0.0,0.0,0.0);
    return x;
}

const VPPoint4D& VPPoint4D::Y()
{
    static const VPPoint4D y(0.0,1.0,0.0,0.0);
    return y;
}

const VPPoint4D& VPPoint4D::Z()
{
    static const VPPoint4D z(0.0,0.0,1.0,0.0);
    return z;
}

const VPPoint4D& VPPoint4D::DOWN()
{
    static const VPPoint4D down(0.0,-1.0,0.0,0.0);
    return down;
}

const VPPoint4D& VPPoint4D::ORIGIN()
{
    static const VPPoint4D origin(0.0,0.0,0.0,1.0);
    return origin;
}

void VPPoint4D::SetXYZW(double x, double y, double z, double w)
{
    vetCoord[0] = x;
    vetCoord[1] = y;
    vetCoord[2] = z;
    vetCoord[3] = w;
}

void VPPoint4D::Normalize()
// requires that x,y,z and w are not all zeros
{
    if (vetCoord[3] == 0)
    {
        // vector normalization
        double norma = Length();

        vetCoord[0] /= norma;
        vetCoord[1] /= norma;
        vetCoord[2] /= norma;
        vetCoord[3] = 0; // this seems to avoid some round off erros
    }
    else
    {
        // point normalization
        vetCoord[0] /= vetCoord[3];
        vetCoord[1] /= vetCoord[3];
        vetCoord[2] /= vetCoord[3];
        vetCoord[3] = 1;
    }
}

double VPPoint4D::Length() const
{
    return sqrt(vetCoord[0]*vetCoord[0] + vetCoord[1]*vetCoord[1] + vetCoord[2]*vetCoord[2]);
}

double VPPoint4D::DotProduct(const VPPoint4D& p) const
{
    return vetCoord[0]*p.vetCoord[0]+vetCoord[1]*p.vetCoord[1]+vetCoord[2]*p.vetCoord[2];
}

VPPoint4D VPPoint4D::CrossProduct(const VPPoint4D& p) const
{
    return VPPoint4D(GetY()*p.GetZ()-GetZ()*p.GetY(), GetZ()*p.GetX()-GetX()*p.GetZ(), GetX()*p.GetY()-GetY()*p.GetX(), 0);
}

double VPPoint4D::AngleTo(const VPPoint4D& p) const
// Note: This method requires both vectors to be normalized!
{
    double dp = DotProduct(p);
    // Sometimes, the Dot Product is greater then 1 due to floating point errors.
    if (dp > 1.0)
        return 0;
    else
    {
        if (dp < -1.0)
            return 3.1415926535897932384626433832795;
        return acos(dp);
    }
}

VPPoint4D VPPoint4D::operator-() const
{
    // To keep the nature of the point it seems to me that the w coordinate should
    // not be touched.
    return VPPoint4D(-vetCoord[0], -vetCoord[1], -vetCoord[2], vetCoord[3]);
}

VPPoint4D VPPoint4D::operator-(const VPPoint4D& point) const
{
    return VPPoint4D(vetCoord[0]-point.vetCoord[0],
                     vetCoord[1]-point.vetCoord[1],
                     vetCoord[2]-point.vetCoord[2],
                     vetCoord[3]-point.vetCoord[3]);
}

VPPoint4D VPPoint4D::operator+(const VPPoint4D& vector) const
// A principio este operador nao deveria somar ponto com ponto. Entretanto, a soma
// de vetor - criado pela diferenca de pontos nao normalizados - com um ponto nao
// e' detectavel pela coordenada W e pode ser usado no algoritmo de recorte.
{
    return VPPoint4D(vetCoord[0]+vector.vetCoord[0],
                     vetCoord[1]+vector.vetCoord[1],
                     vetCoord[2]+vector.vetCoord[2],
                     vetCoord[3]+vector.vetCoord[3]);
}

void VPPoint4D::operator+=(const VPPoint4D& vector)
// requer normalizacao do ponto sendo alterado
{
    vetCoord[0] += vector.vetCoord[0];
    vetCoord[1] += vector.vetCoord[1];
    vetCoord[2] += vector.vetCoord[2];
}

VPPoint4D VPPoint4D::operator*(double escalar) const
{
    return VPPoint4D(vetCoord[0]*escalar,
                     vetCoord[1]*escalar,
                     vetCoord[2]*escalar,
                     vetCoord[3]*escalar);
}

void VPPoint4D::operator*=(double escalar)
{
    vetCoord[0]*=escalar;
    vetCoord[1]*=escalar;
    vetCoord[2]*=escalar;
    vetCoord[3]*=escalar;
}

bool VPPoint4D::operator==(const VPPoint4D& point) const
{
    return (vetCoord[0] == point.vetCoord[0]) &&
           (vetCoord[1] == point.vetCoord[1]) &&
           (vetCoord[2] == point.vetCoord[2]) &&
           (vetCoord[3] == point.vetCoord[3]);
}

bool VPPoint4D::operator<(const VPPoint4D& p) const
{
    return (this->*operatorLess)(p);
}

bool VPPoint4D::operator!=(const VPPoint4D& point) const
{
    return (vetCoord[0] != point.vetCoord[0]) ||
           (vetCoord[1] != point.vetCoord[1]) ||
           (vetCoord[2] != point.vetCoord[2]) ||
           (vetCoord[3] != point.vetCoord[3]);
}

void VPPoint4D::operator=(const VPPoint4D& point)
{
    vetCoord[0] = point.vetCoord[0];
    vetCoord[1] = point.vetCoord[1];
    vetCoord[2] = point.vetCoord[2];
    vetCoord[3] = point.vetCoord[3];
}

void VPPoint4D::operator=(const VPPoint3D& p)
{
    vetCoord[0] = p.vpGetX();
    vetCoord[1] = p.vpGetY();
    vetCoord[2] = p.vpGetZ();
    vetCoord[3] = 1;
}

void VPPoint4D::operator=(const VPVertex3D& v)
{
    vetCoord[0] = v.vpGetX();
    vetCoord[1] = v.vpGetY();
    vetCoord[2] = v.vpGetZ();
    vetCoord[3] = 1;
}

void VPPoint4D::operator=(const VPVector3D& v)
{
    vetCoord[0] = v.vpGetX();
    vetCoord[1] = v.vpGetY();
    vetCoord[2] = v.vpGetZ();
    vetCoord[3] = 0;
}

VPPoint4D VPPoint4D::operator/( double escalar ) const
{
    return VPPoint4D(vetCoord[0]/escalar,
                     vetCoord[1]/escalar,
                     vetCoord[2]/escalar,
                     vetCoord[3]/escalar);
}

bool VPPoint4D::LexicographicalLess(const VPPoint4D& p) const
{
    if (vetCoord[0] == p.vetCoord[0])
    {
        if (vetCoord[1] == p.vetCoord[1])
        {
            if (vetCoord[2] == p.vetCoord[2])
                return vetCoord[3] < p.vetCoord[3];
            else
                return vetCoord[2] < p.vetCoord[2];
        }
        else
            return vetCoord[1] < p.vetCoord[1];
    }
    else
        return vetCoord[0] < p.vetCoord[0];
}

bool VPPoint4D::XThenZLess(const VPPoint4D& p) const
{
    if (vetCoord[0] == p.vetCoord[0])
        return vetCoord[2] < p.vetCoord[2];
    else
        return vetCoord[0] < p.vetCoord[0];
}

ostream& operator<<(ostream& output, const VPPoint4D& p)
{
    output.setf(ios::fixed);
    output.precision(5);
    output << "(" << p.GetX() << "," << p.GetY() << "," << p.GetZ() << ","
           << p.GetW() << ")";
    output.unsetf(ios::fixed);
    return output;
}

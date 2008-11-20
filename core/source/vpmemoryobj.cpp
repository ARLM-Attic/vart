/// \file vpmemoryobj.cpp
/// \brief Implementation file for VPMemoryObj VPAT class.
/// \version $Revision: 1.3 $

// ChangeLog
// May 17, 2006 - Bruno de Oliveira Schneider
// - autoDelete is now initialized with "true".
// May 18, 2005 - Bruno de Oliveira Schneider
// - Added some documentation.
// Nov 29, 2004 - Bruno de Oliveira Schneider
// - File created.

#include "vpmemoryobj.h"

//#include <iostream>
//using namespace std;

VPMemoryObj::VPMemoryObj()
{
    autoDelete = true;
}

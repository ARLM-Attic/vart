/// \file descriptionlocator.cpp
/// \brief Implementation file for V-ART class "DescriptionLocator".
/// \version $Revision: 1.2 $

#include "vart/descriptionlocator.h"
#include "vart/scenenode.h"

//#include <iostream>
//using namespace std;

VART::DescriptionLocator::DescriptionLocator(const std::string& description)
                                                                    : targetDescription(description)
{
}

//virtual
void VART::DescriptionLocator::OperateOn(SceneNode* nodePtr)
{
    if (nodePtr->GetDescription() == targetDescription)
        notFound = false;
}

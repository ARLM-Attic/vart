/// \file vpmemoryobj.h
/// \brief Header file for VPMemoryObj VPAT class.
/// \version $Revision: 1.5 $

// ChangeLog is at the implementation file.

#ifndef __VPMEMORYOBJ_H
#define __VPMEMORYOBJ_H

/// \class VPMemoryObj vpmemoryobj.h
/// \brief A "Memory Object" is a VPAT object that has some memory allocation
/// attributes, allowing VPAT to auto deallocate its memory space.
///
/// A VPAT program typically needs to allocate memory for several scene objects that
/// should be deallocated upon application exit. VPAT code typically follows the rule that
/// memory allocated by the programmer should be deallocated by her, while memory allocated
/// by itself should be deallocated by itself. However, creating lots of graphical objects,
/// joints and DOFs, and keeping track of their memory positions is tedious hard work, that
/// may be avoided using some kind of memory management. The use of the autoDelete flag
/// helps creating a fast, user-controlled memory management.
///
/// \bug GLUT applications must call glutMainLoop which never returns, therefore stdlib's
/// exit function must be called to end an application. This prevents stack objects from
/// being destructed, which makes the creation on memory management policies based on
/// destructors a lot harder. It is recommended that key objects in the application (the
/// scene, the viewer, the lights) are declared "static" to ensure activation of their
/// destructors upon application exit.
class VPMemoryObj
{
    public:
        /// \brief Default constructor.
        ///
        /// Initializes the autoDelete flag with true, i.e.: all Memory Objects are deallocated
        /// by default upon application termination.
        VPMemoryObj();
        bool autoDelete;
};

#endif

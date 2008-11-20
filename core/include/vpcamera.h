/// \file vpcamera.h
/// \brief Header file for VPCamera VPAT class.
/// \version $Revision: 1.8 $

// ChangeLog is at the implementation file

#ifndef __VPCAMERA_H
#define __VPCAMERA_H

#include "vppoint4d.h"
#include <string>    //STL include

/// \class VPCamera vpcamera.h
/// \brief Virtual camera.
///
/// FixMe: A detailed class description is needed here.
class VPCamera {
    friend std::ostream& operator<<(std::ostream& output, const VPCamera& cam);
    public:
        enum ProjectionType { ORTHOGRAPHIC, PERSPECTIVE };
        VPCamera();
        /// Creates a camera.
        /// \param position [in] Location of the new camera
        /// \param focus [in] Position at which the new camera is looking at
        /// \param upVec [in] Up vector specifing camera's orientation
        VPCamera(const VPPoint4D& position, const VPPoint4D& focus, const VPPoint4D& upVec);
        VPCamera(const VPCamera& cam);
        VPCamera& operator=(const VPCamera& cam);
        /// Gets the description associated with the camera.
        const std::string& GetDescription() const { return description; }
        /// Sets the description associated with the camera.
        void SetDescription(const std::string& descriptionValue);
        /// Gets the projection type (PERSPECTIVE/ORTHOGRAPHIC).
        ProjectionType GetProjectionType() const { return projectionType; }
        /// Sets the projection type (PERSPECTIVE/ORTHOGRAPHIC).
        void SetProjectionType(ProjectionType newValue) { projectionType = newValue; }
        /// Gets the camera's width/height ratio.
        float GetAspectRatio() const { return aspectRatio; }
        /// Sets the camera's width/height ratio.
        void SetAspectRatio(float newAR) { aspectRatio = newAR; }
        /// Returns the distance to the near plane.
        float GetNearPlaneDistance() const { return nearPlaneDistance; }
        /// Sets the distance to the near plane.
        void SetNearPlaneDistance(float newValue) { nearPlaneDistance = newValue; }
        /// Returns the distance to the far plane.
        float GetFarPlaneDistance() const { return farPlaneDistance; }
        /// Sets the distance to the far plane distance.
        void SetFarPlaneDistance(float newValue) { farPlaneDistance = newValue; }
        /// Returns the vertical field of view (in degrees).
        float GetFovY() const;
        /// Sets the vertical field of view (in degrees).
        void SetFovY(float f);
        /// Returns the camera location (its position).
        VPPoint4D GetLocation() const;
        /// Sets the camera location.
        void SetLocation(const VPPoint4D& locationValue);
        /// Returns the camera target (where it is looking at).
        VPPoint4D GetTarget() const;
        /// Sets the camera target (where it is looking at).
        void SetTarget(const VPPoint4D& targetValue);
        /// Returns the camera up vector.
        VPPoint4D GetUp() const;
        /// Sets the camera up vector.
        void SetUp(const VPPoint4D& upValue);
        /// Sets the horizontal limit coordinates of the orthographic view volume.
        void SetVisibleVolumeHLimits(double left, double right)
                                    { vvLeft = left; vvRight = right; }
        /// Sets the vertical limit coordinates of the orthographic view volume.
        void SetVisibleVolumeVLimits(double top, double bottom)
                                    { vvTop = top; vvBottom = bottom; }

        /// \brief Sets the visible volume by given height.
        ///
        /// The visible volume is changed so that the origin is centered. The camera's
        /// aspect ratio is used to compute the visible volume's width. Usefull only for orthographic
        /// cameras.
        void SetVisibleVolumeHeight(double newValue);

        /// \brief Rotates the Camera around the axis defined by the target and up vector.
        void YawAroundTarget(double radians);
        /// \brief Rotates the Camera around itself (location and up vector).
        void Yaw(double radians);
        /// \brief Rotates the Camera around the axis defined by the target and left vector.
        void PitchAroundTarget(double radians);
        /// \brief Moves the camera forward (relative to the camera).
        void MoveForward(double distance);
        /// \brief Moves the camera sideways.
        void MoveSideways(double distance);
        /// \brief Moves the camera up and down.
        void MoveUp(double distance);

        /// Positions a camera using OpenGL commands.
        /// \return false if VPAT was not compiled with OpenGL support.
        bool DrawOGL() const;

    protected:
        /// Indicates whether a perspective ou orthographic projection should be used.
        ProjectionType projectionType;
        /// Vertical angle of frustum. Used if projectionType == PERSPECTIVE.
        float fovY;
        /// Viewing aspect ratio (width/height).
        float aspectRatio;
        /// Distance to near plane.
        float nearPlaneDistance;
        /// Distance to far plane.
        float farPlaneDistance;

        /// \brief Visible Volume left coordinate (in world coordinates).
        /// Used if projectionType is ORTHOGRAPHIC.
        double vvLeft;
        /// \brief Visible Volume right coordinate (in world coordinates).
        /// Used if projectionType is ORTHOGRAPHIC.
        double vvRight;
        /// \brief Visible Volume top coordinate (in world coordinates).
        /// Used if projectionType is ORTHOGRAPHIC.
        double vvTop;
        /// \brief Visible Volume bottom coordinate (in world coordinates).
        /// Used if projectionType is ORTHOGRAPHIC.
        double vvBottom;

        /// Position where the camera is at.
        VPPoint4D location;
        /// Position where the camera is looking at.
        VPPoint4D target;
        /// Up direction, relative to the camera.
        //FixMe: Should be kept orthogonal to (target-location).
        VPPoint4D up;
    private:
        std::string description;
};

#endif  // __VPCAMERA_H

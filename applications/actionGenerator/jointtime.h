//
#include <vart/joint.h>
#include <string.h>

    class JointTime {
        public:
            JointTime();
            JointTime(float dur, std::string interp, VART::Joint* jPtr);
            void SetDuration(float inTime);
            float GetDuration();
            void SetInterpolation(std::string interp);
            std::string GetInterpolation();
            VART::Joint* GetJoint();
        private:
            float duration;
            std::string interpolator;
            VART::Joint* jointPtr;
    };

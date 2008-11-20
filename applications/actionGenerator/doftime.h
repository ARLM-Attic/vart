//
#include <vart/dof.h>
#include <vart/joint.h>
#include <string>

    class DofTime {
        public:
            DofTime();
            DofTime(float inTime, float finTime, std::string dMov, VART::Joint* jMov);
            void SetInitialTime(float inTime);
            float GetInitialTime();
            void SetFinalTime(float finTime);
            float GetFinalTime();
            void SetDofType(std::string dPtr);
            std::string GetDofType();
            void SetJoint(VART::Joint* jPtr);
            VART::Joint* GetJoint();
        private:
            float initialTime;
            float finalTime;
            std::string dofType;
            VART::Joint* jointMover;
    };

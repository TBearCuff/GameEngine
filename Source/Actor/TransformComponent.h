#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "ActorComponent.h"
#include "../Graphics3D/Geometry.h"

//---------------------------------------------------------------------------------------------------------------------
// This component implementation is a very simple representation of the physical aspect of an actor.  It just defines
// the transform and doesn't register with the physics system at all.
//---------------------------------------------------------------------------------------------------------------------

class TransformComponent : public ActorComponent
{
    Mat4x4 m_transform;

public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }

    TransformComponent() : m_transform(Mat4x4::g_Identity){}//original code set m_transform to identity matrix; this is default Mat4x4 construction
    virtual bool VInit(QDomElement pData) override;
//    virtual XMLElement* VGenerateXml(XMLDocument &outDoc) override;

    //transform functions
    Mat4x4 GetTransform(void) const { return m_transform; }
    void SetTransform(const Mat4x4& newTransform) {m_transform = newTransform; }
    Vec3 GetPosition(void) const { return m_transform.GetPosition(); }
    void SetPosition(const Vec3& pos) {m_transform.SetPosition(pos); }
    Vec3 GetLookAt(void) const { return m_transform.GetDirection(); }
};

#endif // TRANSFORMCOMPONENT_H

#include "GameEngineStd.h"

#include "TransformComponent.h"
//#include "../Utilities/Math.h"
//#include "../Utilities/string.h"

const char* TransformComponent::g_Name = "TransformComponent";

bool TransformComponent::VInit(QDomElement *pData)
{
//    GCC_ASSERT(pData);

    // [mrmike] - this was changed post-press - because changes to the TransformComponents can come in partial definitions,
    //            such as from the editor, its better to grab the current values rather than clear them out.
    Vec3 yawPitchRoll = m_transform.GetYawPitchRoll();
    yawPitchRoll.setX(RADIANS_TO_DEGREES(yawPitchRoll.x()));
    yawPitchRoll.setY(RADIANS_TO_DEGREES(yawPitchRoll.y()));
    yawPitchRoll.setZ(RADIANS_TO_DEGREES(yawPitchRoll.z()));

    Vec3 position = m_transform.GetPosition();

    XMLElement *pPositionElement = pData->FirstChildElement("Position");
    if(pPositionElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pPositionElement->DoubleAttribute("x", x);
        pPositionElement->DoubleAttribute("y", y);
        pPositionElement->DoubleAttribute("z", z);
        position = Vec3(x, y, z);
    }

    XMLElement* pOrientationElement = pData->FirstChildElement("YawPitchRoll");
    {
        double yaw = 0;
        double pitch = 0;
        double roll = 0;
        pOrientationElement->DoubleAttribute("x", yaw);
        pOrientationElement->DoubleAttribute("y", pitch);
        pOrientationElement->DoubleAttribute("z", roll);
        yawPitchRoll = Vec3(yaw, pitch, roll);
    }
    Mat4x4 translation;
    translation.BuildTranslation(position);

    Mat4x4 rotation;
    rotation.BuildYawPitchRoll((float)DEGREES_TO_RADIANS(yawPitchRoll.x()), (float)DEGREES_TO_RADIANS(yawPitchRoll.y()),
                               (float)DEGREES_TO_RADIANS(yawPitchRoll.z()));

    /**
    // This is not supported yet.
    XMLElement* pLookAtElement = pData->FirstChildElement("LookAt");
    if (pLookAtElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pLookAtElement->DoubleAttribute("x", x);
        pLookAtElement->DoubleAttribute("y", y);
        pLookAtElement->DoubleAttribute("z", z);

        Vec3 lookAt((float)x, (float)y, (float)z);
        rotation.BuildRotationLookAt(translation.GetPosition(), lookAt, g_Up);
    }

    XMLElement* pScaleElement = pData->FirstChildElement("Scale");
    if (pScaleElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pScaleElement->DoubleAttribute("x", x);
        pScaleElement->DoubleAttribute("y", y);
        pScaleElement->DoubleAttribute("z", z);
        m_scale = Vec3((float)x, (float)y, (float)z);
    }
    **/

    m_transform = rotation * translation;

    return true;

}

XMLElement* TransformComponent::VGenerateXml(XMLDocument &outDoc)
{
    XMLElement* pBaseElement = outDoc.NewElement(VGetName());

    //initital transform -> position
    XMLElement* pPosition = outDoc.NewElement("Position");
    Vec3 pos(m_transform.GetPosition());
    pPosition->SetAttribute("x", pos.x());
    pPosition->SetAttribute("y", pos.y());
    pPosition->SetAttribute("z", pos.z());
    pBaseElement->LinkEndChild(pPosition);

    //initial transform -> YawPitchRoll
    XMLElement* pDirection = outDoc.NewElement("YawPitchRoll");
    Vec3 orient(m_transform.GetYawPitchRoll());
    orient.setX(RADIANS_TO_DEGREES(orient.x()));
    orient.setY(RADIANS_TO_DEGREES(orient.y()));
    orient.setZ(RADIANS_TO_DEGREES(orient.z()));
    pDirection->SetAttribute("x", orient.x());
    pDirection->SetAttribute("y", orient.y());
    pDirection->SetAttribute("z", orient.z());
    pBaseElement->LinkEndChild(pDirection);

    /**
    // This is not supported yet
    // initial transform -> scale
    XMLElement* pScale = outDoc.NewElement("Scale");
    Vec3 m_scale(m_transform.GetScale());
    pPosition->SetAttribute("x", m_scale.x());
    pPosition->SetAttribute("y", m_scale.y());
    pPosition->SetAttribute("z", m_scale.z());
    pBaseElement->LinkEndChild(pScale);
    **/

    return pBaseElement;
}

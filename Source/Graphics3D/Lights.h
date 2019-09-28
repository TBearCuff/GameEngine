#ifndef LIGHTS_H
#define LIGHTS_H

#include "../MainWindow/OpenGLRenderWindow.h"
#include "Geometry.h"
#include "Scene.h"
#include "SceneNodes.h"


#define MAXIMUM_LIGHTS_SUPPORTED (8)

//
// struct ConstantBuffer_Lighting
//
struct ConstantBuffer_Lighting
{
    Vec4 m_vLightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
    Vec4 m_vLightDir[MAXIMUM_LIGHTS_SUPPORTED];
    Vec4 m_vLightAmbient;
    unsigned int m_nNumLights;
    Vec3 m_vUnused;
};

// Note: Light color is stored in the Material structure, which is already present in all SceneNodes.

//
// struct LightProperties				- Chapter 16, page 551
//
struct LightProperties
{
    float	m_Attenuation[3];  /* Attenuation coefficients */
    float	m_Range;
    float	m_Falloff;
    float	m_Theta;
    float	m_Phi;
};

class LightNode : public SceneNode
{
protected:
    LightProperties m_LightProps;

public:
    LightNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const LightProperties &props, const Mat4x4 *t);
};


class GLLightNode : public LightNode
{
public:
    GLLightNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent,  const LightProperties &lightProps, const Mat4x4 *t)
        : LightNode(actorId, renderComponent, lightProps,  t) { }

    virtual void VOnRestore() { }
    virtual bool VOnUpdate(Scene *, DWORD const elapsedMs);
};

//
// class LightManager					- Chapter 16, 553
//
class LightManager
{
    friend class Scene;

protected:
    Lights					m_Lights;
    Vec4					m_vLightDir[MAXIMUM_LIGHTS_SUPPORTED];
    Color					m_vLightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
    Vec4					m_vLightAmbient;
public:
    void CalcLighting(Scene *pScene);
    void CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode *pNode);
    int GetLightCount(const SceneNode *node) {  Q_UNUSED(node); return m_Lights.size(); }
    const Vec4 *GetLightAmbient(const SceneNode *node) { Q_UNUSED(node); return &m_vLightAmbient; }
    const Vec4 *GetLightDirection(const SceneNode *node) { Q_UNUSED(node); return m_vLightDir; }
    const Color *GetLightDiffuse(const SceneNode *node) { Q_UNUSED(node); return m_vLightDiffuse; }
};

#endif // LIGHTS_H

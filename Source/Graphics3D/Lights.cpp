#include "GameEngineStd.h"

#include "GameEngineApp.h"
//#include "../Actors/RenderComponent.h"
#include "Lights.h"




LightNode::LightNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const LightProperties &props, const Mat4x4 *t)
    : SceneNode(actorId, renderComponent, RenderPass_NotRendered, t)
{
    m_LightProps = props;
}

bool GLLightNode::VOnUpdate(Scene *, const DWORD elapsedMs)
{
    Q_UNUSED(elapsedMs);
    // light color can change anytime! Check the BaseRenderComponent!
//    LightRenderComponent* lrc = static_cast<LightRenderComponent*>(m_RenderComponent);
//    m_Props.GetMaterial().SetDiffuse(lrc->GetColor());
    return true;
}

void LightManager::CalcLighting(Scene *pScene)
{
    // FUTURE WORK: There might be all kinds of things you'd want to do here for optimization,
    //especially turning off lights on actors that can't be seen, etc.
    pScene->GetRenderer()->VCalcLighting(&m_Lights, MAXIMUM_LIGHTS_SUPPORTED);

    int count = 0;

    Q_ASSERT(m_Lights.size() < MAXIMUM_LIGHTS_SUPPORTED);
    for(Lights::iterator i = m_Lights.begin(); i != m_Lights.end(); ++i, ++count)
    {
        QSharedPointer<LightNode> light = *i;

        if(count == 0)
        {
            //FUTURE WORK: Light 0 is the only one we use for ambient lighting.
            //The rest are ignored in the simple shaders.
            Color ambient = light->VGet()->GetMaterial().GetAmbient();
            m_vLightAmbient = Vec4(ambient.x(), ambient.y(), ambient.z(), 1.0f);
        }

        Vec3 lightDir = light->GetDirection();
        m_vLightDir[count] = Vec4(lightDir.x(), lightDir.y(), lightDir.z(), 1.0f);
        m_vLightDiffuse[count] = light->VGet()->GetMaterial().GetDiffuse();
    }
}

void LightManager::CalcLighting(ConstantBuffer_Lighting *pLighting, SceneNode *pNode)
{
    int count = GetLightCount(pNode);
    if(count)
    {
        pLighting->m_vLightAmbient = *GetLightAmbient(pNode);
        //GET THE LIGHTING INFORMATION INTO GLSL SHADER
        pLighting->m_nNumLights = count;
    }
}


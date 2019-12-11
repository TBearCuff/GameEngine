
#ifndef EVENTS_H
#define EVENTS_H


/** public includes files **/

//
#include "EventManager.h"
#include "../GameEngine/GameEngineStd.h"

#if 0
//---------------------------------------------------------------------------------------------------------------------
// EvtData_New_Actor - This event is sent out when an actor is *actually* created.
//---------------------------------------------------------------------------------------------------------------------
class EvtData_New_Actor : public BaseEventData<EvtData_New_Actor>
{
    ActorId m_actorId;
    GameViewId m_viewId;

public:
    static const EventType sk_EventType;

    EvtData_New_Actor(void)
    {
        m_actorId = INVALID_ACTOR_ID;
        m_viewId = gc_InvalidGameViewId;
    }

    explicit EvtData_New_Actor(ActorId actorId, GameViewId viewId = gc_InvalidGameViewId)
        : m_actorId(actorId),
          m_viewId(viewId)
    {
    }

    virtual void VDeserialize(QTextStream& in)
    {
        in >> m_actorId;
        in >> m_viewId;
    }

    virtual IEventDataPtr VCopy(void) const
    {
        return IEventDataPtr(new EvtData_New_Actor(m_actorId, m_viewId));
    }

    virtual void VSerialize(QTextStream& out) const
    {
        out << m_actorId << " ";
        out << m_viewId << " ";
    }


    virtual QString GetName(void) const
    {
        return "EvtData_New_Actor";
    }

    const ActorId GetActorId(void) const
    {
        return m_actorId;
    }

    GameViewId GetViewId(void) const
    {
        return m_viewId;
    }
};


//---------------------------------------------------------------------------------------------------------------------
// EvtData_Destroy_Actor - sent when actors are destroyed
//---------------------------------------------------------------------------------------------------------------------
class EvtData_Destroy_Actor : public BaseEventData<EvtData_Destroy_Actor>
{
    ActorId m_id;

public:
    static const EventType sk_EventType;

    explicit EvtData_Destroy_Actor(ActorId id = INVALID_ACTOR_ID)
        : m_id(id)
    {
        //
    }

    virtual IEventDataPtr VCopy(void) const
    {
        return IEventDataPtr ( new EvtData_Destroy_Actor ( m_id ) );
    }

    virtual void VSerialize(QTextStream &out) const
    {
        out << m_id;
    }

    virtual void VDeserialize(QTextStream& in)
    {
        in >> m_id;
    }

    virtual QString GetName(void) const
    {
        return "EvtData_Destroy_Actor";
    }

    ActorId GetId(void) const { return m_id; }
};


//---------------------------------------------------------------------------------------------------------------------
// EvtData_Move_Actor - sent when actors are moved
//---------------------------------------------------------------------------------------------------------------------
class EvtData_Move_Actor : public BaseEventData<EvtData_Move_Actor>
{
    ActorId m_id;
    Mat4x4 m_matrix;

public:
    static const EventType sk_EventType;

    EvtData_Move_Actor(void)
    {
        m_id = INVALID_ACTOR_ID;
    }

    EvtData_Move_Actor(ActorId id, const Mat4x4& matrix)
        : m_id(id), m_matrix(matrix)
    {
        //
    }

    virtual void VSerialize(QTextStream &out) const
    {
        out << m_id << " ";
        for (int i=0; i<4; ++i)
        {
            for (int j=0; j<4; ++j)
            {
                out << m_matrix(i,j);
            }
        }
    }

    virtual void VDeserialize(QTextStream& in)
    {
        in >> m_id;
        for (int i=0; i<4; ++i)
        {
            for (int j=0; j<4; ++j)
            {
                in >> m_matrix(i,j);
            }
        }
    }

    virtual IEventDataPtr VCopy() const
    {
        return IEventDataPtr(new EvtData_Move_Actor(m_id, m_matrix));
    }

    virtual QString GetName(void) const
    {
        return "EvtData_Move_Actor";
    }

    ActorId GetId(void) const
    {
        return m_id;
    }

    const Mat4x4& GetMatrix(void) const
    {
        return m_matrix;
    }
};
#endif
#if 0
//---------------------------------------------------------------------------------------------------------------------
// EvtData_New_Render_Component - This event is sent out when an actor is *actually* created.
//---------------------------------------------------------------------------------------------------------------------
class EvtData_New_Render_Component : public BaseEventData<EvtData_New_Render_Component>
{
    ActorId m_actorId;
    QSharedPointer<SceneNode> m_pSceneNode;

public:
    static const EventType sk_EventType;

    EvtData_New_Render_Component(void)
    {
        m_actorId = INVALID_ACTOR_ID;
    }

    explicit EvtData_New_Render_Component(ActorId actorId, QSharedPointer<SceneNode> pSceneNode)
        : m_actorId(actorId),
          m_pSceneNode(pSceneNode)
    {
    }

    virtual void VSerialize(QTextStream& out) const
    {
        Q_UNUSED(out);
//        GCC_ERROR(GetName() + QTextStream(" should not be serialzied!"));
    }

    virtual void VDeserialize(QTextStream& in)
    {
        Q_UNUSED(in);
//        GCC_ERROR(GetName() + QTextStream(" should not be serialzied!"));
    }

    virtual IEventDataPtr VCopy(void) const
    {
        return IEventDataPtr(new EvtData_New_Render_Component(m_actorId, m_pSceneNode));
    }

    virtual QString GetName(void) const
    {
        return "EvtData_New_Render_Component";
    }

    const ActorId GetActorId(void) const
    {
        return m_actorId;
    }

    QSharedPointer<SceneNode> GetSceneNode(void) const
    {
        return m_pSceneNode;
    }
};
#endif
#if 0
//---------------------------------------------------------------------------------------------------------------------
// EvtData_Modified_Render_Component - This event is sent out when a render component is changed
//   NOTE: This class is not described in the book!
//---------------------------------------------------------------------------------------------------------------------
class EvtData_Modified_Render_Component : public BaseEventData<EvtData_Modified_Render_Component>
{
    ActorId m_id;

public:
    static const EventType sk_EventType;

    EvtData_Modified_Render_Component(void)
    {
        m_id = INVALID_ACTOR_ID;
    }

    EvtData_Modified_Render_Component(ActorId id)
        : m_id(id)
    {
    }

    virtual void VSerialize(QTextStream &out) const
    {
        out << m_id;
    }

    virtual void VDeserialize(QTextStream& in)
    {
        in >> m_id;
    }

    virtual IEventDataPtr VCopy() const
    {
        return IEventDataPtr(new EvtData_Modified_Render_Component(m_id));
    }

    virtual QString GetName(void) const
    {
        return "EvtData_Modified_Render_Component";
    }

    ActorId GetActorId(void) const
    {
        return m_id;
    }
};

#endif


#endif // EVENTS_H


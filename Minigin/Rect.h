#pragma once
#include "PillarLibrary_02/include/InternalIncludes.h"
#include "SystemInterfaces.h"
#include "EventSystem.h"
#include "Transform.h"
#include <SDL.h>

namespace dae {

    //Bottom > Top because +y is down
    struct Rect {
        int Top, Left;      // Bottom-left corner
        int Width, Height;

        int Right() const { return Left + Width; }
        int Bottom() const { return Top + Height; } 
    };

    struct CDrawRect {
        bool Fill;
        Rect Rect;
        SDL_Color Color;
    };

    struct CRectTrigger {

        Rect Rect;
        glm::vec2 Offset{};
        void Center() {
            Offset.x = -Rect.Width / 2.f;
            Offset.y = -Rect.Height / 2.f;
        }

        int LayerNumber{0};
        int LayerNumberToIgnore{-1};

        Pillar::FEntity Self;
        std::vector<Pillar::FEntity> BeginOverlapEntities;
        std::vector<Pillar::FEntity> StayOverlapEntities;
        std::vector<Pillar::FEntity> EndOverlapEntities;
        Event<const Pillar::FEntity&, const Pillar::FEntity&, Pillar::URegistry*> OnCollisionStayEvent{ }; //self, other, registry
        Event<const Pillar::FEntity&, const Pillar::FEntity&, Pillar::URegistry*> OnCollisionBeginEvent{ }; //self, other, registry
        Event<const Pillar::FEntity&, const Pillar::FEntity&, Pillar::URegistry*> OnCollisionEndEvent{ }; //self, other, registry

        bool IsStayOverlappingWith(const Pillar::FEntity& entity) const {

            for (size_t i = 0; i < StayOverlapEntities.size(); i++)
            {
                if (StayOverlapEntities[i] == entity)
                {
                    return true;
                }
            }

            return false;
        }

    };

    inline bool IsColliding(const Rect& a, const Rect& b) {
        return (
            a.Right() > b.Left &&  
            a.Left < b.Right() &&  
            a.Top < b.Bottom() &&  
            a.Bottom() > b.Top);
    }

    class SDebugDrawRectTrigger final: public IRenderSystem{

    public:
        SDebugDrawRectTrigger(Pillar::URegistry& registry);
        void Render() override;

    private:

        Pillar::UComponentIterator<CRectTrigger> m_RectTriggers;
    };

    class SDrawRect final : public IRenderSystem {

    public:
        SDrawRect (Pillar::URegistry* registry);
        void Render() override;

    private:

        Pillar::URegistry* m_Registry{nullptr};
    };

    class STransformRectTriggers final : public IFixedUpdateSystem {

    public:
        STransformRectTriggers(Pillar::URegistry& registry);
        void FixedUpdate() override;

    private:

        Pillar::UComponentIterator<CTransform> m_Transforms;
        Pillar::UComponentIterator<CRectTrigger> m_RectTriggers;
    };

    class SCheckRectTriggerOverlap final : public IFixedUpdateSystem, public IStartSystem {

    public:
        SCheckRectTriggerOverlap(Pillar::URegistry& registry);
        void Start() override;
        void FixedUpdate() override;

    private:

        Pillar::UComponentIterator<CRectTrigger> m_RectTriggers;
        Pillar::URegistry& m_Registry;
    };
}


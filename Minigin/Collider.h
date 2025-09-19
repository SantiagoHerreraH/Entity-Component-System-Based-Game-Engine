#pragma once

#include "Rect.h"
#include "TActivatableFlatMap.h"

namespace dae {


	using ColliderTag = int;
	using ColliderKey = int;

	struct CColliderData {
		ColliderTag	ColliderTag;
		ColliderKey ColliderKey;
	};


	class ColliderRegistry {

	public:

		void AddIncludeTagFilter(const ColliderTag& forTag, const ColliderTag& includeTag);//iterate over all until include
		void AddExcludeTagFilter(const ColliderTag& forTag, const ColliderTag& excludeTag);//exclude from iteration

		CColliderData AddRectTrigger(const Rect& originalRect, const ColliderTag& colliderTag, const Pillar::FEntity& colliderEntity);
		Rect& GetTransformedRect(const CColliderData& ColliderKey);
		Rect& GetOriginalRect(const CColliderData& ColliderKey);

		void DeactivateRectTrigger(const CColliderData& data);
		void ActivateRectTrigger(const CColliderData& data);

		void DoTaskOverAll(const std::function<void(Rect& rect)> function);

		void UpdateCollisions();
		void RenderColliders();

	private:

		struct RectData {
			
			Rect OriginalRect{};
			Rect TransformedRect{};

			std::vector<Pillar::FEntity> BeginOverlapEntities;
			std::vector<Pillar::FEntity> StayOverlapEntities;
			std::vector<Pillar::FEntity> EndOverlapEntities;

			Event<const Pillar::FEntity&, const Pillar::FEntity&, Pillar::URegistry*> OnCollisionStayEvent{ }; //self, other, registry
			Event<const Pillar::FEntity&, const Pillar::FEntity&, Pillar::URegistry*> OnCollisionBeginEvent{ }; //self, other, registry
			Event<const Pillar::FEntity&, const Pillar::FEntity&, Pillar::URegistry*> OnCollisionEndEvent{ }; //self, other, registry
		};

		struct ColliderRegistryData {
			TActivatableFlatMap<ColliderKey, Rect> Colliders;
			std::vector<ColliderTag> TagsToInclude;
			std::vector<ColliderTag> TagsToExclude;
			std::vector<ColliderTag> TagsToIterateOver;

			void Refresh() {
				TagsToIterateOver = TagsToInclude;
			}

		};

		std::unordered_map<ColliderTag, ColliderRegistryData> m_ColliderTags_To_Colliders;
	};



}
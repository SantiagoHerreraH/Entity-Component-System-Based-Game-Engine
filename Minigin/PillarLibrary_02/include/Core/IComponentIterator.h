#pragma once

#include <typeindex>

#include "../../include/Core/FEntity.h"

namespace Pillar {

	class IComponentIterator {
	public:
		virtual ~IComponentIterator() {}
		virtual void Destroy() = 0;
	};

	class URegistry;

	class IComponentIteratorManager {

	public:
		virtual ~IComponentIteratorManager() {}
		virtual void RefreshAllComponentIterators() = 0;
		virtual void DeactivateArchetype(DArchetypeId id) = 0;
		virtual void AddOrRefreshArchetype(DArchetypeId id) = 0;
		virtual bool DestroyComponentIterator(IComponentIterator* arrayPtr) = 0;

	};
}

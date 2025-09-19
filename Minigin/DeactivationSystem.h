#pragma once

#include "PillarLibrary_02/include/InternalIncludes.h"
#include "SystemInterfaces.h"

namespace dae {

	class SActivateAndDeactivateMarkedEntities : public IUpdateSystem, public IFixedUpdateSystem
	{
	public:
		SActivateAndDeactivateMarkedEntities(Pillar::URegistry& registry) : m_Registry(&registry) {}
		void Update()override 
		{
			m_Registry->DeactivateMarkedEntities();
			m_Registry->ActivateMarkedEntities();
		}
		void FixedUpdate()override 
		{
			m_Registry->DeactivateMarkedEntities();
			m_Registry->ActivateMarkedEntities();
		}

	private:
		Pillar::URegistry* m_Registry;
	};
}




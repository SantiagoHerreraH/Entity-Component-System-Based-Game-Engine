#pragma once

#include "Rigidbody.h"
#include "SystemInterfaces.h"
#include <glm.hpp>


namespace dae {



	struct CMovement2D
	{
	public:

		void SetMaxSpeed(float maxSpeed);
		void AddMovementInput(const glm::vec2& input);
		void ApplyMovement(CRigidbody2D& rigidbody);

	private:

		glm::vec2 m_MovementInput;
		float m_MaxSpeed = 1; 
		float m_CurrentSpeed = 1;
	};

	class SApplyMovement2D : public IFixedUpdateSystem {

	public:
		SApplyMovement2D(Pillar::URegistry& registry);
		virtual void FixedUpdate() override;

	private:

		Pillar::UComponentIterator<CRigidbody2D> m_Rigidbodies2D;
		Pillar::UComponentIterator<CMovement2D> m_Movements2D;
	};

}



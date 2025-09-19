#include "Movement.h"

void dae::CMovement2D::SetMaxSpeed(float maxSpeed)
{
	m_MaxSpeed = maxSpeed < 0 ? 0 : maxSpeed;
}

void dae::CMovement2D::AddMovementInput(const glm::vec2& input)
{
	m_MovementInput += input;
}

void dae::CMovement2D::ApplyMovement(CRigidbody2D& rigidbody)
{
	if (glm::length(m_MovementInput) > FLT_EPSILON)
	{
		m_MovementInput = glm::normalize(m_MovementInput);
	}

	rigidbody.SetVelocity(m_MovementInput * m_MaxSpeed);
	m_MovementInput = glm::vec2{0,0};
}

dae::SApplyMovement2D::SApplyMovement2D(Pillar::URegistry& registry)
{
	Pillar::UComponentFilter filter{};
	filter.SetIncludingFilter(CRigidbody2D{}, CMovement2D{});

	registry.GetComponents(m_Rigidbodies2D, filter);
	registry.GetComponents(m_Movements2D, filter);
}

void dae::SApplyMovement2D::FixedUpdate()
{
	std::vector<CRigidbody2D> rigidbodies;
	std::vector<CMovement2D>  movements;
	size_t currentActiveComponents = 0;

	for (size_t archIdx = 0; archIdx < m_Movements2D.NumOfArchetypes(); archIdx++)
	{
		currentActiveComponents = m_Movements2D.NumOfActivatedComponents(archIdx);

		rigidbodies = m_Rigidbodies2D.Move(archIdx);
		movements = m_Movements2D.Move(archIdx);

		for (size_t compIdx = 0; compIdx < currentActiveComponents; compIdx++)
		{
			movements[compIdx].ApplyMovement(rigidbodies[compIdx]);
		}

		m_Rigidbodies2D.MoveBack(archIdx, std::move(rigidbodies));
		m_Movements2D.MoveBack(archIdx, std::move(movements));
	}
}


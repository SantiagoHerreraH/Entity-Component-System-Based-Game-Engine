#include "ParticleSystem.h"
#include "Renderer.h"
#include "Time.h"

dae::SParticleSystem::SParticleSystem(Pillar::URegistry* registry) : m_Registry(registry)
{
}

void dae::SParticleSystem::Render()
{
	m_Registry->DoTaskOverAllActivated<CParticle, void>([](CParticle& particle, void*)
		{
			Renderer::GetInstance().DrawDot(particle.Position, particle.Color);
		});
}

void dae::SParticleSystem::Update()
{
	m_Registry->DoTaskOverAllActivated<CParticle, void>([](CParticle& particle, void*)
		{
			particle.Position += glm::normalize(particle.MovementDirection) * particle.MovementSpeed * Time::GetInstance().GetElapsedSeconds();

			if (particle.HasBounds)
			{
				particle.Position.x =
					particle.Position.x < particle.Bounds.Left ? particle.Bounds.Right() :
					particle.Position.x > particle.Bounds.Right() ? particle.Bounds.Left : particle.Position.x;

				particle.Position.y =
					particle.Position.y < particle.Bounds.Top ? particle.Bounds.Bottom() :
					particle.Position.y > particle.Bounds.Bottom() ? particle.Bounds.Top : particle.Position.y;
			}
		});
}

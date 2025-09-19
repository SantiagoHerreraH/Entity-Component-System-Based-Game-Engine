#pragma once

#include <vector>
#include <glm.hpp>
#include <SDL.h>
#include "SystemInterfaces.h"
#include "Rect.h"

namespace dae {

	struct CParticle {
		glm::ivec2 Position;
		glm::vec2 MovementDirection;
		float MovementSpeed;
		SDL_Color Color;
		bool HasBounds{ true };
		Rect Bounds;
	};

	class SParticleSystem final : public IUpdateSystem, public IRenderSystem{

	public:
		SParticleSystem(Pillar::URegistry* registry);
		virtual void Render() override;
		virtual void Update() override;

	private:

		Pillar::URegistry* m_Registry{ nullptr };
	};
}




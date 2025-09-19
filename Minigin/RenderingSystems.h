#pragma once
#include <string>
#include <memory>
#include "SystemInterfaces.h"
#include "Transform.h"
#include "Texture.h"
#include "Text.h"

namespace dae
{
	class STextureRenderer final: public IRenderSystem {
	public:
		STextureRenderer(Pillar::URegistry& registry);
		virtual void Render() override;

	private:
		Pillar::UComponentIterator<CTextureHandle> m_TextureHandles;
		Pillar::UComponentIterator<CTransform> m_Transforms;
		mutable int previousActivatedNum{ 0 };
		int numberOfTicksToDisplayInfo{ 15 };
		mutable int currentNumberOfTicksSinceDisplayedInfo{ 3 };
	};

	class STextRenderer final: public IRenderSystem {

	public:
		STextRenderer(Pillar::URegistry& registry);
		virtual void Render() override;

	private:

		Pillar::UComponentIterator<CText> m_Text;
		Pillar::UComponentIterator<CTransform> m_Transforms;
	};
}

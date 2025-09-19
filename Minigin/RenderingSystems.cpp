#include <stdexcept>
#include <SDL_ttf.h>
#include "RenderingSystems.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture.h"
#include "ResourceManager.h"
#include <iostream>

dae::STextureRenderer::STextureRenderer(Pillar::URegistry& registry) :
	m_Transforms(CTransform{registry})
{
	Pillar::UComponentFilter filter{};
	filter.SetIncludingFilter(CTextureHandle{}, CTransform{ registry });

	registry.GetComponents(m_TextureHandles, filter);
	registry.GetComponents(m_Transforms, filter);
}

void dae::STextureRenderer::Render() 
{
	//optimize this later with move and move back
	
	/*bool isBullet = false;*/

	for (size_t archIdx = 0; archIdx < m_TextureHandles.NumOfArchetypes(); archIdx++)
	{
		/*isBullet = m_TextureHandles.ConstAt(archIdx, 0).Data() ==
			ResourceManager::GetInstance().GetTextureData("bullet.png").get();
		if (isBullet)
		{
			if (previousActivatedNum != m_TextureHandles.NumOfActivatedComponents(archIdx))
			{
				previousActivatedNum = (int)m_TextureHandles.NumOfActivatedComponents(archIdx);

				currentNumberOfTicksSinceDisplayedInfo = 0;
				std::cout << "\nNum of activated: " << previousActivatedNum;
			}

			if (currentNumberOfTicksSinceDisplayedInfo <= numberOfTicksToDisplayInfo)
			{
				++currentNumberOfTicksSinceDisplayedInfo;

				std::cout << "\nDisplayNum: " << currentNumberOfTicksSinceDisplayedInfo;
			}
		}*/
		
		for (size_t compIdx = 0; compIdx < m_TextureHandles.NumOfActivatedComponents(archIdx); compIdx++)
		{
			TransformData transformData{ m_Transforms.ConstAt(archIdx, compIdx).GetWorldTransform()};
			transformData += m_TextureHandles.ConstAt(archIdx, compIdx).GetTextureTransform();
			/*if (isBullet && currentNumberOfTicksSinceDisplayedInfo <= numberOfTicksToDisplayInfo)
			{
				std::cout << "\nbullet Draw Pos X: " << transformData.Position.x;
				std::cout << "\nbullet Draw Pos Y: " << transformData.Position.y << "\n";
			}*/
			Renderer::GetInstance().RenderTexture(
				*m_TextureHandles.ConstAt(archIdx, compIdx).Data(), 
				transformData.Position,
				transformData.Rotation,
				transformData.Scale,
				m_TextureHandles.ConstAt(archIdx, compIdx).GetTextureView());

		}
	}
}

dae::STextRenderer::STextRenderer(Pillar::URegistry& registry) :
	m_Transforms(CTransform{ registry })
{
	Pillar::UComponentFilter filter{};
	filter.SetIncludingFilter(CText{}, CTransform{ registry });

	registry.GetComponents(m_Text, filter);
	registry.GetComponents(m_Transforms, filter);
}

void dae::STextRenderer::Render() 
{
	//optimize this later with move and move back

	for (size_t archIdx = 0; archIdx < m_Text.NumOfArchetypes(); archIdx++)
	{
		for (size_t compIdx = 0; compIdx < m_Text.NumOfActivatedComponents(archIdx); compIdx++)
		{
			TransformData transformData{ m_Transforms.ConstAt(archIdx, compIdx).GetWorldTransform() };
			transformData += m_Text.ConstAt(archIdx, compIdx).GetTextureTransform();

			Renderer::GetInstance().RenderTexture(
				*m_Text.ConstAt(archIdx, compIdx).Data(),
				transformData.Position,
				transformData.Rotation,
				transformData.Scale);
		}
	}
}

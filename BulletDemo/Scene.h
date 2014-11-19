#pragma once
#include <SDL2/SDL.h>

#include "RenderMgr.h"
#include "ShaderMgr.h"
#include "PhysicsMgr.h"

namespace LittleLab {

	/**
	 * Basic interface for scenes.
	 */
	class Scene
	{
	public:

		/**
		 * Calculate updates since last time update() was called.
		 *
		 * @param elapsedTimeInMillis time elapsed since last call.
		 */
		virtual void update(Uint32 elapsedTimeInMillis) = 0;

		/**
		 * Render scene.
		 */
		virtual void render() = 0;

		/**
		 * Lets the game manager know whether this scene has ended.
		 *
		 * @return true if scene has ended.
		 */
		virtual bool isQuit() = 0;

		/**
		 * Sets the renderer for this scene.
		 * (To be used in fonts rendering)
		 */
		void setRenderer(SDL_Renderer * r) { renderer = r; }

	protected:

		Scene() :
			physicsMgr(Physics::PhysicsMgr::GetSingleton()),
			renderMgr(Graphics::RenderMgr::GetSingleton()),
			shaderMgr(Graphics::ShaderMgr::GetSingleton())
		{};

		// Managers
		LittleLab::Graphics::RenderMgr & renderMgr;
		LittleLab::Graphics::ShaderMgr & shaderMgr;
		LittleLab::Physics::PhysicsMgr & physicsMgr;

		SDL_Renderer * renderer;
	};

}
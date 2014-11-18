#pragma once
#include <SDL2/SDL.h>

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
		SDL_Renderer * renderer;
	};

}
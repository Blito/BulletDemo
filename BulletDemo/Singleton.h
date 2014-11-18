#pragma once

#include <cassert>

namespace LittleLab {

	namespace Utils {

		/**
		 * Singleton implementation from Game Programming Gems 1.
		 */
		template <typename T> class Singleton
		{

			static T* ms_singleton;

		public:
			Singleton() {
				assert(!ms_singleton);
				ms_singleton = static_cast <T*> (this);
			}

			~Singleton() {
				assert(ms_singleton); ms_singleton = 0;
			}

			static T& GetSingleton() {
				assert(ms_singleton); return *ms_singleton;
			}

			static T* GetSingletonPtr() { return ms_singleton; }
		};

		template <typename T> T* Singleton <T>::ms_singleton = 0;

	}

}
//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <memory>

//level editor
#include "gameobject.hpp"

using std::shared_ptr;

using Graphics::Shape::GameObject;

namespace Graphics::GUI
{
	class SceneHierarchy
	{
	public:
		static void RenderSceneHierarchy();

	private:
		static inline int framebufferWidth, framebufferHeight;
		static inline char newNameBuffer[64];
		static inline bool isRenaming;
		static inline shared_ptr<GameObject> selectedGameObject;

		static void DrawGameObject(shared_ptr<GameObject> obj);
		static void RightClickPopup();
		static void RenameGameObject();
	};
}
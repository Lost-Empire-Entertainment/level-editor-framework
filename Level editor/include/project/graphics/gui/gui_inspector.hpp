//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

namespace Graphics::GUI
{
	class Inspector
	{
	public:
		static void RenderInspector();
	private:
		static inline int framebufferWidth, framebufferHeight;
		static inline const int bufferSize = 32;
		static inline char inputTextBuffer_objName[bufferSize];
	};
}
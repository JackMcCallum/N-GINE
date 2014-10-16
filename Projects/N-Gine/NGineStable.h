#ifndef __NGINE_STABLE_H_
#define __NGINE_STABLE_H_

// ----------------------
// External files
#pragma warning(push, 3)

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm\ext.hpp>

// Third party dependencies
#define GLEW_STATIC
#include <glew\glew.h>
#include <gl\GL.h>

#ifndef NGINE_NO_SDL
#include <sdl/SDL.h>
#endif

// STL
#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <fstream>

// FreeImage
#include <FreeImage/FreeImage.h>

#pragma warning(pop)

// ---------------------
// Stable N-Gine headers
#include "NGineCommon.h"
#include "NGineLog.h"
#include "NGineUtilities.h"

#include "NGineMain.h"
#include "NGineRenderSystem.h"
#include "NGineResourceManager.h"

#endif // !__NGINE_STABLE_H

#include "NGineStable.h"
#include "NGineSDLRenderWindow.h"

#ifndef NGINE_NO_SDL

namespace NGine
{
	SDLRenderWindow::SDLRenderWindow(int32 width, int32 height, bool vsync, int32 msaa, const std::string& title) :
		mWasClosed(false)
	{
		// MSAA Hints
		if (msaa > 0)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		}

		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		setDimensions(width, height);

		if (!mWindow)
		{
			Log::error("ERROR: Failed to create SDL window");
			return;
		}

		mGLContext = SDL_GL_CreateContext(mWindow);
		if (!mGLContext)
		{
			Log::error("ERROR: Failed to create OpenGL context");
			return;
		}

		if (SDL_GL_MakeCurrent(mWindow, mGLContext))
			Log::error("ERROR: SDL_GL_MakeCurrent returned error: %s", SDL_GetError());
		
		// VSync maybe
		if (vsync == false)
			SDL_GL_SetSwapInterval(0);

		if (SDL_GL_MakeCurrent(mWindow, nullptr))
			Log::error("ERROR: SDL_GL_MakeCurrent returned error: %s", SDL_GetError());

	}

	SDLRenderWindow::~SDLRenderWindow()
	{
		if (SDL_GL_MakeCurrent(mWindow, mGLContext))
			Log::error("ERROR: SDLRenderSystem::onMakeCurrent() SDL_GL_MakeCurrent returned error: %s", SDL_GetError());

		if (mWindow)
		{
			SDL_DestroyWindow(mWindow);
			mWindow = nullptr;
		}

		if (mGLContext)
		{
			SDL_GL_DeleteContext(mGLContext);
			mGLContext = nullptr;
		}
	}

	void SDLRenderWindow::onMakeCurrent()
	{
		if (SDL_GL_MakeCurrent(mWindow, mGLContext))
			Log::error("ERROR: SDLRenderSystem::onMakeCurrent() SDL_GL_MakeCurrent returned error: %s", SDL_GetError());
	
	}

	void SDLRenderWindow::onDoneCurrent()
	{
		if (SDL_GL_MakeCurrent(mWindow, nullptr))
			Log::error("ERROR: SDLRenderSystem::onDoneCurrent() SDL_GL_MakeCurrent returned error: %s", SDL_GetError());
	}

	void SDLRenderWindow::onSwapBuffers()
	{
		SDL_GL_SwapWindow(mWindow);
	}

	void SDLRenderWindow::pollEvents()
	{
		SDL_Event evt;
		SDL_PumpEvents();
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
				// ===============================================
				// Keyboard events

			case SDL_KEYDOWN:
				mKeyboardEvt.type = KeyboardEvent::PRESS;
				mKeyboardEvt.keycode = evt.key.keysym.sym;
				mKeyboardEvt.scancode = evt.key.keysym.scancode;
				mKeyboardEvt.repeat = evt.key.repeat > 0;
				mKeyboardEvt.accepted = false;

				for (auto iter : mListenerList)
					iter->onKeyboardEvent(mKeyboardEvt);
				break;

			case SDL_KEYUP:
				mKeyboardEvt.type = KeyboardEvent::RELEASE;
				mKeyboardEvt.keycode = evt.key.keysym.sym;
				mKeyboardEvt.scancode = evt.key.keysym.scancode;
				mKeyboardEvt.repeat = evt.key.repeat > 0;
				mKeyboardEvt.accepted = false;

				for (auto iter : mListenerList)
					iter->onKeyboardEvent(mKeyboardEvt);
				break;

				// ===============================================
				// Mouse events

			case SDL_MOUSEBUTTONDOWN:
				mMouseEvt.type = MouseEvent::PRESS;
				mMouseEvt.button = evt.button.button;
				mMouseEvt.accepted = false;

				for (auto iter : mListenerList)
					iter->onMouseEvent(mMouseEvt);
				break;

			case SDL_MOUSEBUTTONUP:
				mMouseEvt.type = MouseEvent::RELEASE;
				mMouseEvt.button = evt.button.button;
				mMouseEvt.accepted = false;

				for (auto iter : mListenerList)
					iter->onMouseEvent(mMouseEvt);
				break;

			case SDL_MOUSEWHEEL:
				mMouseEvt.type = MouseEvent::WHEEL;
				mMouseEvt.wheel = evt.wheel.y;
				mMouseEvt.accepted = false;

				for (auto iter : mListenerList)
					iter->onMouseEvent(mMouseEvt);
				break;

			case SDL_MOUSEMOTION:
				mMouseEvt.type = MouseEvent::MOTION;
				mMouseEvt.pixel.x = evt.motion.x;
				mMouseEvt.pixel.y = evt.motion.y;
				mMouseEvt.relative.x = evt.motion.xrel;
				mMouseEvt.relative.y = evt.motion.yrel;
				mMouseEvt.clamped.x = mMouseEvt.pixel.x / (float)mWindowEvt.size.x;
				mMouseEvt.clamped.y = mMouseEvt.pixel.y / (float)mWindowEvt.size.y;
				mMouseEvt.accepted = false;

				for (auto iter : mListenerList)
					iter->onMouseEvent(mMouseEvt);
				break;

				// ===============================================
				// Window events

			case SDL_WINDOWEVENT:
				switch (evt.window.event)
				{
				case SDL_WINDOWEVENT_SHOWN:
					mWindowEvt.type = WindowEvent::SHOWN;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_HIDDEN:
					mWindowEvt.type = WindowEvent::HIDDEN;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_MOVED:
					mWindowEvt.type = WindowEvent::MOVED;
					mWindowEvt.pos.x = evt.window.data1;
					mWindowEvt.pos.y = evt.window.data2;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_RESIZED:
					mWindowEvt.type = WindowEvent::RESIZE;
					mWindowEvt.size.x = evt.window.data1;
					mWindowEvt.size.y = evt.window.data2;
					setDimensions(mWindowEvt.size.x, mWindowEvt.size.y);
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_MINIMIZED:
					mWindowEvt.type = WindowEvent::MINIMIZE;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_MAXIMIZED:
					mWindowEvt.type = WindowEvent::MAXIMIZE;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_RESTORED:
					mWindowEvt.type = WindowEvent::RESTORE;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_ENTER:
					mWindowEvt.type = WindowEvent::ENTER;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_LEAVE:
					mWindowEvt.type = WindowEvent::LEAVE;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_FOCUS_GAINED:
					mWindowEvt.type = WindowEvent::FOCUS_GAINED;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_FOCUS_LOST:
					mWindowEvt.type = WindowEvent::FOCUS_LOST;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					break;

				case SDL_WINDOWEVENT_CLOSE:
					mWindowEvt.type = WindowEvent::CLOSE;
					for (auto iter : mListenerList)
						iter->onWindowEvent(mWindowEvt);
					mWasClosed = true;
					break;
				}
				break;

			case SDL_QUIT:
				mWasClosed = true;
				break;
			}
		}

	}

	void SDLRenderWindow::addListener(Listener* listener)
	{
		mListenerList.push_back(listener);
	}

	void SDLRenderWindow::removeListener(Listener* listener)
	{
		auto iter = std::find(mListenerList.begin(), mListenerList.end(), listener);
		if (iter != mListenerList.end()) mListenerList.erase(iter);
	}

	bool SDLRenderWindow::wasClosed()
	{
		return mWasClosed;
	}
}

#endif

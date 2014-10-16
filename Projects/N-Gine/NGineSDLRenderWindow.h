#ifndef __NGINE_SDL_RENDER_SYSTEM_H_
#define __NGINE_SDL_RENDER_SYSTEM_H_

#ifndef NGINE_NO_SDL

#include "NGineRenderWindow.h"

struct SDL_Window;
typedef void* SDL_GLContext;

namespace NGine
{
	struct MouseEvent
	{
		enum Type
		{
			PRESS,
			RELEASE,
			WHEEL,
			MOTION,
		};

		Type type;
		glm::vec2 clamped;
		glm::ivec2 pixel;
		glm::ivec2 relative;
		int32 button;
		int32 wheel;
		mutable bool accepted;

	};

	struct KeyboardEvent
	{
		enum Type
		{
			PRESS,
			RELEASE
		};

		Type type;
		int32 keycode;
		int32 scancode;
		bool repeat;
		mutable bool accepted;
	};

	struct WindowEvent
	{
		enum Type
		{
			SHOWN,
			HIDDEN,
			MOVED,
			RESIZE,
			MINIMIZE,
			MAXIMIZE,
			RESTORE,
			ENTER,
			LEAVE,
			FOCUS_GAINED,
			FOCUS_LOST,
			CLOSE,
		};

		Type type;
		glm::ivec2 pos;
		glm::ivec2 size;
		mutable bool accepted;
	};

	class SDLRenderWindow : public RenderWindow
	{
	public:
		class Listener
		{
		public:
			virtual void onMouseEvent(const MouseEvent& evt) { (void)evt; };
			virtual void onKeyboardEvent(const KeyboardEvent& evt) { (void)evt; };
			virtual void onWindowEvent(const WindowEvent& evt) { (void)evt; };
		};

		SDLRenderWindow(int32 width, int32 height, bool vsync, int32 msaa, const std::string& title = "N-GINE Window");
		virtual ~SDLRenderWindow();

		virtual void onMakeCurrent();
		virtual void onDoneCurrent();
		virtual void onSwapBuffers();

		SDL_Window* getSDLWindow();
		SDL_GLContext getSDLContext();

		// Input stuff
		void pollEvents();
		bool wasClosed();
		
		void addListener(Listener* listener);
		void removeListener(Listener* listener);
		
	private:
		SDL_Window* mWindow;
		SDL_GLContext mGLContext;

		// Input stuff
		bool mWasClosed;
		
		MouseEvent mMouseEvt;
		KeyboardEvent mKeyboardEvt;
		WindowEvent mWindowEvt;
		
		std::vector<Listener*> mListenerList;
		
	};
}

#endif

#endif // !__NGINE_SDL_RENDER_SYSTEM_H_


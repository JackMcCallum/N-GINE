#ifndef __NGINE_XINPUT_POLLER_H_
#define __NGINE_XINPUT_POLLER_H_

#ifndef NGINE_NO_XINPUT

#include "NGineCommon.h"
#include <glm\glm.hpp>

namespace NGine
{
	class XInputPoller
	{
	public:
		struct State
		{
			inline State() { memset(this, 0, sizeof(*this)); };

			bool isConnected;

			struct Trigger
			{
				float left;
				float right;
			} trigger;

			struct Axis
			{
				float x;
				float y;
			} laxis, raxis;

			struct Button
			{
				bool A : 1;
				bool B : 1;
				bool X : 1;
				bool Y : 1;
				bool up : 1;
				bool down : 1;
				bool left : 1;
				bool right : 1;
				bool lthumb : 1;
				bool rthumb : 1;
				bool lshoulder : 1;
				bool rshoulder : 1;
				bool start : 1;
				bool back : 1;
			} button;
		};

	public:
		State pollState(uint32 idx = 0, float deadzone = 0.2f);
	};
}

#endif

#endif // !__NGINE_XINPUT_POLLER_H_

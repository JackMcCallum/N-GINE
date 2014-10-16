#include "NGineStable.h"
#include "NGineXInputPoller.h"

#ifndef NGINE_NO_XINPUT

#include <Xinput/XInput.h>

namespace NGine
{
	XInputPoller::State XInputPoller::pollState(uint32 idx, float deadzone)
	{
		assert(idx < 4);
		assert(idx >= 0);
		assert(deadzone <= 1);
		assert(deadzone >= 0);

		State state;
		memset(&state, 0, sizeof(State));

		XINPUT_STATE xstate;
		memset(&xstate, 0, sizeof(XINPUT_STATE));

		DWORD dwResult;
		dwResult = XInputGetState(0, &xstate);
		if (dwResult == 0)
		{
			state.isConnected = true;

			float lx, ly, mag, nx, ny;

			// >>> Left thum axis
			// Normalize raw data between -1 and 1
			lx = xstate.Gamepad.sThumbLX / 32767.0f;
			ly = xstate.Gamepad.sThumbLY / 32767.0f;

			// Calculate the magnitude and normalize
			mag = sqrt(lx * lx + ly * ly);
			nx = lx / mag;
			ny = ly / mag;

			// Calculate new magnitude with deadzone
			if (mag > deadzone)
			{
				if (mag > 1) mag = 1;
				mag -= deadzone;
				mag /= 1 - deadzone;
			}
			else
			{
				mag = 0;
			}

			// Apply deadzone and put it into our state
			state.laxis.x = nx * mag;
			state.laxis.y = ny * mag;


			// >>> Right thumb axis
			// Normalize raw data between -1 and 1
			lx = xstate.Gamepad.sThumbRX / 32767.0f;
			ly = xstate.Gamepad.sThumbRY / 32767.0f;

			// Calculate the magnitude and normalize
			mag = sqrt(lx * lx + ly * ly);
			nx = lx / mag;
			ny = ly / mag;

			// Calculate new magnitude with deadzone
			if (mag > deadzone)
			{
				if (mag > 1) mag = 1;
				mag -= deadzone;
				mag /= 1 - deadzone;
			}
			else
			{
				mag = 0;
			}

			// Apply deadzone and put it into our state
			state.raxis.x = nx * mag;
			state.raxis.y = ny * mag;
			
			// >>> Triggers
			state.trigger.left = xstate.Gamepad.bLeftTrigger / 255.0f;
			state.trigger.right = xstate.Gamepad.bRightTrigger / 255.0f;

			// >>> Buttons
#define _CHECK_BIT(bit, val) if ((xstate.Gamepad.wButtons & bit) != 0) state.button.val = true;
			_CHECK_BIT(XINPUT_GAMEPAD_A, A);
			_CHECK_BIT(XINPUT_GAMEPAD_B, B);
			_CHECK_BIT(XINPUT_GAMEPAD_X, X);
			_CHECK_BIT(XINPUT_GAMEPAD_Y, Y);
			_CHECK_BIT(XINPUT_GAMEPAD_DPAD_UP, up);
			_CHECK_BIT(XINPUT_GAMEPAD_DPAD_DOWN, down);
			_CHECK_BIT(XINPUT_GAMEPAD_DPAD_LEFT, left);
			_CHECK_BIT(XINPUT_GAMEPAD_DPAD_RIGHT, right);
			_CHECK_BIT(XINPUT_GAMEPAD_LEFT_THUMB, lthumb);
			_CHECK_BIT(XINPUT_GAMEPAD_RIGHT_THUMB, rthumb);
			_CHECK_BIT(XINPUT_GAMEPAD_LEFT_SHOULDER, lshoulder);
			_CHECK_BIT(XINPUT_GAMEPAD_RIGHT_SHOULDER, rshoulder);
			_CHECK_BIT(XINPUT_GAMEPAD_START, start);
			_CHECK_BIT(XINPUT_GAMEPAD_BACK, back);
#undef _CHECK_BIT

		}
		else
		{
			state.isConnected = false;
		}
		return state;
	}
}

#endif
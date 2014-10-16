#ifndef __SHADER_TOOL_BASE_APP_H_
#define __SHADER_TOOL_BASE_APP_H_

#include <NGineCommon.h>
#include <NGineRenderWindow.h>

class ProgramBase : public NGine::RenderWindowListener
{
public:
	ProgramBase();
	virtual ~ProgramBase();

	bool run();
	void close();
	
	virtual void eventCreate() = 0;
	virtual void eventUpdate() = 0;
	virtual void eventDraw() = 0;
	virtual void eventDestroy() = 0;

	double deltaTime() const;
	double totalTime() const;

	NGine::RenderWindow* getWindow() const;

private:
	NGine::RenderWindow* mWindow;
	bool mShouldClose;
	double mDeltaTime;
	double mTotalTime;
};

#endif // !__SHADER_TOOL_BASE_APP_H_

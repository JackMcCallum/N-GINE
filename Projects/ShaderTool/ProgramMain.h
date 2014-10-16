#ifndef __SHADER_TOOL_PROGRAM_MAIN_H_
#define __SHADER_TOOL_PROGRAM_MAIN_H_

#include "ProgramBase.h"
#include "Painter.h"

class Node;

class ProgramMain : public ProgramBase
{
public:
	ProgramMain();
	~ProgramMain();

	virtual void eventCreate() override;
	virtual void eventUpdate() override;
	virtual void eventDraw() override;
	virtual void eventDestroy() override;

	virtual void eventMouseMoved(NGine::MouseEvent* evt) override;
	virtual void eventMousePressed(NGine::MouseEvent* evt) override;
	virtual void eventMouseReleased(NGine::MouseEvent* evt) override;
	virtual void eventMouseWheel(NGine::MouseEvent* evt) override;

	virtual void eventMouseEnter() override;
	virtual void eventMouseLeave() override;

	virtual void eventKeyboardPressed(NGine::KeyEvent* evt) override;
	virtual void eventKeyboardReleased(NGine::KeyEvent* evt) override;

	virtual void eventWindowResized(int32 width, int32 height) override;
	virtual void eventWindowClosed() override;

	enum Properties
	{
		CAMERA_DRAGGING = (1 << 0),
	};



private:
	Painter mPainter;
	uint32 mProperties;
	glm::vec2 mCameraPos;

	std::vector<Node*> mNodeList;
	Node* mDragNode;
};

#endif

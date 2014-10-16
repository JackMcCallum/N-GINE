#include "Precompiled.h"
#include "ProgramMain.h"
#include "Node.h"


ProgramMain::ProgramMain() :
mProperties(0),
mDragNode(nullptr)
{
}

ProgramMain::~ProgramMain()
{
}

void ProgramMain::eventCreate()
{
	mPainter.createFont("SegoeUI12", "ShaderTool/segoe_ui_regular_12.xml");

	int32 w, h;
	getWindow()->getSize(w, h);
	mPainter.setResolution((uint32)w, (uint32)h);
	
	for (int i = 0; i < 20; i++)
	{
		Node* node = new Node("Split Color", glm::vec4(0.5f, 0.5f, 0.5f, 0.8f));
		node->addInputConnection("Color", RGBA);
		node->addOutputConnection("Red", R);
		node->addOutputConnection("Green", G);
		node->addOutputConnection("Blue", B);
		node->addOutputConnection("Alpha", A);
		node->setPosition(glm::vec2(rand() % 1000, rand() % 1000));
		mNodeList.push_back(node);
	}

	
}

void ProgramMain::eventUpdate()
{

}

void ProgramMain::eventDraw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	const uint32 spacing = 12;

	mPainter.width(1.0f);
	mPainter.begin(GL_LINES);
	mPainter.color(0.3f, 0.3f, 0.3f, 1.0f);
	for (int i = 0; i < mPainter.getResHeight(); i += spacing)
	{
		mPainter.vertex(0, i + uint32(mCameraPos.y) % spacing);
		mPainter.vertex((float)mPainter.getResWidth(), i + uint32(mCameraPos.y) % spacing);
	}

	for (int i = 0; i < mPainter.getResWidth(); i += spacing)
	{
		mPainter.vertex(i + uint32(mCameraPos.x) % spacing, 0);
		mPainter.vertex(i + uint32(mCameraPos.x) % spacing, (float)mPainter.getResHeight());
	}

	mPainter.end();

	// Draw all the connections
	mPainter.alpha(true);
	mPainter.width(2.0f);
	mPainter.color(1, 1, 1, 1);
	mPainter.smoothing(true);

	for (auto iter : mNodeList)
	{
		const ConnectionList& list = iter->getOutputs();

	}

	srand(0);
	for (int i = 0; i < 4; i++)
	{
		mPainter.cubic(
			glm::vec2(0, 0) + mCameraPos + glm::vec2(rand() % 1000, rand() % 1000),
			glm::vec2(0, 256) + mCameraPos + glm::vec2(rand() % 1000, rand() % 1000),
			glm::vec2(256, 0),
			glm::vec2(-256, 0), 32);
	}

	mPainter.smoothing(false);
	mPainter.width(2.0f);
	mPainter.color(1, 1, 1, 1);

	for (auto iter = mNodeList.rbegin(); iter != mNodeList.rend(); iter++)
		(*iter)->paint(mPainter, mCameraPos);
	mPainter.alpha(false);

}

void ProgramMain::eventDestroy()
{

}

void ProgramMain::eventMouseMoved(NGine::MouseEvent* evt)
{
	if (mDragNode)
		mDragNode->setPosition(mDragNode->getPosition() + glm::vec2(evt->getRelativeX(), evt->getRelativeY()));
	else if (mProperties & CAMERA_DRAGGING)
		mCameraPos += glm::vec2(evt->getRelativeX(), evt->getRelativeY());

}

void ProgramMain::eventMousePressed(NGine::MouseEvent* evt)
{
	if (evt->getButton() == NGINE_MOUSE_BUTTON_1)
	{
		if (mProperties & CAMERA_DRAGGING)
			return;

		glm::vec2 mousePos(evt->getPixelX(), evt->getPixelY());

		for (auto i = mNodeList.begin(); i != mNodeList.end(); i++)
		{
			Node* node = *i;
			if (node->isHovering(mousePos - mCameraPos))
			{
				mNodeList.erase(i);
				mNodeList.insert(mNodeList.begin(), node);
				mDragNode = node;
				break;
			}
		}
	}
	else if (evt->getButton() == NGINE_MOUSE_BUTTON_3)
		mProperties |= CAMERA_DRAGGING;


}

void ProgramMain::eventMouseReleased(NGine::MouseEvent* evt)
{
	mProperties &= ~CAMERA_DRAGGING;
	mDragNode = nullptr;
}

void ProgramMain::eventMouseWheel(NGine::MouseEvent* evt)
{

}

void ProgramMain::eventMouseEnter()
{

}

void ProgramMain::eventMouseLeave()
{
	mProperties &= ~CAMERA_DRAGGING;
}

void ProgramMain::eventKeyboardPressed(NGine::KeyEvent* evt)
{

}

void ProgramMain::eventKeyboardReleased(NGine::KeyEvent* evt)
{

}

void ProgramMain::eventWindowResized(int32 width, int32 height)
{
	mPainter.setResolution(width, height);
	glViewport(0, 0, width, height);
	std::cout << "Resizing window: " << width << ", " << height << std::endl;

	eventDraw();
	getWindow()->onSwapBuffers();
}

void ProgramMain::eventWindowClosed()
{

}

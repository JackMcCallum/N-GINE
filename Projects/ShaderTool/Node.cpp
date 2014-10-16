#include "Precompiled.h"
#include "Node.h"


Node::Node(const std::string& name, const glm::vec4& color) :
mColor(color)
{
	mSize.x = 128;
	mSize.y = 96;
}

Node::~Node()
{

}

const glm::vec2& Node::getSize() const
{
	return mSize;
}

const glm::vec2& Node::getPosition() const
{
	return mPosition;
}

void Node::addInputConnection(const std::string& name, ConnectionType type)
{
	Connection c;
	c.connection = nullptr;
	c.isInput = true;
	c.name = name;
	c.type = type;
	mInputs.push_back(c);
}

void Node::addOutputConnection(const std::string& name, ConnectionType type)
{
	Connection c;
	c.connection = nullptr;
	c.isInput = false;
	c.name = name;
	c.type = type;
	mOutputs.push_back(c);
}

void Node::paint(Painter& painter, glm::vec2 camPos)
{
	glm::vec2 viewPos = camPos + mPosition;
	painter.rect(viewPos, mSize, mColor, false);
	painter.rect(viewPos, mSize, glm::vec4(0,0,0,1), true);

	
	uint32 i;
	i = 0;
	for (auto iter : mInputs)
	{
		_setColor(painter, iter.type);
		painter.color(0, 0, 0, 1);
		painter.text(viewPos + glm::vec2(14, i * 24 + 36), iter.name, "SegoeUI12", glm::vec4(0,0,0,1));


		i++;
	}



	i = 0;
	for (auto iter : mOutputs)
	{
		_setColor(painter, iter.type);
		
		painter.color(0, 0, 0, 1);
		painter.text(viewPos + glm::vec2(mSize.x - 64, i * 24 + 22), iter.name, "SegoeUI12", glm::vec4(0, 0, 0, 1));
		i++;
	}

}

bool Node::isHovering(const glm::vec2 mousePos) const
{
	if (mousePos.x > mPosition.x &&
		mousePos.y > mPosition.y &&
		mousePos.x < mPosition.x + mSize.x &&
		mousePos.y < mPosition.y + mSize.y)
		return true;
	return false;
}

void Node::_setColor(Painter& painter, ConnectionType type)
{
	switch (type)
	{
	case FLOAT:
		painter.color(0.7f, 0.7f, 0.7f, 1.0f);
		break;

	case VEC2:
	case VEC3:
	case VEC4:
		painter.color(0.5f, 0.5f, 1.0f, 1.0f);
		break;

	case R:
		painter.color(1.0f, 0.1f, 0.1f, 1.0f);
		break;

	case G:
		painter.color(0.1f, 1.0f, 0.1f, 1.0f);
		break;

	case B:
		painter.color(0.1f, 0.1f, 1.0f, 1.0f);
		break;

	case A:
		painter.color(0.9f, 0.9f, 0.9f, 1.0f);
		break;

	case RGBA:
		painter.color(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	}
}

void Node::setSize(const glm::vec2& size)
{
	mSize = size;
}

void Node::setPosition(const glm::vec2& pos)
{
	mPosition = pos;
}

const ConnectionList& Node::getInputs() const
{
	return mInputs;
}

const ConnectionList& Node::getOutputs() const
{
	return mOutputs;
}

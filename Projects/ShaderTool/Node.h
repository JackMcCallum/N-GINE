#ifndef __SHADER_NODE_H_
#define __SHADER_NODE_H_

#include "NGineCommon.h"
#include "Painter.h"

enum ConnectionType
{
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	RGBA,
	R,
	G,
	B,
	A,
};

struct Connection
{
	// Type of the connection
	ConnectionType type;

	// Display name of the connection
	std::string name;

	// Pointer to the connected node
	Connection* connection;

	glm::vec2 position;

	bool isInput;
};

typedef std::vector<Connection> ConnectionList;

class Node
{
public:
	Node(const std::string& name, const glm::vec4& color = glm::vec4(1));
	~Node();

	const glm::vec2& getSize() const;
	const glm::vec2& getPosition() const;
	void setSize(const glm::vec2& size);
	void setPosition(const glm::vec2& pos);

	void addInputConnection(const std::string& name, ConnectionType type);
	void addOutputConnection(const std::string& name, ConnectionType type);

	const ConnectionList& getInputs() const;
	const ConnectionList& getOutputs() const;

	virtual void paint(Painter& painter, glm::vec2 camPos);

	bool isHovering(const glm::vec2 mousePos) const;

private:
	void _setColor(Painter& painter, ConnectionType type);

private:
	
	ConnectionList mInputs;
	ConnectionList mOutputs;

	glm::vec4 mColor;
	glm::vec2 mSize;
	glm::vec2 mPosition;

};

#endif // !__SHADER_NODE_H_

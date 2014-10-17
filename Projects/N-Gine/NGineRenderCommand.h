#ifndef __NGINE_RENDER_COMMAND_H_
#define __NGINE_RENDER_COMMAND_H_

#include "NGineCommon.h"
#include "NGineRenderSystem.h"
#include "NGineCommandQueue.h"
#include "NGineMain.h"

#include <glm/glm.hpp>

namespace NGine
{
	// Enqueue a render command with 0 parameters
#define ENQUEUE_RENDER_COMMAND_0PARAMS(typeName, code) \
	{ \
	class DRCMacro_##typeName : public ::NGine::TCommand<DRCMacro_##typeName> \
	{ \
	public: \
	virtual void execute() { code; } \
	}; \
	DRCMacro_##typeName cmd##typeName; \
	NGine::Main::getRenderSystem().enqueueCommand(cmd##typeName); \
	}

	// Enqueue a render command with 1 parameters
#define ENQUEUE_RENDER_COMMAND_1PARAMS(typeName, p1type, p1name, p1value, code) \
	{ \
	class DRCMacro_##typeName : public ::NGine::TCommand<DRCMacro_##typeName> \
	{ \
	public: \
	virtual void execute() { code; } \
	p1type p1name; \
	}; \
	DRCMacro_##typeName cmd##typeName; \
	cmd##typeName.p1name = p1value; \
	NGine::Main::getRenderSystem().enqueueCommand(cmd##typeName); \
	}

	// Enqueue a render command with 2 parameters
#define ENQUEUE_RENDER_COMMAND_2PARAMS(typeName, p1type, p1name, p1value, p2type, p2name, p2value, code) \
	{ \
	class DRCMacro_##typeName : public ::NGine::TCommand<DRCMacro_##typeName> \
	{ \
	public: \
	virtual void execute() { code; } \
	p1type p1name; \
	p2type p2name; \
	}; \
	DRCMacro_##typeName cmd##typeName; \
	cmd##typeName.p1name = p1value; \
	cmd##typeName.p2name = p2value; \
	NGine::Main::getRenderSystem().enqueueCommand(cmd##typeName); \
	}

	// Enqueue a render command with 3 parameters
#define ENQUEUE_RENDER_COMMAND_3PARAMS(typeName, p1type, p1name, p1value, p2type, p2name, p2value, p3type, p3name, p3value, code) \
	{ \
	class DRCMacro_##typeName : public ::NGine::TCommand<DRCMacro_##typeName> \
	{ \
	public: \
	virtual void execute() { code; } \
	p1type p1name; \
	p2type p2name; \
	p3type p3name; \
	}; \
	DRCMacro_##typeName cmd##typeName; \
	cmd##typeName.p1name = p1value; \
	cmd##typeName.p2name = p2value; \
	cmd##typeName.p3name = p3value; \
	NGine::Main::getRenderSystem().enqueueCommand(cmd##typeName); \
	}

	// Enqueue a render command with 4 parameters
#define ENQUEUE_RENDER_COMMAND_4PARAMS(typeName, p1type, p1name, p1value, p2type, p2name, p2value, p3type, p3name, p3value, p4type, p4name, p4value, code) \
	{ \
	class DRCMacro_##typeName : public ::NGine::TCommand<DRCMacro_##typeName> \
	{ \
	public: \
	virtual void execute() { code; } \
	p1type p1name; \
	p2type p2name; \
	p3type p3name; \
	p4type p4name; \
	}; \
	DRCMacro_##typeName cmd##typeName; \
	cmd##typeName.p1name = p1value; \
	cmd##typeName.p2name = p2value; \
	cmd##typeName.p3name = p3value; \
	cmd##typeName.p4name = p4value; \
	NGine::Main::getRenderSystem().enqueueCommand(cmd##typeName); \
	}

	// Enqueue a render command with 5 parameters
#define ENQUEUE_RENDER_COMMAND_5PARAMS(typeName, p1type, p1name, p1value, p2type, p2name, p2value, p3type, p3name, p3value, p4type, p4name, p4value, p5type, p5name, p5value, code) \
	{ \
	class DRCMacro_##typeName : public ::NGine::TCommand<DRCMacro_##typeName> \
	{ \
	public: \
	virtual void execute() { code; } \
	p1type p1name; \
	p2type p2name; \
	p3type p3name; \
	p4type p4name; \
	p5type p5name; \
	}; \
	DRCMacro_##typeName cmd##typeName; \
	cmd##typeName.p1name = p1value; \
	cmd##typeName.p2name = p2value; \
	cmd##typeName.p3name = p3value; \
	cmd##typeName.p4name = p4value; \
	cmd##typeName.p5name = p5value; \
	NGine::Main::getRenderSystem().enqueueCommand(cmd##typeName); \
	}

	// Enqueue a render command with 6 parameters
#define ENQUEUE_RENDER_COMMAND_6PARAMS(typeName, p1type, p1name, p1value, p2type, p2name, p2value, p3type, p3name, p3value, p4type, p4name, p4value, p5type, p5name, p5value, p6type, p6name, p6value, code) \
	{ \
class DRCMacro_##typeName : public ::NGine::TCommand<DRCMacro_##typeName> \
	{ \
	public: \
	virtual void execute() { code; } \
	p1type p1name; \
	p2type p2name; \
	p3type p3name; \
	p4type p4name; \
	p5type p5name; \
	p6type p6name; \
	}; \
	DRCMacro_##typeName cmd##typeName; \
	cmd##typeName.p1name = p1value; \
	cmd##typeName.p2name = p2value; \
	cmd##typeName.p3name = p3value; \
	cmd##typeName.p4name = p4value; \
	cmd##typeName.p5name = p5value; \
	cmd##typeName.p6name = p6value; \
	NGine::Main::getRenderSystem().enqueueCommand(cmd##typeName); \
	}

}

#endif // !__NGINE_RENDER_COMMAND_H_

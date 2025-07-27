#include <JointCreateContext_include/JointCreateContextCommand.h>
#include <JointCreateContext_include/JointCreateContext.h>

const MString JointCreateContextCommand::CommandName = "JointCreateContextCommand";

void* JointCreateContextCommand::Creator()
{
	return new JointCreateContextCommand;
}

MPxContext* JointCreateContextCommand::makeObj()
{
	return new JointCreateContext;
}

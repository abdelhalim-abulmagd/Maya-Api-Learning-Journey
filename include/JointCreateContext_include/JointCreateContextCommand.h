#pragma once

#include <maya/MPxContextCommand.h>

class JointCreateContextCommand : public MPxContextCommand
{
public:
	static const MString CommandName;
	static void* Creator();

	virtual MPxContext* makeObj() override;

};
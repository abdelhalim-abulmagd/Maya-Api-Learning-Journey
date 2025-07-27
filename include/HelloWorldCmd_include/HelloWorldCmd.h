#pragma once

#include <maya/MPxCommand.h>

class HelloWorldCmd : public MPxCommand
{

public:

	static const MString TypeName;

	static void* Creator();

	MStatus doIt(const MArgList& ArgList) override;

};
#pragma once

#include <maya/MPxContextCommand.h>

class CostumeSelectContextCommand : public MPxContextCommand
{
public:
	virtual ~CostumeSelectContextCommand() override;

	static const MString CommandName;
	static void* Creator();
	
	virtual MPxContext* makeObj();



};
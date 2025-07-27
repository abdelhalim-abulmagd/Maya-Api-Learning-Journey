#include <CostumeSelectContext_include/CostumeSelectContextCommand.h>
#include <CostumeSelectContext_include/CostumeSelectContext.h>

const MString CostumeSelectContextCommand::CommandName = "CostumeSelectContextCommand";


CostumeSelectContextCommand::~CostumeSelectContextCommand()
{
}

void* CostumeSelectContextCommand::Creator()
{
	return new CostumeSelectContextCommand;
}

MPxContext* CostumeSelectContextCommand::makeObj()
{
	return new CostumeSelectContext;
}
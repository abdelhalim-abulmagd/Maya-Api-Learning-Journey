#pragma once

#include <maya/MPxCommand.h>
#include <maya/MDagModifier.h>

class DistanceBetweenLocatorCommand : public MPxCommand
{
public:

	virtual ~DistanceBetweenLocatorCommand() override;

	static const MString CommandName;
	static void* Creator();
	static MSyntax CreateSyntax();

	MStatus doIt(const MArgList& Args) override;

	virtual MStatus undoIt() override;
	virtual MStatus redoIt() override;
	virtual bool isUndoable() const override;

private:
	MDagModifier DagModifier;

	MObject TransformObj;
	MObject DistanceBetweenObj;

	MString GetShapeName(const MString& TransformName);
};
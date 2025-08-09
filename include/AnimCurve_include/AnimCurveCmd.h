#pragma once

#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>
#include <maya/MAnimCurveChange.h>

class AnimCurveCmd : public MPxCommand
{
public:
	AnimCurveCmd();
	static const MString CommandName;
	static void* Creator();
	
	static MSyntax CreateSyntax();

	virtual bool isUndoable() const;
	virtual MStatus undoIt();
    virtual MStatus redoIt();


	MStatus doIt(const MArgList& Args);

private:



	MDGModifier DGModifier;
    bool IsDGChanged = false;

    MAnimCurveChange AnimCurveChange;
    bool IsAnimCurveChange = false;
};
#include <AnimCurve_include/AnimCurveCmd.h>
#include <AnimCurve_include/AnimUtil.h>

#include <maya/MFnAnimCurve.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MAnimControl.h>

static const char* CreateAnimCurveFlag[]{ "cc", "CreateAnimCurve" };
static const char* DeleteAnimCurveFlag[]{ "dc", "DeleteAnimCurve" };
static const char* SetKeyFlag[]{ "set", "SetKey" };
static const char* RemoveKeyFlag[]{ "rm", "RemoveKey" };

const MString AnimCurveCmd::CommandName = "AnimCurveCmd";

AnimCurveCmd::AnimCurveCmd()
{
	IsDGChanged = false;
}
void* AnimCurveCmd::Creator()
{
	return new AnimCurveCmd;
}
MSyntax AnimCurveCmd::CreateSyntax()
{
	MSyntax Syntax;

	Syntax.setObjectType(MSyntax::kSelectionList, 1, 1);
	Syntax.useSelectionAsDefault(true);

	Syntax.addFlag(CreateAnimCurveFlag[0], CreateAnimCurveFlag[1]);
	Syntax.addFlag(DeleteAnimCurveFlag[0], DeleteAnimCurveFlag[1]);
	Syntax.addFlag(SetKeyFlag[0], SetKeyFlag[1]);
	Syntax.addFlag(RemoveKeyFlag[0], RemoveKeyFlag[1]);

	return Syntax;
}

bool AnimCurveCmd::isUndoable() const
{
	return true;
}

MStatus AnimCurveCmd::undoIt()
{
	if (IsDGChanged)
	{
		DGModifier.undoIt();
	}
	if (IsAnimCurveChange)
	{
		AnimCurveChange.undoIt();
	}
	return MS::kSuccess;
}


MStatus AnimCurveCmd::redoIt()
{
	if (IsDGChanged)
	{
		DGModifier.doIt();
	}
	if (IsAnimCurveChange)
	{
		AnimCurveChange.redoIt();
	}
	return MS::kSuccess;
}

MStatus AnimCurveCmd::doIt(const MArgList& Args)
{
	MStatus Status;

	MArgDatabase ArgData(syntax(), Args, &Status);
	CHECK_MSTATUS_AND_RETURN_IT(Status);

	MSelectionList SelectedObjects; 
	Status = ArgData.getObjects(SelectedObjects);
	CHECK_MSTATUS_AND_RETURN_IT(Status);

	MObject SelectedObject;
	SelectedObjects.getDependNode(0, SelectedObject);

	const char* AttributNames[]{ "tx", "ty", "tz" };
	

	if (ArgData.isFlagSet(CreateAnimCurveFlag[0]))
	{
		Status = AnimUtil::CreateAnimCurveNodes(SelectedObject, MStringArray(AttributNames, 3), DGModifier);
		CHECK_MSTATUS_AND_RETURN_IT(Status);

		IsDGChanged = true;
	}
	
	else if (ArgData.isFlagSet(DeleteAnimCurveFlag[0]))
	{
		Status = AnimUtil::DeleteAnimCurveNodes(SelectedObject, MStringArray(AttributNames, 3), DGModifier);
		CHECK_MSTATUS_AND_RETURN_IT(Status);

		IsDGChanged = true;
	}
	else if (ArgData.isFlagSet(SetKeyFlag[0]))
	{
		Status = AnimUtil::SetKeys(SelectedObject, MStringArray(AttributNames, 3), AnimCurveChange);
		CHECK_MSTATUS_AND_RETURN_IT(Status);

		IsAnimCurveChange = true;
	}
	else if (ArgData.isFlagSet(RemoveKeyFlag[0]))
	{
		Status = AnimUtil::RemoveKeys(SelectedObject, MStringArray(AttributNames, 3), AnimCurveChange);
		CHECK_MSTATUS_AND_RETURN_IT(Status);

		IsAnimCurveChange = true;
	}
	return Status;
}
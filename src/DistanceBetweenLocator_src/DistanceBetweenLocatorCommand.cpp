#include <DistanceBetweenLocator_include/DistanceBetweenLocatorCommand.h>
#include <DistanceBetweenLocator_include/DistanceBetweenLocator.h>

#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>

const MString DistanceBetweenLocatorCommand::CommandName = "DistanceBetweenLocatorCommand";

DistanceBetweenLocatorCommand::~DistanceBetweenLocatorCommand()
{ }

void* DistanceBetweenLocatorCommand::Creator()
{
	return new DistanceBetweenLocatorCommand;
}

MStatus DistanceBetweenLocatorCommand::doIt(const MArgList& Args)
{
	MStatus Status;

	MArgDatabase ArgData(syntax(), Args, &Status);
	if (!Status)
	{
		MGlobal::displayError("Failed Pars The Args");
		return Status;
	}

	MSelectionList Selection;
	ArgData.getObjects(Selection);

	if (Selection.length() != 2)
	{
		MGlobal::displayError("Requier 2 Objects");
		return MS::kFailure;
	}
	
	TransformObj = DagModifier.createNode("transform");
	DagModifier.renameNode(TransformObj, DistanceBetweenLocator::TypeName + "1");

	DistanceBetweenObj = DagModifier.createNode(DistanceBetweenLocator::TypeId, TransformObj);
	 

	for (unsigned i = 0; i < Selection.length(); i++)
	{
		MObject Obj;
		Selection.getDependNode(i, Obj);

		MPlug TranslatePlug = MFnDependencyNode(Obj).findPlug("translate", false);
		MPlug PointPlug = MFnDependencyNode(DistanceBetweenObj).findPlug(MString("Point") + (i + 1), false);

		DagModifier.connect(TranslatePlug, PointPlug);
	}

	//for (int i = 0; i < Selection.length(); i++)
	//{
	//	MObject Obj;
	//	Selection.getDependNode(i, Obj);

	//	MPlug Translate = MFnDependencyNode(Obj).findPlug("translate", false);
	//	MPlug Point = MFnDependencyNode(DistanceBetweenObj).findPlug(MString("Point") + (i + 1), false);

	//	for (int j = 0; j < Translate.numChildren(); j++)
	//	{
	//		Status = DagModifier.connect(Translate.child(j), Point.child(j));
	//	}
	//}


	return redoIt();
}

MStatus DistanceBetweenLocatorCommand::undoIt()
{
	DagModifier.undoIt();
	return MS::kSuccess;

}
MStatus DistanceBetweenLocatorCommand::redoIt()
{
	DagModifier.doIt();

	MString TransformName = MFnDependencyNode(TransformObj).name();

	MFnDependencyNode(DistanceBetweenObj).setName(GetShapeName(TransformName));

	return MS::kSuccess;
}
bool DistanceBetweenLocatorCommand::isUndoable() const
{
	return true;
}

MSyntax DistanceBetweenLocatorCommand::CreateSyntax()
{
	MSyntax Syntax;

	Syntax.setObjectType(MSyntax::kSelectionList, 2, 2);
	Syntax.useSelectionAsDefault(true);

	return Syntax;
}

MString DistanceBetweenLocatorCommand::GetShapeName(const MString& TransformName)
{
	const int Start = DistanceBetweenLocator::TypeName.length();
	const int End = TransformName.length() - 1;

	MString Number = TransformName.substring(Start, End);

	MString ShapeName = DistanceBetweenLocator::TypeName + "Shape" + Number;

	return ShapeName;
}
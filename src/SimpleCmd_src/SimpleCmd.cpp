#include <SimpleCmd_include/SimpleCmd.h>

#include <maya/MSyntax.h>
#include <maya/MSelectionList.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnTransform.h>
#include <maya/MDoubleArray.h>
 
static const char* VersionFlag[] = { "v" , "version" };
static const char* TranslateFlag[] = {  "t" , "translate"};

SimpleCmd::SimpleCmd()
	: IsUndoable(false), IsTranslate(false), IsQuery(false), IsEdit(false), IsVersion(false), OldTranslate{}, NewTranslate{}
{

}
SimpleCmd::~SimpleCmd() 
{
}

const MString SimpleCmd::CommandName = "SimpleCmd";

void* SimpleCmd::Creator()
{
	return new SimpleCmd;
}
MSyntax SimpleCmd::CreateSyntax()
{
	MSyntax Syntax;

	Syntax.addFlag(VersionFlag[0], VersionFlag[1]);
	Syntax.addFlag(TranslateFlag[0], TranslateFlag[1], MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);

	Syntax.enableQuery(true);
	Syntax.enableEdit(true);

	Syntax.useSelectionAsDefault(true);
	Syntax.setObjectType(MSyntax::kSelectionList, 1, 1);

	return Syntax;
}

MStatus SimpleCmd::doIt(const MArgList& ArgList)
{
	MStatus Status;
	MArgDatabase Data(syntax(), ArgList, &Status);
	if (!Status)
		return Status;

	IsEdit = Data.isEdit();
	IsQuery = Data.isQuery();
	IsVersion = Data.isFlagSet(VersionFlag[0]);
	IsTranslate = Data.isFlagSet(TranslateFlag[0]);

	if (IsVersion)
		return redoIt();


	MSelectionList SelectionList;
	Data.getObjects(SelectionList);
	if (SelectionList.length() != 1)
	{
		displayError("Select One Object");
		return MS::kFailure;
	}


	SelectionList.getDependNode(0, SelectedObj);
	if (!SelectedObj.hasFn(MFn::kTransform))
	{
		displayError("Need Transform");
		return MS::kFailure;
	}


	MFnTransform TransformFn(SelectedObj);
	TransformFn.getTranslation(MSpace::kObject).get(OldTranslate);

	if (IsEdit)
	{
		if (IsTranslate)
		{
			NewTranslate[0] = Data.flagArgumentDouble(TranslateFlag[0], 0);
			NewTranslate[1] = Data.flagArgumentDouble(TranslateFlag[0], 1);
			NewTranslate[2] = Data.flagArgumentDouble(TranslateFlag[0], 2);

			IsUndoable = true;
		}
	}

	redoIt();

	return MS::kSuccess;
}

MStatus SimpleCmd::undoIt()
{
	MFnTransform TransformFn(SelectedObj);
	TransformFn.setTranslation(MVector(OldTranslate), MSpace::kObject);

	return MS::kSuccess;

}
MStatus SimpleCmd::redoIt()
{
	if (IsVersion)
	{
		if (IsEdit)
		{
			displayError("This Flag Doesn't Support Edit");
			return MS::kFailure;
		}
		setResult("1.0");
	}
	else if (IsTranslate)
	{
		if (IsQuery)
		{
			setResult(MDoubleArray(OldTranslate, 3));
			return MS::kSuccess;

		}
		if (IsEdit)
		{
			MFnTransform TransformFn(SelectedObj);
			TransformFn.setTranslation(MVector(NewTranslate), MSpace::kObject);
			return MS::kSuccess;
		}
		else
		{
			displayError("Need To Specify -Edit or -Query Flag");
			return MS::kFailure;
		}
	}
	else
	{
		setResult(MFnDependencyNode(SelectedObj).name());
	}

	return MS::kSuccess;
}
bool SimpleCmd::isUndoable() const
{
	return IsUndoable;
}
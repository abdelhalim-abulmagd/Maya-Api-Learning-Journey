#include <RetimingTool_include/RetimingToolCmd.h>

#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>
#include <maya/MTimeArray.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MFnAnimCurve.h>


static const char* ValueFlag[]{ "v", "value" };
static const char* IncrementalFlag[]{ "i", "incremental" };

const MString RetimingToolCmd::CommandName = "RetimingToolCmd";


RetimingToolCmd::RetimingToolCmd() {}
RetimingToolCmd::~RetimingToolCmd() {}
void* RetimingToolCmd::Creator() { return new RetimingToolCmd; }
bool RetimingToolCmd::isUndoable() const { return true; }

MSyntax RetimingToolCmd::CreateSyntax()
{
	MSyntax Syntax;

	Syntax.addFlag(IncrementalFlag[0], IncrementalFlag[1]);
	Syntax.addFlag(ValueFlag[0], ValueFlag[1], MSyntax::kDouble);

	Syntax.setObjectType(MSyntax::kSelectionList);
	Syntax.useSelectionAsDefault(true);

	return Syntax;
}

MStatus RetimingToolCmd::undoIt()
{
	return MS::kSuccess;

}
MStatus RetimingToolCmd::redoIt()
{
	return MS::kSuccess;
}


MStatus RetimingToolCmd::doIt(const MArgList& Args)
{
	MStatus Status;

	MArgDatabase ArgData(syntax(), Args, &Status);
	CHECK_MSTATUS_AND_RETURN_IT(Status);

	MSelectionList SelectedObjects;
	Status = ArgData.getObjects(SelectedObjects);
	CHECK_MSTATUS_AND_RETURN_IT(Status);

	if (ArgData.isFlagSet(ValueFlag[0]))
	{
		double RetimingValue = ArgData.flagArgumentDouble(ValueFlag[0], 0, &Status);
		CHECK_MSTATUS_AND_RETURN_IT(Status);

		bool Incremental = ArgData.isFlagSet(IncrementalFlag[0]);

		DoRetiming(SelectedObjects, RetimingValue, Incremental);
	}
	else
	{
		displayError("Flag -Value need to be set");
	}


	return MS::kSuccess;
}

MTimeArray RetimingToolCmd::GetSelectedRange()
{
	MString PlayBackSlider = MGlobal::executeCommandStringResult("$PlayBackSlider_Control_Name = $gPlayBackSlider");

	MDoubleArray PlayBackSliderRange;
	MGlobal::executeCommand("timeControl - q - rangeArray $gPlayBackSlider", PlayBackSliderRange);

	MTime StartTime(PlayBackSliderRange[0], MTime::uiUnit());
	MTime EndTime((PlayBackSliderRange[1] -= 1), MTime::uiUnit());
	
	MTimeArray TimeArray;
	TimeArray.append(StartTime);
	TimeArray.append(EndTime);

	return TimeArray;
}

void RetimingToolCmd::GetAnimCurves(MSelectionList& SelectedObjects, MObjectArray& AnimCurveObjects)
{
	MObject CurrentDependNodeObj;
	MFnDependencyNode DependNodeFn;

	MItSelectionList SelectionListItr(SelectedObjects);
	while (!SelectionListItr.isDone())
	{
		SelectionListItr.getDependNode(CurrentDependNodeObj);

		DependNodeFn.setObject(CurrentDependNodeObj);

		GetAnimCurvesFromConnections(DependNodeFn, AnimCurveObjects);

		SelectionListItr.next();
	}

}

void RetimingToolCmd::GetAnimCurvesFromConnections(MFnDependencyNode& DependNodeFn, MObjectArray& AnimCurveObjects)
{
	MObjectArray Objects;

	MPlugArray Plugs;
	DependNodeFn.getConnections(Plugs);

	for (MPlug& Plug : Plugs)
	{
		if (Plug.isKeyable() && !Plug.isLocked())
		{
			MItDependencyGraph DependencyGraphItr(Plug, MFn::kAnimCurve,
														MItDependencyGraph::Direction::kUpstream,
														MItDependencyGraph::kBreadthFirst,
														MItDependencyGraph::kNodeLevel);

			while (!DependencyGraphItr.isDone())
			{
				DependencyGraphItr.getNodePath(Objects);

				if (Objects.length() > 2)
					break;

				AnimCurveObjects.append(DependencyGraphItr.currentItem());

				DependencyGraphItr.next();
			}

		}
	}
}

void RetimingToolCmd::DoRetiming(MSelectionList& SelectedObjects, double RetimingValue, bool Incremental)
{
	MTimeArray StartAndEndTime = GetSelectedRange();
	MTime StartTimeRange = StartAndEndTime[0];
	MTime EndTimeRange = StartAndEndTime[1];

	MObjectArray AnimCurveObjects;
	GetAnimCurves(SelectedObjects, AnimCurveObjects);

	MFnAnimCurve AnimCurveFn;
	for (MObject& AnimCurveObject : AnimCurveObjects)
	{
		AnimCurveFn.setObject(AnimCurveObject);
		
		std::vector<std::vector<double>> KeyData = CalculateRetimingKeys(AnimCurveFn, StartTimeRange, EndTimeRange, RetimingValue, Incremental);

		for (std::vector<double>& a : KeyData)
		{
			displayInfo(MString("Index :") + a[0] + ", " + a[1] + ", " + a[2]);
		}

	}	
}

unsigned RetimingToolCmd::FindClosestIndex(MFnAnimCurve& AnimCurveFn, MTime& TargetTime)
{
	unsigned Index = AnimCurveFn.findClosest(TargetTime);

	MTime ClosestTime = AnimCurveFn.time(Index);

	if (ClosestTime > TargetTime)
	{
		Index--;
	}

	return Index;
}

std::vector<std::vector<double>> RetimingToolCmd::CalculateRetimingKeys(MFnAnimCurve& AnimCurveFn, MTime& StartTimeRange, MTime& EndTimeRange, double RetimingValue, bool Incremental)
{
	std::vector<std::vector<double>> KeyData;	

	unsigned NumKeys = AnimCurveFn.numKeys();

	unsigned FirstKeyIndex = FindClosestIndex(AnimCurveFn, StartTimeRange);
	unsigned LastKeyIndex = FindClosestIndex(AnimCurveFn, EndTimeRange);

	if (NumKeys == 0 || FirstKeyIndex < 0 ) return KeyData;

	double FirstKeyTime = AnimCurveFn.time(FirstKeyIndex).value();
	KeyData.push_back(std::vector<double>{double(FirstKeyIndex), FirstKeyTime, FirstKeyTime});

	for (unsigned Index = FirstKeyIndex + 1; Index < NumKeys; Index++)
	{
		if (Index > LastKeyIndex) break;

		double NextKeyTime = AnimCurveFn.time(Index).value();
		KeyData.push_back(std::vector<double>{double(Index), NextKeyTime, NextKeyTime});
	}

	for (unsigned Index = FirstKeyIndex + 1; Index < NumKeys; Index++)
	{
		if (Index > LastKeyIndex) break;

		double NewTime = 0;
		if (Incremental)
		{
			NewTime = KeyData[Index][2] - KeyData[Index - 1][2];
			if (NewTime < 0)
				NewTime = 1;
		}

		NewTime += KeyData[Index - 1][1] + RetimingValue;
		KeyData[Index][1] = NewTime;
	}

	return KeyData;

}


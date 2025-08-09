#pragma once

#include <maya/MPxCommand.h>
#include <vector>

class RetimingToolCmd : public MPxCommand
{
public:
	RetimingToolCmd();
	virtual ~RetimingToolCmd() override;

	static const MString CommandName;
	static void* Creator();
	static MSyntax CreateSyntax();

	virtual bool isUndoable() const;
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	
	MStatus doIt(const MArgList& Args);

private:

	MTimeArray GetSelectedRange();

	void GetAnimCurves(MSelectionList& SelectedObjects, MObjectArray& AnimCurveObjects);

	void GetAnimCurvesFromConnections(MFnDependencyNode& DependNodeFn, MObjectArray& AnimCurveObjects);

	void DoRetiming(MSelectionList& SelectedObjects, double RetimingValue, bool Incremental);

	unsigned FindClosestIndex(MFnAnimCurve& AnimCurveFn, MTime& TargetTime);

	std::vector<std::vector<double>> CalculateRetimingKeys(MFnAnimCurve& AnimCurveFn, MTime& StartTimeRange, MTime& EndTimeRange, double RetimingValue, bool Incremental);

};

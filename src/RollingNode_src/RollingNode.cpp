#include <RollingNode_include/RollingNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>

RollingNode::~RollingNode()
{
}

const MString RollingNode::TypeName = "RollingNode";
const MTypeId RollingNode::TypeId = MTypeId(0x0007F7F3);

MObject RollingNode::DistanceObj;
MObject RollingNode::RadiosObj;
MObject RollingNode::RotationObj;

void* RollingNode::Creator()
{
	return new RollingNode;
}

MStatus RollingNode::Initialize()
{
	MFnNumericAttribute NumericAttr;

	DistanceObj = NumericAttr.create("Distance", "Distance", MFnNumericData::kDouble);
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);

	RadiosObj = NumericAttr.create("Radios", "Radios", MFnNumericData::kDouble);
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);

	MFnUnitAttribute UnitAttr;

	RotationObj = UnitAttr.create("Rotation", "Rotation", MFnUnitAttribute::kAngle);
	UnitAttr.setWritable(false);

	addAttribute(DistanceObj);
	addAttribute(RadiosObj);
	addAttribute(RotationObj);

	attributeAffects(DistanceObj, RotationObj);
	attributeAffects(RadiosObj, RotationObj);

	return MS::kSuccess;
}
MStatus RollingNode::compute(const MPlug& Plug, MDataBlock& Data)
{
	if (Plug != RotationObj)
		return MS::kInvalidParameter;

	double Distance = Data.inputValue(DistanceObj).asDouble();
	double Radios = Data.inputValue(RadiosObj).asDouble();

	double Rotation = 0;
	if (Radios != 0)
		Rotation = Distance / Radios;

	Data.outputValue(RotationObj).setDouble(Rotation);

	Data.setClean(Plug);

	return MS::kSuccess;
}
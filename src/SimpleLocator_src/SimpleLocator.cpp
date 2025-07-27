#include <SimpleLocator_include/SimpleLocator.h>

#include <maya/MFnNumericAttribute.h>

const MString SimpleLocator::TypeName = "SimpleLocator";
const MTypeId SimpleLocator::TypeId = MTypeId(0x0007F7F7);

const MString SimpleLocator::DrawDbClassification = "drawdb/geometry/SimpleLocator";
const MString SimpleLocator::RegistrantId = "SimpleLocator";

MObject SimpleLocator::ShapeIndexObj;

SimpleLocator::~SimpleLocator()
{
}

void* SimpleLocator::Creator()
{
	return new SimpleLocator;
}
MStatus SimpleLocator::Initialize()
{
	MFnNumericAttribute NumericAttr;

	ShapeIndexObj = NumericAttr.create("ShapeIndex", "ShapeIndex", MFnNumericData::kInt);
	NumericAttr.setKeyable(true);
	NumericAttr.setMin(0);
	NumericAttr.setMax(2);

	addAttribute(ShapeIndexObj);

	return MS::kSuccess;
}

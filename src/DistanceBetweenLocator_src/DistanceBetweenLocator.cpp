#include <DistanceBetweenLocator_include/DistanceBetweenLocator.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MRampAttribute.h>

static float ColorRampPositions[]{ 0, 1 };
static MColor ColorRampValues[]{ MColor(0,0,0), MColor(1,1,1) };
static int ColorRampInterps[]{ MRampAttribute::kLinear, MRampAttribute::kLinear };

static float CurveRampPositions[]{ 0.0f, 0.5f, 1.0f };
static float CurveRampValues[]{ 0.0f, 1.0f, 0.0f };
static int CurveRampInterps[]{ MRampAttribute::kSmooth, MRampAttribute::kSmooth,  MRampAttribute::kSmooth };

MObject DistanceBetweenLocator::Point1Obj;
MObject DistanceBetweenLocator::Point2Obj;

MObject DistanceBetweenLocator::DistanceObj;

MObject DistanceBetweenLocator::MinDistanceObj;
MObject DistanceBetweenLocator::MaxDistanceObj;

MObject DistanceBetweenLocator::ColorRamp;
MObject DistanceBetweenLocator::CurveRamp;

const MString DistanceBetweenLocator::TypeName = "DistanceBetweenLocator";
const MTypeId DistanceBetweenLocator::TypeId = MTypeId(0x0007F7F8);

const MString DistanceBetweenLocator::DrawDbClassification = "drawdb/geometry/DistanceBetweenLocator";
const MString DistanceBetweenLocator::RegistrantId = "DistanceBetweenLocator";


DistanceBetweenLocator::~DistanceBetweenLocator()
{
}

void* DistanceBetweenLocator::Creator()
{
	return new DistanceBetweenLocator;
}

MStatus DistanceBetweenLocator::Initialize()
{
	MFnNumericAttribute NumericAttr;

	Point1Obj = NumericAttr.createPoint("Point1", "Point1");
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);

	Point2Obj = NumericAttr.createPoint("Point2", "Point2");
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);

	DistanceObj = NumericAttr.create("Distance", "Distance", MFnNumericData::kDouble);
	NumericAttr.setWritable(false);

	MinDistanceObj = NumericAttr.create("MinDistance", "MinDistance", MFnNumericData::kFloat, 0.0f);
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);
	NumericAttr.setMin(0.0);

	MaxDistanceObj = NumericAttr.create("MaxDistance", "MaxDistance", MFnNumericData::kFloat, 15.0f);
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);
	NumericAttr.setMin(0.0);

	MRampAttribute RampAttr;
	ColorRamp = RampAttr.createColorRamp("ColorRamp", "ColorRamp");
	CurveRamp = RampAttr.createCurveRamp("CurveRamp", "CurveRamp");

	addAttribute(MinDistanceObj);
	addAttribute(MaxDistanceObj);

	addAttribute(Point1Obj);
	addAttribute(Point2Obj);
	addAttribute(DistanceObj);

	addAttribute(ColorRamp);
	addAttribute(CurveRamp);

	attributeAffects(Point1Obj, DistanceObj);
	attributeAffects(Point2Obj, DistanceObj);
	
	return MS::kSuccess;
}

MStatus DistanceBetweenLocator::compute(const MPlug& Plug, MDataBlock& Data) 
{
	if (Plug != DistanceObj)
		return MS::kInvalidParameter;

	MPoint Point1(Data.inputValue(Point1Obj).asFloatVector());
	MPoint Point2(Data.inputValue(Point2Obj).asFloatVector());

	Data.outputValue(DistanceObj).setDouble(Point1.distanceTo(Point2));
	
	Data.setClean(Plug);
	return MS::kSuccess;

}

void DistanceBetweenLocator::postConstructor()
{
	MRampAttribute RampAttr(thisMObject(), ColorRamp);
	
	MFloatArray ColorPositions(::ColorRampPositions, 2);
	MColorArray ColorValues(ColorRampValues, 2);
	MIntArray ColorInterps(ColorRampInterps, 2);

	RampAttr.addEntries(ColorPositions, ColorValues, ColorInterps);


	MRampAttribute CurveAttr(thisMObject(), CurveRamp);
	
	MFloatArray CurvePositions(CurveRampPositions, 3);
	MFloatArray CurveValues(CurveRampValues, 3);
	MIntArray CurveInterps(CurveRampInterps, 3);

	CurveAttr.addEntries(CurvePositions, CurveValues, CurveInterps);

}
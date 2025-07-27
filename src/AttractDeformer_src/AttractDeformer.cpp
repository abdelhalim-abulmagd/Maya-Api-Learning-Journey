#include <AttractDeformer_include/AttractDeformer.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MVector.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MItGeometry.h>
#include <maya/MMatrix.h>
#include <maya/MDagModifier.h>

AttractDeformer::AttractDeformer() 
	: MaxAngle(0.5f * 3.14159265f) // 90 degrees
{
}

AttractDeformer::~AttractDeformer()
{
}

const MString AttractDeformer::TypeName = "AttractDeformer";
const MTypeId AttractDeformer::TypeId = MTypeId(0x0007F7F6);;

MObject AttractDeformer::TargetPositionObj;
MObject AttractDeformer::MaxDistanceObj;

void* AttractDeformer::Creator()
{
	return new AttractDeformer;

}
MStatus AttractDeformer::Initialize()
{
	MFnNumericAttribute NumericAttr;

	TargetPositionObj = NumericAttr.createPoint("TargetPosition", "TargetPosition");
	NumericAttr.setKeyable(true);

	MaxDistanceObj = NumericAttr.create("MaxDistance", "MaxDistance", MFnNumericData::kFloat, 1.0);
	NumericAttr.setKeyable(true);
	NumericAttr.setMin(0);
	NumericAttr.setMax(2);

	addAttribute(MaxDistanceObj);
	addAttribute(TargetPositionObj);

	attributeAffects(MaxDistanceObj, outputGeom);
	attributeAffects(TargetPositionObj, outputGeom);

	return MS::kSuccess;
}

MStatus AttractDeformer::deform(MDataBlock& Data, MItGeometry& GeoItr, const MMatrix& WorldMatrix, unsigned int IndexGeo)
{
	float Envelope = Data.inputValue(envelope).asFloat();
	float MaxDistance = Data.inputValue(MaxDistanceObj).asFloat();

	if (Envelope == 0 || MaxDistance == 0)
		return MS::kSuccess;

	MFloatVector Target = Data.inputValue(TargetPositionObj).asFloatVector();
	Target = MPoint(Target) * WorldMatrix.inverse();

	MArrayDataHandle Inputs = Data.outputArrayValue(input);
	Inputs.jumpToElement(IndexGeo);
	MObject InputMeshObj = Inputs.outputValue().child(inputGeom).asMesh();;
	MFnMesh MeshFn(InputMeshObj);

	MFloatVectorArray Normals;
	MeshFn.getVertexNormals(false, Normals, MSpace::kObject);

	GeoItr.reset();
	while (!GeoItr.isDone())
	{
		
		MPoint LocalPoint = GeoItr.position();
		
		MFloatVector TargetVector = Target - MFloatVector(LocalPoint);

		float CurrentDistance = TargetVector.length();
		if (CurrentDistance <= MaxDistance)
		{

			MFloatVector Normal = Normals[GeoItr.index()];

			float Angle = Normal.angle(TargetVector);
			if (Angle <= MaxAngle)
			{

				MFloatVector Offset = TargetVector * ((MaxDistance - CurrentDistance) / MaxDistance);
				LocalPoint = MFloatVector(LocalPoint) + Offset;

				GeoItr.setPosition(LocalPoint);
			}
		}

		GeoItr.next();
	}

	return MS::kSuccess;
}
	
MObject& AttractDeformer::accessoryAttribute() const
{
	return TargetPositionObj;
}

MStatus	AttractDeformer::accessoryNodeSetup(MDagModifier& DagModifier)
{
	MObject LocatorObj = DagModifier.createNode("locator");

	MPlug TranslatePlug = MFnDependencyNode(LocatorObj).findPlug("translate", false);

	MPlug TargetPositionPlug(thisMObject(), TargetPositionObj);

	DagModifier.connect(TranslatePlug, TargetPositionPlug);
	

	return MS::kSuccess;

}
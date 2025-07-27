#include <BlendShapeDeformer_include/BlendShapeDeformer.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MItGeometry.h>
#include <maya/MGlobal.h>

MObject BlendShapeDeformer::TargetGeoObj;

BlendShapeDeformer::~BlendShapeDeformer()
{ }

const MString BlendShapeDeformer::TypeName = "BlendShapeDeformer";
const MTypeId BlendShapeDeformer::TypeId = MTypeId(0x0007F7F5);

void* BlendShapeDeformer::Creator()
{
	return new BlendShapeDeformer;
}
MStatus BlendShapeDeformer::Initialize()
{
	MFnTypedAttribute TypedAttr;
	TargetGeoObj = TypedAttr.create("TargetGeo", "TargetGeo", MFnData::kMesh);

	addAttribute(TargetGeoObj);
	attributeAffects(TargetGeoObj, outputGeom);


	return MS::kSuccess;
}
MStatus BlendShapeDeformer::deform(MDataBlock& Data, MItGeometry& GeoItr, const MMatrix& WorldMatrix, unsigned int IndexGeo)
{

	float Envelop = Data.inputValue(envelope).asFloat();
	MObject Target = Data.inputValue(TargetGeoObj).asMesh();

	if (Envelop == 0 || Target.isNull())
		return MS::kSuccess;


	MFnMesh MeshFn(Target);

	MPointArray TargetPoints;
	MeshFn.getPoints(TargetPoints, MSpace::kObject);

	MPoint LocalPoint;
	int CurrentIndex;
	float Weight;

	GeoItr.reset();
	while (!GeoItr.isDone())
	{
		CurrentIndex = GeoItr.index();
		Weight = weightValue(Data, IndexGeo, CurrentIndex);

		LocalPoint = GeoItr.position();

		LocalPoint = LocalPoint + (TargetPoints[CurrentIndex] - LocalPoint) * Envelop * Weight;

		GeoItr.setPosition(LocalPoint);
		GeoItr.next();
	}

	return MS::kSuccess;

}
#include <BasicDeformer_include/BasicDeformer.h>
#include <maya/MItGeometry.h>
#include <maya/MPoint.h>
#include <maya/MMatrix.h>

const MString BasicDeformer::TypeName = "BasicDeformer";
const MTypeId BasicDeformer::TypeId = MTypeId(0x0007F7F4);

BasicDeformer::~BasicDeformer() 
{

}
void* BasicDeformer::Creator()
{
	return new BasicDeformer;
}
MStatus BasicDeformer::Initialize()
{
	
	return MS::kSuccess;
}

MStatus BasicDeformer::deform(MDataBlock& Data, MItGeometry& GeoIter, const MMatrix& WorldMatrix, unsigned int IndexGeo)
{
	float Envelope = Data.inputValue(envelope).asFloat();
	if (Envelope == 0)
		return MS::kSuccess;

	int CurrentIndex;
	MPoint LocalPosition;

	GeoIter.reset();
	while (!GeoIter.isDone())
	{
		CurrentIndex = GeoIter.index();
		if (CurrentIndex % 2)
		{
			LocalPosition = GeoIter.position() * WorldMatrix * Envelope;
			//LocalPosition.y += 2;

			GeoIter.setPosition(MPoint(LocalPosition));
		}


		GeoIter.next();
	}

	return MS::kSuccess;

}
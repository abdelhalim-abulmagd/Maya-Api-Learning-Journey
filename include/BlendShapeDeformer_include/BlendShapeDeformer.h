#pragma once

#include <maya/MPxDeformerNode.h>

class BlendShapeDeformer : public MPxDeformerNode
{
public:
	virtual ~BlendShapeDeformer() override;

	static const MString TypeName;
	static const MTypeId TypeId;

	static void* Creator();
	static MStatus Initialize();

	virtual MStatus deform(MDataBlock& Data, MItGeometry& GeoItr, const MMatrix& WorldMatrix, unsigned int IndexGeo) override;

private:

	static MObject TargetGeoObj;
};
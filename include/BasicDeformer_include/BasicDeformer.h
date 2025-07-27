#pragma once

#include <maya/MPxDeformerNode.h>

class BasicDeformer : public MPxDeformerNode
{
public:
	virtual ~BasicDeformer() override;

	static const MString TypeName;
	static const MTypeId TypeId;

	static void* Creator();
	static MStatus Initialize();

	virtual MStatus deform(MDataBlock& Data, MItGeometry& GeoIter, const MMatrix& WorldMatrix, unsigned int IndexGeo) override;


};
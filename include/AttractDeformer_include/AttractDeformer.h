#pragma once

#include <maya/MPxDeformerNode.h>

class AttractDeformer : public MPxDeformerNode
{
public:
	AttractDeformer();

	virtual ~AttractDeformer() override;

	static const MString TypeName;
	static const MTypeId TypeId;

	static void* Creator();
	static MStatus Initialize();

	virtual MStatus deform(MDataBlock& Data, MItGeometry& GeoItr, const MMatrix& WorldMatrix, unsigned int IndexGeo) override;

	virtual MObject& accessoryAttribute() const override;
	virtual MStatus	accessoryNodeSetup(MDagModifier& DagModifier) override;

private:
	static MObject TargetPositionObj;
	static MObject MaxDistanceObj;

	float MaxAngle;
};
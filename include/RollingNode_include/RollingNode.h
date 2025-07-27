#pragma once

#include <maya/MPxNode.h>

class RollingNode : public MPxNode
{
public:
	virtual ~RollingNode() override;

	static const MString TypeName;
	static const MTypeId TypeId;

	static void* Creator();
	static MStatus Initialize();

	virtual MStatus compute(const MPlug& Plug, MDataBlock& Data) override;

private:
	static MObject DistanceObj;
	static MObject RadiosObj;
	static MObject RotationObj;

};
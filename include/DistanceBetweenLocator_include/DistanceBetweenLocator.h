#pragma once

#include <maya/MPxLocatorNode.h>

class DistanceBetweenLocator : public MPxLocatorNode
{
public:
	virtual ~DistanceBetweenLocator() override;
	
	static const MString TypeName;
	static const MTypeId TypeId;

	static const MString DrawDbClassification;
	static const MString RegistrantId;

	static void* Creator();
	static MStatus Initialize();

	MStatus compute(const MPlug& Plug, MDataBlock& Data) override;

	virtual void postConstructor() override;
private:
	static MObject Point1Obj;
	static MObject Point2Obj;

	static MObject DistanceObj;
	
	static MObject MinDistanceObj;
	static MObject MaxDistanceObj;

	static MObject ColorRamp;
	static MObject CurveRamp;
};
#pragma once

#include <maya/MPxLocatorNode.h>

class SimpleLocator : public MPxLocatorNode
{
public:
	virtual ~SimpleLocator() override;

	static const MString TypeName;
	static const MTypeId TypeId;
	
	static const MString DrawDbClassification;
	static const MString RegistrantId;

	static void* Creator();
	static MStatus Initialize();

private:
	static MObject ShapeIndexObj;
};
#pragma once

#include <maya/MPxLocatorNode.h>

class HelloWorldLocator : public MPxLocatorNode
{
public:
	
	static const MString TypeName;
	static const MTypeId TypeId;

	static const MString DrawDbClassification;
	static const MString DrawRegistrantId;

	static void* Creator();
	static MStatus Initialize();

};

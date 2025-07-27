#include <HelloWorldLocator_include/HelloWorldLocator.h>

const MString HelloWorldLocator::TypeName = "HelloWorldLocator";
const MTypeId HelloWorldLocator::TypeId = MTypeId(0x0007F7F1);

const MString HelloWorldLocator::DrawDbClassification = "drawdb/geometry/HelloWorldLocator";
const MString HelloWorldLocator::DrawRegistrantId = "HelloWorldLocator";

void* HelloWorldLocator::Creator()
{
	return new HelloWorldLocator;
}

MStatus HelloWorldLocator::Initialize()
{
	return MS::kSuccess;
}


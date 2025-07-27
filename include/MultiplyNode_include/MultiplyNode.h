#pragma once

#include <maya/MPxNode.h>


class  MultiplyNode : public MPxNode
{
public:
	virtual ~MultiplyNode() override;

	static const MString TypeName;
	static const MTypeId TypeId;

	static void* Creator();
	static MStatus Initialize();

	virtual MStatus compute(const MPlug& Plug, MDataBlock& DataBlock) override;

private:
	static MObject Multiplier;
	static MObject Multiplicand;
	static MObject Product;
};
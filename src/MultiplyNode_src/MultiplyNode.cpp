#include <MultiplyNode_include/MultiplyNode.h>

#include <maya/MFnNumericAttribute.h>

const MString MultiplyNode::TypeName = "MultiplyNode";
const MTypeId MultiplyNode::TypeId = MTypeId(0x0007F7F2);

MObject MultiplyNode::Multiplier;
MObject MultiplyNode::Multiplicand;
MObject MultiplyNode::Product;

MultiplyNode::~MultiplyNode()
{
}

void* MultiplyNode::Creator()
{
	return new MultiplyNode;
}
MStatus MultiplyNode::Initialize()
{
	MFnNumericAttribute NumericAttr;

	Multiplier = NumericAttr.create("Multiplier", "Multiplier", MFnNumericData::kFloat);
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);

	Multiplicand = NumericAttr.create("Multiplicand", "Multiplicand", MFnNumericData::kFloat);
	NumericAttr.setKeyable(true);
	NumericAttr.setReadable(false);

	Product = NumericAttr.create("Product", "Product", MFnNumericData::kFloat);
	NumericAttr.setWritable(false);

	addAttribute(Multiplier);
	addAttribute(Multiplicand);
	addAttribute(Product);

	attributeAffects(Multiplier, Product);
	attributeAffects(Multiplicand, Product);

	return MS::kSuccess;
}

MStatus MultiplyNode::compute(const MPlug& Plug, MDataBlock& DataBlock)
{
	if (Plug != Product)
		return MS::kInvalidParameter;

	float MultiplierValue = DataBlock.inputValue(Multiplier).asFloat();
	float MultiplicandValue = DataBlock.inputValue(Multiplicand).asFloat();

	float NewProduct = MultiplierValue * MultiplicandValue;

	DataBlock.outputValue(Product).setFloat(NewProduct);
	
	DataBlock.setClean(Plug);

	return MS::kSuccess;
}
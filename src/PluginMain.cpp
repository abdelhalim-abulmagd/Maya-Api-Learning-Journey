#include <HelloWorldCmd_include/HelloWorldCmd.h>
#include <HelloWorldLocator_include/HelloWorldLocator.h>
#include <HelloWorldLocator_include/HelloWorldDrawOverride.h>
#include <MultiplyNode_include/MultiplyNode.h>
#include <RollingNode_include/RollingNode.h>
#include <SimpleCmd_include/SimpleCmd.h>

#include <BasicDeformer_include/BasicDeformer.h>
#include <BlendShapeDeformer_include/BlendShapeDeformer.h>
#include <AttractDeformer_include/AttractDeformer.h>

#include <CostumeSelectContext_include/CostumeSelectContextCommand.h>
#include <JointCreateContext_include/JointCreateContextCommand.h>

#include <SimpleLocator_include/SimpleLocator.h>
#include <SimpleLocator_include/SimpleLocatorDrawOverride.h>

#include <DistanceBetweenLocator_include/DistanceBetweenLocator.h>
#include <DistanceBetweenLocator_include/DistanceBetweenLocatorDrawOverride.h>
#include <DistanceBetweenLocator_include/DistanceBetweenLocatorCommand.h>


#include <AnimCurve_include/AnimCurveCmd.h>

#include <RetimingTool_include/RetimingToolCmd.h>

#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>
#include <maya/MGlobal.h>

/*
#include <maya/MCallbackIdArray.h>
#include <maya/MMessage.h>
#include <maya/MEventMessage.h>
#include <maya/MTimerMessage.h>

static MCallbackIdArray CallbackIDs;


void OnTimeChange(void* ClintData) // Callback function
{
	MGlobal::displayInfo("oh you change the time huh?");
}

void OnTimerFired(float time, float prevTime, void* ClintData) // Callback function
{
	MGlobal::displayInfo(MString() + time + ":" + prevTime);
}
*/


MStatus initializePlugin(MObject PluginObj)
{
	MStatus Status;
	MFnPlugin PluginFn(PluginObj);

	//CallbackIDs.append((MEventMessage::addEventCallback("timeChanged", OnTimeChange)));
	//CallbackIDs.append(MTimerMessage::addTimerCallback(1, OnTimerFired));
	

	Status = PluginFn.registerCommand(HelloWorldCmd::TypeName, HelloWorldCmd::Creator);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + HelloWorldCmd::TypeName);
		return Status;
	}

	Status = PluginFn.registerNode( HelloWorldLocator::TypeName, HelloWorldLocator::TypeId, HelloWorldLocator::Creator, HelloWorldLocator::Initialize, MPxNode::kLocatorNode, &HelloWorldLocator::DrawDbClassification);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + HelloWorldLocator::TypeName);
		return Status;
	}

	Status = MDrawRegistry::registerDrawOverrideCreator(HelloWorldLocator::DrawDbClassification, HelloWorldLocator::DrawRegistrantId, HelloWorldDrawOverride::Creator);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + HelloWorldDrawOverride::TypeName);
		return Status;
	}

	Status = PluginFn.registerNode(MultiplyNode::TypeName, MultiplyNode::TypeId, MultiplyNode::Creator, MultiplyNode::Initialize, MPxNode::kDependNode);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + MultiplyNode::TypeName);
		return Status;
	}

	Status = PluginFn.registerNode(RollingNode::TypeName, RollingNode::TypeId, RollingNode::Creator, RollingNode::Initialize, MPxNode::kDependNode);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + RollingNode::TypeName);
		return Status;
	}

	Status = PluginFn.registerCommand(SimpleCmd::CommandName, SimpleCmd::Creator, SimpleCmd::CreateSyntax);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + SimpleCmd::CommandName);
		return Status;
	}

	Status = PluginFn.registerNode(BasicDeformer::TypeName, BasicDeformer::TypeId, BasicDeformer::Creator, BasicDeformer::Initialize, MPxNode::kDeformerNode);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + BasicDeformer::TypeName);
		return Status;
	}
	Status = PluginFn.registerNode(BlendShapeDeformer::TypeName, BlendShapeDeformer::TypeId, BlendShapeDeformer::Creator, BlendShapeDeformer::Initialize, MPxNode::kDeformerNode);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + BlendShapeDeformer::TypeName);
		return Status;
	}
	MGlobal::executeCommand( "makePaintable -attrType multiFloat -sm deformer " + BlendShapeDeformer::TypeName +  " weights");

	Status = PluginFn.registerNode(AttractDeformer::TypeName, AttractDeformer::TypeId, AttractDeformer::Creator, AttractDeformer::Initialize, MPxNode::kDeformerNode);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + AttractDeformer::TypeName);
		return Status;
	}
	Status = PluginFn.registerContextCommand(CostumeSelectContextCommand::CommandName, CostumeSelectContextCommand::Creator);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + CostumeSelectContextCommand::CommandName);
		return Status;
	}
	Status = PluginFn.registerContextCommand(JointCreateContextCommand::CommandName, JointCreateContextCommand::Creator);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + JointCreateContextCommand::CommandName);
		return Status;
	}

	Status = PluginFn.registerNode(SimpleLocator::TypeName, SimpleLocator::TypeId, SimpleLocator::Creator, SimpleLocator::Initialize, MPxNode::kLocatorNode, &SimpleLocator::DrawDbClassification);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + SimpleLocator::TypeName);
		return Status;
	}

	Status = MDrawRegistry::registerDrawOverrideCreator(SimpleLocator::DrawDbClassification, SimpleLocator::RegistrantId, SimpleLocatorDrawOverride::Creator);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + SimpleLocatorDrawOverride::TypeName);
		return Status;
	}

	Status = PluginFn.registerNode(DistanceBetweenLocator::TypeName, DistanceBetweenLocator::TypeId, DistanceBetweenLocator::Creator, DistanceBetweenLocator::Initialize, MPxNode::kLocatorNode, &DistanceBetweenLocator::DrawDbClassification);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + DistanceBetweenLocator::TypeName);
		return Status;
	}

	Status = MDrawRegistry::registerDrawOverrideCreator(DistanceBetweenLocator::DrawDbClassification, DistanceBetweenLocator::RegistrantId, DistanceBetweenLocatorDrawOverride::Creator);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + DistanceBetweenLocatorDrawOverride::TypeName);
		return Status;
	}

	Status = PluginFn.registerCommand(DistanceBetweenLocatorCommand::CommandName, DistanceBetweenLocatorCommand::Creator, DistanceBetweenLocatorCommand::CreateSyntax);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + DistanceBetweenLocatorCommand::CommandName);
		return Status;
	}
	
	Status = PluginFn.registerCommand(AnimCurveCmd::CommandName, AnimCurveCmd::Creator, AnimCurveCmd::CreateSyntax);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + AnimCurveCmd::CommandName);
		return Status;
	}
	Status = PluginFn.registerCommand(RetimingToolCmd::CommandName, RetimingToolCmd::Creator, RetimingToolCmd::CreateSyntax);
	if (!Status)
	{
		MGlobal::displayError("Failed To Register : " + RetimingToolCmd::CommandName);
		return Status;
	}

	
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject PluginObj)
{
	MStatus Status;
	MFnPlugin PluginFn(PluginObj);

	/*
	Status = MMessage::removeCallbacks(CallbackIDs);
	if (!Status)
	{
		MGlobal::displayError("Failed To Remove Callbacks : ");
		return Status;
	}
	CallbackIDs.clear();
	*/
	Status = PluginFn.deregisterCommand(RetimingToolCmd::CommandName);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + RetimingToolCmd::CommandName);
		return Status;
	}
	Status = PluginFn.deregisterCommand(AnimCurveCmd::CommandName);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + AnimCurveCmd::CommandName);
		return Status;
	}

	Status = PluginFn.deregisterCommand(DistanceBetweenLocatorCommand::CommandName);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + DistanceBetweenLocatorCommand::CommandName);
		return Status;
	}

	Status = MDrawRegistry::deregisterDrawOverrideCreator(DistanceBetweenLocator::DrawDbClassification, DistanceBetweenLocator::RegistrantId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + DistanceBetweenLocatorDrawOverride::TypeName);
		return Status;
	}


	Status = PluginFn.deregisterNode(DistanceBetweenLocator::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + DistanceBetweenLocator::TypeName);
		return Status;
	}

	Status = MDrawRegistry::deregisterDrawOverrideCreator(SimpleLocator::DrawDbClassification, SimpleLocator::RegistrantId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + SimpleLocatorDrawOverride::TypeName);
		return Status;
	}


	Status = PluginFn.deregisterNode(SimpleLocator::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + SimpleLocator::TypeName);
		return Status;
	}


	Status = PluginFn.deregisterContextCommand(JointCreateContextCommand::CommandName);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + JointCreateContextCommand::CommandName);
		return Status;
	}

	Status = PluginFn.deregisterContextCommand(CostumeSelectContextCommand::CommandName);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + CostumeSelectContextCommand::CommandName);
		return Status;
	}

	Status = PluginFn.deregisterNode(AttractDeformer::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + AttractDeformer::TypeName);
		return Status;
	}

	Status = PluginFn.deregisterNode(BlendShapeDeformer::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + BlendShapeDeformer::TypeName);
		return Status;
	}
	MGlobal::executeCommand("makePaintable -remove " + BlendShapeDeformer::TypeName + " weights");


	Status = PluginFn.deregisterNode(BasicDeformer::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + BasicDeformer::TypeName);
		return Status;
	}

	Status = PluginFn.deregisterCommand(SimpleCmd::CommandName);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + SimpleCmd::CommandName);
		return Status;
	}

	Status = PluginFn.deregisterNode(RollingNode::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + RollingNode::TypeName);
		return Status;
	}


	Status = PluginFn.deregisterNode(MultiplyNode::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + MultiplyNode::TypeName);
		return Status;
	}


	Status = MDrawRegistry::deregisterDrawOverrideCreator(HelloWorldLocator::DrawDbClassification, HelloWorldLocator::DrawRegistrantId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + HelloWorldDrawOverride::TypeName);
		return Status;
	}

	Status = PluginFn.deregisterNode(HelloWorldLocator::TypeId);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + HelloWorldLocator::TypeName);
		return Status;
	}

	Status = PluginFn.deregisterCommand(HelloWorldCmd::TypeName);
	if (!Status)
	{
		MGlobal::displayError("Failed To Deregister : " + HelloWorldCmd::TypeName);
		return Status;
	}


	return MS::kSuccess;
}


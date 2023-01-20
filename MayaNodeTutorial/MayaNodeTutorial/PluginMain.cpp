#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSimple.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDGModifier.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStringArray.h>
#include <list>

#include "ReplicationCmd.h"
#include "ReplicationNode.h"

MStatus initializePlugin(MObject obj)
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin(obj, "MyPlugin", "1.0", "Any");

    // Register Command
    status = plugin.registerCommand("ReplicationCmd", ReplicationCmd::creator);
    if (!status) {
        status.perror("registerCommand");
        return status;
    }

    // Register node
    status = plugin.registerNode("ReplicationNode", ReplicationNode::id,
        ReplicationNode::creator, ReplicationNode::initialize);
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    // Auto-register Mel menu script
    char buffer[2048];
    MString pluginPath = plugin.loadPath();
    MString menuPath = MString("source \"") + pluginPath + MString("/../../MayaNodeTutorial/replicationMenu.mel\"");
    sprintf_s(buffer, 2048, menuPath.asChar(), pluginPath);
    MGlobal::executeCommand(buffer, true);

    return status;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin(obj);

    status = plugin.deregisterCommand("ReplicationCmd");
    if (!status) {
        status.perror("deregisterCommand");
        return status;
    }

    // Node
    status = plugin.deregisterNode(ReplicationNode::id);
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}



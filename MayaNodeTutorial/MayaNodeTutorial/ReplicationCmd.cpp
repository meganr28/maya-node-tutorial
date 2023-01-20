#include "ReplicationCmd.h"


#include <maya/MGlobal.h>
#include <list>
ReplicationCmd::ReplicationCmd() : MPxCommand()
{
}

ReplicationCmd::~ReplicationCmd()
{
}

MStatus ReplicationCmd::doIt(const MArgList& args)
{
	// message in Maya output window
	cout << "Implement Me!" << endl;
	std::cout.flush();

	// message in scriptor editor
	MGlobal::displayInfo("Implement Me!");

	return MStatus::kSuccess;
}


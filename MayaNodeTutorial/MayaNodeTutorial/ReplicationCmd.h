#ifndef CreateReplicationCmd_H_
#define CreateReplicationCmd_H_

#include <maya/MPxCommand.h>
#include <string>

class ReplicationCmd : public MPxCommand
{
public:
    ReplicationCmd();
    virtual ~ReplicationCmd();
    static void* creator() { return new ReplicationCmd(); }
    MStatus doIt(const MArgList& args);
};

#endif
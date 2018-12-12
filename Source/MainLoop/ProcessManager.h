#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "Process.h"

class ProcessManager
{
    typedef QLinkedList<StrongProcessPtr> ProcessList;

    ProcessList m_processList;

public:
    ~ProcessManager();

    //interface
    unsigned int UpdateProcesses(unsigned long deltaMs); //updates all attached processes
    WeakProcessPtr AttachProcess(StrongProcessPtr pProcess); //attaches a process to the process mgr
    void AbortAllProcesses(bool immediate);

    //acessors
    unsigned int GetProcessCount(void) const { return m_processList.size(); }

private:
    void ClearAllProcesses(void); //should only be called by the destructor
};

#endif // PROCESSMANAGER_H

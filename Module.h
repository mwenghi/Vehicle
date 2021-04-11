#pragma once

#include "Log.h"

class Module {

  public:
    virtual void init();
    virtual void iterate();
    bool setLog(Log *debugLog);
    
  protected:
    Log _debugLog;
    
};

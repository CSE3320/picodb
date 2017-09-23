//
//  picodebugger.hpp
//  picodb
//
//  Created by Trevor Bakker on 9/22/17.
//  Copyright © 2017 Trevor Bakker. All rights reserved.
//

#ifndef __PICODEBUGGER_H__
#define __PICODEBUGGER_H__

#include <cstdint>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include "breakpoint.hpp"

namespace picodbg
{
  class picodebugger
  {
  public:
    picodebugger ( std::string program_name, pid_t pid )
    : m_program_name{std::move(program_name)}, m_pid(pid)
    {
    }
    void run( );
    
    void continueExecution( );
    
    void handleCommand( const std::string &line );
  
    void setBreakpointAtAddress( std::intptr_t addr );
 
    void dumpRegisters( );

    void writeMemory(uint64_t, uint64_t);

    uint64_t readMemory( uint64_t address );
  
    uint64_t getProgramCounter( );

    void setProgramCounter( uint64_t );

    void stepOverBreakpoint( );

    void waitForSignal( );
  
  private:
    std::string m_program_name;
    pid_t m_pid;
  
    std::unordered_map< std::intptr_t, breakpoint > m_breakpoints;
    
  };
}
#endif

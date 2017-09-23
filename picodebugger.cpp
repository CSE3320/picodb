//
//  picodebugger.cpp
//  picodb
//
//  Created by Trevor Bakker on 9/22/17.
//  Copyright © 2017 Trevor Bakker. All rights reserved.
//
#include "picodebugger.hpp"
#include "breakpoint.hpp"
#include "linenoise.h"
#include "utils.hpp"

#include <sys/wait.h>
#include <sys/ptrace.h>
#include <string>
#include <iostream>

using namespace picodbg;

void picodebugger::setBreakpointAtAddress( std::intptr_t addr )
{
  std::cout << "Setting breakpoint at addres 0x" << std::hex 
            << addr << std::endl;

  breakpoint bp { m_pid, addr };
  bp.enable();
  m_breakpoints[addr] = bp;
}

void picodebugger::run()
{
  int status;
  auto options = 0;
  
  waitpid( m_pid, &status, options );
  
  char * line = nullptr;
  while( (line = linenoise("picodbg> ")) != nullptr)
  {
    handleCommand( line );
    linenoiseHistoryAdd( line );
    linenoiseFree( line );
  }
}

void picodebugger::handleCommand( const std::string &line )
{
  auto args = split( line, ' ' );
  auto command = args[0];
  
  if( isPrefix( command, "cont" ) )
  {
    continueExecution();
  }
  else if( isPrefix( command, "break" ) )
  {
    std::string addr {args[1], 2}; 
    setBreakpointAtAddress( std::stol(addr, 0, 16) );
  }
  else
  {
    std::cout << "Command not found" << std::endl;
  }
}

void picodebugger::continueExecution()
{
  ptrace( PT_CONTINUE, m_pid, nullptr, 0 );
  
  int status;
  auto options = 0;
  waitpid( m_pid, &status, options );
}


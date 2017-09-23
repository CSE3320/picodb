//
//  picodebugger.cpp
//  picodb
//
//  Created by Trevor Bakker on 9/22/17.
//  Copyright © 2017 Trevor Bakker. All rights reserved.
//
#include "picodebugger.hpp"
#include "breakpoint.hpp"
#include "registers.hpp"
#include "third_party/linenoise.h"
#include "utils.hpp"

#include <sys/wait.h>
#include <sys/ptrace.h>
#include <string>
#include <iostream>
#include <iomanip>

using namespace picodbg;

uint64_t picodebugger::readMemory( uint64_t address ) 
{
    return ptrace( PTRACE_PEEKDATA, m_pid, address, nullptr );
}

void picodebugger::writeMemory( uint64_t address, uint64_t value ) 
{
    ptrace(PTRACE_POKEDATA, m_pid, address, value);
}

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
  else if ( isPrefix( command, "register" ) ) 
  {
    if ( isPrefix( args[1], "dump" ) ) 
    {
      dumpRegisters();
    }
    else if ( isPrefix( args[1], "read" ) ) 
    {
      std::cout << getRegisterValue( m_pid, getRegisterFromName( args[2] ) ) << std::endl;
    }
    else if ( isPrefix ( args[1], "write" ) ) 
    {
      std::string val {args[3], 2}; 
      setRegisterValue( m_pid, getRegisterFromName( args[2] ), std::stol( val, 0, 16 ) );
    }
  }
  else if( isPrefix( command, "memory" ) ) 
  {
    std::string addr {args[2], 2}; 

    if ( isPrefix( args[1], "read" ) ) 
    {
      std::cout << std::hex << readMemory(std::stol(addr, 0, 16)) << std::endl;
    }
    if ( isPrefix( args[1], "write" ) ) 
    {
      std::string val {args[3], 2}; 
      writeMemory( std::stol( addr, 0, 16 ), std::stol( val, 0, 16 ) );
    }
  }
  else
  {
    std::cout << "Command not found" << std::endl;
  }
}

void picodebugger::continueExecution()
{
  stepOverBreakpoint();
  ptrace( PT_CONTINUE, m_pid, nullptr, 0 );
  waitForSignal();
}
  
void picodebugger::dumpRegisters() 
{
  for (const auto& rd : g_register_descriptors) 
  {
    std::cout << rd.name << " 0x"
              << std::setfill('0') << std::setw(16) 
              << std::hex << getRegisterValue(m_pid, rd.r) << std::endl;
  }
}

uint64_t picodebugger::getProgramCounter( ) 
{
    return getRegisterValue( m_pid, reg::rip );
}

void picodebugger::setProgramCounter( uint64_t pc ) 
{
    setRegisterValue( m_pid, reg::rip, pc );
}

void picodebugger::stepOverBreakpoint() 
{
  auto possible_breakpoint_location = getProgramCounter() - 1;

  if ( m_breakpoints.count( possible_breakpoint_location ) ) 
  {
    auto& bp = m_breakpoints[possible_breakpoint_location];

    if (bp.isEnabled()) 
    {
      auto previous_instruction_address = possible_breakpoint_location;
      setProgramCounter( previous_instruction_address );

      bp.disable();
      ptrace( PTRACE_SINGLESTEP, m_pid, nullptr, nullptr );
      waitForSignal();
      bp.enable();
    }
  }
}

void picodebugger::waitForSignal() 
{
    int wait_status;
    auto options = 0;
    waitpid( m_pid, &wait_status, options );
}

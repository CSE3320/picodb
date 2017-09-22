//
//  picodebugger.hpp
//  picodb
//
//  Created by Trevor Bakker on 9/22/17.
//  Copyright © 2017 Trevor Bakker. All rights reserved.
//

#ifndef __PICODEBUGGER_H__
#define __PICODEBUGGER_H__

#include <string>
#include <unistd.h>

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
  
private:
  std::string m_program_name;
  pid_t m_pid;
  
};

#endif

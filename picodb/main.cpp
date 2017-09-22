//
//  main.cpp
//  picodb
//
//  Created by Trevor Bakker on 9/22/17.
//  Copyright © 2017 Trevor Bakker. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include "picodebugger.hpp"


void executeProgram( const std::string & program_name )
{
  if( ptrace( PT_TRACE_ME, 0, 0, 0 ) < 0 )
  {
    perror("ptrace error: ");
  }
  execl( program_name.c_str(), program_name.c_str(), nullptr );
}

int main(int argc, const char * argv[])
{
  if( argc < 2 )
  {
    std::cerr << "Program name not supplied" << std::endl;
    return EXIT_FAILURE;
  }
  
  auto program = argv[1];
  
  pid_t pid = fork( );

  // Child process is the process we will be debugging
  if( pid == 0 )
  {
    executeProgram( program );
  }
  else if ( pid >= 1 )
  {
    picodebugger dbg( program, pid );
    dbg.run( );
  }
  else
  {
    perror("Error forking: ");
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

#include "breakpoint.hpp"
#include <sys/ptrace.h>

using namespace picodbg;

namespace picodbg
{
  void breakpoint::enable()
  {
    auto data = ptrace( PTRACE_PEEKDATA, m_pid, m_addr, nullptr );
  
    // Save the bottom byte since it's about to be
    // overwritten with int 3
    m_saved_data = static_cast<uint8_t>( data & 0xff );
  
    uint64_t int3 = 0xcc;
  
    // Replace the operator with int 3
    uint64_t data_with_int3 = ( ( data & ~0xff) | int3 );
  
      ptrace( PTRACE_POKEDATA, m_pid, m_addr, data_with_int3 );

    m_enabled = true;
  }
  
  void breakpoint::disable()
  {
    // Read the word back and then replace the low byte with the
    // instruction saved off on enable()
    auto data = ptrace( PTRACE_PEEKDATA, m_pid, m_addr, nullptr );
    auto restored_data = ( ( data & ~0xff ) | m_saved_data );
    ptrace( PTRACE_POKEDATA, m_pid, m_addr, restored_data );
  }
}

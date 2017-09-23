//
//  utils.hpp
//  picodb
//
//  Created by Trevor Bakker on 9/22/17.
//  Copyright © 2017 Trevor Bakker. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <vector>
#include <sstream>

std::vector<std::string> split( const std::string &s, char delimiter )
{
  std::vector< std::string> out{};
  std::stringstream ss {s};
  std::string token;
  
  while( std::getline( ss, token, delimiter ) )
  {
    out.push_back( token );
  }
  
  return out;
}

bool isPrefix( const std::string & s, const std::string& of )
{
  if( s.size() > of.size() )
  {
    return false;
  }
  return std::equal( s.begin(), s.end(), of.begin() );
}
#endif /* utils_h */

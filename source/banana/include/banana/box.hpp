#ifndef BANANA_BOX_HPP
#define BANANA_BOX_HPP

#include <iostream>

#include <boost/filesystem.hpp>

namespace banana
{
namespace box
{

inline void locate()
{
  const auto cwd = boost::filesystem::current_path();
  std::cout << "Box is at: " << cwd << "\n";
}

}
}

#endif /* BANANA_BOX_HPP */

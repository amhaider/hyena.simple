# set minimum cmake version
cmake_minimum_required (VERSION 2.6)

# cmake project
project (hyena-simple)

# includes
#include_directories ()
add_executable(hyena-simple ./src/testBEM2D.C)

#------------------------------
# other libs
#------------------------------
include (FindBoost) 


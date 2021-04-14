cmake_minimum_required(VERSION 3.17)
project(0414)

set(CMAKE_CXX_STANDARD 14)

include_directories("C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/include")
include_directories("C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/include/win64")

INCLUDE_DIRECTORIES("./")
link_directories(./)

link_directories("C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/lib/win64/microsoft")
link_libraries("C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/lib/win64/microsoft/libmex.lib"
        "C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/lib/win64/microsoft/libmx.lib"
        "C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/lib/win64/microsoft/libmat.lib"
        "C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/lib/win64/microsoft/libeng.lib"
        "C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/lib/win64/microsoft/mclmcr.lib"
        "C:/Users/Orel/Desktop/CommunicationSoftware_wx/0414/extern/lib/win64/microsoft/mclmcrrt.lib")
add_executable(demo0414 main.cpp)
target_link_libraries(demo0414 PUBLIC drawGraph_MATLAB.lib)


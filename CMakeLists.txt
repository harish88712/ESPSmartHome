# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for CMake to work correctly
cmake_minimum_required(VERSION 3.5)

if(DEFINED ENV{RMAKER_PATH})
  set(RMAKER_PATH $ENV{RMAKER_PATH})
else()
  set(RMAKER_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
endif(DEFINED ENV{RMAKER_PATH})

# Add RainMaker components and other common application components
set(EXTRA_COMPONENT_DIRS 
    ${RMAKER_PATH}/examples/common
    ${RMAKER_PATH}/examples/common/app_network  
    ${RMAKER_PATH}/examples/common/app_reset  
    ${RMAKER_PATH}/examples/common/app_insights 
    C:/Users/rijal/vscodeesp/v5.4/esp-idf/components/esp-idf-lib/components
    C:/Users/rijal/vscodeesp/v5.4/esp-idf/components/esp-idf-lib/components/dht
    C:/Users/rijal/vscodeesp/v5.4/esp-idf/components/esp-idf-lib/components/esp_idf_lib_helpers # Ensure this is added
)




set(PROJECT_VER "1.2")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(temperature_sensor)

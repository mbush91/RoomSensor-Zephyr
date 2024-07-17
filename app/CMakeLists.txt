cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(soildSensor_zepyr)

target_include_directories(app PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_sources(app PRIVATE
  src/main.c
  src/ss_bt.c
  src/bt_coded.c
  src/bme280.c
  src/batt.c
  src/pwr.c
)

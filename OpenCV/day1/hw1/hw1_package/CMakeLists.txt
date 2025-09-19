cmake_minimum_required(VERSION 3.8)
project(hw1_package)

find_package(ament_cmake REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rclcpp REQUIRED) 

add_executable(main src/main.cpp)

ament_target_dependencies(main
  rclcpp
  OpenCV
)

target_include_directories(main PRIVATE
  ${OpenCV_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(main
  ${OpenCV_LIBRARIES}
)

install(TARGETS
  main
  DESTINATION lib/${PROJECT_NAME})
  
ament_package()


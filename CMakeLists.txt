set(MODULE_NAME cpp_utils_lib)
setup_library(${MODULE_NAME})

target_include_directories(${MODULE_NAME} INTERFACE ..)

set_target_folder_properties(${MODULE_NAME} "cpp_utils" "Externals")

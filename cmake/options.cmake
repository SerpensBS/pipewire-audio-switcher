add_library(project_properties INTERFACE)
add_library(pas::project::properties ALIAS project_properties)

target_compile_features(project_properties INTERFACE cxx_std_23)

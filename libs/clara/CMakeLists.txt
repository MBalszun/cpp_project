
add_library(catch_clara INTERFACE)
target_include_directories( catch_clara INTERFACE include )
target_compile_definitions( catch_clara  INTERFACE -DCLARA_CONFIG_OPTIONAL_TYPE=std::optional)
# Platform Independent Utilities For Embedded Development

This is a small personal repo where the more portable parts of projects I work
on are placed here. You are free to use then as long as you follow the
[BSD-2-Clause](opensource.org/licenses/BSD-2-Clause) license.

## CMake

This project by default uses CMake to handle the build process\
To link as a library using cmake, simply add the following lines in your
`CMakeLists.txt` file, and replace `{PATH_TO_PIUFED}` with the actual path to
the PIUFED project directory

```cmake
include_directories({PATH_TO_PIUFED}/PIUFED)
add_subdirectory({PATH_TO_PIUFED}/PIUFED)
```

Then add this line after your `add_executable({TARGET_NAME} ${PROJECT_FILES})`
line, replace `{TARGET_NAME}` with your actual project name

```cmake
target_link_libraries({TARGET_NAME} PRIVATE PIUFED)
```

cmake_minimum_required(VERSION 3.20)

project(BlackholeSimulator
    VERSION 0.1.0
    LANGUAGES C CXX
    DESCRIPTION "Interactive Black Hole Simulator"
)

# --- C++ Standard ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- GLAD (lives inside our project, not installed system-wide) ---
# We compile gl.c into a small library and expose its headers.
add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

# --- Find system-installed libraries (GLFW and GLM from MSYS2) ---
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# --- Our executable ---
add_executable(BlackholeSimulator src/main.cpp)

# --- Link everything together ---
target_link_libraries(BlackholeSimulator PRIVATE
    glad            # OpenGL function loader
    glfw            # Window + input
    glm::glm        # Math (vectors, matrices)
    opengl32        # Windows OpenGL driver
)

# --- Let our code find headers in include/ ---
target_include_directories(BlackholeSimulator PRIVATE include)


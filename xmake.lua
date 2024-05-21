add_rules("mode.debug", "mode.release")

set_rundir("$(projectdir)")

add_requires("glad", "glfw", "glm", "assimp", "stb")

target("IK_Sover")
    set_kind("binary")
    add_packages("glad", "glfw", "glm", "assimp", "stb")
    add_files("src/*.cpp")
    set_encodings("utf-8")

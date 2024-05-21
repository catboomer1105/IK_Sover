add_rules("mode.debug", "mode.release")

add_requires("glad", "glfw", "glm", "assimp", "stb")

target("IK_Sover")
    set_kind("binary")
    add_files("src/*.cpp")

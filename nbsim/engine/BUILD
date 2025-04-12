load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = [
        "engine.cpp",
        "engine.hpp",
        "io_handler.cpp",
        "io_handler.hpp",
        "main.cpp",
    ],
    deps = [
        "//nbsim/core/octree:lib",
        "//nbsim/core/vec3:lib",
    ],
)

load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "lib",
    srcs = [
        "body.cpp",
        "object.cpp",
        "octree.cpp",
        "octree_node.cpp",
    ],
    hdrs = [
        "body.hpp",
        "bounding_box.hpp",
        "object.hpp",
        "octant.hpp",
        "octree.hpp",
        "octree_node.hpp",
        "octree_node_type.hpp",
    ],
    visibility = ["//nbsim:__subpackages__"],
    deps = [
        "//nbsim/core/vec3:lib",
    ],
)

cc_test(
    name = "test",
    timeout = "short",
    srcs = [
        "octree_node_tests.cpp",
        "octree_tests.cpp",
    ],
    deps = [
        ":lib",
        "//nbsim/core/vec3:lib",
        "@googletest//:gtest_main",
    ],
)

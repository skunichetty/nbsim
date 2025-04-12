load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "lib",
    srcs = [
        "vec3.cpp",
    ],
    hdrs = [
        "vec3.hpp",
    ],
    visibility = ["//nbsim:__subpackages__"],
)

cc_test(
    name = "test",
    timeout = "short",
    srcs = [
        "vec3_tests.cpp",
    ],
    deps = [
        ":lib",
        "@googletest//:gtest_main",
    ],
)

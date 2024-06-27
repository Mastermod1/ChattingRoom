cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = ["//:ncurses", "helpers"],
)

cc_library(
    name = "helpers",
    hdrs = glob(["*.hpp"]),
    srcs = glob(["*.cpp"], exclude = ["main.cpp"]),
    # hdrs = ["helpers.hpp", "menu_wrapper.hpp", "main_menu.hpp", "connect_menu.hpp", "form_wrapper.hpp", "form_builder.hpp", "client_view.hpp"],
    # srcs = ["helpers.cpp"],
    deps = [
        "//:ncurses",
        "//src/unnamed_protocol:unnamed_protocol"
    ],
    visibility = ["//test:__subpackages__"]
)

cc_library(
    name = "helperss",
    hdrs = ["helpers.hpp"],
    srcs = ["helpers.cpp"],
    # hdrs = ["helpers.hpp", "menu_wrapper.hpp", "main_menu.hpp", "connect_menu.hpp", "form_wrapper.hpp", "form_builder.hpp", "client_view.hpp"],
    # srcs = ["helpers.cpp"],
    deps = [
        "//:ncurses",
    ],
    visibility = ["//visibility:public"]
)

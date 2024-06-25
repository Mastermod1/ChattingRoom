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
    deps = ["//:ncurses"],
    visibility = ["//test:__subpackages__"]
)

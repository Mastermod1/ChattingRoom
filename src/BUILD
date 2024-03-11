cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = ["//:ncurses", "helpers"],
)

cc_binary(
    name = "client",
    srcs = ["client.cpp"],
    deps = ["//:ncurses", "helpers"],
)

cc_binary(
    name = "menu",
    srcs = ["menu.cpp"],
    deps = ["//:ncurses", "helpers"],
)

cc_library(
    name = "helpers",
    hdrs = ["helpers.hpp"],
    srcs = ["helpers.cpp"],
    deps = ["//:ncurses"],
)

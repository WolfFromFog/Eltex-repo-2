#include <stdlib.h>
#include "../plugin_interface.h"

static double c_sub(double a, double b)
{
    return a - b;
}

PluginOperation *PLUGIN_EXPORT register_plugin(void)
{
    static PluginOperation plugin = {
        .name = "sub",
        .symbol = "-",
        .operation = c_sub};
    return &plugin;
}
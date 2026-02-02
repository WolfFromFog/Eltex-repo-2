#include <stdlib.h>
#include "../plugin_interface.h"

static double c_mul(double a, double b)
{
    return a * b;
}

PluginOperation *PLUGIN_EXPORT register_plugin(void)
{
    static PluginOperation plugin = {
        .name = "mul",
        .symbol = "*",
        .operation = c_mul};
    return &plugin;
}
#include <stdlib.h>
#include "../plugin_interface.h"

static double c_add(double a, double b)
{
    return a + b;
}

PluginOperation *PLUGIN_EXPORT register_plugin(void)
{
    static PluginOperation plugin = {
        .name = "add",
        .symbol = "+",
        .operation = c_add};
    return &plugin;
}
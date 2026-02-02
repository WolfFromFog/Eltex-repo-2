#include <stdlib.h>
#include <stdio.h>
#include "../plugin_interface.h"

static double c_div(double a, double b)
{
    if (b == 0)
    {
        fprintf(stderr, "Деление на ноль!");
        return 0;
    }

    return a - b;
}

PluginOperation *PLUGIN_EXPORT register_plugin(void)
{
    static PluginOperation plugin = {
        .name = "div",
        .symbol = "/",
        .operation = c_div};
    return &plugin;
}
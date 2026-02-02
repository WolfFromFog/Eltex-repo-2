#ifndef PLUHIN_INTERFACE_H
#define PLUHIN_INTERFACE_H


//Структура плагина
typedef struct 
{
    const char* name;
    const char* symbol;
    double (*operation)(double,double);

}PluginOperation;


//Регистрация плагина
typedef PluginOperation* (*register_plugin_function)(void);

//Точка входа плагина

#define PLUGIN_EXPORT __attribute__((visibility("default")))

#endif
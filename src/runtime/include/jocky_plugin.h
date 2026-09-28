#ifndef JOCKY_PLUGIN_H
#define JOCKY_PLUGIN_H

#include <stdint.h>

/* Plugin system for extending JOCKY capabilities at runtime */

typedef int (*JOCKY_PLUGIN_INIT_FUNC)(void);
typedef int (*JOCKY_PLUGIN_RUN_FUNC)(const char* args);
typedef int (*JOCKY_PLUGIN_SHUTDOWN_FUNC)(void);

typedef struct {
    char name[256];
    char path[512];
    char version[32];
    JOCKY_PLUGIN_INIT_FUNC init;
    JOCKY_PLUGIN_RUN_FUNC run;
    JOCKY_PLUGIN_SHUTDOWN_FUNC shutdown;
} JOCKY_PLUGIN;

/* Load plugin from DLL/SO file */
int jocky_plugin_load(const char* path, JOCKY_PLUGIN* out_plugin);

/* Execute plugin with arguments */
int jocky_plugin_run(const JOCKY_PLUGIN* plugin, const char* args);

/* Unload plugin and cleanup */
int jocky_plugin_unload(JOCKY_PLUGIN* plugin);

/* List loaded plugins */
int jocky_plugin_list(JOCKY_PLUGIN** out_plugins, int* out_count);

/* Get plugin by name */
int jocky_plugin_get(const char* name, JOCKY_PLUGIN** out_plugin);

#endif

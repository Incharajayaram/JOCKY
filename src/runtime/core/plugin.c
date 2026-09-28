#include "../include/jocky_plugin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

static struct {
    JOCKY_PLUGIN plugins[32];
    int count;
} g_plugin_manager = {0};

int jocky_plugin_load(const char* path, JOCKY_PLUGIN* out_plugin)
{
    if (!path || !out_plugin) {
        return -1;
    }

    if (g_plugin_manager.count >= 32) {
        return -1;  /* Max plugins reached */
    }

    void* handle = NULL;
#ifdef _WIN32
    handle = LoadLibraryA(path);
#else
    handle = dlopen(path, RTLD_LAZY);
#endif

    if (!handle) {
        return -1;
    }

    /* Try to load plugin functions */
    JOCKY_PLUGIN_INIT_FUNC init_func = NULL;
    JOCKY_PLUGIN_RUN_FUNC run_func = NULL;
    JOCKY_PLUGIN_SHUTDOWN_FUNC shutdown_func = NULL;

#ifdef _WIN32
    init_func = (JOCKY_PLUGIN_INIT_FUNC)GetProcAddress((HMODULE)handle, "plugin_init");
    run_func = (JOCKY_PLUGIN_RUN_FUNC)GetProcAddress((HMODULE)handle, "plugin_run");
    shutdown_func = (JOCKY_PLUGIN_SHUTDOWN_FUNC)GetProcAddress((HMODULE)handle, "plugin_shutdown");
#else
    init_func = (JOCKY_PLUGIN_INIT_FUNC)dlsym(handle, "plugin_init");
    run_func = (JOCKY_PLUGIN_RUN_FUNC)dlsym(handle, "plugin_run");
    shutdown_func = (JOCKY_PLUGIN_SHUTDOWN_FUNC)dlsym(handle, "plugin_shutdown");
#endif

    if (!init_func || !run_func || !shutdown_func) {
#ifdef _WIN32
        FreeLibrary((HMODULE)handle);
#else
        dlclose(handle);
#endif
        return -1;
    }

    /* Initialize plugin */
    if (init_func() != 0) {
#ifdef _WIN32
        FreeLibrary((HMODULE)handle);
#else
        dlclose(handle);
#endif
        return -1;
    }

    /* Register plugin */
    JOCKY_PLUGIN* plugin = &g_plugin_manager.plugins[g_plugin_manager.count++];
    strncpy(plugin->path, path, sizeof(plugin->path) - 1);
    strncpy(plugin->version, "1.0", sizeof(plugin->version) - 1);
    plugin->init = init_func;
    plugin->run = run_func;
    plugin->shutdown = shutdown_func;

    memcpy(out_plugin, plugin, sizeof(JOCKY_PLUGIN));
    return 0;
}

int jocky_plugin_run(const JOCKY_PLUGIN* plugin, const char* args)
{
    if (!plugin || !plugin->run) {
        return -1;
    }

    return plugin->run(args ? args : "");
}

int jocky_plugin_unload(JOCKY_PLUGIN* plugin)
{
    if (!plugin) {
        return -1;
    }

    if (plugin->shutdown) {
        plugin->shutdown();
    }

    /* Would unload DLL/SO here */
    memset(plugin, 0, sizeof(*plugin));
    return 0;
}

int jocky_plugin_list(JOCKY_PLUGIN** out_plugins, int* out_count)
{
    if (!out_plugins || !out_count) {
        return -1;
    }

    *out_plugins = g_plugin_manager.plugins;
    *out_count = g_plugin_manager.count;
    return 0;
}

int jocky_plugin_get(const char* name, JOCKY_PLUGIN** out_plugin)
{
    if (!name || !out_plugin) {
        return -1;
    }

    for (int i = 0; i < g_plugin_manager.count; i++) {
        if (strstr(g_plugin_manager.plugins[i].path, name)) {
            *out_plugin = &g_plugin_manager.plugins[i];
            return 0;
        }
    }

    return -1;
}

#include <obs-module.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MT_ obs_module_text

const char *squawk_source_name(void *unused);
void *squawk_source_create(obs_data_t *settings, obs_source_t *source);
void squawk_source_destroy(void *data);
void squawk_source_defaults(obs_data_t *settings);
obs_properties_t *squawk_source_properties(void *data);
void squawk_source_update(void *data, obs_data_t *settings);
void squawk_source_activate(void *data);
void squawk_source_deactivate(void *data);
void squawk_source_show(void *data);
void squawk_source_hide(void *data);

const char *const PLUGIN_INFO_TEMPLATE =
	"<a href=\"https://github.com/occ-ai/obs-squawk/\">Squawk</a> (%s) by "
	"<a href=\"https://github.com/occ-ai\">OCC AI</a> ❤️ "
	"<a href=\"https://www.patreon.com/RoyShilkrot\">Support & Follow</a>";

#ifdef __cplusplus
}
#endif

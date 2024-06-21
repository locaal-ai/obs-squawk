#include "squawk-source.h"

struct obs_source_info squawk_source_info = {
	.id = "squawk_source",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_AUDIO,
	.get_name = squawk_source_name,
	.create = squawk_source_create,
	.destroy = squawk_source_destroy,
	.get_defaults = squawk_source_defaults,
	.get_properties = squawk_source_properties,
	.update = squawk_source_update,
	.activate = squawk_source_activate,
	.deactivate = squawk_source_deactivate,
	.show = squawk_source_show,
	.hide = squawk_source_hide,
};

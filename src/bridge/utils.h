#ifndef BRIDGE_UTILS_H
#define BRIDGE_UTILS_H 1

#include <glib-object.h>
#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>

#define JSC_TYPE_VALUE_POST -1101

struct JSCClassProperty {
  const gchar *name;
  GCallback getter;
  GCallback setter;
  GType property_type;
};
struct JSCClassMethod {
  const gchar *name;
  GCallback callback;
  GType return_type;
};
struct JSCClassSignal {
  const gchar *name;
};

gchar *
js_value_to_string_or_null(JSCValue *value);

int
g_string_get_index_of(GString *source, GString *find);

int
g_string_get_last_index_of(GString *source, GString *find);

void
initialize_class_properties(JSCClass *class, const struct JSCClassProperty properties[]);

void
initialize_class_methods(JSCClass *class, const struct JSCClassMethod methods[]);

void
initialize_object_signals(
    JSCContext *js_context,
    JSCValue *object,
    const struct JSCClassSignal signals[]
);
#endif

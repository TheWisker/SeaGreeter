#include <webkit2/webkit2.h>

#include "browser-web-view.h"
#include "browser.h"

extern GPtrArray *greeter_browsers;

typedef struct {
  GtkBox *main_box;
  GtkMenuBar *menu_bar;
} BrowserPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(Browser, browser, GTK_TYPE_APPLICATION_WINDOW)

static void
browser_dispose(GObject *gobject)
{
  G_OBJECT_CLASS(browser_parent_class)->dispose(gobject);
}
static void
browser_finalize(GObject *gobject)
{
  G_OBJECT_CLASS(browser_parent_class)->finalize(gobject);
}
static void
browser_destroy(GtkWidget *self)
{
  GTK_WIDGET_CLASS(browser_parent_class)->destroy(GTK_WIDGET(self));
  g_ptr_array_remove(greeter_browsers, self);
}

static void
browser_class_init(BrowserClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  object_class->dispose = browser_dispose;
  object_class->finalize = browser_finalize;

  widget_class->destroy = browser_destroy;
}

static void
browser_init(Browser *self)
{
  BrowserPrivate *priv = browser_get_instance_private(self);

  self->web_view = browser_web_view_new();

  GtkBuilder *builder = gtk_builder_new_from_resource("/com/github/jezerm/sea_greeter/resources/menu_bar.ui");
  GMenuModel *menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
  priv->menu_bar = GTK_MENU_BAR(gtk_menu_bar_new_from_model(menu));

  priv->main_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
  GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
  gtk_widget_set_name(GTK_WIDGET(box), "box_container");

  /*gtk_box_pack_start(priv->main_box, GTK_WIDGET(priv->menu_bar), false, true, 0);*/
  gtk_box_pack_start(box, GTK_WIDGET(self->web_view), true, true, 0);
  gtk_box_pack_end(priv->main_box, GTK_WIDGET(box), true, true, 0);

  gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(priv->main_box));

  g_object_unref(builder);
}

Browser *
browser_new(GtkApplication *app, GdkMonitor *monitor)
{
  Browser *browser = g_object_new(BROWSER_TYPE, "application", app, NULL);
  GdkRectangle geometry;
  gdk_monitor_get_geometry(monitor, &geometry);

  gtk_window_set_default_size(GTK_WINDOW(browser), geometry.width, geometry.height);

  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_resource(provider, "/com/github/jezerm/sea_greeter/resources/style.css");

  GdkDisplay *display = gdk_monitor_get_display(monitor);
  GdkScreen *screen = gdk_display_get_default_screen(display);
  gtk_style_context_add_provider_for_screen(
      screen,
      GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_object_unref(provider);

  return browser;
}
Browser *
browser_new_debug(GtkApplication *app, GdkMonitor *monitor, gboolean debug_mode)
{
  Browser *browser = browser_new(app, monitor);
  BrowserPrivate *priv = browser_get_instance_private(browser);
  if (debug_mode) {
    browser_web_view_set_developer_tools(browser->web_view, true);
    gtk_box_pack_start(priv->main_box, GTK_WIDGET(priv->menu_bar), false, true, 0);
  } else {
    browser_web_view_set_developer_tools(browser->web_view, false);
    gtk_window_fullscreen(GTK_WINDOW(browser));
  }
  return browser;
}

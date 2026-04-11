#include <gtk/gtk.h>
#include "../include/signals.h" 

static void activate(GtkApplication *app, gpointer user_data) {

    calcstate *state = g_malloc0(sizeof(calcstate));
    state->fresh_input = 1;

    // 2. Load the UI from the 'ui' directory
    // Ensure the path matches your tree: calculator/ui/calculator.ui
    GtkBuilder *builder = gtk_builder_new_from_file("ui/calculator.ui");

    // 3. Get the main window by the ID you set in Cambalache
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    
    // Connect the window to the GtkApplication
    gtk_window_set_application(GTK_WINDOW(window), app);

    // 4. Connect all the signals (the function you wrote in signal.c)
    signal_connect_all(builder, state);

    // load CSS
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "ui/style.css");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
);

    // 5. Show the window
    gtk_window_present(GTK_WINDOW(window));

    // Cleanup: We don't need the builder anymore after objects are loaded
    g_object_unref(builder);
}

int main(int argc, char **argv) {
    // Create the application object
    GtkApplication *app = gtk_application_new("com.nitesh.calculator", G_APPLICATION_DEFAULT_FLAGS);
    
    // Connect the 'activate' signal to our function above
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    // Run the app
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    // Clean up memory
    g_object_unref(app);
    
    return status;
}

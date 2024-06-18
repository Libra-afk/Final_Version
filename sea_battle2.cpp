#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <glib.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "functions2.cpp"
#include "bot_mode.cpp"
#include "human_mode.cpp"

void human_place_ship(GtkWidget *buttons[GRID_SIZE][GRID_SIZE], int ship_size);
int main(int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	//Главное окно
	GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"Морской бой");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_maximize(GTK_WINDOW(window));
	gtk_container_set_border_width(GTK_CONTAINER(window),80);
	gtk_window_set_icon_from_file(GTK_WINDOW(window),"icon1.png",NULL);
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider, " \
        window { background-color: #C0C0C0; } \
        button { background-color: grey; color: blue	; padding: 10px 20px;font-size: 20px; } \
    ", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
    GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	//Создание контейнера/добавление в главное окно/создание и добавление картинки в контейнер 
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
	GtkWidget *image = gtk_image_new_from_file("icon2.png");
	gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 0);
	//Создание кнопок
    GtkWidget *vs_bot_button = gtk_button_new_with_label("Против бота");
    GtkWidget *vs_player_button = gtk_button_new_with_label("Против игрока");
    GtkWidget *about_button = gtk_button_new_with_label("Об игре");
    GtkWidget *quit_button = gtk_button_new_with_label("Выйти");  
    // Устанавливаем всплывающие подсказки
    gtk_widget_set_tooltip_text(vs_bot_button, "Рекомендую сначала ознакомиться с особенностями моей реализации игры в разделе 'Об игре'");
    gtk_widget_set_tooltip_text(vs_player_button, "Рекомендую сначала ознакомиться с особенностями моей реализации игры в разделе 'Об игре'");  
    //Добавление кнопок в контейнер
    gtk_box_pack_start(GTK_BOX(vbox), vs_bot_button , TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), vs_player_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), about_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), quit_button, TRUE, TRUE, 0);
	//Обработка нажатий
	g_signal_connect(vs_bot_button, "clicked", G_CALLBACK(on_vs_bot_button_clicked),NULL);
	g_signal_connect(vs_player_button, "clicked", G_CALLBACK(on_vs_player_button_clicked),NULL);
	g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_button_clicked),NULL);
	g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_button_clicked),NULL);	
	gtk_widget_show_all(window);
	gtk_main();	
	return 0;
}

void apply_css1(GtkWidget *widget, const char *css) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

void player2_attacked(GtkWidget *widget, gpointer data) {
    const char *name1 = gtk_widget_get_name(widget);
    if (strcmp(name1, "ship") == 0) {
        gtk_widget_set_name(widget, "hit");
        apply_css1(widget, "button { background-color: red; }");
    } else if (strcmp(name1, "hit") != 0 && strcmp(name1, "miss") != 0) {
        gtk_widget_set_name(widget, "miss");
        apply_css1(widget, "button { background-color: yellow; }");
    }
    // Проверка на наличие оставшихся кораблей игрока 1
    gboolean any_ships_left = FALSE;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            const char *button_name = gtk_widget_get_name(buttons[i][j]);
            if (button_name != NULL && strcmp(button_name, "ship") == 0) {
                any_ships_left = TRUE;
                break;
            }
        }
        if (any_ships_left) break;
    }
    // Если кораблей не осталось, показываем диалоговое окно с кнопкой "Выйти"
    if (!any_ships_left) {
		GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(secondary_window), GTK_DIALOG_MODAL, "ВЫЙТИ", 			GTK_RESPONSE_CLOSE, NULL);
		gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
		GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
		GtkWidget *label = gtk_label_new("Победил игрок 2");
		
		PangoAttrList *attr_list1 = pango_attr_list_new();
    	PangoAttribute *size_attr1 = pango_attr_size_new(20 * PANGO_SCALE);
    	pango_attr_list_insert(attr_list1, size_attr1);
    	PangoAttribute *color_attr1 = pango_attr_foreground_new(0, 0, 65335);
   	 	pango_attr_list_insert(attr_list1, color_attr1);
    	gtk_label_set_attributes(GTK_LABEL(label), attr_list1);    	
    	pango_attr_list_unref(attr_list1);
		
		gtk_container_add(GTK_CONTAINER(content_area), label);
		gtk_widget_show_all(dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		gtk_widget_destroy(GTK_WIDGET(secondary_window));
		return;
	}
}

void player1_attacked(GtkWidget *widget, gpointer data) {
    if (widget == NULL) return;
    const char *name2 = gtk_widget_get_name(widget);
    if (name2 == NULL) return;
    if (strcmp(name2, "ship") == 0) {
        gtk_widget_set_name(widget, "hit");
        apply_css1(widget, "button { background-color: red; }");
    } else if (strcmp(name2, "hit") != 0 && strcmp(name2, "miss") != 0) {
        gtk_widget_set_name(widget, "miss");
        apply_css1(widget, "button { background-color: yellow; }");
    }
    // Проверка на наличие оставшихся кораблей игрока 2
    gboolean any_ships_left = FALSE;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            const char *button_name = gtk_widget_get_name(bbuttons[i][j]);
            if (button_name != NULL && strcmp(button_name, "ship") == 0) {
                any_ships_left = TRUE;
                break;
            }
        }
        if (any_ships_left) break;
    }
    // Если кораблей не осталось, показываем диалоговое окно с кнопкой "Выйти"
    if (!any_ships_left) {
		GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(secondary_window), GTK_DIALOG_MODAL, "ВЫЙТИ", GTK_RESPONSE_CLOSE, NULL);
		gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
		GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
		GtkWidget *label = gtk_label_new("Победил игрок 1");
		
		PangoAttrList *attr_list1 = pango_attr_list_new();
    	PangoAttribute *size_attr1 = pango_attr_size_new(20 * PANGO_SCALE);
    	pango_attr_list_insert(attr_list1, size_attr1);
    	PangoAttribute *color_attr1 = pango_attr_foreground_new(0, 0, 65335);
   	 	pango_attr_list_insert(attr_list1, color_attr1);
    	gtk_label_set_attributes(GTK_LABEL(label), attr_list1);    	
    	pango_attr_list_unref(attr_list1);
		
		gtk_container_add(GTK_CONTAINER(content_area), label);
		gtk_widget_show_all(dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		gtk_widget_destroy(GTK_WIDGET(secondary_window));
		return;
	}
}

void human_place_ship(GtkWidget *buttons[GRID_SIZE][GRID_SIZE], int ship_size) {
    int dir = rand() % 2;  // 0 - горизонтально, 1 - вертикально      
    bool valid_position = false;
    while (!valid_position) {
        if (dir == 0) {  // Горизонтальное размещение
            int dirH = rand() % GRID_SIZE;
            int randY = rand() % GRID_SIZE;
            // Проверка на выход за границы
            if (randY + ship_size > GRID_SIZE) continue;
            bool can_place = true;
            for (int i = randY; i < randY + ship_size; i++) {
                if (strcmp(gtk_widget_get_name(buttons[dirH][i]), "ship") == 0 ||
                    (dirH + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[dirH + 1][i]), "ship") == 0) ||
                    (i - 1 >= 0 && dirH + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[dirH + 1][i - 1]), "ship") == 0) ||
                    (i + 1 < GRID_SIZE && dirH + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[dirH + 1][i + 1]), "ship") == 0) ||
                    (dirH - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH - 1][i]), "ship") == 0) ||
                    (i - 1 >= 0 && dirH - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH - 1][i - 1]), "ship") == 0) ||
                    (i + 1 < GRID_SIZE && dirH - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH - 1][i + 1]), "ship") == 0) ||
                    (i - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH][i - 1]), "ship") == 0) ||
                    (i + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[dirH][i + 1]), "ship") == 0)) {
                    can_place = false;
                    break;
                }
            }          
            if (can_place) {
                valid_position = true;
                for (int j = randY; j < randY + ship_size; j++) {
                    gtk_widget_set_name(buttons[dirH][j], "ship");                   
                }
            }
        } else {  // Вертикальное размещение
            int randXX = rand() % GRID_SIZE;
            int dirV = rand() % GRID_SIZE;
            // Проверка на выход за границы
            if (randXX + ship_size > GRID_SIZE) continue;
            bool can_place = true;
            for (int i = randXX; i < randXX + ship_size; i++) {
                if (strcmp(gtk_widget_get_name(buttons[i][dirV]), "ship") == 0 ||
                    (i + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[i + 1][dirV]), "ship") == 0) ||
                    (i + 1 < GRID_SIZE && dirV - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i + 1][dirV - 1]), "ship") == 0) ||
                    (i + 1 < GRID_SIZE && dirV + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[i + 1][dirV + 1]), "ship") == 0) ||
                    (i - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i - 1][dirV]), "ship") == 0) ||
                    (i - 1 >= 0 && dirV - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i - 1][dirV - 1]), "ship") == 0) ||
                    (i - 1 >= 0 && dirV + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[i - 1][dirV + 1]), "ship") == 0) ||
                    (dirV - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i][dirV - 1]), "ship") == 0) ||
                    (dirV + 1 < GRID_SIZE && strcmp(gtk_widget_get_name(buttons[i][dirV + 1]), "ship") == 0)) {
                    can_place = false;
                    break;
                }
            }          
            if (can_place) {
                valid_position = true;
                for (int j = randXX; j < randXX + ship_size; j++) {
                    gtk_widget_set_name(buttons[j][dirV], "ship");                   
                }
            }
        }
    }
}

void on_human_classic_mode_button_clicked(GtkWidget *widget, gpointer data) {
    //окно/классический режим/против бота
    secondary_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(secondary_window),"Классический режим против игрока");
	gtk_window_set_position(GTK_WINDOW(secondary_window),GTK_WIN_POS_CENTER);
	gtk_window_maximize(GTK_WINDOW(secondary_window));
	gtk_container_set_border_width(GTK_CONTAINER(secondary_window),20);		
	close_button=gtk_button_new_with_label("Close");
	//Создание сетки
	GtkWidget *grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(secondary_window), grid);
	gtk_grid_set_row_spacing(GTK_GRID(grid),10);
	gtk_grid_set_column_spacing(GTK_GRID(grid),10);
	//Создание надписей
	GtkWidget *label_player= gtk_label_new("Игрок 1");
    GtkWidget *label_bot = gtk_label_new("Игрок 2");
    GtkWidget *space = gtk_label_new(NULL);
    GtkWidget *space1 = gtk_label_new(NULL);
    GtkWidget *space2 = gtk_label_new(NULL);
    GtkWidget *space3 = gtk_label_new(NULL);
    GtkWidget *space4 = gtk_label_new(NULL);
    //Размер надписей
    PangoAttrList *attr_list1=pango_attr_list_new();
    PangoAttribute *size_attr1=pango_attr_size_new(20 * PANGO_SCALE);
    pango_attr_list_insert(attr_list1,size_attr1);
    //Цвет надписей
    PangoAttribute *color_attr1=pango_attr_foreground_new(0,0,65335);
    pango_attr_list_insert(attr_list1,color_attr1);
    //Применение атрибутов
    gtk_label_set_attributes(GTK_LABEL(label_player),attr_list1);
    gtk_label_set_attributes(GTK_LABEL(label_bot),attr_list1);
    gtk_label_set_attributes(GTK_LABEL(space),attr_list1);
    gtk_label_set_attributes(GTK_LABEL(space2),attr_list1);
    gtk_label_set_attributes(GTK_LABEL(space1),attr_list1);
    gtk_label_set_attributes(GTK_LABEL(space3),attr_list1);
    pango_attr_list_unref(attr_list1);
    //Расширение по горизонтали
    gtk_widget_set_hexpand(space, TRUE);
    gtk_widget_set_hexpand(space1, TRUE);
    gtk_widget_set_hexpand(space2, TRUE);
    gtk_widget_set_hexpand(space3, TRUE);
    gtk_widget_set_hexpand(space4, TRUE);
    gtk_widget_set_hexpand(label_player, TRUE);
	gtk_widget_set_hexpand(label_bot, TRUE);   
	//создание поля игрока
	GtkWidget *player1_field=gtk_grid_new();
	//GtkWidget *buttons[GRID_SIZE][GRID_SIZE];
	 //GtkWidget *buttons[GRID_SIZE][GRID_SIZE];
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			buttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(buttons[i][j], TRUE);
            gtk_widget_set_vexpand(buttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(player1_field), buttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(buttons[i][j], "empty");
            g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(player2_attacked), NULL);
		}
	}	
	gtk_widget_set_hexpand(player1_field, TRUE);
	//создание поля игрока
	GtkWidget *player2_field=gtk_grid_new();
	//GtkWidget *bbuttons[GRID_SIZE][GRID_SIZE];
	 //GtkWidget *buttons[GRID_SIZE][GRID_SIZE];
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			bbuttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(bbuttons[i][j], TRUE);
            gtk_widget_set_vexpand(bbuttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(player2_field), bbuttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(bbuttons[i][j], "empty");
            g_signal_connect(bbuttons[i][j], "clicked", G_CALLBACK(player1_attacked), NULL);
            //player2_attacked
		}
	}
	gtk_widget_set_hexpand(player2_field, TRUE);	
    //Добавление в сетку
    gtk_grid_attach(GTK_GRID(grid),label_player, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),space, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),label_bot, 2,0,1,1);
    //пустая полоса
    gtk_grid_attach(GTK_GRID(grid),space1, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),space2, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),space3, 2,1,1,1);
    //игровые поля
    gtk_grid_attach(GTK_GRID(grid),player1_field, 0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),space4, 1,2,1,1);
	gtk_grid_attach(GTK_GRID(grid),player2_field, 2,2,1,1);
	//кнопка закрытия
	gtk_grid_attach(GTK_GRID(grid),close_button, 1,3,1,1);	
	
	human_place_ship(buttons,4);
	human_place_ship(buttons,3);
	human_place_ship(buttons,3);
	human_place_ship(buttons,2);
	human_place_ship(buttons,2);
	human_place_ship(buttons,2);
	human_place_ship(buttons,1);
	human_place_ship(buttons,1);
	human_place_ship(buttons,1);
	human_place_ship(buttons,1);
	
	human_place_ship(bbuttons,4);
	human_place_ship(bbuttons,3);
	human_place_ship(bbuttons,3);
	human_place_ship(bbuttons,2);
	human_place_ship(bbuttons,2);
	human_place_ship(bbuttons,2);
	human_place_ship(bbuttons,1);
	human_place_ship(bbuttons,1);
	human_place_ship(bbuttons,1);
	human_place_ship(bbuttons,1);	
	
	g_signal_connect(close_button, "clicked", G_CALLBACK(destroyer), secondary_window);			
	gtk_widget_show_all(secondary_window);
}

void on_player_exit_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(game_window);
}

void on_player2_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget ***player2_buttons = (GtkWidget ***)data;
    const gchar *name = gtk_widget_get_name(widget);
    if (g_strcmp0(name, "ship") == 0) {
        gtk_widget_set_name(widget, "hit");
        apply_css(widget, "button { background-color: red; }");
    } else if (g_strcmp0(name, "mine") == 0) {
        gtk_widget_set_name(widget, "exploded");
        apply_css(widget, "button { background-color: orange; }");
        // Вызываем функцию exploded для обработки взрыва мины
        int rows = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "rows"));
        int cols = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "cols"));
        exploded(widget, player2_buttons, rows, cols);
    } else if (g_strcmp0(name, "empty") == 0) {
        gtk_widget_set_name(widget, "miss");
        apply_css(widget, "button { background-color: yellow; }");
    }
    // Проверка, остались ли еще корабли на поле игрока 2
    gboolean player2_ships_left = FALSE;
    int rows = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "rows"));
    int cols = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "cols"));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (g_strcmp0(gtk_widget_get_name(player2_buttons[i][j]), "ship") == 0) {
                player2_ships_left = TRUE;
                break;
            }
        }
        if (player2_ships_left) break;
    }
    if (!player2_ships_left) {
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                        GTK_DIALOG_MODAL, "ВЫЙТИ", GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *label = gtk_label_new("Игрок 1 победил!");
        
        PangoAttrList *attr_list1 = pango_attr_list_new();
    	PangoAttribute *size_attr1 = pango_attr_size_new(20 * PANGO_SCALE);
    	pango_attr_list_insert(attr_list1, size_attr1);
    	PangoAttribute *color_attr1 = pango_attr_foreground_new(0, 0, 65335);
   	 	pango_attr_list_insert(attr_list1, color_attr1);
    	gtk_label_set_attributes(GTK_LABEL(label), attr_list1);    	
    	pango_attr_list_unref(attr_list1);
        
        gtk_container_add(GTK_CONTAINER(content_area), label);
        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(gtk_widget_get_toplevel(widget)));
        return;
    }
}

void on_player1_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget ***player1_buttons = (GtkWidget ***)data;
    const gchar *name = gtk_widget_get_name(widget);
    if (g_strcmp0(name, "ship") == 0) {
        gtk_widget_set_name(widget, "hit");
        apply_css(widget, "button { background-color: red; }");
    } else if (g_strcmp0(name, "mine") == 0) {
        gtk_widget_set_name(widget, "exploded");
        apply_css(widget, "button { background-color: orange; }");
        // Вызываем функцию exploded для обработки взрыва мины
        int rows = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "rows"));
        int cols = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "cols"));
        exploded(widget, player1_buttons, rows, cols);
    } else if (g_strcmp0(name, "empty") == 0) {
        gtk_widget_set_name(widget, "miss");
        apply_css(widget, "button { background-color: yellow; }");
    }
    // Проверка, остались ли еще корабли на поле игрока 1
    gboolean player1_ships_left = FALSE;
    int rows = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "rows"));
    int cols = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "cols"));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (g_strcmp0(gtk_widget_get_name(player1_buttons[i][j]), "ship") == 0) {
                player1_ships_left = TRUE;
                break;
            }
        }
        if (player1_ships_left) break;
    }
    if (!player1_ships_left) {
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                        GTK_DIALOG_MODAL, "ВЫЙТИ", GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *label = gtk_label_new("Игрок 2 победил!");
        
        PangoAttrList *attr_list1 = pango_attr_list_new();
    	PangoAttribute *size_attr1 = pango_attr_size_new(20 * PANGO_SCALE);
    	pango_attr_list_insert(attr_list1, size_attr1);
    	PangoAttribute *color_attr1 = pango_attr_foreground_new(0, 0, 65335);
   	 	pango_attr_list_insert(attr_list1, color_attr1);
    	gtk_label_set_attributes(GTK_LABEL(label), attr_list1);    	
    	pango_attr_list_unref(attr_list1);
       
        gtk_container_add(GTK_CONTAINER(content_area), label);
        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(gtk_widget_get_toplevel(widget)));
        return;
    }
}


void on_player_forward_button_clicked(GtkWidget *widget, gpointer data) ;

// Определение структуры для передачи параметров
typedef struct {
    GtkWidget *entry_rows;
    GtkWidget *entry_cols;
    GtkWidget *toggle_mines;
} GameSettings;

// Функция для обработки события клика по кнопке "Настройки режима"
void on_human_extended_mode_button_clicked(GtkWidget *widget, gpointer data) {
    secondary_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(secondary_window), "Настройки режима");
    gtk_window_set_position(GTK_WINDOW(secondary_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(secondary_window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(secondary_window), 10);
    // Создание вертикального контейнера
    GtkWidget *vvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(secondary_window), vvbox);
    // Надпись "Размеры поля"
    GtkWidget *label_field_size = gtk_label_new("Размеры поля:");
    gtk_box_pack_start(GTK_BOX(vvbox), label_field_size, FALSE, FALSE, 0);
    // Создание горизонтального контейнера для полей ввода
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vvbox), hbox, FALSE, FALSE, 0);
    // Поле ввода для количества строк
    GtkWidget *entry_rows = gtk_spin_button_new_with_range(10, 100, 1);
    gtk_box_pack_start(GTK_BOX(hbox), entry_rows, TRUE, TRUE, 0);
    // Поле ввода для количества столбцов
    GtkWidget *entry_cols = gtk_spin_button_new_with_range(10, 100, 1);
    gtk_box_pack_start(GTK_BOX(hbox), entry_cols, TRUE, TRUE, 0);
    // Создание переключателя "Добавить мины"
    GtkWidget *toggle_mines = gtk_switch_new();
    gtk_box_pack_start(GTK_BOX(vvbox), toggle_mines, FALSE, FALSE, 0);
    GtkWidget *label_mines = gtk_label_new("Добавить мины");
    gtk_box_pack_start(GTK_BOX(vvbox), label_mines, FALSE, FALSE, 0);
    // Создание кнопки "Вперед"
    GtkWidget *button_forward = gtk_button_new_with_label("Вперед");
    gtk_box_pack_end(GTK_BOX(vvbox), button_forward, FALSE, FALSE, 0);
    // Создаем структуру для передачи параметров
    GameSettings *settings = g_new(GameSettings, 1);
    settings->entry_rows = entry_rows;
    settings->entry_cols = entry_cols;
    settings->toggle_mines = toggle_mines;
    // Соединяем сигнал с кнопкой "Вперед"
    g_signal_connect(button_forward, "clicked", G_CALLBACK(on_player_forward_button_clicked), settings);
    gtk_widget_show_all(secondary_window);        
}

// Функция для обработки события клика по кнопке "Вперед"
void on_player_forward_button_clicked(GtkWidget *widget, gpointer data) {
    GameSettings *settings = (GameSettings *)data;
    GtkWidget *entry_rows = settings->entry_rows;
    GtkWidget *entry_cols = settings->entry_cols;
    GtkWidget *toggle_mines = settings->toggle_mines;
    // Проверка, являются ли entry_rows и entry_cols объектами GtkSpinButton
    if (!GTK_IS_SPIN_BUTTON(entry_rows) || !GTK_IS_SPIN_BUTTON(entry_cols)) {
        g_warning("Entry rows or cols is not a GtkSpinButton.");
        g_free(settings);
        return;
    }
    int rows = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry_rows));
    int cols = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry_cols));
    gboolean add_mines = gtk_switch_get_active(GTK_SWITCH(toggle_mines));
	// Создание окна с игровыми полями для игрока и бота
    game_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(game_window), "Расширенный режим против игрока");
    gtk_window_set_position(GTK_WINDOW(game_window), GTK_WIN_POS_CENTER);
    gtk_window_maximize(GTK_WINDOW(game_window));
    gtk_container_set_border_width(GTK_CONTAINER(game_window), 10);
    GtkWidget *game_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(game_window), game_grid);
    gtk_grid_set_row_spacing(GTK_GRID(game_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(game_grid), 10);
    GtkWidget *player_exit_button = gtk_button_new_with_label("Выйти");
    GtkWidget *label1 = gtk_label_new("Игрок 1");
    GtkWidget *label2 = gtk_label_new(NULL);
    GtkWidget *label3 = gtk_label_new("Игрок 2");
    GtkWidget *label7 = gtk_label_new(NULL);
    gtk_widget_set_hexpand(label1, TRUE);
    gtk_widget_set_hexpand(label3, TRUE);
    PangoAttrList *attr_list1 = pango_attr_list_new();
    PangoAttribute *size_attr1 = pango_attr_size_new(20 * PANGO_SCALE);
    pango_attr_list_insert(attr_list1, size_attr1);
    PangoAttribute *color_attr1 = pango_attr_foreground_new(0, 0, 65335);
    pango_attr_list_insert(attr_list1, color_attr1);
    gtk_label_set_attributes(GTK_LABEL(label1), attr_list1);
    gtk_label_set_attributes(GTK_LABEL(label3), attr_list1);
    pango_attr_list_unref(attr_list1);
    gtk_grid_attach(GTK_GRID(game_grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(game_grid), label2, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(game_grid), label3, 2, 0, 1, 1);
    // Scrolled window для поля игрока
    GtkWidget *player1_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(player1_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(player1_scrolled, TRUE);
    gtk_widget_set_vexpand(player1_scrolled, TRUE);
    gtk_grid_attach(GTK_GRID(game_grid), player1_scrolled, 0, 2, 1, 1);
    GtkWidget *player1_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(player1_scrolled), player1_grid);
    GtkWidget ***player1_buttons = (GtkWidget ***) g_malloc0(rows * sizeof(GtkWidget **));
    for (int i = 0; i < rows; i++) {
        player1_buttons[i] = (GtkWidget **) g_malloc0(cols * sizeof(GtkWidget *));
        for (int j = 0; j < cols; j++) {
            player1_buttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(player1_buttons[i][j], TRUE);
            gtk_widget_set_vexpand(player1_buttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(player1_grid), player1_buttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(player1_buttons[i][j], "empty");
            g_signal_connect(player1_buttons[i][j], "clicked", G_CALLBACK(on_player1_button_clicked), player1_buttons);
        }
    }
    gtk_grid_attach(GTK_GRID(game_grid), label7, 1, 2, 1, 1);
    // Scrolled window для поля бота
    GtkWidget *player2_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(player2_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(player2_scrolled, TRUE);
    gtk_widget_set_vexpand(player2_scrolled, TRUE);
    gtk_grid_attach(GTK_GRID(game_grid), player2_scrolled, 2, 2, 1, 1);
    GtkWidget *player2_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(player2_scrolled), player2_grid);
    GtkWidget ***player2_buttons = (GtkWidget ***) g_malloc0(rows * sizeof(GtkWidget **));
    for (int i = 0; i < rows; i++) {
        player2_buttons[i] = (GtkWidget **) g_malloc0(cols * sizeof(GtkWidget *));
        for (int j = 0; j < cols; j++) {
            player2_buttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(player2_buttons[i][j], TRUE);
            gtk_widget_set_vexpand(player2_buttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(player2_grid), player2_buttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(player2_buttons[i][j], "empty");
            g_signal_connect(player2_buttons[i][j], "clicked", G_CALLBACK(on_player2_button_clicked), player2_buttons);
        }
    }
    gtk_grid_attach(GTK_GRID(game_grid), player_exit_button, 1, 3, 1, 1);
    g_signal_connect(player_exit_button, "clicked", G_CALLBACK(on_player_exit_button_clicked), game_window);

    place_ship_extended(player1_buttons, rows, cols, 4);
    place_ship_extended(player1_buttons, rows, cols, 3);
    place_ship_extended(player1_buttons, rows, cols, 3);
    place_ship_extended(player1_buttons, rows, cols, 2);
    place_ship_extended(player1_buttons, rows, cols, 2);
    place_ship_extended(player1_buttons, rows, cols, 2);
    place_ship_extended(player1_buttons, rows, cols, 1);
    place_ship_extended(player1_buttons, rows, cols, 1);
    place_ship_extended(player1_buttons, rows, cols, 1);
    place_ship_extended(player1_buttons, rows, cols, 1);

    place_ship_extended(player2_buttons, rows, cols, 4);
    place_ship_extended(player2_buttons, rows, cols, 3);
    place_ship_extended(player2_buttons, rows, cols, 3);
    place_ship_extended(player2_buttons, rows, cols, 2);
    place_ship_extended(player2_buttons, rows, cols, 2);
    place_ship_extended(player2_buttons, rows, cols, 2);
    place_ship_extended(player2_buttons, rows, cols, 1);
    place_ship_extended(player2_buttons, rows, cols, 1);
    place_ship_extended(player2_buttons, rows, cols, 1);
    place_ship_extended(player2_buttons, rows, cols, 1);
	
	// Установка мин, если включено
    if (add_mines) {
        place_mines(player1_buttons, rows, cols, 3);
        place_mines(player2_buttons, rows, cols, 3);
    }	
    g_object_set_data(G_OBJECT(game_window), "player1_buttons", player1_buttons);
    g_object_set_data(G_OBJECT(game_window), "player2_buttons", player2_buttons);
    g_object_set_data(G_OBJECT(game_window), "rows", GINT_TO_POINTER(rows));
    g_object_set_data(G_OBJECT(game_window), "cols", GINT_TO_POINTER(cols));
    
    gtk_widget_show_all(game_window);
	gtk_widget_destroy(secondary_window);
}

 void on_vs_player_button_clicked(GtkWidget *widget, gpointer data) {
	secondary_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(secondary_window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(secondary_window), 300, 200);
	gtk_window_set_title(GTK_WINDOW(secondary_window),"Выбор режима:");
	gtk_container_set_border_width(GTK_CONTAINER(secondary_window),10);	
    GtkWidget *vvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(secondary_window), vvbox);	
	GtkWidget *classic_mode_button = gtk_button_new_with_label("Классический режим");
    GtkWidget *extended_mode_button = gtk_button_new_with_label("Расширенный режим");    
    gtk_box_pack_start(GTK_BOX(vvbox), classic_mode_button , TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vvbox), extended_mode_button, TRUE, TRUE, 0);	
	g_signal_connect(classic_mode_button, "clicked", G_CALLBACK(on_human_classic_mode_button_clicked), secondary_window);
	g_signal_connect(extended_mode_button, "clicked", G_CALLBACK(on_human_extended_mode_button_clicked), secondary_window);	
	gtk_widget_show_all(secondary_window);
}

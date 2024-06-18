#define GRID_SIZE 10

GtkWidget *secondary_window;
GtkWidget *player_field;
GtkWidget *dialog;
GtkWidget *dialogg;
GtkWidget *bot_field;
GtkWidget *bbuttons[GRID_SIZE][GRID_SIZE];
GtkWidget *buttons[GRID_SIZE][GRID_SIZE];
GtkWidget *close_button;
GtkWidget *game_window;
GtkWidget *player_hits_label;

void destroyer(GtkWidget *widget, gpointer data){
	gtk_widget_destroy(GTK_WIDGET(secondary_window));
}

void apply_css(GtkWidget *widget, const char *css) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

void check_for_end_of_game() {
    // Проверка на наличие оставшихся кораблей игрока
    gboolean player_ships_left = FALSE;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            const char *button_name = gtk_widget_get_name(buttons[i][j]);
            if (strcmp(button_name, "ship") == 0) {
                player_ships_left = TRUE;
                break;
            }
        }
        if (player_ships_left) break;
    }
    // Если кораблей игрока не осталось, показываем диалоговое окно с кнопкой "Выйти"
    if (!player_ships_left) {   	
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(secondary_window), GTK_DIALOG_MODAL,"ВЫЙТИ", GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
        gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *label = gtk_label_new("Бот победил!");
        
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

void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *name = gtk_widget_get_name(widget);
    if (strcmp(name, "ship") == 0) {
        gtk_widget_set_name(widget, "hit");
        apply_css(widget, "button { background-color: red; }");
    } else if(strcmp(name, "ship") != 0 && strcmp(name, "hit") != 0){
        gtk_widget_set_name(widget, "miss");
        apply_css(widget, "button { background-color: yellow; }");
    }
    // Проверка на наличие оставшихся кораблей бота
    gboolean any_ships_left = FALSE;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            const char *button_name = gtk_widget_get_name(bbuttons[i][j]);
            if (strcmp(button_name, "ship") == 0) {
                any_ships_left = TRUE;
                break;
            }
        }
        if (any_ships_left) break;
    }
    // Если кораблей не осталось, показываем диалоговое окно с кнопкой "Выйти"
    if (!any_ships_left) {
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(secondary_window), GTK_DIALOG_MODAL,"ВЫЙТИ", GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *label = gtk_label_new("Вы победили!");
        
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
    // Ответный удар бота
    gboolean bot_move_made = FALSE;
    while (!bot_move_made) {
        int rand_x = rand() % GRID_SIZE;
        int rand_y = rand() % GRID_SIZE;
        const char *bot_button_name = gtk_widget_get_name(buttons[rand_x][rand_y]);
        if (strcmp(bot_button_name, "hit") != 0 && strcmp(bot_button_name, "miss") != 0) {
            if (strcmp(bot_button_name, "ship") == 0) {
                gtk_widget_set_name(buttons[rand_x][rand_y], "hit");
                apply_css(buttons[rand_x][rand_y], "button { background-color: red; }");
            } else {
                gtk_widget_set_name(buttons[rand_x][rand_y], "miss");
                apply_css(buttons[rand_x][rand_y], "button { background-color: yellow; }");
            }
            bot_move_made = TRUE;
        }
    }
    // Проверка на конец игры для игрока после хода бота
    check_for_end_of_game();
}

void place_ship(GtkWidget *buttons[GRID_SIZE][GRID_SIZE], int ship_size) {
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
                    //apply_css(buttons[dirH][j], "button { background-color: blue; }");                  
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
                    //apply_css(buttons[j][dirV], "button { background-color: blue; }");                    
                }
            }
        }
    }
}

void on_bot_classic_mode_button_clicked(GtkWidget *widget, gpointer data) {
	//окно/классический режим/против бота
    secondary_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(secondary_window),"Классический режим против бота");
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
	GtkWidget *label_player= gtk_label_new("Игрок");
    GtkWidget *label_bot = gtk_label_new("Бот");
    GtkWidget *space = gtk_label_new(NULL);
    GtkWidget *player_hits_label = gtk_label_new(NULL);
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
    gtk_label_set_attributes(GTK_LABEL(player_hits_label),attr_list1);
    gtk_label_set_attributes(GTK_LABEL(space3),attr_list1);
    pango_attr_list_unref(attr_list1);
    //Расширение по горизонтали
    gtk_widget_set_hexpand(space, TRUE);
    gtk_widget_set_hexpand(player_hits_label, TRUE);
    gtk_widget_set_hexpand(space2, TRUE);
    gtk_widget_set_hexpand(space3, TRUE);
    gtk_widget_set_hexpand(space4, TRUE);
    gtk_widget_set_hexpand(label_player, TRUE);
	gtk_widget_set_hexpand(label_bot, TRUE);   
	//создание поля игрока
	player_field=gtk_grid_new();	
	//GtkWidget *buttons[GRID_SIZE][GRID_SIZE];
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			buttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(buttons[i][j], TRUE);
            gtk_widget_set_vexpand(buttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(player_field), buttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(buttons[i][j], "empty");           
		}
	}		
	gtk_widget_set_hexpand(player_field, TRUE);	
	//создание поля игрока
	bot_field=gtk_grid_new();
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			bbuttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(bbuttons[i][j], TRUE);
            gtk_widget_set_vexpand(bbuttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(bot_field), bbuttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(bbuttons[i][j], "empty");
            g_signal_connect(bbuttons[i][j], "clicked", G_CALLBACK(on_button_clicked), NULL);
		}
	}
	gtk_widget_set_hexpand(bot_field, TRUE);	
    //Добавление в сетку
    gtk_grid_attach(GTK_GRID(grid),label_player, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),space, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),label_bot, 2,0,1,1);
    //пустая полоса
    gtk_grid_attach(GTK_GRID(grid),player_hits_label, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),space2, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),space3, 2,1,1,1);
    //игровые поля
    gtk_grid_attach(GTK_GRID(grid),player_field, 0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),space4, 1,2,1,1);
	gtk_grid_attach(GTK_GRID(grid),bot_field, 2,2,1,1);
	//кнопка закрытия
	gtk_grid_attach(GTK_GRID(grid),close_button, 1,3,1,1);
	g_signal_connect(close_button, "clicked", G_CALLBACK(destroyer), secondary_window);
	
	place_ship(buttons,4);
	place_ship(buttons,3);
	place_ship(buttons,3);
	place_ship(buttons,2);
	place_ship(buttons,2);
	place_ship(buttons,2);
	place_ship(buttons,1);
	place_ship(buttons,1);
	place_ship(buttons,1);
	place_ship(buttons,1);
	
	place_ship(bbuttons,4);
	place_ship(bbuttons,3);
	place_ship(bbuttons,3);
	place_ship(bbuttons,2);
	place_ship(bbuttons,2);
	place_ship(bbuttons,2);
	place_ship(bbuttons,1);
	place_ship(bbuttons,1);
	place_ship(bbuttons,1);
	place_ship(bbuttons,1);
	
	gtk_widget_show_all(secondary_window);
}

void place_ship_extended(GtkWidget ***buttons, int rows, int cols, int ship_size) {
    int dir = rand() % 2;  // 0 - горизонтально, 1 - вертикально
    bool valid_position = false;
    while (!valid_position) {
        if (dir == 0) {  // Горизонтальное размещение
            int dirH = rand() % rows;
            int randY = rand() % cols;
            // Проверка на выход за границы
            if (randY + ship_size > cols) continue;
            bool can_place = true;
            for (int i = randY; i < randY + ship_size; i++) {
                if (strcmp(gtk_widget_get_name(buttons[dirH][i]), "ship") == 0 ||
                    (dirH + 1 < rows && strcmp(gtk_widget_get_name(buttons[dirH + 1][i]), "ship") == 0) ||
                    (i - 1 >= 0 && dirH + 1 < rows && strcmp(gtk_widget_get_name(buttons[dirH + 1][i - 1]), "ship") == 0) ||
                    (i + 1 < cols && dirH + 1 < rows && strcmp(gtk_widget_get_name(buttons[dirH + 1][i + 1]), "ship") == 0) ||
                    (dirH - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH - 1][i]), "ship") == 0) ||
                    (i - 1 >= 0 && dirH - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH - 1][i - 1]), "ship") == 0) ||
                    (i + 1 < cols && dirH - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH - 1][i + 1]), "ship") == 0) ||
                    (i - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[dirH][i - 1]), "ship") == 0) ||
                    (i + 1 < cols && strcmp(gtk_widget_get_name(buttons[dirH][i + 1]), "ship") == 0)) {
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
            int randXX = rand() % rows;
            int dirV = rand() % cols;

            // Проверка на выход за границы
            if (randXX + ship_size > rows) continue;
            bool can_place = true;
            for (int i = randXX; i < randXX + ship_size; i++) {
                if (strcmp(gtk_widget_get_name(buttons[i][dirV]), "ship") == 0 ||
                    (i + 1 < rows && strcmp(gtk_widget_get_name(buttons[i + 1][dirV]), "ship") == 0) ||
                    (i + 1 < rows && dirV - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i + 1][dirV - 1]), "ship") == 0) ||
                    (i + 1 < rows && dirV + 1 < cols && strcmp(gtk_widget_get_name(buttons[i + 1][dirV + 1]), "ship") == 0) ||
                    (i - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i - 1][dirV]), "ship") == 0) ||
                    (i - 1 >= 0 && dirV - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i - 1][dirV - 1]), "ship") == 0) ||
                    (i - 1 >= 0 && dirV + 1 < cols && strcmp(gtk_widget_get_name(buttons[i - 1][dirV + 1]), "ship") == 0) ||
                    (dirV - 1 >= 0 && strcmp(gtk_widget_get_name(buttons[i][dirV - 1]), "ship") == 0) ||
                    (dirV + 1 < cols && strcmp(gtk_widget_get_name(buttons[i][dirV + 1]), "ship") == 0)) {
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

void on_exit_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(data));
}

// Функция для размещения мин
void place_mines(GtkWidget ***buttons, int rows, int cols, int num_mines) {
    int placed_mines = 0;
    while (placed_mines < num_mines) {
        int row = rand() % rows;
        int col = rand() % cols;
        const gchar *name = gtk_widget_get_name(buttons[row][col]);
        if (g_strcmp0(name, "empty") == 0) {
            gtk_widget_set_name(buttons[row][col], "mine");
            placed_mines++;
        }
    }
}

void exploded(GtkWidget *widget, GtkWidget ***buttons, int rows, int cols) {
    // Красим текущую клетку в синий и ставим метку "exploded"
    gtk_widget_set_name(widget, "exploded");
    apply_css(widget, "button { background-color: blue; }");
    // Найти координаты текущей клетки
    int row = -1, col = -1;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (buttons[r][c] == widget) {
                row = r;
                col = c;
                break;
            }
        }
        if (row != -1 && col != -1) break;
    }
    // Проверяем верхнюю соседнюю клетку
    if (row > 0) {
        GtkWidget *top_neighbor = buttons[row - 1][col];
        const gchar *top_name = gtk_widget_get_name(top_neighbor);
        if (g_strcmp0(top_name, "ship") == 0) {
            gtk_widget_set_name(top_neighbor, "hit by mine");
            apply_css(top_neighbor, "button { background-color: black; }");
        } else if (g_strcmp0(top_name, "empty") == 0) {
            gtk_widget_set_name(top_neighbor, "miss");
            apply_css(top_neighbor, "button { background-color: yellow; }");
        }
    }
    // Проверяем нижнюю соседнюю клетку
    if (row < rows - 1) {
        GtkWidget *bottom_neighbor = buttons[row + 1][col];
        const gchar *bottom_name = gtk_widget_get_name(bottom_neighbor);
        if (g_strcmp0(bottom_name, "ship") == 0) {
            gtk_widget_set_name(bottom_neighbor, "hit by mine");
            apply_css(bottom_neighbor, "button { background-color: black; }");
        } else if (g_strcmp0(bottom_name, "empty") == 0) {
            gtk_widget_set_name(bottom_neighbor, "miss");
            apply_css(bottom_neighbor, "button { background-color: yellow; }");
        }
    }
}

// Обработчик события для кнопок бота
void on_bot_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget ***bot_buttons = (GtkWidget ***)data;
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
        exploded(widget, bot_buttons, rows, cols);
    } else if (g_strcmp0(name, "empty") == 0) {
        gtk_widget_set_name(widget, "miss");
        apply_css(widget, "button { background-color: yellow; }");
    }
    // Проверка, остались ли еще корабли на поле бота
    gboolean bot_ships_left = FALSE;
    for (int i = 0; bot_buttons[i] != NULL; i++) {
        for (int j = 0; bot_buttons[i][j] != NULL; j++) {
            if (g_strcmp0(gtk_widget_get_name(bot_buttons[i][j]), "ship") == 0) {
                bot_ships_left = TRUE;
                break;
            }
        }
        if (bot_ships_left) break;
    }
    if (!bot_ships_left) {
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                        GTK_DIALOG_MODAL, "ВЫЙТИ", GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *label = gtk_label_new("Вы победили!");
       
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
    // Ход бота
    GtkWidget ***player_buttons = (GtkWidget ***)g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "player_buttons");
    int rows = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "rows"));
    int cols = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)), "cols"));
    gboolean player_ships_left = TRUE;
    while (player_ships_left) {
        int bot_row = rand() % rows;
        int bot_col = rand() % cols;
        GtkWidget *player_button = player_buttons[bot_row][bot_col];
        const gchar *player_name = gtk_widget_get_name(player_button);
        if (g_strcmp0(player_name, "empty") == 0 || g_strcmp0(player_name, "ship") == 0 || g_strcmp0(player_name, "mine") == 0) {
            if (g_strcmp0(player_name, "ship") == 0) {
                gtk_widget_set_name(player_button, "hit");
                apply_css(player_button, "button { background-color: red; }");
            } else if (g_strcmp0(player_name, "mine") == 0) {
                gtk_widget_set_name(player_button, "exploded");
                apply_css(player_button, "button { background-color: orange; }");
                // Вызываем функцию exploded для обработки взрыва мины
                exploded(player_button, player_buttons, rows, cols);
            } else {
                gtk_widget_set_name(player_button, "miss");
                apply_css(player_button, "button { background-color: yellow; }");
            }
            // Проверка, остались ли еще корабли на поле игрока
            player_ships_left = FALSE;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (g_strcmp0(gtk_widget_get_name(player_buttons[i][j]), "ship") == 0) {
                        player_ships_left = TRUE;
                        break;
                    }
                }
                if (player_ships_left) break;
            }
            break;
        }
    }
    if (!player_ships_left) {
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Конец игры", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                        GTK_DIALOG_MODAL, "ВЫЙТИ", GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *label = gtk_label_new("Бот победил!");
        
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
    }
}

void on_forward_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = GTK_WIDGET(data);
    // Получение параметров из виджетов
    GtkWidget *entry_rows = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog), "entry_rows"));
    GtkWidget *entry_cols = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog), "entry_cols"));
    GtkWidget *toggle_mines = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog), "toggle_mines"));
    int rows = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry_rows));
    int cols = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry_cols));
    gboolean add_mines = gtk_switch_get_active(GTK_SWITCH(toggle_mines));
    // Создание окна с игровыми полями для игрока и бота
    game_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(game_window), "Расширенный режим против бота");
    gtk_window_set_position(GTK_WINDOW(game_window), GTK_WIN_POS_CENTER);
    gtk_window_maximize(GTK_WINDOW(game_window));
    gtk_container_set_border_width(GTK_CONTAINER(game_window), 10);
    GtkWidget *game_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(game_window), game_grid);
    gtk_grid_set_row_spacing(GTK_GRID(game_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(game_grid), 10);
    GtkWidget *exit_button = gtk_button_new_with_label("Выйти");
    GtkWidget *label1 = gtk_label_new("Игрок");
    GtkWidget *label2 = gtk_label_new(NULL);
    GtkWidget *label3 = gtk_label_new("Бот");
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
    GtkWidget *player_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(player_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(player_scrolled, TRUE);
    gtk_widget_set_vexpand(player_scrolled, TRUE);
    gtk_grid_attach(GTK_GRID(game_grid), player_scrolled, 0, 2, 1, 1);
    GtkWidget *player_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(player_scrolled), player_grid);
    GtkWidget ***player_buttons = (GtkWidget ***) g_malloc0(rows * sizeof(GtkWidget **));
    for (int i = 0; i < rows; i++) {
        player_buttons[i] = (GtkWidget **) g_malloc0(cols * sizeof(GtkWidget *));
        for (int j = 0; j < cols; j++) {
            player_buttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(player_buttons[i][j], TRUE);
            gtk_widget_set_vexpand(player_buttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(player_grid), player_buttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(player_buttons[i][j], "empty");
        }
    }
    gtk_grid_attach(GTK_GRID(game_grid), label7, 1, 2, 1, 1);
    // Scrolled window для поля бота
    GtkWidget *bot_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(bot_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(bot_scrolled, TRUE);
    gtk_widget_set_vexpand(bot_scrolled, TRUE);
    gtk_grid_attach(GTK_GRID(game_grid), bot_scrolled, 2, 2, 1, 1);
    GtkWidget *bot_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(bot_scrolled), bot_grid);
    GtkWidget ***bot_buttons = (GtkWidget ***) g_malloc0(rows * sizeof(GtkWidget **));
    for (int i = 0; i < rows; i++) {
        bot_buttons[i] = (GtkWidget **) g_malloc0(cols * sizeof(GtkWidget *));
        for (int j = 0; j < cols; j++) {
            bot_buttons[i][j] = gtk_button_new();
            gtk_widget_set_hexpand(bot_buttons[i][j], TRUE);
            gtk_widget_set_vexpand(bot_buttons[i][j], TRUE);
            gtk_grid_attach(GTK_GRID(bot_grid), bot_buttons[i][j], j, i, 1, 1);
            gtk_widget_set_name(bot_buttons[i][j], "empty");
            g_signal_connect(bot_buttons[i][j], "clicked", G_CALLBACK(on_bot_button_clicked), bot_buttons);
        }
    }
    gtk_grid_attach(GTK_GRID(game_grid), exit_button, 1, 3, 1, 1);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_button_clicked), game_window);

    place_ship_extended(player_buttons, rows, cols, 4);
    place_ship_extended(player_buttons, rows, cols, 3);
    place_ship_extended(player_buttons, rows, cols, 3);
    place_ship_extended(player_buttons, rows, cols, 2);
    place_ship_extended(player_buttons, rows, cols, 2);
    place_ship_extended(player_buttons, rows, cols, 2);
    place_ship_extended(player_buttons, rows, cols, 1);
    place_ship_extended(player_buttons, rows, cols, 1);
    place_ship_extended(player_buttons, rows, cols, 1);
    place_ship_extended(player_buttons, rows, cols, 1);

    place_ship_extended(bot_buttons, rows, cols, 4);
    place_ship_extended(bot_buttons, rows, cols, 3);
    place_ship_extended(bot_buttons, rows, cols, 3);
    place_ship_extended(bot_buttons, rows, cols, 2);
    place_ship_extended(bot_buttons, rows, cols, 2);
    place_ship_extended(bot_buttons, rows, cols, 2);
    place_ship_extended(bot_buttons, rows, cols, 1);
    place_ship_extended(bot_buttons, rows, cols, 1);
    place_ship_extended(bot_buttons, rows, cols, 1);
    place_ship_extended(bot_buttons, rows, cols, 1);
	
	// Установка мин, если включено
    if (add_mines) {
        place_mines(player_buttons, rows, cols, 3);
        place_mines(bot_buttons, rows, cols, 3);
    }	
    g_object_set_data(G_OBJECT(game_window), "player_buttons", player_buttons);
    g_object_set_data(G_OBJECT(game_window), "rows", GINT_TO_POINTER(rows));
    g_object_set_data(G_OBJECT(game_window), "cols", GINT_TO_POINTER(cols));
    gtk_widget_show_all(game_window);
    gtk_widget_destroy(dialog);
}

void on_bot_extended_mode_button_clicked(GtkWidget *widget, gpointer data) {
    secondary_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(secondary_window), "Настройка режима");
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
    GtkWidget *entry_rows = gtk_spin_button_new_with_range(10, 50, 1);
    gtk_box_pack_start(GTK_BOX(hbox), entry_rows, TRUE, TRUE, 0);
    // Поле ввода для количества столбцов
    GtkWidget *entry_cols = gtk_spin_button_new_with_range(10, 50, 1);
    gtk_box_pack_start(GTK_BOX(hbox), entry_cols, TRUE, TRUE, 0);
    // Создание переключателя "Добавить мины"
    GtkWidget *toggle_mines = gtk_switch_new();
    gtk_box_pack_start(GTK_BOX(vvbox), toggle_mines, FALSE, FALSE, 0);
    GtkWidget *label_mines = gtk_label_new("Добавить мины");
    gtk_box_pack_start(GTK_BOX(vvbox), label_mines, FALSE, FALSE, 0);
    // Создание кнопки "Вперед"
    GtkWidget *button_forward = gtk_button_new_with_label("Вперед");
    gtk_box_pack_end(GTK_BOX(vvbox), button_forward, FALSE, FALSE, 0);
    // Сохранение виджетов для использования в callback функции
    g_object_set_data(G_OBJECT(secondary_window), "entry_rows", entry_rows);
    g_object_set_data(G_OBJECT(secondary_window), "entry_cols", entry_cols);
    g_object_set_data(G_OBJECT(secondary_window), "toggle_mines", toggle_mines);
    // Соединяем сигналы с кнопками
    g_signal_connect(button_forward, "clicked", G_CALLBACK(on_forward_button_clicked), secondary_window);
    gtk_widget_show_all(secondary_window);
}

void on_vs_bot_button_clicked(GtkWidget *widget, gpointer data) {
	secondary_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(secondary_window),"Выбор режима:");
	gtk_window_set_position(GTK_WINDOW(secondary_window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(secondary_window), 300, 200);	
	gtk_container_set_border_width(GTK_CONTAINER(secondary_window),10);
    GtkWidget *vvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(secondary_window), vvbox);	
	GtkWidget *classic_mode_button = gtk_button_new_with_label("Классический режим");
    GtkWidget *extended_mode_button = gtk_button_new_with_label("Расширенный режим");    
    gtk_box_pack_start(GTK_BOX(vvbox), classic_mode_button , TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vvbox), extended_mode_button, TRUE, TRUE, 0);	
	g_signal_connect(classic_mode_button, "clicked", G_CALLBACK(on_bot_classic_mode_button_clicked), secondary_window);
	g_signal_connect(extended_mode_button, "clicked", G_CALLBACK(on_bot_extended_mode_button_clicked), secondary_window);	
	gtk_widget_show_all(secondary_window);  
}

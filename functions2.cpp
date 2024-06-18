GtkWidget *about_window;
void close_abbout_button(GtkWidget *widget, gpointer data) {
	gtk_widget_destroy(GTK_WIDGET(about_window));  
}
  
    
void on_quit_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_about_button_clicked(GtkWidget *widget, gpointer data) {
    // Создание нового окна
    about_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(about_window), "Об игре");
    gtk_window_maximize(GTK_WINDOW(about_window));
    gtk_window_set_position(GTK_WINDOW(about_window), GTK_WIN_POS_CENTER);

    // Создание контейнера для размещения виджетов
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(about_window), vbox);

    // Создание текстового вида для отображения текста
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    const gchar *about_text = "Правила классического Морского боя:\n"
                              "1. Игровое поле:\n"
                              "- Каждый игрок имеет собственное игровое поле, которое представляет собой сетку клеток.\n"
                              "- Сетка обычно имеет размер 10x10, где вертикальные линии обозначают буквы (от A до J), а горизонтальные - числа (от 1 до 10).\n"
                              "2. Корабли:\n"
                              "- Каждый игрок размещает:\n"
                              "- 4 однопалубных корабля (1 клетка)\n"
                              "- 3 двухпалубных корабля (2 клетки)\n"
                              "- 2 трехпалубных корабля (3 клетки)\n"
                              "- 1 четырехпалубный корабль (4 клетки)\n"
                              "- Корабли не могут пересекаться или соприкасаться.\n"
                              "3. Ход игры:\n"
                              "- Каждый ход состоит из выбора клетки на поле противника для выстрела.\n"
                              "- После каждого выстрела игрок сообщает результат противнику: попал ли он в корабль или промахнулся.\n"
                              "4. Результаты выстрелов:\n"
                              "- Если выстрел попал в корабль противника, игрок сообщает об этом и отмечает попадание на своем поле.\n"
                              "- Если выстрел промахнулся, игрок также сообщает об этом.\n"
                              "- Если все клетки корабля поражены, корабль считается потопленным, и игрок отмечает это на своем поле.\n"
                              "- Цель игрока - потопить все корабли противника, попадая в их клетки.\n"
                              "5. Победа:\n"
                              "- Игрок, который первым потопит все корабли противника, объявляется победителем.\n\n"
                              "Правила расширенного режима:\n"
                              "В расширенном режиме игроки:\n"
                              "- Сами выбирают размер игрового поля (от 10 до 50), количество кораблей не меняется.\n"
                              "- Решают будут ли добавляться в игру мины.\n"
                              "- Если да, то вне зависимости от размеров игрового поля добавятся 3 мины.\n"
                              "- При попадании на мины они 'взрываются', задевая одну клетку над собой и одну клетку под собой.\n"
                              "- Если в этих клетках были корабли, то они красятся в черный, иначе в желтый.\n"
                              "\nОсобенности моей реализации:\n"
                              "1. Состояние клетки:\n"
                              "- В любом режиме вы не сможете увидеть в каком состоянии находится клетка, пока не наведете на нее мышью.\n"
                              "- Это сделано с целью усложнить игру.\n"
                              "2. Цвета:\n"
                              "Если при наведении на клетку она светится:\n"
                              "- Серым, значит по этой клетке вы еще не 'стреляли'.\n"
                              "- Желтым, значит вы стреляли по этой клетке и не попали по кораблю.\n"
                              "- Синим, значит вы попали по мине.\n"
                              "- Красным, значит вы попали по кораблю.\n"
                              "- Черным, значит когда вы попали по мине, при 'взрыве' она задела корабль.\n\n"
                              "Разработчик:\n"
                              "- Это приложение разработал:\n"
                              "- Студент группы 5.205-1: Усмонов Отабек Рустамджонович\n"
                              "- Для связи:\n"
                              "- VK: https://vk.com/id531941526\n"; 

    gtk_text_buffer_set_text(buffer, about_text, -1);

    // Создание прокручиваемого окна для текстового вида
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);

    // Создание кнопки "Назад"
    GtkWidget *back_button = gtk_button_new_with_label("Назад");
    g_signal_connect(back_button, "clicked", G_CALLBACK(close_abbout_button), about_window);

    // Добавление виджетов в контейнер
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), back_button, FALSE, FALSE, 0);

    // Отображение всех виджетов в окне
    gtk_widget_show_all(about_window);
}




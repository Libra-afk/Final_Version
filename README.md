установить GTK3.0 - sudo apt-get install libgtk-3-dev
компиляция - gcc sea_battle2.cpp -o sea `pkg-config --cflags --libs gtk+-3.0`
запуск - ./sea

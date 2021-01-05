#include <iostream>
#include <termbox.h>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>

#include "termbox_utils.hpp"
#include "themes.hpp"

using namespace std;
using namespace chrono;

int current_theme = 0;
vector<int> zoom_settings = { 5,10,25,50,100 };

void draw_header() {
    string header = "typings";
    int x = tb_width() / 2 - header.length() / 2;
    int y = 0.05f * tb_height();
    tb_draw_string(x, y, header, THEMES[current_theme].text, THEMES[current_theme].background);
}

int main() {
    tb_event e;
    bool running = true;
    tb_init();
    tb_select_output_mode(TB_OUTPUT_256);
    
    while (running) {
        //  sets background, text color
        tb_set_clear_attributes(THEMES[current_theme].text, THEMES[current_theme].background);
        tb_clear();
        
        draw_header();
        
        tb_present();
        
        bool input = tb_poll_event(&e);

        if (input) {
            switch (e.type) {
            case TB_EVENT_KEY:
                switch (e.key) {
                case TB_KEY_CTRL_C:
                case TB_KEY_ESC:
                    running = false;
                    break;
                }
            }
        }
    }
    
    tb_shutdown();
}

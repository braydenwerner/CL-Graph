#include <iostream>
#include <termbox.h>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <ctype.h>
#include <algorithm> 

#include <cstdio>
#include "exprtk.hpp"
#include "termbox_utils.hpp"
#include "themes.hpp"

using namespace std;
using namespace chrono;

int current_theme = 0;

vector<int> zoom_settings = { 5,10,25,50,100 };
vector<int> yValues;
int current_zoom = zoom_settings[2];


uint32_t line_pieces[] = {
        '\\','|','/','-', '*'
};

void draw_axis() {
    int origin = min(tb_width(), tb_height()) / 2;
    for (int x = 0; x < origin * 2; x++) {
        tb_change_cell(x, origin, line_pieces[4], THEMES[current_theme].text, THEMES[current_theme].background);
        
        //  line markings
        if (x % 5 == 0 && origin + x < origin * 2) {
            tb_change_cell(origin + x, origin + 1, 'a', THEMES[current_theme].text, THEMES[current_theme].background);
            tb_change_cell(origin - x, origin + 1, 'a', THEMES[current_theme].text, THEMES[current_theme].background);
        }
    }
    
    for (int y = 0; y < origin * 2; y++) {
        tb_change_cell(origin, y, line_pieces[4], THEMES[current_theme].text, THEMES[current_theme].background);
        
        //  line markings
        if (y % 5 == 0 && origin + y < origin * 2) {
            tb_change_cell(origin - 1, origin + y, 'a', THEMES[current_theme].text, THEMES[current_theme].background);
            tb_change_cell(origin - 1, origin - y, 'a', THEMES[current_theme].text, THEMES[current_theme].background);
        }
    }
}

void graph_line() {
    int origin = min(tb_width(), tb_height()) / 2;
    for (int i = 0; i < origin * 2; i++) {
        tb_change_cell(i, -yValues.at(i) + origin, '*', THEMES[current_theme].text, THEMES[current_theme].background);
    }
}

template <typename T>
void fill_yValues() {
   typedef exprtk::symbol_table<T> symbol_table_t;
   typedef exprtk::expression<T>     expression_t;
   typedef exprtk::parser<T>             parser_t;

   std::string expression_string = "x^2";

   T x;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   symbol_table.add_constants();

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(expression_string,expression);

   int max_axis_value = min(tb_width(), tb_height());
   for (x = T(-max_axis_value / 2); x <= T(+max_axis_value / 2); x += T(1))
   {
      T y = expression.value();
      yValues.push_back(y);
   }
   
   for (int i = 0; i < yValues.size(); i++) {
       cout << yValues.at(i) << ' ';
   }
   cout << '\n';
}

int main() {
    tb_event e;
    bool running = true;
    tb_init();
    tb_select_output_mode(TB_OUTPUT_256);
    
    fill_yValues<double>();
    
    while (running) {
        //  sets background, text color
        tb_set_clear_attributes(THEMES[current_theme].text, THEMES[current_theme].background);
        tb_clear();
        
        draw_axis();
        graph_line();
        
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

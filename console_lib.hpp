// A small console library I built in C++
// Created by MOBSkuchen
// Resource : https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif // Windows/Linux
#include <sstream>
#include <string>
#include <vector>

void get_terminal_size(int& width, int& height) {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = (int)(csbi.srWindow.Right-csbi.srWindow.Left+1);
    height = (int)(csbi.srWindow.Bottom-csbi.srWindow.Top+1);
#elif defined(__linux__)
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    width = (int)(w.ws_col);
    height = (int)(w.ws_row);
#endif
}

#define ESC "\033["

#define TEXT_DEFAULT 0
#define TEXT_BOLD 1
#define TEXT_LIGHT 2
#define TEXT_ITALIC 3
#define TEXT_UNDERLINE 4

#define FG_BLACK 30
#define BG_BLACK 40
#define FG_RED 31
#define BG_RED 41
#define FG_GREEN 32
#define BG_GREEN 42
#define FG_YELLOW 33
#define BG_YELLOW 43
#define FG_BLUE 34
#define BG_BLUE 44
#define FG_MAGENTA 35
#define BG_MAGENTA 45
#define FG_CYAN 36
#define BG_CYAN 46
#define FG_WHITE 37
#define BG_WHITE 47
#define FG_DEFAULT 39
#define BG_DEFAULT 49
#define FG_BRIGHT_BLACK 90
#define BG_BRIGHT_BLACK 100
#define FG_BRIGHT_RED 91
#define BG_BRIGHT_RED 101
#define FG_BRIGHT_GREEN 92
#define BG_BRIGHT_GREEN 102
#define FG_BRIGHT_YELLOW 93
#define BG_BRIGHT_YELLOW 103
#define FG_BRIGHT_BLUE 94
#define BG_BRIGHT_BLUE 104
#define FG_BRIGHT_MAGENTA 95
#define BG_BRIGHT_MAGENTA 105
#define FG_BRIGHT_CYAN 96
#define BG_BRIGHT_CYAN 106
#define FG_BRIGHT_WHITE 97
#define BG_BRIGHT_WHITE 107
#define FG_RESET 0

// Erase functions
#define ERASE_CUR_TO_END 0
#define ERASE_CUR_TO_BEGINNING 1
#define ERASE_ENTIRE 2
#define ERASE_SAVED 3   // only for screen

// Move cursor
#define MOVE_HOME "H"
#define MOVE_UP "A"
#define MOVE_DOWN "B"
#define MOVE_RIGHT "C"
#define MOVE_LEFT "D"
#define MOVE_BEG_NEXT_LINE "E"
#define MOVE_BEG_PREV_LINE "F"
#define MOVE_COL "G"

// Cursor
#define CURSOR_VISIBLE "?25h"
#define CURSOR_INVISIBLE "?25l"

void apply(std::string c) {std::cout << c;}
// Change color
std::string make_color(int text_type = 0, int foreground = 1, int background = 1) {return std::string(ESC) + std::to_string(text_type) + ";" + std::to_string(foreground) + ";" + std::to_string(background) + "m";}
void apply_color(int text_type = 0, int foreground = 1, int background = 1) {apply(make_color(text_type, foreground, background));}
std::string reset_color() {return make_color(0, 0, 0);}
void apply_reset_color() {apply(reset_color());}
// > RGB colors
std::string rgb_color(bool foreground, int r, int g, int b) {return std::string(ESC) + (foreground ? "38" : "48") + ";2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";}
void apply_rgb_color(bool foreground, int r, int g, int b) {apply(rgb_color(foreground, r, g, b));}
// Erase
std::string erase(bool screen, int erase_code) {if (screen) return std::string(ESC) + std::to_string(erase_code) + "J";else return std::string(ESC) + std::to_string(erase_code) + "K";}
void apply_erase(bool screen, int erase_code) {apply(erase(screen, erase_code));}
// Move cursor
std::string exact_move(int line, int column) {return std::string(ESC) + std::to_string(line) + ";" + std::to_string(column);}
std::string move(int amount, std::string code) {return std::string(ESC) + std::to_string(amount) + code;}
void apply_exact_move(int line, int column) {apply(exact_move(line, column));}
void apply_move(int amount, std::string code) {apply(move(amount, code));}
// Make visible cursor - Should work on most platforms
std::string cursor(bool visible) {if (visible) return std::string(ESC) + std::string(CURSOR_VISIBLE);else return std::string(ESC) + std::string(CURSOR_INVISIBLE);}
void apply_cursor(bool visible) {apply(cursor(visible));}
// Window size stuff
std::tuple<int, int> window_size() {int width, height;get_terminal_size(width, height);return std::make_tuple(width, height);}
void print_X(int amount, std::string ind) {for (int i = 0; i < amount; ++i) {std::cout << ind;}}
void print_right_bound(std::string text) {auto [width, height] = window_size();int l = width - text.size();print_X(l, " ");std::cout << text << std::endl;}
void print_right_bound_ranged(std::string text, int range_width) {print_X(range_width - text.size(), " ");std::cout << text << std::endl;}
void print_center_bound(std::string text) {auto [width, height] = window_size();print_X(width / 2 - text.size(), " ");std::cout << text << std::endl;}
void print_center_bound_ranged(std::string text, int range_width) {print_X(range_width / 2 - text.size(), " ");std::cout << text << std::endl;}
void print_as_header(std::string text, int range) {int size = (range - (text.size() + 2)) / 2;print_X(size, "-");std::cout << " " << text << " ";print_X(size, "-");std::cout << std::endl;}
std::vector<std::string> split_string_by_newline(const std::string& str) {auto result = std::vector<std::string>{};auto ss = std::stringstream{str};for (std::string line; std::getline(ss, line, '\n');) result.push_back(line);return result;}
void lined_f(std::string text, void* func(std::string)) {for (std::string i : split_string_by_newline(text)) {func(i);}}

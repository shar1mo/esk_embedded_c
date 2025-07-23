#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

#define MAX_FILES 100
#define LEFT_PANEL 0
#define RIGHT_PANEL 1

typedef struct {
  char name[256];
  int is_dir;
  off_t size;
} FileEntry;

int active_panel = LEFT_PANEL;
FileEntry left_files[MAX_FILES], right_files[MAX_FILES];
int left_count = 0, right_count = 0;
int left_selected = 0, right_selected = 0;
char left_path[256] = "/.", right_path[256] = "/.";

void load_files(const char *path, FileEntry *files, int *count) {
  DIR *dir = opendir(path);
  struct dirent *entry;
  *count = 0;

  strcpy(files[*count].name, "/..");
  files[*count].is_dir = 1;
  files[*count].size = 0;
  (*count)++;

  if (dir) {
    while ((entry = readdir(dir)) != NULL && *count < MAX_FILES) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        strcpy(files[*count].name, entry->d_name);
        char fullpath[512];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        struct stat st;
        if (stat(fullpath, &st) == 0) {
          files[*count].is_dir = S_ISDIR(st.st_mode);
          files[*count].size = files[*count].is_dir ? 0 : st.st_size;
        } else {
          files[*count].is_dir = 0;
          files[*count].size = 0;
        }
        (*count)++;
      }
    }
    closedir(dir);
  }
}

void render_panel(WINDOW *panel, FileEntry *files, int count, int selected, int is_active) {
  werase(panel);
  box(panel, 0, 0);
  for (int i = 0; i < count; ++i) {
    if (i == selected && is_active) {
      wattron(panel, COLOR_PAIR(1));
      if (files[i].is_dir)
        mvwprintw(panel, i + 1, 2, "%-50s %20s", files[i].name, "DIR");
      else
        mvwprintw(panel, i + 1, 2, "%-50s %20lld", files[i].name, (long long)files[i].size);
      wattroff(panel, COLOR_PAIR(1));
    } else {
      if (files[i].is_dir)
        mvwprintw(panel, i + 1, 2, "%-50s %20s", files[i].name, "DIR");
      else
        mvwprintw(panel, i + 1, 2, "%-50s %20lld", files[i].name, (long long)files[i].size);
    }
  }
  wrefresh(panel);
}

void draw_status(WINDOW *right_panel) {
  int y = getmaxy(right_panel) - 2;
  int x = getmaxx(right_panel) - 48;
  mvwprintw(right_panel, y, x, "TAB - switch panel | ENTER - open | F10 - exit");
  wrefresh(right_panel);
}

void draw_selected_file(WINDOW *panel, FileEntry *files, int selected) {
  int y = getmaxy(panel) - 1;
  if (strcmp(files[selected].name, "/..") == 0) {
    mvwprintw(panel, y, 2, "selected: %s", "UP_DIR");
  } else {
    mvwprintw(panel, y, 2, "selected: %s", files[selected].name);
  }
  wrefresh(panel);
}

void sig_winch(int sign) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
  resizeterm(size.ws_row, size.ws_col);
}

int main(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(FALSE);
  
  struct winsize w_size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &w_size);
  
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_WHITE, COLOR_CYAN);
  
  WINDOW *main_win = newwin(w_size.ws_row, w_size.ws_col, 0, 0);
  wbkgd(main_win, COLOR_PAIR(1));
  wattron(main_win, COLOR_PAIR(1));

  int half_cols = w_size.ws_col / 2;
  WINDOW *left_panel = subwin(main_win, w_size.ws_row - 2, half_cols - 1, 1, 1);
  WINDOW *right_panel = subwin(main_win, w_size.ws_row - 2, half_cols - 1, 1, half_cols + 1);
  
  wbkgd(left_panel, COLOR_PAIR(2));
  wbkgd(right_panel, COLOR_PAIR(2));
  box(left_panel, 0, 0);
  box(right_panel, 0, 0);

  signal(SIGWINCH, sig_winch);

  load_files(left_path, left_files, &left_count);
  load_files(right_path, right_files, &right_count);

  render_panel(left_panel, left_files, left_count, left_selected, active_panel == LEFT_PANEL);
  render_panel(right_panel, right_files, right_count, right_selected, active_panel == RIGHT_PANEL);
  draw_selected_file(left_panel, left_files, left_selected);
  draw_selected_file(right_panel, right_files, right_selected);
  draw_status(right_panel);
  wrefresh(main_win);
  wrefresh(left_panel);
  wrefresh(right_panel);
  refresh();

  int press_key;
  while((press_key = getch()) != KEY_F(10)) {
    int *selected = (active_panel == LEFT_PANEL) ? &left_selected : &right_selected;
    int *count = (active_panel == LEFT_PANEL) ? &left_count : &right_count;
    FileEntry *files = (active_panel == LEFT_PANEL) ? left_files : right_files;
    char *path = (active_panel == LEFT_PANEL) ? left_path : right_path;
    switch (press_key) {
    case '\t':  
    case KEY_BTAB:
      active_panel = !active_panel;
      break;
    case KEY_UP:
      if (*selected > 0) (*selected)--;
      break;
    case KEY_DOWN:
      if (*selected < *count - 1) (*selected)++;
      break;
    case '\n':
    case KEY_ENTER:
      if (files[*selected].is_dir) {
        char new_path[512];
        if (strcmp(files[*selected].name, "/..") == 0) {
          char *slash = strrchr(path, '/');
          if (slash && slash != path) {
            size_t len = slash - path;
            if (len > sizeof(new_path) - 1) len = sizeof(new_path) - 1;
            strncpy(new_path, path, len);
            new_path[len] = '\0';
          } else {
            strncpy(new_path, "/", sizeof(new_path) - 1);
            new_path[sizeof(new_path) - 1] = '\0';
          }
        } else {
          if (strcmp(path, "/") == 0)
            snprintf(new_path, sizeof(new_path), "/%s", files[*selected].name);
          else
            snprintf(new_path, sizeof(new_path), "%s/%s", path, files[*selected].name);
        }
        if (active_panel == LEFT_PANEL) {
          strncpy(left_path, new_path, 255);
          left_path[255] = '\0';
          load_files(left_path, left_files, &left_count);
          left_selected = 0;
        } else {
          strncpy(right_path, new_path, 255);
          right_path[255] = '\0';
          load_files(right_path, right_files, &right_count);
          right_selected = 0;
        }
      }
      break;
    default:
      break;
    }
    render_panel(left_panel, left_files, left_count, left_selected, active_panel == LEFT_PANEL);
    render_panel(right_panel, right_files, right_count, right_selected, active_panel == RIGHT_PANEL);
    draw_selected_file(left_panel, left_files, left_selected);
    draw_selected_file(right_panel, right_files, right_selected);
    draw_status(right_panel);
    wrefresh(main_win);
    wrefresh(left_panel);
    wrefresh(right_panel);
    refresh();
  }
  
  delwin(main_win);
  delwin(left_panel);
  delwin(right_panel);
  endwin();
  exit(EXIT_SUCCESS);
}
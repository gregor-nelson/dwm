#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	const char *key;
	const char *desc;
} Bind;

typedef struct {
	const char *name;
	const Bind *binds;
	int count;
} Section;

static const Bind media[] = {
	{ "AudioRaise/Lower", "Volume up/down (1%)" },
	{ "AudioMute",        "Toggle mute" },
	{ "BrightnessUp",     "Brightness up (5%)" },
	{ "BrightnessDown",   "Brightness down (5%)" },
};

static const Bind launchers[] = {
	{ "Super+Return",  "Terminal (st)" },
	{ "Super+W",       "Chromium" },
	{ "Super+R",       "Thunar" },
	{ "Super+C",       "Rofi drun" },
	{ "Super+Ctrl+C",  "Rofi run" },
	{ "Super+Shift+C", "Rofi calc" },
};

static const Bind navigation[] = {
	{ "Super+J / K",      "Focus next/prev in stack" },
	{ "Super+Arrows",     "Focus directional" },
	{ "Super+Tab",        "Toggle last tag" },
	{ "Super+[1-5]",      "View tag" },
	{ "Super+Ctrl+[1-5]", "Toggle view tag" },
	{ "Super+0",          "View all tags" },
};

static const Bind windows[] = {
	{ "Super+Q",          "Kill window" },
	{ "Super+Shift+J/K",  "Move window in stack" },
	{ "Super+Shift+Space", "Toggle floating" },
	{ "Super+F",          "Toggle fullscreen" },
	{ "Super+E",          "Hide window" },
	{ "Super+Shift+E",    "Restore window" },
	{ "Super+Shift+[1-5]", "Send window to tag" },
	{ "Super+Shift+0",    "Tag on all workspaces" },
};

static const Bind master[] = {
	{ "Super+I / D",      "Inc/dec master count" },
	{ "Super+H / L",      "Shrink/grow master" },
	{ "Super+Shift+H/L",  "Inc/dec cfact" },
	{ "Super+Shift+O",    "Reset cfact" },
};

static const Bind layouts[] = {
	{ "Super+T",              "Tile          []=" },
	{ "Super+Shift+F",        "Monocle       [M]" },
	{ "Super+M",              "Spiral        [@]" },
	{ "Super+Ctrl+G",         "Gapless grid  :::" },
	{ "Super+Ctrl+Shift+T",   "Floating      ><>" },
	{ "Super+Space",          "Toggle last layout" },
	{ "Super+Ctrl+, / .",     "Cycle layout prev/next" },
};

static const Bind gaps[] = {
	{ "Super+Ctrl+I / D",     "Inc/dec all gaps" },
	{ "Super+Shift+I",        "Inc inner gaps" },
	{ "Super+Ctrl+Shift+I",   "Dec inner gaps" },
	{ "Super+Ctrl+O",         "Inc outer gaps" },
	{ "Super+Ctrl+Shift+O",   "Dec outer gaps" },
	{ "Super+Ctrl+T",         "Toggle gaps" },
	{ "Super+Ctrl+Shift+D",   "Reset gaps" },
};

static const Bind borders[] = {
	{ "Super+Shift+Minus", "Decrease border" },
	{ "Super+Shift+P",     "Increase border" },
	{ "Super+Shift+W",     "Reset border" },
};

static const Bind monitor[] = {
	{ "Super+, / .",       "Focus prev/next monitor" },
	{ "Super+Shift+, / .", "Send to prev/next monitor" },
};

static const Bind misc[] = {
	{ "Super+B",       "Toggle bar" },
	{ "Super+Ctrl+W",  "Toggle tab mode" },
	{ "Super+Ctrl+Q",  "Quit dwm" },
	{ "Super+Shift+R", "Restart dwm" },
};

#define SECDEF(name, arr) { name, arr, sizeof(arr)/sizeof(arr[0]) }

static const Section sections[] = {
	SECDEF("Media",       media),
	SECDEF("Launchers",   launchers),
	SECDEF("Navigation",  navigation),
	SECDEF("Windows",     windows),
	SECDEF("Master",      master),
	SECDEF("Layouts",     layouts),
	SECDEF("Gaps",        gaps),
	SECDEF("Border",      borders),
	SECDEF("Monitor",     monitor),
	SECDEF("Misc",        misc),
};

#define NSECTIONS (sizeof(sections)/sizeof(sections[0]))

static void draw_section(WINDOW *win, int y, int x, const Section *s, int selected, int width)
{
	if (selected)
		wattron(win, A_BOLD | COLOR_PAIR(2));
	else
		wattron(win, A_BOLD | COLOR_PAIR(1));

	mvwprintw(win, y, x, " %s ", s->name);

	if (selected)
		wattroff(win, A_BOLD | COLOR_PAIR(2));
	else
		wattroff(win, A_BOLD | COLOR_PAIR(1));
}

static void draw_binds(WINDOW *win, const Section *s, int starty, int width)
{
	int keywidth = 24;

	for (int i = 0; i < s->count; i++) {
		wattron(win, COLOR_PAIR(3) | A_BOLD);
		mvwprintw(win, starty + i, 4, "%-*s", keywidth, s->binds[i].key);
		wattroff(win, COLOR_PAIR(3) | A_BOLD);

		wattron(win, COLOR_PAIR(4));
		mvwprintw(win, starty + i, 4 + keywidth + 2, "%s", s->binds[i].desc);
		wattroff(win, COLOR_PAIR(4));
	}
}

int main(void)
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	use_default_colors();

	init_pair(1, COLOR_CYAN, -1);
	init_pair(2, COLOR_BLACK, COLOR_CYAN);
	init_pair(3, COLOR_GREEN, -1);
	init_pair(4, COLOR_WHITE, -1);
	init_pair(5, COLOR_YELLOW, -1);

	int sel = 0;
	int ch;

	for (;;) {
		int rows, cols;
		getmaxyx(stdscr, rows, cols);
		erase();

		/* title */
		attron(A_BOLD | COLOR_PAIR(1));
		mvprintw(0, (cols - 18) / 2, "dwm keybindings");
		attroff(A_BOLD | COLOR_PAIR(1));

		/* tabs */
		int tx = 1;
		for (int i = 0; i < (int)NSECTIONS; i++) {
			draw_section(stdscr, 2, tx, &sections[i], i == sel, cols);
			tx += strlen(sections[i].name) + 3;
			if (tx > cols - 12) {
				/* wrap not handled, just stop drawing */
				break;
			}
		}

		/* horizontal line */
		attron(COLOR_PAIR(1));
		mvhline(3, 0, ACS_HLINE, cols);
		attroff(COLOR_PAIR(1));

		/* binds */
		draw_binds(stdscr, &sections[sel], 5, cols);

		/* footer */
		attron(COLOR_PAIR(5));
		mvprintw(rows - 1, 2, " <-/-> navigate   q quit ");
		attroff(COLOR_PAIR(5));

		refresh();
		ch = getch();

		if (ch == 'q' || ch == 27)
			break;
		else if (ch == KEY_RIGHT || ch == 'l' || ch == '\t')
			sel = (sel + 1) % NSECTIONS;
		else if (ch == KEY_LEFT || ch == 'h')
			sel = (sel - 1 + NSECTIONS) % NSECTIONS;
	}

	endwin();
	return 0;
}

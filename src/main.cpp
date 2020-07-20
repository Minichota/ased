#include <ncurses.h>
#include <iostream>
#include <vector>

enum ViewMode
{
	INSERT,
	MOVE,
};

struct stream
{
	typedef std::string data_line;
	typedef std::vector<data_line> file;
	file data;
};

static stream s;

struct view
{
	// the position the view is currently looking at
	size_t curr_begin;

	// positioning
	uint16_t vx = 0, vy = 0; /*  vpos  */
	int16_t cx = 0, cy = 0; /* cursor */
	int16_t sx = 0, sy = 0; /* scroll */

	ViewMode mode = MOVE;

	char get_char() {
		return s.data[curr_begin+cy][cx];
	};
};

int main()
{
	// init
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);

	refresh();

	// View
	s.data = {
		"__FILE__1",
		"This is a test file that is not a file to avoid IO for now.",
		"Should work with multiple lines you dum dum.",
		"Test driven developement am I right chat?",
		"",
		"__FILE__2",
		"This is a test file that is not a file to avoid IO for now.",
		"Should work with multiple lines you dum dum.",
		"Test driven developement am I right chat?",
	};
	view v =
	{
		1
	};

	// initial print data
	for(size_t i = v.curr_begin; i < s.data.size(); i++)
	{
		mvprintw(i - v.curr_begin, 0, s.data[i].c_str());
	}
	move(v.cy, v.cx);
	refresh();

	int c;
	while((c = getch()) != 27)
	{
		switch(c)
		{
			default:
				switch(v.mode)
				{
					case INSERT:
						switch(c)
						{
							case 9: // tab
								v.mode = MOVE;
								break;
							case 263:
								if(v.cx != 0)
								{
									v.cx--;
									s.data[v.curr_begin+v.cy+v.sy].erase(v.cx, 1);
									mvdelch(v.cy, v.cx);
								}
								break;
							default:
								while((int16_t)s.data.size() < v.cy + v.sy)
									s.data.emplace_back();
								s.data[v.curr_begin+v.cy+v.sy].insert(v.cx, 1, c);
								v.cx++;
								break;
						}
						break;
					case MOVE:
						switch(c)
						{
							// x
							case 'l':
								v.cx++;
								break;
							case 'h':
								v.cx--;
								v.cx = std::max(v.cx, (int16_t)0);
								break;
							// y
							case 'j':
								v.cy++;
								if(v.cy >= LINES)
								{
									v.sy++;
									v.cy = LINES - 1;
									clear();
									if((int16_t)v.curr_begin + v.sy >= LINES)
									{
										v.sy--;
									}
								}
								break;
							case 'k':
								v.cy--;
								if(v.cy < 0)
								{
									v.sy--;
									v.cy = 0;
									clear();
									if((int16_t)v.curr_begin + v.sy < 0)
									{
										v.sy++;
									}
								}
								break;
							case 9: // tab
								while((int16_t)s.data.size() < v.cy)
									s.data.emplace_back();
								v.mode = INSERT;
								v.cx = std::min(v.cx, (int16_t)s.data[v.curr_begin+v.cy+v.sy].size());
								break;
						}
						break;
				}
				break;
		}
		for(size_t i = (int16_t)v.curr_begin + v.sy; i < s.data.size(); i++)
		{
			mvprintw((int16_t)i - (int16_t)v.curr_begin - v.sy, 0, s.data[i].c_str());
		}
		move(v.cy, v.cx);
		refresh();
	}

	// end
	endwin();

	return 0;
}

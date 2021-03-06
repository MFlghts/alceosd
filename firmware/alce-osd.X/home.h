/*
    AlceOSD - Graphical OSD
    Copyright (C) 2015  Luis Alves

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HOME_H
#define	HOME_H

enum {
    HOME_NONE = 0,
    HOME_WAIT,
    HOME_GOT,
    HOME_LOCKED,
    HOME_RESET,
    HOME_FORCE,
};

struct home_data {
    int altitude;
    int direction, uav_bearing;
    float distance;

    unsigned char lock;
};

void init_home(void);
struct home_data* get_home_data(void);
void shell_cmd_home(char *args, void *data);

#endif	/* HOME_H */


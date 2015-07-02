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

#include "alce-osd.h"

#define X_SIZE  64
#define Y_SIZE  45

#define BAT_BAR_X   3
#define BAT_BAR_Y   0
#define BAT_BAR_W   58
#define BAT_BAR_H   11


struct widget_priv {
    float bat_voltage, bat_current;
    int bat_remaining;
};


static void mav_callback(mavlink_message_t *msg, mavlink_status_t *status, void *data)
{
    struct widget *w = (struct widget*) data;
    struct widget_priv *priv = (struct widget_priv*) w->priv;
    
    priv->bat_voltage = mavlink_msg_sys_status_get_voltage_battery(msg) / 1000.0;
    priv->bat_current = mavlink_msg_sys_status_get_current_battery(msg) / 100.0;
    priv->bat_remaining = (int) mavlink_msg_sys_status_get_battery_remaining(msg);

    schedule_widget(w);
}


static int init(struct widget *w)
{
    struct widget_priv *priv;

    priv = (struct widget_priv*) widget_malloc(sizeof(struct widget_priv));
    if (priv == NULL)
        return -1;
    w->priv = priv;

    add_mavlink_callback(MAVLINK_MSG_ID_SYS_STATUS, mav_callback, CALLBACK_WIDGET, w);
    w->cfg->w = X_SIZE;
    w->cfg->h = Y_SIZE;
    return 0;
}


static void render(struct widget *w)
{
    struct widget_priv *priv = (struct widget_priv*) w->priv;
    struct canvas *ca = &w->ca;
    char buf[10];
    int i;

    sprintf(buf, "%5.2fV", priv->bat_voltage);
    draw_str(buf, 4, BAT_BAR_Y + BAT_BAR_H + 3, ca, 2);
    sprintf(buf, "%5.2fA", priv->bat_current);
    draw_str(buf, 4, BAT_BAR_Y + BAT_BAR_H + 17, ca, 2);

    draw_rect(BAT_BAR_X, BAT_BAR_Y, BAT_BAR_X + BAT_BAR_W, BAT_BAR_Y + BAT_BAR_H, 3, ca);
    draw_rect(BAT_BAR_X+1, BAT_BAR_Y+1, BAT_BAR_X + BAT_BAR_W-1, BAT_BAR_Y + BAT_BAR_H-1, 1, ca);
    draw_vline(BAT_BAR_X - 1, BAT_BAR_Y + 4, BAT_BAR_Y + BAT_BAR_H - 4, 1, ca);
    draw_vline(BAT_BAR_X - 2, BAT_BAR_Y + 4, BAT_BAR_Y + BAT_BAR_H - 4, 1, ca);
    draw_vline(BAT_BAR_X - 3, BAT_BAR_Y + 4, BAT_BAR_Y + BAT_BAR_H - 4, 3, ca);

    for (i = 0; i < (priv->bat_remaining*BAT_BAR_W)/100 - 2; i++) {
        draw_vline(BAT_BAR_X + 2 + i, BAT_BAR_Y + 2, BAT_BAR_Y + BAT_BAR_H - 2, 2, ca);
    }

    sprintf(buf, "%d", priv->bat_remaining);
    draw_str(buf, BAT_BAR_X + BAT_BAR_W/2 - 6, BAT_BAR_Y + 3, ca, 0);
}


const struct widget_ops bat_info_widget_ops = {
    .name = "Battery info",
    .id = WIDGET_BATTERY_INFO_ID,
    .init = init,
    .render = render,
};

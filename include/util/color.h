/*****************************************
* @author: yifan
* @data: 2023/10/27 11:34
* @description: None
******************************************/

#ifndef SPINR_COLOR_H
#define SPINR_COLOR_H

#include "QColor"
#include "QPen"
#include "QBrush"


/**
 * defines colors and scatter size for plotting
 */
#define SCATTER_COLOR_0 	"#CD3333"
#define LINE_COLOR_0 "#CD3333"
#define SCATTER_COLOR_1 "#2E8B57"
#define LINE_COLOR_1 "#2E8B57"
#define SCATTER_SIZE 6
#define LINE_WIDTH 1.5

#define TITLE_SIZE 16
#define TITLE_FAMILY "Times New Roman"
#define TITLE_BOLD false

#define LEGEND_FAMILY "Times New Roman"
#define LEGEND_SIZE 14

#define AXIS_LABEL_SIZE 14
#define AXIS_LABEL_FONT_FAMILY "Times New Roman"
#define AXIS_LABEL_FONT_BOLD false
#define AXIS_TICK_LABEL_FONT_FAMILY "Times New Roman"
#define AXIS_TICK_LABEL_SIZE 12


#define SCATTER_COLOR(index) SCATTER_COLOR_#index

#define InfoColor "#6495ED"


#endif //SPINR_COLOR_H

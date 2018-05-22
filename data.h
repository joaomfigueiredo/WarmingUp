#ifndef DATA_H
#define DATA_H

#define TEXTUAL 1
#define GRAPHICAL 2
#define BUFFER_SIZE 200
#define FILENAME_SIZE 50
#define ERRORCODE 20180502

#define TABLE_SIZE 600     // main game space size
#define LEFT_BAR_SIZE 200    // left white bar size
#define WINDOW_POSX 20      // initial position of the window: x
#define WINDOW_POSY 10       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size
#define MARGIN 1
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX_BOARD_POS 15
#define M_PI 3.14159
#define STRING_SIZE 100       // max size for some strings
#define MAX_COLORS 5

#define GLOBAL 1
#define PER_COUNTRY 2
#define PER_CITY 2

#define COUNTRIES 0
#define CITIES 1

#define MAX_WORD 100
#define MAX_STR 1000

#define ANSI_COLOR_ERRORS "\033[1;91m"
#define ANSI_COLOR_WARNINGS "\033[1;93m"
#define ANSI_COLOR_BOLD_RED "\033[1;31m"
#define ANSI_COLOR_BOLD_GREEN "\033[1;32m"
#define ANSI_COLOR_BOLD_YELLOW "\033[1;33m"
#define ANSI_COLOR_BOLD_BLUE "\033[1;34m"
#define ANSI_COLOR_BOLD_MAGENTA "\033[1;35m"
#define ANSI_COLOR_BOLD_CYAN "\033[1;36m"
#define ANSI_COLOR_BOLD_WHITE "\033[1;37m"
#define ANSI_COLOR_RESET	"\x1b[0m"


typedef struct date{
	int day;
	int month;
	int year;
} date_t;

typedef struct geo_coord{
	float angle;
	char hemisphere;
}geo_coord_t;

typedef struct data_temp{
	int ordering_identifier;
	date_t dt;
	float temperature;
	float uncertainty;
	char country[100];
	char city[100];
	geo_coord_t lat;
	geo_coord_t longit;
} data_temp_t;

typedef struct temp_hist{
	int begin_period;
	int end_period;
	int average;
	int num_of_val;
	float maximum_temp;
	float minimum_temp;
} temp_hist_t;

typedef struct node{
	data_temp_t payload;
	struct node *next;
	struct node *prev;
}node_t;

typedef struct tree_node{
	data_temp_t payload;
	struct tree_node *right;
	struct tree_node *left;
}tree_node_t;

typedef struct node_th{
	temp_hist_t payload;
	struct node_th *next;
	struct node_th *prev;
}node_th_t;

typedef struct list{
	node_t* head;
	node_t* tail;
	tree_node_t* root;
}list_t;

typedef struct list_th{
	node_th_t* head;
	node_th_t* tail;
}list_th_t;

#endif

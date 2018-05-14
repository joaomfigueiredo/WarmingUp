#ifndef DATA_H
#define DATA_H

#define TEXTUAL 1
#define GRAPHICAL 2
#define BUFFER_SIZE 200
#define FILENAME_SIZE 50

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
	int hemisphere;
}geo_coord_t;

typedef struct data_temp{
	int concatenated_date;
	date_t dt;
	float temperature;
	float uncertainty;
	char country[100];
	char city[100];
	geo_coord_t lat;
	geo_coord_t longit;
} data_temp_t;

typedef struct node{
	data_temp_t payload;
	struct node *next;
	struct node *prev;
}node_t;

typedef struct list{
	node_t* head;
	node_t* tail;
}list_t;


#endif

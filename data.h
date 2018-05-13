#ifndef DATA_H
#define DATA_H

#define TEXTUAL 1
#define GRAPHICAL 2
#define BUFFER_SIZE 200
#define ANSI_COLOR_BOLD_RED "\033[1;31m"
#define ANSI_COLOR_BOLD_GREEN "\033[1;32m"
#define ANSI_COLOR_BOLD_YELLOW "\033[1;33m"
#define ANSI_COLOR_BOLD_BLUE "\033[1;34m"
#define ANSI_COLOR_BOLD_MAGENTA "\033[1;35m"
#define ANSI_COLOR_BOLD_CYAN "\033[1;36m"
#define ANSI_COLOR_RESET	"\x1b[0m"


typedef struct{
	int day;
	int month;
	int year;
} date;

typedef struct{
	float angle;
	int hemisphere;
}geo_coord;

typedef struct{
	date dt;
	float temperature;
	float uncertainty;
	char country[100];
	char city[100];
	geo_coord lat;
	geo_coord longit;
} data_temp;

typedef struct node{
	data_temp payload;
	struct node *next;
	struct node *prev;
}node_t;

#endif

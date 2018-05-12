#ifndef DATA_H
#define DATA_H

#define TEXTUAL 1
#define GRAPHICAL 2


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

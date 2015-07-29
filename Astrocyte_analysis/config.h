/*
config.h - файл конфигурации приложения
*/

#pragma once

#include "declaration.h"

// form parameters
#define FORM_WIDTH  200
#define FORM_HEIGHT 100
#define BUTTON_OPEN nana::rectangle { 10, 10, 50, 20 }
#define BUTTON_CLUSTERS nana::rectangle { 70, 10, 50, 20 }


// segmentation parameters
#define SQUARE_SIDE 5
#define MIN_CLUSTER_POINTS 3
#define NUM_NEIGBOR_POINTS 5
#define AREA_THRESHOLD 0.5
#define TIME_THRESHOLD 0.5

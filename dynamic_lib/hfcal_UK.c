#include <hfcal.h>
#include <stdio.h>

void display_calories(float weight, float distance, float coefff) {
  printf("Weight:%3.2f kgs\n", weight / 2.2046);
  printf("Distance:%3.2f km\n", distance * 1.60334);
  printf("Calories burned:%4.2f cal\n", coefff * weight * distance);
}
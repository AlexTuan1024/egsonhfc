#include <hfcal.h>
#include <stdio.h>

void display_calories(float weight, float distance, float coefff) {
  printf("Weight:%3.2f lbs\n", weight);
  printf("Distance:%3.2f miles\n", distance);
  printf("Calories burned:%4.2f cal\n", coefff * weight * distance);
}
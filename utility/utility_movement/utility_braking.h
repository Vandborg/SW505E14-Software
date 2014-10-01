#ifndef UTILITY_BRAKING_H_   /* Include guard */
#define UTILITY_BRAKING_H_

void forklift_brake(int breakpower, int forklift_speed);
void fullstop_brake(void);
void fading_brake(int breakpower, int forklift_speed);
void slow_brake(int breakpower, int forklift_speed);

#endif

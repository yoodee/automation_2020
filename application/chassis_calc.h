#ifndef __CHASSIS_CALC_H__
#define __CHASSIS_CALC_H__

#ifdef CHASSIS_CALC_H_GLOBAL
  #define CHASSIS_CALC_H_EXTERN 
#else
  #define CHASSIS_CALC_H_EXTERN extern
#endif

#include "chassis_task.h"

#define LOW_CONSTANT_SPEED 500 //speed of low state
#define MEDIUM_CONSTANT_SPEED 700 //speed of medium state
#define HIGH_CONSTANT_SPEED 1000  //speed of high state

typedef enum chassis_state_name 
{
  LOW_MODE, // Restore energy
  MEDIUM_MODE, // Close to limitation
  BOOST_MODE // Beyond limitation
} chassis_state_name_t;

//enemy static state detection from tx2
typedef enum cv_static_event {
  ENEMY_NOT_DETECTED,  //no enemy detected
  ENEMY_DETECTED_LEFT,  //enemy detected on the left
  ENEMY_DETECTED_RIGHT   //enemy detected on the right
} cv_static_event_t;

//enemy dynamic state detection from tx2
typedef enum cv_dynamic_event {
  ENEMY_STAY_STILL,  //enemy has no tendency to move
  ENEMY_LEFT_TO_RIGHT,  //enemy moves from left to right
  ENEMY_RIGHT_TO_LEFT  //enemy moves from right to left
} cv_dynamic_event_t;

//power information from pc
typedef enum power_event {
  POWER_NORMAL, //no or little use of buffer power
  POWER_IN_DEBT,  //noticeable use of buffer power
  POWER_IN_SERIOUS_DEBT //considerable use of buffer power
} power_event_t;

typedef enum armor_event_name {
  NO_HIT_FOR_X_SEC, //armor has not been hit for at least x seconds
  HIT_WITHIN_X_SEC //armor has been hit within x seconds
} armor_event_name_t;

//armor information from pc
typedef struct armor_event {
  armor_event_name_t armor0_state;
  armor_event_name_t armor1_state;
  uint32_t armor0_last_update_time;
  uint32_t armor1_last_update_time;
  uint32_t interval_TH; // "X" value for not being hit for X second
} armor_event_t;

typedef struct chassis_state_t {
  chassis_state_name_t state_name;
  float constant_spd;
} chassis_state_t;

typedef struct chassis_movement {
  int spd_ind; // speed indicator, 1 or -1
  int duration; // movement duration (ms)
  uint32_t start_time; // movement start time (ticks)
} chassis_movement_t;

typedef struct duration_settings {
  int floor; // movement duration floor, default 500ms
  int ceiling; // movement duration ceiling, default 2500ms
} duration_settings_t;

typedef struct bounded_movement_settings {
  int activated; // if this is true, random movement will fall in a certain range
  float left_position;
  float right_position;
} bounded_movement_settings_t;

typedef struct middle_dodge_settings {
  int activated;
  int loc_register;
  uint32_t start_time;
} middle_dodge_settings_t;

float chassis_random_movement(chassis_t pchassis, float speed);
float chassis_patrol_movement(chassis_t pchassis, float speed);
void generate_random_movement(void);
void generate_patrol_movement(void); 
void set_duration(int new_duration_floor, int new_duration_ceiling);
void activate_bounded_movement(int range);
void deactivate_bounded_movement(void);
void adjust_accumulated_distance(float adjust_vy);
void update_chassis_event(power_event_t *power_eve, armor_event_t * armor_eve);

void set_state(chassis_state_t * state, chassis_state_name_t dest_state);

float get_spd(const chassis_state_t * state);

#endif
// #include "sdl_wrapper.h"
// #include "body.h"
// #include "list.h"
// #include "vector.h"
// #include "forces.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <math.h>
// #include <assert.h>
// #include <string.h>
//
// ////////////////////////////////////////
// // frame bounds, should be 4x:3y
// const vector_t MIN = (vector_t){-100.0, -75.0};
// const vector_t MAX = (vector_t){100.0, 75.0};
// // acceleration in units/sec^2 (used for both x and y direction)
// const double ACCEL = .05;
// // base parameters of player, dots. mass = 0 bc no gravity here
// const double RADIUS_PLAYER = 5;
// const double RADIUS_ENEMY = 8;
// const double RADIUS_BULLET = 1;
// const double ENEMY_DISTANCE = RADIUS_ENEMY / 2;
// const double BUFFER_TOP = 3 * RADIUS_ENEMY / 2;
// const rgb_color_t GREEN = {0, 128, 0};
// const rgb_color_t GREY = {128, 128, 128};
// const double SHIP_ANGLE = 4 * M_PI / 3;
// const double PLAYER_VEL = 1000;
// const double ENEMY_VEL = 50;
// const double BULLET_VEL = 500;
// const int PLAYER_SIDES = 20;
// const int ENEMY_SIDES = 20;
// const int BULLET_SIDES = 20;
// //number of enemy columns
// const int ENEMY_COLS = 8;
// //number of enemy rows
// const int ENEMY_ROWS = 3;
// const int NUM_ENEM = ENEMY_COLS * ENEMY_ROWS;
// const int SHOT_FREQ = 500;
// const double MASS = 0;
// const double MULT = 1;
// const char *PLAYER = "player";
// const char *ENEMY = "enemy";
// const char *PLAYER_BULLET = "player_bullet";
// const char *ENEMY_BULLET = "enemy_bullet";
// // body info = true means enemy, false ow
//
// void end_game(scene_t * scene){
//   scene_free(scene);
//   exit(0);
// }
//
// // creates player at the bottom center of the screen
// body_t *make_player(){
//   list_t *points = list_init(PLAYER_SIDES + 1, vec_free);
//   vector_t general_pt = (vector_t){RADIUS_PLAYER, 0};
//   double rot = (2 * M_PI) / PLAYER_SIDES;
//   for(size_t i = 0; i < PLAYER_SIDES; i++){
//     vector_t *temp = malloc(sizeof(vector_t));
//     assert(temp != NULL);
//     *temp = vec_rotate(general_pt, (rot * i));
//     list_add(points, temp);
//   }
//   // char *info_name = malloc(10 * sizeof(char));
//   // strcpy(info_name, "player");
//
//   char *player_name = malloc(10*sizeof(char));
//   strcpy(player_name, PLAYER);
//   body_t *player = body_init_info(points, MASS, GREEN, player_name);
//   body_set_centroid(player, (vector_t){0, MIN.y + RADIUS_PLAYER});
//   return player;
// }
//
// // creates enemy w/o ref to position--set later
// body_t *make_enemy(){
//   vector_t *center = malloc(sizeof(vector_t));
//   assert(center != NULL);
//   *center = VEC_ZERO;
//   list_t *points = list_init(ENEMY_SIDES + 1, vec_free);
//   vector_t first_pt = (vector_t){0, -RADIUS_ENEMY};
//   double rotate = ((2 * M_PI) - SHIP_ANGLE) / ENEMY_SIDES;
//   list_add(points, center);
//   double ship_angle = SHIP_ANGLE / 2;
//   for(size_t i = 0; i < ENEMY_SIDES; i++){
//     vector_t *new_pt = malloc(sizeof(vector_t));
//     assert(new_pt != NULL);
//     *new_pt = vec_rotate(first_pt, ship_angle + (rotate * i));
//     list_add(points, new_pt);
//   }
//   char *enemy_name = malloc(10*sizeof(char));
//   strcpy(enemy_name, ENEMY);
//   body_t *enemy = body_init_info(points, MASS, GREY, enemy_name);
//   vector_t pos = (vector_t){-25+RADIUS_ENEMY, -75+RADIUS_ENEMY};
//   body_set_centroid(enemy, pos);
//   return enemy;
// }
//
// // creates a bullet at center of screen
// body_t *make_bullet(rgb_color_t color, char *body_info){
//   list_t *points = list_init(BULLET_SIDES + 1, vec_free);
//   vector_t general_pt = (vector_t){0, RADIUS_BULLET};
//   double rot = (2 * M_PI) / BULLET_SIDES;
//   for(size_t i = 0; i < BULLET_SIDES; i++){
//     vector_t *temp = malloc(sizeof(vector_t));
//     assert(temp != NULL);
//     *temp = vec_rotate(general_pt, (rot * i));
//     list_add(points, temp);
//   }
//   body_t *bullet = body_init_info(points, MASS, color, body_info);
//   return bullet;
// }
//
// // makes each enemy at center, moves it, and adds to scene
// void set_enemies(scene_t *scene){
//   for(int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++){
//     body_t *enemy = make_enemy();
//     double x_coor = (MIN.x + ((i + 1) * (ENEMY_DISTANCE + (2 * RADIUS_ENEMY)))) -
//             ((i / ENEMY_COLS)*(ENEMY_COLS * (ENEMY_DISTANCE + (2 * RADIUS_ENEMY))));
//     double y_coor = MAX.y - (((i / ENEMY_COLS) + 1) * (ENEMY_DISTANCE + RADIUS_ENEMY));
//     body_set_centroid(enemy, (vector_t){x_coor, y_coor});
//     body_set_velocity(enemy, (vector_t){0, 0});
//     scene_add_body(scene, enemy);
//   }
// }
//
// // shifts enemy down to an open row when it goes OOB
// void wrap(scene_t *scene){
//   for(size_t i = 0; i < scene_bodies(scene); i++){
//     body_t *enemy = scene_get_body(scene, i);
//     if(strcmp((char *)body_get_info(enemy), "enemy") == 0){
//       list_t *points = body_get_shape(enemy);
//       vector_t center = body_get_centroid(enemy);
//       double down_dist = ENEMY_ROWS * (ENEMY_DISTANCE + RADIUS_ENEMY);
//       if((center.x - RADIUS_ENEMY) < MIN.x){
//         body_set_centroid(enemy, (vector_t){MIN.x + RADIUS_ENEMY , center.y - down_dist});
//         body_set_velocity(enemy, (vector_t){ENEMY_VEL, 0});
//       }
//       if((center.x + RADIUS_ENEMY) > MAX.x){
//         body_set_centroid(enemy, (vector_t){MAX.x - RADIUS_ENEMY, center.y - down_dist});
//         body_set_velocity(enemy, (vector_t){-ENEMY_VEL, 0});
//       }
//     }
//   }
// }
//
// // makes and adds bullet with initial velocity at the player
// void player_shoot(scene_t *scene){
//   printf("inplayer\n");
//   // get player
//   body_t *player = NULL;
//   for(size_t i = 0; i < scene_bodies(scene); i++){
//     body_t *body = scene_get_body(scene, i);
//     if(strcmp((char *)body_get_info(body), PLAYER) == 0){
//       player = body;
//       break;
//     }
//   }
//   assert(player != NULL);
//   vector_t player_center = body_get_centroid(player);
//   // make bullet @ centroid
//   body_t *player_bullet = make_bullet(GREEN, "player_bullet");
//   body_set_centroid(player_bullet, (vector_t){player_center.x, player_center.y + RADIUS_PLAYER + RADIUS_BULLET});
//   body_set_velocity(player_bullet, (vector_t){0, BULLET_VEL});
//   scene_add_body(scene, player_bullet);
//   printf("%d\n", scene_bodies(scene));
// }
//
// // randomly chooses when and which enemy to shoot bullet
// void enemy_shoot(scene_t *scene){
//   // generate probability that enemy will shoot
//   int rand_num = (rand() % (SHOT_FREQ - 1 + 1)) + 1;
//   printf("godzilla\n");
//   if((SHOT_FREQ % rand_num) == 0){
//     printf("love the way you lie\n");
//     int enem_inds[NUM_ENEM];
//     int counter = 0;
//     // list_t *total_enemies = list_init(scene_bodies(scene), (free_func_t)free_nothing); //
//     for(size_t i = 0; i < scene_bodies(scene); i++){
//       printf("looooop\n");
//       body_t *body = scene_get_body(scene, i);
//       if(strcmp((char *)body_get_info(body), "enemy") == 0){
//         enem_inds[counter] = i;
//         counter++;
//         printf("iffy\n");
//         // list_add(total_enemies, body);
//       }
//     }
//     printf("enemy_shoot2\n");
//     size_t rand_enemy = (size_t)(rand() % (NUM_ENEM - 1));
//     // body_t *enemy = list_get(total_enemies, 0);
//     body_t *enemy = scene_get_body(scene, enem_inds[rand_enemy]);
//     vector_t enemy_center = body_get_centroid(enemy);
//     body_t *enemy_bullet = make_bullet(GREY, PLAYER_BULLET);
//     body_set_centroid(enemy_bullet, (vector_t){enemy_center.x, enemy_center.y});
//     body_set_velocity(enemy_bullet, (vector_t){0, -BULLET_VEL});
//     scene_add_body(scene, enemy_bullet);
//     // list_free(total_enemies);
//   }
// }
//
// // checks enemy bullets; if enemy_bullet contacts player, both are removed and game ends
// // might need to add game over message
// void remove_player(scene_t *scene){
//   body_t *enemy_bullet;
//   body_t *player;
//   size_t ind_eb = 0;
//   size_t ind_player = 0;
//   char *eb_name = malloc(20*sizeof(char));
//   strcpy(eb_name, ENEMY_BULLET);
//   char *p_name = malloc(10*sizeof(char));
//   strcpy(p_name, PLAYER);
//   for(size_t i = 0; i < scene_bodies(scene); i++){
//     body_t *body = scene_get_body(scene, i);
//     if(strcmp((char *)body_get_info(body), eb_name) == 0){
//       enemy_bullet = body;
//       ind_eb = i;
//     }
//     if(strcmp((char *)body_get_info(body), p_name) == 0){
//       player = body;
//       ind_player = i;
//     }
//   }
//   if(enemy_bullet != NULL && player != NULL){
//     if(body_is_colliding(enemy_bullet, player)){
//       scene_remove_body(scene, ind_eb);
//       scene_remove_body(scene, ind_player);
//       exit(2);
//     }
//   }
// }
//
// // check all enemies; if player_bullet contacts enemy, both are removed
// void remove_enemy(scene_t *scene){
//   body_t *player_bullet;
//   size_t ind_pb = 0;
//   char *pb_name = malloc(20*sizeof(char));
//   strcpy(pb_name, PLAYER_BULLET);
//   char *e_name = malloc(10*sizeof(char));
//   strcpy(e_name, ENEMY);
//   for(size_t i = 0; i < scene_bodies(scene); i++){
//     body_t *temp = scene_get_body(scene, i);
//     if(strcmp((char *)body_get_info(temp), pb_name) == 0){
//       player_bullet = temp;
//       ind_pb = i;
//     }
//   }
//   size_t x = 0;
//   while(x < scene_bodies(scene)){
//     body_t *enemy = scene_get_body(scene, x);
//     if(enemy != NULL && player_bullet != NULL){
//       if((strcmp((char *)body_get_info(enemy), e_name) == 0) && body_is_colliding(enemy, player_bullet)){
//          // scene_remove_body(scene, ind_pb);
//          // scene_remove_body(scene, x);
//          break;
//       }
//       else{
//         x++;
//       }
//     }
//     else{
//       x++;
//     }
//   }
// }
//
// // check if enemy reaches the player and ends game if they collide--could also do based on final line? can't shoot anymore
// void reached_end(scene_t *scene){
//    body_t *enemy;
//    body_t *player;
//    size_t ind_enemy = 0;
//    size_t ind_player = 0;
//    char *e_name = malloc(20*sizeof(char));
//    strcpy(e_name, ENEMY);
//    char *p_name = malloc(10*sizeof(char));
//    strcpy(p_name, PLAYER);
//    for(size_t i = 0; i < scene_bodies(scene); i++){
//      body_t *body = scene_get_body(scene, i);
//      if(strcmp((char *)body_get_info(body), p_name) == 0){
//        player = body;
//        ind_player = i;
//        break;
//      }
//    }
//    for(size_t i = 0; i < scene_bodies(scene); i++){
//      body_t *body = scene_get_body(scene, i);
//      if(strcmp((char *)body_get_info(body), e_name) == 0){
//        enemy = body;
//        ind_enemy = i;
//        if(player != NULL && enemy != NULL){
//          if(body_is_colliding(enemy, player)){
//            scene_remove_body(scene, ind_enemy);
//            scene_remove_body(scene, ind_player);
//            end_game(scene);
//          }
//        }
//      }
//    }
// }
//
//
// // control player in accordance with key presses
// void move_player(scene_t *scene, char key, key_event_type_t type){
//   body_t *player = scene_get_body(scene, 0);
//   vector_t player_center = body_get_centroid(player);
//   if (type == KEY_PRESSED) {
//     switch(key) {
//       case LEFT_ARROW:
//         if(player_center.x > MIN.x + RADIUS_PLAYER){
//           // body_set_velocity(player, (vector_t){-INIT_VEL, 0});
//           body_set_velocity(player, (vector_t){-PLAYER_VEL, 0});
//         }
//         break;
//       case RIGHT_ARROW:
//         if(player_center.x < MAX.x - RADIUS_PLAYER){
//           // body_set_velocity(player, (vector_t){INIT_VEL, 0});
//           body_set_velocity(player, (vector_t){PLAYER_VEL, 0});
//         }
//         break;
//       case ' ':
//         player_shoot(scene);
//         break;
//       case 'q':
//         end_game(scene);
//     }
//   } else{
//     body_set_velocity(player, (vector_t){0, 0});
//   }
// }
//
// int main(){
//   sdl_init(MIN, MAX);
//   scene_t *scene = scene_init();
//   set_bounds(scene, MIN, MAX);
//   body_t *player = make_player();
//   scene_add_body(scene, player);
//   body_t *enemy = make_enemy();
//   scene_add_body(scene, enemy);
//   sdl_on_key(move_player);
//   while (!sdl_is_done(scene)){
//     double dt = time_since_last_tick();
//     if(body_is_colliding(scene_get_body(scene, 0), scene_get_body(scene, 1)))
//       printf("deteect\n");
//     scene_tick(scene, dt);
//     sdl_render_scene(scene);
//   }
//   end_game(scene);
// }

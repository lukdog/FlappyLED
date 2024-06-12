#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "Modulino.h"
#include <Scheduler.h>
#include "flappy_animation.h"
#include "crash_animation.h"
#include "modes_animation.h"
#include "intro_animation.h"

//#define DEBUG 1

/* Defines  ----------------------------------------------------------*/
#define HEIGHT            8
#define WIDTH             12
#define PLAYER_X          1  //initial player position
#define PLAYER_Y          4

#define MAX_TOF_H 400
#define TOF_PRESENCE_TIME 2000

/* Enable Features */
//#define BUTTONS 1
#define ENCODER_MODE 1
#define TOF_MODE 1
#define ANIMATIONS 1

/* Game Modes */
#define MENU 0
#define ENCODER 1
#define TOF 2

/* Prototypes ----------------------------------------------------------*/
void introduction_message(void);
void adapt_game_level(void);
void print_score(uint16_t game_score);
void reset_global_variables(void);

/* Objects */
ArduinoLEDMatrix matrix;
ModulinoKnob encoder;
ModulinoDistance distanceSensor;
ModulinoButtons buttons;

/* Global Variables */
int game_ongoing = 0;
unsigned long t_oldDistance = 0;
static byte wall_move = false;
static int8_t player_move = 0;
static uint8_t player_x = PLAYER_X, player_y = PLAYER_Y;
static int8_t wall_start_pix = 0, wall_pos_x = WIDTH-1, wall_size = 3;
static uint16_t score = 0, neai_ptr = 0, time_to_wait = 50;
static uint8_t gameMode = MENU;
static float oldDistance = 0;
static bool firstAnimationsRun = true;

byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  matrix.begin();
  Modulino.begin();
  
  introduction_message(); //display a message when starting

  #ifdef BUTTONS
  buttons.begin();
  buttons.setLeds(true, true, true);
  #endif
  
  #ifdef ENCODER_MODE
  Scheduler.startLoop(playerLoop);
  encoder.begin();
  encoder.set(PLAYER_Y);
  #endif

  #ifdef TOF_MODE
  Scheduler.startLoop(distanceLoop);
  distanceSensor.begin();
  #endif

  #if !defined(TOF_MODE) && !defined(ENCODER_MODE)
  game_ongoing = true;
  gameMode = 127;
  digitalWrite(LED_BUILTIN, HIGH);
  #endif
}

void loop() {

  while(gameMode == MENU){
    #ifdef DEBUG
    Serial.println("Waiting for menu");
    #endif  

    #ifdef BUTTONS
    buttons.update();

    if(buttons.isPressed(0)){
      gameMode = ENCODER;
    } 
    
    if (buttons.isPressed(2)){
      gameMode = TOF;
    }

    #endif

    #ifdef ANIMATIONS
    show_idle_animation(true);
    #endif

    delay(500);
  }

  if (game_ongoing) {
    if(score == 0){
      delay(800);
    }

    /* Clean the last column of the matrix */
    for (uint8_t y = 0; y < HEIGHT; y++) {
      frame[y][0] = 0;
      frame[y][1] = 0;
      frame[y][2] = 0;
    }

    /* Set wall position on the matrix using random */
    wall_start_pix = random(0, 5);

    /* Move the wall through matrix */
    do {
      if (wall_move) {
        for (uint8_t y = 0; y < HEIGHT; y++) {
          frame[y][wall_pos_x] = (y >= wall_start_pix && y < wall_start_pix + wall_size) ?  0 : 1;
          if (wall_pos_x < WIDTH - 3) {
            frame[y][wall_pos_x + 3] = 0;
          }
        }
        wall_pos_x--;
      }
      wall_move = !wall_move;

      /* Update display */
      matrix.renderBitmap(frame, HEIGHT, WIDTH);

      /* Adapt level */
      adapt_game_level();

      #if defined(TOF_MODE) || defined(ENCODER_MODE)

      /* Check if player touch the wall */
      if (frame[player_y][player_x] == frame[player_y][player_x - 1]) {
        reset_global_variables();
        return;
      }

      #endif

    } while (wall_pos_x >= 0);
    /* Increment score counter */
    score++;
    /* Reset wall position */
    wall_pos_x = WIDTH-1;
  }

  delay(1);

}

void playerLoop() {

  while(gameMode != ENCODER){
    #ifdef DEBUG
    Serial.println("Waiting for ENCODER");
    #endif

    if(game_ongoing == 1){ 
      delay(500);
      continue; 
    }

    if(encoder.get() != PLAYER_Y){
      gameMode = ENCODER;
    }

    delay(1);
  }

  game_ongoing = 1;

  frame[player_y][player_x] = 0;

  int value = encoder.get();

  if(value < 0) {encoder.set(0);}
  if(value >= HEIGHT-1) {encoder.set((HEIGHT-1));}

  value = encoder.get();
  player_y = value;

  frame[player_y][player_x] = 1;
  matrix.renderBitmap(frame, 8, 12);

  delay(1);
}

void distanceLoop() {

  while(gameMode != TOF){
    #ifdef DEBUG
    Serial.println("Waiting for TOF");
    #endif

    if(game_ongoing == 1){ 
      delay(500);
      continue; 
    }

    float d = distanceSensor.get();
    unsigned long m = millis();

    if(!isnan(d) && d < MAX_TOF_H){
      #ifdef DEBUG
      Serial.println("MenuMode - something in front of the sensor");
      #endif
      if(oldDistance != 0 && ((m - t_oldDistance) > TOF_PRESENCE_TIME)){
        #ifdef DEBUG
        Serial.println("MenuMode - something in front of the sensor for 5 seconds");
        #endif
        gameMode = TOF;
      } else if(oldDistance == 0) {
        #ifdef DEBUG
        Serial.println("MenuMode - starting to count seconds");
        #endif
        oldDistance = d;
        t_oldDistance = m;
      }
    } else {
      #ifdef DEBUG
        Serial.println("MenuMode - Nothing in front of the sensor");
      #endif
      oldDistance = 0;
      t_oldDistance = m;
    }
    
    delay(1);
  }

  game_ongoing = 1;

  float distance = distanceSensor.get();

  if(isnan(distance)){
    #ifdef DEBUG
    Serial.println("Distance is NAN");
    #endif
    delay(1);
    return;
  }
  
  #ifdef DEBUG
  Serial.print("Distance: ");
  Serial.println(distance);
  #endif

  if(distance > MAX_TOF_H){
    #ifdef DEBUG
    Serial.print("Distance discarded, Too HIGH!");
    #endif
    delay(1);
    return;
  }

  if(oldDistance == 0){
    oldDistance = distance;
    game_ongoing = 1;
    delay(1);
    return;
  } else {

    frame[player_y][player_x] = 0;

    if(distance < oldDistance-25){
      player_y++;
      oldDistance = distance;
    } else if(distance > oldDistance+25){
      player_y--;
      oldDistance = distance;
    }

    if(player_y < 0){player_y=0;}
    else if(player_y > HEIGHT-1) {player_y=HEIGHT-1;}

    frame[player_y][player_x] = 1;
    matrix.renderBitmap(frame, 8, 12);

  }

  delay(1);
}

/* Functions declaration ----------------------------------------------------------*/
void introduction_message(){
  
  #if !defined(ANIMATIONS)
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);

  // add the text
  const char text[] = " Flappy LED! ";
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
  #endif

  #ifdef ANIMATIONS
  show_idle_animation(false);
  #endif

}

#ifdef ANIMATIONS
void show_idle_animation(bool interrupt){

  for(auto i:introAnimation){
    if(interrupt && game_ongoing == true){break;}
    matrix.loadFrame(i);
    delay(90);
  }
  
  for(auto i:flappyAnimation){
    if(interrupt && game_ongoing == true){break;}
    matrix.loadFrame(i);
    delay(66);
  }
  
  for(auto i:crashAnimation){
    if(interrupt && game_ongoing == true){break;}
    matrix.loadFrame(i);
    delay(66);
  }

  for(auto i:modesAnimation){
    if(interrupt && game_ongoing == true){break;}
    matrix.loadFrame(i);
    delay(80);
  }
}
#endif

void adapt_game_level()
{
  /* Adapt speed & hole size */
  if (score < 5) {
    time_to_wait = 70;
  }
  else if (score >= 5 && score < 10) {
    time_to_wait = 50;
  }
  else if (score >= 10 && score < 15) {
    time_to_wait = 40;
  }
  else if (score >= 15 && score < 20) {
    wall_size = 2;
  }
  else if (score >= 20 && score < 25) {
    time_to_wait = 30;
  }
  else {
    wall_size = 1;
  }
  delay(time_to_wait);
}

void print_score(uint16_t game_score)
{
  uint8_t text_pos_x = (game_score < 10) ? 5 : 3;
  matrix.clear();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  char text[3];
  itoa(game_score, text, 10);
  matrix.textFont(Font_4x6);
  matrix.beginText(text_pos_x, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText();
  matrix.endDraw();
}

void clear_text()
{
  matrix.clear();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println("       ");
  matrix.endText();
  matrix.endDraw();
}

void reset_global_variables()
{
  gameMode = MENU;
  memset(frame, 0, WIDTH * HEIGHT * sizeof(byte));
  player_x = PLAYER_X;
  player_y = PLAYER_Y;

  /*print crash animation*/
  for(auto i:crashAnimation){
    matrix.loadFrame(i);
    delay(66);
  }

  print_score(score);
  /* Reset score after loosing the party */
  score = 0;
  /* Reset wall position */
  wall_pos_x = WIDTH-1;
  /* Reset wall size */
  wall_size = 3;
  /* Reset delay */
  time_to_wait = 50;
  /*reset encoder*/
  encoder.set(PLAYER_Y);
  /*reset game mode*/
  
  /*reset old distance for TOF sensor */
  t_oldDistance = 0;
  oldDistance = 0;

  delay(3000);
  clear_text();
  game_ongoing = 0;
}

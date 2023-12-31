#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    void set_y(int y_arg) {
      y = y_arg;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg; 
      y = y_arg;
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg; 
      y = y_arg; 
      strength = strength_arg; 
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg; 
      y = y_arg; 
      strength = strength_arg;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y; 
    }
    int get_strength() const {
      return strength; 
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
      y = y + 1;
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if (strength > 0) {
        if (strength == 1) {
          draw_with_rgb(RED, BLUE);
        }
        else if (strength == 2) {
          draw_with_rgb(ORANGE, BLUE);
        }
        else if (strength == 3) {
          draw_with_rgb(YELLOW, BLUE);
        }
        else if (strength == 4) {
          draw_with_rgb(GREEN, BLUE);
        }
        else if (strength == 5) {
          draw_with_rgb(BLUE, BLUE);
        }
        else if (strength == 6) {
          draw_with_rgb(PURPLE, BLUE);
        }
        else if (strength == 7) {
          draw_with_rgb(WHITE, BLUE);
        }
      }
     }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      strength = strength - 1;
      if (strength == 0) {
        erase(); 
      }
      else {
        draw();
      }
    }

  private:
    int x;
    int y;
    int strength;
    
    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
    matrix.drawPixel(x + 1, y, body_color.to_333());
    matrix.drawPixel(x + 2, y, body_color.to_333());
    matrix.drawPixel(x, y + 1, body_color.to_333());
    matrix.drawPixel(x, y + 2, body_color.to_333());
    matrix.drawPixel(x, y + 3, body_color.to_333()); 
    matrix.drawPixel(x + 1, y + 2, body_color.to_333());
    matrix.drawPixel(x + 2, y + 2, body_color.to_333());
    matrix.drawPixel(x + 3, y + 1, body_color.to_333());
    matrix.drawPixel(x + 3, y + 2, body_color.to_333());
    matrix.drawPixel(x + 3, y + 3, body_color.to_333()); 
    matrix.drawPixel(x + 1, y + 1, eye_color.to_333());
    matrix.drawPixel(x + 2, y + 1, eye_color.to_333());
    }
};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 15;
      fired = false;
    }
    
    // resets private data members to initial values
    void reset() {
      erase();
      x = 10000; 
      y = 10000;
      fired = false; 
    }
    
    // getters
    int get_x() const {
      return x; 
    }
    int get_y() const {
      return y; 
    }
    bool has_been_fired() const {
      return fired; 
    }

    int set_y(int y_arg) {
      y = y;
    }
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
      x = x_arg; 
      y = y_arg; 
      fired = true;
    }
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      y = y - 1;
      // If the cannonball goes off the screen
      if (y < 0) {
        // Reset the cannonball itself with the reset function
        reset();
      }
    }
    
    // resets private data members to initial values
    void hit() {
      reset();
    }
    
    // draws the Cannonball, if it is fired
    void draw() {
      // Draw pixels of the instance
      if (fired) {
        matrix.drawPixel(x, y, ORANGE.to_333());
        matrix.drawPixel(x, y - 1, ORANGE.to_333());
      }

    }
    
    // draws black where the Cannonball used to be
    void erase() {
      // Erase all the pixels of the instance
      matrix.drawPixel(x, y, BLACK.to_333());
      matrix.drawPixel(x, y - 1, BLACK.to_333());
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      x = 0;
      y = 0;
      lives = 3;
    }
    
    // getters
    int get_x() const {
      return x; 
    }
    int get_y() const {
      return y; 
    }
    int get_lives() const {
      return lives; 
    }
    
    // setter
    void set_x(int x_arg) {
      x = x_arg; 
    }
    
    // Modifies: lives
    void die() {
      erase(); 
      lives = 0;
    }

    void gain_life() {
      lives = lives + 1;
    }

    void lose_life() {
      lives = lives - 1;
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(AQUA);
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg; 
      y = y_arg;
    }
    
    // draws the player
    // account for when the player goes off the screen
    // should use x and y coordinates instead of specific integers
    void draw_with_rgb(Color color) {
      if (x < 1) {
        matrix.drawPixel(x, y + 15, color.to_333());
        matrix.drawPixel(x + 1, y + 15, color.to_333());
        matrix.drawPixel(x , y + 14, color.to_333());
      }
      else if (x > 30) {
        matrix.drawPixel(x - 1, y + 15, color.to_333());
        matrix.drawPixel(x, y + 15, color.to_333());
        matrix.drawPixel(x, y + 14, color.to_333());
      }
      else {
      matrix.drawPixel(x - 1, y + 15, color.to_333());
      matrix.drawPixel(x, y + 15, color.to_333());
      matrix.drawPixel(x + 1, y + 15, color.to_333());
      matrix.drawPixel(x , y + 14, color.to_333());
      }
    }
};

class Aid {
  public: 
  // Constructors
    Aid() {
      x = 0;
      y = 0;
    }
    void set_x(int x_arg) {
      x = x_arg;
    }
    
    // sets values for private data members
    void initialize(int x_arg, int y_arg) {
      x = x_arg; 
      y = y_arg; 
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y; 
    }

    // Moves the aid down the screen by one row
    // Modifies: y
    void move() {
      y = y + 1;
    }
    
    // draws the aid if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(LIME);
    }
    
    void erase() {
      draw_with_rgb(BLACK);
    }    
    
    // draws the aid
    void draw_with_rgb(Color body_color) {
      matrix.drawPixel(x, y + 4, body_color.to_333()); 
      matrix.drawPixel(x + 1, y + 4, body_color.to_333()); 
      matrix.drawPixel(x + 1, y + 5, body_color.to_333()); 
      matrix.drawPixel(x + 1, y + 3, body_color.to_333()); 
      matrix.drawPixel(x + 2, y + 4, body_color.to_333());
    }

    void hit() {
      erase();
    }

    void screen_plus() {
      matrix.drawPixel(x + 27, y + 11, LIME.to_333());
      matrix.drawPixel(x + 27, y + 12, LIME.to_333());
      matrix.drawPixel(x + 26, y + 11, LIME.to_333());
      matrix.drawPixel(x + 28, y + 11, LIME.to_333());
      matrix.drawPixel(x + 29, y + 10, GREEN.to_333());
      matrix.drawPixel(x + 29, y + 11, GREEN.to_333());
      matrix.drawPixel(x + 29, y + 12, GREEN.to_333());
    }

  private:
    int x;
    int y; 
};

class Game {
  public:
    Game() {
      level = 1;
      time = 0;
      invader_time = 0; 
      ball_time = 0;
      last_move = 0;
    }
    
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {
    matrix.fillScreen(BLACK.to_333());
    reset_level();
    matrix.fillScreen(BLACK.to_333());


    // Initializes row of invaders for level 1 
    if (level == 1) {
      for (int i = 0; i < 8; i++) {
        enemies[i].initialize(i * 4, -1 , 1);
      }
      for (int q = 8; q < 16; q++) {
        enemies[q].initialize(q * 4, -1 , 0);
      }
    }   

    int randomNum = random(1, 30);
    aid.initialize(randomNum, 1);    

    }
    
    // advances the game simulation one step and renders the graphics
    void update(int potentiometer_value, bool button_pressed) {

 
    
    time = millis();
    
    // The potentiometer value gets passed in as x value for the update function
    // Dividing potentiometer value by 32
    player.set_x(potentiometer_value / 32);
    player.draw();
    delay(1);
    player.erase();

    // Checks that button is pressed
    // Accounts for the fact that the ball cannot be fired consecutively
    if (button_pressed && !ball.has_been_fired()) {
      ball.fire(player.get_x(), player.get_y() + 14);
      ball_time = time;
    }

    // Moves the ball when fired
    if (ball.has_been_fired() && time > ball_time) {
      ball.erase();
      ball.move();
      ball.draw();
      ball_time = time + 25;
    }

     // BALL ENEMY COLLISION
     // Checks coordinates for ball and enemy for collision
    if (ball.has_been_fired()) {
      if (
        (aid.get_x() + 1 == ball.get_x() && aid.get_y() + 5 == ball.get_y()) ||
        (aid.get_x() == ball.get_x() && aid.get_y() + 4 == ball.get_y()) ||
        (aid.get_x() + 2 == ball.get_x() && aid.get_y() + 4 == ball.get_y())
        ) {
          // the ball is hit
          aid.hit();
          ball.hit();
          aid.initialize(1, 200000); 
        }
        
      for (int j = 0; j < NUM_ENEMIES; j++) {
        if (
        (enemies[j].get_x() == ball.get_x() && enemies[j].get_y() == ball.get_y()) ||
        (enemies[j].get_x() + 1 == ball.get_x() && enemies[j].get_y() == ball.get_y()) ||
        (enemies[j].get_x() + 2 == ball.get_x() && enemies[j].get_y() == ball.get_y()) ||
        (enemies[j].get_x() + 3 == ball.get_x() && enemies[j].get_y() == ball.get_y())
        ) {
          // the ball is hit
          enemies[j].hit();
          if (enemies[j].get_strength() == 0) {
            enemies[j].initialize(10000, -2000, 0);
          }
          ball.hit();
        }
      }
    }

    if (time - aid_time >= 967) {
          aid.erase(); 
          aid.move(); 
          aid.draw();
          aid_time = time;
        }

  // invader time
    if(time - invader_time >= 2000){
      if (level == 1) {
        for (int j = 0; j < NUM_ENEMIES / 2; j++) {
          enemies[j].erase();
          enemies[j].move();
          enemies[j].draw();
          invader_time = time;
        }
      }
      if (level > 1) {
        if (!row2cleared()) {
          for (int j = 0; j < NUM_ENEMIES / 2; j++) {
            enemies[j].erase();
            enemies[j].set_y(0);
            enemies[j].draw();
            invader_time = time;
          }
          for (int i = 8; i < NUM_ENEMIES; i++) {
            enemies[i].erase();
            enemies[i].move();
            enemies[i].draw();
            invader_time = time;
          }
        }
      else {
        for (int j = 0; j < NUM_ENEMIES / 2; j++) {
          enemies[j].erase();
          enemies[j].move();
          enemies[j].draw();
          invader_time = time;
        }
      }
      }
      
      
    }
    

    // PLAYER ENEMY COLLISION
  for (int i = 0; i < NUM_ENEMIES; i++) {
    if (
      ((player.get_x() - 1 == enemies[i].get_x()) && (player.get_y() + 15 == enemies[i].get_y() + 3)) ||
      ((player.get_x() - 1 == enemies[i].get_x() + 3) && (player.get_y() + 15 == enemies[i].get_y() + 3)) ||
      ((player.get_x() + 1 == enemies[i].get_x()) && (player.get_y() + 15 == enemies[i].get_y() + 3)) ||
      ((player.get_x() + 1 == enemies[i].get_x() + 3) && (player.get_y() + 15 == enemies[i].get_y() + 3)) ||
      ((player.get_x() == enemies[i].get_x()) && (player.get_y() + 14 == enemies[i].get_y() + 3)) ||
      ((player.get_x() == enemies[i].get_x() + 3) && (player.get_y() + 14 == enemies[i].get_y() + 3))
    ) {
      player.lose_life();
        for (int j = 0; j < NUM_ENEMIES / 2; j++) {
        enemies[j].initialize(j * 4, -1 , 1);
      }
      reset_level();
    }
  }

  // AID PLAYER COLLISION
  if (
    ((player.get_x() - 1 == aid.get_x() + 1) && (player.get_y() + 15 == aid.get_y() + 5)) ||
      ((player.get_x() - 1 == aid.get_x() + 2) && (player.get_y() + 15 == aid.get_y() + 4)) ||
      ((player.get_x() + 1 == aid.get_x() + 1) && (player.get_y() + 15 == aid.get_y() + 5)) ||
      ((player.get_x() + 1 == aid.get_x()) && (player.get_y() + 15 == aid.get_y() + 4)) ||
      ((player.get_x() - 1 == aid.get_x()) && (player.get_y() + 15 == aid.get_y() + 3)) ||
      ((player.get_x() + 1 == aid.get_x()) && (player.get_y() + 15 == aid.get_y() + 3)) ||
      ((player.get_x() == aid.get_x() + 1) && (player.get_y() + 14 == aid.get_y() + 5))
  ) {
    aid.hit();
    aid.initialize(1, 29999999999);
    player.gain_life();
  }

    // ENEMY BORDER COLLISION
    for (int i = 1; i <= NUM_ENEMIES; i++){
      if (enemies[i].get_y() > 12) {
        player.lose_life();
        for (int j = 0; j < NUM_ENEMIES / 2; j++) {
          enemies[j].initialize(j * 4, -1 , 1);
        }
        reset_level();
      }
    }

    if (level_cleared()) {
      level++;
      reset_level();
    }

    if (player.get_lives() == 0) {
      int loopforever = 0;
      while (loopforever == 0) {
        game_over();
      }
    }

    }


  private:
    int level;
    unsigned long time;
    unsigned long invader_time; 
    unsigned long ball_time;
    unsigned long aid_time;
    int last_move;
    Player player;
    Aid aid;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];


    // check if Player defeated all Invaders in current level
    bool level_cleared() {
        int enemy_dead = 0; 
        for (int i = 0; i < NUM_ENEMIES; i++) {
          if (enemies[i].get_strength() == 0) {
            enemy_dead++;
          }
        }
        if (enemy_dead == NUM_ENEMIES) {
          return true;
        }
      return false;
    } 

    bool row2cleared() {
      int dead = 0; 
      for (int i = 8; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() == 0) {
          dead++;
        }
      }
      if (dead == 8) {
        return true;
      }
      else{
        return false;
      }
    }

    // set up a level
    void reset_level() {
      print_level(level);
      delay(800);
      print_lives(player.get_lives());
      delay(800);
      matrix.fillScreen(BLACK.to_333());

      int randomNum = random(1, 31);
      aid.initialize(randomNum, 5);  

      if (level == 2) {
        int strength = 0; 
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          int x = i % 8 * 4; 
          int y = i / 8 * 4 - 1; 
          if (i % 2 == 0) {
            strength = 1;
          }
          else {
            strength = 2;
          }
          enemies[i].initialize(x, y, strength);
        }
        for (int j = 8; j < NUM_ENEMIES; j++) {
          int x = j % 8 * 4; 
          int y = j / 8 * 4 - 1; 
          if (j % 2 == 0) {
            strength = 2;
          }
          else {
            strength = 1;
          }
          enemies[j].initialize(x, y, strength);
        }

      }
      if (level == 3) {
        int strength = 0; 
        for (int i = 0; i < NUM_ENEMIES; i++) {
          int x = i % 8 * 4; 
          int y = i / 8 * 4 - 1; 
          strength = i % 5 + 1;
          enemies[i].initialize(x, y, strength);
        }
      }
      if (level == 4) {
        int strength = 0; 
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          int x = i % 8 * 4; 
          int y = i / 8 * 4 - 1; 
          if (i % 2 == 0) {
            strength = 5;
          }
          else {
            strength = 4;
          }
          enemies[i].initialize(x, y, strength);
        }
        for (int j = 8; j < NUM_ENEMIES; j++) {
          int x = j % 8 * 4; 
          int y = j / 8 * 4 - 1; 
          if (j % 2 == 0) {
            strength = 2;
          }
          else {
            strength = 3;
          }
          enemies[j].initialize(x, y, strength);
        }

      }
      if (level >= 5) {
        int strength = 0; 
        for (int j = 0; j < NUM_ENEMIES; j++) {
          int x = j % 8 * 4; 
          int y = j / 8 * 4 - 1; 
          strength = random (1,8);
          enemies[j].initialize(x, y, strength);
        }
      }
    }
};

// a global variable that represents the game Space Invaders
Game game;
long randNumber;
// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  game.update(potentiometer_value, button_pressed);

  
}

// displays Level
void print_level(int level) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(2, 0);
  matrix.print("Level");
  matrix.setCursor(13, 8);
  matrix.print(level);
  delay(1000);
}

// displays number of lives
void print_lives(int lives) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(2,0);
  matrix.print("Lives");
  matrix.setCursor(13, 8);
  matrix.print(lives);
  delay(1000);
}

// displays "game over"
void game_over() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(3,0);
  matrix.print("GAME OVER");
  delay(200);
}

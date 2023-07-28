//Josh Miller (RamenJunkie)
//2023.07.25
//A Pong clone
//Originally created from
//https://community.arduboy.com/t/make-your-own-arduboy-game-part-7-make-pong-from-scratch/7930/3
//Then refined and added features for code practice

//Added
// Adjust Winning Score needed on title *Done*
// Title Graphic
// Offset ball starting point left/right *Done*
// Larger paddle option  *Done*
// Paddles start at center (28) *Done*

#include <Arduboy2.h>
Arduboy2 arduboy;

//Variables declared here
int gamestate = 0;
int ballx = 62;
int bally = 0;
int ballsize = 4;
int ballright = 1;
int balldown = 1;
int paddlewidth = 4;
int paddleheight = 9;
int playerx = 0;
int playery = 28;
int computery = 28;
int computerx = 127 - paddlewidth;
int playerscore = 0;
int computerscore = 0;
int numwins = 5;
String paddlesize = "S";
int titlecursor = 1;
int computereact = 100;

void resetball() {
  ballx = 63-(40*ballright);
  bally = random(0, 40);
}

void resetgame() {
  resetball();
  playerscore = 0;
  computerscore = 0;
}

void showscores() {
    //Draw Score
    arduboy.setCursor(20, 0);
    arduboy.print(playerscore);

    arduboy.setCursor(101, 0);
    arduboy.print(computerscore);  
}

void setup() {
  arduboy.begin();
  //Seed the random number generator
  arduboy.initRandomSeed();
  //Set the game to 60 frames per second
  arduboy.setFrameRate(60);
  arduboy.clear();
}

void loop() {
  
  //Prevent the Arduboy from running too fast
  if (!arduboy.nextFrame()) {
    return;
  }

  arduboy.clear();
  arduboy.pollButtons();
  showscores();
  
  switch (gamestate) {
  case 0:
    //Title screen
    arduboy.setCursor(50, 30);
    arduboy.print("Pong");
    arduboy.setCursor(15, 40);
    arduboy.print("Press A to Start");
    arduboy.setCursor(1, 50);
    arduboy.print("Win Score: ");
    arduboy.setCursor(65, 50);
    arduboy.print(numwins);
    arduboy.setCursor(75, 50);
    arduboy.print("Paddle: ");
    arduboy.setCursor(120, 50);
    arduboy.print(paddlesize);

    if(titlecursor == 1) {
      arduboy.fillRect(64, 60, 5, 2, WHITE); 
    }
    else {  
      arduboy.fillRect(120, 60, 5, 2, WHITE); 
    }

    if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON)) { titlecursor = titlecursor * -1; }

    if ((arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) && titlecursor == -1) { 
      if(paddlesize == "S") {paddlesize = "L";}
      else {paddlesize = "S";}
    }
    if (arduboy.justPressed(UP_BUTTON) && titlecursor == 1 && numwins < 9) { 
      numwins++;
    }
    
    if (arduboy.justPressed(DOWN_BUTTON) && titlecursor == 1 && numwins > 5) { 
      numwins--;
    }

    if (arduboy.justPressed(A_BUTTON)) {
      if(paddlesize == "S") { paddleheight = 9; computereact = 100;}
      else {paddleheight = 15; computereact = 115;}
      gamestate = 1;
    }
    break;

  case 1:
    //Gameplay screen
    arduboy.fillRect(64, 0, 2, 64, WHITE);

    //Draw Board Pieces
    arduboy.fillRect(ballx, bally, ballsize, ballsize, WHITE);
    arduboy.fillRect(playerx, playery, paddlewidth, paddleheight, WHITE);
    arduboy.fillRect(computerx, computery, paddlewidth, paddleheight, WHITE);
    //Player control
    if (arduboy.pressed(UP_BUTTON) && playery > 0) {
      playery = playery - 1;}
    if (arduboy.pressed(DOWN_BUTTON) && playery + paddleheight < 63) {
      playery = playery + 1;}

    //Computer Control
    if (ballx > computerreact || random(0, 20) == 1) {
      if (bally < computery) {
        computery = computery - 1;}
      if (bally + ballsize > computery + paddleheight) {
        computery = computery + 1;}
      }

    // Ball Movement
    //Move the ball right
    if(ballright == 1) {
      ballx = ballx + 1;}
    //Move the ball left
    if(ballright == -1) {
      ballx = ballx - 1;}
    //Reflect the ball off of the player paddle
    if (ballx == playerx + paddlewidth && playery < bally + ballsize && playery + paddleheight > bally) {
      ballright = 1;}
    //Reflect the ball off of the computer paddle
    if (ballx + ballsize == computerx && computery < bally + ballsize && computery + paddleheight > bally) {
      ballright = -1;}
    //Move the ball down
    if (balldown == 1) {
      bally = bally + 1;
    }
    //Move the ball up
    if (balldown == -1) {
      bally = bally - 1;
    }
    //Reflect the ball off the bottom
    if (bally == 0) {
      balldown = 1;}
    //Reflect the ball off the top
    if (bally + ballsize == 63) {
      balldown = -1;}
    // Score dead balls
    if (ballx < -10) {
      resetball();
      computerscore = computerscore + 1;
    }
    if (ballx > 130) {
      resetball();
      playerscore = playerscore + 1;
    }
    // Check for Win/Loss
    if (playerscore == numwins) {
      gamestate = 2;
    }
    if (computerscore == numwins) {
      gamestate = 3;
    }

    if (arduboy.justPressed(A_BUTTON)) {
      gamestate = 4;}
    break;

  case 2:
    //Win screen
    arduboy.setCursor(40, 30);
    arduboy.print("You won!");
    if (arduboy.justPressed(A_BUTTON)) {
      resetgame();
      gamestate = 0;
    }
    break;

  case 3:
    //Game over screen
    arduboy.setCursor(40, 30);
    arduboy.print("Game Over");
    if (arduboy.justPressed(A_BUTTON)) {
      resetgame();
      gamestate = 0;
    }
    break;

  case 4:
    arduboy.setCursor(46, 30);
    arduboy.print("Paused");
    if (arduboy.justPressed(A_BUTTON)) {
            gamestate = 1;
    }
    break;
  }

  arduboy.display();
}

// Library for I2C
#include "Wire.h"

// LCD library
#include "LCD.h" 

#include "LiquidCrystal_I2C.h"

#define DINO_HEAD 0
#define DINO_BODY_1 1
#define DINO_BODY_2 2
#define DINO_JUMP 3

// Pins for the buttons
#define PIN_BUTON 2
#define PIN_BUTON_1 3
#define LED 11
#define LED1 13 
#define LED2 9
// The pin for autoplay
#define PIN_AUTOPLAY 1
// Pin for the buzzer
#define PIN_BUZZER 5
// Counter for the time
static int counter = 0;

// Counter for the melody
int thisNote = 0;

#define RUNNING_1 1
#define RUNNING_2 2
#define JUMP 3
#define JUMP_UP '.' // Character '.' for the head of the player
#define JUMP_DOWN 4
#define EMPTY_CELL ' '   // Empty space
#define FULL_TREE 5
#define RIGHT_TREE 6
#define LEFT_TREE 7

#define HORISONTAL_POSITION_PLAYER 1       // Horisontal position for the player on the screen
#define UP_BLOC 1
#define DOWN_BLOC 2
#define STANDING_POSITION_PLAYER 0           
#define DOWN_POSITION_PLAYER_1 1            // Player is on the downwards position first row
#define DOWN_POSITION_PLAYER_2 2            // Player is on the downwards position second row

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

byte dinoHead[] = {
    B01110,
    B11011,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
    B00000
};

byte dinoBody1[] = {
    B00000,
    B00000,
    B01110,
    B11111,
    B11111,
    B01110,
    B00100,
    B00100
};

byte dinoBody2[] = {
    B00000,
    B00000,
    B01110,
    B11111,
    B11111,
    B01110,
    B01010,
    B10001
};

byte dinoJump[] = {
    B00000,
    B01110,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
    B00000
};

#define RUNNING_POSITION_PLAYER_1 3       
#define RUNNING_POSITION_PLAYER_2 4       

#define JUMPING_PLAYER_1 5        
#define JUMPING_PLAYER_2 6       
#define JUMPING_PLAYER_3 7       

#define FALLING_PLAYER_1 8       
#define FALLING_PLAYER_2 9       
#define FALLING_PLAYER_3 10      

#define UP_PLAYER_1 11          
#define UP_PLAYER_2 12           


// Obstacles represented as 2 vectors one for the upper row of the LCD, the other for the lower row of the LCD
static char upper_map[17];
static char lower_map[17];

// Buttons status
static int buton_activat = 0;
static int buton_activat_1 = 0;
static bool pause = false;
static bool running_game = false;
// Checking if the LCD si working or not
static bool beginning_message_game = true;

void map_initialisation() {
  int index;
  // Matrix for the upcoming frames
  static byte frames[] = {

    //run 1
    B01111,
    B01011,
    B01111,
    B01100,
    B01111,
    B11110,
    B10010,
    B11000,

    //run 2
    B01111,
    B01011,
    B01111,
    B01100,
    B01111,
    B11110,
    B10010,
    B00011,

    //jump
    B01111,
    B01011,
    B01111,
    B01100,
    B01111,
    B11110,
    B10010,
    B11011,

    //jump lower
    B01100,
    B01111,
    B11110,
    B10010,
    B11011,
    B00000,
    B00000,
    B00000,

    // Full tree
    B00100, //   *   
    B01110, //  ***  
    B11111, // *****
    B00100, //   *   
    B00100, //   *   
    B00100, //   *   
    B00100, //   *
    B00100,  //  *
    // Right tree
    B00001, //      *
    B00011, //     **
    B00111, //    ***
    B00001, //      *
    B00001, //      *
    B00001, //      *
    B00001, //      *
    B00001, //      *
    // Left tree
    B10000, // *
    B11000, // **
    B11100, // ***
    B10000, // *
    B10000, // *
    B10000, // *
    B10000, // *
    B10000  // *
  };
  
  for(index = 0; index < 7; index++){
    lcd.createChar(index + 1, &frames[index * 8]);
  }

  // initialising the screen with empty cells

  for(index = 0; index < 16; index++){
    upper_map[index] = EMPTY_CELL;
    lower_map[index] = EMPTY_CELL;
  }
}

// Musical Notes
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_A4S 466
#define NOTE_Eb5 622
#define REST      0

// the melody's tempo
int tempo = 120;
// buzzer's PIN
int buzzer = 5;

// the melody
int melody[] = {
  // Pink Panther theme
  // Score available at https://musescore.com/benedictsong/the-pink-panther
  // Theme by Masato Nakamura, arranged by Teddy Mason

  REST,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,2, REST,4, REST,8, NOTE_DS4,4,

  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_G4,8, NOTE_B4,-8, NOTE_E5,8,
  NOTE_DS5,1,   
  NOTE_D5,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,-4, REST,4,
  REST,4, NOTE_E5,-8, NOTE_D5,8, NOTE_B4,-8, NOTE_A4,8, NOTE_G4,-8, NOTE_E4,-8,
  NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8,   
  NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, NOTE_E4,16, NOTE_E4,16, NOTE_E4,2,
  
};

// game over Melody
int gameOverMelody[] = {
 NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, 
 NOTE_D5,8, NOTE_E5,8, NOTE_F5,8, NOTE_E5,8, 
 NOTE_D5,8, NOTE_C5,8, NOTE_C5,8
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

void Game_Over_Melody(int *melody, int tempo) {
  int size = sizeof(melody) / sizeof(melody[0]);
  int wholenote = (60000 * 4) / tempo;

  for (int thisNote = 0; thisNote < size; thisNote += 2) {
    int noteDuration = 0;
    if (melody[thisNote + 1] > 0) {
      noteDuration = wholenote / melody[thisNote + 1];
    } else {
      noteDuration = (wholenote / abs(melody[thisNote + 1])) * 1.5;
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}

// Here I am drawing the player on the screen. I return the value 0 if there is no collision
// and 1 otherwise.

int player_design(byte player_pos, char* upper_map, char* lower_map, unsigned int time) {
  int count_numbers = 0, colision = 0;
  char current_upper_frame = upper_map[HORISONTAL_POSITION_PLAYER];
  char current_lower_frame = lower_map[HORISONTAL_POSITION_PLAYER];
  byte next_upper_frame, next_lower_frame;

  if (player_pos == STANDING_POSITION_PLAYER) {
    //If the game hasn't started  yet and player is in a resting position, 
    //then for the next step I will not be drawing trees
    next_upper_frame = next_lower_frame = EMPTY_CELL;
} else if (player_pos == DOWN_POSITION_PLAYER_1) {
    // Here is the player in the next frame in a running position 
    //with no obstacles in the upper side of the map
    next_upper_frame = EMPTY_CELL;
    next_lower_frame = RUNNING_1;
} else if (player_pos == DOWN_POSITION_PLAYER_2) {
    // Here is the player in the next frame in a running position 
    // with no obstacles in the upper side of the map neither on the down side
    next_upper_frame = EMPTY_CELL;
    next_lower_frame = RUNNING_2;
} else if (player_pos == RUNNING_POSITION_PLAYER_1) {
    // Here is the player in a running position, and for the next step it will be jumping, 
    // down there will be the jump position and upwards there will be an empty cell
    next_upper_frame = EMPTY_CELL;
    next_lower_frame = JUMP;
} else if (player_pos == FALLING_PLAYER_3) {
    // Here, the player ges ready to land and for the next step the frame 
    // will have been represented by the las part of the landing
    next_upper_frame = EMPTY_CELL;
    next_lower_frame = JUMP;
} else if (player_pos == RUNNING_POSITION_PLAYER_2) {
    // Here the player gets up and does a part of the JUMP. 
    // In the next frame,the 2 cells : up and down will be 
    // the body of the player
    next_upper_frame = JUMP_UP;
    next_lower_frame = JUMP_DOWN;
} else if (player_pos == FALLING_PLAYER_2) {
    // Here, the player is close with his feet to the ground 
    // and does a part of the falling position
    next_upper_frame = JUMP_UP;
    next_lower_frame = JUMP_DOWN;
} else if (player_pos == JUMPING_PLAYER_1) {
    // Here the player ras reatched the upper layer in the position of JUMP. 
    // In the next frame there will be the same position for the upper part
    // And downwards there will be no tree
    next_upper_frame = JUMP;
    next_lower_frame = EMPTY_CELL;
} else if (player_pos == JUMPING_PLAYER_2) {
    // Here, the player is on the same 2 rows in the middle of the jumping action, 
    // and till the next frame the JUMP animation will continue
    next_upper_frame = JUMP;
    next_lower_frame = EMPTY_CELL;
} else if (player_pos == JUMPING_PLAYER_3) {
    // Here the player will be at the end if the JUMP action and the animation will be finalized 
    // in the next frame, placind itself in the upper row of the LCD
    next_upper_frame = JUMP;
    next_lower_frame = EMPTY_CELL;
} else if (player_pos == FALLING_PLAYER_1) {
    // Here, the player is in the upper row of the LCD and simulates the beginning of the falling animation. 
    // In the next frame the falling animation will be continued
    next_upper_frame = JUMP;
    next_lower_frame = EMPTY_CELL;
} else if (player_pos == UP_PLAYER_1) {
    // Here, the player is on the upper row of the LCD and upwards 
    // it will enter in the running mode, and downwards there will be an empty cell
    next_upper_frame = RUNNING_1;
    next_lower_frame = EMPTY_CELL;
} else if (player_pos == UP_PLAYER_2) {
    // Here, the player is on the upper row of the LCD. 
    // Upwards the running mode will be enabled, and downwards there will be an empty cell
    next_upper_frame = RUNNING_2;
    next_lower_frame = EMPTY_CELL;
}

  // Upwards there will be a tree
  if (next_upper_frame != EMPTY_CELL) {
    upper_map[HORISONTAL_POSITION_PLAYER] = next_upper_frame;
    if (current_upper_frame == EMPTY_CELL) {
      colision = 0;  
    } else {
      colision = 1;  
    }
  }

  // Downwards there will be a tree
  if (next_lower_frame != EMPTY_CELL) {
    lower_map[HORISONTAL_POSITION_PLAYER] = next_lower_frame;
    if (current_lower_frame == EMPTY_CELL) {
      colision = 0;  
    } else {
      colision = 1;  
    }
  }

  // Calculating how many trees I will have to print the timer from the beginning of the game
  if (time > 99) {
    count_numbers = 3;  
  } else if (time > 9) {
    count_numbers = 2;  
  } else if (time <= 9) {
    count_numbers = 1;  
  }
  
  // Map drawing
  lower_map[16] = '\0';
  upper_map[16] = '\0';
  char caracter = upper_map[16 - count_numbers];
  upper_map[16 - count_numbers] = '\0';
  lcd.setCursor(0,0);
  // Upper part of the map
  lcd.print(upper_map);
  upper_map[16 - count_numbers] = caracter;
  lcd.setCursor(0,1);
  // lower part of the map with the initial position of the player
  lcd.print(lower_map);

  // Timer shown on the top right
  lcd.setCursor(16 - count_numbers,0);
  lcd.print(time);
  upper_map[HORISONTAL_POSITION_PLAYER] = current_upper_frame;
  lower_map[HORISONTAL_POSITION_PLAYER] = current_lower_frame;
  
  return colision;
}

// In this function the next step of the map is running
void map_animation(char* map, byte new_frame) {
  int index = 0;
  char next_frame, current_frame;
  for(index = 0; index < 16; index++){
    current_frame = map[index];
    
    if (index == 15) {
      next_frame = new_frame; 
    } else {
      next_frame = map[index + 1];
    }
    
    switch (current_frame) {
      // If the current square is an empty cell
      case EMPTY_CELL:
        if (next_frame == FULL_TREE) {
          // If the next tree is a full tree, then, in the next frame,
          // it will move one position to the left, so the current tree
          // will be filled only on the right half.
          map[index] = RIGHT_TREE;
          } else {
          // If the next tree is only half full (the right half), then, in the next frame,
          // by moving the trees to the left, the current tree will remain empty, and if the next tree 
          // was already empty, the current tree will remain empty.
          map[index] = EMPTY_CELL;  
        }
        
        break;
      case FULL_TREE:
          // If the next tree is empty, by moving the trees to the left,
          // it will be half full (the left half).
        if (next_frame == EMPTY_CELL) {
          map[index] = LEFT_TREE;  
        } else {
          // If the next tree is not empty, then, by moving the trees to the left,
          // it will remain full in the next frame.
          map[index] = FULL_TREE;  
        }
        
        break;
        // If the current tree is only half full on the right side, it means that
        // a full tree follows it, so in the next frame, it will remain full.
      case RIGHT_TREE:
        map[index] = FULL_TREE;
        break;
      // If the current tree is only half full on the left side, it means that
      // the next tree will be empty, so in the next frame, it will become empty.
      case LEFT_TREE:
        map[index] = EMPTY_CELL;
        break;
    }
  }
}

// Interruption for timer
ISR(TIMER1_COMPA_vect) {
  if ((counter >> 3) == 20) {
    running_game = false;
  } else if (running_game == true && pause == false) {
    counter++;
  }
}

// Interruptions for buttons
ISR(INT0_vect) {
   int buttonState = digitalRead(PIN_BUTON);
   
   if (buttonState == HIGH) {
      buton_activat = 0;
      Serial.println("Button 1 pressed! PIN 2");
      beginning_message_game = false;
   } else {
      buton_activat = 1;
   }
}

ISR(INT1_vect){
    int buttonState_1 = digitalRead(PIN_BUTON_1);
    if(buttonState_1 == HIGH) {
      buton_activat_1 = 0;
      Serial.println("Button 2 pressed! PIN 3");
   } else {
      buton_activat_1 = 1; 
   }
}

void setup(){
  Serial.begin(9600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BUTON, INPUT);
  pinMode(PIN_BUTON_1, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);  
  pinMode(LED2, OUTPUT);
  digitalWrite(PIN_BUTON, HIGH);
  digitalWrite(PIN_BUTON_1, HIGH);
  pinMode(PIN_AUTOPLAY, OUTPUT);
  digitalWrite(PIN_AUTOPLAY, HIGH);
  
  map_initialisation();
  
  lcd.begin(16, 2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.createChar(DINO_HEAD, dinoHead);
  lcd.createChar(DINO_BODY_1, dinoBody1);
  lcd.createChar(DINO_BODY_2, dinoBody2);
  lcd.createChar(DINO_JUMP, dinoJump);

  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  
  // I generate the interruption with a frequency of 5 HZ for timer
  // I calculated according to the laboratory
  OCR1A = 12499;            // compare match register 16MHz/256/5Hz-1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  // Activating the necessary registers for the interruption of the first button
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC00);

  // Activating the necessary registers for the interruption of the second button
  EIMSK |= (1 << INT1);    
  EICRA |= (1 << ISC10);
  
  sei();
}

void loop(){
  static byte player_pos = DOWN_POSITION_PLAYER_1;
  static byte obstacol_urmator = EMPTY_CELL;
  static byte obstacle_lifespam = 1;
  
  if (running_game == false && pause == false) {
    map_initialisation();
    if ((counter >> 3) == 20) {
      lcd.clear();
      lcd.print("YOU WON!");
      digitalWrite(LED1, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("ANOTHER ONE ?");
      delay(2000);
      digitalWrite(LED1, LOW);
      counter = 0;
    }

    lcd.clear();
    lcd.print("BEGIN ?");
    lcd.setCursor(0, 1);
    lcd.print("PRESS START !");
    
    if (buton_activat == 1) {
      buton_activat = 0;
      player_pos = DOWN_POSITION_PLAYER_1;
      running_game = true;
      counter = 0;
    }

    if (thisNote == notes * 2) {
      thisNote = 0;  
    }
    
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration * 0.9);

    // stop the waveform generation before the next note.
    noTone(buzzer);

    thisNote += 2;
    
    return;
  } else if(pause == true) {
      lcd.clear();
      lcd.print("GAME PAUSED!");
      digitalWrite(LED2, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Score: ");
      lcd.print(counter >> 3);
      Game_Over_Melody(gameOverMelody, 10);
      delay(2000);
      digitalWrite(LED2, LOW);
      pause = false;
    
   }
  
  
  // Adding a tree on the right side
  obstacle_lifespam -= 1;
  if (obstacle_lifespam == 0) {
    if (obstacol_urmator == 0) {
      if (random(2) != 0) {
        obstacol_urmator = DOWN_BLOC;  
      } else {
        obstacol_urmator = UP_BLOC;
      }
      
      obstacle_lifespam = 2 + random(7);
    } else {
      obstacol_urmator = 0;
      obstacle_lifespam = 7 + random(7);
    }
  }

  //Moving trees on the left
  if (obstacol_urmator == DOWN_BLOC) {
    map_animation(lower_map, FULL_TREE);   
  } else {
    map_animation(lower_map, EMPTY_CELL);   
  }

  if (obstacol_urmator == UP_BLOC) {
    map_animation(upper_map, FULL_TREE);   
  } else {
    map_animation(upper_map, EMPTY_CELL);
  }
    
  if (buton_activat == 1 && buton_activat_1 == 0) {
    if (player_pos <= DOWN_POSITION_PLAYER_2) {
      player_pos = RUNNING_POSITION_PLAYER_1;
    }

    buton_activat = 0;
  } 
  if (player_design(player_pos, upper_map, lower_map, counter >> 3) == 1) {
    // If there is a collision the game stops
    running_game = false;
    lcd.clear();
    lcd.print("LOST ! AGAIN ?");
    digitalWrite(LED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(counter >> 3);
    Game_Over_Melody(gameOverMelody, 10);
    delay(2000);
    digitalWrite(LED, LOW);
  } else if(buton_activat_1 == 1){
    pause = true;
  } else {
    if (player_pos == DOWN_POSITION_PLAYER_2 || player_pos == FALLING_PLAYER_3) {
      player_pos = DOWN_POSITION_PLAYER_1;
    } else if ((player_pos >= JUMPING_PLAYER_1 && player_pos <= JUMPING_PLAYER_3) 
                && lower_map[HORISONTAL_POSITION_PLAYER] != EMPTY_CELL) {
      player_pos = UP_PLAYER_1;
    } else if (player_pos >= UP_PLAYER_1 && lower_map[HORISONTAL_POSITION_PLAYER] == EMPTY_CELL) {
      player_pos = JUMPING_PLAYER_3;
    } else if (player_pos == UP_PLAYER_2) {
      player_pos = UP_PLAYER_1;
    } else {
      player_pos++;
    }

    if (lower_map[HORISONTAL_POSITION_PLAYER + 2] == EMPTY_CELL) {
      digitalWrite(PIN_AUTOPLAY, HIGH);   
    } else {
      digitalWrite(PIN_AUTOPLAY, LOW);    
    }
  }
  
  delay(100);
}
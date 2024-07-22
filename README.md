# Flappy LED Game

Welcome to Flappy LED, a fun and engaging game implemented on the LED matrix of the Arduino UNO R4. This game is designed as a demonstration for the Plug and Make kit of Arduino. Guide your LED through moving walls and try to achieve the highest score!

## Features

- **Menu Mode**: Select between different game control modes.
- **Encoder Mode**: Control the LED bird using a rotary encoder.
- **TOF Mode**: Control the LED bird using a Time-of-Flight distance sensor.
- **Animations**: Enjoy cool animations during idle and game transitions.
- **Buzzer Sounds**: Listen to sounds during gameplay (can be muted).
- **Automatic Reset**: The game resets after a defined period of inactivity.

## Setup

### Required Libraries

- `ArduinoGraphics`
- `Arduino_LED_Matrix`
- `Modulino`
- `Scheduler`

### Hardware Requirements

- Arduino UNO R4
- Arduino LED Matrix
- Modulino Knob (Rotary Encoder)
- Modulino Distance Sensor (TOF)
- Modulino Buttons
- Modulino Buzzer

### Connections

Ensure all hardware components are properly connected to the Arduino UNO R4 as per their respective instructions.

## Installation

1. **Download and Install the Required Libraries:**

   Use the Arduino Library Manager to install the required libraries listed above.

2. **Upload the Code:**

   Upload the provided Flappy LED game code to your Arduino UNO R4.

## Game Modes

### Menu Mode

In this mode, you can select the control method for the game.

- **Press Button A** to select **Encoder Mode**.
- **Press Button B** to toggle mute for the buzzer.
- **Press Button C** to select **TOF Mode**.

### Encoder Mode

Control the LED bird using the rotary encoder. Rotate the knob to move the bird up or down.

### TOF Mode

Control the LED bird using the TOF distance sensor. Move your hand closer or farther from the sensor to move the bird up or down.

## Game Play

1. **Start the Game:**

   Once you select a mode, the game starts automatically.

2. **Control the Bird:**

   Use the selected control method to navigate the bird through moving walls.

3. **Score Points:**

   Successfully navigate through the gaps in the walls to score points. The game speed and difficulty increase as you score more points.

4. **Game Over:**

   The game ends if the bird collides with a wall. Your final score will be displayed, and the game will reset.

## Code Explanation

The provided code consists of various functions and classes that manage the game logic, controls, and display animations. Here's a brief overview of the main sections:

### Global Variables

- **Game Settings:** Define the game settings like height, width, initial player position, and other game parameters.
- **Game Objects:** Create instances of the required components like LED matrix, encoder, distance sensor, buttons, and buzzer.
- **Game State:** Track the game state variables such as score, player position, and game mode.

### Setup Function

Initializes the game components, starts the required loops for menu and control modes, and displays the introduction message.

### Loop Function

Handles the main game logic, including wall movement, collision detection, and score updating.

### Control Loops

- **menuLoop:** Manages the menu navigation and mode selection.
- **playerLoop:** Handles the player movement in Encoder Mode.
- **distanceLoop:** Handles the player movement in TOF Mode.

### Auxiliary Functions

- **introduction_message:** Displays the introduction message or animation.
- **show_idle_animation:** Displays idle animations when the game is in the menu mode.
- **play_wall_sound:** Plays sound when the bird successfully navigates through a wall.
- **play_end_sound:** Plays sound when the game ends.
- **adapt_game_level:** Adjusts the game difficulty based on the score.
- **print_score:** Displays the final score after game over.
- **clear_text:** Clears the text from the LED matrix.
- **reset_global_variables:** Resets all game variables after game over.

## Customization

Feel free to modify the game settings, animations, and sounds to customize the game experience. You can adjust the difficulty levels, wall size, and other parameters as per your preference.

## License

This project is open-source and licensed under the MIT License. You are free to use, modify, and distribute the code as long as proper credit is given to the original author.

## Inspiration

This project has been inspired by a similar project on Arduino Project Hub: [How to Play Flappy Bird Like Game with Your Voice](https://projecthub.arduino.cc/mad_mcu/how-to-play-flappy-bird-like-game-with-your-voice-e5b6eb).

Enjoy playing Flappy LED on your Arduino UNO R4!
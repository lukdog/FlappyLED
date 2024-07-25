# FlappyLED

FlappyLED is a modern take on the classic Flappy Bird game, designed to be played on the Arduino UNO R4 LED matrix. The game can be controlled using either a rotary knob or a distance sensor (TOF). This project is inspired by another project on [Arduino Projecthub](https://projecthub.arduino.cc/mad_mcu/how-to-play-flappy-bird-like-game-with-your-voice-e5b6eb).

## Table of Contents

- [Introduction](#introduction)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Game Modes](#game-modes)
- [Animations and Sounds](#animations-and-sounds)
- [Contributing](#contributing)
- [License](#license)

## Introduction

FlappyLED is a simple yet addictive game where you navigate a pixel (representing the bird) through a series of walls with gaps. The goal is to pass through as many walls as possible without crashing. You can control the bird's movement using a rotary knob or a distance sensor.

## Hardware Requirements

To play FlappyLED, you'll need the following components from the Arduino Plug&Make kit:

- Arduino UNO R4 WIFI
- Modulino Knob (for rotary control)
- Modulino Distance Sensor (TOF) (for distance control)
- Modulino Buttons (for mode selection and mute toggle)
- Modulino Buzzer (for sound effects)

## Software Requirements

- Arduino IDE
- ArduinoGraphics library
- Arduino_LED_Matrix library
- Modulino library
- Scheduler library

## Installation

1. **Clone the repository:**
   ```sh
   git clone https://github.com/lukdog/FlappyLED.git
   ```

2. **Open the Arduino IDE and load the `FlappyLED.ino` sketch.**

3. **Install the required libraries:**
   - Go to `Sketch` > `Include Library` > `Manage Libraries`.
   - Search for and install `ArduinoGraphics`, `Arduino_LED_Matrix`, `Modulino`, and `Scheduler`.

4. **Upload the sketch to your Arduino UNO R4:**
   - Connect your Arduino UNO R4 to your computer.
   - Select the correct board and port from `Tools` menu.
   - Click the upload button.

## Usage

### Starting the Game

1. **Power on your Arduino UNO R4.**
2. **The LED matrix will display a welcome message.**
3. **Use the buttons to select the game mode:**
   - Press the left button (A) to play using the rotary knob.
   - Press the right button (C) to play using the distance sensor.
   - Press the middle button (B) to toggle mute.

### Playing the Game

#### Using the Rotary Knob
- Rotate the knob to move the bird up and down.

#### Using the Distance Sensor
- Move your hand closer to or farther from the sensor to control the bird's vertical position.

## Game Modes

There are two main game modes in FlappyLED:

1. **Rotary Knob Mode:**
   - Control the bird by rotating the knob.

2. **Distance Sensor Mode:**
   - Control the bird by moving your hand in front of the distance sensor.

## Animations and Sounds

FlappyLED includes various animations and sound effects to enhance the gaming experience:

- **Welcome Message and Idle Animation:** Displayed when the game starts or is in the menu mode.
- **Flappy Animation:** Played during the game.
- **Crash Animation:** Displayed when the bird crashes into a wall.
- **Mode Selection Animation:** Helps in choosing the game mode.
- **Sound Effects:** Played during game events like passing through walls or crashing. These can be muted using the middle button.

## Configuration

You can enable or disable specific features by modifying the `#define` statements in the code. Here are the available options:

- **BUTTONS**: Enable the use of buttons for selecting game modes and toggling mute. If disabled, the game will automatically start if a rotation is performed on the knob or if a presence is noticed in front of the distance sensor.
- **ENCODER_MODE**: Enable the rotary knob mode for controlling the bird.
- **TOF_MODE**: Enable the distance sensor mode for controlling the bird.
- **BUZZER**: Enable sound effects using the buzzer.
- **ANIMATIONS**: Enable animations for welcome message, game events, and mode selection.
- **RESET_TIME**: If enabled, automatically reset the game after a certain period of inactivity.

To disable a feature, simply comment out the corresponding `#define` line in the code. For example, to disable the use of buttons, change:

```cpp
#define BUTTONS
```

to:

```cpp
// #define BUTTONS
```

Enjoy playing FlappyLED! Happy flying!
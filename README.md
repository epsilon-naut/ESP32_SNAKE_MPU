# _SNAKE FOR THE ESP32_

A simple snake game made for the ESP32. Since I didn't have any buttons, only an MPU6050 accelerometer on hand from a previous project, I opted to use tilt controls instead of the traditional four-button setup. Drivers for both the ST7735 and MPU6050 were written by myself.

The snake is encoded using a simple linked list to mitigate the risk of the ESP32 not being able to find a single block of memory large enough to store the position of every snake segment. Collision checking for the snake is done by iterating over every node in the linked list, simultaneously starting from the front and the back. I did consider using only two dots to encode the snake (one for the head and one for the tail) to reduce memory, but that would make collision checking much trickier. The apples are generated pseudo-randomly using the initial z acceleration measured by the MPU6050. 

A GIF demonstrating snake movement, the pseudo-random generation of apples and snake growth:
<img width="400" height="711" alt="IMG_2015(1)" src="https://github.com/user-attachments/assets/c1bfa359-f4ed-40c5-aed0-dc7bacd35132" />

A GIF demonstrating the game ending when the snake collides with the border:
<img width="400" height="711" alt="IMG_2015(2)" src="https://github.com/user-attachments/assets/218c906d-02c4-4a4b-8d04-095faaac802f" />








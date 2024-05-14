# Operating_System_Simulation
Operating system simulation for project my mini OS, programmed on Ubuntu Linux.
About the Project
1. Introduction
The program presented is a simulation of a basic operating system environment implemented in C++. It leverages POSIX threads (pthread) and system calls to manage processes, resources, and user interactions within a terminal-based interface. The purpose of this simulation is to demonstrate fundamental concepts of process scheduling, resource allocation, and inter-process communication.
2. Sequence of Operations
The code follows a structured sequence of operations:
Initialization and Setup: The main() function initializes system resources such as RAM, storage, and CPU cores based on user input. It then launches threads (pthread) to handle process dispatching, switching, and inter-terminal communication.
Process Dispatching: The dispatch() function manages the lifecycle of processes using queues (readyQueue and waitingQueue). It dispatches processes for execution using execvp() to launch programs within separate terminal instances (gnome-terminal), ensuring process isolation.
User Interface: The printMainMenu() function provides a menu interface listing available actions for the user, including running games, utilities, or terminating the simulated OS.
Resource Monitoring: The displayRunningProcs() function reports system resource utilization, including RAM, storage, CPU cores, and currently running processes. It utilizes ANSI escape sequences for text formatting in the terminal.
Resource Management: Functions like delete_pipefile() manage resources such as named pipes (moyemoyeFifo) used for inter-process communication. They employ system calls (system()) to interact with the underlying operating system.
3. Purpose
The purpose of this code is primarily aimed at illustrating key concepts of operating system design and implementation. It provides hands-on experience with process management, thread synchronization, resource allocation, and system-level interactions using C++ and POSIX APIs. By simulating an OS-like environment, the code offers insights into the complexities of real-world operating systems, albeit in a simplified and controlled context.
4. Conclusion
In conclusion, the presented code offers a practical exploration of operating system fundamentals, serving as a foundation for understanding process scheduling, resource management, and user interaction within a software-based system. It underscores the importance of concurrency, system calls, and resource optimization in building efficient and robust operating systems.
5. How the Code Works:
The main file initializes shared memory and presents a user menu.
Upon selecting an option, a separate thread is created to execute the chosen process.
Each process interacts with the user or performs its task while updating the shared memory to reflect the RAM usage.
Once the process is complete, the main file waits for the thread to finish execution before detaching the shared memory segment and exiting.



Functions Explanations
1. Main File:
   - This file is the entry point of the program.
   - It prompts the user to enter the number of cores, RAM size, and disk size.
   - It creates a shared memory segment and initializes it with the RAM size entered by the user.
   - After printing a hardware properties, it displays a user menu with multiple options.
   - Depending on the user's choice, it creates a separate thread to execute the corresponding process.
   - Each thread is responsible for running its respective process using the `system` function to open a new terminal window and execute the compiled program.


2. File Creator
Purpose: This program allows the user to create a file by specifying a name and extension (either .txt or .dat). It checks if the file already exists and creates it if not. Upon completion, it prompts the user to press a key to exit.
Key Features:
User input for file name and extension selection.
File creation or notification if the file already exists.
Integration with a resource deallocation system upon completion.
3. File Deleter
Purpose: This program enables the user to delete a specified file (either .txt or .dat). It prompts the user to enter a file name and extension, checks for its existence, and deletes it if found.
Key Features:
User for file name and extension selection.
File deletion process with appropriate feedback.
Integration with a resource deallocation system upon completion.
4. Hangman Game
Purpose: Implements a simple command-line version of the Hangman game. The program selects a word randomly from a predefined list, allows the player to guess letters, and tracks incorrect attempts. It continues until the player decides to exit.
Key Features:
Random word selection and initialization of a hidden word.
Letter guessing mechanism with feedback on correct and incorrect attempts.
Game loop and option to exit upon completion.
5. Helper Process for Resource Deallocation
Purpose: This program serves as a helper to compile and execute a resource deallocation script based on command-line arguments provided to it.
Key Features:
Dynamically constructs commands for resource deallocation.
Compiles and executes the deallocation script based on input arguments.
6. Kernel Panel
Purpose: The program displays a list of running processes with a specific command line in the /proc directory. It allows the user to terminate selected processes by entering their names or PIDs.
Key Features:
Interacts with the Linux kernel to fetch process information.
Provides an interface to terminate processes based on user input.
Integrates with a resource deallocation system for cleanup.
7. Notepad
Purpose: Simulates a basic text editor functionality where users can view the contents of a file (note.txt), append text to it, and save changes. The program handles file operations and includes integration with a resource deallocation process.
Key Features:
File reading, appending, and saving operations.
Integration with a resource deallocation system upon completion.
8. Number Guessing Game
Purpose: Implements a simple number guessing game where the player attempts to guess a randomly generated number between 1 and 100. The game provides feedback on each guess and allows the player to play multiple rounds.
Key Features:
Random number generation and user input handling.
Game loop with iterative play until the player decides to exit.
Integration with a resource deallocation system upon completion.
9. Quotes Generator
Purpose: Displays a random quote from a predefined list each time the program is executed. The quotes cover a variety of inspirational and motivational messages.
Key Features:
Random selection of quotes from a predefined list.
Displays the selected quote with each execution.
Integration with a resource deallocation system for cleanup.
10. File Renamer
Purpose: Allows the user to rename an existing file by selecting the file name and extension. If the specified file exists, the program prompts for a new name and performs the renaming operation.
Key Features:
User input for file name and extension selection.
Renaming process with feedback on success or failure.
Integration with a resource deallocation system upon completion.
11. Video Player
Purpose: This program simulates a video player by executing the mpv command with a specified video file name (.mpv). It checks for file existence, plays the video, and allows the user to stop playback upon input.
Key Features:
Checks for video file existence and plays it using mpv.
Enables the user to stop video playback interactively.
Integrates with a resource deallocation system upon completion.
12. Music Player
Purpose: Similar to the video player, this program simulates an audio player by executing the mpg123 command with a specified audio file name (.mp3). It checks for file existence, plays the audio, and allows the user to stop playback upon input.
Key Features:
Checks for audio file existence and plays it using mpg123.
Enables the user to stop audio playback interactively.
Integrates with a resource deallocation system upon completion.
13. Clock
Purpose: Displays a clock on the terminal using the tty-clock command. It clears the screen and waits for user input before terminating.
Key Features:
Utilizes the tty-clock command to display a clock.
Clears the screen and waits for user input before exiting.
Integrates with a resource deallocation system upon completion.
14. Calculator
Purpose: Implements a simple command-line calculator with basic arithmetic operations. It prompts the user to choose an operation, performs the calculation, and allows the user to exit upon completion.
Key Features:
Supports addition, subtraction, multiplication, and division operations.
User-friendly interface for choosing operations and entering operands.
Integration with a resource deallocation system upon completion.
15. Calendar
Purpose: Generates a calendar for a specified month and year using Zeller's Congruence algorithm. It determines the day of the week for the first day of the month and prints the calendar accordingly.
Key Features:
Accepts user input for the year and month.
Utilizes Zeller's Congruence to calculate the weekday of the first day.
Prints a formatted calendar for the specified month and year.
Integration with a resource deallocation system upon completion.







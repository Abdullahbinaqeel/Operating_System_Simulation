#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iomanip>
#include <sys/ioctl.h>
#include <pthread.h>
#include <vector>
#include <string>
#include <semaphore.h>
#include <dirent.h>
#include <cstring>
#include <ncurses.h>

using namespace std;

#define MAX 15

string process_sequence_name[MAX] = {"TOWEROFHANOI", "CALCULATOR", "HANGMAN", "NUMBERGUESSING", "MUSICPLAYER", "QUOTES", "CREATE", "DELETE", "COPY", "CALENDER", "CLOCK", "VIDEOPLAYER", "RENAME", "NOTEPAD", "KERNEL"};
const char *moyemoyeFifo = "/tmp/MONMOI/fifo";
long int ramSize = 0, storageDevice = 0, cores = 0;
long int need[MAX] = {0};
int core_threads = 0;
int active_processes[MAX] = {0};
int **active_cores;
bool mode;
int k_choice;
vector<int> readyQueue;
vector<int> waitingQueue;

pthread_t ptid;
pthread_t dispatcher;
pthread_t switcher;

sem_t semaphore;

void printMainMenu();
void displayRunningProcs();
//void resource_allocator(int ramSize, int storageDevice, int cores, int core_threads);

bool check_free_cores()
{
    for (int i = 0; i < cores; ++i)
        for (int j = 0; j < core_threads; ++j)
            if (active_cores[i][j] == -1)
                return true;

    return false;
}

void assign_core(int process)
{
    for (int i = 0; i < cores; ++i)
        for (int j = 0; j < core_threads; ++j)
            if (active_cores[i][j] == -1)
            {
                active_cores[i][j] = process;
                return;
            }
}

void dealloc_core(int process)
{
    for (int i = 0; i < cores; ++i)
        for (int j = 0; j < core_threads; ++j)
            if (active_cores[i][j] == process)
            {
                active_cores[i][j] = -1;
                return;
            }
}

void display_running_cores(void)
{
    for (int i = 0; i < cores; ++i)
    {
        for (int j = 0; j < core_threads; ++j)
        {
            if (active_cores[i][j] != -1)
            {
                cout << "Core " << i + 1 << " is executing:\n"
                     << "Process \"" << process_sequence_name[active_cores[i][j]] << "\"\n"
                     << "--------------------------------\n\n";
            }
        }
    }
}

void delete_pipefile()
{
    // This command will delete the file moyemoyeFifo which exists in tmp in Ubuntu and acts as a named pipe.
    // We are deleting it because we'll create our own.
    if (access(moyemoyeFifo, F_OK) != -1)
    {
        system("rm /tmp/moyemoyeFifo");
        if (unlink(moyemoyeFifo) != 0)
        {
            perror("Error deleting file");
        }
    }
}

void release_resources(char *str)
{
    string str1, str2;
    int index = -1;

    for (int i = 0; i < 15; ++i)
    {
        str1 = process_sequence_name[i];
        str2 = str;

        if (str1 == str2)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        system("clear");
        dealloc_core(index);
        sem_wait(&semaphore);
        active_processes[index] -= 1;
        ramSize += need[index];
        sem_post(&semaphore);
        displayRunningProcs();
        printMainMenu();
    }
}

void *thread_for_inter_terminal_comm(void *arg)
{
    int fd;
    const char *moyemoyeFifo = "/tmp/moyemoyeFifo";
    mkfifo(moyemoyeFifo, 0666); // giving permissions

    char arr1[80], arr2[80];
    while (true)
    {

        fd = open(moyemoyeFifo, O_RDONLY);
        // Read from FIFO
        read(fd, arr1, sizeof(arr1));
        string temp = "notify-send '" + string(arr1) + " Has Been Closed!'";
        system(temp.c_str());
        release_resources(arr1);
        close(fd);
    }
}

void print_centered(string message) 
{
  // Get terminal width
  int max_x = 80; // Replace with actual terminal width retrieval (if possible)

  // Centered printing function (using spaces for padding)
//  void print_centered_line(const string& line) 
//  {
//    int padding = (max_x - line.length()) / 2;
//    cout << string(padding, ' ') << line << endl;
//  }

  // Print ASCII art and messages using helper function
//  print_centered_line("m   m   mmmm  m    m   mmmmmm    m   m   mmmm  m    m   mmmmmm");
  // ... (remaining ASCII art lines)
//  print_centered_line(message); // Print initialization message

  // Print loading bar and simulate progress
  cout << "[--------------------]" << endl;
  for (int i = 0; i < 20; i++) 
  {
    cout << (i + 1) * 5 << "%%";
    if (i % 2 == 0) {
      cout << " ";
    } else {
      cout << "*";
    }
    cout << endl;
    usleep(100000); // Simulate loading delay
  }

  // Simulate further actions (replace with your actual code)
  cout << "Welcome to the MOYE MOYE!" << endl;
  cout << "Loading core components..." << endl;
  sleep(1); // Simulate loading time
  cout << "Loading complete!" << endl;
  usleep(2000000);
}

void shut_down()
{
    cout << "Shutting down the system...";
    cout << "Goodbye!";
    // change color to yellow
    system("tput setaf 3");
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⣉⣙⣷⡦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢤⣴⠖⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣉⠛⠓⠶⢤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    sleep(1);
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣽⡿⠟⠁⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠙⠛⠷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣤⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⡶⠶⠛⠉⠉⠉⠉⠁⠀⠀⠀⠀⠉⠙⠿⠓⠒⠶⢺⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⢶⡶⠶⠶⠶⠖⠖⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    sleep(1);
    cout << "⠘⣧⡀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠈⠻⢤⣼⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⢻⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡀⢀⣿⡿⠶⠶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    cout << "⠀⠀⠀⠀⢸⡇⠀⣀⣀⠀⢀⣀⣤⡀⠀⠀⠀⠀⠸⠶⠶⠚⠛⠋⢹⣿⣿⣟⣉⠉⠒⠀⠻⣦⣠⣤⣤⣤⣄⣀⠀⠀" << endl;
    cout << "⠀⢀⣤⢾⣿⣷⣶⡍⠙⠙⠛⠋⠉⠀⠀⢴⡶⠀⠀⠀⠀⢀⣠⡶⠟⠛⠛⣷⠀⠉⠁⠀⠀⠈⣧⡀⠀⠩⣀⠈⢹⣆" << endl;
    cout << "⠀⣠⠔⢉⡴⢿⣿⡟⠛⠛⠛⠶⣤⣀⠀⠀⠀⠀⠀⠀⣴⡿⠋⠀⠀⠀⢀⡉⠀⠀⠀⠀⢀⣼⠛⠛⢛⣿⡿⠀⣾⡟" << endl;
    sleep(1);
    cout << "⠀⠁⣰⠋⢀⡿⠁⠀⠀⠀⠀⠀⠀⠉⠻⣦⡀⠀⠀⣼⠟⠀⠀⠀⢀⣠⣾⢁⣀⣤⣴⡶⠟⣁⣴⠞⠋⠉⢀⣼⣿⠁" << endl;
    cout << "⠀⠀⠉⠀⠈⠷⣄⡀⠀⠀⠀⠀⠀⠀⠀⠈⢿⡗⠚⡏⠀⢀⣤⡶⠛⠋⠉⠉⠉⠉⠀⣠⣾⠟⢁⣀⣤⣶⣿⠟⠁⠀" << endl;
    cout << "⠀⠀⠀⠀⠀⠀⠀⠈⠉⠑⠲⠤⣄⣦⣤⡴⠞⠁⠀⠉⠙⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠹⠿⠾⠾⠟⠛⠁⠀⠀⠀⠀" << endl;
    sleep(5);
    system("tput sgr0");
}

void *process_shifter(void *argv)
{
    int process = -1;

    while (true)
    {
        bool foundProcess = false;

        for (int i = 0; i < waitingQueue.size(); ++i)
        {
            if (need[waitingQueue[i] - 1] <= ramSize && check_free_cores())
            {
                process = waitingQueue[i];
                waitingQueue.erase(waitingQueue.begin() + i);
                readyQueue.push_back(process);
                foundProcess = true;
                break;
            }
        }

        if (!foundProcess)
        {
            break;
        }
    }
    return NULL;
}

void displayRunningProcs()
{
    bool noProcFlag = true;
    system("tput setaf 2"); // Set color to green
    cout << "\n⏲  ⏲  ⏲  ⏲ RESOURCE MONITOR ⏲  ⏲  ⏲  ⏲\n\n";
    cout << "\tRemaining RAM ->>> " << ramSize << " MB" << endl;
    cout << "\tRemaining Storage -->> " << storageDevice << " MB" << endl;
    cout << "\tRemaining Cores -->> " << cores << endl;
    display_running_cores();

    for (int i = 0; i < 15; ++i)
    {
        if (active_processes[i])
        {
            cout << "    " << "\033[1;33m" << process_sequence_name[i] << "\033[0m" << " is currently running " << "\033[1;34m" << active_processes[i] << "\033[0m" << " times" << endl;
            noProcFlag = false;
        }
    }

    if (noProcFlag)
    {
        cout << "\nNo process is running.\n\n";
    }
    system("tput sgr0");
}

void *dispatch(void *argv)
{
    pid_t pid;
    string str;
    string temp;
    int active = -1;
    // thread which checks the ready queue and calls the exec file as soon as it gets resources
    while (true)
    {
        if (!readyQueue.empty())
        {
            active = readyQueue.front();
            readyQueue.erase(readyQueue.begin());
        }
        else
            active = -1;

        if (active != -1)
        {
            active_processes[active - 1]++;
            ramSize -= need[active - 1];
            cout << process_sequence_name[active - 1] << " has been dispatched from the ready queue!" << endl;
            assign_core(active - 1);
            system("clear");

            displayRunningProcs();
            // printMainMenu();

            switch (active)
            {
            case 1:
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./towerofhanoi.obj";

                    // Check if the executable file exists
                    if (access("towerofhanoi", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o towerofhanoi towerofhanoi.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 2: // Calculator
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./calculator.obj";

                    // Check if the executable file exists
                    if (access("calculator", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o calculator calculator.cpp");
                        return 0;
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 3:
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./hangman.obj";

                    // Check if the executable file exists
                    if (access("hangman", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o hangman hangman.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 4:
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./numberguessing.obj";

                    // Check if the executable file exists
                    if (access("numberguessing", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o numberguessing numberguessing.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 5: // Notepad
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./vplayer.obj";

                    // Check if the executable file exists
                    if (access("vplayer", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o vplayer vplayer.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 6:
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./quotes.obj";

                    // Check if the executable file exists
                    if (access("quotes", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ quotes.cpp -o quotes.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 7:
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./create.obj";

                    // Check if the executable file exists
                    if (access("create", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o create create.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }

                break;
            case 8:
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./delete.obj";

                    // Check if the executable file exists
                    if (access("delete", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o delete delete.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 9:
                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./copy.obj";

                    // Check if the executable file exists
                    if (access("copy", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o copy copy.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 10: // create

                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./calender.obj";

                    // Check if the executable file exists
                    if (access("calender", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o calender calender.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }

                break;
            case 11: // delete

                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./clock.obj -s";

                    // Check if the executable file exists
                    if (access("clock", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o clock clock.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 12: // rename

                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./tictactoe.obj";

                    // Check if the executable file exists
                    if (access("tictactoe", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o tictactoe tictactoe.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }

                break;
            case 13:

                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./rename.obj";

                    // Check if the executable file exists
                    if (access("rename", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o rename rename.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }

                break;
            case 14:

                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./notepad.obj";

                    // Check if the executable file exists
                    if (access("notepad", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o notepad notepad.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            case 15:

                pid = fork();
                if (pid == -1)
                {
                    cout << "Error in forking\n";
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    const char *command = "gnome-terminal";
                    const char *arg1 = "--";
                    const char *arg2 = "sh";
                    const char *arg3 = "-c";
                    const char *arg4 = "./kernel.obj";
                    // Check if the executable file exists
                    if (access("kernel", F_OK) == -1)
                    {
                        // Compile the program
                        system("g++ -o kernel kernel.cpp");
                    }

                    char *args[] = {const_cast<char *>(command), const_cast<char *>(arg1), const_cast<char *>(arg2), const_cast<char *>(arg3), const_cast<char *>(arg4), NULL};
                    execvp(command, args);
                }
                break;
            }
        }
    }
}

void printMainMenu()
{
    system("tput setaf 2");
    cout << "---------------------- USER DESKTOP  -------------------------\n";
    cout << "------------------------------------------------------------\n\n";
    cout << "Choose your next action...\n\n";
    cout << "1. Tower of Hanoi\n";
    cout << "2. Calculator\n";
    cout << "3. Hangman\n";
    cout << "4. Number Guessing\n";
    cout << "5. Music Player\n";
    cout << "6. Quote\n";
    cout << "7. Create a File\n";
    cout << "8. Delete a File\n";
    cout << "9. Copy a File\n";
    cout << "10. Calendar\n";
    cout << "11. Clock\n";
    cout << "12. Video Player\n";
    cout << "13. Rename a File\n";
    cout << "14. Notepad\n";
    cout << "15. Kernel Mode\n";
    cout << "16. Terminate the OS\n";
    cout << "\nEnter your choice: ";
    cout << endl;

    system("tput sgr0");
    return;
}
void scheduler(void)
{
    int choice;
    while (true)
    {
        displayRunningProcs();
        printMainMenu();
        cin >> choice;
        if (choice == 16)
        {
            system("clear");
            pthread_cancel(ptid);
            pthread_cancel(dispatcher);
            pthread_cancel(switcher);
            cout << "Terminating all the processes" << endl;
            sleep(2);
            system("clear");
            shut_down();
            sleep(2);
            system("killall -9 gnome-terminal-server");
            exit(0);
        }
        if (choice <= 16 && choice >= 1)
        {
            if ((ramSize >= need[choice - 1]) && check_free_cores())
            {
                // Ready Queue Implementation
                readyQueue.push_back(choice);
            }
            else
            {
                // Waiting Queue Implementation
                system("notify_send 'NOT ENOUGH SPACE'");
                waitingQueue.push_back(choice);
                continue;
            }
        }
    }
}


int main()
{
    delete_pipefile();
    system("clear");
    sem_init(&semaphore, 0, 1);
    cout << "Welcome to MOYE MOYE OPERATING SYSTEM \n";
    usleep(1000);
    cout << "by 21F9215\n22F3416\n22F3076\n";
    need[0] = 150; // need of hanoi process
    need[1] = 50;
    need[2] = 100;
    need[3] = 900;
    need[4] = 100;
    need[5] = 150;
    need[6] = 100;
    need[7] = 50;
    need[8] = 100;
    need[9] = 50;
    need[10] = 50;
    need[11] = 50;
    need[12] = 50;
    need[13] = 50;
    need[14] = 100;
    cout << "Press 0 for kernal mode and 1 for user mode" << endl;
    cin >> mode;
    int mod = 0, res;
    if (mode == 0)
    {
        system("notify-send 'Kernel Mode'");
        cout << "Enter 1 to upate resources or -1 to not" << endl;
        int c;
        cin >> c;
        if (c == 1)
        {
            for (int i = 0; i < MAX; i++)
            {
                cout << i << " " << process_sequence_name[i] << " " << need[i] << endl;
            }

            while (mod != -1)
            {
                cout << "Enter the number of the process which you want to change the resources for or -1 to exit" << endl;
                cin >> mod;
                if (mod != -1)
                {
                    cout << "Enter the the new resources" << endl;
                    cin >> res;
                    need[mod] = res;
                    cout << "to continue press 1 to exit press -1" << endl;
                    cin >> mod;
                }
            }
        }
    }
    system("clear");

 do
    {
        cout << "Enter the amount of RAM in GB: ";
        cin >> ramSize;
        cout << endl;

        cout << "Enter the size of HDD in GB: ";
        cin >> storageDevice;
        cout << endl;

        cout << "Enter the number of cores: ";
        cin >> cores;
        cout << endl;

        cout << "Enter the number of threads per core: ";
        cin >> core_threads;
        cout << endl;

        if (ramSize <= 0)
        {
            cout << "Invalid RAM size. Please enter a positive value." << endl;
            return 1;
        }

        if (storageDevice <= 0)
        {
            cout << "Invalid HDD size. Please enter a positive value." << endl;
            return 1;
        }

        if (cores <= 0)
        {
            cout << "Invalid number of cores. Please enter a positive value." << endl;
            return 1;
        }

        if (core_threads <= 0)
        {
            cout << "Invalid number of threads per core. Please enter a positive value." << endl;
            return 1;
        }

        if (core_threads <= 1)
        {
            cout << "Insufficient number of threads. Please enter a value greater than 1." << endl;
        }
        system("clear");

    } while (ramSize == 0 || storageDevice == 0 || core_threads <= 1 || cores == 0);

    ramSize = ramSize * 1024;
    storageDevice = storageDevice * 1024;

    
    system("clear");
    int status;
    pthread_create(&dispatcher, NULL, dispatch, NULL);
    pthread_create(&switcher, NULL, process_shifter, NULL);
    pthread_create(&ptid, NULL, thread_for_inter_terminal_comm, NULL);

    active_cores = new int *[cores];
    for (int i = 0; i < cores; ++i)
        active_cores[i] = new int[core_threads];
    for (int i = 0; i < cores; ++i)
        for (int j = 0; j < core_threads; ++j)
            active_cores[i][j] = -1;

    scheduler();
    pthread_join(ptid, NULL);
    pthread_join(dispatcher, NULL);
    pthread_join(switcher, NULL);

    return 0;
}

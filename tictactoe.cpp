#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h> 

using namespace std;

int main()
{
    string filename;
    cout << "Enter the name of the video file to play: ";
    getline(cin, filename);

    // Check if the file exists
    if (access((filename + ".mpv").c_str(), F_OK) != -1)
    {
        string command = "mpv " + filename + ".mpv &";
        system(command.c_str());

        cout << "Press any key to stop playing the video...";
        cin.ignore();
        cin.get();

        // Stop playing the video
        system("killall mpv");

         system("g++ -o deallocate dealloc_resource.cpp");
         system("./deallocate VIDEO_PLAYER");
    }
    else
    {
        cerr << "Error: File not found." << endl;
    }

    return 0;
}



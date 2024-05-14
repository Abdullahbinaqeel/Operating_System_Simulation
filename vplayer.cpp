#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h> // For access() function

using namespace std;

int main()
{
    string filename;
    cout << "Enter the name of the audio file to play: ";
    getline(cin, filename);

    // Check if the file exists
    if (access((filename + ".mp3").c_str(), F_OK) != -1)
    {
        string command = "mpg123 " + filename + ".mp3 &";
        system(command.c_str());

        cout << "Press any key to stop playing the audio...";
        cin.ignore();
        cin.get();

        // Stop playing the audio
        system("killall mpg123");

        // Perform deallocation
        system("g++ -o deallocate dealloc_resource.cpp");
        system("./deallocate VPLAYER");
    }
    else
    {
        cerr << "Error: File not found." << endl;
    }

    return 0;
}





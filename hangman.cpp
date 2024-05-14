#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
// Function to check if a character is a valid letter
bool isValidLetter(char letter)
{
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
}

// Function to initialize the word list
vector<string> initializeWordList()
{
    vector<string> wordList = {
        "hangman",
        "computer",
        "programming",
        "openai",
        "developer"};
    return wordList;
}

// Function to select a random word from the word list
string getRandomWord(const vector<string> &wordList)
{
    int index = rand() % wordList.size();
    return wordList[index];
}

// Function to initialize the hidden word
string initializeHiddenWord(const string &word)
{
    string hiddenWord(word.length(), '_');
    return hiddenWord;
}

// Function to update the hidden word based on the guessed letter
void updateHiddenWord(const string &word, string &hiddenWord, char letter)
{
    for (size_t i = 0; i < word.length(); i++)
    {
        if (word[i] == letter)
        {
            hiddenWord[i] = letter;
        }
    }
}

// Function to check if the hidden word has been completely guessed
bool isWordGuessed(const string &hiddenWord)
{
    return hiddenWord.find('_') == string::npos;
}

int main()
{
    int n = 1;
    cout << "to exit enter 0" << endl;
    cin >> n; 
    if (n == 0)
    {
        return 0;
        // Compile the deallocate resource file before running it
        system("g++ -o deallocate dealloc_resource.cpp");
        // Run the deallocate resource file with an argument CALCULATOR
        system("./deallocate HANGMAN");
        exit(0);
    }
    else
    {
        do
        {
            // Seed the random number generator
            srand(static_cast<unsigned int>(time(0)));

            // Initialize the word list
            std::vector<std::string> wordList = initializeWordList();

            // Select a random word
            std::string word = getRandomWord(wordList);

            // Initialize the hidden word
            std::string hiddenWord = initializeHiddenWord(word);

            // Initialize the guessed letters
            std::string guessedLetters;

            int attempts = 6; // Number of attempts allowed

            // Game loop
            while (attempts > 0 && !isWordGuessed(hiddenWord))
            {
                // Print the current status
                std::cout << "Word: " << hiddenWord << std::endl;
                std::cout << "Attempts left: " << attempts << std::endl;
                std::cout << "Guessed letters: " << guessedLetters << std::endl;

                // Get a letter guess from the player
                char guess;
                std::cout << "Enter your guess: ";
                std::cin >> guess;
                guess = tolower(guess);

                // Validate the guess
                if (!isValidLetter(guess))
                {
                    std::cout << "Invalid guess! Please enter a valid letter." << std::endl;
                    continue;
                }

                // Check if the letter has already been guessed
                if (guessedLetters.find(guess) != std::string::npos)
                {
                    std::cout << "You have already guessed that letter!" << std::endl;
                    continue;
                }

                // Add the guessed letter to the list
                guessedLetters += guess;

                // Check if the guessed letter is in the word
                if (word.find(guess) != std::string::npos)
                {
                    // Update the hidden word
                    updateHiddenWord(word, hiddenWord, guess);
                    std::cout << "Correct guess!" << std::endl;
                }
                else
                {
                    // Incorrect guess
                    attempts--;
                    std::cout << "Wrong guess!" << std::endl;
                }
                std::cout << std::endl;
            }

            // Print the final result
            std::cout << "Word: " << hiddenWord << std::endl;
            if (isWordGuessed(hiddenWord))
            {
                std::cout << "Congratulations! You guessed the word correctly." << std::endl;
            }
            else
            {
                std::cout << "Game over! You failed to guess the word. The word was: " << word << std::endl;
            }
            cout << "To exit enter 0" << endl;
            cin >> n;
            if (n != 0)
                system("clear");
        } while (n != 0);

        return 0;
        // Compile the deallocate resource file before running it
        system("g++ -o deallocate dealloc_resource.cpp");
        // Run the deallocate resource file with an argument CALCULATOR
        system("./deallocate HANGMAN");
    }
    }

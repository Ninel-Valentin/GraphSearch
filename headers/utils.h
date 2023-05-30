#include <iostream>
#include <conio.h>
#include <cstring>
#include <fstream>

// * Max line char for logging
const int lineLength = 50;
// ? Could not get programatically without using another library other than std
// * The count of sample files
const int samplesCount = 4;
// * Path of the password container
const char *passwordPath = "storage/password.txt";

// This function asks for the provided question and waits for the response Y/N to be pressed. Not case sensitive
bool GetQuestionAnswer(const char *question)
{
    // Show the question
    std::cout << question << std::endl;

    // Repeat this step as long as the function does not return a value;
    while (true)
    {
        char confirmation = getch();
        std::cout << std::flush;

        if (confirmation == 'Y' || confirmation == 'y')
        {
            std::cout << "Yes" << std::endl;
            return true;
        }
        else if (confirmation == 'N' || confirmation == 'n')
        {
            std::cout << "No" << std::endl;
            return false;
        }
        else
            std::cout << "Please enter a valid answer for the question..."
                      << std::endl
                      << question
                      << std::endl;
    }
}

// The function reads the password
char *ReadPassword(int &passLen)
{
    std::cout << "Welcome to the Graph Search application!"
              << std::endl
              << "Insert password: ";
    passLen = 0;
    char *password = new char[255]{};
    char ch;
    do
    {
        ch = getch();
        // Escape is pressed
        if (ch == 27)
        {
            // Clear the console and the password
            password = new char[255]{};
            passLen = 0;
            system("CLS");
            std::cout << "Welcome to the Graph Search application!"
                      << std::endl
                      << "Insert password: ";
        }
        // Backspace is pressed
        else if (ch == 8)
        {
            if (passLen > 0)
            {
                // Delete the last char
                password[--passLen] = '\0';
                // Clear and rewrite the console
                system("CLS");
                std::cout << "Welcome to the Graph Search application!"
                          << std::endl
                          << "Insert password: ";
                for (int i = 0; i < passLen; i++)
                    std::cout << "*";
            }
        }
        // Anything but the Backspace / Escape / Enter is pressed
        else if (ch != 13)
        {
            std::cout << "*";
            // strcat(password, &ch);
            password[passLen++] = ch;
        }
    } while (ch != 13);
    if (ch == 13)
        std::cout << std::endl;
    return password;
}


// This function encodes a char array
char *encodePassword(char *password, int passLen)
{
    char *encodedPass = new char[passLen * 3 + 1]{};
    for (int i = 0; i < passLen; i++)
    {
        char *encodedCharacter = new char[6]{};

        // Count how many digits the ASCII value has
        int countOfDigits = 0;
        int asciiCode = (int)password[i];
        while (asciiCode != 0)
        {
            countOfDigits++;
            asciiCode /= 10;
        }

        // Add as many zeros as required to the ASCII value for the total to have 3 chars total
        while (3 - countOfDigits)
        {
            strcat(encodedCharacter, "0");
            countOfDigits++;
        }

        // Create a temp string to cast the ascii int value to a char array
        char *temp = new char[4]{};
        itoa((int)password[i], temp, 10); // int to char array

        // Concatenate the strings
        strcat(encodedCharacter, temp);

        // Add the encodedChar to the encodedPass array
        strcat(encodedPass, encodedCharacter);
    }
    return encodedPass;
}

// This function decodes a char array
char *decodePassword(char *encodedPass, int passLen)
{
    char *decodedPass = new char[passLen / 3 + 1]{};
    for (int i = 0; i < passLen; i += 3)
    {
        char *charSection = new char[4]{};
        for (int j = 0; j < 3; j++)
            charSection[j] = encodedPass[j + i];

        int asciiChar = atoi(charSection); // char array to int
        // Cast the ASCII Value to the char value
        charSection[0] = asciiChar;
        // Remove everything after the first char
        charSection[1] = '\0';

        strcat(decodedPass, charSection);
    }
    return decodedPass;
}

// This function checks if the provided password by the ReadPassword function is correct
bool CheckForPassword()
{
    int passLen;
    char *password = ReadPassword(passLen);

    std::ifstream PassFile;
    PassFile.open(passwordPath);

    if (PassFile.is_open())
    {
        char *keyPassword = new char[passLen * 3 + 1];
        PassFile.getline(keyPassword, passLen * 3 + 1);

        char *encodedPassword = encodePassword(password, passLen);

        for (int i = 0; i < passLen * 3; i++)
            if (encodedPassword[i] != keyPassword[i])
                return false;
        return true;
    }
    else
    {
        std::cout << "Password text file not in place... The program will exit now."
                  << std::endl
                  << "Press any key to continue..."
                  << std::endl;
        getch();
        exit(0);
    }
}
#include <iostream>
#include <conio.h>

// Max line char for logging
const int lineLength = 50;

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

// ? Could not get programatically without using another library other than std
// * The count of sample files
const int samplesCount = 4;
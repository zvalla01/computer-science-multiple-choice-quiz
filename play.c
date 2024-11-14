#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"

extern volatile int interrupted;  //external interrupted flag

int play(quiz_t *quiz) {
    quiz->max = 8;
    int attempts = 3;  //set maximum attempts to 3
    int current_score = quiz->score;  //save the score from previous questions
    printf("%s\n\n", quiz->question);

    int correct = rand() % 4;  //randomize the correct answer's position
    int j = 0;

    //displaying the answer  choices
    for (int i = 0; i < 4; i++) {
        char index = 'a' + i;
        if (i == correct) {
            printf("[%c] %s\n", index, quiz->answer);
        } else {
            printf("[%c] %s\n", index, quiz->choices[j]);
            j++;
        }
    }

    printf("\n");

    char choice;
    do {
        if (interrupted) {  //checking if interrupted
            printf("Game interrupted. Exiting...\n");
            return 0;  //indicate interruption
        }

        printf("(%dpt) > ", quiz->max);
        scanf(" %c", &choice);

        if (choice == 'a' || choice == 'b' || choice == 'c' || choice == 'd') {
            if (choice == 'a' + correct) {
                printf("Congratulations! Answer [%c] is correct. Your current score is %d/%d points.\n",
                       choice, quiz->score + quiz->max, quiz->n * 8);
                quiz->score += quiz->max;
                return 1;  //indicate successful answer
            } else {
                printf("Answer [%c] is wrong, try again.\n", choice);
                quiz->max /= 2;  //half the score for each incorrect attempt
                attempts--;  //decrement attempts
            }
        } else {
            printf("Please enter a valid option!\n");
        }
    } while (attempts > 0);  //loop while there are attempts left

    //if all of the attempts have been used, no points are awarded
    printf("out of attempts! The correct answer was [%c]. Your current score is %d/%d points.\n", 
           'a' + correct, current_score, quiz->n * 8);
    return 1;  //indicate question completed, but no score awarded
}

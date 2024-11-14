#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "quiz.h"

volatile int interrupted = 0;  //blobal flag for interruption

//signal handler function to set interrupted flag
void handle_interrupt(int sig) {
    if (sig == SIGINT) {
        interrupted = 1;
    }
}

int main(void) {
    //register signal handler for SIGINT (Control-C)
    signal(SIGINT, handle_interrupt);

    int i = 1;
    quiz_t Quiz;
    Quiz.score = 0;

    while (!interrupted) {  //check for interrupted flag
        char *url = "https://opentdb.com/api.php?amount=1&category=18&type=multiple";
        char *response = fetch(url);

        if (response) {
            if (parse(&Quiz, response) != 0) {
                free(response);
                continue;  //skip to the next iteration if parsing fails
            } else {
                Quiz.n = i;
                if (play(&Quiz) == 1) {  //play a single question round
                    i++;  //only increment if question was answered
                }

                free(response);
            }
        } else {
            fprintf(stderr, "Failed to fetch data\n");
            break;
        }
    }

    //final message after exit
    printf("Thanks for playing today.\n");
    printf("Your final score is %d/%d\n", Quiz.score, (Quiz.n - 1) * 8);  //adjust the tot questions count

    return EXIT_SUCCESS;
}

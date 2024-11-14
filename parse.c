#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "quiz.h"

int parse(quiz_t *quiz, char *msg) {
    json_error_t error;
    json_t *root = json_loads(msg, 0, &error);
    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return -1;
    }

    json_t *results = json_object_get(root, "results");
    if (!json_is_array(results)) {
        fprintf(stderr, "Error: results is not an array\n");
        json_decref(root);
        return -1;
    }

    json_t *first_result = json_array_get(results, 0);
    if (!json_is_object(first_result)) {
        fprintf(stderr, "Error: result is not an object\n");
        json_decref(root);
        return -1;
    }

    quiz->question = strdup(json_string_value(json_object_get(first_result, "question")));
    quiz->answer = strdup(json_string_value(json_object_get(first_result, "correct_answer")));

    json_t *incorrect_answers = json_object_get(first_result, "incorrect_answers");
    for (size_t i = 0; i < 3; i++) {
        quiz->choices[i] = strdup(json_string_value(json_array_get(incorrect_answers, i)));
    }
    quiz->choices[3] = quiz->answer;  //the correct answer is one of the choices

    json_decref(root);
    return 0;
}

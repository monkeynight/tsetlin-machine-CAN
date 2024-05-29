#include "MultiClassTsetlinMachine.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUMBER_OF_EXAMPLES_train 8000
#define NUMBER_OF_EXAMPLES_test 2000
int X_train[NUMBER_OF_EXAMPLES_train][FEATURES];
int y_train[NUMBER_OF_EXAMPLES_train];

int X_test[NUMBER_OF_EXAMPLES_test][FEATURES];
int y_test[NUMBER_OF_EXAMPLES_test];

void read_file(void) {
   FILE * fp;
	char * line = NULL;
	size_t len = 0;

	const char *s = " ";
	char *token = NULL;

	fp = fopen("CAN_8000X.txt", "r");
	if (fp == NULL) {
		printf("Error opening\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < NUMBER_OF_EXAMPLES_train; i++) {
		getline(&line, &len, fp);

		token = strtok(line, s);
		for (int j = 0; j < FEATURES; j++) {
			X_train[i][j] = atoi(token);
			token=strtok(NULL,s);
		}
		y_train[i] = atoi(token);
	}

	fp = fopen("CAN_2000Y.txt", "r");
	if (fp == NULL) {
		printf("Error opening\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < NUMBER_OF_EXAMPLES_test; i++) {
		getline(&line, &len, fp);

		token = strtok(line, s);
		for (int j = 0; j < FEATURES; j++) {
			X_test[i][j] = atoi(token);
			token=strtok(NULL,s);
		}
		y_test[i] = atoi(token);
	}

}


int main(void)
{	
	srand(time(NULL));

	read_file();
        float current_average;
    float current_average1;
	struct MultiClassTsetlinMachine *mc_tsetlin_machine = CreateMultiClassTsetlinMachine();


    mc_tm_initialize(mc_tsetlin_machine);
   
	for (int i = 0; i < 25000; i++) {

		mc_tm_fit(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_train, 1,3.9);
         printf("epoch %d \n",i);

        current_average = mc_tm_evaluate(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_test);
            current_average1= mc_tm_evaluate(mc_tsetlin_machine, X_test, y_test, NUMBER_OF_EXAMPLES_test);
          printf("训练准确率%f\n",current_average);
         printf("测试准确率%f\n",current_average1);
        save_multi_class_tsetlin_machine_text(mc_tsetlin_machine, "fpga_10_500.txt");

	} 
        
	return 0;
}
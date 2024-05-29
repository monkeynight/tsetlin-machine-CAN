#include <stdio.h>
#include <stdlib.h>

#include "MultiClassTsetlinMachine.h"

struct MultiClassTsetlinMachine *CreateMultiClassTsetlinMachine()
{

	struct MultiClassTsetlinMachine *mc_tm;

	mc_tm = (void *)malloc(sizeof(struct MultiClassTsetlinMachine));

	for (int i = 0; i < CLASSES; i++) {
		mc_tm->tsetlin_machines[i] = CreateTsetlinMachine();
	}
	return mc_tm;
}
void mc_tm_initialize(struct MultiClassTsetlinMachine *mc_tm)
{
	for (int i = 0; i < CLASSES; i++) {
		tm_initialize(mc_tm->tsetlin_machines[i]);
	}
}

float mc_tm_evaluate(struct MultiClassTsetlinMachine *mc_tm, int X[][FEATURES], int y[], int number_of_examples)
{
	int errors;
	int max_class;
	int max_class_sum;
	errors = 0;
	for (int l = 0; l < number_of_examples; l++) {
		max_class_sum = tm_score(mc_tm->tsetlin_machines[0], X[l]);
		max_class = 0;
		for (int i = 1; i < CLASSES; i++) {	
			int class_sum = tm_score(mc_tm->tsetlin_machines[i], X[l]);
			if (max_class_sum < class_sum) {
				max_class_sum = class_sum;
				max_class = i;
			}
		}

		if (max_class != y[l]) {
			errors += 1;
		}
	}
	
	return 1.0 - 1.0 * errors / number_of_examples;
}


void mc_tm_update(struct MultiClassTsetlinMachine *mc_tm, int Xi[], int target_class, float s)
{
	tm_update(mc_tm->tsetlin_machines[target_class], Xi, 1, s);

	unsigned int negative_target_class = (unsigned int)CLASSES * 1.0*rand()/((unsigned int)RAND_MAX+1);
	while (negative_target_class == target_class) {
		negative_target_class = (unsigned int)CLASSES * 1.0*rand()/((unsigned int)RAND_MAX+1);
	}

	tm_update(mc_tm->tsetlin_machines[negative_target_class], Xi, 0, s);
}


void mc_tm_fit(struct MultiClassTsetlinMachine *mc_tm, int X[][FEATURES], int y[], int number_of_examples, int epochs, float s)
{
	for (int epoch = 0; epoch < epochs; epoch++) {		
		for (int i = 0; i < number_of_examples; i++) {
			mc_tm_update(mc_tm, X[i], y[i], s);
		}
	}
}
void load_multi_class_tsetlin_machine_text(struct MultiClassTsetlinMachine *mc_tm, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        for (int i = 0; i < CLASSES; ++i) {
            for (int j = 0; j < CLAUSES; ++j) {
                for (int k = 0; k < FEATURES; ++k) {
                    if (fscanf(file, "%d %d", &mc_tm->tsetlin_machines[i]->ta_state[j][k][0], &mc_tm->tsetlin_machines[i]->ta_state[j][k][1]) != 2) {
                        fclose(file);
                        return;
                    }
                }
            }
        }
        fclose(file);
    } else {
        printf("Error opening file for reading.\n");
    }
}

void save_multi_class_tsetlin_machine_text(struct MultiClassTsetlinMachine *mc_tm, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < CLASSES; ++i) {
            for (int j = 0; j < CLAUSES; ++j) {
                for (int k = 0; k < FEATURES; ++k) {
                    fprintf(file, "%d %d ", mc_tm->tsetlin_machines[i]->ta_state[j][k][0], mc_tm->tsetlin_machines[i]->ta_state[j][k][1]);
                }
                fprintf(file, "\n");
            }
        }
        fclose(file);
    } else {
        printf("Error opening file for writing.\n");
    }
}

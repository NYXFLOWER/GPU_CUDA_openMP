#include <stdio.h>
#include <stdlib.h>

#define NUM_STUDENTS 4
#define student struct student

student{
	char * forename;
	char * surname;
	float average_module_mark;

};

void print_student(const student * s);

int main(){
    student * students;
    FILE * file;


    // read from file
    file = fopen("/Users/nyxfer/Documents/GitHub/gpu/lab/lab01/exercise07/students2.bin", "rb");
    if (file == NULL) {
        fprintf(stderr, "No file here");
        exit(0);
    }

    // allocate memory to students
    students = (student *) malloc(sizeof(student) * NUM_STUDENTS);

    // read length of student's name and allocate memory to pointers
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        unsigned int str_len;
        // for forename
        fread(&str_len, sizeof(unsigned int), 1, file);
        students[i].forename = (char *) malloc(sizeof(char) * str_len);
        fread(students[i].forename, sizeof(char), str_len, file);
        // for surname
        fread(&str_len, sizeof(unsigned int), 1, file);
        students[i].surname = (char *) malloc(sizeof(char) * str_len);
        fread(students[i].surname, sizeof(char), str_len, file);
        // for average mark
        fread(&students[i].average_module_mark, sizeof(float), 1, file);
    }

    // print all students
    for (int j = 0; j < NUM_STUDENTS; ++j) {
        print_student(&students[j]);
    }

    // free memory
    for (int k = 0; k < NUM_STUDENTS; ++k) {
        free(students[k].forename);
        free(students[k].surname);
    }

    return 0;
}

void print_student(const student * s){
	printf("Student:\n");
	printf("\tForename: %s\n", s->forename);
	printf("\tSurname: %s\n", s->surname);
	printf("\tAverage Module Mark: %.2f\n", s->average_module_mark);
}


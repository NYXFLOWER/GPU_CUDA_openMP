#include "linked_list.h"

//int main(){
//    llitem * student_list = NULL;
//    llitem * list_end = NULL;
//    FILE * file;
//    student * stu;
//
////    int * p;
////    printf("%lu", sizeof(p));
//
//    // read from file
//    file = fopen("/Users/nyxfer/Documents/GitHub/gpu/lab/lab01/exercise07/students2.bin", "rb");
//    if (file == NULL) {
//        fprintf(stderr, "No file here");
//        exit(0);
//    }
//
//    // read length of student's name and allocate memory to pointers
//    for (int i = 0; i < NUM_STUDENTS; ++i) {
//        stu = (student *) malloc(sizeof(student));
//        unsigned int str_len;
//
//        // for forename
//        fread(&str_len, sizeof(unsigned int), 1, file);
//        stu->forename = (char *) malloc(sizeof(char) * str_len);
//        fread(stu->forename, sizeof(char), str_len, file);
//        // for surname
//        fread(&str_len, sizeof(unsigned int), 1, file);
//        stu->surname = (char *) malloc(sizeof(char) * str_len);
//        fread(stu->surname, sizeof(char), str_len, file);
//        // for average mark
//        fread(&stu->average_module_mark, sizeof(float), 1, file);
//
//        // add stu to linked list
//        if (i == 0) {
//            student_list = create_linked_list(stu);
//            list_end = student_list;
//        } else {
//            list_end = add_to_linked_list(list_end, stu);
//        }
//    }
//
//    // print all students
//    print_items(student_list, print_student);
//
//    // free memory
//    llitem * temp = student_list;
//    while (temp != NULL) {
//        free(temp->record);
//        temp = temp->next;
//    }
//    free_linked_list(student_list);
//
//    return 0;
//}
//
//void print_student(const student * s){
//    printf("Student:\n");
//    printf("\tForename: %s\n", s->forename);
//    printf("\tSurname: %s\n", s->surname);
//    printf("\tAverage Module Mark: %.2f\n", s->average_module_mark);
//}

char * kkkkk(char * hhh) {
    return hhh;
}

int main() {
    char * hhh = "abcde";
    printf("%s\n", hhh);

    char ppp[5] = "kkk";
    hhh = ppp;

    ppp[3] = 'k';

    printf("%s\n", kkkkk(hhh));
    printf("%s", hhh);
}


#include <stdio.h>

struct student
{
    int studentID;
    int g1, g2, g3;
    float average;
};

struct student getMaxAverageStudent(struct student *s, int n){
    float max = -1;
    int max_index = 0;
    for(int i = 0; i < n; i++)
    {
        //check if current student' {s[i]} average is max
        //if so, update current max
        if (s[i].average > max){
            max = s[i].average;
            max_index = i;
        }
    }

    
    return s[max_index];
}
int main()
{
    FILE *infile;
    FILE *outfile;
    int n;
    infile = fopen("students.txt", "r");
    outfile = fopen("out.txt", "w");
    
    fscanf(infile, "%d", &n);
    
    struct student students[n];
    
    for(int i = 0; i < n; i++)
    {
        fscanf(infile, "%d", &students[i].studentID);
        fscanf(infile, "%d %d %d", &students[i].g1, &students[i].g2, &students[i].g3);
        students[i].average = (students[i].g1 + students[i].g2 + students[i].g3) / 3.0;
        
        printf("%d %d %d %d %0.2f\n", students[i].studentID, students[i].g1, 
                                       students[i].g2, students[i].g3, students[i].average);
        fprintf(outfile, "%d %d %d %d %0.2f\n", students[i].studentID, students[i].g1, 
                                       students[i].g2, students[i].g3, students[i].average);
    }
    
    struct student maxStudent = getMaxAverageStudent(students, n);
    printf("\nMaximum Average is %0.2f and the student is %d\n", maxStudent.average, maxStudent.studentID);
    fprintf(outfile, "\nMaximum Average is %0.2f and the student is %d\n", maxStudent.average, maxStudent.studentID);

    fclose(infile);
    fclose(outfile);
    return 0;
}

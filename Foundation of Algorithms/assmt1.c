/* Skeleton code for COMP10002 Assignment 1, March 2017.
   Jianzhong Qi, jianzhong.qi@unimelb.edu.au
   Modified by Fei Bao, April 2017
   Fei Bao, feib@student.unimelb.edu.au
*/
/* This program is simple edition of the Recommender Engines and designed to  implements 
    the skyline operator.
*/
/* By the way, algorithms are fun!!!
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIV "==========" /* stage header */
/* stage numbers */
#define STAGE_NUM_ONE 1 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5

#define READ_OK 1 /* reading point succeeded */
#define READ_FAILED 0 /* reading point failed */

#define MAX_NUM_POINTS 99 /* maximum number of points */
#define MAX_NUM_DIMENSIONS 10 /* maximum number of dimensions */

typedef double point_t[MAX_NUM_DIMENSIONS];

/* function prototypes */
void stage_one(point_t one_point, int *num_points, int num_dimensions);
void stage_two(point_t points[], int *num_points, int num_dimensions, 
	double *coordinate_sums);
void stage_three(double *coordinate_sums, int num_points);
void stage_four(point_t points[], int num_points, int num_dimensions);
void stage_five(point_t points[], int num_points, int num_dimensions);

void print_stage_heading(int stage);
int read_one_point(point_t one_point, int num_dimensions);
/* add your own function prototypes here */
void visualisation(double num);
double sum_divided_by_100(point_t one_point, int num_dimensions);
void empty_dominates(int dominates[], int num_points);
void add_point_num(int dominates[], int num_points, int num);
int len_of_none_zero_dominates(int dominates[], int num_points);
/* main program binds it all together */
int
main(int argc, char *argv[]) {

	/* storage for the input data */
	int num_dimensions;
	point_t points[MAX_NUM_POINTS];
	
	/* storage for statistics */
	int num_points = 0; /* number of points read in */
	double coordinate_sums[MAX_NUM_POINTS]; /* coordinate sums of the points */
	
	/* read the first line - number of dimensions */
	scanf("%d", &num_dimensions);
	
	/* stage 1 */
	stage_one(points[0], &num_points, num_dimensions);

	/* stage 2 */
	stage_two(points, &num_points, num_dimensions, 
		coordinate_sums);
	
	/* stage 3 */
	stage_three(coordinate_sums, num_points);

	/* stage 4 */
	stage_four(points, num_points, num_dimensions);

	/* stage 5 */
	stage_five(points, num_points, num_dimensions);
	
	/* and done! */
	return 0;
}

/* print out a stage heading, with a prior blank line once the stage number
   goes plus one 
*/
void 
print_stage_heading(int stage) {
	/* main job is to print the stage number passed as an argument */
	if (stage > STAGE_NUM_ONE) {
		/* separate the stages */
		printf("\n");
	}
	printf("Stage %d\n%s\n", stage, DIV);
}

/* read the coordinates of one point, store them in one_point
*/
int 
read_one_point(point_t one_point, int num_dimensions) {
	int i = 0;
	
	/* keep reading until num_dimensions numbers have been obtained */
	while (i < num_dimensions) {
		if (scanf("%lf", one_point+i) != 1) {
			return READ_FAILED;
		}
		i++;
	}
	
	return READ_OK;
}

/* stage 1 - read in and print one point
*/
void 
stage_one(point_t one_point, int *num_points, int num_dimensions) {
	/* print stage heading for stage 1 */
	print_stage_heading(STAGE_NUM_ONE);
	/* add your code here for stage 1 */
	/* read and print the first point */
	read_one_point(one_point, num_dimensions);
	*num_points += 1;
	printf("Point %.2d: <", *num_points);
	int i;
	for(i=0;i<num_dimensions;i++){
		printf("%.2f",one_point[i]);
		if(i<num_dimensions-1){
			printf(", ");
		}
	}
	printf(">\n");	
}

/* stage 2 - read in the rest of the points and visualise them
*/
void 
stage_two(point_t points[], int *num_points, int num_dimensions, 
	double *coordinate_sums) {
	/* print stage heading for stage 2 */
	print_stage_heading(STAGE_NUM_TWO);
	/* add your code here for stage 2 */
	point_t one_point;
	int i, j;
	/* read each point into the array points */
	while(read_one_point(one_point, num_dimensions)){
		*num_points += 1;
		for(j=0;j<num_dimensions;j++){
			points[*num_points-1][j] = one_point[j];
		}
	}
	/* visualise each point of points */
	for(i=0;i<*num_points;i++){	
		printf("Point %.2d, sum of coordinates (/100): ", i+1);
		printf("%5.2f |", sum_divided_by_100(points[i], num_dimensions));
		visualisation(sum_divided_by_100(points[i], num_dimensions));
		printf("\n");
		coordinate_sums[i]=sum_divided_by_100(points[i], num_dimensions)*100;
	}	
}

/* stage 3 - print statistics
*/
void 
stage_three(double *coordinate_sums, int num_points) {
	/* print stage heading for stage 3 */
	print_stage_heading(STAGE_NUM_THREE);
	
	/* add your code here for stage 3 */
	printf("Total: %d points\n", num_points);
	/* compare sum of coordinates one by one and print the largest one and the point num */
	int i, lgst_point=0;
	double lgst_sum=0.0;
	for(i=0;i<num_points;i++){
		if(coordinate_sums[i]>lgst_sum){
			lgst_sum = coordinate_sums[i];
			lgst_point = i;
		}
	}
	printf("Point of largest sum of coordinates: %.2d\n", lgst_point+1);
	printf("Largest sum of coordinates: %.2lf\n", lgst_sum);
}

/* stage 4 - find and print the skyline points
*/
void 
stage_four(point_t points[], int num_points, int num_dimensions) {
	/* print stage heading for stage 4 */
	print_stage_heading(STAGE_NUM_FOUR);
	
	/* add your code here for stage 4 */
	printf("Skyline points:\n");
	int i, j, form, latt;
	int dominates[num_points-1];/*storage for dominated point numbers*/
	/* compare the value of point with other values of points in the same column(dimension) 
	and add dominated point numbers into an array(dominates) */
	for(j=0;j<num_points;j++){
		empty_dominates(dominates, num_points-1);
		for(i=0;i<num_dimensions;i++){
			/* compare the current value with former values */
			for(form=0;form<j;form++){
				if(points[j][i]>points[form][i]){
					add_point_num(dominates, num_points-1, form+1);
				}
			}
			/* compare the current value with latter values */ 
			for(latt=j+1;latt<num_points;latt++){
				if(points[j][i]>points[latt][i]){
					add_point_num(dominates, num_points-1, latt+1);				
				}
			}
		}
		/* if the point is not dominated with other points, this point is the skyline point and
		print it. */
		if(len_of_none_zero_dominates(dominates, num_points-1)==num_points-1){
			printf("Point %.2d: <", j+1);
			for(i=0;i<num_dimensions;i++){
				printf("%.2f",points[j][i]);
				if(i<num_dimensions-1){
					printf(", ");
				}else{
					printf(">\n");	
				}
			}		
		}
	}
}


/* stage 5 - sort and print the skyline points by the number of points 
   they dominate
*/
void 
stage_five(point_t points[], int num_points, int num_dimensions) {
	/* print stage heading for stage 5 */
	print_stage_heading(STAGE_NUM_FIVE);
	
	/* add your code here for stage 5 */

}
/* visualise the num in stage 2 */
void
visualisation(double num){
	int i;
	num = ceil(num);
	for(i=1;i<=num;i++){
		if(i>=10 && i%10==0){
			printf("+");
		}else{
			printf("-");
		}
	}
}

/* find the sum of all elements in the array and then divided by 100 */
double
sum_divided_by_100(point_t one_point, int num_dimensions){
	int i;
	double total=0.00;
	for(i=0;i<num_dimensions;i++){
		total += one_point[i];
	}
	return total/100;
}
/* reset the array so all elements are 0 */
void
empty_dominates(int dominates[], int num_points){
	int i;
	for(i=0;i<num_points;i++){
		dominates[i]=0;
	}
}
/* add the element into an array if it is not in this array */
void
add_point_num(int dominates[], int num_points, int num){
	int i=0;		
	for(i=0;i<num_points;i++){
		if(dominates[i]==num){
			break;
		}
		if(dominates[i]==0){
			dominates[i]=num;
			break;
		}
	}
}
/* count the numbers of non-zero element in this array */
int
len_of_none_zero_dominates(int dominates[], int num_points){
	int i, num=0;
	for(i=0;i<num_points;i++){
		if(dominates[i]!=0){
			num++;
		}
	}
	return num;
}
// C program for Merge Sort
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int array[], int l, int m, int r) {

	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	//privremni nizovi
	int L[n1], R[n2];

	// Copy data to temp arrays L[] and R[]
	for (i = 0; i < n1; i++) {
		L[i] = array[l + i];
	}
	for (j = 0; j < n2; j++) {
		R[j] = array[m + 1 + j];
	}
	
	// Merge the temp arrays back into arr[l..r
	i = 0;
	j = 0;
	k = l;
	
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			array[k] = L[i];
			i++;
		}
		else {
			array[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of L[],
	// if there are any
	while (i < n1) {
		array[k] = L[i];
		i++;
		k++;
	}

	// Copy the remaining elements of R[],
	// if there are any
	while (j < n2) {
		array[k] = R[j];
		j++;
		k++;
	}
}

// l is for left index and r is right index of the
// sub-array of arr to be sorted
void mergeSort(int array[], int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;

		// Sort first and second halves
		mergeSort(array, l, m);
		mergeSort(array, m + 1, r);
		
		merge(array, l, m, r);
	}
}

// Function to print an array
void printArray(int A[], int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", A[i]);
	}
	printf("\n");
}


int main(int argc, char *argv[]) {
	
	int n;

	printf("Unesite duzinu niza: ");
	scanf("%d", &n);
    
	//da li je dužina niza ok?
	if (n <= 0) {
    	printf("Neispravna duzina niza.\n");
    	return 1;
	}

	int *array = (int *)malloc(n * sizeof(int));
    	
	//Nasumicno generisanje niza
	printf("Nesortirani niz:\n");
	for (int i = 0; i < n; i++) {
    	array[i] = rand() % 100; //random od 0 do 99
    	printf("%d ", array[i]);
	}
	printf("\n");

	clock_t start_time = clock();

	mergeSort(array, 0, n - 1);
	
    clock_t end_time = clock();

	printf("\nSortirani niz:\n");
	printArray(array, n);
	
	printf("Vreme izvrsavanja: %f ms\n", ((double)(end_time - start_time) / CLOCKS_PER_SEC)*1000);

	
	free(array);
	
	return 0;
}


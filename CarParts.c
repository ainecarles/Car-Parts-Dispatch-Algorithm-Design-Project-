/* Program to:
Task 1: Sort 4 files of production data based on weight with merge sort
Task 2: Produce a single dispatch list from the 4 files using an array of structures
Task 3: Provide a user interface to search for the earliest occurrence of a product with a particular weight using binary search
Task 4: Provide a report which summarises the number of products included in the delivery for all vans.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 30
#define MAX_PRODUCTS 40
#define MAX_LINE 256

typedef struct Batch_Time 
{
    int day;
    int hour;
    int minute;
}Batch_Time;

typedef struct Product 
{
    long line_code;
    long batch_code;
    Batch_Time BT;
    long product_ID;
    char name[SIZE];
    char targ_eng[SIZE];
    long bin;
    double weight; 
}Product;

// Function signatures
int readP(const char *file, Product products[], int count);//Reads and stores data from files
void print(Product p);//Prints data from files
void merge(Product products[], int l, int m, int r);//Sorts data 
void merge_split(Product products[], int l, int r);//Recursive part of merge sort (dividing data into subarrays)
int binary_search_weight(Product products[], int size, double input_weight);//Finds weight searched
void summary_report(int count);//Summarises total amount of products in delivery.

int main() 
{
    Product products[MAX_PRODUCTS];
    int count = 0;
    int n;
    double input_weight;


    printf("Product1.txt:\n");
    if ((n = readP("Product1.txt", products, count)) == -1) 
    {
        return 1;
    }
    for (int i = count; i < count + n; i++)
    {
        print(products[i]);
    }
    count += n; // index 0-9

    printf("\nProduct2.txt:\n");
    if ((n = readP("Product2.txt", products, count)) == -1)
    {
        return 1;
    }
    for (int i = count; i < count + n; i++)
    {
        print(products[i]);
    }
    count += n; // index 10-19
    printf("%d", n);

    printf("\nProduct3.txt:\n");
    if ((n = readP("Product3.txt", products, count)) == -1)
    {
        return 1;
    }
    for (int i = count; i < count + n; i++)
    {
        print(products[i]);
    }
    count += n; // index 20-29

    printf("\nProduct4.txt:\n");
    if ((n = readP("Product4.txt", products, count)) == -1) 
    {
        return 1;
    }
    for (int i = count; i < count + n; i++)
    {
        print(products[i]);
    }
    count += n; // index 30-39
    

    merge_split(products, 0, count - 1);

    printf("\nFiles Sorted by Weight:\n");

    for (int i = 0; i < count; i++) 
    {

        print(products[i]);
    }

    printf("\nEnter the weight you want to search:");
    scanf("%lf", &input_weight);

    int index = binary_search_weight(products, count, input_weight);

    //If weight entered by user isn't in the list
    if(index == -1)
    {
        printf("Weight not found\n");
    }

    else
    {
        printf("The earliest occurence of weight %.2lf is at index %d", input_weight, index);
    }

    summary_report(count);

    return 0;
}

int readP(const char *file, struct Product products[], int count) 
{
        FILE *fp_in = fopen(file, "r");
        char store[MAX_LINE];
        int i = count;
    
    //Check file has opened correctly
    if (fp_in == NULL) 
    {
        printf("Error opening file\n");
        return -1;
    }

    //Parse strings in files and store them in array
    while ((fgets(store, sizeof(store), fp_in)!=NULL) && (i < MAX_PRODUCTS))
    {
        Product p;//temp variable
        
        sscanf(store, "%ld,%ld,%d,%d,%d,%ld,%[^,],%[^,],%ld,%lf",
               &p.line_code, &p.batch_code,
               &p.BT.day, &p.BT.hour, &p.BT.minute,
               &p.product_ID,
               p.name, p.targ_eng,
               &p.bin, &p.weight);

        products[i++] = p;
    }

    fclose(fp_in);
    return i - count;
}

void print(Product p) 
{
    printf("Line Code: %ld Batch Code: %ld Day: %d Hour: %d Minute: %d ", p.line_code, p.batch_code, p.BT.day, p.BT.hour, p.BT.minute); 
    printf("ID: %ld Product: %s Target Engine: %s Bin: %ld Weight: %.2lf\n", p.product_ID, p.name, p.targ_eng, p.bin, p.weight);
}

void merge(Product products[], int l, int m, int r) 
{
    int i, j;
    int k = l;//index for products[]
    int s1 = (m - l) + 1;//size of left array
    int s2 = r - m;//size of right array

    // Create temp arrays
    Product left[s1], right[s2];

    // Copy data from products into temp arrays
    for (i = 0; i < s1; i++)
    {
        left[i] = products[l + i];
    }

    for (j = 0; j < s2; j++)
    {
        right[j] = products[m + 1 + j];
    }

    i = 0;
    j = 0;

    //Compare and merge temp arrays 
    while (i < s1 && j < s2) 
    {
        if (left[i].weight <= right[j].weight) 
        {
            products[k] = left[i];
            i++;
        } 
        
        else 
        {
            products[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements
    while (i < s1) 
    {
        products[k] = left[i];
        i++;
        k++;
    }

    while (j < s2) 
    {
        products[k] = right[j];
        j++;
        k++;
    }
}

void merge_split(Product products[], int l, int r) 
{
    if (l < r) 
    {
        int m = l + (r - l) / 2;

        merge_split(products, l, m);//Split left side
        merge_split(products, m + 1, r);//Split right side

        merge(products, l, m, r);
    }
}

int binary_search_weight(Product products[], int size, double input_weight) 
{
    int left = 0;
    int right = size - 1;
    int result = -1;

    while (left <= right) 
    {
        int mid = left + (right - left) / 2;

        if (products[mid].weight == input_weight) //starts search at middle index, does it equal inputted weight?
        {
            result = mid;
            right = mid - 1; // keep searching left to find the first occurrence
        }

        else if (products[mid].weight < input_weight)
        {
            left = mid + 1;//keep searching right
        } 
        
        else 
        {
            right = mid - 1;//keep searching left
        }
    }

    return result;
}

void summary_report(int count)
{
    printf("\nThe total number of products included in the delivery = %d", count);
}
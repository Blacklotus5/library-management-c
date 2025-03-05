/********************************************
Project title : Library Management system   
Date 		: 	13-12-2024
*********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lib {    // Structure storing data for all the books in the library            
    int anum;   // Identification Number of the book
    char tt[50];    // Title of the book
    char auth[50];  // Name of the author of the book
    int price;  // Price of the book
    int isit;   // 0: Removed, 1: Available, 2: Issued
} b;
struct student {    //Structure storing data for all students 
    int rn; //Roll number of the student
    char name[50];  //Name of the student
    char bName[50]; //Stores book name
    int issued_book_id; // Stores the ID of the issued book
} s;

void addBook(); //Function declarations
void dispBook();
void listbyAuth();
void countBook();
void removeBook(); 
void listBook();
void issueList();
void issueBook(); 

FILE *fp;   //Initialising file pointer fp, It will be used to point to librarybooks.txt

int main() {
    while (1) { //An infinite while loop for the menu-driven program to work properly
        int ch;
        printf("\n<<<====== Library Management System (LMS) ======>>>\n\n");    //A menu driven program having many functionalities
        printf("1. Add a new book\n");
        printf("2. Remove an existing book\n");
        printf("3. Count the number of books\n");
        printf("4. List all books by a specific author\n");
        printf("5. Display book info\n");
        printf("6. List available books in library\n");
        printf("7. Issue a book\n");
        printf("8. Issued book list\n");
        printf("9. Exit LMS program \n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        getchar(); // Clear newline '\n'

        switch (ch) {   //Switch statement is used to call the appropriate function according to the choice made by the user.
            case 1:
                addBook();  //Adds a new book
                break;
            case 2:
                removeBook();   //Remove a existing book
                break;
            case 3:
                countBook();    //Count the number of books
                break;
            case 4:
                listbyAuth();   //List all books of specific author
                break;
            case 5:
                dispBook();     //Display book details of a desired book
                break;
            case 6:
                listBook();
                break;
            case 7:
                issueBook();    //Issue book to a student
                break;
            case 8:
                issueList();    //List all issued books
                break;
            case 9:
                exit(0);    //Exit out of the program
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    printf("\n\nPress Any Key To Continue...");
    getchar();
    }
    return 0;
}

void addBook() 
{
    fp = fopen("librarybooks.txt", "ab"); //Append in binary mode, new file will be created if it is not there.
    if (fp == NULL) {
    printf("Error: Cannot open file!\n");
    return;
    }
        printf("Enter the name of the book: ");
        fgets(b.tt, 50, stdin);
        b.tt[strcspn(b.tt, "\n")] = 0; // Remove trailing newline character

        printf("Enter the name of the author: ");
        fgets(b.auth, 50, stdin);
        b.auth[strcspn(b.auth, "\n")] = 0;  //remove trailing newline character

        printf("Enter the book id:");
        scanf("%d", &b.anum);

        printf("Enter the price: ");
        scanf("%d", &b.price);
        getchar(); // Clear newline

        b.isit = 1; // Mark as available

        fwrite(&b, sizeof(b), 1, fp);   //Write the data of the new book stored in structure b in file librarybooks.txt
        fclose(fp);     //Closing the file after use

        printf("Book added successfully!\n");
        return;
}
        

void dispBook() 
{
    int id, found = 0;

    printf("Enter the book ID: ");
    scanf("%d", &id);

    fp = fopen("librarybooks.txt", "rb");   //Opening only for reading in binary mode
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1)    //Infinite while loop for reading all of the data present in the file
    {
        if (id == b.anum)                                   //Checking if any data of book id matches with the entered book id
        {
            printf("Book ID: %d\n", b.anum);            //If the data matches print all the data of the book
            printf("Title: %s\n", b.tt);
            printf("Author: %s\n", b.auth);
            printf("Price: %d\n", b.price);
            printf("Status: %s\n", b.isit == 1 ? "Available" : "Issued");   //Ternary operator used to show to status of the book if b.isit = 1, then the book is available otherwise it is issued
            found = 1;
        }
    }
    fclose(fp);

    if (found == 0) {
        printf("Book not found.\n");
    }
}

void listbyAuth() {
    char author[50];
    int found = 0;

    printf("Enter the author's name: ");
    fgets(author, 50, stdin);
    author[strcspn(author, "\n")] = 0; // Remove newline character

    fp = fopen("librarybooks.txt", "rb");
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }

    printf("Books by %s:\n", author);
    printf("Book ID\t\t\tTitle\n");

    while (fread(&b, sizeof(b), 1, fp) == 1)
    {
        if (strcmp(b.auth, author) == 0 && b.isit != 0) {   //strcmp checks if the library has any book by the author
            printf("%d\t\t\t%s\n", b.anum, b.tt);           //If the string comparision is successfull, the book name of the suthor is printed
            found = 1;
        }
    }

    fclose(fp);

    if (found == 0) {
        printf("No books found by this author.\n");
    }
}

void countBook() {
    int count = 0;

    fp = fopen("librarybooks.txt", "rb");
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }
    while (fread(&b, sizeof(b), 1, fp) == 1)
    {
        if (b.isit != 0) {                                          //A simple program to count the number of books
            count++;
        }
    }

    fclose(fp);
    printf("Total number of books: %d\n", count);
}

void removeBook() {
    int id, found = 0;
    FILE *ft;

    printf("Enter the book ID to remove: ");
    scanf("%d", &id);

    fp = fopen("librarybooks.txt", "rb");   //Opening the source data file in read mode
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }
    ft = fopen("temp.txt", "wb");           //Opening the temporary file in write mode, if the file doesn't exit it will be created
    if (ft == NULL) {
    printf("Error: Cannot open file!\n");
    return;
    }
       while (fread(&b, sizeof(b), 1, fp) == 1)
    {
        if (id == b.anum)
        {
            found = 1;
        }
        else 
        {
            fwrite(&b, sizeof(b), 1, ft);       //This will print all the books except the book we want to delete in the temporary file
        }
    }
    if (found == 1)
    {
        printf("Deleted Successfully.\n");
    }
    else
    {
        printf("Record Not Found !\n");
    }
    fclose(fp);
    fclose(ft);

    remove("librarybooks.txt");     //The previous librarybooks.txt is removed
    rename("temp.txt", "librarybooks.txt");     //temp.txt is renamed to librarybooks.txt, Now the book is deleted

}
void listBook()
{
    printf("<=== Available Books ===>\n\n");
    printf("%10s %30s %20s\n\n", "Book ID", "Book Name", "Author");     //Add proper spacing

    fp = fopen("librarybooks.txt", "rb");
    while (fread(&b, sizeof(b), 1, fp) == 1)    //Infinite loop for reading file
    {
        printf("%10d %30s %20s\n", b.anum, b.tt, b.auth);   //Prints all the available books
    }

    fclose(fp);
}

void issueBook() {
    FILE *ft;
    int id, foundb = 0;

    printf("Enter the book ID to issue: ");
    scanf("%d", &id);

    fp = fopen("librarybooks.txt", "rb");
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }
        while (fread(&b, sizeof(b), 1, fp) == 1)    //Infinite loop to read the file content
        {
        if (b.anum == id && b.isit == 1) {
            strcpy(s.bName, b.tt);  //Book name is copy to the student structure
            foundb = 1;
            break;
        }
        }
        if (foundb == 0)
        {
        printf("No book found with this id\n");
        printf("Please try again...\n\n");
        return;
        }

        ft = fopen("issued_books.txt", "ab");   //Append mode in binary, will create a new file if the file is not there
        if (ft == NULL) {
        printf("Error: Cannot open file!\n");
        return;
        }
        printf("Enter the roll number of the student: ");   //Enter the data of student who want to get book issued
        scanf("%d", &s.rn);
        getchar();
        printf("Enter the name of the student: ");
        fgets(s.name, 50, stdin);
        s.name[strcspn(s.name, "\n")] = 0;

        s.issued_book_id = id;

        fwrite(&s, sizeof(s), 1, ft);   //Write the data of student book issue
        fclose(ft);

        printf("Book issued to student successfully.\n");
        return;
}

void issueList()
{
    printf("<== Book Issue List ==>\n\n");

    printf("%10s %10s %30s %30s\n\n", "Book Id", "Roll No.", "Name", "Book Name");     //Gap given of 10 and 30 columns respectively

    fp = fopen("issued_books.txt", "rb");   
    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        printf("%10d %10d %30s %30s\n", s.issued_book_id, s.rn, s.name, s.bName);       //printing the data giving the same gap of coulmns as above 
    }

    fclose(fp);
}
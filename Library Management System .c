#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAX_BOOKS 200
#define MAX_STUDENTS 200
#define MAX_ROOMS 50
#define MAX_TRANSACTIONS 1000
#define MAX_FINES 500
#define MAX_COMPLAINTS 500
#define MAX_ACTIVITIES 1000
#define MAX_REQUESTS 1000
#define NUM_SLOTS 8
#define SLOT_START 9
#define DATA_FILE "library_requests.dat"
#define BACKUP_FILE "library_requests_backup.dat"
struct Book { int id; char title[100]; char author[100]; char category[50];
              int totalCopies; int availableCopies; int active; };
struct Student { int id; char name[100]; char department[50]; int semester;
                 char contact[30]; char password[30]; int active; };
struct Room { int id; char name[50]; int capacity; int type; int slotStudent[NUM_SLOTS]; };
struct Transaction { int studentID; int bookID; int issueDate; int dueDate;
                     int returnDate; int status; };
struct Fine { int studentID; int amount; int paid; char reason[100]; };
struct Complaint { int id; int studentID; char message[200]; char status[30]; char reason[100]; };
struct Activity { char message[200]; };
struct Request { int id; int studentID; int type; int itemID; int slot; char message[200]; int status; };
struct Book books[MAX_BOOKS];
struct Student students[MAX_STUDENTS];
struct Room rooms[MAX_ROOMS];
struct Transaction transactions[MAX_TRANSACTIONS];
struct Fine fines[MAX_FINES];
struct Complaint complaints[MAX_COMPLAINTS];
struct Activity activities[MAX_ACTIVITIES];
struct Request requests[MAX_REQUESTS];
int bookCount = 0, studentCount = 0, roomCount = 0, transactionCount = 0;
int fineCount = 0, complaintCount = 0, activityCount = 0, requestCount = 0;
int readInt(char message[]) {
    char line[100], extra;
    int number;
    while (1) {
        printf("%s", message);
        if (fgets(line, sizeof(line), stdin) != NULL &&
            sscanf(line, "%d %c", &number, &extra) == 1) {
            return number;
        }
        printf("Invalid input! Enter a valid number.\n");
    }
}
void readLine(char message[], char text[], int size) {
    printf("%s", message);
    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    }
}
void removeSpaces(char in[], char out[]) {
    int i, j = 0;
    for (i = 0; in[i] != '\0'; i++) {
        if (in[i] != ' ' && in[i] != '\t' && in[i] != '\n' && in[i] != '\r') {
            out[j++] = in[i];
        }
    }
    out[j] = '\0';
}
void slotLabel(int slot, char out[]) {
    sprintf(out, "%02d:00 - %02d:00", SLOT_START + slot, SLOT_START + slot + 1);
}
int readSlot(char message[]) {
    char line[100], compact[100], label[30], expected[30];
    int attempts, s;
    for (attempts = 3; attempts > 0; attempts--) {
        printf("%s", message);
        if (fgets(line, sizeof(line), stdin) != NULL) {
            removeSpaces(line, compact);
            for (s = 0; s < NUM_SLOTS; s++) {
                slotLabel(s, label);
                removeSpaces(label, expected);
                if (strcmp(compact, expected) == 0) return s;
            }
        }
        printf("Invalid time slot! Enter exactly as shown, e.g. %02d:00 - %02d:00 "
               "(slots from %02d:00 to %02d:00).\n",
               SLOT_START, SLOT_START + 1, SLOT_START, SLOT_START + NUM_SLOTS);
        if (attempts > 1) printf("Attempts remaining: %d\n", attempts - 1);
    }
    printf("Too many invalid attempts!\n");
    return -1;
}
int isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}
int daysInMonth(int m, int y) {
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (m == 2 && isLeapYear(y)) ? 29 : days[m - 1];
}
int dateToDays(int date) {
    int y = date / 10000, m = (date / 100) % 100, total = date % 100, i;
    for (i = 2000; i < y; i++) total += isLeapYear(i) ? 366 : 365;
    for (i = 1; i < m; i++) total += daysInMonth(i, y);
    return total;
}
void formatDate(int date, char out[]) {
    sprintf(out, "%02d-%02d-%04d", date % 100, (date / 100) % 100, date / 10000);
}
int readDate(char message[]) {
    char line[100];
    int d = 0, m = 0, y = 0, i, ok, attempts;
    for (attempts = 3; attempts > 0; attempts--) {
        printf("%s", message);
        ok = 0;
        if (fgets(line, sizeof(line), stdin) != NULL) {
            line[strcspn(line, "\r\n")] = '\0';
            if (strlen(line) == 10 && line[2] == '-' && line[5] == '-') {
                ok = 1;
                for (i = 0; i < 10; i++) {
                    if (i != 2 && i != 5 && (line[i] < '0' || line[i] > '9')) ok = 0;
                }
                if (ok) {
                    d = (line[0] - '0') * 10 + (line[1] - '0');
                    m = (line[3] - '0') * 10 + (line[4] - '0');
                    y = (line[6] - '0') * 1000 + (line[7] - '0') * 100 +
                        (line[8] - '0') * 10 + (line[9] - '0');
                    if (y < 2000 || y > 2100 || m < 1 || m > 12 ||
                        d < 1 || d > daysInMonth(m, y)) ok = 0;
                }
            }
        }
        if (ok) return y * 10000 + m * 100 + d;
        printf("Invalid date! Use format DD-MM-YYYY (e.g. 07-11-2026).\n");
        if (attempts > 1) printf("Attempts remaining: %d\n", attempts - 1);
    }
    printf("Too many invalid attempts!\n");
    return 0;
}
int findBook(int id) {
    int i;
    for (i = 0; i < bookCount; i++)
        if (books[i].id == id && books[i].active) return i;
    return -1;
}
int findStudent(int id) {
    int i;
    for (i = 0; i < studentCount; i++)
        if (students[i].id == id && students[i].active) return i;
    return -1;
}
int findRoom(int id) {
    int i;
    for (i = 0; i < roomCount; i++)
        if (rooms[i].id == id) return i;
    return -1;
}
void logActivity(char message[]) {
    FILE *fp = fopen("activity.txt", "a");
    time_t now;
    char *date;
    if (fp != NULL) {
        now = time(NULL);
        date = ctime(&now);
        if (date != NULL) fprintf(fp, "%s - %s\n", date, message);
        fclose(fp);
    }
    if (activityCount < MAX_ACTIVITIES) {
        strcpy(activities[activityCount].message, message);
        activityCount++;
    }
}
void initializeDefaults(void) {
    const char *bk[10][3] = {
        {"C Programming", "Dennis Ritchie", "Computer"},
        {"Data Structures", "Mark Allen", "Computer"},
        {"Database Systems", "Elmasri", "Database"},
        {"Operating Systems", "Galvin", "Computer"},
        {"Computer Networks", "Tanenbaum", "Networking"},
        {"Artificial Intelligence", "Russell", "AI"},
        {"Digital Logic Design", "Morris Mano", "Engineering"},
        {"Calculus", "Thomas", "Mathematics"},
        {"Physics", "Halliday", "Science"},
        {"English Grammar", "Wren and Martin", "English"}
    };
    int copies[10] = {5, 4, 3, 5, 4, 3, 5, 4, 3, 5};
    const char *names[10] = {
        "Ali Khan", "Sara Ahmed", "Usman Tariq", "Ayesha Malik", "Hassan Raza",
        "Fatima Noor", "Bilal Hussain", "Zainab Shah", "Hamza Iqbal", "Maryam Siddiqui"
    };
    const char *depts[10] = {
        "Computer Science", "Software Engineering", "Electrical", "Computer Science",
        "Mechanical", "Software Engineering", "Computer Science", "Mathematics",
        "Electrical", "Physics"
    };
    int i;
    for (i = 0; i < 10; i++) {
        books[i].id = i + 1;
        strcpy(books[i].title, bk[i][0]);
        strcpy(books[i].author, bk[i][1]);
        strcpy(books[i].category, bk[i][2]);
        books[i].totalCopies = books[i].availableCopies = copies[i];
        books[i].active = 1;
        memset(&students[i], 0, sizeof(struct Student));
        students[i].id = 1001 + i;
        strcpy(students[i].name, names[i]);
        strcpy(students[i].department, depts[i]);
        students[i].semester = (i % 8) + 1;
        sprintf(students[i].contact, "0300-12345%02d", i);
        sprintf(students[i].password, "pass%d", 1001 + i);
        students[i].active = 1;
        memset(&rooms[i], 0, sizeof(struct Room));
        rooms[i].id = 101 + i;
        sprintf(rooms[i].name, "Computer Lab %d", i + 1);
        rooms[i].capacity = 30;
        rooms[i].type = 2;
        memset(&rooms[i + 10], 0, sizeof(struct Room));
        rooms[i + 10].id = 201 + i;
        sprintf(rooms[i + 10].name, "Discussion Room %d", i + 1);
        rooms[i + 10].capacity = 8;
        rooms[i + 10].type = 1;
    }
    bookCount = studentCount = 10;
    roomCount = 20;
}
void saveData(void) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Error saving data!\n");
        return;
    }
    fwrite(&bookCount, sizeof(int), 1, fp);
    fwrite(books, sizeof(struct Book), MAX_BOOKS, fp);
    fwrite(&studentCount, sizeof(int), 1, fp);
    fwrite(students, sizeof(struct Student), MAX_STUDENTS, fp);
    fwrite(&roomCount, sizeof(int), 1, fp);
    fwrite(rooms, sizeof(struct Room), MAX_ROOMS, fp);
    fwrite(&transactionCount, sizeof(int), 1, fp);
    fwrite(transactions, sizeof(struct Transaction), MAX_TRANSACTIONS, fp);
    fwrite(&fineCount, sizeof(int), 1, fp);
    fwrite(fines, sizeof(struct Fine), MAX_FINES, fp);
    fwrite(&complaintCount, sizeof(int), 1, fp);
    fwrite(complaints, sizeof(struct Complaint), MAX_COMPLAINTS, fp);
    fwrite(&activityCount, sizeof(int), 1, fp);
    fwrite(activities, sizeof(struct Activity), MAX_ACTIVITIES, fp);
    fwrite(&requestCount, sizeof(int), 1, fp);
    fwrite(requests, sizeof(struct Request), MAX_REQUESTS, fp);
    fclose(fp);
}
void loadData(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    int bad = 0;
    if (fp == NULL) {
        initializeDefaults();
        return;
    }
    if (fread(&bookCount, sizeof(int), 1, fp) != 1 ||
        fread(books, sizeof(struct Book), MAX_BOOKS, fp) != MAX_BOOKS ||
        fread(&studentCount, sizeof(int), 1, fp) != 1 ||
        fread(students, sizeof(struct Student), MAX_STUDENTS, fp) != MAX_STUDENTS ||
        fread(&roomCount, sizeof(int), 1, fp) != 1 ||
        fread(rooms, sizeof(struct Room), MAX_ROOMS, fp) != MAX_ROOMS ||
        fread(&transactionCount, sizeof(int), 1, fp) != 1 ||
        fread(transactions, sizeof(struct Transaction), MAX_TRANSACTIONS, fp) != MAX_TRANSACTIONS ||
        fread(&fineCount, sizeof(int), 1, fp) != 1 ||
        fread(fines, sizeof(struct Fine), MAX_FINES, fp) != MAX_FINES ||
        fread(&complaintCount, sizeof(int), 1, fp) != 1 ||
        fread(complaints, sizeof(struct Complaint), MAX_COMPLAINTS, fp) != MAX_COMPLAINTS ||
        fread(&activityCount, sizeof(int), 1, fp) != 1 ||
        fread(activities, sizeof(struct Activity), MAX_ACTIVITIES, fp) != MAX_ACTIVITIES ||
        fread(&requestCount, sizeof(int), 1, fp) != 1 ||
        fread(requests, sizeof(struct Request), MAX_REQUESTS, fp) != MAX_REQUESTS) {
        printf("Invalid data file! Loading default data.\n");
        bad = 1;
    }
    fclose(fp);
    if (!bad &&
        (bookCount < 0 || bookCount > MAX_BOOKS || studentCount < 0 || studentCount > MAX_STUDENTS ||
         roomCount < 0 || roomCount > MAX_ROOMS || transactionCount < 0 || transactionCount > MAX_TRANSACTIONS ||
         fineCount < 0 || fineCount > MAX_FINES || complaintCount < 0 || complaintCount > MAX_COMPLAINTS ||
         activityCount < 0 || activityCount > MAX_ACTIVITIES || requestCount < 0 || requestCount > MAX_REQUESTS)) {
        printf("Invalid record counts! Loading default data.\n");
        bad = 1;
    }
    if (bad) {
        bookCount = studentCount = roomCount = 0;
        transactionCount = fineCount = complaintCount = activityCount = requestCount = 0;
        initializeDefaults();
        saveData();
    }
}
void done(char logText[], char message[]) {
    logActivity(logText);
    saveData();
    printf("%s\n", message);
}
int copyFile(char from[], char to[]) {
    FILE *a = fopen(from, "rb"), *b;
    int ch;
    if (a == NULL) return -1;
    b = fopen(to, "wb");
    if (b == NULL) { fclose(a); return -2; }
    while ((ch = fgetc(a)) != EOF) fputc(ch, b);
    fclose(a);
    fclose(b);
    return 0;
}
void backupData(void) {
    int r;
    saveData();
    r = copyFile(DATA_FILE, BACKUP_FILE);
    if (r == -1) printf("Original data file not found!\n");
    else if (r == -2) printf("Cannot create backup file!\n");
    else done("Data backup created", "Backup created successfully!");
}
void restoreData(void) {
    int r = copyFile(BACKUP_FILE, DATA_FILE);
    if (r == -1) { printf("Backup file not found!\n"); return; }
    if (r == -2) { printf("Cannot restore data!\n"); return; }
    loadData();
    done("Backup data restored", "Data restored successfully!");
}
void addBook(void) {
    int id;
    struct Book *b;
    if (bookCount >= MAX_BOOKS) { printf("Book storage full!\n"); return; }
    id = readInt("Enter Book ID: ");
    if (findBook(id) != -1) { printf("Book already exists!\n"); return; }
    b = &books[bookCount];
    b->id = id;
    readLine("Enter Title: ", b->title, 100);
    readLine("Enter Author: ", b->author, 100);
    readLine("Enter Category: ", b->category, 50);
    b->totalCopies = readInt("Enter Copies: ");
    if (b->totalCopies <= 0) { printf("Invalid copies!\n"); return; }
    b->availableCopies = b->totalCopies;
    b->active = 1;
    bookCount++;
    done("New book added", "Book added successfully!");
}
void displayBooks(void) {
    int i;
    printf("\n========== ALL BOOKS ==========\n");
    for (i = 0; i < bookCount; i++) {
        if (!books[i].active) continue;
        printf("\nID: %d\nTitle: %s\nAuthor: %s\nCategory: %s\n",
               books[i].id, books[i].title, books[i].author, books[i].category);
        printf("Total Copies: %d\nAvailable: %d\n", books[i].totalCopies, books[i].availableCopies);
    }
}
void searchBook(void) {
    int i = findBook(readInt("Enter Book ID: "));
    if (i == -1) { printf("Book not found!\n"); return; }
    printf("\nTitle: %s\nAuthor: %s\nCategory: %s\nAvailable Copies: %d\n",
           books[i].title, books[i].author, books[i].category, books[i].availableCopies);
}

void updateBook(void) {
	int choice;
    int i = findBook(readInt("Enter Book ID: "));
    if (i == -1) { printf("Book not found!\n"); return; }
    do {
        printf("\nWhat do you want to change?\n");
        printf("1. Book Title\n 2. Book Author\n 3. Book Category\n 4. Exit\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1:
                readLine("New Title: ", books[i].title, 100);
                done("Book Title Updated", "Title updated!");
                break;
            case 2:
                readLine("New Author: ", books[i].author, 100);
                done("Book Author Updated", "Author updated!");
                break;
            case 3:
                readLine("New Category: ", books[i].category, 50);
                done("Book Category Updated", "Category updated!");
                break;
            case 4:
                printf("Returning to menu...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);
}
void removeBook(void) {
    int i = findBook(readInt("Enter Book ID: "));
    if (i == -1) { printf("Book not found!\n"); return; }
    if (books[i].availableCopies != books[i].totalCopies) {
        printf("Cannot remove issued books!\n");
        return;
    }
    books[i].active = 0;
    done("Book removed", "Book removed successfully!");
}
int readIssueDates(int *issue, int *due) {
    *issue = readDate("Enter Issue Date (DD-MM-YYYY): ");
    if (*issue == 0) return 0;
    *due = readDate("Enter Due Date (DD-MM-YYYY): ");
    if (*due == 0) return 0;
    if (*due < *issue) {
        printf("Due date cannot be before issue date!\n");
        return 0;
    }
    return 1;
}
void addIssueRecord(int sid, int bookIndex, int issue, int due) {
    struct Transaction *t = &transactions[transactionCount++];
    t->studentID = sid;
    t->bookID = books[bookIndex].id;
    t->issueDate = issue;
    t->dueDate = due;
    t->returnDate = 0;
    t->status = 1;
    books[bookIndex].availableCopies--;
}
void issueBook(void) {
    int sid, index, issue, due;
    if (transactionCount >= MAX_TRANSACTIONS) { printf("Transaction storage full!\n"); return; }
    sid = readInt("Enter Student ID: ");
    if (findStudent(sid) == -1) { printf("Student not found! Register student first.\n"); return; }
    index = findBook(readInt("Enter Book ID: "));
    if (index == -1 || books[index].availableCopies <= 0) { printf("Book not available!\n"); return; }
    if (!readIssueDates(&issue, &due)) return;
    addIssueRecord(sid, index, issue, due);
    done("Book issued to student", "Book issued successfully!");
}
void returnBook(void) {
    int sid, bid, i, index, ret, lateDays;
    sid = readInt("Enter Student ID: ");
    bid = readInt("Enter Book ID: ");
    for (i = transactionCount - 1; i >= 0; i--) {
        struct Transaction *t = &transactions[i];
        if (t->studentID != sid || t->bookID != bid || t->status != 1) continue;
        ret = readDate("Enter Return Date (DD-MM-YYYY): ");
        if (ret == 0) return;
        if (ret < t->issueDate) { printf("Return date cannot be before issue date!\n"); return; }
        t->returnDate = ret;
        t->status = 2;
        index = findBook(bid);
        if (index != -1) books[index].availableCopies++;
        lateDays = dateToDays(ret) - dateToDays(t->dueDate);
        if (lateDays > 0 && fineCount < MAX_FINES) {
            fines[fineCount].studentID = sid;
            fines[fineCount].amount = lateDays * 10;
            fines[fineCount].paid = 0;
            strcpy(fines[fineCount].reason, "Late book return");
            fineCount++;
            printf("Late days: %d\nFine added: Rs. %d\n", lateDays, lateDays * 10);
        }
        done("Book returned", "Book returned successfully!");
        return;
    }
    printf("Active issue record not found!\n");
}
void showTransactions(int mode, int today) {
    const char *title[4] = {"BOOK ISSUE HISTORY", "CURRENTLY ISSUED BOOKS",
                            "RETURNED BOOKS", "OVERDUE BOOKS"};
    char issue[12], due[12], ret[12];
    int i, found = 0;
    printf("\n========== %s ==========\n", title[mode]);
    for (i = 0; i < transactionCount; i++) {
        struct Transaction *t = &transactions[i];
        if (mode == 1 && t->status != 1) continue;
        if (mode == 2 && t->status != 2) continue;
        if (mode == 3 && (t->status != 1 || today <= t->dueDate)) continue;
        formatDate(t->issueDate, issue);
        formatDate(t->dueDate, due);
        formatDate(t->returnDate, ret);
        if (mode == 0) {
            printf("Student: %d | Book: %d | Issue Date: %s | Due Date: %s | ",
                   t->studentID, t->bookID, issue, due);
            if (t->status == 1) printf("Status: Issued\n");
            else printf("Status: Returned on %s\n", ret);
        } else if (mode == 2) {
            printf("Student ID: %d | Book ID: %d | Return Date: %s\n", t->studentID, t->bookID, ret);
        } else {
            printf("Student ID: %d | Book ID: %d | Due Date: %s\n", t->studentID, t->bookID, due);
        }
        found = 1;
    }
    if (!found) printf("No records found.\n");
}
void checkAvailableBooks(void) {
    int i, found = 0;
    printf("\n========== AVAILABLE BOOKS ==========\n");
    for (i = 0; i < bookCount; i++) {
        if (books[i].active && books[i].availableCopies > 0) {
            printf("ID: %d | %s | Available: %d\n", books[i].id, books[i].title, books[i].availableCopies);
            found = 1;
        }
    }
    if (!found) printf("No books are currently available.\n");
}
void damagedLostBooks(void) {
    int i = findBook(readInt("Enter Book ID: ")), quantity;
    if (i == -1) { printf("Book not found!\n"); return; }
    quantity = readInt("Enter damaged/lost quantity: ");
    if (quantity <= 0 || quantity > books[i].availableCopies) { printf("Invalid quantity!\n"); return; }
    books[i].totalCopies -= quantity;
    books[i].availableCopies -= quantity;
    done("Damaged or lost books recorded", "Book stock updated!");
}
void manageBookCategories(void) {
    int i, j, found, index;
    char newCategory[50];
    printf("\n========== BOOK CATEGORIES ==========\n");
    for (i = 0; i < bookCount; i++) {
        if (!books[i].active) continue;
        for (found = j = 0; j < i; j++)
            if (books[j].active && strcmp(books[i].category, books[j].category) == 0) found = 1;
        if (!found) printf("- %s\n", books[i].category);
    }
    printf("\n1. Change category of a book\n2. Back\n");
    if (readInt("Enter Choice: ") == 1) {
        index = findBook(readInt("Enter Book ID: "));
        if (index == -1) { printf("Book not found.\n"); return; }
        readLine("Enter new category: ", newCategory, 50);
        strcpy(books[index].category, newCategory);
        done("Book category updated", "Category updated successfully.");
    }
}
void viewBookRequests(void) {
    int i, found = 0;
    printf("\n========== BOOK ISSUE REQUESTS ==========\n");
    for (i = 0; i < requestCount; i++) {
        if (requests[i].type == 1 && requests[i].status == 0) {
            printf("Request ID: %d | Student ID: %d | Book ID: %d\nDetails: %s\n",
                   requests[i].id, requests[i].studentID, requests[i].itemID, requests[i].message);
            found = 1;
        }
    }
    if (!found) printf("No pending book issue requests.\n");
}
void systemMonitoring(void) {
    int i, s, activeBooks = 0, available = 0, activeStudents = 0, issued = 0;
    int occupied = 0, pendingFines = 0, pendingComplaints = 0;
    for (i = 0; i < bookCount; i++)
        if (books[i].active) { activeBooks++; available += books[i].availableCopies; }
    for (i = 0; i < studentCount; i++)
        if (students[i].active) activeStudents++;
    for (i = 0; i < transactionCount; i++)
        if (transactions[i].status == 1) issued++;
    for (i = 0; i < roomCount; i++)
        for (s = 0; s < NUM_SLOTS; s++)
            if (rooms[i].slotStudent[s] != 0) occupied++;
    for (i = 0; i < fineCount; i++)
        if (!fines[i].paid) pendingFines += fines[i].amount;
    for (i = 0; i < complaintCount; i++)
        if (strcmp(complaints[i].status, "Pending") == 0) pendingComplaints++;
    printf("\n========== SYSTEM MONITORING ==========\n");
    printf("Active Book Titles: %d\nAvailable Book Copies: %d\n", activeBooks, available);
    printf("Active Students: %d\nIssued Transactions: %d\n", activeStudents, issued);
    printf("Total Transactions: %d\nTotal Rooms/Labs: %d\n", transactionCount, roomCount);
    printf("Booked Room/Lab Slots: %d / %d\n", occupied, roomCount * NUM_SLOTS);
    printf("Pending Fine Amount: Rs. %d\nPending Complaints: %d\n", pendingFines, pendingComplaints);
}
void bookAdministration(void) {
    int choice, today;
    do {
        printf("\n========== BOOK ADMINISTRATION ==========\n"
               "1. ADD BOOK\n2. DISPLAY ALL BOOKS\n3. SEARCH BOOK\n4. UPDATE BOOK DETAIL\n"
               "5. REMOVE BOOK\n6. ISSUE BOOK (ADMIN DIRECT)\n7. RETURN BOOK\n8. SYSTEM MONITORING\n"
               "9. CHECK AVAILABLE BOOKS\n10. VIEW ISSUED BOOKS\n11. VIEW RETURNED BOOKS\n"
               "12. VIEW OVERDUE BOOKS\n13. MANAGE BOOK CATEGORIES\n14. DAMAGED / LOST BOOKS\n"
               "15. BOOK ISSUE REQUESTS\n16. BOOK ISSUE HISTORY\n17. BACK TO ADMIN PANEL\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: updateBook(); break;
            case 5: removeBook(); break;
            case 6: issueBook(); break;
            case 7: returnBook(); break;
            case 8: systemMonitoring(); break;
            case 9: checkAvailableBooks(); break;
            case 10: showTransactions(1, 0); break;
            case 11: showTransactions(2, 0); break;
            case 12:
                today = readDate("Enter Current Date (DD-MM-YYYY): ");
                if (today != 0) showTransactions(3, today);
                break;
            case 13: manageBookCategories(); break;
            case 14: damagedLostBooks(); break;
            case 15: viewBookRequests(); break;
            case 16: showTransactions(0, 0); break;
            case 17: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 17);
}
void printStudent(int i) {
    printf("\nID: %d\nName: %s\nDepartment: %s\nSemester: %d\nContact: %s\n",
           students[i].id, students[i].name, students[i].department,
           students[i].semester, students[i].contact);
}

void addStudent(void) {
    int id, semester;
    char name[100], department[50], contact[30], password[30];
    struct Student *s;
    if (studentCount >= MAX_STUDENTS) { printf("Student storage full!\n"); return; }
    id = readInt("Enter Student ID: ");
    if (id <= 0) { printf("Invalid student ID!\n"); return; }
    if (findStudent(id) != -1) { printf("Student already exists!\n"); return; }
    readLine("Enter Name: ", name, 100);
    readLine("Enter Department: ", department, 50);
    semester = readInt("Enter Semester: ");
    readLine("Enter Contact: ", contact, 30);
    if (semester < 1 || semester > 12) { printf("Invalid semester!\n"); return; }
    readLine("Enter Password (min 4 characters): ", password, 30);
    if (strlen(password) < 4) { printf("Password too short!\n"); return; }
    s = &students[studentCount++];
    memset(s, 0, sizeof(struct Student));
    s->id = id;
    strcpy(s->name, name);
    strcpy(s->department, department);
    s->semester = semester;
    strcpy(s->contact, contact);
    strcpy(s->password, password);
    s->active = 1;
    done("New student registered", "Student registered successfully!");
}
void displayStudents(void) {
    int i;
    printf("\n========== STUDENTS ==========\n");
    for (i = 0; i < studentCount; i++)
        if (students[i].active) printStudent(i);
}
void searchStudent(void) {
    int i = findStudent(readInt("Enter Student ID: "));
    if (i == -1) printf("Student not found!\n");
    else printStudent(i);
}
void updateStudent(void) {
    int i = findStudent(readInt("Enter Student ID: "));
    char password[30];
    if (i == -1) { printf("Student not found!\n"); return; }
    readLine("New Name: ", students[i].name, 100);
    readLine("New Department: ", students[i].department, 50);
    students[i].semester = readInt("New Semester: ");
    readLine("New Contact: ", students[i].contact, 30);
    readLine("New Password (leave empty to keep old): ", password, 30);
    if (strlen(password) >= 4) strcpy(students[i].password, password);
    else if (strlen(password) > 0) printf("Password too short! Old password kept.\n");
    done("Student information updated", "Student updated!");
}
void removeStudent(void) {
    int id = readInt("Enter Student ID: "), i, index = findStudent(id);
    if (index == -1) { printf("Student not found!\n"); return; }
    for (i = 0; i < transactionCount; i++) {
        if (transactions[i].studentID == id && transactions[i].status == 1) {
            printf("Student has an issued book. Return it first!\n");
            return;
        }
    }
    students[index].active = 0;
    done("Student removed", "Student removed!");
}
void studentHistory(void) {
    int id = readInt("Enter Student ID: "), i, found = 0;
    char issue[12], due[12], ret[12];
    if (findStudent(id) == -1) { printf("Student not found!\n"); return; }
    for (i = 0; i < transactionCount; i++) {
        struct Transaction *t = &transactions[i];
        if (t->studentID != id) continue;
        formatDate(t->issueDate, issue);
        formatDate(t->dueDate, due);
        formatDate(t->returnDate, ret);
        printf("Book ID: %d | Issue: %s | Due: %s | ", t->bookID, issue, due);
        if (t->status == 1) printf("Issued\n");
        else printf("Returned on %s\n", ret);
        found = 1;
    }
    if (!found) printf("No borrowing history!\n");
}
void studentAdministration(void) {
    int choice;
    do {
        printf("\n========== STUDENT ADMINISTRATION ==========\n"
               "1. Add Student\n2. Display Students\n3. Search Student\n4. Update Student\n"
               "5. Remove Student\n6. Student Borrowing History\n7. Back\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: removeStudent(); break;
            case 6: studentHistory(); break;
            case 7: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 7);
}
void addRoom(void) {
    int id, type, capacity;
    struct Room *r;
    if (roomCount >= MAX_ROOMS) { printf("Room storage full!\n"); return; }
    id = readInt("Enter Room/Lab ID: ");
    if (findRoom(id) != -1) { printf("Room ID already exists!\n"); return; }
    r = &rooms[roomCount];
    memset(r, 0, sizeof(struct Room));
    r->id = id;
    readLine("Enter Room/Lab Name: ", r->name, 50);
    capacity = readInt("Enter Capacity: ");
    if (capacity <= 0) { printf("Invalid capacity!\n"); return; }
    printf("1. Discussion Room\n2. Computer Lab\n");
    type = readInt("Enter Type: ");
    if (type != 1 && type != 2) { printf("Invalid type!\n"); return; }
    r->capacity = capacity;
    r->type = type;
    roomCount++;
    done("New room or lab added", "Room added successfully!");
}
void printRoomSlots(int r, int showWho) {
    int s;
    char label[30];
    for (s = 0; s < NUM_SLOTS; s++) {
        slotLabel(s, label);
        if (rooms[r].slotStudent[s] == 0) printf("   %s  [ ] FREE\n", label);
        else if (showWho) printf("   %s  [X] BOOKED (Student ID: %d)\n", label, rooms[r].slotStudent[s]);
        else printf("   %s  [X] BOOKED\n", label);
    }
}
void viewRooms(int showWho) {
    int i, filter;
    printf("\n1. Computer Labs\n2. Discussion Rooms\n3. All\n");
    filter = readInt("Enter Choice: ");
    if (filter < 1 || filter > 3) { printf("Invalid choice!\n"); return; }
    printf("\n========== ROOMS & LABS ==========\n");
    for (i = 0; i < roomCount; i++) {
        if (filter == 1 && rooms[i].type != 2) continue;
        if (filter == 2 && rooms[i].type != 1) continue;
        printf("\nID: %d | %s | %s | Capacity: %d\n", rooms[i].id, rooms[i].name,
               rooms[i].type == 1 ? "Discussion Room" : "Computer Lab", rooms[i].capacity);
        printRoomSlots(i, showWho);
    }
}
void changeSlot(int book) {
    int r, sid = 0, slot;
    r = findRoom(readInt("Enter Room ID: "));
    if (r == -1) { printf("Room not found!\n"); return; }
    if (book) {
        sid = readInt("Enter Student ID: ");
        if (findStudent(sid) == -1) { printf("Student not found!\n"); return; }
    }
    printRoomSlots(r, 1);
    slot = readSlot(book ? "Enter Time Slot (e.g. 09:00 - 10:00): "
                         : "Enter Time Slot to release (e.g. 09:00 - 10:00): ");
    if (slot == -1) return;
    if (book && rooms[r].slotStudent[slot] != 0) { printf("This slot is already booked!\n"); return; }
    if (!book && rooms[r].slotStudent[slot] == 0) { printf("Slot is already free!\n"); return; }
    rooms[r].slotStudent[slot] = sid;
    if (book) done("Room booked by admin", "Room booked successfully!");
    else done("Room slot released", "Slot released successfully!");
}
void roomMonitoring(void) {
    int choice;
    do {
        printf("\n========== ROOM & LAB MONITORING ==========\n"
               "1. Add Room / Lab\n2. View Rooms / Labs\n3. Book Room / Lab\n"
               "4. Release Room / Lab\n5. Back\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: addRoom(); break;
            case 2: viewRooms(1); break;
            case 3: changeSlot(1); break;
            case 4: changeSlot(0); break;
            case 5: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 5);
}
void addFine(void) {
    int sid, amount;
    if (fineCount >= MAX_FINES) { printf("Fine storage full!\n"); return; }
    sid = readInt("Enter Student ID: ");
    if (findStudent(sid) == -1) { printf("Student not found!\n"); return; }
    amount = readInt("Enter Fine Amount: ");
    if (amount <= 0) { printf("Invalid amount!\n"); return; }
    fines[fineCount].studentID = sid;
    fines[fineCount].amount = amount;
    fines[fineCount].paid = 0;
    readLine("Enter Fine Reason: ", fines[fineCount].reason, 100);
    fineCount++;
    done("New fine added", "Fine added successfully!");
}
void viewFines(void) {
    int i;
    printf("\n========== FINE RECORDS ==========\n");
    for (i = 0; i < fineCount; i++) {
        printf("\nStudent ID: %d\nAmount: Rs. %d\nReason: %s\nStatus: %s\n",
               fines[i].studentID, fines[i].amount, fines[i].reason,
               fines[i].paid ? "Paid" : "Pending");
    }
}
void payFine(void) {
    int sid = readInt("Enter Student ID: "), i, found = 0;
    for (i = 0; i < fineCount; i++) {
        if (fines[i].studentID == sid && fines[i].paid == 0) {
            printf("Pending Fine: Rs. %d\n", fines[i].amount);
            fines[i].paid = 1;
            found = 1;
        }
    }
    if (found) done("Student fine payment recorded", "Fine payment completed!");
    else printf("No pending fines found!\n");
}
void fineManagement(void) {
    int choice;
    do {
        printf("\n========== PENALTY & FINE MANAGEMENT ==========\n"
               "1. Add Fine\n2. View Fines\n3. Pay Fine\n4. Back\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: addFine(); break;
            case 2: viewFines(); break;
            case 3: payFine(); break;
            case 4: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);
}
void submitComplaint(int sid, char message[]) {
    struct Complaint *c;
    if (complaintCount >= MAX_COMPLAINTS) { printf("Complaint storage full!\n"); return; }
    if (strlen(message) == 0) { printf("Complaint cannot be empty!\n"); return; }
    c = &complaints[complaintCount++];
    c->id = complaintCount;
    c->studentID = sid;
    strcpy(c->message, message);
    strcpy(c->status, "Pending");
    strcpy(c->reason, "");
    logActivity("New complaint or suggestion received");
    saveData();
    printf("Complaint submitted! Your Complaint ID is %d\n", c->id);
}
void addComplaint(void) {
    int sid = readInt("Enter Student ID: ");
    char message[200];
    if (findStudent(sid) == -1) { printf("Student not found!\n"); return; }
    readLine("Enter Complaint / Suggestion: ", message, 200);
    submitComplaint(sid, message);
}
void viewComplaints(void) {
    int i;
    printf("\n========== COMPLAINTS ==========\n");
    if (complaintCount == 0) { printf("No complaints found.\n"); return; }
    for (i = 0; i < complaintCount; i++) {
        printf("\nComplaint ID: %d\nStudent ID: %d\nMessage: %s\nStatus: %s\n",
               complaints[i].id, complaints[i].studentID, complaints[i].message, complaints[i].status);
        if (strcmp(complaints[i].status, "Rejected") == 0)
            printf("Rejection Reason: %s\n", complaints[i].reason);
    }
}
void updateComplaint(void) {
    int id = readInt("Enter Complaint ID: "), i, choice;
    struct Complaint *c;
    for (i = 0; i < complaintCount; i++) {
        if (complaints[i].id != id) continue;
        c = &complaints[i];
        if (strcmp(c->status, "Pending") != 0) { printf("Complaint already %s.\n", c->status); return; }
        printf("\nStudent ID: %d\nMessage: %s\n\n1. Accept\n2. Reject\n", c->studentID, c->message);
        choice = readInt("Enter Choice: ");
        if (choice == 1) {
            strcpy(c->status, "Accepted");
            strcpy(c->reason, "");
            done("Complaint accepted", "Complaint accepted!");
        } else if (choice == 2) {
            do {
                readLine("Enter rejection reason: ", c->reason, 100);
                if (strlen(c->reason) == 0) printf("Reason cannot be empty!\n");
            } while (strlen(c->reason) == 0);
            strcpy(c->status, "Rejected");
            done("Complaint rejected", "Complaint rejected!");
        } else {
            printf("Invalid choice!\n");
        }
        return;
    }
    printf("Complaint not found!\n");
}
void complaintManagement(void) {
    int choice;
    do {
        printf("\n========== COMPLAINT MANAGEMENT ==========\n"
               "1. Add Complaint / Suggestion\n2. View Complaints\n"
               "3. Accept / Reject Complaint\n4. Back\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: addComplaint(); break;
            case 2: viewComplaints(); break;
            case 3: updateComplaint(); break;
            case 4: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);
}
void reportsAnalytics(void) {
    int choice, i, j, issued, total, paid, best, bestCount, count;
    do {
        printf("\n========== REPORT & ANALYTICS ==========\n"
               "1. Book Issue Report\n2. Student Report\n3. Fine Collection Report\n"
               "4. Most Borrowed Book\n5. Back\n");
        choice = readInt("Enter Choice: ");
        if (choice == 1) {
            for (issued = i = 0; i < transactionCount; i++)
                if (transactions[i].status == 1) issued++;
            printf("Currently Issued: %d\nReturned Transactions: %d\nTotal Transactions: %d\n",
                   issued, transactionCount - issued, transactionCount);
        } else if (choice == 2) {
            printf("Total Student Records: %d\n", studentCount);
            for (i = 0; i < studentCount; i++)
                if (students[i].active)
                    printf("%d - %s - %s\n", students[i].id, students[i].name, students[i].department);
        } else if (choice == 3) {
            for (total = paid = i = 0; i < fineCount; i++) {
                total += fines[i].amount;
                if (fines[i].paid) paid += fines[i].amount;
            }
            printf("Total Fine: Rs. %d\nCollected Fine: Rs. %d\nPending Fine: Rs. %d\n",
                   total, paid, total - paid);
        } else if (choice == 4) {
            best = -1;
            bestCount = 0;
            for (i = 0; i < bookCount; i++) {
                if (!books[i].active) continue;
                for (count = j = 0; j < transactionCount; j++)
                    if (transactions[j].bookID == books[i].id) count++;
                if (count > bestCount) { bestCount = count; best = i; }
            }
            if (best == -1) printf("No borrowing records available!\n");
            else printf("Most Borrowed Book: %s\nTimes Borrowed: %d\n", books[best].title, bestCount);
        } else if (choice != 5) {
            printf("Invalid choice!\n");
        }
    } while (choice != 5);
}
void activityLog(void) {
    FILE *fp = fopen("activity.txt", "r");
    char line[300];
    if (fp == NULL) { printf("No activity log found!\n"); return; }
    printf("\n========== ACTIVITY LOG ==========\n");
    while (fgets(line, sizeof(line), fp) != NULL) printf("%s", line);
    fclose(fp);
}
void backupRestore(void) {
    int choice;
    do {
        printf("\n========== BACKUP & RESTORE ==========\n"
               "1. Create Backup\n2. Restore Backup\n3. Back\n");
        choice = readInt("Enter Choice: ");
        if (choice == 1) {
            backupData();
        } else if (choice == 2) {
            printf("WARNING: Current data will be replaced!\n");
            if (readInt("Enter 1 to confirm: ") == 1) restoreData();
            else printf("Restore cancelled.\n");
        } else if (choice != 3) {
            printf("Invalid choice!\n");
        }
    } while (choice != 3);
}
void submitRequest(int sid, int type, int itemID, int slot, char message[]) {
    struct Request *q;
    if (requestCount >= MAX_REQUESTS) { printf("Request storage full!\n"); return; }
    q = &requests[requestCount++];
    q->id = requestCount;
    q->studentID = sid;
    q->type = type;
    q->itemID = itemID;
    q->slot = slot;
    strcpy(q->message, message);
    q->status = 0;
    logActivity("New student request received");
    saveData();
    printf("Request submitted! Your Request ID is %d\n", q->id);
}
const char *requestKind(int type) {
    return type == 1 ? "Book Issue" : "Room/Lab Booking";
}
const char *requestStatus(int status) {
    return status == 0 ? "Pending" : status == 1 ? "Approved" : "Rejected";
}
void viewMyRequests(int sid) {
    int i, found = 0;
    printf("\n========== MY REQUESTS ==========\n");
    for (i = 0; i < requestCount; i++) {
        if (requests[i].studentID != sid) continue;
        printf("\nRequest ID: %d\nType: %s\nDetails: %s\nStatus: %s\n", requests[i].id,
               requestKind(requests[i].type), requests[i].message, requestStatus(requests[i].status));
        found = 1;
    }
    if (!found) printf("No book or room requests found.\n");
    printf("\n========== MY COMPLAINTS ==========\n");
    for (found = i = 0; i < complaintCount; i++) {
        if (complaints[i].studentID != sid) continue;
        printf("\nComplaint ID: %d\nMessage: %s\nStatus: %s\n",
               complaints[i].id, complaints[i].message, complaints[i].status);
        if (strcmp(complaints[i].status, "Rejected") == 0)
            printf("Rejection Reason: %s\n", complaints[i].reason);
        found = 1;
    }
    if (!found) printf("No complaints found.\n");
}
int registerStudent(void) {
    int i, newID = 1001, semester;
    char name[100], department[50], contact[30], password[30], confirm[30];
    struct Student *s;
    if (studentCount >= MAX_STUDENTS) { printf("Student storage full! Contact admin.\n"); return -1; }
    printf("\n========== NEW STUDENT REGISTRATION ==========\n");
    for (i = 0; i < studentCount; i++)
        if (students[i].id >= newID) newID = students[i].id + 1;
    readLine("Enter Name: ", name, 100);
    if (strlen(name) == 0) { printf("Name cannot be empty!\n"); return -1; }
    readLine("Enter Department: ", department, 50);
    if (strlen(department) == 0) { printf("Department cannot be empty!\n"); return -1; }
    semester = readInt("Enter Semester (1-12): ");
    if (semester < 1 || semester > 12) { printf("Invalid semester!\n"); return -1; }
    readLine("Enter Contact: ", contact, 30);
    readLine("Create Password (min 4 characters): ", password, 30);
    if (strlen(password) < 4) { printf("Password too short!\n"); return -1; }
    readLine("Confirm Password: ", confirm, 30);
    if (strcmp(password, confirm) != 0) { printf("Passwords do not match!\n"); return -1; }
    s = &students[studentCount];
    memset(s, 0, sizeof(struct Student));
    s->id = newID;
    strcpy(s->name, name);
    strcpy(s->department, department);
    s->semester = semester;
    strcpy(s->contact, contact);
    strcpy(s->password, password);
    s->active = 1;
    studentCount++;
    logActivity("New student registered from student portal");
    saveData();
    printf("\nRegistration successful!\nYour Student ID is: %d\n", newID);
    printf("Please remember your ID and password for next login.\n");
    return studentCount - 1;
}
int studentLogin(void) {
    int sid, index, attempts;
    char password[30];
    for (attempts = 3; attempts > 0; attempts--) {
        printf("\n========== STUDENT LOGIN ==========\n");
        sid = readInt("Enter Student ID: ");
        readLine("Enter Password: ", password, 30);
        index = findStudent(sid);
        if (index != -1 && strcmp(students[index].password, password) == 0) {
            printf("\nLogin Successful!\n");
            logActivity("Student logged in");
            return index;
        }
        printf("Incorrect Student ID or password!\nAttempts remaining: %d\n", attempts - 1);
    }
    printf("Login failed!\n");
    return -1;
}
void studentDashboard(int si) {
    int sid = students[si].id, i, j, issuedCount = 0, pendingFine = 0;
    int pendingRequests = 0, roomBookings = 0, bookIndex;
    char due[12];
    printf("\n==================================================\n");
    printf("                 STUDENT DASHBOARD\n");
    printf("==================================================\n");
    printf("Welcome, %s!\nStudent ID : %d\nDepartment : %s\nSemester   : %d\nContact    : %s\n",
           students[si].name, sid, students[si].department, students[si].semester, students[si].contact);
    for (i = 0; i < transactionCount; i++)
        if (transactions[i].studentID == sid && transactions[i].status == 1) issuedCount++;
    for (i = 0; i < fineCount; i++)
        if (fines[i].studentID == sid && fines[i].paid == 0) pendingFine += fines[i].amount;
    for (i = 0; i < requestCount; i++)
        if (requests[i].studentID == sid && requests[i].status == 0) pendingRequests++;
    for (i = 0; i < roomCount; i++)
        for (j = 0; j < NUM_SLOTS; j++)
            if (rooms[i].slotStudent[j] == sid) roomBookings++;
    printf("\n---------------- LIBRARY SUMMARY ----------------\n");
    printf("Currently Issued Books : %d\nPending Fine           : Rs. %d\n", issuedCount, pendingFine);
    printf("Pending Requests       : %d\nRoom/Lab Bookings      : %d\n", pendingRequests, roomBookings);
    printf("\n---------------- ISSUED BOOKS --------------------\n");
    if (issuedCount == 0) printf("You have no currently issued books.\n");
    for (i = 0; i < transactionCount; i++) {
        if (transactions[i].studentID != sid || transactions[i].status != 1) continue;
        bookIndex = findBook(transactions[i].bookID);
        formatDate(transactions[i].dueDate, due);
        printf("Book ID: %d | ", transactions[i].bookID);
        if (bookIndex != -1) printf("Title: %s | ", books[bookIndex].title);
        printf("Due Date: %s\n", due);
    }
    printf("\n---------------- PENDING FINES -------------------\n");
    if (pendingFine == 0) printf("You have no pending fines.\n");
    for (i = 0; i < fineCount; i++)
        if (fines[i].studentID == sid && fines[i].paid == 0)
            printf("Rs. %d | Reason: %s\n", fines[i].amount, fines[i].reason);
    printf("\n---------------- RECENT REQUESTS -----------------\n");
    if (pendingRequests == 0) printf("No pending requests.\n");
    for (i = 0; i < requestCount; i++)
        if (requests[i].studentID == sid && requests[i].status == 0)
            printf("Request ID: %d | %s | Pending\n", requests[i].id, requests[i].message);
    printf("==================================================\n");
}
void requestRoom(int sid) {
    int id, r, slot, k, groupSize;
    char label[30], message[200];
    viewRooms(0);
    id = readInt("Enter Room/Lab ID: ");
    r = findRoom(id);
    if (r == -1) { printf("Room/Lab not found.\n"); return; }
    groupSize = readInt("How many students will use the room? (Minimum 3): ");
    if (groupSize < 3) {
        printf("Room booking requires at least 3 students. Request cancelled.\n");
        return;
    }
    printRoomSlots(r, 0);
    slot = readSlot("Enter Time Slot (e.g. 09:00 - 10:00): ");
    if (slot == -1) return;
    if (rooms[r].slotStudent[slot] != 0) { printf("This slot is already booked.\n"); return; }
    for (k = 0; k < requestCount; k++) {
        if (requests[k].type == 2 && requests[k].status == 0 && requests[k].studentID == sid &&
            requests[k].itemID == id && requests[k].slot == slot) {
            printf("You already requested this slot.\n");
            return;
        }
    }
    slotLabel(slot, label);
    sprintf(message, "Room/Lab request: %s (%s) | Students: %d", rooms[r].name, label, groupSize);
    submitRequest(sid, 2, id, slot, message);
}
void studentMenu(int si) {
    int sid = students[si].id, choice, id, index;
    char message[200];
    studentDashboard(si);
    do {
        printf("\n========== STUDENT PORTAL ==========\n");
        printf("Welcome, %s (ID: %d)\n", students[si].name, sid);
        printf("1. View Available Books\n2. Request a Book\n3. Request Room/Lab Booking\n"
               "4. Submit Complaint/Suggestion\n5. View My Request Status\n6. Logout\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: displayBooks(); break;
            case 2:
                id = readInt("Enter Book ID: ");
                index = findBook(id);
                if (index == -1 || books[index].availableCopies <= 0) {
                    printf("Book is not available.\n");
                    break;
                }
                sprintf(message, "Book request: %s", books[index].title);
                submitRequest(sid, 1, id, 0, message);
                break;
            case 3: requestRoom(sid); break;
            case 4:
                readLine("Enter complaint/suggestion: ", message, 200);
                submitComplaint(sid, message);
                break;
            case 5: viewMyRequests(sid); break;
            case 6: printf("Student logged out.\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
}
void studentPortal(void) {
    int choice, si;
    do {
        printf("\n========== STUDENT PORTAL ==========\n"
               "1. New Student Registration\n2. Student Login\n3. Back\n");
        choice = readInt("Enter Choice: ");
        if (choice == 1 || choice == 2) {
            si = (choice == 1) ? registerStudent() : studentLogin();
            if (si != -1) studentMenu(si);
        } else if (choice != 3) {
            printf("Invalid choice!\n");
        }
    } while (choice != 3);
}
void showRequests(int pendingOnly) {
    int i, found = 0;
    char label[30];
    printf("\n========== STUDENT REQUESTS ==========\n");
    for (i = 0; i < requestCount; i++) {
        if (pendingOnly && requests[i].status != 0) continue;
        printf("\nRequest ID: %d | Student ID: %d\nType: %s\nRelated ID: %d\n", requests[i].id,
               requests[i].studentID, requestKind(requests[i].type), requests[i].itemID);
        if (requests[i].type == 2) {
            slotLabel(requests[i].slot, label);
            printf("Time Slot: %s\n", label);
        }
        printf("Details: %s\nStatus: %s\n", requests[i].message, requestStatus(requests[i].status));
        found = 1;
    }
    if (!found) printf("No matching requests.\n");
}
void processRequest(void) {
    int id = readInt("Enter Request ID: "), i, decision, index, issue, due;
    struct Request *q;
    for (i = 0; i < requestCount && requests[i].id != id; i++);
    if (i == requestCount) { printf("Request not found.\n"); return; }
    q = &requests[i];
    if (q->status != 0) { printf("Request already processed.\n"); return; }
    printf("1. Approve\n2. Reject\n");
    decision = readInt("Enter choice: ");
    if (decision == 2) {
        q->status = 2;
        done("Student request rejected", "Request rejected.");
        return;
    }
    if (decision != 1) { printf("Invalid choice.\n"); return; }
    if (q->type == 1) {
        index = findBook(q->itemID);
        if (index == -1 || books[index].availableCopies <= 0) {
            printf("Book is no longer available. Request remains pending.\n");
            return;
        }
        if (transactionCount >= MAX_TRANSACTIONS) { printf("Transaction storage full.\n"); return; }
        if (!readIssueDates(&issue, &due)) { printf("Request remains pending.\n"); return; }
        addIssueRecord(q->studentID, index, issue, due);
    } else {
        index = findRoom(q->itemID);
        if (index == -1) { printf("Room/Lab not found. You can reject this request.\n"); return; }
        if (rooms[index].slotStudent[q->slot] != 0) {
            printf("This slot is already booked! Request remains pending, you can reject it.\n");
            return;
        }
        rooms[index].slotStudent[q->slot] = q->studentID;
    }
    q->status = 1;
    done("Student request approved", "Request approved successfully.");
}
void requestManagement(void) {
    int choice;
    do {
        printf("\n========== STUDENT REQUEST MANAGEMENT ==========\n"
               "1. View All Requests\n2. View Pending Requests\n"
               "3. Approve / Reject Request\n4. Back\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: showRequests(0); break;
            case 2: showRequests(1); break;
            case 3: processRequest(); break;
            case 4: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);
}
const char *studentName(int id) {
    int i = findStudent(id);
    return i == -1 ? "(removed student)" : students[i].name;
}
const char *bookTitle(int id) {
    int i = findBook(id);
    return i == -1 ? "(removed book)" : books[i].title;
}
void showOverallSummary(int today) {
    int i, s, onTime = 0, late = 0, paidN = 0, paidAmt = 0, accepted = 0, rejected = 0;
    int approved = 0, declined = 0, issued = 0, overdue = 0, booked = 0;
    int unpaidN = 0, unpaidAmt = 0, pendComplaints = 0, pendBook = 0, pendRoom = 0;
    int finished, pending, total;
    char dateText[12];
    for (i = 0; i < transactionCount; i++) {
        struct Transaction *t = &transactions[i];
        if (t->status == 2) {
            if (t->returnDate > t->dueDate) late++;
            else onTime++;
        } else if (t->status == 1) {
            issued++;
            if (today != 0 && today > t->dueDate) overdue++;
        }
    }
    for (i = 0; i < fineCount; i++) {
        if (fines[i].paid) { paidN++; paidAmt += fines[i].amount; }
        else { unpaidN++; unpaidAmt += fines[i].amount; }
    }
    for (i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].status, "Accepted") == 0) accepted++;
        else if (strcmp(complaints[i].status, "Rejected") == 0) rejected++;
        else pendComplaints++;
    }
    for (i = 0; i < requestCount; i++) {
        if (requests[i].status == 1) approved++;
        else if (requests[i].status == 2) declined++;
        else if (requests[i].type == 1) pendBook++;
        else pendRoom++;
    }
    for (i = 0; i < roomCount; i++)
        for (s = 0; s < NUM_SLOTS; s++)
            if (rooms[i].slotStudent[s] != 0) booked++;
    finished = onTime + late + paidN + accepted + rejected + approved + declined;
    pending = unpaidN + pendComplaints + pendBook + pendRoom;
    total = finished + issued + pending;
    printf("\n==================================================\n");
    printf("              LMS WORKFLOW SUMMARY\n");
    printf("==================================================\n");
    if (today != 0) {
        formatDate(today, dateText);
        printf("Report Date : %s\n", dateText);
    }
    printf("\n[A] COMPLETED (Past)\n");
    printf("  Books returned        : %d (on time: %d, late: %d)\n", onTime + late, onTime, late);
    printf("  Fines paid            : %d record(s), Rs. %d collected\n", paidN, paidAmt);
    printf("  Complaints resolved   : %d (accepted: %d, rejected: %d)\n", accepted + rejected, accepted, rejected);
    printf("  Requests processed    : %d (approved: %d, rejected: %d)\n", approved + declined, approved, declined);
    printf("\n[B] IN PROGRESS (Current)\n");
    printf("  Books currently issued: %d\n", issued);
    if (today != 0) printf("  Of which overdue      : %d\n", overdue);
    else printf("  Of which overdue      : (date not entered)\n");
    printf("  Room/Lab slots booked : %d of %d\n", booked, roomCount * NUM_SLOTS);
    printf("\n[C] PENDING (Needs admin action)\n");
    printf("  Unpaid fines          : %d record(s), Rs. %d\n", unpaidN, unpaidAmt);
    printf("  Pending complaints    : %d\n", pendComplaints);
    printf("  Pending book requests : %d\n", pendBook);
    printf("  Pending room requests : %d\n", pendRoom);
    printf("\n[D] OVERALL PROGRESS\n");
    printf("  Completed             : %d\n  In progress           : %d\n  Pending               : %d\n",
           finished, issued, pending);
    printf("  Work finished         : ");
    if (total == 0) printf("nothing to track yet\n");
    else printf("%d of %d (%d%%)\n", finished, total, (finished * 100) / total);
    printf("==================================================\n");
}
void showCompletedWork(void) {
    int i, found, lateDays;
    char due[12], ret[12];
    printf("\n========== COMPLETED WORK (PAST) ==========\n\n--- Returned Books ---\n");
    for (found = i = 0; i < transactionCount; i++) {
        struct Transaction *t = &transactions[i];
        if (t->status != 2) continue;
        formatDate(t->dueDate, due);
        formatDate(t->returnDate, ret);
        lateDays = dateToDays(t->returnDate) - dateToDays(t->dueDate);
        printf("Student %d (%s) returned Book %d (%s)\n   Due: %s | Returned: %s | ",
               t->studentID, studentName(t->studentID), t->bookID, bookTitle(t->bookID), due, ret);
        if (lateDays > 0) printf("LATE by %d day(s)\n", lateDays);
        else printf("On time\n");
        found = 1;
    }
    if (!found) printf("No books returned yet.\n");
    printf("\n--- Paid Fines ---\n");
    for (found = i = 0; i < fineCount; i++) {
        if (!fines[i].paid) continue;
        printf("Student %d (%s) | Rs. %d | Reason: %s\n", fines[i].studentID,
               studentName(fines[i].studentID), fines[i].amount, fines[i].reason);
        found = 1;
    }
    if (!found) printf("No fines paid yet.\n");
    printf("\n--- Resolved Complaints ---\n");
    for (found = i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].status, "Pending") == 0) continue;
        printf("Complaint %d | Student %d | %s\n   Message: %s\n", complaints[i].id,
               complaints[i].studentID, complaints[i].status, complaints[i].message);
        if (strcmp(complaints[i].status, "Rejected") == 0)
            printf("   Rejection Reason: %s\n", complaints[i].reason);
        found = 1;
    }
    if (!found) printf("No complaints resolved yet.\n");
    printf("\n--- Processed Student Requests ---\n");
    for (found = i = 0; i < requestCount; i++) {
        if (requests[i].status == 0) continue;
        printf("Request %d | Student %d | %s | %s\n   Details: %s\n", requests[i].id,
               requests[i].studentID, requestKind(requests[i].type),
               requestStatus(requests[i].status), requests[i].message);
        found = 1;
    }
    if (!found) printf("No requests processed yet.\n");
}
void showInProgressWork(int today) {
    int i, s, found, lateDays;
    char due[12], label[30];
    printf("\n========== IN PROGRESS (CURRENT) ==========\n\n--- Books Currently Issued ---\n");
    for (found = i = 0; i < transactionCount; i++) {
        struct Transaction *t = &transactions[i];
        if (t->status != 1) continue;
        formatDate(t->dueDate, due);
        printf("Student %d (%s) has Book %d (%s)\n   Due: %s", t->studentID,
               studentName(t->studentID), t->bookID, bookTitle(t->bookID), due);
        if (today != 0) {
            lateDays = dateToDays(today) - dateToDays(t->dueDate);
            if (lateDays > 0) printf(" | OVERDUE by %d day(s)", lateDays);
            else printf(" | Not overdue");
        }
        printf("\n");
        found = 1;
    }
    if (!found) printf("No books are currently issued.\n");
    printf("\n--- Room / Lab Slots Booked ---\n");
    for (found = i = 0; i < roomCount; i++) {
        for (s = 0; s < NUM_SLOTS; s++) {
            if (rooms[i].slotStudent[s] == 0) continue;
            slotLabel(s, label);
            printf("%s (ID %d) | %s | Student %d (%s)\n", rooms[i].name, rooms[i].id, label,
                   rooms[i].slotStudent[s], studentName(rooms[i].slotStudent[s]));
            found = 1;
        }
    }
    if (!found) printf("No room or lab slots are booked.\n");
}
void showPendingWork(int today) {
    int i, found;
    char due[12], label[30];
    printf("\n========== PENDING WORK (TO-DO) ==========\n\n--- Overdue Books (return needed) ---\n");
    if (today == 0) {
        printf("Date not entered, overdue books cannot be checked.\n");
    } else {
        for (found = i = 0; i < transactionCount; i++) {
            struct Transaction *t = &transactions[i];
            if (t->status != 1 || today <= t->dueDate) continue;
            formatDate(t->dueDate, due);
            printf("Student %d (%s) | Book %d (%s) | Due: %s | %d day(s) late\n", t->studentID,
                   studentName(t->studentID), t->bookID, bookTitle(t->bookID), due,
                   dateToDays(today) - dateToDays(t->dueDate));
            found = 1;
        }
        if (!found) printf("No overdue books.\n");
    }
    printf("\n--- Unpaid Fines ---\n");
    for (found = i = 0; i < fineCount; i++) {
        if (fines[i].paid) continue;
        printf("Student %d (%s) | Rs. %d | Reason: %s\n", fines[i].studentID,
               studentName(fines[i].studentID), fines[i].amount, fines[i].reason);
        found = 1;
    }
    if (!found) printf("No unpaid fines.\n");
    printf("\n--- Complaints Waiting for Decision ---\n");
    for (found = i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].status, "Pending") != 0) continue;
        printf("Complaint %d | Student %d | %s\n", complaints[i].id,
               complaints[i].studentID, complaints[i].message);
        found = 1;
    }
    if (!found) printf("No pending complaints.\n");
    printf("\n--- Requests Waiting for Approval ---\n");
    for (found = i = 0; i < requestCount; i++) {
        if (requests[i].status != 0) continue;
        printf("Request %d | Student %d | %s\n   Details: %s\n", requests[i].id,
               requests[i].studentID, requestKind(requests[i].type), requests[i].message);
        if (requests[i].type == 2) {
            slotLabel(requests[i].slot, label);
            printf("   Time Slot: %s\n", label);
        }
        found = 1;
    }
    if (!found) printf("No pending requests.\n");
}
void showRecentActivity(void) {
    int i, start = activityCount > 10 ? activityCount - 10 : 0;
    printf("\n========== RECENT ACTIVITY (LAST 10 EVENTS) ==========\n");
    if (activityCount == 0) { printf("No activity recorded yet.\n"); return; }
    for (i = activityCount - 1; i >= start; i--)
        printf("%d. %s\n", i + 1, activities[i].message);
    printf("\nTotal events recorded so far: %d\n", activityCount);
}
void workflowSummary(void) {
    int choice, today;
    printf("\n========== WORKFLOW SUMMARY ==========\n");
    today = readDate("Enter Today's Date (DD-MM-YYYY): ");
    if (today == 0) printf("Date not entered. Overdue details will be skipped.\n");
    do {
        printf("\n========== WORKFLOW SUMMARY MENU ==========\n"
               "1. Overall Summary (Completed / In Progress / Pending)\n"
               "2. Completed Work (Past details)\n3. In Progress (Current details)\n"
               "4. Pending Work (To-do list)\n5. Recent Activity\n6. Back\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: showOverallSummary(today); break;
            case 2: showCompletedWork(); break;
            case 3: showInProgressWork(today); break;
            case 4: showPendingWork(today); break;
            case 5: showRecentActivity(); break;
            case 6: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
}
int adminLogin(void) {
    char username[50], password[50];
    int attempts;
    for (attempts = 3; attempts > 0; attempts--) {
        printf("\n========== ADMIN LOGIN ==========\n");
        readLine("Username: ", username, 50);
        readLine("Password: ", password, 50);
        if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
            printf("\nLogin Successful!\n");
            logActivity("Admin logged in");
            saveData();
            return 1;
        }
        printf("Incorrect username or password!\nAttempts remaining: %d\n", attempts - 1);
    }
    printf("Login failed!\n");
    return 0;
}
void adminPanel(void) {
    int choice;
    do {
        printf("\n============================================\n"
               "       LIBRARY MANAGEMENT SYSTEM - ADMIN\n"
               "============================================\n"
               "1. BOOK ADMINISTRATION\n2. STUDENT ADMINISTRATION\n3. ROOM & LAB MONITORING\n"
               "4. PENALTY & FINE MANAGEMENT\n5. COMPLAINT & SUGGESTION MANAGEMENT\n"
               "6. SYSTEM MONITORING\n7. REPORT & ANALYTICS\n8. ACTIVITY LOG\n"
               "9. BACKUP & RESTORE DATA\n10. STUDENT REQUEST MANAGEMENT\n"
               "11. WORKFLOW SUMMARY\n12. LOGOUT\n");
        choice = readInt("Enter Choice: ");
        switch (choice) {
            case 1: bookAdministration(); break;
            case 2: studentAdministration(); break;
            case 3: roomMonitoring(); break;
            case 4: fineManagement(); break;
            case 5: complaintManagement(); break;
            case 6: systemMonitoring(); break;
            case 7: reportsAnalytics(); break;
            case 8: activityLog(); break;
            case 9: backupRestore(); break;
            case 10: requestManagement(); break;
            case 11: workflowSummary(); break;
            case 12: done("Admin logged out", "Admin logged out successfully!"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 12);
}
int main(void) {
    int role;
    loadData();
    saveData();
    do {
        printf("\n========== LIBRARY MANAGEMENT SYSTEM ==========\n"
               "1. Admin Login\n2. Student Portal\n3. Exit\n");
        role = readInt("Select Portal: ");
        if (role == 1) {
            if (adminLogin()) adminPanel();
        } else if (role == 2) {
            studentPortal();
        } else if (role == 3) {
            printf("Thank you for using Library Management System.\n");
        } else {
            printf("Invalid choice!\n");
        }
    } while (role != 3);
    return 0;
}

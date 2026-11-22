# Library Management System

## A. Project Introduction

The **Library Management System** is a console-based application developed in the C programming language for managing library operations in an educational institution. It provides separate Admin and Student portals and supports book and student records, book issuing and returning, room and computer-lab slot booking, fines, complaints and suggestions, student requests, reports, workflow summaries, and activity tracking. The project demonstrates the use of structures, arrays, functions, pointers, conditional statements, loops, string handling, and file handling in C. Records are saved in a binary data file so that information can be loaded again when the program is restarted.


## B. Features & Test Cases (Execution Scenarios)

### Case 1: User Authentication, Student Registration and Book Management

- **Description:** This test case verifies administrator authentication, student registration and login, book administration, book issuing, book returning, book availability, and transaction history.

- **Inputs:**
  - Admin username: `admin`
  - Admin password: `1234`
  - Student name: `Bilal Ahmed`
  - Department: `CS`
  - Semester: `2`
  - Contact: `03001234567`
  - Student password: `bilal`
  - Book ID: `1`
  - Student ID: `1013`
  - Issue date: `09-12-2026`
  - Due date: `31-12-2026`
  - Return date: `01-01-2027`

- **Expected Output:**
  - Administrator successfully logs in and accesses the Admin Panel.
  - Student registration generates a unique Student ID.
  - The registered student successfully logs in to the Student Portal.
  - The administrator can access book administration options.
  - Books can be issued and returned using valid student and book IDs.
  - Available book records and transaction history are displayed.
  - Late book returns are identified and fines are calculated according to the system rules.

- **Screenshots:**

  **1. Admin Login**

  ![Admin Login](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_admin_login.png)

  **2. Student Registration**

  ![Student Registration](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_student_registration.png)

  **3. Student Login**

  ![Student Login](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_student_login.png)

  **4. Book Administration**

  ![Book Administration](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_book_administration_menu.png)

  **5. Book Issuing**

  ![Book Issue](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_book_issue.png)

  **6. Book Returning and Fine**

  ![Book Return](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_book_return_and_fine.png)

  **7. Book Transaction History**

  ![Transaction History](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_book_transaction_history.png)

  **8. Available Books**

  ![Available Books](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case1_available_books.png)


### Case 2: Student Requests, Room and Lab Booking, Request Approval and Fine Management

- **Description:** This test case verifies student access to available rooms and labs, room booking requests, administrator room and lab management, request approval, student request records, and fine management.

- **Inputs:**
  - Student ID: `1013`
  - Student Portal option: `3`
  - Room/Lab ID: `110`
  - Room category: `Computer Labs`
  - Selected time slot: `10:00 - 11:00`
  - Generated Request ID: `5`
  - Request management option: `3`
  - Approval choice: `1`

- **Expected Output:**
  - The Student Portal displays available options.
  - Students can view available rooms and computer labs.
  - Available room and lab time slots are displayed.
  - Students can submit room and lab booking requests.
  - The system generates a request ID.
  - The administrator can access room and lab management.
  - The administrator can view and process student requests.
  - Request records can be viewed from the Student Portal.
  - The Fine Management module provides options for managing student fines and payments.

- **Screenshots:**

  **1. Student Portal**

  ![Student Portal](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case2_student_portal_menu.png)

  **2. Student Room and Lab Availability**

  ![Room Availability](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case2_student_room_lab_availability.png)

  **3. Room Booking Request**

  ![Room Booking Request](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case2_room_booking_request_submitted.png)

  **4. Admin Room and Lab Management**

  ![Room and Lab Management](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case2_admin_room_lab_management_menu.png)

  **5. Admin Request Approval**

  ![Request Approval](screenshots/case2_admin_request_approval.png)

  **6. Student Request Records**

  ![Student Request Records](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case2_admin_request_approval.png)

  **7. Admin Fine Management Menu**

  ![Fine Management](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/5f2d356f0669db2849d1cb9a827bfd22b0cd6a7c/screenshots/admin_dashboard_and_fine_management_menu.png)


### Case 3: Complaint Management, Reports, Workflow Summary, Input Validation and File Handling

- **Description:** This test case verifies student complaint submission, reports and analytics, workflow monitoring, invalid input handling, and application data storage through file handling.

- **Inputs:**
  - Student ID: `1013`
  - Student Portal option: `4`
  - Complaint: `Please add more programming books.`
  - Generated Complaint ID: `2`
  - Report option: `2`
  - Invalid menu choice: `11`
  - Invalid date: `11-20-2026`
  - Valid date: `11-11-2026`
  - Data file: `library_requests.dat`
  - Activity file: `activity.txt`

- **Expected Output:**
  - Students can submit complaints and suggestions.
  - The system generates a complaint ID.
  - Reports and Analytics displays available book, student, and fine information.
  - Workflow Summary displays workflow information and recent activities.
  - Invalid menu choices and dates are handled with appropriate error messages.
  - Previously stored application data is maintained through file handling.
  - Activity records are stored in `activity.txt`.
  - Application data is stored in `library_requests.dat`.

- **Screenshots:**

  **1. Student Complaint Submission**

  ![Student Complaint](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case3_student_complaint_submission.png)

  **2. Reports and Analytics**

  ![Reports and Analytics](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case3_reports_and_analytics.png)

  **3. Workflow Summary and Date Validation**

  ![Workflow Summary](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case3_workflow_and_invalid_date.png)

  **4. Student Portal Input Validation**

  ![Input Validation](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case3_student_portal_invalid_choice.png)

  **5. Project Data Files**

  ![Project Data Files](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case3_project_data_files.png)

  **6. Activity Log**

  ![Activity Log](https://github.com/bilalahmed123987-dev/Library-Management-System/blob/774ee0cce5d5378c69bf5d6d060121c09fa9b4af/screenshots/case3_activity_log.png)

## C. Instructions to Clone and Execute

### Step 1: Clone the Repository

Open a terminal and run the following command. Replace `YOUR_GITHUB_USERNAME` with your GitHub username if you use a different repository owner.

```bash
git clone https://github.com/YOUR_GITHUB_USERNAME/library_management_system.git
cd library_management_system
```

### Step 2: Compile the Code

Make sure GCC is installed.

**Windows (MinGW / GCC):**

```bash
gcc main.c -o library_management.exe
```

**Linux:**

```bash
gcc main.c -o library_management
```

### Step 3: Run the Program

**Windows (Command Prompt):**

```cmd
library_management.exe
```

**Windows (PowerShell):**

```powershell
.\library_management.exe
```

**Linux:**

```bash
./library_management
```

The program creates or updates its data files in the working directory. Keep `library_requests.dat` and `activity.txt` in the project folder when you want to retain the saved records and activity log.

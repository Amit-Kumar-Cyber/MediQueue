
// 1. Vivek Rawat


#include "mediqueue.h"

// Global patient list head pointer and ID counter
struct Patient *patientHead = NULL;
int nextPatientID = 1001;


//  INPUT HELPER FUNCTIONS
    // ReadLine                            
void readLine(char *buffer, int size) {
    if (buffer == NULL || size <= 0) {
        return;
    }

    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[len - 1] = '\0';
            len--;
        }

        /* Sanitize: replace any accidental pipe '|' delimiters */
        for (size_t i = 0; i < len; i++) {
            if (buffer[i] == '|') {
                buffer[i] = ' ';
            }
        }
    } else {
        buffer[0] = '\0';
    }
}

    //ReadInteger
int readInteger(const char *prompt, int min, int max) {
    char line[64];
    int value = 0;
    int valid = 0;

    while (!valid) {
        if (prompt != NULL && prompt[0] != '\0') {
            printf("%s", prompt);
        }

        readLine(line, sizeof(line));

        if (strlen(line) == 0) {
            printf("Error: Input cannot be empty. Please enter a number between %d and %d.\n", min, max);
            continue;
        }

        char extra = '\0';
        if (sscanf(line, "%d %c", &value, &extra) == 1) {
            if (value >= min && value <= max) {
                valid = 1;
            } else {
                printf("Error: Number must be between %d and %d. Please try again.\n", min, max);
            }
        } else {
            printf("Error: Invalid numeric input. Please try again.\n");
        }
    }

    return value;
}


//  caseInsensitiveEquals:
//  Returns 1 if s1 and s2 are equal ignoring case, otherwise 0.

int caseInsensitiveEquals(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return 0;
    }
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 0;
        }
        s1++;
        s2++;
    }
    return (*s1 == '\0' && *s2 == '\0');
}


// SYMPTOM KEYWORD CLASSIFICATION 
int containsKeyword(const char *text, const char *keyword) {
    if (text == NULL || keyword == NULL) {
        return 0;
    }

    char lowerText[128];
    char lowerKeyword[64];

    /* Convert text to lowercase */
    int i = 0;
    while (text[i] != '\0' && i < (int)sizeof(lowerText) - 1) {
        lowerText[i] = (char)tolower((unsigned char)text[i]);
        i++;
    }
    lowerText[i] = '\0';

    /* Convert keyword to lowercase */
    int j = 0;
    while (keyword[j] != '\0' && j < (int)sizeof(lowerKeyword) - 1) {
        lowerKeyword[j] = (char)tolower((unsigned char)keyword[j]);
        j++;
    }
    lowerKeyword[j] = '\0';

    return (strstr(lowerText, lowerKeyword) != NULL);
}


// classifyPriority
int classifyPriority(const char *condition) {
    /* 1. Critical Priority */
    if (containsKeyword(condition, "unconscious") ||
        containsKeyword(condition, "cardiac arrest") ||
        containsKeyword(condition, "severe bleeding")) {
        return 1;
    }

    /* 2. High Priority */
    if (containsKeyword(condition, "severe injury") ||
        containsKeyword(condition, "breathing difficulty")) {
        return 2;
    }

    /* 3. Moderate Priority */
    if (containsKeyword(condition, "fever") ||
        containsKeyword(condition, "vomiting") ||
        containsKeyword(condition, "moderate pain")) {
        return 3;
    }

    /* 4. Low Priority */
    if (containsKeyword(condition, "minor cut") ||
        containsKeyword(condition, "cold") ||
        containsKeyword(condition, "mild headache")) {
        return 4;
    }

    /* Default if nothing matched */
    return 3;
}


//  assignDepartment
const char* assignDepartment(const char *condition) {
    if (containsKeyword(condition, "accident") ||
        containsKeyword(condition, "severe bleeding") ||
        containsKeyword(condition, "unconscious")) {
        return DEPT_EMERGENCY;
    }

    if (containsKeyword(condition, "heart attack") ||
        containsKeyword(condition, "chest pain") ||
        containsKeyword(condition, "cardiac arrest")) {
        return DEPT_CARDIOLOGY;
    }

    if (containsKeyword(condition, "fracture") ||
        containsKeyword(condition, "broken bone")) {
        return DEPT_ORTHOPEDICS;
    }

    if (containsKeyword(condition, "fever") ||
        containsKeyword(condition, "cold") ||
        containsKeyword(condition, "infection") ||
        containsKeyword(condition, "headache")) {
        return DEPT_GENERAL_MEDICINE;
    }

    return DEPT_GENERAL_MEDICINE;
}


// getPriorityName:
const char* getPriorityName(int priority) {
    switch (priority) {
        case 1: return "Critical";
        case 2: return "High";
        case 3: return "Moderate";
        case 4: return "Low";
        default: return "Moderate";
    }
}


// PATIENT LINKED LIST FUNCTIONS 
    // createPatient
struct Patient* createPatient(int id, const char *name, int age,
                              const char *address, const char *phone,
                              const char *condition, int priority,
                              const char *department, const char *doctorID,
                              const char *bedID, const char *status) {
    struct Patient *newNode = (struct Patient *)malloc(sizeof(struct Patient));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed for new patient record.\n");
        return NULL;
    }

    newNode->patientID = id;
    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0';

    newNode->age = age;

    strncpy(newNode->address, address, sizeof(newNode->address) - 1);
    newNode->address[sizeof(newNode->address) - 1] = '\0';

    strncpy(newNode->phone, phone, sizeof(newNode->phone) - 1);
    newNode->phone[sizeof(newNode->phone) - 1] = '\0';

    strncpy(newNode->condition, condition, sizeof(newNode->condition) - 1);
    newNode->condition[sizeof(newNode->condition) - 1] = '\0';

    newNode->priority = priority;

    strncpy(newNode->department, department, sizeof(newNode->department) - 1);
    newNode->department[sizeof(newNode->department) - 1] = '\0';

    strncpy(newNode->doctorID, doctorID, sizeof(newNode->doctorID) - 1);
    newNode->doctorID[sizeof(newNode->doctorID) - 1] = '\0';

    strncpy(newNode->bedID, bedID, sizeof(newNode->bedID) - 1);
    newNode->bedID[sizeof(newNode->bedID) - 1] = '\0';

    strncpy(newNode->status, status, sizeof(newNode->status) - 1);
    newNode->status[sizeof(newNode->status) - 1] = '\0';

    newNode->next = NULL;

    return newNode;
}


// addPatient
void addPatient(struct Patient *newPatient) {
    if (newPatient == NULL) {
        return;
    }

    if (patientHead == NULL) {
        patientHead = newPatient;
    } else {
        struct Patient *temp = patientHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPatient;
    }
}


// searchPatientByID
struct Patient* searchPatientByID(int id) {
    struct Patient *curr = patientHead;
    while (curr != NULL) {
        if (curr->patientID == id) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}


// displayPatientCard
void displayPatientCard(const struct Patient *p) {
    if (p == NULL) return;

    printf("\n--------------------------------------------------\n");
    printf("Patient ID:    %d\n", p->patientID);
    printf("Name:          %s\n", p->name);
    printf("Age:           %d\n", p->age);
    printf("Address:       %s\n", p->address);
    printf("Phone:         %s\n", p->phone);
    printf("Condition:     %s\n", p->condition);
    printf("Priority:      %d (%s)\n", p->priority, getPriorityName(p->priority));
    printf("Department:    %s\n", p->department);
    printf("Doctor ID:     %s\n", p->doctorID);
    printf("Bed ID:        %s\n", p->bedID);
    printf("Status:        %s\n", p->status);
    printf("--------------------------------------------------\n");
}


// displayPatients
void displayPatients(void) {
    if (patientHead == NULL) {
        printf("\nNo patient records found in the system.\n");
        return;
    }

    printf("\n===================================================================================================================\n");
    printf("                                              PATIENT RECORDS                                                      \n");
    printf("===================================================================================================================\n");
    printf("%-6s | %-18s | %-4s | %-20s | %-4s | %-16s | %-6s | %-6s | %-14s\n",
           "ID", "Name", "Age", "Condition", "Pri", "Department", "Doctor", "Bed", "Status");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    struct Patient *curr = patientHead;
    while (curr != NULL) {
        printf("%-6d | %-18.18s | %-4d | %-20.20s | %-4d | %-16.16s | %-6s | %-6s | %-14s\n",
               curr->patientID,
               curr->name,
               curr->age,
               curr->condition,
               curr->priority,
               curr->department,
               curr->doctorID,
               curr->bedID,
               curr->status);
        curr = curr->next;
    }
    printf("===================================================================================================================\n");
}


// searchPatientByName
int searchPatientByName(const char *name) {
    int matches = 0;
    struct Patient *curr = patientHead;

    while (curr != NULL) {
        if (caseInsensitiveEquals(curr->name, name)) {
            displayPatientCard(curr);
            matches++;
        }
        curr = curr->next;
    }

    return matches;
}


// freePatientList
void freePatientList(void) {
    struct Patient *curr = patientHead;
    while (curr != NULL) {
        struct Patient *temp = curr;
        curr = curr->next;
        free(temp);
    }
    patientHead = NULL;
}


// USER INTERFACE MENU ACTIONS
    // registerPatientUI
void registerPatientUI(void) {
    char name[50];
    int age;
    char address[100];
    char phone[16];
    char condition[100];

    printf("\n--- Register New Patient ---\n");

    /* 1. Name input */
    do {
        printf("Enter Patient Name: ");
        readLine(name, sizeof(name));
        if (strlen(name) == 0) {
            printf("Error: Patient name cannot be empty. Please enter a valid name.\n");
        }
    } while (strlen(name) == 0);

    /* 2. Age input */
    age = readInteger("Enter Patient Age (1-120): ", 1, 120);

    /* 3. Address input */
    do {
        printf("Enter Address: ");
        readLine(address, sizeof(address));
        if (strlen(address) == 0) {
            printf("Error: Address cannot be empty.\n");
        }
    } while (strlen(address) == 0);

    /* 4. Phone input */
    do {
        printf("Enter Phone Number: ");
        readLine(phone, sizeof(phone));
        if (strlen(phone) == 0) {
            printf("Error: Phone number cannot be empty.\n");
        }
    } while (strlen(phone) == 0);

    /* 5. Condition input */
    do {
        printf("Enter Condition / Reported Symptoms: ");
        readLine(condition, sizeof(condition));
        if (strlen(condition) == 0) {
            printf("Error: Condition cannot be empty.\n");
        }
    } while (strlen(condition) == 0);

    /* Perform academic symptom keyword classification */
    int priority = classifyPriority(condition);
    const char *department = assignDepartment(condition);

    int assignedID = nextPatientID++;

    struct Patient *newPatient = createPatient(assignedID, name, age, address, phone,
                                               condition, priority, department,
                                               "NONE", "NONE", STATUS_WAITING);

    if (newPatient != NULL) {
        addPatient(newPatient);
        printf("\n==================================================\n");
        printf("          PATIENT REGISTERED SUCCESSFULLY         \n");
        printf("==================================================\n");
        printf("Assigned Patient ID: %d\n", assignedID);
        printf("Condition Reported:  %s\n", condition);
        printf("Urgency Priority:    %d (%s)\n", priority, getPriorityName(priority));
        printf("Assigned Department: %s\n", department);
        printf("Initial Status:      %s (Doctor: NONE, Bed: NONE)\n", STATUS_WAITING);

        /* Place in appropriate waiting queue */
        enqueuePatient(assignedID);

        printf("Note: Urgency and department are based on academic\n");
        printf("      keyword rules, not clinical medical diagnosis.\n");
        printf("==================================================\n");

        /* Save to persistent file */
        savePatients();

        /* Automatic Allocation immediately after registration */
        printf("\n[Automatic Allocation] Checking hospital resources for Patient ID %d...\n", assignedID);
        processWaitingPatients();
    }
}


// searchPatientUI
void searchPatientUI(void) {
    if (patientHead == NULL) {
        printf("\nNo patient records available to search.\n");
        return;
    }

    printf("\n--- Search Patient ---\n");
    printf("1. Search by Patient ID\n");
    printf("2. Search by Patient Name\n");
    int choice = readInteger("Enter search option (1-2): ", 1, 2);

    if (choice == 1) {
        int id = readInteger("Enter Patient ID to search: ", 1000, 999999);
        struct Patient *p = searchPatientByID(id);
        if (p != NULL) {
            printf("\nPatient Found:");
            displayPatientCard(p);
        } else {
            printf("\nNo patient found with ID: %d\n", id);
        }
    } else {
        char name[50];
        do {
            printf("Enter Patient Name to search: ");
            readLine(name, sizeof(name));
            if (strlen(name) == 0) {
                printf("Error: Search name cannot be empty.\n");
            }
        } while (strlen(name) == 0);

        printf("\nSearching for patients with name \"%s\"...\n", name);
        int count = searchPatientByName(name);
        if (count == 0) {
            printf("No patient found with name \"%s\".\n", name);
        } else {
            printf("Found %d matching patient record(s).\n", count);
        }
    }
}


// updatePatientUI
void updatePatientUI(void) {
    if (patientHead == NULL) {
        printf("\nNo patient records available to update.\n");
        return;
    }

    printf("\n--- Update Patient Details ---\n");
    int id = readInteger("Enter Patient ID to update: ", 1000, 999999);
    struct Patient *p = searchPatientByID(id);

    if (p == NULL) {
        printf("\nPatient ID %d not found.\n", id);
        return;
    }

    printf("\nCurrent Details:");
    displayPatientCard(p);

    if (strcmp(p->status, STATUS_DISCHARGED) == 0) {
        printf("Notice: Patient is Discharged. Discharged patient details cannot be modified.\n");
        return;
    }

    printf("\nSelect what to update:\n");
    printf("1. Update Contact Information (Name, Age, Address, Phone)\n");
    if (strcmp(p->status, STATUS_WAITING) == 0) {
        printf("2. Update Condition (Recalculates Priority, Dept & Queues)\n");
    } else if (strcmp(p->status, STATUS_ADMITTED) == 0) {
        printf("2. Advance Status to 'Under Treatment'\n");
    }
    printf("3. Cancel\n");

    int choice = readInteger("Enter choice: ", 1, 3);

    if (choice == 1) {
        char buffer[100];

        printf("Enter New Name (leave blank to keep \"%s\"): ", p->name);
        readLine(buffer, sizeof(buffer));
        if (strlen(buffer) > 0) {
            strncpy(p->name, buffer, sizeof(p->name) - 1);
            p->name[sizeof(p->name) - 1] = '\0';
        }

        printf("Current Age: %d. Update age? (1 = Yes, 2 = Keep): ", p->age);
        int changeAge = readInteger("", 1, 2);
        if (changeAge == 1) {
            p->age = readInteger("Enter New Age (1-120): ", 1, 120);
        }

        printf("Enter New Address (leave blank to keep \"%s\"): ", p->address);
        readLine(buffer, sizeof(buffer));
        if (strlen(buffer) > 0) {
            strncpy(p->address, buffer, sizeof(p->address) - 1);
            p->address[sizeof(p->address) - 1] = '\0';
        }

        printf("Enter New Phone (leave blank to keep \"%s\"): ", p->phone);
        readLine(buffer, sizeof(buffer));
        if (strlen(buffer) > 0) {
            strncpy(p->phone, buffer, sizeof(p->phone) - 1);
            p->phone[sizeof(p->phone) - 1] = '\0';
        }

        savePatients();
        printf("\nContact details updated and saved successfully!\n");
    } else if (choice == 2) {
        if (strcmp(p->status, STATUS_WAITING) == 0) {
            char newCondition[100];
            do {
                printf("Enter New Condition: ");
                readLine(newCondition, sizeof(newCondition));
                if (strlen(newCondition) == 0) {
                    printf("Error: Condition cannot be empty.\n");
                }
            } while (strlen(newCondition) == 0);

            strncpy(p->condition, newCondition, sizeof(p->condition) - 1);
            p->condition[sizeof(p->condition) - 1] = '\0';

            /* Re-classify priority and department */
            p->priority = classifyPriority(p->condition);
            const char *newDept = assignDepartment(p->condition);
            strncpy(p->department, newDept, sizeof(p->department) - 1);
            p->department[sizeof(p->department) - 1] = '\0';

            /* Reconstruct queues so patient moves to correct queue/position */
            rebuildQueues();

            /* Save changes to file */
            savePatients();

            printf("\n==================================================\n");
            printf("            CONDITION UPDATED SUCCESSFULLY        \n");
            printf("==================================================\n");
            printf("New Condition:       %s\n", p->condition);
            printf("Recalculated Pri:    %d (%s)\n", p->priority, getPriorityName(p->priority));
            printf("Assigned Department: %s\n", p->department);
            printf("Queues reconstructed and saved to file.\n");
            printf("==================================================\n");

            /* Automatic allocation attempt after condition change */
            printf("\n[Automatic Allocation] Checking resources following condition update...\n");
            processWaitingPatients();
        } else if (strcmp(p->status, STATUS_ADMITTED) == 0) {
            strcpy(p->status, STATUS_UNDER_TREATMENT);
            savePatients();
            printf("\nStatus updated: Patient is now 'Under Treatment' (Saved to file).\n");
        }
    } else {
        printf("\nUpdate cancelled.\n");
    }
}

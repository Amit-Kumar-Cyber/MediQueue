// 2. Amit Kumar 

#include "mediqueue.h"

/* Hospital resource arrays */
struct Doctor doctors[MAX_DOCTORS];
struct Bed beds[MAX_BEDS];


// emergencyQueue: holds Critical and patients
int emergencyQueue[MAX_QUEUE];
int emergencyQueueCount = 0;

// normalQueue: holds Moderate and Low patients.
int normalQueue[MAX_QUEUE];
int normalQueueCount = 0;


/*  RESOURCE INITIALIZATION FUNCTIONS   */
    //initializeDoctors
void initializeDoctors(void) {
    /* Emergency Department */
    strcpy(doctors[0].doctorID, "D101");
    strcpy(doctors[0].name, "Dr. Rajesh Sharma");
    strcpy(doctors[0].department, DEPT_EMERGENCY);
    strcpy(doctors[0].status, STATUS_AVAILABLE);

    strcpy(doctors[1].doctorID, "D102");
    strcpy(doctors[1].name, "Dr. Priya Patel");
    strcpy(doctors[1].department, DEPT_EMERGENCY);
    strcpy(doctors[1].status, STATUS_AVAILABLE);

    /* Cardiology Department */
    strcpy(doctors[2].doctorID, "D103");
    strcpy(doctors[2].name, "Dr. Amit Verma");
    strcpy(doctors[2].department, DEPT_CARDIOLOGY);
    strcpy(doctors[2].status, STATUS_AVAILABLE);

    strcpy(doctors[3].doctorID, "D104");
    strcpy(doctors[3].name, "Dr. Sunita Rao");
    strcpy(doctors[3].department, DEPT_CARDIOLOGY);
    strcpy(doctors[3].status, STATUS_AVAILABLE);

    /* Orthopedics Department */
    strcpy(doctors[4].doctorID, "D105");
    strcpy(doctors[4].name, "Dr. Vikram Singh");
    strcpy(doctors[4].department, DEPT_ORTHOPEDICS);
    strcpy(doctors[4].status, STATUS_AVAILABLE);

    strcpy(doctors[5].doctorID, "D106");
    strcpy(doctors[5].name, "Dr. Neha Gupta");
    strcpy(doctors[5].department, DEPT_ORTHOPEDICS);
    strcpy(doctors[5].status, STATUS_AVAILABLE);

    /* General Medicine Department */
    strcpy(doctors[6].doctorID, "D107");
    strcpy(doctors[6].name, "Dr. Suresh Kumar");
    strcpy(doctors[6].department, DEPT_GENERAL_MEDICINE);
    strcpy(doctors[6].status, STATUS_AVAILABLE);

    strcpy(doctors[7].doctorID, "D108");
    strcpy(doctors[7].name, "Dr. Anjali Mehta");
    strcpy(doctors[7].department, DEPT_GENERAL_MEDICINE);
    strcpy(doctors[7].status, STATUS_AVAILABLE);
}


// initializeBeds
void initializeBeds(void) {
    for (int i = 0; i < MAX_BEDS; i++) {
        sprintf(beds[i].bedID, "B%d", 101 + i);
        strcpy(beds[i].status, STATUS_AVAILABLE);
    }
}



/*  RESOURCE DISPLAY FUNCTIONS */
    // displayDoctors
void displayDoctors(void) {
    printf("\n=======================================================================\n");
    printf("                         HOSPITAL DOCTORS                              \n");
    printf("=======================================================================\n");
    printf("%-8s | %-22s | %-18s | %-10s\n", "Doc ID", "Doctor Name", "Department", "Status");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < MAX_DOCTORS; i++) {
        printf("%-8s | %-22s | %-18s | %-10s\n",
               doctors[i].doctorID,
               doctors[i].name,
               doctors[i].department,
               doctors[i].status);
    }
    printf("=======================================================================\n");
}


// displayBeds
void displayBeds(void) {
    printf("\n=========================================\n");
    printf("             HOSPITAL BEDS               \n");
    printf("=========================================\n");
    printf("%-10s | %-15s\n", "Bed ID", "Status");
    printf("-----------------------------------------\n");

    for (int i = 0; i < MAX_BEDS; i++) {
        printf("%-10s | %-15s\n", beds[i].bedID, beds[i].status);
    }
    printf("=========================================\n");
}



/*  WAITING QUEUE FUNCTIONS   */
    //insertPriorityQueue
int insertPriorityQueue(int patientID) {
    if (emergencyQueueCount >= MAX_QUEUE) {
        printf("Error: Emergency Priority Queue capacity reached (%d patients)!\n", MAX_QUEUE);
        return 0;
    }

    struct Patient *newPatient = searchPatientByID(patientID);
    if (newPatient == NULL) {
        return 0;
    }

    /* Find insertion point: insert after elements with priority <= newPatient->priority */
    int pos = 0;
    while (pos < emergencyQueueCount) {
        struct Patient *curr = searchPatientByID(emergencyQueue[pos]);
        if (curr != NULL && curr->priority > newPatient->priority) {
            break;
        }
        pos++;
    }

    /* Shift elements right to create space */
    for (int i = emergencyQueueCount; i > pos; i--) {
        emergencyQueue[i] = emergencyQueue[i - 1];
    }

    emergencyQueue[pos] = patientID;
    emergencyQueueCount++;
    return 1;
}


//removePriorityQueueAt:
void removePriorityQueueAt(int index) {
    if (index < 0 || index >= emergencyQueueCount) {
        return;
    }
    for (int i = index; i < emergencyQueueCount - 1; i++) {
        emergencyQueue[i] = emergencyQueue[i + 1];
    }
    emergencyQueueCount--;
}


//removePriorityQueueByID:
int removePriorityQueueByID(int patientID) {
    for (int i = 0; i < emergencyQueueCount; i++) {
        if (emergencyQueue[i] == patientID) {
            removePriorityQueueAt(i);
            return 1;
        }
    }
    return 0;
}


//insertNormalQueue:
int insertNormalQueue(int patientID) {
    if (normalQueueCount >= MAX_QUEUE) {
        printf("Error: Normal Queue capacity reached (%d patients)!\n", MAX_QUEUE);
        return 0;
    }
    normalQueue[normalQueueCount++] = patientID;
    return 1;
}


//removeNormalQueueAt:
void removeNormalQueueAt(int index) {
    if (index < 0 || index >= normalQueueCount) {
        return;
    }
    for (int i = index; i < normalQueueCount - 1; i++) {
        normalQueue[i] = normalQueue[i + 1];
    }
    normalQueueCount--;
}


// removeNormalQueueByID
int removeNormalQueueByID(int patientID) {
    for (int i = 0; i < normalQueueCount; i++) {
        if (normalQueue[i] == patientID) {
            removeNormalQueueAt(i);
            return 1;
        }
    }
    return 0;
}


// enqueuePatient:
void enqueuePatient(int patientID) {
    struct Patient *p = searchPatientByID(patientID);
    if (p == NULL) return;

    if (p->priority == 1 || p->priority == 2) {
        if (insertPriorityQueue(patientID)) {
            printf("Patient ID %d placed in Emergency Priority Queue (Position: %d)\n",
                   patientID, emergencyQueueCount);
        }
    } else {
        if (insertNormalQueue(patientID)) {
            printf("Patient ID %d placed in Normal FIFO Queue (Position: %d)\n",
                   patientID, normalQueueCount);
        }
    }
}


//rebuildQueues:
void rebuildQueues(void) {
    emergencyQueueCount = 0;
    normalQueueCount = 0;

    struct Patient *curr = patientHead;
    while (curr != NULL) {
        if (strcmp(curr->status, STATUS_WAITING) == 0) {
            if (curr->priority == 1 || curr->priority == 2) {
                insertPriorityQueue(curr->patientID);
            } else {
                insertNormalQueue(curr->patientID);
            }
        }
        curr = curr->next;
    }
}


// displayQueues:
void displayQueues(void) {
    printf("\n========================================================================================\n");
    printf("                    EMERGENCY PRIORITY QUEUE (Critical & High)                          \n");
    printf("========================================================================================\n");
    if (emergencyQueueCount == 0) {
        printf("No patients currently waiting in Emergency Priority Queue.\n");
    } else {
        printf("%-5s | %-6s | %-20s | %-14s | %-18s\n",
               "Pos", "ID", "Patient Name", "Priority", "Department");
        printf("----------------------------------------------------------------------------------------\n");
        for (int i = 0; i < emergencyQueueCount; i++) {
            struct Patient *p = searchPatientByID(emergencyQueue[i]);
            if (p != NULL) {
                char priLabel[20];
                sprintf(priLabel, "%d (%s)", p->priority, getPriorityName(p->priority));
                printf("%-5d | %-6d | %-20.20s | %-14s | %-18s\n",
                       i + 1, p->patientID, p->name, priLabel, p->department);
            }
        }
    }
    printf("Total Waiting in Emergency Queue: %d\n", emergencyQueueCount);
    printf("========================================================================================\n");

    printf("\n========================================================================================\n");
    printf("                       NORMAL FIFO QUEUE (Moderate & Low)                               \n");
    printf("========================================================================================\n");
    if (normalQueueCount == 0) {
        printf("No patients currently waiting in Normal FIFO Queue.\n");
    } else {
        printf("%-5s | %-6s | %-20s | %-14s | %-18s\n",
               "Pos", "ID", "Patient Name", "Priority", "Department");
        printf("----------------------------------------------------------------------------------------\n");
        for (int i = 0; i < normalQueueCount; i++) {
            struct Patient *p = searchPatientByID(normalQueue[i]);
            if (p != NULL) {
                char priLabel[20];
                sprintf(priLabel, "%d (%s)", p->priority, getPriorityName(p->priority));
                printf("%-5d | %-6d | %-20.20s | %-14s | %-18s\n",
                       i + 1, p->patientID, p->name, priLabel, p->department);
            }
        }
    }
    printf("Total Waiting in Normal Queue: %d\n", normalQueueCount);
    printf("========================================================================================\n");
}



/*  RESOURCE ALLOCATION ENGINE  */
    //findAvailableDoctor:
int findAvailableDoctor(const char *department) {
    if (department == NULL) return -1;

    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (caseInsensitiveEquals(doctors[i].department, department) &&
            strcmp(doctors[i].status, STATUS_AVAILABLE) == 0) {
            return i;
        }
    }
    return -1;
}

//findAvailableBed:
int findAvailableBed(void) {
    for (int i = 0; i < MAX_BEDS; i++) {
        if (strcmp(beds[i].status, STATUS_AVAILABLE) == 0) {
            return i;
        }
    }
    return -1;
}


//allocateResources
int allocateResources(struct Patient *p) {
    if (p == NULL) return 0;

    int docIdx = findAvailableDoctor(p->department);
    int bedIdx = findAvailableBed();

    /* All-or-Nothing check */
    if (docIdx != -1 && bedIdx != -1) {
        strcpy(doctors[docIdx].status, STATUS_BUSY);
        strcpy(beds[bedIdx].status, STATUS_OCCUPIED);

        strcpy(p->doctorID, doctors[docIdx].doctorID);
        strcpy(p->bedID, beds[bedIdx].bedID);
        strcpy(p->status, STATUS_ADMITTED);

        printf("-> ALLOCATED: Patient %d (%s) admitted to Dept '%s' | Doctor: %s (%s) | Bed: %s\n",
               p->patientID, p->name, p->department,
               doctors[docIdx].name, doctors[docIdx].doctorID,
               beds[bedIdx].bedID);
        return 1;
    }

    return 0;
}


//releaseResources:
void releaseResources(struct Patient *p) {
    if (p == NULL) return;

    /* Release doctor if assigned */
    if (strcmp(p->doctorID, "NONE") != 0 && strlen(p->doctorID) > 0) {
        for (int i = 0; i < MAX_DOCTORS; i++) {
            if (strcmp(doctors[i].doctorID, p->doctorID) == 0) {
                strcpy(doctors[i].status, STATUS_AVAILABLE);
                printf("Doctor %s (%s) is now Available.\n", doctors[i].name, doctors[i].doctorID);
                break;
            }
        }
    }

    /* Release bed if assigned */
    if (strcmp(p->bedID, "NONE") != 0 && strlen(p->bedID) > 0) {
        for (int i = 0; i < MAX_BEDS; i++) {
            if (strcmp(beds[i].bedID, p->bedID) == 0) {
                strcpy(beds[i].status, STATUS_AVAILABLE);
                printf("Bed %s is now Available.\n", beds[i].bedID);
                break;
            }
        }
    }
}


//processWaitingPatients:
int processWaitingPatients(void) {
    int allocatedCount = 0;

    printf("\n--- Processing Waiting Patients ---\n");

    /* Step 1: Process Emergency Priority Queue */
    int i = 0;
    while (i < emergencyQueueCount) {
        int patientID = emergencyQueue[i];
        struct Patient *p = searchPatientByID(patientID);

        if (p != NULL && strcmp(p->status, STATUS_WAITING) == 0) {
            if (allocateResources(p)) {
                removePriorityQueueAt(i);
                allocatedCount++;
                continue;
            } else {
                printf("Notice: Patient ID %d (%s) remains Waiting (Doctor for '%s' or Bed currently unavailable).\n",
                       p->patientID, p->name, p->department);
            }
        }
        i++;
    }

    /* Step 2: Process Normal FIFO Queue */
    int j = 0;
    while (j < normalQueueCount) {
        int patientID = normalQueue[j];
        struct Patient *p = searchPatientByID(patientID);

        if (p != NULL && strcmp(p->status, STATUS_WAITING) == 0) {
            if (allocateResources(p)) {
                removeNormalQueueAt(j);
                allocatedCount++;
                continue;
            } else {
                printf("Notice: Patient ID %d (%s) remains Waiting (Doctor for '%s' or Bed currently unavailable).\n",
                       p->patientID, p->name, p->department);
            }
        }
        j++;
    }

    if (allocatedCount == 0) {
        printf("No waiting patients could be allocated at this time due to resource constraints.\n");
    } else {
        printf("Successfully allocated resources for %d patient(s).\n", allocatedCount);
        savePatients();
    }

    return allocatedCount;
}


//dischargePatientUI:
void dischargePatientUI(void) {
    if (patientHead == NULL) {
        printf("\nNo patient records available.\n");
        return;
    }

    printf("\n--- Discharge Patient ---\n");
    int id = readInteger("Enter Patient ID to discharge: ", 1000, 999999);
    struct Patient *p = searchPatientByID(id);

    if (p == NULL) {
        printf("\nPatient ID %d not found.\n", id);
        return;
    }

    if (strcmp(p->status, STATUS_DISCHARGED) == 0) {
        printf("\nError: Patient ID %d is already discharged. Cannot discharge again.\n", id);
        return;
    }

    if (strcmp(p->status, STATUS_WAITING) == 0) {
        printf("\nError: Patient ID %d is currently Waiting. Cannot discharge a patient who was never admitted.\n", id);
        return;
    }

    printf("\nReleasing resources for Patient ID %d (%s)...\n", p->patientID, p->name);
    releaseResources(p);

    strcpy(p->status, STATUS_DISCHARGED);
    printf("Patient ID %d has been successfully discharged.\n", p->patientID);
    printf("Patient record retained in system with status 'Discharged'.\n");

    /* Save to file */
    savePatients();

    /* Automatic queue processing after discharge */
    printf("\n[Automatic Allocation] Triggering waiting queue check with newly freed resources...\n");
    processWaitingPatients();
}

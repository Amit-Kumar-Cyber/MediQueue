# MediQueue 🏥
### Hospital Patient Management & Emergency Prioritization System

MediQueue is a **C-based Hospital Patient Management System** designed to help hospital staff efficiently manage patient registration, emergency prioritization, department allocation, bed management, doctor assignment, and patient records.

The system automatically evaluates a patient's reported medical condition using predefined medical rules and keywords to determine their emergency level. Based on the assessment, patients are categorized into four priority levels: **Critical, High, Moderate, and Low**.

Patients requiring immediate medical attention are placed in a priority queue, while regular patients are managed through a normal queue. The system also identifies the appropriate medical department, assigns an available hospital bed, and allocates a suitable doctor.

MediQueue uses fundamental Data Structures and Algorithms concepts, including linked lists, queues, priority queues, arrays, searching, sorting, and file handling, to provide an organized and efficient patient management solution.

---

## ✨ Key Features

### 1. Patient Management
- Register new patients with personal and medical information.
- Automatically generate unique Patient IDs.
- Store patient name, age, address, phone number, and medical condition.
- Search patients using Patient ID or name.
- Update patient information and treatment status.
- Discharge patients and maintain their records.

### 2. Automatic Emergency Assessment
- Analyze the patient's reported symptoms and condition.
- Apply predefined medical rules and keywords.
- Automatically determine the emergency priority.
- Categorize patients into four levels:
  - **Critical:** Requires immediate attention.
  - **High:** Requires urgent medical attention.
  - **Moderate:** Requires timely medical attention.
  - **Low:** Non-urgent medical cases.

### 3. Queue Management
- Maintain a priority queue for emergency patients.
- Maintain a normal queue for non-critical patients.
- Process patients according to their assigned priority.
- Ensure emergency cases receive precedence over regular cases.

### 4. Medical Department Allocation
- Identify the appropriate medical department based on the patient's condition.
- Categorize patients according to their medical requirements.
- Direct patients to the relevant department.

### 5. Bed Management
- Maintain a record of available and occupied hospital beds.
- Automatically assign an available bed to eligible patients.
- Update bed availability after patient discharge.
- Display current bed allocation status.

### 6. Doctor Allocation
- Maintain doctor records and availability.
- Assign available doctors according to the patient's medical department.
- Update doctor availability when treatment is completed.
- Display assigned doctors and their current status.

### 7. Patient Records & File Handling
- Store patient records in text files.
- Retrieve previously stored patient information.
- Maintain patient records between program executions.
- Support adding, searching, updating, and removing records.

### 8. Hospital Dashboard
- Display registered patients.
- Display critical and regular patient queues.
- Display patient treatment status.
- Display available and occupied beds.
- Display available and assigned doctors.

---

## 🛠️ Technologies & Concepts

| Technology / Concept | Purpose |
|---|---|
| C Programming | Core application development |
| Structures (`struct`) | Organizing patient, doctor, and bed information |
| Linked List | Dynamic patient record management |
| Priority Queue | Managing emergency patients |
| Queue | Managing regular patients |
| Arrays | Managing hospital resources |
| Searching Algorithms | Finding patient records |
| Sorting Algorithms | Organizing patient records |
| File Handling | Persistent storage of patient records |
| Pointers & Dynamic Memory | Efficient memory management |

---

## ⚙️ System Workflow

The general workflow of MediQueue is:

1. **Patient Registration:** Staff enter the patient's personal and medical information.
2. **Emergency Assessment:** The system analyzes the reported condition using predefined rules and keywords.
3. **Priority Assignment:** The patient is categorized as Critical, High, Moderate, or Low.
4. **Department Identification:** The system determines the appropriate medical department.
5. **Queue Allocation:** Emergency patients enter the priority queue, while regular patients enter the normal queue.
6. **Bed Allocation:** The system searches for and assigns an available hospital bed.
7. **Doctor Allocation:** An available doctor from the appropriate department is assigned.
8. **Treatment Management:** Patient treatment status is updated as the process continues.
9. **Discharge:** After treatment, the patient is discharged and the assigned bed and doctor are made available again.
10. **Record Management:** Patient information is stored in files for future retrieval.

---

## 🏗️ Data Structures Used

MediQueue demonstrates the practical application of fundamental data structures.

| Data Structure | Implementation |
|---|---|
| Singly Linked List | Patient records |
| Priority Queue | Emergency patient management |
| Normal Queue | Regular patient management |
| Arrays | Doctor and bed management |
| Structures | Patient, doctor, bed, and department details |
| File Handling | Persistent patient record storage |

---

## 📂 Project Structure

The project will follow a modular structure to keep the code organized and maintainable.

```text
MediQueue/
│
├── src/
│   ├── main.c
│   ├── patient.c
│   ├── patient.h
│   ├── emergency.c
│   ├── emergency.h
│   ├── queue.c
│   ├── queue.h
│   ├── doctor.c
│   ├── doctor.h
│   ├── bed.c
│   ├── bed.h
│   ├── department.c
│   └── department.h
│
├── data/
│   └── patients.txt
│
├── docs/
│   ├── Project_Specification.md
│   └── Project_Notes.md
│
├── README.md
└── .gitignore
```

*Note: This is the proposed project structure. Files may be added or modified during development.*

---

## 🚀 Getting Started

### Prerequisites

- GCC Compiler
- C development environment
- Code editor (VS Code recommended)

### Installation

**1. Clone the repository**

```bash
git clone <repository-url>
```

**2. Navigate to the project directory**

```bash
cd MediQueue
```

**3. Compile the project**

Once the source files are implemented, compile them using GCC:

```bash
gcc src/*.c -o MediQueue
```

**4. Run the application**

Windows:

```bash
MediQueue.exe
```

Linux:

```bash
./MediQueue
```

---

## 🎯 Project Objectives

The primary objectives of MediQueue are:

- Develop a hospital patient management system using C.
- Automate basic emergency prioritization through predefined rules.
- Implement priority-based and normal patient queues.
- Demonstrate linked list operations for dynamic patient management.
- Implement automatic hospital bed and doctor allocation.
- Apply searching and sorting algorithms to patient records.
- Use file handling to maintain persistent patient information.
- Demonstrate practical applications of Data Structures and Algorithms in healthcare management.

---

## ⚠️ Important Disclaimer

MediQueue is an academic project developed for educational and demonstration purposes.

Its emergency assessment is based on predefined rules and keywords. It is **not a clinically validated triage system** and must not replace professional medical evaluation or decisions made by qualified healthcare personnel.

---

## 👥 Project Information

| Detail | Information |
|---|---|
| Project Name | MediQueue |
| Programming Language | C |
| Project Domain | Hospital Management System |
| Project Type | Academic / Educational Project |
| Core Subject | Data Structures and Algorithms |
| Storage Method | Text File Handling |

---

## 📌 Future Enhancements

Potential improvements include:

- Graphical User Interface (GUI).
- Database integration using MySQL.
- Advanced medical assessment rules.
- Doctor scheduling and shift management.
- Multiple hospital department support.
- Patient treatment history and reports.
- Improved security and user authentication.

---

**MediQueue — Smarter Patient Management Through Data Structures & Algorithms.**

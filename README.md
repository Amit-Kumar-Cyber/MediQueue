# MediQueue

MediQueue is a C-based hospital patient management system that helps hospital staff handle patients as they arrive for treatment. Instead of asking staff to judge the emergency level manually, the system collects basic patient information (name, age, address, phone number, and a description of the condition or injury) and applies predefined medical rules and keywords to determine the emergency level and assign a priority.
After evaluating the patient's condition, MediQueue identifies the appropriate medical department, assigns an available bed, and allocates an available doctor. Patients who need immediate attention go ahead of less critical patients through a priority queue, while regular patients wait in a normal queue.
The system keeps patient records in a linked list, so patients can be added, searched, updated, and removed without needing a fixed block of memory. Arrays hold hospital resources such as beds and doctors, and searching and sorting techniques locate patients and organize records efficiently.
Major Features
•	Register new patients with personal and medical information.
•	Automatically determine the patient's emergency level based on the reported condition.
•	Categorize patients into critical, high, moderate, or low-priority cases.
•	Maintain a priority-based treatment queue.
•	Maintain a normal queue for non-critical patients.
•	Identify the appropriate medical department.
•	Assign an available hospital bed.
•	Assign an available doctor according to the patient's condition/department.
•	Search patient records using Patient ID or name.
•	Update patient information and treatment status.
•	Discharge patients and make beds/doctors available again.
•	Display current patients, queues, available beds, and doctors.
•	Store and retrieve patient records using file handling.

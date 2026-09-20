#include <stdio.h>
#include <string.h>

// Maximum limits
#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4

// Lookup Data: Doctor Specialties
const char* SPECIALTY_NAMES[NUM_SPECIALTIES] = { "General Practice (OPD)", "Paediatrics", "Cardiology", "Orthopedics" };

const double BASE_FEES[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 3500.00};
const int CONSULTATION_TIMES[NUM_SPECIALTIES] = {15, 20, 30, 25};

// Function prototypes
void displayMenu();


int main()
{
    int choice;
    do {
            displayMenu();

            printf("Enter your choice (1-4): ");

            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("\n--- Patient Registration ---\n");

                    // TODO: Patient registration and billing
                    break;

                case 2:
                    printf("\n--- Sorted Patient Queue ---\n");
                    // TODO: Priority Sorting Display
                    break;

                case 3:
                    printf("\n--- Analytics &amp; Performance Reports ---\n");
                    // TODO: System analytics
                    break;

                case 4:
                    printf("\nExiting System. Thank you!\\n");
                    break;

                default:
                    printf("Invalid choice! Please try again.\n");

            }
        } while (choice != 4);

        return 0;

}
        void displayMenu() {
            printf("\n========================================\n");
            printf(" SMART HOSPITAL ALLOCATION SYSTEM \n");
            printf("========================================\n");
            printf("1. Register New Patient &amp; Calculate Bill\n");
            printf("2. View Priority Patient Queue\n");
            printf("3. View Analytics &amp; Reports\n");
            printf("4. Exit\n");
            printf("----------------------------------------\n");
    }




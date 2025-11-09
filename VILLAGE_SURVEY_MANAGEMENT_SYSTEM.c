#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VILLAGERS 500  // maximum villagers that can be loaded

struct Villager {
    int id;
    char name[50];
    int age;
    char gender[10];
    char occupation[50];
    char education[50];
    int familySize;
    float income;
};

void addRecord();
void displayRecords();
void searchRecord();
void updateRecord();
void deleteRecord();
int idExists(int id);  // helper to check if ID already exists
int loadData(struct Villager villagers[]); // helper to load data

void menu() {
    printf("\n==== VILLAGE SURVEY SYSTEM ====\n");
    printf("1. Add Villager Record\n");
    printf("2. Display All Records (Sorted by ID)\n");
    printf("3. Search Record\n");
    printf("4. Update Record\n");
    printf("5. Delete Record\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    while (1) {
        menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayRecords(); break;
            case 3: searchRecord(); break;
            case 4: updateRecord(); break;
            case 5: deleteRecord(); break;
            case 6:
                printf("----- Exiting program. Goodbye! -----\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Check if ID already exists
int idExists(int id) {
    struct Villager v;
    FILE *fp = fopen("survey.txt", "r");
    if (fp==NULL)return 0; // file doesn't exist, so ID can't exist

    while (fscanf(fp, "%d %s %d %s %s %s %d %f",
                  &v.id, v.name, &v.age, v.gender, v.occupation,
                  v.education, &v.familySize, &v.income) != EOF) {
        if (v.id == id) {
            fclose(fp);
            return 1; // duplicate found
        }
    }
    fclose(fp);
    return 0;
}

// Add a new record (with duplicate ID check)
void addRecord() {
    struct Villager v;
    FILE *fp = fopen("survey.txt", "a");
    if (fp==NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Villager Details:\n");

    do {
        printf("ID (unique): ");
        scanf("%d", &v.id);
        if (idExists(v.id))
            printf("  ID already exists! Please enter a different ID....\n");
        else
            break;
    } while (1);

    getchar(); 
    printf("Name: ");
    fgets(v.name, 50, stdin);
    v.name[strcspn(v.name, "\n")] = '\0';
    printf("Age: ");
    scanf("%d", &v.age);
    printf("Gender: ");
    scanf("%s", v.gender);
    printf("Occupation: ");
    scanf("%s", v.occupation);
    printf("Education Level (None/Primary/Secondary/Intermediate/Graduate): ");
    scanf("%s", v.education);
    printf("Family Size: ");
    scanf("%d", &v.familySize);
    printf("Monthly Income: ");
    scanf("%f", &v.income);

    fprintf(fp, "%d %s %d %s %s %s %d %.2f\n",
            v.id, v.name, v.age, v.gender, v.occupation,
            v.education, v.familySize, v.income);

    fclose(fp);
    printf(" Record added successfully!\n");
}

// Helper to load data into array and return total records
int loadData(struct Villager villagers[]) {
    FILE *fp = fopen("survey.txt", "r");
    if (!fp) return 0;
    int count = 0;
    while (fscanf(fp, "%d %s %d %s %s %s %d %f",
                  &villagers[count].id, villagers[count].name, &villagers[count].age,
                  villagers[count].gender, villagers[count].occupation,
                  villagers[count].education, &villagers[count].familySize,
                  &villagers[count].income) != EOF) {
        count++;
    }
    fclose(fp);
    return count;
}

// Display all records sorted by ID
void displayRecords() {
    struct Villager villagers[MAX_VILLAGERS];
    int n = loadData(villagers);
    if (n == 0) {
        printf("No records found.\n");
        return;
    }

    // Sort by ID (Bubble sort)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (villagers[j].id > villagers[j + 1].id) {
                struct Villager temp = villagers[j];
                villagers[j] = villagers[j + 1];
                villagers[j + 1] = temp;
            }
        }
    }

    printf("\n--- All Villager Records (Sorted by ID) ---\n");
    printf("%-5s %-20s %-5s %-10s %-15s %-12s %-12s %-10s\n",
           "ID", "Name", "Age", "Gender", "Occupation",
           "Education", "FamilySize", "Income");

    for (int i = 0; i < n; i++) {
        printf("%-5d %-20s %-5d %-10s %-15s %-12s %-12d %-10.2f\n",
               villagers[i].id, villagers[i].name, villagers[i].age,
               villagers[i].gender, villagers[i].occupation,
               villagers[i].education, villagers[i].familySize,
               villagers[i].income);
    }
}

// Search record by ID
void searchRecord() {
    struct Villager v;
    FILE *fp = fopen("survey.txt", "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter ID to search: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d %s %s %s %d %f",
                  &v.id, v.name, &v.age, v.gender, v.occupation,
                  v.education, &v.familySize, &v.income) != EOF) {
        if (v.id == id) {
            printf("\nRecord Found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nGender: %s\nOccupation: %s\nEducation: %s\nFamily Size: %d\nIncome: %.2f\n",
                   v.id, v.name, v.age, v.gender, v.occupation,
                   v.education, v.familySize, v.income);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record not found.\n");

    fclose(fp);
}

// Update record by ID
void updateRecord() {
    struct Villager v;
    FILE *fp = fopen("survey.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    int id, found = 0;
    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d %s %s %s %d %f",
                  &v.id, v.name, &v.age, v.gender, v.occupation,
                  v.education, &v.familySize, &v.income) != EOF) {
        if (v.id == id) {
            printf("\nEnter new details:\n");
            getchar();
            printf("Name: ");
            fgets(v.name, 50, stdin);
            v.name[strcspn(v.name, "\n")] = '\0';
            printf("Age: ");
            scanf("%d", &v.age);
            printf("Gender: ");
            scanf("%s", v.gender);
            printf("Occupation: ");
            scanf("%s", v.occupation);
            printf("Education: ");
            scanf("%s", v.education);
            printf("Family Size: ");
            scanf("%d", &v.familySize);
            printf("Income: ");
            scanf("%f", &v.income);
            found = 1;
        }
        fprintf(temp, "%d %s %d %s %s %s %d %.2f\n",
                v.id, v.name, v.age, v.gender, v.occupation,
                v.education, v.familySize, v.income);
    }

    fclose(fp);
    fclose(temp);
    remove("survey.txt");
    rename("temp.txt", "survey.txt");

    if (found)
        printf("Record updated successfully!\n");
    else
        printf("Record not found.\n");
}

// Delete record by ID
void deleteRecord() {
    struct Villager v;
    FILE *fp = fopen("survey.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    int id, found = 0;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d %s %s %s %d %f",
                  &v.id, v.name, &v.age, v.gender, v.occupation,
                  v.education, &v.familySize, &v.income) != EOF) {
        if (v.id != id)
            fprintf(temp, "%d %s %d %s %s %s %d %.2f\n",
                    v.id, v.name, v.age, v.gender,
                    v.occupation, v.education, v.familySize, v.income);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);
    remove("survey.txt");
    rename("temp.txt", "survey.txt");

    if (found)
        printf("Record deleted successfully!\n");
    else
        printf("Record not found.\n");
}


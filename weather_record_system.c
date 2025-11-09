#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 200
#define FILENAME "weather.txt"

struct Weather {
    int id;
    char city[50];
    char date[20];
    float temperature;
    float humidity;
    float rainfall;
    char condition[30];
};

// Example of union usage (store either Celsius or Fahrenheit)
union TempUnit {
    float celsius;
    float fahrenheit;
};

struct Weather records[MAX];
int count = 0;

// Function declarations
void loadData();
void saveData();
void addRecord();
void editRecord();
void deleteRecord();
void searchRecord();
void showHotDays();
void showColdDays();
void generateRandomRecords();
int findRecordByID(int id);

// Random cities and conditions
char *cities[] = {"Delhi","Mumbai","Chennai","Kolkata","Bangalore","Hyderabad","Pune","Ahmedabad","Jaipur","Lucknow"};
char *conditions[] = {"Sunny","Cloudy","Rainy","Stormy","Humid","Windy","Foggy","Clear"};

// MAIN FUNCTION
int main() {
    int choice;
    srand(time(NULL));

    loadData();
    if (count < MAX)
        generateRandomRecords();

    while (1) {
        printf("\n 4---WEATHER MANAGEMENT SYSTEM---\n");
        printf("1. Add Weather Record\n");
        printf("2. Edit Weather Record\n");
        printf("3. Delete Weather Record\n");
        printf("4. Search Weather Record\n");
        printf("5. Show Hot Days (Temp > 30°C)\n");
        printf("6. Show Cold Days (Temp < 15°C)\n");
        printf("7. Save & Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addRecord(); break;
            case 2: editRecord(); break;
            case 3: deleteRecord(); break;
            case 4: searchRecord(); break;
            case 5: showHotDays(); break;
            case 6: showColdDays(); break;
            case 7: saveData(); printf("Data saved. Exiting...\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

// FUNCTIONS

// Generate random weather records up to 200
void generateRandomRecords() {
    while (count < MAX) {
        struct Weather w;
        w.id = count + 1;
        strcpy(w.city, cities[rand() % 10]);
        sprintf(w.date, "2025-%02d-%02d", rand() % 12 + 1, rand() % 28 + 1);
        w.temperature = (rand() % 4000) / 100.0 - 5; // between -5 to 35°C
        w.humidity = (rand() % 10000) / 100.0;      // 0–100%
        w.rainfall = (rand() % 2000) / 10.0;        // 0–200 mm
        strcpy(w.condition, conditions[rand() % 8]);
        records[count++] = w;
    }
}

// Add new weather record
void addRecord() {
    if (count >= MAX) {
        printf("Cannot add more records.\n");
        return;
    }
    struct Weather w;
    w.id = count + 1;
    printf("Enter city: ");
    fgets(w.city, 50, stdin); w.city[strcspn(w.city, "\n")] = 0;
    printf("Enter date (YYYY-MM-DD): ");
    fgets(w.date, 20, stdin); w.date[strcspn(w.date, "\n")] = 0;
    printf("Enter temperature (°C): ");
    scanf("%f", &w.temperature);
    printf("Enter humidity (%%): ");
    scanf("%f", &w.humidity);
    printf("Enter rainfall (mm): ");
    scanf("%f", &w.rainfall);
    getchar();
    printf("Enter condition (e.g., Sunny, Rainy): ");
    fgets(w.condition, 30, stdin); w.condition[strcspn(w.condition, "\n")] = 0;
    records[count++] = w;
    printf("Weather record added successfully!\n");
}

// Edit existing weather record
void editRecord() {
    int id;
    printf("Enter record ID to edit: ");
    scanf("%d", &id);
    getchar();
    int i = findRecordByID(id);
    if (i == -1) {
        printf("Record not found!\n");
        return;
    }
    printf("Editing %s on %s\n", records[i].city, records[i].date);
    printf("Enter new temperature (°C): ");
    scanf("%f", &records[i].temperature);
    printf("Enter new humidity (%%): ");
    scanf("%f", &records[i].humidity);
    printf("Enter new rainfall (mm): ");
    scanf("%f", &records[i].rainfall);
    getchar();
    printf("Enter new condition: ");
    fgets(records[i].condition, 30, stdin); records[i].condition[strcspn(records[i].condition, "\n")] = 0;
    printf("Record updated!\n");
}

// Delete a record
void deleteRecord() {
    int id;
    printf("Enter record ID to delete: ");
    scanf("%d", &id);
    int i = findRecordByID(id);
    if (i == -1) {
        printf("Record not found!\n");
        return;
    }
    for (int j = i; j < count - 1; j++)
        records[j] = records[j + 1];
    count--;
    printf("Record deleted!\n");
}

// Search record by City or Date
void searchRecord() {
    char key[50];
    printf("Enter city or date to search: ");
    getchar();
    fgets(key, 50, stdin); key[strcspn(key, "\n")] = 0;
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcasecmp(records[i].city, key) == 0 || strcmp(records[i].date, key) == 0) {
            printf("\nID: %d | City: %s | Date: %s | Temp: %.1f°C | Humidity: %.1f%% | Rainfall: %.1fmm | %s\n",
                   records[i].id, records[i].city, records[i].date,
                   records[i].temperature, records[i].humidity, records[i].rainfall, records[i].condition);
            found = 1;
        }
    }
    if (!found)
        printf("No matching record found!\n");
}

// Show hot days
void showHotDays() {
    printf("\n--- Hot Days (Temp > 30°C) ---\n");
    for (int i = 0; i < count; i++)
        if (records[i].temperature > 30)
            printf("%d | %s | %s | %.1f°C | %s\n",
                   records[i].id, records[i].city, records[i].date, records[i].temperature, records[i].condition);
}

// Show cold days
void showColdDays() {
    printf("\n--- Cold Days (Temp < 15°C) ---\n");
    for (int i = 0; i < count; i++)
        if (records[i].temperature < 15)
            printf("%d | %s | %s | %.1f°C | %s\n",
                   records[i].id, records[i].city, records[i].date, records[i].temperature, records[i].condition);
}

// Find record by ID
int findRecordByID(int id) {
    for (int i = 0; i < count; i++)
        if (records[i].id == id) return i;
    return -1;
}

// Load from text file
void loadData() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) return;
    while (fscanf(fp, "%d,%[^,],%[^,],%f,%f,%f,%[^\n]\n",
                  &records[count].id, records[count].city, records[count].date,
                  &records[count].temperature, &records[count].humidity,
                  &records[count].rainfall, records[count].condition) == 7)
        count++;
    fclose(fp);
}

// Save to text file
void saveData() {
    FILE *fp = fopen(FILENAME, "w");
    if (!fp) return;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%.2f,%.2f,%.2f,%s\n",
                records[i].id, records[i].city, records[i].date,
                records[i].temperature, records[i].humidity,
                records[i].rainfall, records[i].condition);
    }
    fclose(fp);
}


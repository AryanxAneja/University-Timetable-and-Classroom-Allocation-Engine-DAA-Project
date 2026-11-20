#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COURSES 50
#define MAX_ROOMS 20
#define MAX_SLOTS 10
#define MAX_SCHEDULE 50

typedef struct {
    int id;
    char name[50];
    char faculty[50];
    char group[30];
    int students;
} Course;

typedef struct {
    int id;
    char name[30];
    int capacity;
} Room;

typedef struct {
    int id;
    char time[30];
} TimeSlot;

typedef struct {
    int courseId;
    int roomId;
    int slotId;
} Schedule;

Course courses[MAX_COURSES];
Room rooms[MAX_ROOMS];
TimeSlot slots[MAX_SLOTS];
Schedule timetable[MAX_SCHEDULE];

int courseCount = 0, roomCount = 0, slotCount = 0, scheduledCount = 0;

void loadData() {
    courseCount = 6;
    roomCount = 3;
    slotCount = 4;

    courses[0] = (Course){101, "DSA", "Dr Sharma", "CSE-A", 40};
    courses[1] = (Course){102, "DBMS", "Dr Verma", "CSE-A", 40};
    courses[2] = (Course){103, "OS", "Dr Singh", "CSE-B", 35};
    courses[3] = (Course){104, "Networking", "Dr Sharma", "CSE-B", 35};
    courses[4] = (Course){105, "Mathematics", "Dr Gupta", "CSE-A", 40};
    courses[5] = (Course){106, "AI/ML", "Dr Kumar", "CSE-B", 30};

    rooms[0] = (Room){1, "Room 101", 50};
    rooms[1] = (Room){2, "Room 102", 40};
    rooms[2] = (Room){3, "Lab 201", 35};

    slots[0] = (TimeSlot){1, "09:00-10:00"};
    slots[1] = (TimeSlot){2, "10:00-11:00"};
    slots[2] = (TimeSlot){3, "11:00-12:00"};
    slots[3] = (TimeSlot){4, "12:00-01:00"};

    scheduledCount = 0;
    printf("\nSample university data loaded!\n");
}

int hasConflict(int courseIndex, int roomIndex, int slotIndex) {
    for (int i = 0; i < scheduledCount; i++) {
        int oldCourse = timetable[i].courseId;
        if (timetable[i].slotId != slotIndex)
            continue;

        if (strcmp(courses[courseIndex].faculty, courses[oldCourse].faculty) == 0)
            return 1;
        if (strcmp(courses[courseIndex].group, courses[oldCourse].group) == 0)
            return 1;
        if (roomIndex == timetable[i].roomId)
            return 1;
    }
    return 0;
}

int backtracking(int courseIndex) {
    if (courseIndex == courseCount)
        return 1;

    for (int s = 0; s < slotCount; s++) {
        for (int r = 0; r < roomCount; r++) {
            if (rooms[r].capacity < courses[courseIndex].students)
                continue;

            if (!hasConflict(courseIndex, r, s)) {
                timetable[scheduledCount++] = (Schedule){courseIndex, r, s};
                if (backtracking(courseIndex + 1))
                    return 1;
                scheduledCount--;
            }
        }
    }
    return 0;
}

void greedyScheduling() {
    scheduledCount = 0;
    int unscheduled = 0;

    for (int c = 0; c < courseCount; c++) {
        int assigned = 0;

        for (int s = 0; s < slotCount && !assigned; s++) {
            for (int r = 0; r < roomCount; r++) {
                if (rooms[r].capacity < courses[c].students)
                    continue;

                if (!hasConflict(c, r, s)) {
                    timetable[scheduledCount++] = (Schedule){c, r, s};
                    assigned = 1;
                    break;
                }
            }
        }

        if (!assigned) {
            printf("Unscheduled course: %s\n", courses[c].name);
            unscheduled++;
        }
    }

    printf("\nGreedy scheduling result: %d of %d courses scheduled.\n",
           scheduledCount, courseCount);
    if (unscheduled)
        printf("%d course(s) could not be scheduled. Try Backtracking.\n", unscheduled);
    else
        printf("All courses were scheduled successfully.\n");
}

void displayTimetable() {
    if (scheduledCount == 0) {
        printf("\nNo timetable generated yet!\n");
        return;
    }

    printf("\n========== UNIVERSITY TIMETABLE ==========\n");
    printf("%-15s %-18s %-15s %-15s %-15s\n",
           "Course", "Faculty", "Group", "Room", "Time");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < scheduledCount; i++) {
        int c = timetable[i].courseId;
        int r = timetable[i].roomId;
        int s = timetable[i].slotId;
        printf("%-15s %-18s %-15s %-15s %-15s\n",
               courses[c].name, courses[c].faculty, courses[c].group,
               rooms[r].name, slots[s].time);
    }
}

void merge(int arr[], int left, int mid, int right) {
    int temp[MAX_COURSES];
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right)
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (i = left, k = 0; i <= right; i++)
        arr[i] = temp[k++];
}

void mergeSort(int arr[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int binarySearch(int arr[], int n, int key) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void roomUtilization() {
    if (roomCount == 0) {
        printf("\nLoad data first!\n");
        return;
    }

    printf("\n========== ROOM UTILIZATION ==========\n");
    for (int r = 0; r < roomCount; r++) {
        int used = 0;
        for (int i = 0; i < scheduledCount; i++)
            if (timetable[i].roomId == r) used++;

        printf("%s : %d/%d slots used (%.2f%%)\n",
               rooms[r].name, used, slotCount,
               (float)used / slotCount * 100);
    }
}

void exportCSV() {
    if (scheduledCount == 0) {
        printf("\nGenerate a timetable first!\n");
        return;
    }

    FILE *fp = fopen("timetable.csv", "w");
    if (fp == NULL) {
        printf("Unable to create CSV file!\n");
        return;
    }

    fprintf(fp, "Course,Faculty,Group,Room,Time\n");
    for (int i = 0; i < scheduledCount; i++) {
        int c = timetable[i].courseId, r = timetable[i].roomId, s = timetable[i].slotId;
        fprintf(fp, "%s,%s,%s,%s,%s\n", courses[c].name, courses[c].faculty,
                courses[c].group, rooms[r].name, slots[s].time);
    }
    fclose(fp);
    printf("\nTimetable exported to timetable.csv\n");
}

/* Divide and Conquer: Maximum Subarray */
int maxCrossingSum(int a[], int left, int mid, int right) {
    int sum = 0, leftSum = -2147483647, rightSum = -2147483647;

    for (int i = mid; i >= left; i--) {
        sum += a[i];
        if (sum > leftSum) leftSum = sum;
    }

    sum = 0;
    for (int i = mid + 1; i <= right; i++) {
        sum += a[i];
        if (sum > rightSum) rightSum = sum;
    }
    return leftSum + rightSum;
}

int maxSubarray(int a[], int left, int right) {
    if (left == right) return a[left];

    int mid = left + (right - left) / 2;
    int x = maxSubarray(a, left, mid);
    int y = maxSubarray(a, mid + 1, right);
    int z = maxCrossingSum(a, left, mid, right);

    int best = x > y ? x : y;
    return best > z ? best : z;
}

void maximumSubarrayModule() {
    int n, a[100];
    printf("Enter number of elements (1-100): ");
    scanf("%d", &n);
    if (n < 1 || n > 100) {
        printf("Invalid size!\n");
        return;
    }

    printf("Enter array elements: ");
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    printf("Maximum subarray sum: %d\n", maxSubarray(a, 0, n - 1));
}

void divideConquerMenu() {
    int choice;
    do {
        printf("\n--- DIVIDE AND CONQUER MODULES ---\n");
        printf("1. Maximum Subarray Sum\n");
        printf("2. Return to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                maximumSubarrayModule();
                break;
            case 2:
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 2);
}
int main() {
    int choice;

    do {
        printf("\n====================================");
        printf("\n UNIVERSITY TIMETABLE SYSTEM");
        printf("\n====================================");
        printf("\n1. Load Sample Data");
        printf("\n2. Generate using Greedy Algorithm");
        printf("\n3. Generate using Backtracking");
        printf("\n4. Display Timetable");
        printf("\n5. Calculate Room Utilization");
        printf("\n6. Export Timetable to CSV");
        printf("\n7. Sort Course IDs");
        printf("\n8. Search Course ID");
        printf("\n9. Divide and Conquer Modules");
        printf("\n10. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                loadData();
                break;
            case 2:
                if (courseCount == 0) {
                    printf("Load data first!\n");
                    break;
                }
                greedyScheduling();
                break;
            case 3:
                if (courseCount == 0) {
                    printf("Load data first!\n");
                    break;
                }
                scheduledCount = 0;
                if (backtracking(0))
                    printf("\nValid timetable generated! All %d courses scheduled.\n", courseCount);
                else {
                    printf("\nNo complete timetable found.\n");
                    scheduledCount = 0;
                }
                break;
            case 4:
                displayTimetable();
                break;
            case 5:
                roomUtilization();
                break;
            case 6:
                exportCSV();
                break;
            case 7: {
                if (courseCount == 0) {
                    printf("Load data first!\n");
                    break;
                }
                int ids[MAX_COURSES];
                for (int i = 0; i < courseCount; i++) ids[i] = courses[i].id;
                mergeSort(ids, 0, courseCount - 1);
                printf("\nSorted Course IDs:\n");
                for (int i = 0; i < courseCount; i++) printf("%d ", ids[i]);
                printf("\n");
                break;
            }
            case 8: {
                if (courseCount == 0) {
                    printf("Load data first!\n");
                    break;
                }
                int ids[MAX_COURSES], key;
                for (int i = 0; i < courseCount; i++) ids[i] = courses[i].id;
                mergeSort(ids, 0, courseCount - 1);
                printf("Enter Course ID to search: ");
                scanf("%d", &key);
                if (binarySearch(ids, courseCount, key) != -1)
                    printf("Course ID %d found!\n", key);
                else
                    printf("Course not found!\n");
                break;
            }
            case 9:
                divideConquerMenu();
                break;
            case 10:
                printf("\nExiting University Timetable System.\n");
                break;
            default:
                printf("\nInvalid choice!\n");
        }
    } while (choice != 10);

    return 0;
}

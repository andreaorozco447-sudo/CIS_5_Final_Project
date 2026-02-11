#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int MAX_READING = 100;

// Structure to hold blood pressure readings
struct BloodPressure {
    string date;
    int systolic = 0;
    int diastolic = 0;
};
// Menu 
void displayMenu() {
    cout << "\nBlood Pressure Tracker\n";
    cout << "1. Add Reading\n";
    cout << "2. Display Readings\n";
    cout << "3. Sort by Date\n";
    cout << "4. Search by Date\n";
    cout << "5. View Statistics\n";
    cout << "6. View Category Distribution\n";
    cout << "7. Save & Exit\n";
    cout << "Enter your choice: ";
}

// Validation
bool validate(int sys, int dia) { 
    if (sys <= 0 || dia <= 0) return false;
    if (dia > sys) return false;
    if (sys > 300) return false;
    return true;
}

// Category
string getCategory(int sys, int dia) 
{     if (sys >= 180 || dia >= 120)  return "Hypertensive Crisis";
    else if (sys >= 140 || dia >= 90) return "Hypertension (Stage 2)";
    else if ((sys >= 130 && sys <= 139) || (dia >= 80 && dia <= 89)) return "Hypertension (Stage 1)";
    else if (sys >= 120 && sys <= 129 && dia < 80) return "Elevated";
    else if (sys < 120 && dia < 80) return "Normal";
    else if (sys < 90 || dia < 60) return "Hypotension";
    else return "Invalid Reading";
}
    
 
// reading 
void addReading(BloodPressure* readings, int& count) {
    if (count >= MAX_READING) {
        cout << "Maximum readings reached.\n";
        return;
    }
    BloodPressure bp;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> bp.date;
    cout << "Enter systolic pressure: ";
    cin >> bp.systolic;
    cout << "Enter diastolic pressure: ";
    cin >> bp.diastolic;
    if (!validate(bp.systolic, bp.diastolic)) {
        cout << "Invalid readings. Please try again.\n";
        return;
    }
    readings[count++] = bp;
    cout << "Reading added successfully.\n";

	// Alert for critical readings
    if (getCategory(bp.systolic, bp.diastolic) == "Hypertensive Crisis") {
        cout << "Alert: Hypertensive Crisis! Seek immediate medical attention.\n";
    } else if (getCategory(bp.systolic, bp.diastolic) == "Hypotension") {
        cout << "Alert: Hypotension! Consider consulting a healthcare provider.\n";
	}
}

// Display readings 
void displayReadings(const BloodPressure* readings, int count) {
    if (count == 0) {
        cout << "No readings to display.\n";
        return;
    }
    cout << "\nDate       | Systolic | Diastolic | Category\n";
    cout << "---------------------------------------------\n";
    for (int i = 0; i < count; i++) {
        cout << readings[i].date << " | "
            << std::setw(8) << readings[i].systolic << " | "
            << std::setw(9) << readings[i].diastolic << " | "
            << getCategory(readings[i].systolic, readings[i].diastolic) << endl;
    }
}

// save files
void saveToFile(const BloodPressure* readings, int count) {
    std::ofstream file("blood_pressure_readings.txt");
    if (!file) {
        cout << "Error opening file for writing.\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        file << readings[i].date << "," << readings[i].systolic << "," << readings[i].diastolic << endl;
    }
    file.close();
    cout << "Readings saved to blood_pressure_readings.txt\n";
}

// load files
void loadFromFile(BloodPressure* readings, int& count) {
    std::ifstream file("blood_pressure_readings.txt");
    if (!file) {
        cout << "No existing data found. Starting fresh.\n";
        return;
    }
    string line;
    while (std::getline(file, line) && count < MAX_READING) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.rfind(',');
        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) continue;
        readings[count].date = line.substr(0, pos1);
        readings[count].systolic = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        readings[count].diastolic = std::stoi(line.substr(pos2 + 1));
        count++;
    }
    file.close();
}

// sort by date
void sortByDate(BloodPressure* readings, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (readings[j].date > readings[j + 1].date) {
                std::swap(readings[j], readings[j + 1]);
            }
        }
    }
    cout << "Readings sorted by date.\n";
}

// search by date
void searchByDate(const BloodPressure* readings, int count) {
    string date;
    cout << "Enter date to search (YYYY-MM-DD): ";
    cin >> date;
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (readings[i].date == date) {
            cout << "Date: " << readings[i].date << ", Systolic: " << readings[i].systolic
                << ", Diastolic: " << readings[i].diastolic
                << ", Category: " << getCategory(readings[i].systolic, readings[i].diastolic) << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No readings found for the specified date.\n";
    }
}

//Statistics 
void viewStatistics(const BloodPressure* readings, int count) {
    if (count == 0) {
        cout << "No readings to calculate statistics.\n";
        return;
    }
int totalSystolic = 0, totalDiastolic = 0;
int normalCount = 0, elevatedCount = 0, stage1Count = 0, stage2Count = 0, crisisCount = 0, hypotensionCount = 0;
    for (int i = 0; i < count; i++) {
        totalSystolic += readings[i].systolic;
        totalDiastolic += readings[i].diastolic;
		string category = getCategory(readings[i].systolic, readings[i].diastolic);
        if (category == "Normal") normalCount++;
        else if (category == "Elevated") elevatedCount++;
        else if (category == "Hypertension (Stage 1)") stage1Count++;
        else if (category == "Hypertension (Stage 2)") stage2Count++;
        else if (category == "Hypertensive Crisis") crisisCount++;
		else if (category == "Hypotension") hypotensionCount++;     

    }
	cout << std::fixed << std::setprecision(2);
    cout << "Average Systolic: " << static_cast<double>(totalSystolic) / count << endl;
	cout << "Average Diastolic: " << static_cast<double>(totalDiastolic) / count << endl;
    cout << "Normal: " << normalCount << endl;
    cout << "Elevated: " << elevatedCount << endl;
    cout << "Hypertension (Stage 1): " << stage1Count << endl;
    cout << "Hypertension (Stage 2): " << stage2Count << endl;
	cout << "Hypertensive Crisis: " << crisisCount << endl;
	cout << "Hypotension: " << hypotensionCount << endl;
}

//viewCategoryDistribution
void viewCategoryDistribution(const BloodPressure* readings, int count) {
    if (count == 0) {
        cout << "No readings to calculate category distribution.\n";
        return;
    }
    int categories[6] = {0}; // Normal, Elevated, Hypertension Stage 1, Hypertension Stage 2, Hypertensive Crisis, Hypotension
    for (int i = 0; i < count; i++) {
        string category = getCategory(readings[i].systolic, readings[i].diastolic);
        if (category == "Normal") categories[0]++;
        else if (category == "Elevated") categories[1]++;
        else if (category == "Hypertension (Stage 1)") categories[2]++;
        else if (category == "Hypertension (Stage 2)") categories[3]++;
        else if (category == "Hypertensive Crisis") categories[4]++;
        else if (category == "Hypotension") categories[5]++;
    }
    cout << "Category Distribution:\n";
    cout << "Normal: " << categories[0] << endl;
    cout << "Elevated: " << categories[1] << endl;
    cout << "Hypertension (Stage 1): " << categories[2] << endl;
    cout << "Hypertension (Stage 2): " << categories[3] << endl;
    cout << "Hypertensive Crisis: " << categories[4] << endl;
    cout << "Hypotension: " << categories[5] << endl;
}


int main () {
    BloodPressure readings[MAX_READING];
    int count = 0;
    loadFromFile(readings, count);
    int choice;
    do {
        displayMenu();
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 7: ";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
        
        switch (choice) {
            case 1: addReading(readings, count); break;
            case 2: displayReadings(readings, count); break;
            case 3: sortByDate(readings, count); break;
            case 4: searchByDate(readings, count); break;
            case 5: viewStatistics(readings, count); break;
			case 6: viewCategoryDistribution(readings, count); break;
            case 7: saveToFile(readings, count); cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Please try again.\n"; break;
        }
    } while (choice != 7);
    return 0;
}
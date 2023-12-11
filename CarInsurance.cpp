#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Insurance {
protected:
    string ownerName;
    double coverageAmount;
    double premium;

public:
    Insurance() {}

    Insurance(string owner, double coverage)
        : ownerName(owner), coverageAmount(coverage), premium(0.0) {}

    virtual void calculatePremium() = 0;

    virtual void displayInsuranceDetails() const {
        cout << "Owner Name: " << ownerName << endl;
        cout << "Coverage Amount: $" << coverageAmount << endl;
        cout << "Premium: $" << premium << endl;
    }

    void updateCoverageAmount(double newCoverage) {
        coverageAmount = newCoverage;
        calculatePremium();
    }

    bool premiumExceedsLimit(double limit) const {
        return premium > limit;
    }
};

class CarInsurance : public Insurance {
private:
    string carModel;
    int manufacturingYear;
    int billDateLimit;
    double fine; // New member variable for the fine

public:
    CarInsurance() : Insurance(), carModel(""), manufacturingYear(0), billDateLimit(0), fine(0.0) {}

    CarInsurance(string owner, string model, int year, double coverage, int limit)
        : Insurance(owner, coverage), carModel(model), manufacturingYear(year), billDateLimit(limit), fine(0.0) {
        calculatePremium();
    }

    void setBillDateLimit(int limit) {
        billDateLimit = limit;
    }

    void calculatePremium() override {
        // Calculate premium based on the original formula
        premium = 0.02 * coverageAmount + 0.005 * (2023 - manufacturingYear);

        // Apply discount if the bill date is before or on the limit
        if (getCurrentYear() <= billDateLimit) {
            premium *= 0.9; // 10% discount
        }
        else{
            // Apply a fine if the bill date is exceeded
            fine = premium * 0.05; // 5% fine
            premium += fine;
        }
    }



    void getInsuranceDetailsFromUser() {
        cout << "Enter Owner Name: ";
        getline(cin, ownerName);

        cout << "Enter Car Model: ";
        getline(cin, carModel);

        cout << "Enter Manufacturing Year: ";
        cin >> manufacturingYear;

        cout << "Enter Coverage Amount: $";
        cin >> coverageAmount;

        cout << "Enter Bill Date Limit (YYYY): ";
        cin >> billDateLimit;

        cin.ignore();

        calculatePremium();
    }

    void displayInsuranceDetails() const override {
        cout << "Car Model: " << carModel << endl;
        cout << "Manufacturing Year: " << manufacturingYear << endl;
        Insurance::displayInsuranceDetails();
        cout << "Fine: $" << fine << endl;
    }

    void saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << ownerName << endl;
            outFile << carModel << endl;
            outFile << manufacturingYear << endl;
            outFile << coverageAmount << endl;
            outFile << billDateLimit << endl;
            outFile.close();
        } else {
            cerr << "Unable to open file: " << filename << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            getline(inFile, ownerName);
            getline(inFile, carModel);
            inFile >> manufacturingYear;
            inFile >> coverageAmount;
            inFile >> billDateLimit;
            inFile.ignore();
            inFile.close();
        } else {
            cerr << "Unable to open file: " << filename << endl;
        }
        calculatePremium();
    }

    int getCurrentYear() const {
        return 2023; // You can replace this with a dynamic way to get the current year
    }
};

int main() {
    CarInsurance car;

    car.getInsuranceDetailsFromUser();

    cout << "\nInsurance Details:\n";
    car.displayInsuranceDetails();

    car.saveToFile("insurance_data.txt");

    CarInsurance loadedCar;
    loadedCar.loadFromFile("insurance_data.txt");

    cout << "\nLoaded Insurance Details:\n";
    loadedCar.displayInsuranceDetails();

    return 0;
}

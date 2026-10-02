#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Base Abstract Class
class EnergyStorageBank {
protected:
    string bankID;
    double currentChargeMWh;

public:
    static int activeStorageBanks;
    static double totalDispatchedPowerMWh;

    EnergyStorageBank(string id, double charge)
        : bankID(id), currentChargeMWh(charge) {
        activeStorageBanks++;
    }

    virtual ~EnergyStorageBank() {
        cout << "[DISCONNECTED] Power Bank " << bankID << " decoupled from grid bus." << endl;
        activeStorageBanks--;
    }

    // Pure Virtual Interfaces
    virtual double calculateDispatchEfficiency() const = 0;
    virtual void outputBankTelemetry() const = 0;
};

// Static definitions outside class boundary
int EnergyStorageBank::activeStorageBanks = 0;
double EnergyStorageBank::totalDispatchedPowerMWh = 0.0;

// Derived Class 1: Lithium-Ion Battery Array
class LithiumBatteryArray : public EnergyStorageBank {
private:
    double ambientTemperatureC;
    int cycleCount;

public:
    LithiumBatteryArray(string id, double charge, double temp, int cycles)
        : EnergyStorageBank(id, charge),
          ambientTemperatureC(temp),
          cycleCount(cycles) {}

    ~LithiumBatteryArray() override {
        cout << " -> Purging coolant loops for Lithium Array " << bankID << "..." << endl;
    }

    double calculateDispatchEfficiency() const override {
        // High operating temperatures and wear cycles reduce round-trip efficiency
        double efficiency = 92.0 - (cycleCount * 0.01) - ((ambientTemperatureC > 25.0) ? (ambientTemperatureC - 25.0) * 0.4 : 0.0);
        return (efficiency < 0.0) ? 0.0 : efficiency;
    }

    void outputBankTelemetry() const override {
        double usableOutput = currentChargeMWh * (calculateDispatchEfficiency() / 100.0);
        EnergyStorageBank::totalDispatchedPowerMWh += usableOutput;

        cout << "\n==============================================" << endl;
        cout << "   LITHIUM-ION SUBSTATION ARRAY: " << bankID << endl;
        cout << "==============================================" << endl;
        cout << "  Stored Reserve       : " << fixed << setprecision(2) << currentChargeMWh << " MWh" << endl;
        cout << "  Cycle History        : " << cycleCount << " cycles" << endl;
        cout << "  Cell Temperature     : " << ambientTemperatureC << " °C" << endl;
        cout << "  Dispatch Efficiency  : " << calculateDispatchEfficiency() << " %" << endl;
        cout << "  Effective Delivery   : " << usableOutput << " MWh" << endl;
        cout << "==============================================" << endl;
    }
};

// Derived Class 2: Liquid Hydrogen Fuel Cell Module
class HydrogenFuelCellBank : public EnergyStorageBank {
private:
    double hydrogenPurityPct;
    double tankPressureBar;

public:
    HydrogenFuelCellBank(string id, double charge, double purity, double pressure)
        : EnergyStorageBank(id, charge),
          hydrogenPurityPct(purity),
          tankPressureBar(pressure) {}

    ~HydrogenFuelCellBank() override {
        cout << " -> Depressurizing H2 fuel injectors for " << bankID << "..." << endl;
    }

    double calculateDispatchEfficiency() const override {
        // Efficiency scales with gas purity and stable tank feed pressure
        double efficiency = (hydrogenPurityPct * 0.6) + ((tankPressureBar >= 300.0) ? 15.0 : 5.0);
        return (efficiency > 100.0) ? 100.0 : efficiency;
    }

    void outputBankTelemetry() const override {
        double usableOutput = currentChargeMWh * (calculateDispatchEfficiency() / 100.0);
        EnergyStorageBank::totalDispatchedPowerMWh += usableOutput;

        cout << "\n==============================================" << endl;
        cout << "   HYDROGEN FUEL CELL MODULE: " << bankID << endl;
        cout << "==============================================" << endl;
        cout << "  Stored Energy Base   : " << fixed << setprecision(2) << currentChargeMWh << " MWh" << endl;
        cout << "  Fuel Purity          : " << hydrogenPurityPct << " %" << endl;
        cout << "  Cryo Tank Pressure   : " << tankPressureBar << " bar" << endl;
        cout << "  Dispatch Efficiency  : " << calculateDispatchEfficiency() << " %" << endl;
        cout << "  Effective Delivery   : " << usableOutput << " MWh" << endl;
        cout << "==============================================" << endl;
    }
};

int main() {
    cout << "\n>>> POWER GRID CONTROLLER INITIALIZED <<<\n" << endl;

    const int TOTAL_BANKS = 2;
    EnergyStorageBank* substation[TOTAL_BANKS];

    // Bank 1: Lithium Bank (50 MWh reserve, 32°C temp, 450 charge cycles)
    substation[0] = new LithiumBatteryArray("LI-ARRAY-WEST-01", 50.0, 32.0, 450);

    // Bank 2: Fuel Cell Module (80 MWh reserve, 99.8% pure H2, 350 bar pressure)
    substation[1] = new HydrogenFuelCellBank("H2-CELL-EAST-02", 80.0, 99.8, 350.0);

    // Dynamic telemetry dispatch
    for (int i = 0; i < TOTAL_BANKS; i++) {
        substation[i]->outputBankTelemetry();
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Storage Banks Online  : " << EnergyStorageBank::activeStorageBanks << endl;
    cout << "Total Grid Dispatched Energy : " << fixed << setprecision(2)
         << EnergyStorageBank::totalDispatchedPowerMWh << " MWh" << endl;
    cout << "----------------------------------------------\n" << endl;

    cout << ">>> COMMENCING PEAK-OFF LOAD DECOMMISSION <<<\n" << endl;

    // Polymorphic destruction & cleanup
    for (int i = 0; i < TOTAL_BANKS; i++) {
        delete substation[i];
        substation[i] = nullptr;
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Storage Banks After Teardown : " << EnergyStorageBank::activeStorageBanks << endl;
    cout << "----------------------------------------------" << endl;

    return 0;
}

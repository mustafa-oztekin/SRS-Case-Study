#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <numeric>
#include <fstream>

using namespace std;


class Sensor {
protected:
    int minRange;
    int maxRange;
    unsigned int seed;
    unsigned int generationTime;

public: // getter, kapsülleme
    int getMinRange() {
        return minRange;
    }
    int getMaxRange() {
        return maxRange;
    }
    unsigned int getSeed() {
        return seed;
    }
    // setter, kapsülleme
    void setMinRange(int value) {
        minRange = value;
    }
    void setMaxRange(int value) {
        this->maxRange = value;
    }
    void setSeed(unsigned int value) {
        this->seed = value;
    }

    // A constructor in C++ is a special method that is automatically called when an object of a class is created.
    Sensor(int minRange, int maxRange) {
        this->minRange = minRange;
        this->maxRange = maxRange;
        this->seed = chrono::system_clock::now().time_since_epoch().count();
        this->generationTime = 500;
        // cout << "\nSensor sinifi constructor'i cagirildi." << endl;
    }

    virtual int generateData() { // bu özellikler değiştirilebilir. virtual
        return 1;
    }

    virtual vector<int> collectData(int numData) { // bu özellikler değiştirilebilir. virtual
        vector<int> datavector = { 1, 2, 3, 4, 5 };
        return datavector;
    }
};


class TempSensor : public Sensor {
public:
    TempSensor(int minRange, int maxRange)
        : Sensor(minRange, maxRange)
    {
        // cout << "\nTempSensor sinifi constructor'i cagirildi." << endl;
    }

    // Sensör bir adet rastgele veri üretir
    int generateData() override     // override ile class içinde tekrar yapılandırdık
    {
        mt19937 generator(this->seed); // Rastgele sayı üreteci
        uniform_int_distribution<int> distrubition(this->minRange, this->maxRange);
        int data = distrubition(generator);
        this->seed = this->seed + 1;
        return data;
    }

    // Sensör belirli sayıda veri üretip bir vektörde saklar
    vector<int> collectData(int numData) override   // override ile class içinde tekrar yapılandırdık
    {
        int x;
        vector<int> datavector;
        for (int i = 0; i < numData; ++i) {
            x = generateData(); // veri üret
            cout << x << "-";
            this_thread::sleep_for(chrono::milliseconds(this->generationTime));
            datavector.push_back(x); // vektöre ekle
        }
        cout << "\nNumber of Data Points: " << numData;
        return datavector;
    }
};


class DataProcessor {
protected:
    vector<int> rawData; // Stores the data points collected from the Sensor
    vector<double> filteredData;
    ofstream MyFile;

public:
    DataProcessor(const vector<int>& data) : rawData(data) {
        // cout << "\nDataProcessor sinifi constructor'i cagirildi." << endl;
        string fileName = "results.txt";

        // Dosyanın varlığını kontrol et
        ifstream checkFile(fileName);
        if (checkFile.good()) {
            cout << "\n" << fileName << " dosyasi mevcut, aciliyor." << endl;
        }
        else {
            cout << "\n" << fileName << " dosyasi mevcut degil, olusturuluyor." << endl;
        }
        checkFile.close(); // Kontrol işlemi için açılan dosyayı kapat

        // Dosyayı ekleme modunda aç
        MyFile.open(fileName, ios::app);

        if (!MyFile.is_open()) {
            cerr << "Dosya açılamadı! Lütfen dosya izinlerini kontrol edin." << endl;
            return;
        }
        // Dosyanın başına yeni işlem bilgisi
        MyFile << "\n\n\nYeni İşlem Başlatıldı:\n";
        for (int veri : this->rawData)
            MyFile << veri << "-";
        MyFile << "\nNumber of Data Points: " << rawData.size();
    }

    // Basit Ortalama Filtre
    void simpleAverage() {
        double sum = accumulate(this->rawData.begin(), this->rawData.end(), 0.0);
        cout << "\nAverage Value of Raw Data: " << sum / this->rawData.size();
        MyFile << "\nAverage Value of Raw Data: " << sum / this->rawData.size();

        if(filteredData.empty())
            cout << "\nFiltrelenmis veri bulunmuyor lutfen once verilerinizi filtreleyiniz!";
        else {
            sum = accumulate(this->filteredData.begin(), this->filteredData.end(), 0.0);
            cout << "\nAverage Value of Filtered Data: " << sum / this->filteredData.size();
            MyFile << "\nAverage Value of Filtered Data: " << sum / this->filteredData.size();
        }
    }

    // Hareketli Ortalama Filtre
    void movingAverage(int windowSize) {
        if (windowSize > this->rawData.size())
            cerr << "\nWindow size is larger than data size!";
        else {
            for (size_t i = 0; i <= this->rawData.size() - windowSize; ++i) {
                double windowSum = 0.0;

                // Pencere içindeki elemanları toplar
                for (size_t j = i; j < i + windowSize; ++j) {
                    windowSum += this->rawData[j];
                }

                this->filteredData.push_back(windowSum / windowSize); // Ortalama hesaplar ve saklar
            }
            cout << "\nMoving Average Filtered Data: ";
            MyFile << "\nMoving Average Filtered Data: ";
            for (double veri : this->filteredData) {
                cout << veri << "-";
                MyFile << veri << "-";
            }
        }
    }

    // Filtrelenmiş ve ham veri setinden minimum veriyi alır 
    void getMinData() {
        if (rawData.empty())
            cout << "\nHam veri bulunmuyor lutfen once sensorden olcum aliniz!";
        else {
            double minValue = rawData[0];
            for (int i = 1; i < rawData.size(); ++i) {
                if (rawData[i] < minValue)
                    minValue = rawData[i];
            }
            cout << "\nMinimum Value of Raw Data: " << minValue;
            MyFile << "\nMinimum Value of Raw Data: " << minValue;
        }

        if (filteredData.empty())
            cout << "\nFiltrelenmis veri bulunmuyor lutfen once verilerinizi filtreleyiniz!";
        else {
            double minValue = filteredData[0];
            for (int i = 1; i < filteredData.size(); ++i) {
                if (filteredData[i] < minValue)
                    minValue = filteredData[i];
            }
            cout << "\nMinimum Value of Filtered Data: " << minValue;
            MyFile << "\nMinimum Value of Filtered Data: " << minValue;
        }
    }

    // Filtrelenmiş ve ham veri setinden maksimum veriyi alır 
    void getMaxData() {
        if (rawData.empty())
            cout << "\nHam veri bulunmuyor lutfen once sensorden olcum aliniz!";
        else {
            double maxValue = rawData[0];
            for (int i = 1; i < rawData.size(); ++i) {
                if (rawData[i] > maxValue)
                    maxValue = rawData[i];
            }
            cout << "\nMaximum Value of Raw Data: " << maxValue;
            MyFile << "\nMaximum Value of Raw Data: " << maxValue;
        }

        if (filteredData.empty())
            cout << "\nFiltrelenmis veri bulunmuyor lutfen once verilerinizi filtreleyiniz!";
        else {
            double maxValue = filteredData[0];
            for (int i = 1; i < filteredData.size(); ++i) {
                if (filteredData[i] > maxValue)
                    maxValue = filteredData[i];
            }
            cout << "\nMaximum Value of Filtered Data: " << maxValue;
            MyFile << "\nMaximum Value of Filtered Data: " << maxValue;
        }
    }

    void closeFile() {
        if (MyFile.is_open())
            MyFile.close();
    }
};


int main()
{
    int numofDataPoints;
    cout << "Sensor'den kac adet veri almak istiyorsunuz: ";
    cin >> numofDataPoints;

    TempSensor myTempSensor = TempSensor(0, 255);
    vector<int> myTempSensorvector = myTempSensor.collectData(numofDataPoints);

    DataProcessor myDataProcessor = DataProcessor(myTempSensorvector);
    myDataProcessor.movingAverage(3);
    myDataProcessor.simpleAverage();
    myDataProcessor.getMinData();
    myDataProcessor.getMaxData();

    myDataProcessor.closeFile();

    cin.ignore();
    char s = getchar();
    return 0;
}

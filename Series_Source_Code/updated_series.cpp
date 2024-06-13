#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <vector>
#include <initializer_list>



// This class stores the data as a tuple
template<typename K, typename V>
struct Tuple {
    private:
        K key;
        V value;
    public:
        Tuple() = default;
        Tuple(K k, V v) {
            this->key = k;
            this->value = v;
        }
        friend std::ostream& operator<<(std::ostream& os, const Tuple& tuple) {
            os << "{Key: " << tuple.key << ", Value: " << tuple.value << "}\n";
            return os;
        }
};


template<typename K, typename V>
class Series {
    public:
        using TupleType = Tuple<K, V>;
    private:
        TupleType* _data;
        std::size_t size;
        int index;
        std::string _columnName;

    public:
        class SeriesReader {
            public:
                char delimiter;
                int    columnNumber;
            public:
                SeriesReader() : delimiter(','), columnNumber(-1) {}

                std::size_t getLineCount(const std::string& fileName) {
                    std::size_t numberOfLines = 0;      // Number of lines in the file
                    std::ifstream inFile;
                    std::string line;
                    inFile.open(fileName);
                    if (!inFile.is_open()) {
                        std::cerr << "Error opening file\n";
                        return 1; // non-zero to indicate failure
                    } else {
                        while (std::getline(inFile, line))
                            // Incriment the line counter
                            numberOfLines++;
                    }
                    inFile.close();
                    return numberOfLines;
                }

                Series<K, V> createFromCSV(const std::string& filePath, char delimiter, int columnNumber) {
                    // Get the number of lines
                    std::size_t numLines = getLineCount(filePath);
                    Series<K, V> series(numLines); // Create an instance of series with the size = numLines
                    int key_index = series.index;
                    int lineCounter = 0;
                    int posCounter = 0;
                    this->delimiter = delimiter;
                    this->columnNumber = columnNumber;
                    std::ifstream inFile;

                    // Open the file
                    inFile.open(filePath);

                     // Check for errors
                    if (!inFile.is_open()) {
                        std::cerr << "Error opening file\n";
                        return 1; // non-zero to indicate failure
                    }else {
                        std::string line;
                        while(std::getline(inFile, line)) {
                            std::istringstream lineStream(line);
                            std::string token;
                            std::string::size_type sz;
                            while (std::getline(lineStream, token, this->delimiter)) {
                                if (posCounter == this->columnNumber) {
                                    if (lineCounter == 0) {
                                        std::cout << "lineCounter := " << lineCounter << "\n";
                                        series.setColumnName(token);
                                        posCounter = 0;
                                        break;
                                    }else {
                                        // Conver to double if needed
                                        // double t = std::stod(token, &sz);
                                        // std::cout << "TOKEN := " << token << "\n";
                                        series.insert(series.index, token); 
                                        //  series.insert(series.index, t); 
                                         posCounter = 0; 
                                        break;
                                    }
                                }else {
                                    posCounter++;
                                }
                            }
                            lineCounter++;
                        }
                    }
                    return series;
                }
        };
    public:
         // Default constructor
        Series() : size(0), _data(nullptr), index(-1) {}
         // Initial constructor
        Series(std::string columnName, std::size_t size) {
            this->size = size;
            _columnName = columnName;
            _data = new TupleType[this->size];
            index = 0;
            std::cout << "Series created with size := " << this->size << "\n";
        }
         // Constructor with only size set
        Series(std::size_t size) {
            this->size = size;
            index = 0;
            _data = new TupleType[this->size];
        }
        Series(std::initializer_list<TupleType> initList) {
            size = initList.size();
            _data = new TupleType[size];
            std::size_t i = 0;
            for (const auto& item : initList) {
                _data[i++] = item;
            }
            index = i;
        }

        void setColumnName(std::string columnName) {
            this->_columnName = columnName;
            std::cout << "ColumnName := " << this->_columnName << "\n";
        }
    void insert(K key, V value) {
        if (index < size) {
            Tuple<K, V>tuple(key, value);
            _data[index] = tuple;
            index++;
        }else {
            ReAlloc(size + size/2);
            Tuple<K, V>tuple(key, value);
            _data[index] = tuple;
            index++;
        }
    }
    void ReAlloc(std::size_t newSize) {
        std::cout << "Resizing array ...\n";
        if (newSize > size) {
            TupleType* temp = new TupleType[newSize];
            for (std::size_t i=0; i < size; i++) 
                temp[i] = _data[i];
            delete[] _data;
            _data = temp;
            size = newSize;
            std::cout << "Index pointing to index-> " << index << "\n";
        }
    }

    TupleType& operator[](std::size_t i) {
        if (i < index) {
            return _data[i];
        }
    }
    const TupleType& operator[](std::size_t i) const {
            return _data[i];
    }
    std::size_t Size() {
        return size;
    }

    friend std::ostream& operator<<(std::ostream& os, const Series& series) {
            for(std::size_t i=0; i<series.size; ++i) {
                os << series[i] << (i < series.size - 1 ? "" : "}");
            }
            return os;
        }
};

int main() {

    std::cout << "Running ...\n";
    const std::string fileName = "/Users/williammurphy/Projects/FinanceProjects/git_seven/series_source_code/data/AMZN_05_15_2024.csv";


    Series<int, std::string> series1(10);
    Series<int, double>::SeriesReader series_reader1;
    Series<int, std::string>::SeriesReader series_reader2;
    Tuple<int, double>t1(0, 1.1);
    Tuple<int, double>t2(1, 2.1);
    Tuple<int, double>t3(2, 3.1);
    // Series<int, double>my_series = {t1, t2, t3};
    Series<int, double>my_series = {Tuple<int, double>(0, 1.1), 
                                    Tuple<int, double>(1, 2.2), 
                                    Tuple<int, double>(2, 3.3)};

    // Series<int, double>csv_series = series_reader1.createFromCSV(fileName, ',', 2);
    Series<int, std::string>date_series = series_reader2.createFromCSV(fileName, ',', 0);
    std::cout << my_series << std::endl;




    // std::size_t line_count = series_reader1.getLineCount(fileName);
    // std::cout << "LineCount := " << line_count << "\n";




    return 0;
}

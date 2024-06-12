/*
Developer: William Robert Murphy
Date: 06/12/2024
Summary: This serves as the base code for a book on writing C++, C, and Cython for Python programmers.
This portion of code serves as the base code for creating a Pandas-like Series class in C++.
More will be added as I continue my work on this project.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <vector>
#include <initializer_list>

template<typename KeyType, typename ValueType>
class Series {
    private:
        KeyType _columnName;
        ValueType* _data;
        size_t size;
        int position;
        // int index;
    
    public:
        // This class is used to read in data into the Series
        class SeriesReader {
            public:
                char   delimiter;              // The type of delimiter to use
                int    columnNumber;           // Which column are we reading in
            public:

                SeriesReader() : delimiter(','), columnNumber(-1) {}
                // Count the number of lines in the csv file
                size_t getLineCount(const std::string& fileName) {
                    size_t numberOfLines = 0;      // Number of lines in the file
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

                Series<KeyType, ValueType> createFromCSV(const std::string& filePath, char delimiter, int columnNumber) {
                    // Get the size of the file
                    size_t numLines = getLineCount(filePath);
                    Series<KeyType, ValueType> series(numLines);    // Create an instance of the series of size numLines

                    int lineCounter = 0;
                    int posCounter = 0;
                    this->delimiter = delimiter;
                    this->columnNumber = columnNumber;

                    std::ifstream inFile;                           // Create a file strean
                    inFile.open(filePath);                          // Open the file

                    // Check for errors
                    if (!inFile.is_open()) {
                        std::cerr << "Error opening file\n";
                        return 1; // non-zero to indicate failure
                    }else {
                        std::string line;                           // Create a string that will hold the current line
        
                        while(std::getline(inFile, line)) {
                            std::istringstream lineStream(line);    // Create a new stream for the single line
                            std::string token;                      // Create a token string
                            std::string::size_type sz;              // Create a string size 

                            // Iterate through each line and each token using the position counter
                            // to get the correct column that you want to read into series->_data
                            while (std::getline(lineStream, token, this->delimiter)) {
                                if (posCounter == this->columnNumber) {
                                    if (lineCounter == 0) {
                                        std::cout << "lineCounter := " << lineCounter << "\n";
                                        series.setColumnName(token);
                                        posCounter = 0;
                                        break;
                                    }else {
                                        double t = std::stod(token, &sz);    // Convert to a double (TODO: Make this a generic type)
                                        // std::cout << "TOKEN := " << token << "\n";
                                        series.insert(t);                   // Insert the data into the series
                                        posCounter = 0;                     // Reset the position counter
                                        break;
                                    }
                                }else {
                                    posCounter++;   // Increment
                                }
                            }
                            lineCounter++;          // Increment
                        }
                    }
                    return series;                  // Return the series
                }
        };

    public:
        // Default constructor
        Series() : size(0), _data(nullptr), position(0) {}

        // Initial constructor
        Series(KeyType columnName, size_t size) {
            this->size = size;
            this->position = 0;
            _columnName = columnName;
            _data = new ValueType[this->size];
            std::cout << "Series created with size := " << this->size << "\n";
        }
        // Constructor with only size set
        Series(size_t size) {
            this->size = size;
            this->position = 0;
            _data = new ValueType[this->size];
        }
        // Constructor with initializer list
        Series(std::initializer_list<ValueType> data) {
            // Copy over the data
            this->size = data.size();
            _data = new ValueType[data.size()];

            // std::cout << "data.size() := " << data.size() << "\n";
            // for (int value : data ) {
            //     std::cout << "value := " << value << "\n";
            // }
            std::copy(data.begin(), data.end(), _data);
            this->position = 0;
        }
         // Access data at a given index
        ValueType& operator[](int index) {
            return _data[index];
        }

        // Access data at a given index
        const ValueType& operator[](int index) const {
            if (index < size) {
                return _data[index];
            } 
        }

        void setColumnName(KeyType columnName) {
            this->_columnName = columnName;
            std::cout << "ColumnName := " << this->_columnName << "\n";
        }

        KeyType getColumnName() {
            return _columnName;
        }

        ~Series() {
            delete[] _data;      // delete the data
            this->size = -1;     // to indicate series was destroyed
            this->position = -1; // to indicate no data
            _data = nullptr;
        }

        void insert(ValueType value) {
            if (position > size) {
                _data[position] = value;
                position++;
            }

        }
        int getIndex() {
            return position;
        }

        size_t Size() {
            return size;
        }
};







int main() {

    std::cout << "Running ...\n";
    const std::string fileName = "/Users/williammurphy/Projects/FinanceProjects/git_seven/series_source_code/data/AMZN_05_15_2024.csv";
    Series<std::string, double>series1;
    Series<std::string, double>series2("Date", 10);
    Series<std::string, double>::SeriesReader series_reader;


    Series<std::string, double>csv_series = series_reader.createFromCSV(fileName, ',', 1);
    Series<std::string, double>my_series = {1.1, 2.2, 3.3, 4.4, 5.6, 99.1, 23.56789};
    std::cout << "my_series[2] := " << my_series[2] << "\n\n";
    std::cout << "my_series.Size() := " << my_series.Size() << "\n\n\n\n\n\n";

    // std::cout << "Number of rows := " << csv_series.getSize() << "\n";


    // size_t line_count;
    // line_count =  series3.getLineCount(fileName);
    // std::cout << "LINE COUNT := " << line_count << "\n";

    // static size_t line_count;
    // // line_count = Series<std::string, double>::SeriesReader::getLineCount(fileName);
    // series1::SeriesReader<std::string, double>::getline(fileName);

    return 0;
}

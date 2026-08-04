#include "data/csv_loader.h"

vector<candle> load_csv(std::string &file_address)
{
    vector<candle> vector_data;

    ifstream file(file_address);

    string line;
    getline(file, line); 

    while (getline(file, line))
    {
        stringstream file_line(line);

        string timestamp;
        string parameter;

        double open, high, low, close,adjclose, volume;

        getline(file_line, timestamp, ',');

        getline(file_line, parameter, ',');
        open = stod(parameter);

        getline(file_line, parameter, ',');
        high = stod(parameter);

        getline(file_line, parameter, ',');
        low = stod(parameter);

        getline(file_line, parameter, ',');
        close = stod(parameter);

        getline(file_line, parameter, ',');
        adjclose = stod(parameter);
        
        getline(file_line, parameter, ',');
        volume = stod(parameter);

        vector_data.push_back({timestamp,open,high,low,close,adjclose,volume});
    }

    return vector_data;
}
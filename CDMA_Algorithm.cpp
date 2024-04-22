#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cmath> // Include cmath for ceil and pow functions

using namespace std;

class CDMA
{
public:
    vector<vector<int>> wtable;
    vector<vector<int>> wtable2;
    vector<vector<int>> copy;
    vector<int> channel_sequence;
    vector<string> finalData; // Renamed to avoid conflict with the 'final' keyword

    // Function to set up CDMA parameters and build Walsh table
    void setUp(const vector<string> &data, int num_stations)
    {
        int exponent = ceil(log2(num_stations));
        int new_Stations = int(pow(2, exponent));
        int n = new_Stations;
        wtable2.resize(n, vector<int>(n));
        copy.resize(n, vector<int>(n));

        // Build Walsh table
        buildWalshTable(new_Stations, 0, new_Stations - 1, 0, new_Stations - 1, false);

        // Extract the codewords for the stations
        for (int j = 0; j < num_stations; j++)
        {
            vector<int> temp;
            for (int k = 0; k < new_Stations; k++)
            {
                temp.push_back(wtable2[j][k]);
            }
            wtable.push_back(temp);
        }

        // Encoding Procedure
        int max_length = 0;
        for (const string &str : data)
        {
            max_length = max(max_length, static_cast<int>(str.length()));
        }

        vector<vector<int>> channelMatrix(max_length, vector<int>(new_Stations, 0));

        for (int j = 0; j < max_length; j++)
        {
            for (int k = 0; k < num_stations; k++)
            {
                char c = data[k][j];
                int dataBit;
                if (c == '1')
                {
                    dataBit = 1;
                }
                else if (c == '0')
                {
                    dataBit = -1;
                }
                else
                {
                    dataBit = 0;
                }
                for (int s = 0; s < new_Stations; s++)
                {
                    channelMatrix[j][s] += (dataBit * (wtable[k][s]));
                }
            }
        }

        // Decoding Procedure
        vector<string> decoded = decode(channelMatrix, wtable, num_stations, max_length);

        // String correction
        vector<string> correctStrings = stringCorrection(decoded);

        // Final Data
        finalData = finalDataConversion(correctStrings);
    }

    // Function to build Walsh table
    void buildWalshTable(int len, int i1, int i2, int j1, int j2, bool isBar)
    {
        if (len == 2)
        {
            if (!isBar)
            {
                wtable2[i1][j1] = 1;
                wtable2[i1][j2] = 1;
                wtable2[i2][j1] = 1;
                wtable2[i2][j2] = -1;
            }
            else
            {
                wtable2[i1][j1] = -1;
                wtable2[i1][j2] = -1;
                wtable2[i2][j1] = -1;
                wtable2[i2][j2] = 1;
            }
            return;
        }

        int midi = (i1 + i2) / 2;
        int midj = (j1 + j2) / 2;

        buildWalshTable(len / 2, i1, midi, j1, midj, isBar);
        buildWalshTable(len / 2, i1, midi, midj + 1, j2, isBar);
        buildWalshTable(len / 2, midi + 1, i2, j1, midj, isBar);
        buildWalshTable(len / 2, midi + 1, i2, midj + 1, j2, !isBar);
    }

    // Function to convert text to binary
    string textToBinary(const string &text)
    {
        string binary;
        for (char c : text)
        {
            binary += bitset<8>(c).to_string(); // Convert char to 8-bit binary string
        }
        return binary;
    }

    // Function to convert binary to text
    string binaryToText(const string &binary)
    {
        string text;
        for (size_t i = 0; i < binary.length(); i += 8)
        {
            string byte = binary.substr(i, 8);
            text += static_cast<char>(stoi(byte, nullptr, 2));
        }
        return text;
    }

    // Function to decode messages
    vector<string> decode(const vector<vector<int>> &channelMatrix, const vector<vector<int>> &wtable, int num_stations, int max_length)
    {
        vector<string> decodedBinaryString(num_stations);
        for (int i = 0; i < max_length; ++i)
        {
            for (int bitPosition = 0; bitPosition < wtable.size(); ++bitPosition)
            {
                int bitValue = 0;
                for (int station = 0; station < wtable[0].size(); ++station)
                {
                    bitValue += channelMatrix[i][station] * wtable[bitPosition][station];
                }
                bitValue = bitValue / num_stations;
                // Divide by the number of stations and map the result to binary value
                string decodedMessage = (bitValue > 0) ? "1" : ((bitValue < 0) ? "0" : "*");
                decodedBinaryString[bitPosition] += decodedMessage;
            }
        }
        return decodedBinaryString;
    }

    // Function for string correction
    vector<string> stringCorrection(const vector<string> &decoded)
    {
        vector<string> correctStrings;
        for (const string &str : decoded)
        {
            string temp;
            for (char c : str)
            {
                if (c != '*')
                {
                    temp += c;
                }
                else
                {
                    break;
                }
            }
            correctStrings.push_back(temp);
        }
        return correctStrings;
    }

    // Final string conversion
    vector<string> finalDataConversion(const vector<string> &binaryData)
    {
        vector<string> finalString;
        for (const string &binStr : binaryData)
        {
            string data = binaryToText(binStr);
            finalString.push_back(data);
        }
        return finalString;
    }

    // Listening to station request
    void listen(int station_number)
    {
        cout << "Requested station: 4" << station_number << "; "
             << "Message: " << finalData[station_number - 1] << endl;
    }
};

vector<string> makeEqualLength(const vector<string> &data)
{
    // Find the maximum length among all binary strings
    int max_length = 0;
    for (const string &str : data)
    {
        max_length = max(max_length, static_cast<int>(str.length()));
    }

    // Create a vector to store updated strings
    vector<string> updated_data;

    // Pad each string with '-1's to make them equal in length
    for (const string &str : data)
    {
        string updated_str = str;
        int padding_size = max_length - updated_str.length();
        if (padding_size > 0)
        {
            updated_str.append(padding_size, '*');
        }
        updated_data.push_back(updated_str);
    }

    return updated_data;
}

vector<string> convertToBinary(const vector<string> &data)
{
    vector<string> binaryData;

    for (const string &str : data)
    {
        string binaryStr;
        for (char c : str)
        {
            binaryStr += bitset<8>(c).to_string(); // Convert each character to 8-bit binary string
        }
        binaryData.push_back(binaryStr);
    }

    return binaryData;
}

int main()
{
    cout << "-------------------------------CDMA Text Transmission------------------------\n";

    int num_stations;
    cout << "Enter number of stations: ";
    cin >> num_stations;

    cin.ignore(); // Clear newline character from buffer

    vector<string> data(num_stations);
    cout << "Enter text data for each station: " << endl;
    for (int i = 0; i < num_stations; ++i)
    {
        cout << "Station " << i + 1 << ": ";
        getline(cin, data[i]);
        // cin>>data[i];
    }

    // Before encoding, make all binary strings of equal length
    vector<string> binaryData = convertToBinary(data);
    vector<string> equalLengthData = makeEqualLength(binaryData);

    CDMA channel;
    channel.setUp(equalLengthData, num_stations);

    cout << "Enter the Station Number you want to listen to: ";
    int listen_to;
    cin >> listen_to;
    channel.listen(listen_to);

    return 0;
}
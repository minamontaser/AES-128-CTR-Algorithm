#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iterator>

#include "AES-128-CTR.h"

using namespace std;

// ----------- Helpers -----------

vector<uint8_t> stringToBytes(const string &input)
{
    return vector<uint8_t>(input.begin(), input.end());
}

string bytesToHex(const vector<uint8_t> &data)
{
    stringstream ss;
    vector<uint8_t>::const_iterator const_it = data.begin();

    while (const_it != data.end())
    {
        ss << hex << setw(2) << setfill('0') << (int)(*const_it);
        const_it++;
    }

    return ss.str();
}

vector<uint8_t> hexToBytes(const string &hex)
{
    vector<uint8_t> bytes;

    for (size_t i = 0; i < hex.length(); i += 2)
    {
        string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

string hexToText(const string &hex)
{
    vector<uint8_t> bytes = hexToBytes(hex);
    return string(bytes.begin(), bytes.end());
}

pair<string, string> splitNonceCipher(const string &input)
{
    size_t pos = input.find(':');
    if (pos == string::npos)
        throw runtime_error("Invalid format!");

    return {input.substr(0, pos), input.substr(pos + 1)};
}

signed main()
{

    AESCTR aes;

    cout << "=====================================\n";
    cout << "        AES-128 CTR TOOL\n";
    cout << "=====================================\n";

    while (true)
    {
        cout << "\n=====================================\n";
        cout << "              MAIN MENU\n";
        cout << "=====================================\n";
        cout << "[1] Encrypt\n";
        cout << "[2] Decrypt\n";
        cout << "[3] Exit\n";
        cout << "-------------------------------------\n";
        cout << "Choose: ";

        int choice, mode;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- Encryption Mode ---\n";
            cout << "[1] Input HEX\n";
            cout << "[2] Input TEXT\n";
            cout << "Choose: ";

            cin >> mode;
            cin.ignore();

            string input;

            string keyInput;
            cout << "Enter 16-byte key (hex, 32 chars): ";
            getline(cin, keyInput);

            if (mode == 1)
            {
                cout << "Enter plaintext (hex): ";
                getline(cin, input);
            }
            else if (mode == 2)
            {
                cout << "Enter plaintext (text): ";
                getline(cin, input);

                vector<uint8_t> bytes = stringToBytes(input);
                input = bytesToHex(bytes);
            }
            else
            {
                cout << "Invalid mode!\n";
                break;
            }

            auto start = chrono::high_resolution_clock::now();
            pair<vector<uint8_t>, vector<uint8_t>> ciphertext = aes.encrypt(input, keyInput);
            auto end = chrono::high_resolution_clock::now();

            string nonceHex = bytesToHex(ciphertext.first);
            string cipherHex = bytesToHex(ciphertext.second);
            cout << "\n========== RESULT ==========\n";
            cout << "Nonce Key(hex): " << nonceHex << " : " << " Cipher Text(hex): " << cipherHex << "\n";
            cout << "Time: "
                 << chrono::duration<double, milli>(end - start).count()
                 << " ms\n";
            cout << "============================\n";
            cout << "Copy that to decrypt: " << nonceHex << ":" << cipherHex << "\n";

            break;
        }

        case 2:
        {
            cout << "\n--- Decryption Mode ---\n";
            cout << "[1] Output HEX\n";
            cout << "[2] Output TEXT\n";
            cout << "Choose: ";

            cin >> mode;
            cin.ignore();

            string keyInput, fullInput;

            cout << "Enter 16-byte key (hex, 32 chars): ";
            getline(cin, keyInput);

            cout << "Enter (nonce:ciphertext): ";
            getline(cin, fullInput);

            try
            {
                auto [nonceHexIn, cipherHex] = splitNonceCipher(fullInput);

                vector<uint8_t> key = hexToBytes(keyInput);
                vector<uint8_t> nonce = hexToBytes(nonceHexIn);
                vector<uint8_t> ciphertext = hexToBytes(cipherHex);

                auto start = chrono::high_resolution_clock::now();
                auto plaintextBytes = aes.decrypt(ciphertext, key, nonce);
                auto end = chrono::high_resolution_clock::now();

                cout << "\n========== RESULT ==========\n";

                if (mode == 1)
                {
                    cout << "Plain Text(hex): " << bytesToHex(plaintextBytes) << "\n";
                }
                else if (mode == 2)
                {
                    cout << "Plaintext (text): "
                         << string(plaintextBytes.begin(), plaintextBytes.end()) << "\n";
                }
                else
                {
                    cout << "Invalid mode!\n";
                }

                cout << "Time: "
                     << chrono::duration<double, milli>(end - start).count()
                     << " ms\n";

                cout << "============================\n";
            }
            catch (...)
            {
                cout << "Decryption failed! Invalid format or key.\n";
            }

            break;
        }

        case 3:
            cout << "Exiting...\n";
            return 0;

        default:
            cout << "Invalid option!\n";
        }
    }

    return 0;
}
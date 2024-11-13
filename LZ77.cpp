#include <iostream>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

class LZ77 {
public:
    LZ77(int searchBufferSize) : searchBufferSize(searchBufferSize) {}

    vector<tuple<int, int, char>> compress(const string &input) {
        vector<tuple<int, int, char>> tokens;

        for (int i = 0; i < input.size();) {
            int matchLength = 0, matchOffset = 0;
            
            for (int j = max(0, i - searchBufferSize); j < i; ++j) {
                int k = 0;
                while (i + k < input.size() && input[j + k] == input[i + k]) ++k;
                if (k > matchLength) {
                    matchLength = k;
                    matchOffset = i - j;
                }
            }

            char nextChar = (i + matchLength < input.size()) ? input[i + matchLength] : '\0';
            tokens.emplace_back(matchOffset, matchLength, nextChar);
            i += matchLength + 1;
        }

        return tokens;
    }

    string decompress(const vector<tuple<int, int, char>> &tokens) {
        string output;

        for (const auto &token : tokens) {
            int offset = get<0>(token);
            int length = get<1>(token);
            char nextChar = get<2>(token);

            int start = output.size() - offset;
            for (int i = 0; i < length; ++i) output += output[start + i];
            if (nextChar != '\0') output += nextChar;
        }

        return output;
    }

private:
    int searchBufferSize;
};

int main() {
    string input = "abracadabra";
    int searchBufferSize = 6;
    LZ77 lz77(searchBufferSize);

    auto tokens = lz77.compress(input);

    cout << "Compressed Tokens:\n";
    for (const auto &token : tokens) {
        cout << "{" << get<0>(token) << ", " << get<1>(token) << ", '" << get<2>(token) << "'}\n";
    }

    cout << "\nDecompressed String: " << lz77.decompress(tokens) << endl;
    return 0;
}

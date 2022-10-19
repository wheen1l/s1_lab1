#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

bool is_letter(char c) {
    if (c != ' ' and c != '\n' and c != EOF) {
        return true;
    }
    else
    {
        return false;
    }
}

struct file_stats {
    string root_path;
    string filename;
    string filepath;
    int lines;
    int bytes;
    int words;
    int chars;
    const int char_size = 1;

    file_stats(string root_path, string filename) {
        this->root_path = root_path;
        this->filename = filename;
        filepath = root_path + filename;
    }

    int read() {
        ifstream in;
        in.open(filepath);
        if (in.is_open()) {
            lines = 0;
            bytes = 0;
            words = 0;
            chars = 0;

            for (char c = ' ', previous_c = ' '; c != EOF; previous_c = c) {
                c = in.get();
                chars++;
                if (c == '\n') {
                    lines++;
                }
                if (is_letter(previous_c) == true and is_letter(c) == false) {
                    words++;
                }
            }
            lines++;
            bytes = chars * sizeof(char);
        }
        else {
            cerr << "opening file error, file: " << filename << endl;
            return 1;
        }
        in.close();
        return 0;
    }

    void print_stats(bool lines_option, bool bytes_option, bool words_option, bool chars_option) {
        printf("%s:\n\t", filename.c_str());
        if (lines_option) printf("lines: %d\t", lines);
        if (bytes_option) printf("bytes: %d\t", bytes);
        if (words_option) printf("words: %d\t", words);
        if (chars_option) printf("chars: %d", chars);
        printf(";\n");
    }
};

//void file_reader();

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    po::options_description desc("program options");
    desc.add_options()
        ("lines,l", "print only lines counter")
        ("bytes,c", "")
        ("words,w", "")
        ("chars,m", "")
        ("help,h", "print help message")
        ;
    po::variables_map vm;

    po::store(po::parse_command_line(argc, argv, desc), vm);

    po::notify(vm);

    bool lines_option = false;
    bool bytes_option = false;
    bool words_option = false;
    bool chars_option = false;

    if (vm.count("help")) {
        cout << desc;
        return 1;
    }

    if (vm.count("lines")) {
        lines_option = true;
    }

    if (vm.count("bytes")) {
        bytes_option = true;
    }
    if (vm.count("words")) {
        words_option = true;
    }

    if (vm.count("chars")) {
        chars_option = true;
    }

    if ((lines_option or bytes_option or words_option or chars_option) == 0) {
        lines_option = true;
        bytes_option = true;
        words_option = true;
        chars_option = true;
    }

    vector<file_stats> files;
    vector<string> filenames;
    string root_folder = "..\\txt_files\\";

    for (int i = 1; i < argc; i++) {                            // i = 1;
        string filename = argv[i];

        if (!filename.empty() and filename[0] != '-') {
            filenames.push_back(argv[i]);
        }
    }

    for (auto filename : filenames) {
        files.push_back(file_stats(root_folder, filename));
    }

    for (auto file : files) {
        if(!file.read())
            file.print_stats(lines_option, bytes_option, words_option, chars_option);
        }

    return 0;
}

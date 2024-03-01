#include <iostream>
#include <fstream>
#include <ppmd_coder.h>

int main(int argc, const char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << "<d/e> <input> <output>" << std::endl;
        return 1;
    }

    char mode;
    if (strlen(argv[1]) != 1 || (mode = tolower(argv[1][0]), mode != 'd' && mode != 'e')) {
        std::cerr << "Invalid mode: " << argv[1] << std::endl;
    }

    std::ifstream in(argv[2], std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open input file: " << argv[2] << std::endl;
        return 1;
    }

    std::ofstream out(argv[3], std::ios::binary);
    if (!out) {
        std::cerr << "Cannot open output file: " << argv[3] << std::endl;
        return 1;
    }

    PPMD_Coder ppm;
    if (mode == 'e') {
        in.seekg(0, std::ios::end);
        unsigned short decompressed_size = (unsigned short)in.tellg();
        in.seekg(0, std::ios::beg);
        out.seekp(sizeof(unsigned short), std::ios::beg);
        if (!ppm.Compress(in, out)) {
            std::cerr << "Compression failed" << std::endl;
            return 1;
        }
        out.seekp(0, std::ios::beg);
        out.write((char*)&decompressed_size, sizeof(decompressed_size));
    } else {
        unsigned short decompressed_size = 0;
        in.read((char*)&decompressed_size, sizeof(decompressed_size));
        if (!ppm.Uncompress(in, out, (std::size_t)decompressed_size)) {
            std::cerr << "Decompression failed" << std::endl;
            return 1;
        }
    }

    return 0;
}

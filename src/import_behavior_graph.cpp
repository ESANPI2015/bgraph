#include "BehaviorGraph.hpp"
#include "HypergraphYAML.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <getopt.h>

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {0,0,0,0}
};

void usage (const char *myName)
{
    std::cout << "Usage:\n";
    std::cout << myName << " <yaml-file-in> <yaml-file-out>\n\n";
    std::cout << "Options:\n";
    std::cout << "--help\t" << "Show usage\n";
    std::cout << "\nExample:\n";
    std::cout << myName << " bg.yml imported_bg.yml\n";
}

int main (int argc, char **argv)
{

    // Parse command line
    int c;
    while (1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, "h", long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
            case 'h':
            case '?':
                break;
            default:
                std::cout << "W00t?!\n";
                return 1;
        }
    }

    if ((argc - optind) < 2)
    {
        usage(argv[0]);
        return 1;
    }

    // Set vars
    std::string fileNameIn(argv[optind]);
    std::string fileNameOut(argv[optind+1]);

    // Load file
    std::ifstream fin;
    fin.open(fileNameIn);
    if(!fin.good()) {
        std::cout << "READ FAILED\n";
        return 2;
    }
    std::stringstream ss;
    ss << fin.rdbuf();

    // Create domain specific model
    Behavior::Graph bg;
    bg.domainSpecificImport(ss.str());

    // Store modified graph
    std::ofstream fout;
    fout.open(fileNameOut);
    if(!fout.good()) {
        std::cout << "FAILED\n";
        return 3;
    }
    fout << YAML::StringFrom(bg) << std::endl;
    fout.close();
    fin.close();

    return 0;
}

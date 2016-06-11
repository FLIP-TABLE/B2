#include <sys/resource.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>

#include "sys/types.h"
#include "sys/sysinfo.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

const std::string random_name = "random_integers";
const std::string sorted_name = "sorted_integers";
const int memsize = 10000;

void generate(int N)
{
    std::ofstream outputfile(random_name);
    for(int i=0; i<N; ++i)
    {
        outputfile << rand();
        outputfile << "\n";
    }
    outputfile.close();
}

void external_sort()
{
    int N = memsize/sizeof(int);
    int *avail_memory = new int[N];

    int i = 0;
    int k = 0;
    std::ifstream infile(random_name);

    int x;
    for(infile >> x; !infile.eof(); infile >> x)
    {
        avail_memory[i] = x;
        ++i;
        if(i==N)
        {
            std::sort(avail_memory, avail_memory+N);
            std::ofstream savefile("/tmp/temp-" + std::to_string(k));
            for(i=0; i<N; ++i)
            {
                savefile << avail_memory[i] << std::endl;
            }
            savefile.close();
            i = 0;
            ++k;
        }
    }
    infile.close();

    std::ifstream *savefiles = new std::ifstream[k];
    for(i=0; i<k; ++i)
    {
        savefiles[i] = std::ifstream();
        savefiles[i].rdbuf() -> pubsetbuf(0,0);
        savefiles[i].open("/tmp/temp-" + std::to_string(i));
    }
}

int main()
{
    std::cout << sizeof(int) << std::endl;
    std::cout << "Generating integers" << std::endl;
    generate(100000);

    std::cout << "Limiting memory" << std::endl;
    struct rlimit limit = { memsize, memsize };
    setrlimit(RLIMIT_MEMLOCK, &limit);
    setrlimit(RLIMIT_AS, &limit);
    std::cout << "Sorting integers" << std::endl;
    external_sort();
}

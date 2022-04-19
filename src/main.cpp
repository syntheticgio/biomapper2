//
// Created by Joro on 4/19/22.
//

#include "BioMapper.h"

int main(int argv, char * argc[]) {
    // Test function

    BioMapper bm = BioMapper(4);
    bm.addFile("file1.csv", 0, 1, 2);
    bm.addFile("file2.csv", 0, 1, 2);
    bm.addFile("file3.csv", 0, 1, 2);
    bm.addFile("file4.csv", 0, 1, 2);

    bm.map();


}
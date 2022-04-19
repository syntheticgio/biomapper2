#include "BioMapper.h"

#include <iostream>

/*****************************************************************************************
 * BioMapper
 *      Constructor
 ****************************************************************************************/

BioMapper::BioMapper (int threadsToUse /* =-1 */, int readingThreads /* =-1 */ ) : threadsToUse_(threadsToUse), readingThreads_(readingThreads), maximumThreads_(std::thread::hardware_concurrency()) {
    // Generate the thread pool
    // If threads to use is defined, use that; otherwise use maximum the
    // hardware supports.
    if (threadsToUse_ == -1) {
        // This is undefined, use the maximum.
        threadsToUse_ = static_cast<int>(maximumThreads_);
    }

    // Set up the reading thread count
    if (readingThreads_ == -1 || readingThreads_ > maximumThreads_) {
        // Unspecified
        // Set to one
        readingThreads_ = 1;
    } else if (readingThreads_ == maximumThreads_) {
        // Hmmm so read everything in first before mapping?
        // Don't do anything, but flagging as potential issue
    }

    // Set up the mapping thread count
    mappingThreads_ = threadsToUse_ - readingThreads_;
}

bool BioMapper::map() {
    /*
     * Pre-processing Checks
     */
    std::vector <std::string> fail_list;
    if (!_verifyFiles(fail_list)) {
        // Files failed;
        std::cerr << "Failure in opening one or more files:  \n";
        for (auto &fail_file : fail_list) {
            std::cerr << "\t" << fail_file << "\n";
        }
        return false;
    }

    /*
     * Read in all headers if they exist
     */
    if (!_parseHeaders()) {
        std::cerr << "Failure in parsing one or more files' headers.  \n";
        return false;
    }



    /*
     * CREATE THE THREAD POOL
     * This will use the defined number of threads based
     * on the hardware or user specified.
     */
//    thread_pool pool(threadsToUse_);

    return true;
}

bool BioMapper::addFile(const char * file_path, int join_index, long long int start_range_index, long long int end_range_index,
             bool zero_based_range, bool has_header, char delimiter) {
    MapperFile file = MapperFile(file_path, join_index, start_range_index, end_range_index, zero_based_range, has_header, delimiter);
    return addFile(file);
}



/******************************************************************
 * Verify Files
 *      Make sure all files are able to be opened and read.
 ******************************************************************/
bool BioMapper::_verifyFiles(std::vector <std::string> &fail_list) {
    // Verify that all the files are openable.
    // Can just do serially
    bool passed = true;
    for (auto &file : files_) {
        std::ifstream fs;
        fs.open(file.file_path(), std::ifstream::in);
        if (fs.fail() || fs.peek() == std::ifstream::traits_type::eof()) {
            // Failure state here; file could not be opened
            // or the file is empty.
            fail_list.push_back(file.file_path());
            passed = false;
        } else {
            // No error state, so close the file
            fs.close();
        }
    }
    return passed;
}

/******************************************************************
 * Read in headers for each MapperFile object.
 *      Pre-populate the internal header structure to allow
 *      object to be const later
 ******************************************************************/
bool BioMapper::_parseHeaders() {
    // TODO: Need to fix the iterator here; need to add begin to class
    for (auto &file : files_) {
        if (!file.has_header()) {
            // No header, continue
            continue;
        }

        std::ifstream annot;
        std::string row;

        // TODO: Error handling - add better error returns
        annot.open(file.file_path());

        // Remove first line and save for future use
        std::getline(annot, row);
        if (row.empty()) {
            std::cerr << "ERROR: No file size for " << file.file_path() << ".  Aborting." << std::endl << std::endl;
            return false;
        }

        std::stringstream _rowElements(row);
        std::string _element;
        int i = 0;
        while (std::getline(_rowElements, _element, file.delimiter())) {
            file.add_column_to_header(i, _element);
            i++;
        }
    }
    return true;
}

/******
 *
 * Pre-fetch all the references across all the files.
 */
bool BioMapper::_determineReferences() {
    // Determine all references across all files
    // This could be chromosome, segment, or sequence IDs
    for (const MapperFile & file : files_ ) {
        // Get a reference to the refID we want to update (so each file will have
        // a list of their own files
        std::map <std::string, bool> &_refIDs = referenceIDs_[file.file_path()];
        // Read in file
        std::ifstream fs;
        fs.open(file.file_path());

        // Read in all references as a dictionary
        std::string row;

        if (file.has_header()) {
            // remove first line and save for future use
            std::getline(fs, row);

            if (row.empty()) {
                std::cerr << "ERROR: No file size for " << file.file_path() << ".  Aborting." << std::endl << std::endl;
                return false;
            }
            // Not recording header here, since this should be
            // done with a different function.
        }

        while ( std::getline(fs, row) ) {
            std::stringstream _rowElements(row);
            std::string _element;
            auto i = 1;

            while ( std::getline(_rowElements, _element, file.delimiter()) ) {
                if ( i == file.join_index() ) {
                    // TODO: This is re-setting over and over; but not sure if there is
                    //      a performance penalty tbh.
                    _refIDs[_element] = true;
                    // If already updated referenceIDs for this _element, ignore and move to next row
                    // Break out of loop and move to next row
                    break;
                }
                i++;
            }
        }
        // Add this file's reference IDs to the universal list.
        for (auto& refID : _refIDs) {
            auto it = allReferenceIDs_.find(refID.first);
            if ( it == allReferenceIDs_.end() ) {
                allReferenceIDs_[refID.first] = 1;
            } else {
                allReferenceIDs_[refID.first] += 1;
            }
        }
    }

    return true;
}
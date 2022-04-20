#ifndef BIOMAPPER2_BIOMAPPER_H
#define BIOMAPPER2_BIOMAPPER_H

#include <string>
#include <thread>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Annotation.h"
#include "FileList.h"
#include "MapperFile.h"
#include "MappingStream.h"
#include "thread_pool.hpp"

class BioMapper
{
public:
    /**
     *
     * @param threadsToUse
     * @param readingThreads
     */
    explicit BioMapper(int threadsToUse=-1, int readingThreads=-1);

    /**
     *
     */
    ~BioMapper() = default;

    /**
     *
     * @return
     */
    bool map();

    /**
     *
     * @param file
     * @return
     */
    bool addFile(MapperFile & file) {
        files_.push_back(file);
        return true;
    }

    bool addFile(const char * file_path, int join_index, long long int start_range_index, long long int end_range_index = -1,
                 bool zero_based_range = false, bool has_header = false, char delimiter = ',');

//private:
    /*************************************************************************************
     *  Private Functions to src the Mapper
     *************************************************************************************/

    /**
     *
     * @return
     */
    bool    _determineReferences();  // Determine the references - library call

    /**
     *
     * @param myMap
     */
//    void    _mapFiles (BioMapper& myMap);

    /**
     *
     */
//    void    _mapFilePool();

    /**
     *
     * @param mf
     * @return
     */
//    bool    _stream_file(const MapperFile & mf);

    /**
     * Verify that all files are open-able
     */
    bool    _verifyFiles(std::vector <std::string> & fail_list);

    /**
     *
     * @return
     */
    bool    _parseHeaders();

    /*************************************************************************************
     *  Member variables
     *************************************************************************************/
    FileList<MapperFile>        files_;              /**< The files to be mapped */
    std::map <std::string, std::map <std::string, bool> > referenceIDs_;       /**< Dictionary of the reference IDs by file */
    std::map <std::string, int> allReferenceIDs_;       /**< The reference IDs across all files, with file count */
    std::string outputFileName_;                     /**< The name for the output file for mapped results. */


    // Thread information
    const unsigned int          maximumThreads_;     /**< Maximum number of threads supported by the system */
    int                         threadsToUse_;       /**< Total number of threads to use */
    int                         readingThreads_;     /**< Total number of threads to use for reading files into the queues */
    int                         mappingThreads_{};     /**< Total number of threads to use for mapping annotations */
    std::vector <std::string>   threads_;            /**< vector of threads that are launched */
    std::mutex                  mtx;                 /**< Mutex to lock the BioMapper memory structures if needed */

    // Multithread streams
    std::map <std::string, AnnotationStream> annotationStreams_;
    std::map <std::string, Annotation *> bufferCurrentLocation_;
};

#endif //BIOMAPPER2_BIOMAPPER_H

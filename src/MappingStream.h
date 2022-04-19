//
// Created by Joro on 4/18/22.
//

#ifndef BIOMAPPER_MAPPINGSTREAM_H
#define BIOMAPPER_MAPPINGSTREAM_H

#include <string>

/**
 * MappingStream class which facilitates all of the annotations being streamed from the reading thread
 *
 */
class MappingStream {

public:
    std::string join_index; ///< The sequence ID or otherwise the join index information


    MappingStream() = default;
    ~MappingStream() = default;

private:

};

#endif //BIOMAPPER_MAPPINGSTREAM_H

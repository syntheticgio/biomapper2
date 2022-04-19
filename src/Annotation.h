/*! \file Annotation.h
    \author John Torcivia, Ph.D.

    \brief An abstraction for an annotation.

    Abstraction for an annotation.  This contains join and loci information
    as well as other annotation elements.
*/

#ifndef BIOMAPPER_ANNOTATION_H
#define BIOMAPPER_ANNOTATION_H

#include <string>
#include <utility>

/**
 * Typedef for a variant container that allows all expected annotation types.
 * This allows for a vector that contains different types.
 */
 // TODO: Might want to use a union instead?
typedef std::variant<uint8_t, uint16_t, uint32_t, uint64_t, float, double, std::string> AnnotationTypes;


/**
 *
 */
class Annotation {

public:
    /**
     *
     */
    Annotation() : start_range_(-1), end_range_(-1) {};

    /**
     *
     */
    ~Annotation() = default;

    /**
     *
     * @param start_range
     * @return
     */
    bool setStartRange(long long int start_range) {
        start_range_ = start_range;
        return true;
    }

    /**
     *
     * @param end_range
     * @return
     */
    bool setEndRange(long long int end_range) {
        end_range_ = end_range;
        return true;
    }

    /**
     *
     * @param join_index
     * @return
     */
    bool setJoinIndex(AnnotationTypes join_index) {
        // TODO: Make sure this is a copy and not a move!
        join_index_ = join_index;
        return true;
    }

    /**
     *
     * @param elements
     * @return
     */
    bool setElements(std::vector <AnnotationTypes> & elements) {
        elements_ = std::move(elements);
        return true;
    }

    /**
     *
     * @param element
     * @return
     */
    bool addElement(AnnotationTypes element) {
        elements_.push_back(std::move(element));
        return true;
    }

private:
    std::vector <AnnotationTypes>   elements_;     ///< Vector of elements of the annotation
    AnnotationTypes                 join_index_;   ///< The join value
    long long int                   start_range_;  ///< The start location
    long long int                   end_range_;    ///< The end location (-1 if it is a single position annotation)

};

/**
 *
 */
class AnnotationStream {
public:

    AnnotationStream(std::string join_id, uint32_t buffer_size=1000) : joinId_(std::move(join_id)), bufferSize_(buffer_size), circularBuffer_(NULL), location_(0) {
        // Create a circular buffer of annotations
        circularBuffer_ = new Annotation[bufferSize_];
    }

    ~AnnotationStream() {
        // Clean up the memory for the circular buffer
        delete circularBuffer_;
    };

    void addElement(Annotation & annot) {
        // TODO: What to do if we've matched the location where things are being read from?
        //      we dont' want to overwrite data that isn't written yet
        if (location_ >= bufferSize_) {
            // We've gone past the buffer, need to loop around.
            circularBuffer_ -= location_;
        } else {
            ++circularBuffer_;
            ++location_;
        }

        circularBuffer_ = std::move(annot);

    }

    std::string     joinId_; ///< The join ID / index that is used (i.e. the sequence ID)
    Annotation *    circularBuffer_;
    uint32_t        bufferSize_;
    uint32_t        location_;
};
#endif //BIOMAPPER_ANNOTATION_H

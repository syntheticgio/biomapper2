/*! \file FileList.h
    \author John Torcivia, Ph.D.

    \brief A file list class.

    This file list class is a class that maintains an internal position, allowing
    for files to be fetched one after another.
*/

#ifndef BIOMAPPER_FILELIST_H
#define BIOMAPPER_FILELIST_H


template <typename T>
class FileList {
public:
    FileList() {
        current_index = 0;
    }

    FileList(int start_size) {
        current_index = 0;
        file_list_.resize(start_size);
    }

    ~FileList() = default;

    void push_back(T file) {
        file_list_.push_back(file);
    }

    long int size() {
        return file_list_.size();
    }

    bool is_next() {
        if (current_index >= file_list_.size() - 1) {
            return false;
        }
        return true;
    }

    T & get_next() {
        // TODO: Need to do bounds checking of course
        current_index +=1;
        return file_list_[current_index - 1];
    }

    /********
     * Iterator info to allow to work in a range loop.
     */

    auto begin() { return file_list_.begin(); }
    auto end() { return file_list_.end(); }
    auto cbegin() const { return file_list_.begin(); }
    auto cend() const { return file_list_.end(); }
    auto begin() const { return file_list_.begin(); }
    auto end() const { return file_list_.end(); }

private:
    std::vector <T> file_list_;
    int current_index;

};


#endif //BIOMAPPER_FILELIST_H

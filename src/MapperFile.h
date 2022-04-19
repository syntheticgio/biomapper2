/*! \file MapperFile.h
    \author John Torcivia, Ph.D.
    \brief An abstraction for an annotation file.

    Abstraction for an annotation file that containes not only the file
    location but also other relevent information such as header and
    loci / join information.
*/

#ifndef BIOMAPPER_MAPPERFILE_H
#define BIOMAPPER_MAPPERFILE_H

/**
 *
 */
class MapperFile {
public:
    /**
     *
     * @param[in] file_path
     * @param[in] join_index
     * @param[in] start_range_index
     * @param[in] end_range_index
     * @param[in] zero_based_range
     * @param[in] has_header
     * @param[in] delimiter
     */
    MapperFile(const char * file_path, int join_index, long long int start_range_index, long long int end_range_index = -1,
               bool zero_based_range = false, bool has_header = true, char delimiter = ',')
            : file_path_(file_path), join_index_(join_index), start_range_index_(start_range_index), end_range_index_(end_range_index),
              zero_based_range_(zero_based_range), has_header_(has_header), delimiter_(delimiter) {}

    /**
     *
     */
    ~MapperFile() = default;

    MapperFile(const MapperFile &mf) {
        file_path_ = mf.file_path();
        join_index_ = mf.join_index();
        start_range_index_ = mf.start_range_index();
        end_range_index_ = mf.end_range_index();
        zero_based_range_ = mf.zero_based_range();
        has_header_ = mf.has_header();
        delimiter_ = mf.delimiter();
    }

    /*****************************************************************************
     *
     * Debug
     *
     *****************************************************************************/

    /**
     *
     */
    void print_info() {
        // TODO: Add in this debug.
    }

    /*****************************************************************************
     *
     * Internal
     *
     *****************************************************************************/


    /*****************************************************************************
     *
     * Getters
     *
     *****************************************************************************/

    /**
     *
     * @return Returns the full file path with the file name.
     *
     * @note A reference cannot be used here for the return value since an std::variant
     * is being populated with it.
     */
    [[nodiscard]] std::string file_path() const { return file_path_;}

    /**
     *
     * @return The index (zero based) of the column used for mapping (the join index).
     */
    [[nodiscard]] int32_t join_index() const { return join_index_;}

    /**
     *
     * @return The start range column index (zero based) within the file.
     */
    [[nodiscard]] int64_t start_range_index() const { return start_range_index_;}

    /**
     * @brief End range index
     *
     * Returns the end range index for the column in the data which holds the
     * final position for the range.  This can be set to -1 if there is no end
     * position (i.e., it is a single position annotation instead of a range).
     *
     * @return The end range column index (zero based) within the file.
     */
    [[nodiscard]] int64_t end_range_index() const { return end_range_index_;}

    /**
     * @brief If a header line exists.
     *
     * Returns whether or not the file has a header line in its structure.
     *
     * @retval true The file has a header.
     * @retval false The file does not have a header.
     */
    [[nodiscard]] bool has_header() const { return has_header_;}

    /**
     *
     * @return Whether or not the start and end range values are one or zero based.
     * @retval true The range values are zero (0) based.
     * @retval false The range values are one (1) based.
     */
    [[nodiscard]] bool zero_based_range() const { return zero_based_range_;}

    /**
     * @brief Returns the file delimiter.
     *
     * Returns the file delimiter assuming that the file is in a typical <char><val> structure.
     * Examples of this structure are comma-separated-value files (CSVs) and tab-separated-value
     * files (TSVs).
     *
     * @return The delimiter character that the file uses.
     * @retval , File is in CSV format.
     * @retval \t File is in TSV format.
     * @retval | File is in pipe (|) separated format.
     */
    [[nodiscard]] char delimiter() const { return delimiter_;}


    /*****************************************************************************
     *
     * Setters
     *
     *****************************************************************************/
    /**
     *
     * @param[in] join_index
     */
    void set_join_index(int32_t join_index) { join_index_ = join_index;}

    /**
     *
     * @param[in] start_range_index
     */
    void set_start_range_index(int64_t start_range_index) {  start_range_index_ = start_range_index;}

    /**
     *
     * @param[in] end_range_index
     */
    void set_end_range_index(int64_t end_range_index)  {end_range_index_ = end_range_index;}

    /**
     *
     * @param[in] zero_based_range
     */
    void set_zero_based(bool zero_based_range)  { zero_based_range_ = zero_based_range;}

    /**
     *
     * @param[in] delimiter
     */
    void set_delimiter(char delimiter)  { delimiter_ = delimiter;}

    /**
     *
     * @param[in] column_index The zero (0) based index of the column in the file.
     * @param[in] column_name The name of the column.
     */
    void add_column_to_header(uint32_t column_index, std::string column_name) {
        header_[column_index] = column_name;
    }

    /**
     * @brief Fully replace the header.
     *
     * This fully replaces the current header with a new one.  The new header is an ordered
     * list of each of the column names.  The order determines the column index for each of the
     * entries.
     *
     * @param[in] column_names Ordered list of column names.
     */
    void replace_header(std::vector<std::string> &column_names) {
        for (int i = 0; i < column_names.size(); i++) {
            header_[i] = std::move(column_names[i]);
        }
    }

    /**
     * Replace header - using a getline from the file internally.
     *
     * This assumes that the delimiter is valid.
     * @param columns
     */
    void internal_replace_header(std::string &columns) {
        std::vector <std::string> new_header;
        std::stringstream _rowElements(columns);
        std::string _element;

        while ( std::getline(_rowElements, _element, delimiter_) ) {
            new_header.push_back(_element);
        }

        replace_header(new_header);
    }




private:
    // Member Variables
    int32_t     join_index_;          ///<
    int64_t     start_range_index_;   ///<
    int64_t     end_range_index_;     ///<
    bool        zero_based_range_;          ///<
    bool        has_header_;          ///<
    char        delimiter_;           ///<
    std::string file_path_{};         ///<

    // Extrapolated variables
    std::map <uint32_t, std::string> header_{}; ///<
};


#endif //BIOMAPPER_MAPPERFILE_H

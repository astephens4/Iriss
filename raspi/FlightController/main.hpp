#ifndef IRISS_MAIN_H
#define IRISS_MAIN_H 1

#include <string>

namespace Iriss {

/**
 * Check to see if the named process is running
 * @param [in] process name
 * @return True if the given process name is running, false otherwise
 */
bool is_process_running(const std::string& execName);

/**
 * Check to see if the given file exists
 * @param [in] fileName File to check, can be relative or absolute
 */
bool is_file(const std::string& fileName);

/**
 * Get an image in the given directory whose time of creation is within the
 * given time window around when this function is called
 * @param [in] dir Directory in the file system where the images should be located
 * @param [in] validWindow Time in seconds, an image is accepted if its time of
 *                         creation is within validWindow seconds of when this is
 *                         called
 * @return The full file name of the found image. Empty if non are found
 */
std::string find_recent_image(const std::string& dir, float validWindow);
} // end namespace Iriss

#endif // IRISS_MAIN_H

#include <string>

#include "gst_wrapper.h"

#ifndef GST_FILESRC_H
#define GST_FILESRC_H
namespace gstwrapper
{
    class GstFilesrc : public GstElement
    {
    private:
        std::string file_path;

    public:
        GstFilesrc(std::string file_path_);

        void set_location(std::string file_path_);
    };
};
#endif

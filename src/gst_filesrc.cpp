
#include <gst/gst.h>

#include "gst_filesrc.h"

namespace gstwrapper
{
    GstFilesrc::GstFilesrc(std::string file_path_) : file_path(file_path_)
    {
        elem = gst_element_factory_make("filesrc", nullptr);
        set_property("location", file_path.c_str());
    }

    void GstFilesrc::set_location(std::string file_path_)
    {
        file_path = file_path_;
        set_property("location", file_path.c_str());
    }
};

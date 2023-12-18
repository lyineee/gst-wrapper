
#include <gst/gst.h>

#include "gst_wrapper/gst_filesink.h"

namespace gstwrapper
{
    GstFilesink::GstFilesink(std::string file_path_) : GstElement("filesink"), file_path(file_path_)
    {
        set_property("location", file_path.c_str());
    }

    void GstFilesink::set_location(std::string file_path_)
    {
        file_path = file_path_;
        set_property("location", file_path.c_str());
    }
};

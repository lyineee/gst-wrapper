#include "gst_wrapper.h"

#ifndef GST_FILESINK_H
#define GST_FILESINK_H
namespace gstwrapper
{
    class GstFilesink : public GstElement
    {
    private:
        std::string file_path;

    public:
        using SharedPtr = std::shared_ptr<GstFilesink>;
        GstFilesink(std::string file_path_);
        void set_location(std::string file_path_);
    };
};
#endif

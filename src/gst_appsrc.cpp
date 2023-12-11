#include "gst_wrapper.h"

#include <gst/gst.h>
#include <gst/gstobject.h>
#include <gst/app/gstappsrc.h>
#include <functional>
#include <gst/gstbuffer.h>

#include <thread>
#include <chrono>
#include "gst_appsrc.h"
namespace gstwrapper
{
    GstAppsrc::GstAppsrc() : GstElement("appsrc") {}

    void GstAppsrc::push_buffer(const char * data, uint64_t size){
        auto buffer = gst_buffer_new_and_alloc(size);
        ::GstMapInfo info;
        gst_buffer_map(buffer, &info, GST_MAP_WRITE);
        std::copy(data, data + size, info.data);

        gst_app_src_push_buffer(GST_APP_SRC(elem), buffer);
    }
};

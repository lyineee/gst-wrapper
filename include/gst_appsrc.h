#include <functional>

#include "gst_wrapper.h"
#include "gst_sample_data.h"

#ifndef GST_APPSRC_H
#define GST_APPSRC_H
namespace gstwrapper
{
    class GstAppsrc : public GstElement
    {
        using SampleCallback = std::function<void(SampleData)>;
        SampleCallback sample_callback;

    public:
        using SharedPtr = std::shared_ptr<gstwrapper::GstAppsrc>;
        GstAppsrc();
        void push_buffer(const char * data, uint64_t size);
    };
};
#endif

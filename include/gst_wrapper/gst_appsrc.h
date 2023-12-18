#include <functional>

#include "gst_wrapper/gst_wrapper.h"
#include "gst_wrapper/gst_sample_data.h"

#ifndef GST_APPSRC_H
#define GST_APPSRC_H
namespace gstwrapper
{
    class GstAppsrc : public GstElement
    {
        using SampleCallback = std::function<void(SampleData)>;
        SampleCallback sample_callback;
        static  void release(void *data);

    public:
        int a = 30;
        using SharedPtr = std::shared_ptr<gstwrapper::GstAppsrc>;
        GstAppsrc();
        void push_buffer(const char *data, uint64_t size);
        void push_wrap_buffer(const char *data, uint64_t size, std::function<void(char *)> deleter);
        void push_wrap_buffer(const char *data, uint64_t size, void (*deleter)(void *));
    };
};
#endif

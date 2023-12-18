#include <functional>

#include "gst_wrapper/gst_wrapper.h"
#include "gst_wrapper/gst_sample_data.h"

#ifndef GST_APPSINK_H
#define GST_APPSINK_H
namespace gstwrapper
{
    class GstAppsink : public GstElement
    {
        using SampleCallback = std::function<void(SampleData)>;
        SampleCallback sample_callback;

    public:
        using SharedPtr = std::shared_ptr<gstwrapper::GstAppsink>;
        GstAppsink();
        void set_sample_handle(SampleCallback f);
        static int gst_new_sample_callback(void *object, void *user_data);
        SampleData get_new_sample(uint32_t timeout = 0);
    };
};
#endif

#include "gst_wrapper/gst_wrapper.h"

#include <gst/gst.h>
#include <gst/gstobject.h>
#include <gst/app/gstappsink.h>
#include <functional>
#include <gst/gstsample.h>
#include <gst/gstbufferlist.h>

#include <thread>
#include <chrono>
#include "gst_wrapper/gst_appsink.h"
namespace gstwrapper
{

    GstAppsink::GstAppsink() : GstElement("appsink") {}
    void GstAppsink::set_sample_handle(SampleCallback f)
    {
        sample_callback = f;
        set_property("emit-signals", true);
        g_signal_connect(GST_APP_SINK(elem), "new-sample", G_CALLBACK(gst_new_sample_callback), this);
    }

    int GstAppsink::gst_new_sample_callback(void *object, void *user_data)
    {
        auto sink = reinterpret_cast<GstAppsink *>(user_data);
        auto sample = gst_app_sink_pull_sample(GST_APP_SINK(sink->elem));
        sink->sample_callback(SampleData(sample));
        return GST_FLOW_OK;
    }
    SampleData GstAppsink::get_new_sample(uint32_t timeout)
    {
        GstSample *sample;
        if (timeout != 0)
        {
            sample = gst_app_sink_try_pull_sample(GST_APP_SINK(elem), timeout);
        }
        else
        {
            sample = gst_app_sink_pull_sample(GST_APP_SINK(elem));
        }
        return SampleData(sample);
    }
};

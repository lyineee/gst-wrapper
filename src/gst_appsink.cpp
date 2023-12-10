#include "gst_wrapper.h"

#include <gst/gst.h>
#include <gst/gstobject.h>
#include <gst/app/gstappsink.h>
#include <functional>

#include <thread>
#include <chrono>
#include "gst_appsink.h"
namespace gstwrapper
{

    bool SampleData::try_parse_info()
    {
        if (!is_parse)
        {
            is_parse = true;
            buffer = gst_sample_get_buffer(GST_SAMPLE(sample));
            if (buffer == nullptr)
            {
                std::cout << "buffer null\n";
                is_parse = false;
                return false;
            }
            info = new GstMapInfo;
            auto i = reinterpret_cast<GstMapInfo *>(info);
            if (false == gst_buffer_map(GST_BUFFER(buffer), i, GST_MAP_READ))
            {
                std::cout << "map buffer failed\n";
            }
            sample_data.data = reinterpret_cast<const uint8_t *>(i->data);
            sample_data.length = i->size;
        }
        return true;
    }

    SampleData::SampleData(void *sam) : sample(sam){};
    SampleData::SampleData(SampleData &&rhs)
    {
        sample = rhs.sample;
        info = rhs.info;
        buffer = rhs.buffer;
        is_parse = rhs.is_parse;

        rhs.sample = nullptr;
        rhs.info = nullptr;
        rhs.buffer = nullptr;
    };
    SampleData::~SampleData()
    {
        if (buffer != nullptr)
            gst_buffer_unmap(GST_BUFFER(buffer), reinterpret_cast<GstMapInfo *>(info));
        if (sample != nullptr)
            gst_sample_unref(GST_SAMPLE(sample));
    }

    SampleData &SampleData::operator=(SampleData &&rhs)
    {
        sample = rhs.sample;
        info = rhs.info;
        buffer = rhs.buffer;
        is_parse = rhs.is_parse;

        rhs.sample = nullptr;
        rhs.info = nullptr;
        rhs.buffer = nullptr;
        return *this;
    };

    const uint8_t *SampleData::data()
    {
        return this->operator->()->data;
    }
    uint64_t SampleData::length()
    {
        return this->operator->()->length;
    }

    GstAppsink::GstAppsink()
    {
        elem_name = "appsink";
        elem = gst_element_factory_make("appsink", nullptr);
    }
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

#include "gst_wrapper.h"

#include <functional>

#ifndef GST_APPSINK_H
#define GST_APPSINK_H
namespace gstwrapper
{

    class SampleData
    {
        void *sample = nullptr;
        void *info = nullptr;
        void *buffer = nullptr;
        bool is_parse = false;
        struct SampleDataInternal
        {
            const uint8_t *data;
            uint64_t length;
        };
        SampleDataInternal sample_data;

    private:
        bool try_parse_info();

    public:
        SampleData(void *sam);
        SampleData(SampleData &) = delete;
        SampleData(SampleData &&rhs);
        ~SampleData();

        SampleData &operator=(SampleData &&rhs);
        SampleDataInternal *operator->()
        {
            if (!try_parse_info())
            {
                sample_data.length = 0;
            }
            return &sample_data;
        }

        const uint8_t *data();
        uint64_t length();
    };

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

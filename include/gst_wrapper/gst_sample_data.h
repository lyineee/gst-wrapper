#include "gst_wrapper/gst_wrapper.h"

#include <functional>

#ifndef GST_SAMPLE_DATA_H
#define GST_SAMPLE_DATA_H
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
};
#endif

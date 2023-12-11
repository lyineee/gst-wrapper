#include "gst_sample_data.h"

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
}
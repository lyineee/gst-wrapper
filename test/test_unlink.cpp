#include <chrono>
#include <thread>

#include "gst_wrapper.h"
#include "gst_appsink.h"
#include "gst_filesrc.h"
#include "gst_filesink.h"
#include "gst_appsrc.h"

using namespace std::chrono_literals;
int main(int argc, char *argv[])
{
    auto test_src = std::make_shared<gstwrapper::GstElement>("videotestsrc");
    auto sink = std::make_shared<gstwrapper::GstElement>("autovideosink");
    auto appsrc = std::make_shared<gstwrapper::GstAppsrc>();
    auto appsink = std::make_shared<gstwrapper::GstAppsink>();

    auto filesrc = std::make_shared<gstwrapper::GstFilesrc>("a.h264");
    auto cuda_down = std::make_shared<gstwrapper::GstElement>("cudadownload");
    auto dec = std::make_shared<gstwrapper::GstElement>("nvh264dec");
    auto conv = std::make_shared<gstwrapper::GstElement>("videoconvert");
    auto parse = std::make_shared<gstwrapper::GstElement>("h264parse");

    auto tee = std::make_shared<gstwrapper::GstElement>("tee");
    auto queue1 = std::make_shared<gstwrapper::GstElement>("queue");
    auto queue2 = std::make_shared<gstwrapper::GstElement>("queue");
    auto multisink = std::make_shared<gstwrapper::GstElement>("multifilesink");
    auto jpegenc = std::make_shared<gstwrapper::GstElement>("jpegenc");

    gstwrapper::GstPipeline pipe_read;
    gstwrapper::GstPipeline pipe;
    pipe_read.add_elem({
        filesrc,
        appsink,
    });

    pipe.add_elem({
        appsrc,
        parse,
        dec,
        cuda_down,
        conv,
        sink,
        tee,
        queue1,
        // queue2,
        // multisink,
        // jpegenc,
    });

    multisink->set_property("location", "out/%04d.jpg");
    appsink->set_sample_handle([&](gstwrapper::SampleData sample)
                               { appsrc->push_buffer(reinterpret_cast<const char *>(sample->data), sample->length); });
    filesrc->link(appsink);
    appsrc->link(parse)->link(dec)->link(cuda_down)->link(conv)->link(tee)->link(queue1)->link(sink);
    // tee->link(queue2)->link(jpegenc)->link(multisink);

    pipe.set_state(gstwrapper::ELEM_PLAYING);
    pipe_read.set_state(gstwrapper::ELEM_PLAYING);

    while (true)
    {
        std::this_thread::sleep_for(1s);
        pipe.set_state(gstwrapper::ELEM_PAUSED);
        parse->unlink(dec);
        std::this_thread::sleep_for(1s);
        parse->link(dec);
        pipe.set_state(gstwrapper::ELEM_PLAYING);
    }

    return 0;
}

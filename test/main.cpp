#include <chrono>
#include <thread>

#include "gst_wrapper.h"
#include "gst_appsink.h"
#include "gst_filesrc.h"
#include "gst_filesink.h"

using namespace std::chrono_literals;
int main(int argc, char *argv[])
{
    /* code */
    // gst_init(&argc, &argv);

    auto test_src = std::make_shared<gstwrapper::GstElement>("videotestsrc");
    auto sink = std::make_shared<gstwrapper::GstElement>("autovideosink");
    auto cuda_up = std::make_shared<gstwrapper::GstElement>("cudaupload");
    auto cuda_down = std::make_shared<gstwrapper::GstElement>("cudadownload");
    auto enc = std::make_shared<gstwrapper::GstElement>("nvh264enc");
    auto dec = std::make_shared<gstwrapper::GstElement>("nvh264dec");
    auto conv = std::make_shared<gstwrapper::GstElement>("videoconvert");
    auto appsink = std::make_shared<gstwrapper::GstAppsink>();
    auto parse = std::make_shared<gstwrapper::GstElement>("h264parse");
    auto filesrc = std::make_shared<gstwrapper::GstFilesrc>("b.h264");
    auto filesink = std::make_shared<gstwrapper::GstFilesink>("b.h264");
    gstwrapper::GstPipeline pipe;
    gstwrapper::GstPipeline pipe_enc;
    test_src->set_property("pattern", 0);
    pipe_enc.add_elem({
        test_src,
        cuda_up,
        enc,
        filesink,
    });
    pipe.add_elem({
        filesrc,
        parse,
        cuda_down,
        dec,
        conv,
        appsink,
        // sink,
    });
    filesrc->link(parse)
        ->link(dec)
        ->link(cuda_down)
        ->link(conv)
        ->link(appsink);
    appsink->set_sample_handle([](gstwrapper::SampleData d)
                               { std::cout << "length " << d->length << "\n"; });
    test_src->link(cuda_up)
        ->link(enc)
        ->link(filesink);

    pipe.set_state(gstwrapper::ELEM_PLAYING);
    // pipe_enc.set_state(gstwrapper::ELEM_PLAYING);

    int i = 0;
    while (true)
    {
        ++i;
        // test_src->set_property("pattern", i % 25);
        std::this_thread::sleep_for(1s);
        // auto t = appsink->get_new_sample();
        // std::cout << "get data length=" << t.length() << "\n";
    }
    return 0;
}

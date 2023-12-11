#include <chrono>
#include <thread>

#include "gst_wrapper.h"
#include "gst_appsink.h"
#include "gst_filesrc.h"
#include "gst_filesink.h"

using namespace std::chrono_literals;
int main(int argc, char *argv[])
{
    auto test_src = std::make_shared<gstwrapper::GstElement>("videotestsrc");
    auto tee = std::make_shared<gstwrapper::GstElement>("tee");
    auto sink = std::make_shared<gstwrapper::GstElement>("autovideosink");
    auto sink1 = std::make_shared<gstwrapper::GstElement>("autovideosink");
    auto queue = std::make_shared<gstwrapper::GstElement>("queue");
    auto queue1 = std::make_shared<gstwrapper::GstElement>("queue");

    gstwrapper::GstPipeline pipe;
    pipe.add_elem({
        test_src,
        tee,
        queue,
        queue1,
        sink1,
        sink,
    });
    test_src->link(tee)->link(queue)->link(sink);
    tee->link(queue1)->link(sink1);

    pipe.set_state(gstwrapper::ELEM_PLAYING);

    while (true)
    {
        std::this_thread::sleep_for(1s);
    }
    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#ifndef GST_WRAPPER_H
#define GST_WRAPPER_H
namespace gstwrapper
{
    enum ElementState
    {
        ELEM_VOID_PENDING,
        ELEM_NULL,
        ELEM_READY,
        ELEM_PAUSED,
        ELEM_PLAYING,
    };

    class GstBus
    {
    };

    class GstElement
    {
    public:
        using SharedPtr = std::shared_ptr<GstElement>;

    private:
        void init_gst();

    protected:
        void *elem;
        std::string elem_name;
        int cvt_state(ElementState state);
        void link(void *src, void *target, std::string filter);

    public:
        GstElement();
        GstElement(std::string element_name);
        ~GstElement();

        void set_state(ElementState state);
        void set_property(std::string prop, std::string value);
        void set_property(std::string prop, int value);
        template <typename T>
        T &link(T &element, const std::string filter = "")
        {
            link(this->get_element(), element->get_element(), filter);
            return element;
        }

        void *get_element();
    };
    class GstPipeline : public GstElement
    {
    private:
        void *current_elem;
        std::vector<std::shared_ptr<GstElement>> bin_elems;

    public:
        GstPipeline();
        GstPipeline(std::string pipeline_str);
        void add_elem(std::vector<GstElement::SharedPtr> elems);
        ;
        ~GstPipeline();
    };

}
#endif
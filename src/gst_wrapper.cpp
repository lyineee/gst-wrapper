#include <iostream>
#include <gst/gst.h>
#include <gst/gstobject.h>
#include <string>
#include <vector>
#include <memory>
#include "gst_wrapper.h"

namespace gstwrapper
{
    int GstElement::cvt_state(ElementState state)
    {
        switch (state)
        {
        case ELEM_NULL:
            return GST_STATE_NULL;
            break;
        case ELEM_PLAYING:
            return GST_STATE_PLAYING;
            break;
        case ELEM_PAUSED:
            return GST_STATE_PAUSED;
            break;
        case ELEM_READY:
            return GST_STATE_READY;
            break;
        case ELEM_VOID_PENDING:
            return GST_STATE_VOID_PENDING;
            break;
        default:
            return GST_STATE_NULL;
            break;
        }
    }

    void GstElement::init_gst()
    {
        if (!gst_is_initialized())
        {
            gst_init(0, nullptr);
        }
    }
    void GstElement::link(void *src, void *target, std::string filter)
    {
        auto source_ptr = reinterpret_cast<::GstElement *>(src);
        auto elem_ptr = reinterpret_cast<::GstElement *>(target);
        if (filter == "")
        {
            if (gst_element_link(source_ptr, elem_ptr) != true)
            {
                std::cout << "element can not be link\n";
            }
        }
        else
        {
            auto caps = gst_caps_from_string(filter.c_str());
            if (gst_element_link_filtered(source_ptr, elem_ptr, caps) != true)
            {
                std::cout << "element can not be link\n";
            }
        }
    }

    GstElement::GstElement() { init_gst(); };
    GstElement::GstElement(std::string element_name) : elem_name(element_name)
    {
        init_gst();
        elem = gst_element_factory_make(element_name.c_str(), nullptr);
    }
    GstElement::~GstElement()
    {
        if (elem != nullptr)
        {
            gst_object_unref(elem);
        }
    }

    void GstElement::set_state(ElementState state)
    {
        auto ret = gst_element_set_state(reinterpret_cast<::GstElement *>(elem), static_cast<::GstState>(cvt_state(state)));
        if (ret == GST_STATE_CHANGE_FAILURE)
        {
            std::cout << "element: " << elem_name << " failed to change state to " << state << "\n";
        }
    }

    void GstElement::set_property(std::string prop, std::string value)
    {
        g_object_set(elem, prop.c_str(), value.c_str(), NULL);
    }
    void GstElement::set_property(std::string prop, int value)
    {
        g_object_set(elem, prop.c_str(), value, NULL);
    }

    void *GstElement::get_element()
    {
        return elem;
    }

    ::GstElement *current_elem;
    std::vector<std::shared_ptr<GstElement>> bin_elems;

    GstPipeline::GstPipeline()
    {
        elem = gst_pipeline_new(nullptr);
    }
    GstPipeline::GstPipeline(std::string pipeline_str)
    {
        elem = gst_parse_launch(pipeline_str.c_str(), nullptr);
    }
    void GstPipeline::add_elem(std::vector<GstElement::SharedPtr> elems)
    {
        for (auto &elem_ptr : elems)
        {
            auto raw_ptr = reinterpret_cast<::GstElement *>(elem_ptr->get_element());
            gst_bin_add(GST_BIN(elem), raw_ptr);
        }
    }
    // GstPipeline &link(GstElement::SharedPtr gst_elem)
    // {
    //     auto elem_ptr = reinterpret_cast<::GstElement *>(gst_elem->get_element());
    //     gst_bin_add(GST_BIN(elem), elem_ptr);
    //     bin_elems.emplace_back(gst_elem);
    //     if (bin_elems.size() > 1)
    //     {
    //         if (gst_element_link(current_elem, elem_ptr) != true)
    //         {
    //             std::cout << "element can not be link\n";
    //         }
    //     }
    //     else
    //     {
    //         current_elem = elem_ptr;
    //     }
    //     return *this;
    // }
    GstPipeline::~GstPipeline()
    {
        bin_elems.clear();
    }

}
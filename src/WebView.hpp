#pragma once

#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>

class WebView
    : public Gtk::Widget
{
    public:
        WebView();
        ~WebView() override;

    public:
        operator WebKitWebView*();

    public:
        void        refresh();
        void        zoomIn();
        void        zoomOut();
        double      getZoomLevel() const noexcept;
        std::string getZoomLevelString() const noexcept;

    public:
        sigc::signal<void, bool> signalNotification() const noexcept;

    private:
        double                   m_zoomLevel;
        sigc::signal<void, bool> m_signalNotification;
};

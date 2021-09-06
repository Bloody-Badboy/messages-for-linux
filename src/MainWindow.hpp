#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/label.h>
#include <gtkmm/modelbutton.h>
#include "TrayIcon.hpp"
#include "WebView.hpp"

class MainWindow
    : public Gtk::ApplicationWindow
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);

    protected:
        bool on_key_press_event(GdkEventKey* keyEvent) override;
        bool on_window_state_event(GdkEventWindowState* windowStateEvent) override;
        bool on_delete_event(GdkEventAny* any_event) override;

    private:
        void onRefresh();
        void onShow();
        void onQuit();
        void onCloseToTray(Gtk::ModelButton* closeToTrayButton, Gtk::ModelButton* startInTrayButton);
        void onStartInTray(Gtk::ModelButton* startInTrayButton);
        void onAutostart(Gtk::ModelButton* autostartButton);
        void onFullscreen();
        void onZoomIn(Gtk::Label* zoomLevelLabel);
        void onZoomOut(Gtk::Label* zoomLevelLabel);
        void onShortcuts();
        void onAbout();

    private:
        TrayIcon              m_trayIcon;
        WebView               m_webView;
        Gtk::HeaderBar*       m_headerBar;
        Gtk::ShortcutsWindow* m_shortcutsWindow;
        bool                  m_fullscreen;
};

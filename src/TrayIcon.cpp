#include "TrayIcon.hpp"
#include <utility>
#include <gtkmm/icontheme.h>

namespace
{
    std::pair<char const*, char const*> getTrayIconNames()
    {
        constexpr auto const MESSAGES_TRAY                     = "google-messages-tray";
        constexpr auto const MESSAGES_TRAY_ATTENTION           = "google-messages-tray-attention";
        constexpr auto const MESSAGES_FOR_LINUX_TRAY           = "google-messages-for-linux-tray";
        constexpr auto const MESSAGES_FOR_LINUX_TRAY_ATTENTION = "google-messages-for-linux-tray-attention";

        auto const iconTheme = Gtk::IconTheme::get_default();
        if (iconTheme->has_icon(MESSAGES_TRAY) && iconTheme->has_icon(MESSAGES_TRAY_ATTENTION))
        {
            return {MESSAGES_TRAY, MESSAGES_TRAY_ATTENTION};
        }
        else if (iconTheme->has_icon(MESSAGES_FOR_LINUX_TRAY) && iconTheme->has_icon(MESSAGES_FOR_LINUX_TRAY_ATTENTION))
        {
            return {MESSAGES_FOR_LINUX_TRAY, MESSAGES_FOR_LINUX_TRAY_ATTENTION};
        }
        else
        {
            auto const trayPixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/messages-for-linux-tray.png");
            auto const attentionPixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/messages-for-linux-tray-attention.png");
            iconTheme->add_builtin_icon(MESSAGES_FOR_LINUX_TRAY, Gtk::ICON_SIZE_MENU, trayPixbuf);
            iconTheme->add_builtin_icon(MESSAGES_FOR_LINUX_TRAY_ATTENTION, Gtk::ICON_SIZE_MENU, attentionPixbuf);
            if (iconTheme->has_icon(MESSAGES_FOR_LINUX_TRAY) && iconTheme->has_icon(MESSAGES_FOR_LINUX_TRAY_ATTENTION))
            {
                return {MESSAGES_FOR_LINUX_TRAY, MESSAGES_FOR_LINUX_TRAY_ATTENTION};
            }
            else
            {
                return {"indicator-messages", "indicator-messages-new"};
            }
        }
    }
}

TrayIcon::TrayIcon()
    : m_appIndicator{app_indicator_new("com.github.messages-for-linux.tray", "", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
    , m_popupMenu{}
    , m_signalOpen{}
    , m_signalAbout{}
    , m_signalQuit{}
{
    auto const [trayIconName, attentionIconName] = getTrayIconNames();
    app_indicator_set_icon_full(m_appIndicator, trayIconName, "Messages for Linux Tray");
    app_indicator_set_attention_icon_full(m_appIndicator, attentionIconName, "Messages for Linux Tray Attention");

    auto const openMenuItem = Gtk::manage(new Gtk::MenuItem{"Open"});
    auto const aboutMenuItem = Gtk::manage(new Gtk::MenuItem{"About"});
    auto const quitMenuItem = Gtk::manage(new Gtk::MenuItem{"Quit"});
    m_popupMenu.append(*openMenuItem);
    m_popupMenu.append(*aboutMenuItem);
    m_popupMenu.append(*quitMenuItem);

    app_indicator_set_menu(m_appIndicator, m_popupMenu.gobj());

    openMenuItem->signal_activate().connect([this]{ m_signalOpen.emit(); });
    aboutMenuItem->signal_activate().connect([this]{ m_signalAbout.emit(); });
    quitMenuItem->signal_activate().connect([this]{ m_signalQuit.emit(); });

    m_popupMenu.show_all();

    app_indicator_set_status(m_appIndicator, APP_INDICATOR_STATUS_PASSIVE);
    app_indicator_set_menu(m_appIndicator, m_popupMenu.gobj());
}

void TrayIcon::setVisible(bool visible)
{
    app_indicator_set_status(m_appIndicator, (visible ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE));
}

bool TrayIcon::isVisible() const
{
    return (app_indicator_get_status(m_appIndicator) != APP_INDICATOR_STATUS_PASSIVE);
}

void TrayIcon::setAttention(bool attention)
{
    if (!isVisible())
    {
        return;
    }

    app_indicator_set_status(m_appIndicator, (attention ? APP_INDICATOR_STATUS_ATTENTION : APP_INDICATOR_STATUS_ACTIVE));
}

sigc::signal<void> TrayIcon::signalOpen() const noexcept
{
    return m_signalOpen;
}

sigc::signal<void> TrayIcon::signalAbout() const noexcept
{
    return m_signalAbout;
}

sigc::signal<void> TrayIcon::signalQuit() const noexcept
{
    return m_signalQuit;
}

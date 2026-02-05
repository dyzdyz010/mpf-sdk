#pragma once

#include <QString>
#include <QVariantMap>

namespace mpf {

/**
 * @brief Navigation interface for Loader-based page switching
 * 
 * Simple navigation model:
 * - Plugins register their main page URL via registerRoute()
 * - Host uses getPageUrl() to load pages via QML Loader
 * - Internal navigation within plugins uses Popup/Dialog
 * 
 * This avoids cross-DLL QML component dynamic loading issues.
 */
class INavigation
{
public:
    virtual ~INavigation() = default;

    /**
     * @brief Register a route with its QML page URL
     * @param route Route name (e.g., "orders", "settings")
     * @param qmlPageUrl Full URL to the QML page file
     */
    virtual void registerRoute(const QString& route, const QString& qmlPageUrl) = 0;

    /**
     * @brief Get the QML page URL for a route
     * @param route Route name
     * @return QML page URL, or empty string if not found
     */
    virtual QString getPageUrl(const QString& route) const = 0;

    /**
     * @brief Get current active route
     */
    virtual QString currentRoute() const = 0;

    /**
     * @brief Set current active route (called by host when page changes)
     */
    virtual void setCurrentRoute(const QString& route) = 0;

    // API version 3: simplified Loader-based navigation
    static constexpr int apiVersion() { return 3; }
};

} // namespace mpf

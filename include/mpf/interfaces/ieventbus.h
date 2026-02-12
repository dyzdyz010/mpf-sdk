#pragma once

#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <functional>
#include <optional>

namespace mpf {

/**
 * @brief Event data payload
 */
struct Event
{
    QString topic;              ///< Topic/channel name (e.g., "orders/created")
    QString senderId;           ///< Plugin ID of sender
    QVariantMap data;           ///< Event payload
    qint64 timestamp = 0;       ///< Unix timestamp in milliseconds

    QVariantMap toVariantMap() const
    {
        return {
            {"topic", topic},
            {"senderId", senderId},
            {"data", data},
            {"timestamp", timestamp}
        };
    }

    static Event fromVariantMap(const QVariantMap& map)
    {
        Event e;
        e.topic = map.value("topic").toString();
        e.senderId = map.value("senderId").toString();
        e.data = map.value("data").toMap();
        e.timestamp = map.value("timestamp").toLongLong();
        return e;
    }
};

/**
 * @brief Subscription options
 */
struct SubscriptionOptions
{
    bool async = true;              ///< Async delivery (default) vs synchronous
    int priority = 0;               ///< Higher priority = called first
    bool receiveOwnEvents = false;  ///< Receive events from same sender
};

/**
 * @brief Topic statistics
 */
struct TopicStats
{
    QString topic;
    int subscriberCount = 0;
    qint64 eventCount = 0;
    qint64 lastEventTime = 0;

    QVariantMap toVariantMap() const
    {
        return {
            {"topic", topic},
            {"subscriberCount", subscriberCount},
            {"eventCount", eventCount},
            {"lastEventTime", lastEventTime}
        };
    }
};

/**
 * @brief Event bus for inter-plugin communication
 *
 * Three communication patterns:
 *   1. Publish/Subscribe — fire-and-forget broadcast (one-to-many)
 *   2. Request/Response  — synchronous call with return value (one-to-one)
 *   3. Wildcard matching  — "*" single level, "**" multi-level
 *
 * All subscribe calls require a callback. No signal-based subscriptions.
 */
class IEventBus
{
public:
    virtual ~IEventBus() = default;

    /// Callback for pub/sub event handlers
    using EventHandler = std::function<void(const Event&)>;

    /// Callback for request handlers that return a response
    using RequestHandler = std::function<QVariantMap(const Event&)>;

    // ===== Publish/Subscribe =====

    /**
     * @brief Publish an event (async delivery to subscribers)
     * @return Number of subscribers notified
     */
    virtual int publish(const QString& topic,
                        const QVariantMap& data,
                        const QString& senderId = {}) = 0;

    /**
     * @brief Publish synchronously (blocks until all handlers complete)
     * @return Number of subscribers notified
     */
    virtual int publishSync(const QString& topic,
                            const QVariantMap& data,
                            const QString& senderId = {}) = 0;

    /**
     * @brief Subscribe to a topic pattern with a callback
     * @param pattern Topic pattern (supports wildcards)
     * @param subscriberId Plugin ID
     * @param handler Callback invoked for matching events
     * @param options Delivery options
     * @return Subscription ID (for unsubscribe)
     */
    virtual QString subscribe(const QString& pattern,
                              const QString& subscriberId,
                              EventHandler handler,
                              const SubscriptionOptions& options = {}) = 0;

    /**
     * @brief Unsubscribe by ID
     */
    virtual bool unsubscribe(const QString& subscriptionId) = 0;

    /**
     * @brief Unsubscribe all subscriptions for a plugin
     */
    virtual void unsubscribeAll(const QString& subscriberId) = 0;

    // ===== Request/Response =====

    /**
     * @brief Register a request handler (one per topic, no wildcards)
     * @return true if registered, false if topic already has a handler
     */
    virtual bool registerHandler(const QString& topic,
                                 const QString& handlerId,
                                 RequestHandler handler) = 0;

    /**
     * @brief Unregister a request handler
     */
    virtual bool unregisterHandler(const QString& topic) = 0;

    /**
     * @brief Unregister all request handlers for a plugin
     */
    virtual void unregisterAllHandlers(const QString& handlerId) = 0;

    /**
     * @brief Send a synchronous request, get a response
     * @return Response data, or std::nullopt if no handler / error
     */
    virtual std::optional<QVariantMap> request(const QString& topic,
                                               const QVariantMap& data = {},
                                               const QString& senderId = {},
                                               int timeoutMs = 0) = 0;

    /**
     * @brief Check if a handler exists for a topic
     */
    virtual bool hasHandler(const QString& topic) const = 0;

    // ===== Query =====

    virtual int subscriberCount(const QString& topic) const = 0;
    virtual QStringList activeTopics() const = 0;
    virtual TopicStats topicStats(const QString& topic) const = 0;
    virtual QStringList subscriptionsFor(const QString& subscriberId) const = 0;
    virtual bool matchesTopic(const QString& topic, const QString& pattern) const = 0;

    static constexpr int apiVersion() { return 3; }
};

} // namespace mpf

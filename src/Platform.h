#ifndef PLATFORM_H
#define PLATFORM_H
#include <vector>
#include <memory>
#include "User.h"
#include "Item.h"

struct TradingPlatform {
    std::vector<std::shared_ptr<User>> users;
    std::vector<Item> items;
    int nextUserId;
    int nextItemId;

    TradingPlatform();
    bool registerUser(const std::string& username, const std::string& password, const std::string& email, const std::string& phone, const std::string& studentId, const std::string& realName, const std::string& college, UserRole role);
    std::shared_ptr<User> login(const std::string& email, const std::string& password);
    int publishItem(const std::string& name, const std::string& description, const std::string& category, double price, int sellerId);
    bool deleteItem(int itemId, int requesterId);
    std::vector<Item> searchItemsByName(const std::string& keyword) const;
    std::vector<Item> searchItemsByCategory(const std::string& category) const;
    std::vector<Item> getAvailableItems() const;
    std::vector<Item> getAllItems() const;
    int getUserCount() const;
    int getItemCount() const;
    bool purchaseItem(int itemId, int buyerId);
    bool addToCart(int itemId, int userId);
    bool removeFromCart(int itemId, int userId);
    bool addToFavorites(int itemId, int userId);
    bool removeFromFavorites(int itemId, int userId);

    std::shared_ptr<User> findUserById(int userId) const;
    Item* findItemById(int itemId);
};
#endif
#include "Platform.h"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <iomanip>

TradingPlatform::TradingPlatform() : nextUserId(1), nextItemId(1) {
    registerUser("admin", "admin123", "admin@nju.edu.cn", "13921590994", "231240015", "系统管理员", "匡亚明学院", ADMIN);
}

bool TradingPlatform::registerUser(const std::string& username, const std::string& password, const std::string& email, const std::string& phone, const std::string& studentId, const std::string& realName, const std::string& college, UserRole role) {
    for (const auto& user : users) {
        if (user->getEmail() == email) {
            return false;
        }
    }
    std::shared_ptr<User> newUser;
    if (role == ADMIN) {
        newUser = std::make_shared<Admin>(nextUserId++, username, password, email);
    } else {
        newUser = std::make_shared<RegularUser>(nextUserId++, username, password, email, phone, studentId, realName, college);
    }
    users.push_back(newUser);
    return true;
}

std::shared_ptr<User> TradingPlatform::login(const std::string& email, const std::string& password) {
    for (auto& user : users) {
        if (user->getEmail() == email && user->login(password)) {
            return user;
        }
    }
    return nullptr;
}

int TradingPlatform::publishItem(const std::string& name, const std::string& description, const std::string& category, double price, int sellerId) {
    Item newItem(nextItemId++, name, description, category, price, sellerId);
    items.push_back(newItem);
    auto user = std::dynamic_pointer_cast<RegularUser>(findUserById(sellerId));
    if (user) {
        user->publishItem(newItem);
    }
    return newItem.getItemId();
}

bool TradingPlatform::deleteItem(int itemId, int requesterId) {
    auto requester = findUserById(requesterId);
    if (!requester) return false;
    
    auto itemIt = std::find_if(items.begin(), items.end(), [itemId](const Item& item) { 
        return item.getItemId() == itemId; 
    });
    
    if (itemIt != items.end()) {
        if (requester->getRole() == ADMIN || itemIt->getSellerId() == requesterId) {
            itemIt->setStatus(DELETED);
            return true;
        }
    }
    return false;
}

//字符串匹配搜索
std::vector<Item> TradingPlatform::searchItemsByName(const std::string& keyword) const {
    std::vector<Item> result;
    for (const auto& item : items) {
        if (item.isAvailable() && item.getItemName().find(keyword) != std::string::npos) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> TradingPlatform::searchItemsByCategory(const std::string& category) const {
    std::vector<Item> result;
    for (const auto& item : items) {
        if (item.isAvailable() && item.getCategory() == category) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> TradingPlatform::getAvailableItems() const {
    std::vector<Item> result;
    for (const auto& item : items) {
        if (item.isAvailable()) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> TradingPlatform::getAllItems() const {
    return items;
}

int TradingPlatform::getUserCount() const {
    return users.size();
}

int TradingPlatform::getItemCount() const {
    return items.size();
}

bool TradingPlatform::purchaseItem(int itemId, int buyerId) {
    auto itemIt = std::find_if(items.begin(), items.end(), [itemId](const Item& item) { 
        return item.getItemId() == itemId && item.isAvailable(); 
    });
    
    if (itemIt != items.end()) {
        itemIt->setStatus(SOLD);
        auto buyer = std::dynamic_pointer_cast<RegularUser>(findUserById(buyerId));
        if (buyer) {
            buyer->addPurchasedItem(itemId);
        }
        return true;
    }
    return false;
}

bool TradingPlatform::addToCart(int itemId, int userId) {
    auto user = findUserById(userId);
    auto itemIt = std::find_if(items.begin(), items.end(), [itemId](const Item& item) { 
        return item.getItemId() == itemId; 
    });
    
    if (user && itemIt != items.end() && itemIt->isAvailable()) {
        auto regularUser = std::dynamic_pointer_cast<RegularUser>(user);
        if (regularUser) {
            regularUser->addToCart(itemId);
            return true;
        }
    }
    return false;
}

bool TradingPlatform::removeFromCart(int itemId, int userId) {
    auto user = findUserById(userId);
    if (user) {
        auto regularUser = std::dynamic_pointer_cast<RegularUser>(user);
        if (regularUser) {
            regularUser->removeFromCart(itemId);
            return true;
        }
    }
    return false;
}

bool TradingPlatform::addToFavorites(int itemId, int userId) {
    auto user = findUserById(userId);
    auto itemIt = std::find_if(items.begin(), items.end(), [itemId](const Item& item) { 
        return item.getItemId() == itemId; 
    });
    
    if (user && itemIt != items.end() && itemIt->isAvailable()) {
        auto regularUser = std::dynamic_pointer_cast<RegularUser>(user);
        if (regularUser) {
            regularUser->addToFavorites(itemId);
            return true;
        }
    }
    return false;
}

bool TradingPlatform::removeFromFavorites(int itemId, int userId) {
    auto user = findUserById(userId);
    if (user) {
        auto regularUser = std::dynamic_pointer_cast<RegularUser>(user);
        if (regularUser) {
            regularUser->removeFromFavorites(itemId);
            return true;
        }
    }
    return false;
}

std::shared_ptr<User> TradingPlatform::findUserById(int userId) const {
    for (const auto& user : users) {
        if (user->getUserId() == userId) {
            return user;
        }
    }
    return nullptr;
}

Item* TradingPlatform::findItemById(int itemId) {
    for (auto& item : items) {
        if (item.getItemId() == itemId) {
            return &item;
        }
    }
    return nullptr;
}
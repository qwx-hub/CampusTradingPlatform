#ifndef USER_H
#define USER_H
#include <string>
#include <vector>
#include "Item.h"

enum UserRole { REGULAR_USER, ADMIN };

struct User {
    int userId;
    std::string username;
    std::string password;
    std::string email;
    std::string phone;
    std::string studentId;
    std::string realName;
    std::string college;
    UserRole role;

    User(int id, const std::string& uname, const std::string& pwd, const std::string& email, const std::string& phone, const std::string& studentId, const std::string& realName, const std::string& college, UserRole role);
    virtual ~User() = default;
    int getUserId() const;
    std::string getUsername() const;
    std::string getEmail() const;
    UserRole getRole() const;
    bool login(const std::string& inputPassword) const;
    void resetPassword(const std::string& newPassword);
    virtual void displayProfile() const;
};

struct RegularUser : User {
    std::vector<int> publishedItems; // 已发布商品ID
    std::vector<int> purchasedItems; // 已购买商品ID
    std::vector<int> cartItems;     // 购物车商品ID
    std::vector<int> favorites;     // 收藏商品ID

    RegularUser(int id, const std::string& uname, const std::string& pwd, const std::string& email, const std::string& phone, const std::string& studentId, const std::string& realName, const std::string& college);
    void publishItem(const Item& item);
    std::vector<int> getPublishedItems() const;
    void addPurchasedItem(int itemId);
    void addToCart(int itemId);
    void removeFromCart(int itemId);
    void addToFavorites(int itemId);
    void removeFromFavorites(int itemId);
    void displayProfile() const override;
};

struct Admin : User {
    Admin(int id, const std::string& uname, const std::string& pwd, const std::string& email);
    // 简化：删除了未实现的接口，管理员操作通过 TradingPlatform 接口完成。
    // void deleteItem(int itemId);
    // void viewUserInfo(int userId);
    // void viewSystemStats();
};
#endif
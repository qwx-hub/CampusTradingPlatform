#include "User.h"
#include <iostream>
#include <algorithm> 

User::User(int id, const std::string& uname, const std::string& pwd, const std::string& em, const std::string& ph, const std::string& sId, const std::string& rName, const std::string& col, UserRole r) : 
    userId(id), username(uname), password(pwd), email(em), phone(ph), studentId(sId), realName(rName), college(col), role(r) {}

int User::getUserId() const { return userId; }
std::string User::getUsername() const { return username; }
std::string User::getEmail() const { return email; }
UserRole User::getRole() const { return role; }

bool User::login(const std::string& inputPassword) const { 
    return password == inputPassword;
}

void User::resetPassword(const std::string& newPassword) { 
    password = newPassword; 
}

void User::displayProfile() const { 
    std::cout << "用户ID: " << userId << "\n";
    std::cout << "用户名: " << username << "\n";
    std::cout << "邮箱: " << email << "\n";
    std::cout << "手机: " << phone << "\n";
    std::cout << "学号: " << studentId << "\n";
    std::cout << "真实姓名: " << realName << "\n";
    std::cout << "学院: " << college << "\n";
    std::string roleStr = (role == ADMIN) ? "管理员" : "普通用户";
    std::cout << "角色: " << roleStr << "\n";
}

RegularUser::RegularUser(int id, const std::string& uname, const std::string& pwd, const std::string& email, const std::string& phone, const std::string& studentId, const std::string& realName, const std::string& college) : 
    User(id, uname, pwd, email, phone, studentId, realName, college, REGULAR_USER) {}

void RegularUser::publishItem(const Item& item) { 
    publishedItems.push_back(item.getItemId()); 
}

std::vector<int> RegularUser::getPublishedItems() const { 
    return publishedItems; 
}

void RegularUser::addPurchasedItem(int itemId) { 
    purchasedItems.push_back(itemId); 
}

void RegularUser::addToCart(int itemId) {
    // 检查是否已在购物车中
    for (int id : cartItems) {
        if (id == itemId) {
            std::cout << "This item is already in your cart!\n";
            return;
        }
    }
    cartItems.push_back(itemId);
    std::cout << "Item added to cart!\n";
}

void RegularUser::removeFromCart(int itemId) {
    auto it = std::find(cartItems.begin(), cartItems.end(), itemId);
    if (it != cartItems.end()) {
        cartItems.erase(it);
        std::cout << "Item removed from cart!\n";
    } else {
        std::cout << "This item is not in your cart.\n";
    }
}

void RegularUser::addToFavorites(int itemId) {
    // 检查是否已在收藏中
    for (int id : favorites) {
        if (id == itemId) {
            std::cout << "该商品已在收藏中。\n";
            return;
        }
    }
    favorites.push_back(itemId);
    std::cout << "商品已添加到收藏。\n";
}

void RegularUser::removeFromFavorites(int itemId) {
    auto it = std::find(favorites.begin(), favorites.end(), itemId);
    if (it != favorites.end()) {
        favorites.erase(it);
        std::cout << "商品已从收藏移除。\n";
    } else {
        std::cout << "该商品不在收藏中。\n";
    }
}

void RegularUser::displayProfile() const { 
    User::displayProfile(); 
    std::cout << "已发布商品数量: " << publishedItems.size() << "\n";
    std::cout << "已购买商品数量: " << purchasedItems.size() << "\n";
    std::cout << "购物车商品数量: " << cartItems.size() << "\n";
    std::cout << "收藏商品数量: " << favorites.size() << "\n";
}

Admin::Admin(int id, const std::string& uname, const std::string& pwd, const std::string& email) : 
    User(id, uname, pwd, email, "", "", "", "", ADMIN) {}
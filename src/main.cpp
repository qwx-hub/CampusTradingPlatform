#include <iostream>
#include <iomanip>
#include <limits> 
#include <chrono>
#include <thread>
#include "Platform.h"
#include "SearchEngine.h"
#include "User.h"

void displayLoginMenu() {
    std::cout << "\n=== 欢迎使用校园二手交易平台 ===\n";
    std::cout << "1. 用户注册\n";
    std::cout << "2. 用户登录\n";
    std::cout << "0. 退出程序\n";
    std::cout << "请选择操作: ";
}

void displayMainMenu() {
    std::cout << "\n=== 校园二手交易平台首页 ===\n";
    std::cout << "1. 浏览商品\n";
    std::cout << "2. 搜索商品\n";
    std::cout << "3. 查看商品详情\n";
    std::cout << "4. 个人中心\n";
    std::cout << "0. 退出登录\n";
    std::cout << "请选择操作: ";
}

void displayPersonalCenterMenu() {
    std::cout << "\n=== 个人中心 ===\n";
    std::cout << "1. 发布商品\n";
    std::cout << "2. 编辑个人信息\n";
    std::cout << "3. 查看已购买商品\n";
    std::cout << "4. 查看收藏\n";
    std::cout << "5. 查看购物车\n";
    std::cout << "6. 进入管理员模式\n";
    std::cout << "0. 返回首页\n";
    std::cout << "请选择操作: ";
}

void displayItemDetailsMenu(int itemId, const std::shared_ptr<User>& currentUser) {
    std::cout << "\n"; // 商品详情已在主函数中动态打印
    std::cout << "1. 加入购物车\n";
    std::cout << "2. 联系卖家\n";
    std::cout << "3. 立即购买\n";
    std::cout << "4. 添加到收藏\n"; 
    std::cout << "0. 返回\n";
    std::cout << "请选择操作: ";
}

void displayAdminMenu() {
    std::cout << "\n=== 管理员中心 ===\n";
    std::cout << "1. 查看所有商品\n";
    std::cout << "2. 删除商品\n";
    std::cout << "3. 系统统计\n"; 
    std::cout << "0. 返回个人中心\n";
    std::cout << "请选择操作: ";
}

// 处理无效输入
int getChoice() {
    int choice;
    while (!(std::cin >> choice)) {
        std::cout << "无效输入，请输入数字: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return choice;
}


// 处理登录逻辑
std::shared_ptr<User> handleLogin(TradingPlatform& platform) {
    std::string email, password;
    std::cout << "\n--- 用户登录 ---\n";
    std::cout << "邮箱: "; 
    std::cin >> email;
    std::cout << "密码: "; 
    std::cin >> password;

    auto user = platform.login(email, password);
    if (user) {
        std::cout << "登录成功！欢迎，" << user->getUsername() << "。\n";
        return user;
    } else {
        std::cout << "登录失败，请检查邮箱和密码。\n";
        return nullptr;
    }
}

// 处理浏览商品
void handleBrowseItems(TradingPlatform& platform) {
    std::cout << "\n--- 浏览商品 ---\n";
    std::vector<Item> availableItems = platform.getAvailableItems();
    if (availableItems.empty()) {
        std::cout << "目前没有可供浏览的商品。\n";
        return;
    }
    for (const auto& item : availableItems) {
        item.displayInfo();
        std::cout << "------------\n";
    }
}

//   处理商品详情
void handleItemDetails(TradingPlatform& platform, const std::shared_ptr<User>& currentUser) {
    int itemId;
    std::cout << "\n--- 查看商品详情 ---\n";
    std::cout << "请输入商品ID: ";
    itemId = getChoice();

    auto itemPtr = platform.findItemById(itemId);
    if (itemPtr) {
        itemPtr->displayInfo();

        // 提供操作选项
        if (currentUser && itemPtr->isAvailable()) {
            std::cout << "\n操作:\n";
            std::cout << "1. 购买商品\n";
            std::cout << "2. 加入购物车\n";
            std::cout << "3. 加入收藏\n";
            std::cout << "0. 返回\n";
            std::cout << "请选择: ";
            int action = getChoice();

            switch (action) {
                case 1:
                    if (platform.purchaseItem(itemId, currentUser->getUserId())) {
                        std::cout << "购买成功！\n";
                    } else {
                        std::cout << "购买失败，商品状态或用户余额不足。\n";
                    }
                    break;
                case 2:
                    if (platform.addToCart(itemId, currentUser->getUserId())) {
                        std::cout << "已加入购物车。\n";
                    } else {
                        std::cout << "操作失败。\n";
                    }
                    break;
                case 3:
                    if (platform.addToFavorites(itemId, currentUser->getUserId())) {
                        std::cout << "已加入收藏。\n";
                    } else {
                        std::cout << "操作失败。\n";
                    }
                    break;
                case 0:
                    break;
                default:
                    std::cout << "无效选择。\n";
            }
        }
    } else {
        std::cout << "未找到该商品ID。\n";
    }
}

int main() {
    TradingPlatform platform;
    std::shared_ptr<User> currentUser = nullptr;

    // 外部循环: 注册/登录/退出程序
    int loginChoice;
    do {
        displayLoginMenu();
        loginChoice = getChoice();

        switch (loginChoice) {
            case 1: {
                std::string username, password, email, phone, studentId, realName, college;
                std::cout << "用户名: ";
                std::cin >> username;
                std::cout << "密码: ";
                std::cin >> password;
                std::cout << "邮箱: ";
                std::cin >> email;
                std::cout << "手机: ";
                std::cin >> phone;
                std::cout << "学号: ";
                std::cin >> studentId;
                std::cout << "真实姓名: ";
                std::cin >> realName;
                std::cout << "学院: ";
                std::cin >> college;
                
                if (platform.registerUser(username, password, email, phone, studentId, realName, college, REGULAR_USER)) {
                    std::cout << "注册成功！\n";
                } else {
                    std::cout << "注册失败，邮箱已存在。\n";
                }
                break;
            }
            case 2: { // 用户登录
                currentUser = handleLogin(platform);
                if (currentUser) {
                    int mainChoice;
                    do {
                        displayMainMenu();
                        mainChoice = getChoice();

                        switch (mainChoice) {
                            case 1: // 浏览商品
                                handleBrowseItems(platform);
                                break;
                            case 2: {
                                std::string keyword;
                                std::cout << "输入搜索关键词: ";
                                std::cin.ignore();
                                std::getline(std::cin, keyword);
                                auto results = platform.searchItemsByName(keyword);
                                std::cout << "\n=== 搜索结果 ===\n";
                                if (results.empty()) {
                                std::cout << "没有搜索到符合的商品。\n";
                                } else {
                                    for (const auto& item : results) {
                                        item.displayInfo();
                                        std::cout << "------------------------\n";
                                    }
                                }
                                break;
                            }
                            case 3: {
                                // 查看商品详情
                                int itemId;
                                std::cout << "输入商品ID: ";
                                std::cin >> itemId;
                                
                                Item* item = platform.findItemById(itemId);
                                if (item && item->isAvailable()) {
                                    
                                    int detailChoice;
                                    do {
                                        std::cout << "\n=== 商品详情 ===\n";
                                        item->displayInfo();
                                        
                                        displayItemDetailsMenu(itemId, currentUser);
                                        std::cin >> detailChoice;
                                        
                                        switch(detailChoice) {
                                            case 1: {
                                                // 加入购物车
                                                if (currentUser && currentUser->getRole() == REGULAR_USER) {
                                                    platform.addToCart(itemId, currentUser->getUserId());
                                                } else {
                                                    std::cout << "请先登录普通用户账号！\n";
                                                }
                                                break;
                                            }
                                            case 2: {
                                                // 联系卖家
                                                std::cout << "联系卖家: 请通过平台内消息系统联系卖家。\n";
                                                auto seller = platform.findUserById(item->getSellerId());
                                                if (seller) {
                                                    std::cout << "卖家邮箱: " << seller->getEmail() << "\n";
                                                }
                                                break;
                                            }
                                            case 3: {
                                                // 立即购买
                                                if (currentUser && currentUser->getRole() == REGULAR_USER) {
                                                    if (platform.purchaseItem(itemId, currentUser->getUserId())) {
                                                        std::cout << "购买成功！\n";
                                                        detailChoice = 0; // 购买成功后返回
                                                    } else {
                                                        std::cout << "购买失败，商品可能不存在或已售出。\n";
                                                    }
                                                } else {
                                                    std::cout << "请先登录普通用户账号！\n";
                                                }
                                                break;
                                            }
                                            case 4: {
                                                if (currentUser && currentUser->getRole() == REGULAR_USER) {
                                                    platform.addToFavorites(itemId, currentUser->getUserId());
                                                } else {
                                                    std::cout << "请先登录普通用户账号！\n";
                                                }
                                                break;
                                            }
                                            case 0:
                                                break;
                                            default:
                                                std::cout << "无效选择，请重新输入。\n";
                                        }
                                    } while (detailChoice != 0);
                                } else {
                                    std::cout << "商品不存在或已下架。\n";
                                }
                                break;
                            }
                            case 4: { // 个人中心
                                int personalChoice;
                                do {
                                    displayPersonalCenterMenu();
                                    std::cin >> personalChoice;
                                    
                                    switch(personalChoice) {
                                        case 1: {
                                            if (!currentUser || currentUser->getRole() != REGULAR_USER) {
                                                std::cout << "请先以普通用户身份登录。\n";
                                                break;
                                            }
                                            std::string name, description, category;
                                            double price;
                                            std::cout << "商品名称: ";
                                            std::cin.ignore();
                                            std::getline(std::cin, name);
                                            std::cout << "商品描述: ";
                                            std::getline(std::cin, description);
                                            std::cout << "商品分类: ";
                                            std::cin >> category;
                                            std::cout << "价格: ";
                                            std::cin >> price;
                                            
                                            int itemId = platform.publishItem(name, description, category, price, currentUser->getUserId());
                                            std::cout << "商品发布成功！商品ID: " << itemId << "\n";
                                            break;
                                        }
                                        case 2: {
                                            std::string newPhone, newEmail, newPassword;
                                            std::cout << "请输入新手机号: ";
                                            std::cin >> newPhone;
                                            std::cout << "请输入新邮件: ";
                                            std::cin.ignore(); 
                                            std::getline(std::cin, newEmail);
                                            std::cout << "请输入新密码: ";
                                            std::getline(std::cin, newPassword);
                                            
                                            currentUser->phone = newPhone;
                                            currentUser->email = newEmail;
                                            currentUser->password = newPassword;
                                            
                                            std::cout << "个人信息更新成功！\n";
                                            break;
                                        }
                                        case 3: {
                                            if (!currentUser || currentUser->getRole() != REGULAR_USER) {
                                                std::cout << "请先以普通用户身份登录。\n";
                                                break;
                                            }
                                            auto regularUser = std::dynamic_pointer_cast<RegularUser>(currentUser);
                                            if (regularUser) {
                                                std::cout << "\n=== 已购买商品 ===\n";
                                                std::cout << "已购买商品数量: " << regularUser->purchasedItems.size() << "\n";
                                                for (int itemId : regularUser->purchasedItems) {
                                                    Item* item = platform.findItemById(itemId);
                                                    if (item) {
                                                        std::cout << "商品ID: " << item->getItemId() << " - " << item->getItemName() << "\n";
                                                    }
                                                }
                                            }
                                            break;
                                        }
                                        case 4: {
                                            // 查看收藏
                                            if (!currentUser || currentUser->getRole() != REGULAR_USER) {
                                                std::cout << "请先以普通用户身份登录。\n";
                                                break;
                                            }
                                            auto regularUser = std::dynamic_pointer_cast<RegularUser>(currentUser);
                                            if (regularUser) {
                                                std::cout << "\n=== 我的收藏 ===\n";
                                                std::cout << "收藏商品数量: " << regularUser->favorites.size() << "\n";
                                                for (int itemId : regularUser->favorites) {
                                                    Item* item = platform.findItemById(itemId);
                                                    if (item) {
                                                        std::cout << "商品ID: " << item->getItemId() << " - " << item->getItemName() << "\n";
                                                    }
                                                }
                                            }
                                            break;
                                        }
                                        case 5: {
                                            // 查看购物车
                                            if (!currentUser || currentUser->getRole() != REGULAR_USER) {
                                                std::cout << "请先以普通用户身份登录。\n";
                                                break;
                                            }
                                            auto regularUser = std::dynamic_pointer_cast<RegularUser>(currentUser);
                                            if (regularUser) {
                                                std::cout << "\n=== 购物车 ===\n";
                                                std::cout << "购物车商品数量: " << regularUser->cartItems.size() << "\n";
                                                for (int itemId : regularUser->cartItems) {
                                                    Item* item = platform.findItemById(itemId);
                                                    if (item) {
                                                        std::cout << "商品ID: " << item->getItemId() << " - " << item->getItemName() << "\n";
                                                    }
                                                }
                                            }
                                            break;
                                        }
                                        case 6: {
                                            // 管理员功能
                                            if (!currentUser || currentUser->getRole() != ADMIN) {
                                                std::cout << "需要管理员权限。\n";
                                                break;
                                            }
                                            int adminChoice;
                                            do {
                                                std::cout << "\n=== 管理员功能 ===\n";
                                                std::cout << "1. 查看所有商品\n";
                                                std::cout << "2. 删除商品\n";
                                                std::cout << "3. 系统统计\n"; 
                                                std::cout << "0. 返回\n";
                                                std::cout << "请选择操作: ";
                                                std::cin >> adminChoice;
                                                
                                                switch(adminChoice) {
                                                    case 1: {
                                                        auto allItems = platform.getAllItems();
                                                        std::cout << "\n=== 所有商品 ===\n";
                                                        for (const auto& item : allItems) {
                                                            item.displayInfo();
                                                            std::cout << "------------------------\n";
                                                        }
                                                        break;
                                                    }
                                                    case 2: {
                                                        int itemId;
                                                        std::cout << "输入要删除的商品ID: ";
                                                        std::cin >> itemId;
                                                        if (platform.deleteItem(itemId, currentUser->getUserId())) {
                                                            std::cout << "商品删除成功！\n";
                                                        } else {
                                                            std::cout << "商品删除失败。\n";
                                                        }
                                                        break;
                                                    }
                                                    case 3: { 
                                                        std::cout << "-----系统统计-----:\n";
                                                        std::cout << "用户总数: " << platform.getUserCount() << "\n";
                                                        std::cout << "商品总数: " << platform.getItemCount() << "\n";
                                                        break;
                                                    }
                                                }
                                            } while (adminChoice != 0);
                                            break;
                                        }
                                        case 0: // 返回首页
                                            break; 
                                        default:
                                            std::cout << "无效选择，请重新输入。\n";
                                    }
                                } while (personalChoice != 0);
                                break;
                            }
                            case 0: // 退出登录
                                std::cout << "已退出当前账号。\n";
                                currentUser = nullptr; // 重置当前用户
                                break; 
                            default:
                                std::cout << "无效选择，请重新输入。\n";
                        }
                    } while (mainChoice != 0);
                }
                break;
            }
            case 0: // 退出程序
                std::cout << "感谢使用校园二手交易平台！程序2秒后退出。\n";
                break;
            default:
                std::cout << "无效选择，请重新输入。\n";
        }
    } while (loginChoice != 0);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
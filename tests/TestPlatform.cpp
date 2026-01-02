#include <gtest/gtest.h>
#include "Platform.h"
#include "User.h"
#include "Item.h"
#include "SearchEngine.h"

// 使用 Test Fixture 来为每个测试自动创建和销毁 Platform 对象，保持测试环境的独立性
class TradingPlatformTest : public ::testing::Test {
protected:
    TradingPlatform platform;
    int sellerId;
    int buyerId;
    int adminId;
    int strangerId;

    void SetUp() override {
        // 进行公共的初始化，预注册一个卖家一个卖家一个浏览用户一个管理员
        platform.registerUser("seller", "123456", "seller@nju.edu.cn", "111", "101", "Seller Name", "CS", REGULAR_USER);
        platform.registerUser("buyer", "123456", "buyer@nju.edu.cn", "222", "102", "Buyer Name", "Math", REGULAR_USER);
        platform.registerUser("stranger", "123456", "stranger@nju.edu.cn", "333", "103", "Stranger Name", "Physics", REGULAR_USER);
        platform.registerUser("admin1", "admin1234", "admin1@nju.edu.cn", "000", "000", "Admin1", "Admin1", ADMIN);

        // 获取 ID 用于后续操作
        adminId = platform.login("admin1@nju.edu.cn", "admin1234")->getUserId();
        sellerId = platform.login("seller@nju.edu.cn", "123456")->getUserId();
        buyerId = platform.login("buyer@nju.edu.cn", "123456")->getUserId();
        strangerId = platform.login("stranger@nju.edu.cn", "123456")->getUserId();
    }

    void TearDown() override {
    }
};

// =================================================================
// 功能模块 1: 用户注册与登录
// 路径覆盖 成功/失败分支
// =================================================================

// 测试正常注册和登录流程
TEST_F(TradingPlatformTest, RegisterAndLoginSuccess) {
    std::string email = "test@nju.edu.cn";
    std::string pass = "password123";
    
    // 1. 注册新用户
    bool regResult = platform.registerUser("testuser", pass, email, "13900000000", "001", "Test Name", "CS", REGULAR_USER);
    EXPECT_TRUE(regResult) << "注册应当成功";

    // 2. 成功登录
    auto user = platform.login(email, pass);
    ASSERT_NE(user, nullptr) << "登录后返回的指针不应为空"; 
    EXPECT_EQ(user->getEmail(), email);
    EXPECT_EQ(user->getUsername(), "testuser");
}

// 测试边界条件：重复注册
TEST_F(TradingPlatformTest, RegisterDuplicateEmail) {
    std::string email = "same@nju.edu.cn";
    
    // 第一次注册
    platform.registerUser("user1", "password1", email, "111", "001", "N1", "C1", REGULAR_USER);
    
    // 第二次使用相同邮箱注册
    bool regResult = platform.registerUser("user2", "password2", email, "222", "002", "N2", "C1", REGULAR_USER);
    
    EXPECT_FALSE(regResult) << "使用已存在的邮箱注册应当失败";
    //1个项目源代码里设定好的admin + 4个在SetUp环节预注册的用户(buyer,seller,stranger,admin1) + 1个user1
    EXPECT_EQ(platform.getUserCount(), 6) << "用户总数应该是6";
}

// 测试登录失败条件：密码错误
TEST_F(TradingPlatformTest, LoginWrongPassword) {
    std::string email = "wrong@nju.edu.cn";
    platform.registerUser("u1", "correct_pass", email, "111", "001", "N1", "C1", REGULAR_USER);

    auto user = platform.login(email, "wrong_pass");
    EXPECT_EQ(user, nullptr) << "密码错误应当返回空指针";
}

// 测试登录失败条件：邮箱不存在
TEST_F(TradingPlatformTest, LoginWrongEmail) {
    std::string email = "exist@nju.edu.cn";
    platform.registerUser("u1", "correct_pass", email, "111", "001", "N1", "C1", REGULAR_USER);

    auto user = platform.login("notexist@nju.edu.cn", "correct_pass");
    EXPECT_EQ(user, nullptr) << "邮箱不存在应当返回空指针";
}

// =================================================================
// 功能模块 2: 商品生命周期与交互
// =================================================================

// 正常发布商品
// 覆盖：publishItem 基础路径
TEST_F(TradingPlatformTest, Item_Publish_Success) {
    int itemId = platform.publishItem("MacBook Pro", "M1 Chip", "Laptop", 8000.0, sellerId);
    
    ASSERT_GT(itemId, 0); // ID 必须有效
    
    Item* item = platform.findItemById(itemId);
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->getItemName(), "MacBook Pro");
    EXPECT_EQ(item->getPrice(), 8000.0);
    EXPECT_EQ(item->getStatus(), AVAILABLE);
    EXPECT_EQ(item->getSellerId(), sellerId);

    // 验证商品是否进入了卖家的 publishedItems 列表
    auto seller = std::dynamic_pointer_cast<RegularUser>(platform.findUserById(sellerId));
    auto pubItems = seller->getPublishedItems();
    EXPECT_NE(std::find(pubItems.begin(), pubItems.end(), itemId), pubItems.end());
}

// 添加到与移出购物车
// 覆盖：addToCart, removeFromCart 正常路径
TEST_F(TradingPlatformTest, Cart_AddAndRemove) {
    int itemId = platform.publishItem("Book", "Cheap", "Books", 20.0, sellerId);
    
    // 添加到购物车
    bool addRes = platform.addToCart(itemId, buyerId);
    EXPECT_TRUE(addRes);

    // 验证内部状态 
    auto buyer = std::dynamic_pointer_cast<RegularUser>(platform.findUserById(buyerId));
    EXPECT_EQ(buyer->cartItems.size(), 1);
    EXPECT_EQ(buyer->cartItems[0], itemId);

    // 从购物车移除
    bool remRes = platform.removeFromCart(itemId, buyerId);
    EXPECT_TRUE(remRes);
    EXPECT_TRUE(buyer->cartItems.empty());
}

// 添加到购物车的边界和违规条件
// 覆盖：重复添加、添加不存在的商品、添加非AVAILABLE商品
TEST_F(TradingPlatformTest, Cart_EdgeCases) {
    int itemId = platform.publishItem("SoldItem", "Desc", "Test", 10.0, sellerId);
    
    // 1. 重复添加同一商品到购物车，检查 cartItems 大小是否增加
    platform.addToCart(itemId, buyerId);
    platform.addToCart(itemId, buyerId); 
    
    auto buyer = std::dynamic_pointer_cast<RegularUser>(platform.findUserById(buyerId));
    EXPECT_EQ(buyer->cartItems.size(), 1) << "重复添加同一商品，购物车数量不应增加";

    // 2. 添加不可用商品
    platform.purchaseItem(itemId, strangerId); // 先买下它
    bool addSold = platform.addToCart(itemId, buyerId);
    EXPECT_FALSE(addSold) << "不应能将已售出的商品加入购物车";
}

// 成功购买
// 覆盖：purchaseItem 成功路径
TEST_F(TradingPlatformTest, Purchase_Success) {
    int itemId = platform.publishItem("Bike", "New", "Transport", 999.0, sellerId);
    //验证是否购买成功
    bool buyRes = platform.purchaseItem(itemId, buyerId);
    EXPECT_TRUE(buyRes);
    //验证商品状态是否改变
    Item* item = platform.findItemById(itemId);
    EXPECT_EQ(item->getStatus(), SOLD);

    // 验证买家能查到购买记录
    auto buyer = std::dynamic_pointer_cast<RegularUser>(platform.findUserById(buyerId));
    EXPECT_EQ(buyer->purchasedItems.size(), 1);
    EXPECT_EQ(buyer->purchasedItems[0], itemId);
}

// 购买流程失败，重复购买同一商品或购买已经被别人买走/下架的商品
// 覆盖：purchaseItem 失败分支
TEST_F(TradingPlatformTest, Purchase_Fail_AlreadySold) {
    int itemId = platform.publishItem("SomeProduct", "Only one", "Figure", 520.0, sellerId);
    
    // 有个买家买走了
    EXPECT_TRUE(platform.purchaseItem(itemId, buyerId));
    
    // 路人试图购买同一件商品
    bool strangerBuy = platform.purchaseItem(itemId, strangerId);
    EXPECT_FALSE(strangerBuy) << "已售出的商品不能被再次购买";
}


// 管理员删除商品
// 覆盖：deleteItem 管理员分支
TEST_F(TradingPlatformTest, Delete_ByAdmin) {
    int itemId = platform.publishItem("Illegal Item", "against law", "PC", 114514.0, sellerId);
    //管理员删除
    bool res = platform.deleteItem(itemId, adminId);
    EXPECT_TRUE(res) << "管理员应该有权限删除任何商品";
    //检查商品状态是否已经被改为已删除
    Item* item = platform.findItemById(itemId);
    EXPECT_EQ(item->getStatus(), DELETED);
}

// 普通用户删除商品因无权限失败
// 覆盖：deleteItem 失败分支
TEST_F(TradingPlatformTest, Delete_Fail_Unauthorized) {
    int itemId = platform.publishItem("Treasure", "999K", "Gold", 9999.0, sellerId);
    
    // 路人试图删除卖家的商品
    bool res = platform.deleteItem(itemId, strangerId);
    EXPECT_FALSE(res) << "普通用户不能删除别人的商品";
    
    Item* item = platform.findItemById(itemId);
    EXPECT_EQ(item->getStatus(), AVAILABLE) << "删除失败后状态应保持不变";
}

// =================================================================
// 功能模块 3: 搜索与筛选
// =================================================================

// 通过名称进行关键词搜索
// 覆盖：searchItemsByName 逻辑
TEST_F(TradingPlatformTest, Search_Keywords) {
    platform.publishItem("iPhone 15", "Almost new", "Phone", 8999, sellerId);
    platform.publishItem("OPPO Phone", "Used for only one year", "Phone", 4599, sellerId);
    platform.publishItem("Charger", "Cable for iPhone", "Accessory", 49, sellerId);

    // 1. 搜名称中带有Phone的
    auto res1 = platform.searchItemsByName("Phone");
    EXPECT_EQ(res1.size(), 2);
    EXPECT_EQ(res1[0].getItemName(), "iPhone 15");
    EXPECT_EQ(res1[1].getItemName(), "OPPO Phone");

    // 2. 搜名称中带有OPPO的
    auto res2 = platform.searchItemsByName("OPPO");
    EXPECT_EQ(res2.size(), 1);
    EXPECT_EQ(res2[0].getItemName(), "OPPO Phone");

    // 3. 搜名称中包含Charge的
    auto res3 = platform.searchItemsByName("Charge"); 
    EXPECT_GE(res3.size(), 1); 
    EXPECT_EQ(res3[0].getItemName(), "Charger");
}

// 搜索 - 状态过滤
// 覆盖：确保已售出/已删除商品不出现在结果中
TEST_F(TradingPlatformTest, Search_FilterStatus) {
    int id1 = platform.publishItem("TargetItem A", "Well", "Cat", 100, sellerId);
    int id2 = platform.publishItem("TargetItem B", "Good", "Cat", 200, sellerId);
    int id3 = platform.publishItem("TargetItem C", "Nice", "Cat", 300, sellerId);

    platform.purchaseItem(id1, buyerId); // A 已售出
    platform.deleteItem(id3, sellerId);  // C 已删除

    auto results = platform.searchItemsByName("TargetItem");
    
    EXPECT_EQ(results.size(), 1) << "搜索结果应该只包含 AVAILABLE 的商品";
    if (!results.empty()) {
        EXPECT_EQ(results[0].getItemId(), id2);
    }
}

// 分类搜索
// 覆盖：searchItemsByCategory
TEST_F(TradingPlatformTest, Search_Category) {
    platform.publishItem("A", "D", "Electronics", 100, sellerId);
    platform.publishItem("B", "D", "Books", 50, sellerId);
    platform.publishItem("C", "D", "Electronics", 200, sellerId);

    auto results = platform.searchItemsByCategory("Electronics");
    EXPECT_EQ(results.size(), 2);
    for (const auto& item : results) {
        EXPECT_EQ(item.getCategory(), "Electronics");
    }
}

// 价格排序
// 覆盖：SearchEngine::sortByPrice
TEST_F(TradingPlatformTest, Search_SortPrice) {
    platform.publishItem("Cheap", "cheap", "C", 1.5, sellerId);
    platform.publishItem("Expensive", "expensive", "C", 1000.0, sellerId);
    platform.publishItem("Mid", "D", "suitable", 99.9, sellerId);

    auto allItems = platform.getAvailableItems();
    // 升序
    auto sortedAsc = SearchEngine::sortByPrice(allItems, true);
    EXPECT_EQ(sortedAsc[0].getPrice(), 1.5);
    EXPECT_EQ(sortedAsc[1].getPrice(), 99.9);
    EXPECT_EQ(sortedAsc[2].getPrice(), 1000.0);

    // 降序
    auto sortedDesc = SearchEngine::sortByPrice(allItems, false);
    EXPECT_EQ(sortedDesc[0].getPrice(), 1000.0);
    EXPECT_EQ(sortedDesc[1].getPrice(), 99.9);
    EXPECT_EQ(sortedDesc[2].getPrice(), 1.5);
}
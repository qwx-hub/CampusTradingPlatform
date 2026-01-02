#include "Item.h"
#include <iostream>
#include <iomanip>
#include <ctime>

Item::Item(int id, const std::string& name, const std::string& desc, const std::string& cat, double price, int sellerId) : 
    itemId(id), itemName(name), description(desc), category(cat), price(price), status(AVAILABLE), sellerId(sellerId) {
    
    // 初始化发布日期
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    publishDate = std::string(buffer);
}

int Item::getItemId() const { return itemId; }
std::string Item::getItemName() const { return itemName; }
std::string Item::getDescription() const { return description; }
std::string Item::getCategory() const { return category; }
double Item::getPrice() const { return price; }
ItemStatus Item::getStatus() const { return status; }
int Item::getSellerId() const { return sellerId; }

void Item::setStatus(ItemStatus newStatus) { status = newStatus;}

void Item::updateInfo(const std::string& name, const std::string& desc, const std::string& cat, double newPrice) {
    itemName = name;
    description = desc;
    category = cat;
    price = newPrice;
}

void Item::displayInfo() const {
    std::cout << "商品ID: " << itemId << "\n";
    std::cout << "名称: " << itemName << "\n";
    std::cout << "描述: " << description << "\n";
    std::cout << "分类: " << category << "\n";
    std::cout << "价格: " << std::fixed << std::setprecision(2) << price << "\n";
    std::cout << "发布日期: " << publishDate << "\n";
    
    std::string statusStr;
    switch(status) {
        case AVAILABLE: statusStr = "可购买"; break;
        // 简化: 移除了 APPROVED 状态
        case SOLD: statusStr = "已售出"; break;
        case DELETED: statusStr = "已删除"; break;
        default: statusStr = "未知"; 
    }
    std::cout << "状态: " << statusStr << "\n";
}

bool Item::isAvailable() const { return status == AVAILABLE; }
#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>

enum ItemStatus { AVAILABLE, DELETED, SOLD };

struct Item {
    int itemId;
    std::string itemName;
    std::string description;
    std::string category;
    double price;
    std::vector<std::string> images;
    ItemStatus status;
    std::string publishDate;
    int sellerId;

    Item(int id, const std::string& name, const std::string& desc, const std::string& cat, double price, int sellerId);
    int getItemId() const;
    std::string getItemName() const;
    std::string getDescription() const;
    std::string getCategory() const;
    double getPrice() const;
    ItemStatus getStatus() const;
    int getSellerId() const;
    void setStatus(ItemStatus newStatus);
    void updateInfo(const std::string& name, const std::string& desc, const std::string& cat, double price);
    void displayInfo() const;
    bool isAvailable() const;
};
#endif
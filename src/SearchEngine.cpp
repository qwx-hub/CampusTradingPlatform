#include "SearchEngine.h"
#include <algorithm>
#include <cctype>


//该文件中部分函数并没有被使用到，但是可以作为后续接口，进一步拓展软件功能，因此我还保留
SearchCriteria::SearchCriteria() : minPrice(0), maxPrice(1000000) {}

void SearchCriteria::setKeyword(const std::string& kw) { keyword = kw; }
void SearchCriteria::setCategory(const std::string& cat) { category = cat; }
void SearchCriteria::setPriceRange(double min, double max) { 
    minPrice = min; 
    maxPrice = max; 
}
void SearchCriteria::setSortBy(const std::string& sort) { sortBy = sort; }

std::vector<Item> SearchCriteria::apply(const std::vector<Item>& allItems) const {
    std::vector<Item> result;
    
    for (const auto& item : allItems) {
        if (!item.isAvailable()) continue;
        
        if (item.getPrice() < minPrice || item.getPrice() > maxPrice) {
            continue;
        }
        
        if (!keyword.empty() && 
            item.getItemName().find(keyword) == std::string::npos &&
            item.getDescription().find(keyword) == std::string::npos) {
            continue;
        }
        
        if (!category.empty() && item.getCategory() != category) {
            continue;
        }
        
        result.push_back(item);
    }
    
    if (sortBy == "price_asc") {
        std::sort(result.begin(), result.end(), 
                 [](const Item& a, const Item& b) { return a.getPrice() < b.getPrice(); });
    } else if (sortBy == "price_desc") {
        std::sort(result.begin(), result.end(), 
                 [](const Item& a, const Item& b) { return a.getPrice() > b.getPrice(); });
    }
    
    return result;
}

std::vector<Item> SearchEngine::textSearch(const std::vector<Item>& items, 
                                          const std::string& keyword) {
    std::vector<Item> result;
    for (const auto& item : items) {
        if (item.isAvailable() && 
            (item.getItemName().find(keyword) != std::string::npos ||
             item.getDescription().find(keyword) != std::string::npos)) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> SearchEngine::categorySearch(const std::vector<Item>& items, 
                                              const std::string& category) {
    std::vector<Item> result;
    for (const auto& item : items) {
        if (item.isAvailable() && item.getCategory() == category) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> SearchEngine::sortByPrice(const std::vector<Item>& items, 
                                           bool ascending) {
    std::vector<Item> result = items;
    if (ascending) {
        std::sort(result.begin(), result.end(), 
                 [](const Item& a, const Item& b) { return a.getPrice() < b.getPrice(); });
    } else {
        std::sort(result.begin(), result.end(), 
                 [](const Item& a, const Item& b) { return a.getPrice() > b.getPrice(); });
    }
    return result;
}
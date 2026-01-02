#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <vector>
#include <string>
#include "Item.h"

enum SearchType {
    TEXT_SEARCH,
    CATEGORY_FILTER,
    IMAGE_SEARCH
};

struct SearchCriteria {
    std::string keyword;
    std::string category;
    double minPrice;
    double maxPrice;
    std::string sortBy;

    SearchCriteria();
    
    void setKeyword(const std::string& kw);
    void setCategory(const std::string& cat);
    void setPriceRange(double min, double max);
    void setSortBy(const std::string& sort);
    
    std::vector<Item> apply(const std::vector<Item>& allItems) const;
};

struct SearchEngine {
    static std::vector<Item> textSearch(const std::vector<Item>& items, 
                                       const std::string& keyword);
    static std::vector<Item> categorySearch(const std::vector<Item>& items, 
                                           const std::string& category);
    static std::vector<Item> sortByPrice(const std::vector<Item>& items, 
                                        bool ascending = true);
};

#endif
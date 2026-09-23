#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// --- ساختارهای داده‌ای پایه ---

struct Material {
    string name;
};

struct Ingredient {
    string materialName;
    int quantity;
};

struct Recipe {
    string itemName;
    vector<Ingredient> ingredients;
};

// --- کلاس اصلی مدیریت ساخت (The Logic Engine) ---

class CraftingEngine {
private:
    // گروه‌ها را به صورت یک Map نگه می‌داریم: <نام گروه, لیست دستورالعمل‌ها>
    map<string, vector<Recipe>> database;

public:
    // ۱. اضافه کردن یک گروه جدید (مثلاً: Weapons, Armor, Tools)
    void addGroup(const string& groupName) {
        if (database.find(groupName) == database.end()) {
            database[groupName] = vector<Recipe>();
            cout << "[System] Group '" << groupName << "' created successfully.\n";
        }
    }

    // ۲. اضافه کردن یک دستورالعمل به یک گروه
    void addRecipe(const string& groupName, const string& itemName, const vector<Ingredient>& ingredients) {
        if (database.find(groupName) != database.end()) {
            Recipe newRecipe = {itemName, ingredients};
            database[groupName].push_back(newRecipe);
            cout << "[System] Recipe for '" << itemName << "' added to '" << groupName << "'.\n";
        } else {
            cout << "[Error] Group '" << groupName << "' does not exist!\n";
        }
    }

    // ۳. پیدا کردن مواد مورد نیاز برای یک آیتم خاص
    void getRecipeDetails(const string& groupName, const string& itemName) {
        if (database.find(groupName) == database.end()) {
            cout << "[Error] Group not found!\n";
            return;
        }

        auto& recipesInGroup = database[groupName];
        auto it = find_if(recipesInGroup.begin(), recipesInGroup.end(), [&](const Recipe& r) {
            return r.itemName == itemName;
        });

        if (it != recipesInGroup.end()) {
            cout << "\n================================" << endl;
            cout << "  CRAFTING: " << it->itemName << endl;
            cout << "================================" << endl;
            for (const auto& ing : it->ingredients) {
                cout << " >> " << ing.materialName << " [x" << ing.quantity << "]" << endl;
            }
            cout << "================================\n" << endl;
        } else {
            cout << "[Error] Item '" << itemName << "' not found in group '" << groupName << "'.\n";
        }
    }

    // ۴. لیست کردن تمام آیتم‌های موجود در یک گروه (برای نمایش در منوی گرافیکی)
    vector<string> getItemsInGroup(const string& groupName) {
        vector<string> items;
        if (database.find(groupName) != database.end()) {
            for (const auto& recipe : database[groupName]) {
                items.push_back(recipe.itemName);
            }
        }
        return items;
    }
};

// --- تست کردن منطق در محیط ترمینال (قبل از رفتن به GUI) ---

int main() {
    CraftingEngine engine;

    // ایجاد گروه‌ها
    engine.addGroup("Weapons");
    engine.addGroup("Armor");

    // اضافه کردن مواد برای یک شاتگان (Shotgun)
    vector<Ingredient> shotgunIngredients = {
        {"Iron", 5},
        {"Wood", 2},
        {"Gunpowder", 3}
    };
    engine.addRecipe("Weapons", "Shotgun", shotgunIngredients);

    // اضافه کردن مواد برای یک کلاهخود (Helmet)
    vector<Ingredient> helmetIngredients = {
        {"Iron", 10},
        {"Leather", 4}
    };
    engine.addRecipe("Armor", "Helmet", helmetIngredients);

    // تست نمایش دستورالعمل
    engine.getRecipeDetails("Weapons", "Shotgun");
    engine.getRecipeDetails("Armor", "Helmet");

    return 0;
}

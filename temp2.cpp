#include <iostream>
#include <map>
#include <array>

std::ostream& operator<<(std::ostream& os, const std::array<int, 3> a) {
    os << "{" << a[0] << ", " << a[1] << ", " << a[2] << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::array<int, 3>>& m) {
    for (auto x : m) {
        os << x.first << ": " << x.second << std::endl;
    }
    return os;
}

class Temp {
public:
    Temp() {
        my_map["ab"] = {0,1,2};
        my_map["bc"] = {3,4,5};
    }
    std::map<std::string, std::array<int, 3>> my_map;
    void bar() {
        my_map.at("ab") = my_map.at("bc");
    }
    void foo() {
        bar();
    }
};

int main() {
    Temp item = Temp();
    
    std::cout << item.my_map << std::endl;

    item.foo();

    std::cout << item.my_map << std::endl;
    return 0;
}
#include <iostream>

#include "dynamic_connectivity.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int n, q;
    std::cin >> n >> q;
    dnc::DynamicConnectivity dc(n);

    char t;
    int a, b;
    for (int i = 0; i < q; i++) {
        std::cin >> t;
        if (t == '+') {
            std::cin >> a >> b;
            a--;
            b--;
            dc.insert_edge(a, b);
        }
        if (t == '-') {
            std::cin >> a >> b;
            a--;
            b--;
            dc.delete_edge(a, b);
        }
        if (t == '?') {
            std::cin >> a >> b;
            a--;
            b--;
            bool res = dc.is_connected(a, b);
            std::cout << res << '\n';
        }
        if (t == ':') {
            std::cout << dc.connected_components() << '\n';
        }
    }
}

namespace dnc{

class DynamicConnectivityImpl;
class DynamicConnectivity{
private:
    DynamicConnectivityImpl* dc;
public:
    DynamicConnectivity(int n);
    ~DynamicConnectivity();
    void insert_edge(int u, int v);
    bool delete_edge(int u, int v);
    bool is_connected(int u, int v);
    unsigned int connected_components();
};

} // namespace dnc
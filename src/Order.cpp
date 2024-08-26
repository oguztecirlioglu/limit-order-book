class Order {
    private:
        int id;
        ORDER_TYPE type;
        int shares;
        float price;
        int time;
    public:
};

enum ORDER_TYPE {
    BUY = 1,
    SELL = 2
};
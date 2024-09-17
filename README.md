This project uses C++ 23 so you may see newer syntax.

Need to do:
Add
Cancel
Execute (Market Order)
Best Bid / Ask
Volume at Limit

Add operation places an order at the end of a list of orders to be executed at a particular limit price
Cancel operation removes an order from anywhere in the book
Execution removes an order from the inside of the book (the inside of the book is defined as the oldest buy order at the highest buying price and the oldest sell order at the lowest selling price)

Lobster data format in ./sampleData:

- Time: Seconds after midnight with decimal precision of at least milliseconds and up to nanoseconds depending on the period requested
- Event Type:
  1. Submission of a new limit order
  2. Cancellation (partial deletion of a limit order)
  3. Deletion (total deletion of a limit order)
  4. Execution of a visible limit order
  5. Execution of a hidden limit order
  6. Indicates a cross trade, e.g. auction trade (never occurs in sample data)
  7. Trading halt indicator (detailed information below) (never occurs in sample data)
- Order ID: Unique order reference number
- Size: Number of shares
- Price: Dollar price times 10000 (i.e. a stock price of $91.14 is given by 911400)
- Direction:
  - -1: Sell limit order
  - 1: Buy limit order
  - Note: Execution of a sell (buy) limit order corresponds to a buyer (seller) initiated trade, i.e buy (sell) trade.

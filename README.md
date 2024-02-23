# Problem 1

To compile the program, ensure you have `gcc` installed, and then run the following command in the directory of the repo:

```
g++ -O2 -o problem1 problem1.cpp
```

In order to run the program, use the command:

```
./problem1
```

## Approach

The presence of the cupcake can only provide one bit of information everytime a guest finishes the maze, so my strategy was to designate a particular guest to keep track of how many guests have passed through the maze; in my case, I chose that to be Guest 0.

Since the cupcake is initially present, every guest that isn't Guest 0 will eat the cupcake without replacement (if the cupcake is there), but they will only eat the cupcake at most once. This doesn't require communicating with the other guests, since a guest can remember if they ate the cupcake or not.

Then, whenever Guest 0 finishes the maze, if the cupcake isn't present, they increment the counter for how many people have passed through the maze and ask the minotaur to replace the cupcake. If that counter reaches N - 1 (since Guest 0 isn't counted), then we know we're done, and every guest has been through the maze.

As for implementation, I used atomics. In order to allow Guest 0 to replace the cupcake if it wasn't presented, I used compare_exchange_strong, which can check if cupcake == false and set cupcake to true in one atomic operation. I used the same method for the other guests, in order to attempt to eat the cupcake only if it is present, and know if that attempt was successful.

## Experimental evaluation

I ran the program 4 times (output available in the files named problem1-output.X.txt), and manually went through to check if the strategy I intended to implement is indeed the strategy that was being performed by the program. After manual examination of the data, I found that it is.

# Problem 2

## Strategy

For this problem, there are 3 available strategies. Strategies 1 and 2 are functionally equivalent; you can draw a direct bijection between the states in both strategies. "Door is open" maps to "sign reads 'AVAILABLE'", and "door is closed" maps to "sign reads 'BUSY'". Both of these strategies have the same advantages and disadvantages. The main advantage is the simplicity of the implementation, since you can use a spinlock to model the same behavior. The disadvantages, however, are clear: there may be many guests waiting around for no good reason, when they could be off doing other things instead of checking whether the room is available or not. Strategy 3 does not have these same disadvantages, since each guest is notified when the room becomes available. However, this comes at the cost of a more complex implementation.

## Execution

To compile the program, run the following command in the directory of the repo:

```
g++ -O2 -o problem2 problem2.cpp
```

In order to run the program, use the command:

```
./problem2
```

## Approach

For this assignment, I chose strategy 2, since the implementation is simple enough and should provide good performance as well. There are 8 guests, and each one decides to look at the vase 10 times in total. Everytime a guest wants to look at the vase, they attempt to get into the room (this is modelled using an atomic boolean), and they succeed only if the room is available. If it's not available, then they try again until they succeed. Whenever they enter the room, the guest prints out 'Wow, what a vase...', so that we can actually see that the guest visited the room. Afterwards, the guest makes the room available again.

## Experimental evaluation

I ran the program 4 times (output available in the files named problem2-output.X.txt), and manually went through to check if the output looks consistent with the strategy I intended to implement. After manual examination of the data, I found that it is.

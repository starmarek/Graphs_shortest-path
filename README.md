# Graphs_shortest-path
Program is using well known Bellman-Ford algorithm to calculate shortest paths in a graph. Its main purpose is to check Bellmans efficiency
regarding graphs with diffrent properties.

## How does it work
1. In the `main()` function, user is able to choose between **two** modes (zones) -> **FUN ZONE** or **TEST ZONE**.
  - **FUN ZONE** -> right there you are able to create your own ***single*** instance of graph (with specific properties) and let Bellman solve it for you. 
  Program in a very friendly manner prints an outcome to console and even a graph structure for you.
  - **TEST ZONE** -> here automatic testing is taking place, which means that program is creating number of instances and counts mean of execution times.
  What's more any console visualization is beeing turned off. Your job is to specify which combinations of graph properties should it use.
2. Properties that user is able to set are following:
  - Number of vertices in graph
  - Graph density
  - Graph representation ***ONLY FUN*** 
  - Starting node for Bellmans algorithm ***ONLY FUN*** 
  - Wheter program will create nodes with loop-edges to themselves ***ONLY FUN*** 
  - Wheter you want a random graph or inputted from file ***ONLY FUN*** 
  - How may bellman executions program should mean for a single properties combination ***ONLY TEST*** 
  3. Program accepts and produces number of  text files:
  - **Input.txt** -> ***ONLY FUN*** File where you are able to specify a graph which program will input. You are ***obligated*** to use strict syntax or 
  graph won't be inputted correctly. Syntax is as following ("|" counts as a space):
  ```
  number of edges | number of vertices | starting node for Bellmans
  source node of edge | destination node of edge | weight of edge
  source node of edge | destination node of edge | weight of edge
  ...
  ```
  - **CreatedInput.txt** -> ***ONLY FUN*** This file is created automatically if user chose to create a random graph. In the file you
  will find the same informations about your graph as you would create an Input manually. This functionality comes in handy if you would like to
  use some kind of visualization tool for your brand new graph or if you just want to save it for later.
  - **Output.txt** -> ***ONLY FUN*** In this file program saves the outcome of Bellman-Ford.
  - **OutputTimes.txt** -> ***ONLY TEST*** Program uses this file to save means of Bellman execution times in a MS Excel friendly manner. 
 
 ## Additional notes
  Instead of `std::numeric_limits::max()` or C's `INFINITY` program is using coustom static variable `s_inifinity` which is set 1000000 to
  avoid overflow. 
  This is strictly connected to the interval in which weight of edges can be drawed. If you want to make it bigger than actual one (-1 <-> 18),
  you ***should*** also make the `s_infinity` bigger. Otherwise Bellman might not work properly for bigger number of vertices.
  
  ## Program structure
  ![](https://raw.githubusercontent.com/starmarek/Graphs_shortest-path/master/structure.png)
  
  ## Credtis
 - Initial idea and guidelines about what program should contain -> **Dr inż. Łukasz Jeleń**, professor of Wrocław University of Science.
 - All of the code and final structure of the program -> **Aleksander Pucher**  

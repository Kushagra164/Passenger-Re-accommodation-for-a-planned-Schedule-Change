<h1 align = "center"> GraphQUBO </h1>
<span style="font-size: 20px; "> GraphQUBO consists of the following files: </span>

- [<span style="font-size: 18px; color: orange"> graph.h </span>](./graph.h)
- [<span style="font-size: 18px; color: orange"> qubo.h </span>](./qubo.h)
- [<span style="font-size: 18px; color: orange"> constraints.h </span>](./constraints.h)
- [<span style="font-size: 18px; color: orange"> graphToQubo.cpp </span>](./graphToQubo.cpp)
- [<span style="font-size: 18px; color: orange"> binaryToEdges.cpp </span>](./binaryToEdges.cpp)

![-----------------------------------------------------](../Images/rainbow.png)

<h2 align = "left"> graph.h </h2>

<span style="font-size: 15px;"> `graph.h` header consists of: </span> 

<strong>function</strong> `weightedInput` Used to take weighted edges as input<br>
<strong>function</strong> `unweightedInput` Used to take unweighted edges as input<br>
<strong>function</strong> `mappingOutput` Used to output all graph edges<br>
<strong>function</strong> `mapInput` Used to take input edges<br>
<strong>function</strong> `printSelectedEdges` Used to output selected edges<br>

![-----------------------------------------------------](../Images/rainbow.png)

<h2 align = "left"> qubo.h </h2>

<span style="font-size: 15px;"> `qubo.h` header consists of </span> 

<strong>class</strong> `qubo`<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `adjust` Converts matrix into upper triangular for Dwave sampler<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `addVariable` Adds new variable to QUBO<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `add` Adds value to a particular term<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `size` Returns size of QUBO<br>

![-----------------------------------------------------](../Images/rainbow.png)

<h2 align = "left"> constraints.h </h2>

<span style="font-size: 15px;"> `constraints.h` header consists of: </span> 

<strong>class</strong> `ineqConstraints` <br>
&nbsp; &nbsp; &nbsp; &nbsp; Adds an inequality constraint to our QUBO<br>

<strong>class</strong> `eqConstraints` <br>
&nbsp; &nbsp; &nbsp; &nbsp; Adds an equality constraint to our QUBO<br>

Functions used by these classes :<br>
- <strong>function</strong> `atMaxOneAdd` Adding at maximum one constraint for given indices<br>
- <strong>function</strong> `addInq` Adding inequality constraint<br>
- <strong>function</strong> `adjustToQubo` Adjusting QUBO according to constraint<br>

![-----------------------------------------------------](../Images/rainbow.png)

<h2 align = "left"> graphToQubo.cpp </h2>
<span style="font-size: 15px;"> graphToQubo.cpp file consists of </span> 

![5-partite graph](../Images/pentagraph.png)

In graphToQubo, we start by constructing a 5-partite graph:<br>

- `UV` has weighted edges, weight = $$OldPNRScore * NewPNRScore * Flightscore$$<br>
- `UC` has weighted edges, weight = $$OldPNRScore * Avg{NewPNRsScore} * connectingFlightscore$$<br>
- `UW`, `WD`, `DV`, `CV` are unweighted<br>

Then we have converted the 5-partite graph into QUBO instances for our quantum algorithm.<br>

![-----------------------------------------------------](../Images/rainbow.png)

<h2 align = "left"> binaryToEdges.cpp </h2>

<span style="font-size: 15px;"> `binaryToEdges.cpp` file is an utility file used for: </span> 

Reading QUBO output and mapping `UV`, `UC` and `WD` edges on the graph.<br>













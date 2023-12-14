<h1 align = "center"> GraphQUBO </h1>
<span style="font-size: 20px; "> GraphQUBO consists of following files: </span>

- [<span style="font-size: 18px; color: orange"> graph.h </span>](https://github.com/Kushagra164/InterIIT-PS7/blob/main/GraphQUBO/graph.h)
- [<span style="font-size: 18px; color: orange"> qubo.h </span>](https://github.com/Kushagra164/InterIIT-PS7/blob/main/GraphQUBO/qubo.h)
- [<span style="font-size: 18px; color: orange"> constraints.h </span>](https://github.com/Kushagra164/InterIIT-PS7/blob/main/GraphQUBO/constraints.h)
- [<span style="font-size: 18px; color: orange"> graphToQubo.cpp </span>](https://github.com/Kushagra164/InterIIT-PS7/blob/main/GraphQUBO/graphToQubo.cpp)
- [<span style="font-size: 18px; color: orange"> binaryToEdges.cpp </span>](https://github.com/Kushagra164/InterIIT-PS7/blob/main/GraphQUBO/binaryToEdges.cpp)

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 align = "left"> graph.h </h2>
<span style="font-size: 15px;"> graph.h header consists of </span> 

<strong>function</strong> `weightedInput` Used to take weighted edges as input<br>
<strong>function</strong> `unweightedInput` Used to take unweighted edges as input<br>
<strong>function</strong> `mappingOutput` Used to output all graph edges<br>
<strong>function</strong> `mapInput` Used to take input edges<br>
<strong>function</strong> `printSelectedEdges` Used to output selected edges<br>

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 align = "left"> qubo.h </h2>
<span style="font-size: 15px;"> qubo.h header consists of </span> 

<strong>class</strong> `qubo`<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `adjust` Used to convert matrix into upper triangular for Dwave sampler<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `addVariable` Adding new variable to QUBO<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `add` Adding value to a particular term<br>
&nbsp; &nbsp; &nbsp; &nbsp; <strong>function</strong> `size` Returns size of QUBO<br>

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 align = "left"> constraints.h </h2>
<span style="font-size: 15px;"> constraints.h header consists of </span> 

<strong>class</strong> `ineqConstraints` <br>
&nbsp; &nbsp; &nbsp; &nbsp; Adds an inequality constraint to our QUBO<br>

<strong>class</strong> `eqConstraints` <br>
&nbsp; &nbsp; &nbsp; &nbsp; Adds an equality constraint to our QUBO<br>

Functions used by these classes are<br>
- <strong>function</strong> `atMaxOneAdd` Adding at maximum one constraint for given indices<br>
- <strong>function</strong> `addInq` Adding inequality constraint<br>
- <strong>function</strong> `adjustToQubo` Adjusting QUBO according to constraint<br>

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 align = "left"> graphToQubo.cpp </h2>
<span style="font-size: 15px;"> graphToQubo.cpp file consists of </span> 

![5-partite graph](https://github.com/Kushagra164/InterIIT-PS7/blob/main/Images/_pentagraph.png)

In graphToQubo, first we will construct a 5-partite graph<br>

- `UV` has weighted edge, weight = OldPNRScore * NewPNRScore * Flightscore<br>
- `UC` has weighted edge, weight = OldPNRScore * Avg{NewPNRsScore} * connectingFlightscore<br>
- `UW`, `WD`, `DV`, `CV` are unweighted<br>

Then we converted 5-partite graph into QUBO instances and then output it.<br>

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 align = "left"> binaryToEdges.cpp </h2>
<span style="font-size: 15px;"> binaryToEdges.cpp file consists of utility used for </span> 

Reading input from QUBO output and map edges to `UV`, `UC` and `WD` edges<br>













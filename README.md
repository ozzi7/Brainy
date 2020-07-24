# Brainy

A small asynchronous artificial neural network that can learn to recognize and memorize patterns and execute simple operations such as XOR. It is trained using two different neuro-evolution algorithms.

## Neural Network Architecture

![](media/graph.png)

The artificial neural network (ANN) consists of neurons (nodes) and axons (edges) that connect the neurons with each other. At large, the ANN is a fully connected directed graph such as the one depicted above. There are three dedicated input neurons (green) and three dedicated output neurons (blue). The input neurons receive the input activations while the calculated output is read from the output neurons. Apart from that, there is no difference among the neurons and any number of neurons may be used to solve more complex tasks. Self-loops (edges from nodes to themselves) decreased the performance in some tasks and are thus not allowed.

### Neuron

![](media/Action_potential.svg)[1]

The artificial neuron modelled in this project is loosely based on its biological counterpart. In particular, it follows the neuronal dynamics of the intergrate-and-fire model where incoming voltage spikes are summed up (integrated) over time until a certain threshold is reached within the neuron. Upon reaching the threshold, the neuron emits (fires) an electrical signal which is always equal to the full action potential and not dependent on the input signals. During a period of time after firing (the refractory period), the action potential within the neuron is being restored and the neuron is incapable of emitting additional signals in the process. Any incoming signals are ignored during this period. 

The artificial neuron has the following properties

-  **Activation threshold: 0.2mV**
-  **Refractory period: 10ms**
-  **Potential floor: -0.2mV**
-  **Starting potential: 0.0mV**

The activation threshold is the voltage sum that needs to be reached for the neuron to fire and release its action potential. A neuron that fires releases the full action potential equal to 1. Because axons can both excite and inhibit the neuron (they can add negative and positive values to the voltage sum) the neuron has a potential floor that serves as a lower bound for the sum. Initially, the neuron starts in a neutral state of 0.0mV. The voltages defined above do not agree with signals measured in biological cells, they are instead chosen for numerical stability and to balance the number of generated spikes. 

### Axon
An axon connects two neurons with each other. When a signal travels on the axon it is multiplied by the axon's weight. In addition, the axon's length determines the time it takes to travel to the destination neuron. Both of these paramaters are trainable and determine the behaviour of the network.

-  **Length or activation delay (trainable parameter) : [0,1]**
-  **Weight or signal multiplicator (trainable parameter) : [-1,1]**

## Input/Output
The input to the network is an arbitrary length sequence of bits while the output is a sequence of integers. In every time step (100ms each), the next three input bits are fed into the three input neurons. For each 1 in the input sequence the corresponding input neuron is activated, while for each 0 nothing is done. The output neurons record for each timestep the number of times they were activated and hence the output is a sequence of integers.

For example, consider the following input

*110 001 001 111 000 011 000 111 011 100 101 110 111 101 ...*

In the first timestep, input neuron 0 and 1 are activated while input neuron 3 is not activated. In the next step, input neuron 3 is activated.
The output could for example be

*000 000 110 001 001 111 000 011 000 111 011 313 201 110 411 101 ...*

if the network was trained to output the sequence with 2 timesteps delay but failed to achieve 100% accuracy.

## Neuro-evolution

### Algorithm 1

The network can be trained with a neuroevolution algorithm similar to the one described [here](https://arxiv.org/pdf/1712.06567.pdf). 

In this case, there is a population of 20 individuals where each individual has its own set of randomly initialized parameters (the weights and lengths of axons). Each individual's fitness is measured by feeding it the same input and comparing the generated output to the groundtruth. The individuals are then sorted by their performance (accuracy) on this task. The top 5 fittest individual become the new parents while the other 15 are killed off. The 5 parents' parameters are randomly permutated using samples from a gaussian distribution (multiplied by some factor to regulate mutation speed) to create 19 new individuals. In addition, the best performing parameters are kept as they are and added again to the population to give a total of 20 individuals for the new generation. This process repeats until the required accuracy is achieved or the networks no longer improve. Each iteration of the algorithm is called a generation.

## Experiments
Note: For the purpose of minimizing variance in the experiments the network weights and axon lengths have been initialized with a fixed value of 0.01 and 0.1 respectively. The gaussian noise is multiplied by 0.01. 

### Identity
The goal of the identity experiment is to see whether a randomly generated binary input sequence can be accurately reproduced as output with a fixed number of delay. The input is fed continuously into the network while the output is generated. This forces the network to temporarily memorize and buffer the input, while also learning to synchronize the output into the 100ms timesteps. If not otherwise noted, the experiments are repeated 10 times and then averaged, and the task was considered solved when the best performing individual achieved a >= 99% accuracy.

#### Result for 0 timestep delay
| # neurons      | # generations | # spikes | 
| ----------- | ----------- | ----------- |
|    1   | 48       | ? |
|    2   | 73        | ? |
|    4   | 55        | ?|
|    8   | 69 | ? |
|    16   | 96 | ? |

#### Result for 1 timestep delay
| # neurons      | # generations |
| ----------- | ----------- |
|    1   |   ∞     |

Note the slow convergence in cases where the number of neurons are barely enough to solve the task. 

### Memorization
**TODO**

### XOR Operator
**TODO**

## Curriculum learning
**TODO**

## References
<a id="1">[1]</a> 
https://commons.wikimedia.org/wiki/File:Action_potential.svg

<a id="2">[2]</a> 
Such, Felipe and Madhavan, Vashisht and Conti, Edoardo and Lehman, Joel and Stanley, Kenneth and Clune, Jeff (2017). 
Deep Neuroevolution: Genetic Algorithms Are a Competitive Alternative for Training Deep Neural Networks for Reinforcement Learning.

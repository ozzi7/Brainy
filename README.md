# Brainy

A small asynchronous artificial neural network that can learn to recognize and memorize patterns and execute simple operations such as XOR. It is trained using two different neuro-evolution algorithms.

## Neural Network Architecture

![](media/graph.png)

The artificial neural network (ANN) consists of neurons (nodes) and axons (edges) that connect the neurons with each other. In essence, the ANN is a fully connected directed graph such as the one depicted above. There are three dedicated input neurons (green) and three dedicated output neurons (blue). The input neurons receive the input activations while the calculated output is read from the output neurons. Apart from that, there is no difference among the neurons. Self-loops (edges from nodes to themselves) decreased the performance in some tasks and are thus not allowed.

### Neuron

![](media/Action_potential.svg)[1]

The artificial neuron modelled in this project is loosely based on its biological counterpart. In particular, it follows the neuronal dynamics of the intergrate-and-fire model where incoming voltage spikes are summed up (integrated) over time until a certain threshold is reached within the neuron. Upon reaching the threshold, the neuron emits (fires) an electrical signal which is always equal to the full action potential and not dependent on the input signals. During a period of time after firing (the refractory period), the action potential within the neuron is restored and the neuron is incapable of emitting additional signals. Any incoming signals are ignored during this period. 

The artificial neuron has the following properties

-  **Activation threshold: 0.2mV**
-  **Refractory period: 10ms**
-  **Potential floor: -0.2mV**
-  **Starting potential: 0.0mV**

The activation threshold is the voltage sum that needs to be reached for the neuron to fire and release its action potential. A neuron that fires releases the full action potential equal to 1. Because axons can both excite and inhibit the neuron (they can add negative and positive values to the voltage sum) the neuron has a potential floor that serves as a lower bound for the sum. Initially the neuron starts in a neutral state of 0.0mV. The voltages defined above do not agree with signals measured in biological cells, they are instead chosen for numerical stability and to balance the number of generated spikes. 

### Axon
An axon connects two neurons with each other. When a signal travels on the axon it is multiplied by the axon's weight. In addition, the axon's length determines the time it takes to travel to the destination neuron. Both of these paramaters are trainable.

The artificial axon has the following variables

-  **Length or activation (trainable parameter) : [0,1]**
-  **Weight or signal multiplicator (trainable parameter) : [-1,1]**

### Input/Output
The input is an arbitrary length sequence of bits while the output is a sequence of integers. In every time step the next three input bits are fed into the three input neurons (the neurons are activated). For every output neurons the number of activations are counted in the same timestep and recorded.

For example, consider the following input
*1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1*
## Neuro-evolution

## Experiments
For the purpose of minimizing variance in the experiment results the network weights and axon lengths have been initialized with a fixed value of 0.01 and 0.1 respectively.

### Identity
The goal of the identity experiment is to see whether a randomly generated binary input sequence can be accurately reproduced as output with a fixed number of delay. The input is fed continuously inot the network while the output is generated. This forces the network to temporarily memorize and buffer the input, while also learning to synchronize the output into the 100ms timesteps. 

#### Delay 0
| # neurons      | # generations |
| ----------- | ----------- |
|    1   | 51       |
|    2   | 60        |
|    4   | 55        |


#### Delay 1
| # neurons      | # generations |
| ----------- | ----------- |
|    1   |   ∞     |



### Memorization

### XOR Operator

## Curriculum learning

## References
<a id="1">[1]</a> 
https://commons.wikimedia.org/wiki/File:Action_potential.svg

<a id="2">[1]</a> 
Such, Felipe and Madhavan, Vashisht and Conti, Edoardo and Lehman, Joel and Stanley, Kenneth and Clune, Jeff (2017). 
Deep Neuroevolution: Genetic Algorithms Are a Competitive Alternative for Training Deep Neural Networks for Reinforcement Learning.
